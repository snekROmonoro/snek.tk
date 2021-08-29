#pragma once
#include <cstdint>

/* added padding here too */
#define COMBINE( x, y ) x##y
#define COMBINE2( x, y ) COMBINE( x, y )

#define PAD( sz ) \
	private: \
	std::uint8_t COMBINE2( pad_, __COUNTER__ )[ sz ]; \
	public:

namespace util {
	template < typename T > static T get_virtual_function( void* base , const std::uint16_t index ) {
		return ( *reinterpret_cast< T** >( base ) ) [ index ];
	}
}
