#pragma once
#include <stdio.h>

namespace util {
	namespace console {
		enum colors_t : int {
			BLACK = 0 ,
			RED ,
			GREEN ,
			BLUE ,
			MAGENTA ,
			CYAN ,
			YELLOW ,
			WHITE
		};

		bool initialize_console( const char* window_name );
		bool unload_console( void );
		void set_print_color( int col , bool intense = false , bool background = false );

		inline void reset_color( ) {
			set_print_color( util::console::WHITE );
		}

		enum prefix_t : int {
			NONE = 0 , // when this prefix is set, it prints the cheat prefix
			ERR ,
			SDK ,
			HOOK ,
			SIG ,
			WARN ,
			_max_prefix__ // IDK
		};

		inline int m_prefix = NONE;
		inline void set_prefix( int prefix ) {
			int _prefix = prefix;
			if ( prefix < NONE )
				_prefix = NONE;
			else if ( prefix > _max_prefix__ )
				_prefix = _max_prefix__;

			m_prefix = _prefix;
		}

		template< typename... Args >
		inline void print( const char* format , const Args&... args ) {
			switch ( m_prefix ) {
			case prefix_t::NONE: {
				set_print_color( util::console::CYAN );
				printf( "[snek.tk] " );
			} break;
			case prefix_t::ERR: {
				set_print_color( util::console::RED );
				printf( "[error] " );
			} break;
			case prefix_t::SDK: {
				set_print_color( util::console::MAGENTA );
				printf( "[sdk] " );
			} break;
			case prefix_t::HOOK: {
				set_print_color( util::console::BLUE );
				printf( "[hook] " );
			} break;
			case prefix_t::SIG: {
				set_print_color( util::console::GREEN );
				printf( "[sig] " );
			} break;
			case prefix_t::WARN: {
				set_print_color( util::console::YELLOW );
				printf( "[warn] " );
			} break;
			}

			set_print_color( util::console::WHITE );
			printf( format , args... );
		}
	}
}
