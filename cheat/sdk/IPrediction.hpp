#pragma once
#include "../util/vfunc/vfunc.hpp"
#include "CMoveData.hpp"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/iprediction.h
// more likely: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/client/prediction.h

class Entity;
class CUserCmd;

class IMoveHelper {
public:
	void SetHost( Entity* host ) {
		return util::get_virtual_function< void( __thiscall* )( void* , Entity* ) >( this , 1 )( this , host );
	}

	void ProcessImpacts( ) {
		return util::get_virtual_function< void( __thiscall* )( void* ) >( this , 4 )( this );
	}
};

class CPrediction
{
public:
	PAD( 0x4 );
	int32_t m_hLastGround;				// 0x0004
	bool    m_bInPrediction;			// 0x0008
	bool    m_bFirstTimePredicted;		// 0x0009
	bool    m_bEnginePaused;			// 0x000A
	bool    m_bOldCLPredictValue;		// 0x000B
	int32_t m_nPreviousStartFrame;		// 0x000C
	int32_t m_nCommandsPredicted;		// 0x0010
	PAD( 0x38 );						// 0x0014
	float   m_flBackupRealtime;			// 0x004C
	PAD( 0xC );							// 0x0050
	float   m_flBackupCurtime;			// 0x005C
	PAD( 0xC );							// 0x0060
	float   m_flBackupInterval;			// 0x006C

	void Update(
		int startframe ,		// World update ( un-modded ) most recently received
		bool validframe ,		// Is frame data valid
		int incoming_acknowledged , // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command	// Last command (most recent) sent to server (un-modded)
	)
	{
		util::get_virtual_function< void( __thiscall* )( void* , int , bool , int , int ) >( this , 3 )( this , startframe , validframe , incoming_acknowledged , outgoing_command );
	}

	void GetLocalViewAngles( vec3_t& ang ) {
		return util::get_virtual_function< void( __thiscall* )( void* , vec3_t& ) >( this , 12 )( this , ang );
	}

	void SetLocalViewAngles( vec3_t& ang ) {
		return util::get_virtual_function< void( __thiscall* )( void* , vec3_t& ) >( this , 13 )( this , ang );
	}

	void SetupMove( Entity* player , CUserCmd* cmd , IMoveHelper* helper , CMoveData* data ) {
		return util::get_virtual_function< void( __thiscall* )( void* , Entity* , CUserCmd* , IMoveHelper* , CMoveData* ) >( this , 20 )( this , player , cmd , helper , data );
	}

	void FinishMove( Entity* player , CUserCmd* cmd , CMoveData* data ) {
		return util::get_virtual_function< void( __thiscall* )( void* , Entity* , CUserCmd* , CMoveData* ) >( this , 21 )( this , player , cmd , data );
	}
};
