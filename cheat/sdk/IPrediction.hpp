#pragma once
#include "../util/vfunc/vfunc.hpp"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/iprediction.h
// more likely: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/client/prediction.h

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
	PAD( 0x4 );							// 0x0014
	int m_nServerCommandsAcknowledged;  // 0x0018
	int m_bPreviousAckHadErrors;		// 0x001C
	char pad_0x0020 [ 0x4 ];			// 0x0020
	int m_nIncomingPacketNumber;		// 0x0024
	float m_flIdealPitch;
	int m_nLastCommandAcknowledged;
	bool m_bPreviousAckErrorTriggersFullLatchReset;

	void Update(
		int startframe ,		// World update ( un-modded ) most recently received
		bool validframe ,		// Is frame data valid
		int incoming_acknowledged , // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command	// Last command (most recent) sent to server (un-modded)
	)
	{
		using o_fn = void( __thiscall* )( void* , int , bool , int , int );
		util::get_virtual_function< o_fn >( this , 3 )( this , startframe , validframe , incoming_acknowledged , outgoing_command );
	}
};
