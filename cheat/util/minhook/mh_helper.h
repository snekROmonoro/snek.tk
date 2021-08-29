#pragma once
#include <Windows.h>
#include "minhook.h"
#include "../console/console.hpp"
#include "../vfunc/vfunc.hpp"

namespace util {
	namespace hook {
		template < typename T >
		inline bool hook_address( void* target , void* func , T** original ) {
			if ( !target ) {
				util::console::set_prefix( util::console::ERR );
				util::console::print( "failed to hook a function for null target\n" );
				util::console::set_prefix( util::console::NONE );
				return false;
			}

			MH_STATUS status = MH_UNKNOWN;

			// create hook.
			status = MH_CreateHook( target , func , ( void** ) original );

			if ( status != MH_OK ) {
				util::console::set_prefix( util::console::ERR );
				util::console::print( "failed create hook 0x%p ==> 0x%p. status: %s \n" , target , func , MH_StatusToString( status ) );
				util::console::set_prefix( util::console::NONE );

				return false;
			}

			// enable hook.
			status = MH_EnableHook( target );

			if ( status != MH_OK ) {
				util::console::set_prefix( util::console::ERR );
				util::console::print( "failed enable hook 0x%p ==> 0x%p. status: %s \n" , target , func , MH_StatusToString( status ) );
				util::console::set_prefix( util::console::NONE );

				return false;
			}

			util::console::set_prefix( util::console::HOOK );
			util::console::print( "hooked 0x%p ==> 0x%p\n" , target , func );
			util::console::set_prefix( util::console::NONE );
			return true;
		}

		template < typename T >
		inline bool hook_address( uintptr_t target , void* func , T** original ) {
			return hook_address( reinterpret_cast< void* >( target ) , func , original );
		}

		class vmt_hook {
		private:
			void* m_base;

		public:
			vmt_hook( ) : m_base { nullptr } { }
			vmt_hook( void* base ) : m_base { nullptr } { initialize( base ); }
			vmt_hook( uintptr_t base ) : m_base { nullptr } { initialize( base ); }

			bool initialize( void* base ) {
				if ( !base )
					return false;

				m_base = base;
				return true;
			}

			bool initialize( uintptr_t base ) {
				return initialize( reinterpret_cast< void* >( base ) );
			}

			template < typename T >
			bool hook_method( uintptr_t index , void* func , T** original ) {
				if ( !m_base ) {
					util::console::set_prefix( util::console::ERR );
					util::console::print( "failed to hook a function for null base, index: %i\n" , ( int ) index );
					util::console::set_prefix( util::console::NONE );
					return false;
				}

				// get target to hook at.
				void* target = util::get_virtual_function< void* >( m_base , index );

				return util::hook::hook_address( target , func , original );
			}
		};
	}
}
