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
	extern util::hook::vmt_hook m_vmt_client;
	extern util::hook::vmt_hook m_vmt_clientmode;

	long __stdcall WndProc( HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam );
	extern WNDPROC oWndProc;

	long __fastcall EndScene( REG , IDirect3DDevice9* device );
	long __fastcall Reset( REG , IDirect3DDevice9* device , D3DPRESENT_PARAMETERS* presentation_params );
	void __fastcall LockCursor( REG );
	void __fastcall LevelInitPreEntity( REG , const char* map );
	void __fastcall LevelInitPostEntity( REG );
	void __fastcall LevelShutdown( REG );
	void __fastcall FrameStageNotify( REG , int stage );
	bool  __fastcall CreateMove( REG , float time , CUserCmd* cmd );

	inline decltype( &EndScene ) oEndScene = nullptr;
	inline decltype( &Reset ) oReset = nullptr;
	inline decltype( &LockCursor ) oLockCursor = nullptr;
	inline decltype( &LevelInitPreEntity ) oLevelInitPreEntity = nullptr;
	inline decltype( &LevelInitPostEntity ) oLevelInitPostEntity = nullptr;
	inline decltype( &LevelShutdown ) oLevelShutdown = nullptr;
	inline decltype( &FrameStageNotify ) oFrameStageNotify = nullptr;
	inline decltype( &CreateMove ) oCreateMove = nullptr;
}
