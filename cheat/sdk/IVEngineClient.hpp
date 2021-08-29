#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include "../util/vfunc/vfunc.hpp"
#include "ClientClass.hpp"
#include "../math/Vector.hpp"
#include "matrix.hpp"

#define NET_FRAMES_MASK 63

#define FLOW_OUTGOING 0
#define FLOW_INCOMING 1
#define MAX_FLOWS 2

class INetMessage;

class bf_read {
public:
	const char* m_pDebugName;
	bool m_bOverflow;
	int m_nDataBits;
	unsigned int m_nDataBytes;
	unsigned int m_nInBufWord;
	int m_nBitsAvail;
	const unsigned int* m_pDataIn;
	const unsigned int* m_pBufferEnd;
	const unsigned int* m_pData;

	bf_read( ) = default;

	bf_read( const void* pData , int nBytes , int nBits = -1 ) {
		StartReading( pData , nBytes , 0 , nBits );
	}

	void StartReading( const void* pData , int nBytes , int iStartBit , int nBits ) {
		// Make sure it's dword aligned and padded.
		m_pData = ( uint32_t* ) pData;
		m_pDataIn = m_pData;
		m_nDataBytes = nBytes;

		if ( nBits == -1 ) {
			m_nDataBits = nBytes << 3;
		}
		else {
			m_nDataBits = nBits;
		}
		m_bOverflow = false;
		m_pBufferEnd = reinterpret_cast< uint32_t const* >( reinterpret_cast< uint8_t const* >( m_pData ) + nBytes );
		if ( m_pData )
			Seek( iStartBit );
	}

	bool Seek( int nPosition ) {
		bool bSucc = true;
		if ( nPosition < 0 || nPosition > m_nDataBits ) {
			m_bOverflow = true;
			bSucc = false;
			nPosition = m_nDataBits;
		}
		int nHead = m_nDataBytes & 3; // non-multiple-of-4 bytes at head of buffer. We put the "round off"
									  // at the head to make reading and detecting the end efficient.

		int nByteOfs = nPosition / 8;
		if ( ( m_nDataBytes < 4 ) || ( nHead && ( nByteOfs < nHead ) ) ) {
			// partial first dword
			uint8_t const* pPartial = ( uint8_t const* ) m_pData;
			if ( m_pData ) {
				m_nInBufWord = *( pPartial++ );
				if ( nHead > 1 )
					m_nInBufWord |= ( *pPartial++ ) << 8;
				if ( nHead > 2 )
					m_nInBufWord |= ( *pPartial++ ) << 16;
			}
			m_pDataIn = ( uint32_t const* ) pPartial;
			m_nInBufWord >>= ( nPosition & 31 );
			m_nBitsAvail = ( nHead << 3 ) - ( nPosition & 31 );
		}
		else {
			int nAdjPosition = nPosition - ( nHead << 3 );
			m_pDataIn = reinterpret_cast< uint32_t const* >(
				reinterpret_cast< uint8_t const* >( m_pData ) + ( ( nAdjPosition / 32 ) << 2 ) + nHead );
			if ( m_pData ) {
				m_nBitsAvail = 32;
				GrabNextDWord( );
			}
			else {
				m_nInBufWord = 0;
				m_nBitsAvail = 1;
			}
			m_nInBufWord >>= ( nAdjPosition & 31 );
			m_nBitsAvail = min( m_nBitsAvail , 32 - ( nAdjPosition & 31 ) ); // in case grabnextdword overflowed
		}
		return bSucc;
	}

	FORCEINLINE void GrabNextDWord( bool bOverFlowImmediately = false ) {
		if ( m_pDataIn == m_pBufferEnd ) {
			m_nBitsAvail = 1; // so that next read will run out of words
			m_nInBufWord = 0;
			m_pDataIn++; // so seek count increments like old
			if ( bOverFlowImmediately )
				m_bOverflow = true;
		}
		else if ( m_pDataIn > m_pBufferEnd ) {
			m_bOverflow = true;
			m_nInBufWord = 0;
		}
		else {
			m_nInBufWord = DWORD( *( m_pDataIn++ ) );
		}
	}
};

class bf_write {
public:
	unsigned char* m_pData;
	int m_nDataBytes;
	int m_nDataBits;
	int m_iCurBit;
	bool m_bOverflow;
	bool m_bAssertOnOverflow;
	const char* m_pDebugName;

	void StartWriting( void* pData , int nBytes , int iStartBit = 0 , int nBits = -1 ) {
		// Make sure it's dword aligned and padded.
		// The writing code will overrun the end of the buffer if it isn't dword aligned, so truncate to force alignment
		nBytes &= ~3;

		m_pData = ( unsigned char* ) pData;
		m_nDataBytes = nBytes;

		if ( nBits == -1 ) {
			m_nDataBits = nBytes << 3;
		}
		else {
			m_nDataBits = nBits;
		}

		m_iCurBit = iStartBit;
		m_bOverflow = false;
	}

	bf_write( ) {
		m_pData = NULL;
		m_nDataBytes = 0;
		m_nDataBits = -1; // set to -1 so we generate overflow on any operation
		m_iCurBit = 0;
		m_bOverflow = false;
		m_bAssertOnOverflow = true;
		m_pDebugName = NULL;
	}

	// nMaxBits can be used as the number of bits in the buffer.
	// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	bf_write( void* pData , int nBytes , int nBits = -1 ) {
		m_bAssertOnOverflow = true;
		m_pDebugName = NULL;
		StartWriting( pData , nBytes , 0 , nBits );
	}

	bf_write( const char* pDebugName , void* pData , int nBytes , int nBits = -1 ) {
		m_bAssertOnOverflow = true;
		m_pDebugName = pDebugName;
		StartWriting( pData , nBytes , 0 , nBits );
	}
};

enum CLC_Messages : int
{
	clc_ClientInfo = 8 ,				// client info (table CRC etc)    
	clc_Move = 9 ,				// [CUserCmd]     
	clc_VoiceData = 10 ,				// Voicestream data from a client    
	clc_BaselineAck = 11 ,				// client acknowledges a new baseline seqnr   
	clc_ListenEvents = 12 ,				// client acknowledges a new baseline seqnr   
	clc_RespondCvarValue = 13 ,				// client is responding to a svc_GetCvarValue message.  
	clc_FileCRCCheck = 14 ,				// client is sending a file's CRC to the server to be verified. 
	clc_LoadingProgress = 15 ,				// client loading progress  
	clc_SplitPlayerConnect = 16 ,
	clc_ClientMessage = 17 ,
	clc_CmdKeyValues = 18 ,
	clc_HltvReplay = 20
};

class CLC_Move {
private:
	char __PAD0 [ 0x8 ]; // 0x0 two vtables
public:
	int backup_commands;
	int new_commands;
	std::string* data;
	bf_write* data_out;
	bf_read* data_in;
};                       // size: 0x50

class INetMessage {
public:
	virtual	~INetMessage( ) = default;

	virtual void SetNetChannel( void* netchan ) = 0;
	virtual void SetReliable( bool state ) = 0;
	virtual bool Process( ) = 0;
	virtual	bool ReadFromBuffer( void* buffer ) = 0;
	virtual	bool WriteToBuffer( void* buffer ) = 0;
	virtual bool IsReliable( ) const = 0;
	virtual int	GetType( ) const = 0;
	virtual int	GetGroup( ) const = 0;
	virtual const char* GetName( ) const = 0;
	virtual void* GetNetChannel( ) const = 0;
	virtual const char* ToString( ) const = 0;
};

template < typename T >
class CNetMessagePB : public INetMessage , public T {};
using CCLCMsg_Move_t = CNetMessagePB< CLC_Move >;

class INetChannelInfo
{
public:
	enum
	{
		GENERIC = 0 ,	// must be first and is default group
		LOCALPLAYER ,	// bytes for local player entity update
		OTHERPLAYERS ,	// bytes for other players update
		ENTITIES ,		// all other entity bytes
		SOUNDS ,			// game sounds
		EVENTS ,			// event messages
		TEMPENTS ,		// temp entities
		USERMESSAGES ,	// user messages
		ENTMESSAGES ,	// entity messages
		VOICE ,			// voice data
		STRINGTABLE ,	// a stringtable update
		MOVE ,			// client move cmds
		STRINGCMD ,		// string command
		SIGNON ,			// various signondata
		TOTAL ,			// must be last and is not a real group
	};

	virtual const char* GetName( void ) const = 0;	// get channel name
	virtual const char* GetAddress( void ) const = 0; // get channel IP address as string
	virtual float		GetTime( void ) const = 0;	// current net time
	virtual float		GetTimeConnected( void ) const = 0;	// get connection time in seconds
	virtual int			GetBufferSize( void ) const = 0;	// netchannel packet history size
	virtual int			GetDataRate( void ) const = 0; // send data rate in byte/sec

	virtual bool		IsLoopback( void ) const = 0;	// true if loopback channel
	virtual bool		IsTimingOut( void ) const = 0;	// true if timing out
	virtual bool		IsPlayback( void ) const = 0;	// true if demo playback

	virtual float		GetLatency( int flow ) const = 0;	 // current latency (RTT), more accurate but jittering
	virtual float		GetAvgLatency( int flow ) const = 0; // average packet latency in seconds
	virtual float		GetAvgLoss( int flow ) const = 0;	 // avg packet loss[0..1]
	virtual float		GetAvgChoke( int flow ) const = 0;	 // avg packet choke[0..1]
	virtual float		GetAvgData( int flow ) const = 0;	 // data flow in bytes/sec
	virtual float		GetAvgPackets( int flow ) const = 0; // avg packets/sec
	virtual int			GetTotalData( int flow ) const = 0;	 // total flow in/out in bytes
	virtual int			GetTotalPackets( int flow ) const = 0;
	virtual int			GetSequenceNr( int flow ) const = 0;	// last send seq number
	virtual bool		IsValidPacket( int flow , int frame_number ) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		GetPacketTime( int flow , int frame_number ) const = 0; // time when packet was send
	virtual int			GetPacketBytes( int flow , int frame_number , int group ) const = 0; // group size of this packet
	virtual bool		GetStreamProgress( int flow , int* received , int* total ) const = 0;  // TCP progress if transmitting
	virtual float		GetTimeSinceLastReceived( void ) const = 0;	// get time since last recieved packet in seconds
	virtual	float		GetCommandInterpolationAmount( int flow , int frame_number ) const = 0;
	virtual void		GetPacketResponseLatency( int flow , int frame_number , int* pnLatencyMsecs , int* pnChoke ) const = 0;
	virtual void		GetRemoteFramerate( float* pflFrameTime , float* pflFrameTimeStdDeviation ) const = 0;

	virtual float		GetTimeoutSeconds( ) const = 0;
};

class INetChannel : public INetChannelInfo {
public:
	bool SendNetMsg( INetMessage* msg , bool rel = false , bool voice = false ) {
		return util::get_virtual_function< bool( __thiscall* )( void* , void* , bool , bool ) >( this , 40 )( this , msg , rel , voice );
	}

	PAD( 0x14 );
	bool m_bProcessingMessages;		// 0x0014
	bool m_bShouldDelete;			// 0x0015
	PAD( 0x2 );
	int m_nOutSequenceNr;			// 0x0018 last send outgoing sequence number
	int m_nInSequenceNr;			// 0x001C last received incoming sequnec number
	int m_nOutSequenceNrAck;		// 0x0020 last received acknowledge outgoing sequnce number
	int m_nOutReliableState;		// 0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
	int m_nInReliableState;			// 0x0028 state of incoming reliable data
	int m_nChokedPackets;			// 0x002C number of choked packets
	PAD( 0x414 );					// 0x0030
};

struct player_info_t {
	uint64_t      m_data_map;
	union {
		int64_t   m_xuid;
		struct {
			int   m_xuid_low;
			int   m_xuid_high;
		};
	};
	char          m_name [ 128 ];
	int           m_user_id;
	char          m_guid [ 33 ];
	uint32_t      m_friends_id;
	char          m_friends_name [ 128 ];
	bool          m_fake_player;
	bool          m_is_hltv;
	uint32_t      m_custom_files [ 4 ];
	uint8_t       m_files_downloaded;
};

class CEventInfo {
public:
	enum {
		EVENT_INDEX_BITS = 8 ,
		EVENT_DATA_LEN_BITS = 11 ,
		MAX_EVENT_DATA = 192 ,  // ( 1<<8 bits == 256, but only using 192 below )
	};

	// 0 implies not in use
	short					m_class_id;
	float					m_fire_delay;
	const void* m_send_table;
	const ClientClass* m_client_class;//void*
	int						m_bits;
	uint8_t* m_data;
	int						m_flags;
	PAD( 0x1C );
	CEventInfo* m_next;
};

class CClockDriftMgr {
public:
	enum
	{
		// This controls how much it smoothes out the samples from the server.
		NUM_CLOCKDRIFT_SAMPLES = 16
	};

	// This holds how many ticks the client is ahead each time we get a server tick.
	// We average these together to get our estimate of how far ahead we are.
	float m_ClockOffsets [ NUM_CLOCKDRIFT_SAMPLES ];
	int m_iCurClockOffset;

	int m_nServerTick;		// Last-received tick from the server.
	int	m_nClientTick;		// The client's own tick counter (specifically, for interpolation during rendering).
							// The server may be on a slightly different tick and the client will drift towards it.
};

class CClientState {
public:
	PAD( 156 );
	INetChannel* m_NetChannel; // Our sequenced channel to the remote server.
	int	m_nChallengeNr;	// connection challenge number
	PAD( 4 );
	double m_flConnectTime;	// If gap of connect_time to net_time > 3000, then resend connect packet
	int m_nRetryNumber;	// number of retry connection attempts
	PAD( 84 );
	int m_nSignonState; // see SIGNONSTATE_* definitions
	PAD( 4 );
	double m_flNextCmdTime; // When can we send the next command packet?
	int m_nServerCount;	// server identification for prespawns, must match the svs.spawncount which
						// is incremented on server spawning.  This supercedes svs.spawn_issued, in that
						// we can now spend a fair amount of time sitting connected to the server
						// but downloading models, sounds, etc.  So much time that it is possible that the
						// server might change levels again and, if so, we need to know that.
	int m_nCurrentSequence;	// this is the sequence number of the current incoming packet
	PAD( 8 );
	CClockDriftMgr m_ClockDriftMgr;
	int m_nDeltaTick; // last valid received snapshot (server) tick
	PAD( 4 );
	int m_nViewEntity; // player point of view override
	int m_nPlayerSlot; // own player entity index-1. skips world. Add 1 to get cl_entitites index;
	bool m_bPaused; // send over by server
	PAD( 3 );
	char m_szLevelName [ 260 ];
	char m_szLevelNameShort [ 40 ];
	PAD( 212 );
	int m_nMaxClients;
	PAD( 18836 );
	int oldtickcount; // previous tick
	float m_tickRemainder; // client copy of tick remainder
	float m_frameTime; // dt of the current frame
	int lastoutgoingcommand; // Sequence number of last outgoing command
	int chokedcommands;	// number of choked commands
	int last_command_ack; // last command sequence number acknowledged by server
	int last_server_tick; // same update pattern as last_command_ack, but with server ticks
	int command_ack; // current command sequence acknowledged by server
	int m_nSoundSequence; // current processed reliable sound sequence number
	int m_nLastProgressPercent;	// last progress percent sent to server
	bool ishltv; // true if HLTV server/demo
	PAD( 75 );
	vec3_t viewangles;
	PAD( 204 );
	CEventInfo* events;
};

class CGlobalState {
public:
	uintptr_t* m_manager;
	CClientState* m_client_state;
};

class ISpatialQuery {
public:
	int LeafCount( ) {
		return util::get_virtual_function< int( __thiscall* )( void* ) >( this , 0 )( this );
	}

	int ListLeavesInBox( const vec3_t& mins , const vec3_t& maxs , unsigned short* pList , int listMax ) {
		return util::get_virtual_function< int( __thiscall* )( void* , const vec3_t& , const vec3_t& , unsigned short* , int ) >( this , 6 )( this , mins , maxs , pList , listMax );
	}
};

class IVEngineClient {
public:
	enum indices : size_t {
		GETSCREENSIZE = 5 ,
		GETPLAYERINFO = 8 ,
		GETPLAYERFORUSERID = 9 ,
		GETLOCALPLAYER = 12 ,
		GETLASTTIMESTAMP = 14 ,
		GETVIEWANGLES = 18 ,
		SETVIEWANGLES = 19 ,
		//GETMAXCLIENTS          = 20,
		ISINGAME = 26 ,
		ISCONNECTED = 27 ,
		WORLDTOSCREENMATRIX = 37 ,
		GETBSPTREEQUERY = 43 ,
		FIREEVENTS = 59 ,
		GETNETCHANNELINFO = 78 ,
		ISPAUSED = 90 ,
		//ISTAKINGSCREENSHOT     = 92,
		ISHLTV = 93 ,
		//SETOCCLUSIONPARAMETERS = 96,
		GETSCREENASPECTRATIO = 101 ,
		EXECUTECLIENTCMD = 108 ,
		//CLIENTCMD_UNRESTRICTED = 114 ,
		ISVOICERECORDING = 224 ,
	};

public:
	void GetScreenSize( int& width , int& height ) {
		return util::get_virtual_function< void( __thiscall* )( void* , int& , int& ) >( this , 5 )( this , width , height );
	}

	bool GetPlayerInfo( int index , player_info_t* info ) {
		return util::get_virtual_function< bool( __thiscall* )( void* , int , player_info_t* ) >( this , 8 )( this , index , info );
	}

	int GetPlayerForUserID( int uid ) {
		return util::get_virtual_function< int( __thiscall* )( void* , int ) >( this , 9 )( this , uid );
	}

	int GetLocalPlayer( ) {
		return util::get_virtual_function< int( __thiscall* )( void* ) >( this , 12 )( this );
	}

	float GetLastTimestamp( ) {
		return util::get_virtual_function< float( __thiscall* )( void* ) >( this , 14 )( this );
	}

	void GetViewAngles( vec3_t& angle ) {
		return util::get_virtual_function< void( __thiscall* )( void* , vec3_t& ) >( this , 18 )( this , angle );
	}

	void SetViewAngles( vec3_t& angle ) {
		return util::get_virtual_function< void( __thiscall* )( void* , vec3_t& ) >( this , 19 )( this , angle );
	}

	bool IsInGame( ) {
		return util::get_virtual_function< bool( __thiscall* )( void* ) >( this , 26 )( this );
	}

	bool IsConnected( ) {
		return util::get_virtual_function< bool( __thiscall* )( void* ) >( this , 27 )( this );
	}

	const VMatrix& WorldToScreenMatrix( ) {
		return util::get_virtual_function< const VMatrix& ( __thiscall* )( void* ) >( this , 37 )( this );
	}

	ISpatialQuery* GetBspTreeQuery( ) {
		return util::get_virtual_function< ISpatialQuery* ( __thiscall* )( void* ) >( this , 43 )( this );
	}

	void FireEvents( ) {
		return util::get_virtual_function< void( __thiscall* )( void* ) >( this , 59 )( this );
	}

	INetChannel* GetNetChannelInfo( ) {
		return util::get_virtual_function< INetChannel* ( __thiscall* )( void* ) >( this , 78 )( this );
	}

	bool IsPaused( ) {
		return util::get_virtual_function< bool( __thiscall* )( void* ) >( this , 90 )( this );
	}

	float GetScreenAspectRatio( int viewportWidth , int viewportHeight ) {
		return util::get_virtual_function< float( __thiscall* )( void* , int , int )>( this , 101 )( this , viewportWidth , viewportHeight );
	}

	void ExecuteClientCmd( const char* cmd ) {
		return util::get_virtual_function< void( __thiscall* )( void* , const char* )>( this , 108 )( this , cmd );
	}

	bool IsVoiceRecording( ) {
		return util::get_virtual_function< bool( __thiscall* )( void* ) >( this , 224 )( this );
	}
};
