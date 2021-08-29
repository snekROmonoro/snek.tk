#pragma once
#include "../util/vfunc/vfunc.hpp"
#include "basehandle.hpp"

class Entity;

class IClientEntityList {
public:
	template< typename T = Entity* >
	__forceinline T GetClientEntity( int index ) {
		return util::get_virtual_function< T( __thiscall* )( decltype( this ) , int ) >( this , 3 )( this , index );
	}

	template< typename T = Entity* >
	__forceinline T GetClientEntityFromHandle( EHANDLE handle ) {
		return util::get_virtual_function< T( __thiscall* )( decltype( this ) , EHANDLE ) >( this , 4 )( this , handle );
	}

	template< typename T = Entity* >
	__forceinline T GetClientEntityFromHandle( unsigned long handle ) {
		return util::get_virtual_function< T( __thiscall* )( decltype( this ) , unsigned long ) >( this , 4 )( this , handle );
	}

	__forceinline int GetHighestEntityIndex( ) {
		return util::get_virtual_function< int( __thiscall* )( decltype( this ) ) >( this , 6 )( this );
	}
};
