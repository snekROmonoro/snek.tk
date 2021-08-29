#pragma once
#include "../util/vfunc/vfunc.hpp"
#include "../math/Vector.hpp"
#include "checksum_crc.hpp"

// https://github.com/perilouswithadollarsign/cstrike15_src/blob/master/game/shared/usercmd.h

class CUserCmd
{
public:
	CUserCmd( )
	{
		Reset( );
	}

	virtual ~CUserCmd( ) { };

	void Reset( ) {
		command_number = 0;
		tick_count = 0;
		viewangles.init( );
		aimdirection.init( );
		forwardmove = 0.0f;
		sidemove = 0.0f;
		upmove = 0.0f;
		buttons = 0;
		impulse = 0;
		weaponselect = 0;
		weaponsubtype = 0;
		random_seed = 0;

		mousedx = 0;
		mousedy = 0;

		hasbeenpredicted = false;

		// TrackIR
		headangles.init( );
		headoffset.init( );
		// TrackIR
	}

	CUserCmd& operator =( const CUserCmd& src )
	{
		if ( this == &src )
			return *this;

		command_number = src.command_number;
		tick_count = src.tick_count;
		viewangles = src.viewangles;
		aimdirection = src.aimdirection;
		forwardmove = src.forwardmove;
		sidemove = src.sidemove;
		upmove = src.upmove;
		buttons = src.buttons;
		impulse = src.impulse;
		weaponselect = src.weaponselect;
		weaponsubtype = src.weaponsubtype;
		random_seed = src.random_seed;

		mousedx = src.mousedx;
		mousedy = src.mousedy;

		hasbeenpredicted = src.hasbeenpredicted;

		// TrackIR
		headangles = src.headangles;
		headoffset = src.headoffset;
		// TrackIR

		return *this;
	}

	CUserCmd( const CUserCmd& src )
	{
		*this = src;
	}

	CRC32_t GetChecksum( void ) const
	{
		CRC32_t crc;

		CRC32_Init( &crc );
		CRC32_ProcessBuffer( &crc , &command_number , sizeof( command_number ) );
		CRC32_ProcessBuffer( &crc , &tick_count , sizeof( tick_count ) );
		CRC32_ProcessBuffer( &crc , &viewangles , sizeof( viewangles ) );
		CRC32_ProcessBuffer( &crc , &aimdirection , sizeof( aimdirection ) );
		CRC32_ProcessBuffer( &crc , &forwardmove , sizeof( forwardmove ) );
		CRC32_ProcessBuffer( &crc , &sidemove , sizeof( sidemove ) );
		CRC32_ProcessBuffer( &crc , &upmove , sizeof( upmove ) );
		CRC32_ProcessBuffer( &crc , &buttons , sizeof( buttons ) );
		CRC32_ProcessBuffer( &crc , &impulse , sizeof( impulse ) );
		CRC32_ProcessBuffer( &crc , &weaponselect , sizeof( weaponselect ) );
		CRC32_ProcessBuffer( &crc , &weaponsubtype , sizeof( weaponsubtype ) );
		CRC32_ProcessBuffer( &crc , &random_seed , sizeof( random_seed ) );
		CRC32_ProcessBuffer( &crc , &mousedx , sizeof( mousedx ) );
		CRC32_ProcessBuffer( &crc , &mousedy , sizeof( mousedy ) );

		CRC32_Final( &crc );

		return crc;
	}

	int     command_number;     // 0x04 For matching server and client commands for debugging
	int     tick_count;         // 0x08 the tick the client created this command
	vec3_t  viewangles;         // 0x0C Player instantaneous view angles.
	vec3_t  aimdirection;       // 0x18 For pointing devices.
	float   forwardmove;        // 0x24 forward velocity.
	float   sidemove;           // 0x28 sideways velocity.
	float   upmove;             // 0x2C upward velocity.
	int     buttons;            // 0x30 Attack button states
	char    impulse;            // 0x34 Impulse command issued.
	int     weaponselect;       // 0x38 Current weapon id
	int     weaponsubtype;      // 0x3C
	int     random_seed;        // 0x40 For shared random functions
	short   mousedx;            // 0x44 mouse accum in x from create move
	short   mousedy;            // 0x46 mouse accum in y from create move
	bool    hasbeenpredicted;   // 0x48 Client only, tracks whether we've predicted this command at least once
	vec3_t  headangles;
	vec3_t  headoffset;
};

