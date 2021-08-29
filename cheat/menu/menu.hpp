#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "../renderer/color/color.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "../imgui/impl/imgui_impl_dx9.h"

namespace menu {
	inline bool m_opened = false;
	inline ImGuiStyle  m_style , m_original_style;
	inline ImFont* m_title_font;
	inline ImFont* m_big_font;
	inline ImFont* m_tabs_font;
	inline ImFont* m_font;

	bool init( IDirect3DDevice9* pDevice , HWND hWindow );
	LRESULT CALLBACK wnd_proc( HWND hWnd , UINT message , WPARAM wParam , LPARAM lParam );
	void set_style( bool copy_original = false );
	void on_end_scene( );

	void on_lost_device( );
	void on_reset_device( );

	inline color get_main_color( ) {
		return color( 160 , 160 , 160 );
	}
}
