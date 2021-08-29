#pragma once
#include "CMoveData.hpp"
#include "../util/vfunc/vfunc.hpp"

class CGameMovement {
public:
	void ProcessMovement( void* player , CMoveData* data ) {
		return util::get_virtual_function< void( __thiscall* )( void* , void* , CMoveData* ) >( this , 1 )( this , player , data );
	}

	void Reset( ) {
		return util::get_virtual_function< void( __thiscall* )( void* ) >( this , 2 )( this );
	}

	void StartTrackPredictionErrors( void* player ) {
		return util::get_virtual_function< void( __thiscall* )( void* , void* ) >( this , 3 )( this , player );
	}

	void FinishTrackPredictionErrors( void* player ) {
		return util::get_virtual_function< void( __thiscall* )( void* , void* ) >( this , 4 )( this , player );
	}

	vec3_t const& GetPlayerMins( bool ducked ) {
		return util::get_virtual_function< vec3_t const& ( __thiscall* )( void* , bool ) >( this , 6 )( this , ducked );
	}

	vec3_t const& GetPlayerMaxs( bool ducked ) {
		return util::get_virtual_function< vec3_t const& ( __thiscall* )( void* , bool ) >( this , 7 )( this , ducked );
	}

	vec3_t const& GetPlayerViewOffset( bool ducked ) {
		return util::get_virtual_function< vec3_t const& ( __thiscall* )( void* , bool ) >( this , 8 )( this , ducked );
	}
};
