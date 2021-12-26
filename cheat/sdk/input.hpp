#pragma once
#include "CUserCmd.hpp"

// https://github.com/perilouswithadollarsign/cstrike15_src/blob/master/game/client/input.h

class CVerifiedUserCmd
{
public:
	CUserCmd	m_cmd;
	CRC32_t		m_crc;
};

class IInputSystem {
public:
	void EnableInput( bool enable ) {
		return util::get_virtual_function< void( __thiscall* )( void* , bool ) >( this , 11 )( this , enable );
	}

	void GetCursorPosition( int* x , int* y ) {
		return util::get_virtual_function< void( __thiscall* )( void* , int* , int* ) >( this , 56 )( this , x , y );
	}
};

class CInput {
public:
	std::byte			pad0 [ 0xC ];			//0x0000
	bool				bTrackIRAvailable;		//0x000C
	bool				bMouseInitialized;		//0x000D 
	bool				bMouseActive;			//0x000E
	std::byte			pad1 [ 0xB2 ];			//0x000F
	bool				bCameraInThirdPerson;	//0x00C1
	std::byte			pad2 [ 0x2 ];			//0x00C2
	vec3_t				vecCameraOffset;		//0x00C4
	std::byte			pad3 [ 0x38 ];			//0x00D0
	CUserCmd*			m_pCommands;			//0x0108
	CVerifiedUserCmd*	m_pVerifiedCommands;	//0x010C

	CUserCmd* GetUserCmd( int slot , int sequence_number ) {
		return util::get_virtual_function< CUserCmd* ( __thiscall* )( void* , int , int ) >( this , 8 )( this , slot , sequence_number );
	}

	int CAM_IsThirdPerson( int slot = -1 ) {
		return util::get_virtual_function< int( __thiscall* )( void* , int ) >( this , 32 )( this , slot );
	}

	void CAM_ToThirdPerson( ) {
		return util::get_virtual_function< void( __thiscall* )( void* ) >( this , 35 )( this );
	}

	void CAM_ToFirstPerson( ) {
		return util::get_virtual_function< void( __thiscall* )( void* ) >( this , 36 )( this );
	}
};
