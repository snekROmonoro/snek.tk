#include "console.hpp"
#include "../../includes.hpp"

bool util::console::initialize_console( const char* window_name )
{
	if ( !AllocConsole( ) )
		return false;

	const auto in = freopen_s( reinterpret_cast< FILE** >( stdin ) , "CONIN$" , "r" , stdin );

	if ( in != 0 )
		return false;

	const auto out = freopen_s( reinterpret_cast< FILE** >( stdout ) , "CONOUT$" , "w" , stdout );

	if ( out != 0 )
		return false;

	if ( !SetConsoleTitleA( window_name ) )
		return false;

	return true;
}

bool util::console::unload_console( void ) {
	const auto in = fclose( reinterpret_cast< FILE* >( stdin ) );

	if ( in != 0 )
		return false;

	const auto out = fclose( reinterpret_cast< FILE* >( stdout ) );

	if ( out != 0 )
		return false;

	if ( !FreeConsole( ) )
		return false;

	return true;
}

void util::console::set_print_color( int col , bool intense , bool background )
{
	uintptr_t k = 0;

	if ( background ) {
		switch ( col ) {
		case BLACK: {
			k = 0;
		} break;
		case RED: {
			k = BACKGROUND_RED;
		} break;
		case GREEN: {
			k = BACKGROUND_GREEN;
		} break;
		case BLUE: {
			k = BACKGROUND_BLUE;
		} break;
		case MAGENTA: {
			k = BACKGROUND_RED | BACKGROUND_BLUE;
		} break;
		case CYAN: {
			k = BACKGROUND_BLUE | BACKGROUND_GREEN;
		} break;
		case YELLOW: {
			k = BACKGROUND_RED | BACKGROUND_GREEN;
		} break;
		case WHITE: {
			k = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
		} break;
		}
	}
	else {
		switch ( col ) {
		case BLACK: {
			k = 0;
		} break;
		case RED: {
			k = FOREGROUND_RED;
		} break;
		case GREEN: {
			k = FOREGROUND_GREEN;
		} break;
		case BLUE: {
			k = FOREGROUND_BLUE;
		} break;
		case MAGENTA: {
			k = FOREGROUND_RED | FOREGROUND_BLUE;
		} break;
		case CYAN: {
			k = FOREGROUND_BLUE | FOREGROUND_GREEN;
		} break;
		case YELLOW: {
			k = FOREGROUND_RED | FOREGROUND_GREEN;
		} break;
		case WHITE: {
			k = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
		} break;
		}
	}

	if ( intense ) {
		if ( k != 0 ) {
			k |= background ? BACKGROUND_INTENSITY : FOREGROUND_INTENSITY;
		}
	}

	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute( hConsole , k );
}
