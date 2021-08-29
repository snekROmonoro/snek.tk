#pragma once
#include <cstdint>
#include "../util/vfunc/vfunc.hpp"

enum VGuiPanel_t {
	PANEL_ROOT = 0 , // buy menu problems
	PANEL_GAMEUIDLL , // only called in main menu
	PANEL_CLIENTDLL , // only called ingame
	PANEL_TOOLS , // perfect
	PANEL_INGAMESCREENS ,
	PANEL_GAMEDLL ,
	PANEL_CLIENTDLL_TOOLS
};

class IEngineVGui {
public:
	virtual	~IEngineVGui( void ) {}
	virtual uint32_t GetPanel( VGuiPanel_t type ) = 0;
	virtual bool IsGameUIVisible( ) = 0;
};

class IPanel {
public:

	void SetMouseInputEnabled( unsigned int vguiPanel , bool state )
	{
		return util::get_virtual_function< void( __thiscall* )( void* , unsigned int , bool ) >( this , 32 )( this , vguiPanel , state );
	}

	bool IsMouseInputEnabled( unsigned int vguiPanel )
	{
		return util::get_virtual_function< bool( __thiscall* )( void* , unsigned int ) >( this , 33 )( this , vguiPanel );
	}

	const char* GetName( unsigned int vguiPanel )
	{
		return util::get_virtual_function< const char* ( __thiscall* )( void* , unsigned int ) >( this , 36 )( this , vguiPanel );
	}

	const char* _GetClassName( unsigned int vguiPanel )
	{
		return util::get_virtual_function< const char* ( __thiscall* )( void* , unsigned int ) >( this , 37 )( this , vguiPanel );
	}

};

class ISurface {
public:
	void UnlockCursor( ) {
		return util::get_virtual_function< void( __thiscall* )( void* ) >( this , 66 )( this );
	}

	void LockCursor( ) {
		return util::get_virtual_function< void( __thiscall* )( void* ) >( this , 67 )( this );
	}
};
