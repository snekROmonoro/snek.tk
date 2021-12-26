#pragma once
#include <memory>
#include <windows.h>
#include <psapi.h>
#include <cstdint>
#include "../console/console.hpp"
#include "../address/address.hpp"

#define in_range( x, a, b ) ( x >= a && x <= b )
#define get_bits( x ) ( in_range( ( x & ( ~0x20 ) ), 'A', 'F' ) ? ( ( x & ( ~0x20 ) ) - 'A' + 0xA ) : ( in_range( x, '0', '9' ) ? x - '0' : 0 ) )
#define get_byte( x ) ( get_bits( x[ 0 ] ) << 4 | get_bits( x[ 1 ] ) )

namespace util {
	class pattern : public address {
	public:
		pattern( uintptr_t addr = 0 ) : address( addr ) { }
		pattern( void* addr ) : address( addr ) { }

		pattern( address addr ) : address( addr ) { }

		pattern resolve_rip( ) {
			//if ( !this->m_addr )
			//	return pattern( );
			//
			//return pattern( this->m_addr + *( int* ) ( this->m_addr + 1 ) + 5 );
			return rel32( 0x1 );
		}

		static pattern search( const char* mod , const char* sig , bool silent = false )
		{
			auto _find_signature = [ ] ( const char* _mod , const char* sig ) -> uintptr_t {
				auto mod = util::modules::GetModule( _mod );
				auto pat1 = const_cast< char* >( sig );

				auto range_start = mod.GetBase( ).get( );
				auto end = range_start + mod.GetImageSize( );

				uintptr_t first_match = 0;

				for ( std::uintptr_t current_address = range_start; current_address < end; current_address++ ) {
					if ( !*pat1 ) {
						return first_match;
					}

					if ( *reinterpret_cast< std::uint8_t* >( pat1 ) == '\?' || *reinterpret_cast< std::uint8_t* >( current_address ) == get_byte( pat1 ) ) {
						if ( !first_match )
							first_match = current_address;

						if ( !pat1 [ 2 ] ) {
							return first_match;
						}

						if ( *reinterpret_cast< std::uint16_t* >( pat1 ) == '\?\?' || *reinterpret_cast< std::uint8_t* >( pat1 ) != '\?' )
							pat1 += 3;
						else
							pat1 += 2;
					}
					else {
						pat1 = const_cast< char* >( sig );
						first_match = 0;
					}
				}

				return 0;
			};

			pattern ret = pattern( _find_signature( mod , sig ) );

			if ( ret.get( ) ) {
				if ( !silent ) {
					util::console::set_prefix( util::console::SIG );
					util::console::print( "found in module %s pattern \"%s\" at: 0x%p\n" , mod , sig , ret.get< void* >( ) );
					util::console::set_prefix( util::console::NONE );
				}
			}
			else {
				util::console::set_prefix( util::console::ERR );
				util::console::print( "couldn't find pattern \"%s\" in module %s\n" , sig , mod );
				util::console::set_prefix( util::console::NONE );
			}

			return ret;
		};
	};
}
