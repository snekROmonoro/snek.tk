#include "includes.hpp"
#include <stdexcept>

// it crashes so fuck it
void unload_cheat( void )
{
	util::console::unload_console( );

	if ( hooks::m_hooked ) {
		if ( !hooks::release( ) ) {
			MessageBoxA( NULL , "failed to release hooks" , "DEBUG" , MB_OK | MB_ICONERROR );
			std::abort( );
		}
	}

}

int _stdcall init( uintptr_t mod )
{
	while ( !GetModuleHandleA( "serverbrowser.dll" ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

	if ( !util::console::initialize_console( "debug | snek.tk" ) ) {
		MessageBoxA( NULL , "failed to initialize console" , "DEBUG" , MB_OK | MB_ICONERROR );
		std::abort( );
	}

	try {
		if ( !sdk::interfaces::init( ) )
			throw std::runtime_error( "failed to initialize interfaces" );

		if ( !patterns::init( ) )
			throw std::runtime_error( "failed to grab patterns" );

		if ( !sdk::netvar::init( ) )
			throw std::runtime_error( "failed to initialize netvar" );

		globals::csgo_window = FindWindowA( "Valve001" , nullptr );

		g_renderer.init( sdk::interfaces::d3d_device );
		input::init( globals::csgo_window );
		g_event_listener.init( );

		if ( !vars::init( ) )
			throw std::runtime_error( "failed to initialize vars" );

		if ( !menu::init( sdk::interfaces::d3d_device , globals::csgo_window ) )
			throw std::runtime_error( "failed to initialize menu" );

		if ( !hooks::init( ) )
			throw std::runtime_error( "failed to do hooks" );

		util::console::set_prefix( util::console::NONE );
		util::console::print( "loaded cheat\n" );
	}
	catch ( const std::runtime_error& error ) {
		MessageBoxA( NULL , error.what( ) , "DEBUG" , MB_OK | MB_ICONERROR );
		unload_cheat( );
		FreeLibraryAndExitThread( HMODULE( mod ) , 0 );
	}

	//while ( !GetAsyncKeyState( VK_END ) )
	while ( true )
		std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );

	unload_cheat( );
	FreeLibraryAndExitThread( HMODULE( mod ) , 0 );
}

int __stdcall DllMain( HINSTANCE inst , std::uint32_t reason , void* reserved )
{
	if ( reason == DLL_PROCESS_ATTACH ) {
		CreateThread( nullptr , 0 , LPTHREAD_START_ROUTINE( init ) , HMODULE( inst ) , 0 , nullptr );
	}

	return TRUE;
}

