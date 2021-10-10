#pragma once
#include "../util/vfunc/vfunc.hpp"

class IMemAlloc {
public:
	void* Alloc( size_t size ) {
		return util::get_virtual_function< void* ( __thiscall* )( void* , size_t ) >( this , 1 )( this , size );
	}

	void* Realloc( void* region , size_t size ) {
		return util::get_virtual_function< void* ( __thiscall* )( void* , void* , size_t ) >( this , 3 )( this , region , size );
	}

	void Free( void* region ) {
		return util::get_virtual_function< void( __thiscall* )( void* , void* ) >( this , 5 )( this , region );
	}
};

extern IMemAlloc* g_pMemAlloc;