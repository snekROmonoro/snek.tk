#pragma once
#include "../includes.hpp"
#include "../util/minhook/mh_helper.h" // it was already included in the includes file but it's retarded ok?

#define REG void* ecx, void* edx
#define REG_OUT ecx, edx

namespace hooks {
	extern bool m_hooked;

	bool init( );
	bool release( );

	extern util::hook::vmt_hook m_vmt_d3d;
	extern util::hook::vmt_hook m_vmt_surface;

	long __stdcall wnd_proc( HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam );
	extern WNDPROC o_wndproc;

	long __fastcall endscene_hk( REG , IDirect3DDevice9* device );
	long __fastcall reset_hk( REG , IDirect3DDevice9* device , D3DPRESENT_PARAMETERS* presentation_params );
	void __fastcall lockcursor_hk( REG );

	inline decltype( &endscene_hk ) o_endscene = nullptr;
	inline decltype( &reset_hk ) o_reset = nullptr;
	inline decltype( &lockcursor_hk ) o_lockcursor = nullptr;
}
