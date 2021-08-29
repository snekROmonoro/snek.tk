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
	void* vtable; // 0x00
	bool m_fTrackIRAvailable; // 0x04
	bool m_fMouseInitialized; // 0x05 Has the mouse been initialized?
	bool m_mouse_active; // 0x06 Is the mosue active?
	bool m_fJoystickAdvancedInit; // 0x07 Has the joystick advanced initialization been run?
	PAD( 0x2C ); // 0x08
	void* m_keys; // 0x34
	PAD( 116 ); // 0x38
	bool m_fCameraInterceptingMouse; // 0x9C Is the 3rd person camera using the mouse?
	bool m_fCameraInThirdPerson; // 0x9D Are we in 3rd person view?
	bool m_fCameraMovingWithMouse;	// 0x9E Should we move view along with mouse?
	vec3_t m_vecCameraOffset; // 0xA0 What is the current camera offset from the view origin?
	bool m_fCameraDistanceMove; // 0xAC Is the camera in distance moving mode?
	int m_nCameraOldX; // 0xB0
	int m_nCameraOldY; // 0xB4
	int m_nCameraX; // 0xB8
	int m_nCameraY; // 0xBC
	bool m_CameraIsOrthographic; // 0xC0
	vec3_t m_angPreviousViewAngles; // 0xC4
	vec3_t m_angPreviousViewAnglesTilt;	// 0xD0
	float m_flLastForwardMove; // 0xDC
	int m_nClearInputState; // 0xE0
	CUserCmd* m_pCommands; // 0xEC
	CVerifiedUserCmd* m_pVerifiedCommands; // 0xF0

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
