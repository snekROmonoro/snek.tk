#pragma once
#include <cstdint>

namespace util {
	class address {
	public:
		uintptr_t m_addr;

		address( uintptr_t addr = 0 ) {
			this->m_addr = addr;
		}

		address( void* addr ) {
			this->m_addr = ( uintptr_t ) addr;
		}

		template< typename T = uintptr_t >
		T get( ) {
			return ( ( T ) this->m_addr );
		}

		template < typename T = uintptr_t >
		void set( const T& value ) {
			if ( !this->m_addr )
				return;

			*( T* ) this->m_addr = value;
		}

		address sub( uintptr_t bytes ) {
			return address( this->m_addr - bytes );
		}

		address add( uintptr_t bytes ) {
			return address( this->m_addr + bytes );
		}

		address deref( ) {
			return address( *( uintptr_t* ) this->m_addr );
		}

		template< typename t = address >
		__forceinline t rel32( size_t offset ) {
			if ( !m_addr )
				return t { };

			uintptr_t out = m_addr + offset;

			// get rel32 offset.
			uint32_t r = *( uint32_t* ) out;
			if ( !r )
				return t { };

			// relative to address of next instruction.
			out = ( out + 4 ) + r;

			return ( t ) out;
		}

		// checks if address is not null and has correct page protection.
		static __forceinline bool valid( uintptr_t addr ) {
			MEMORY_BASIC_INFORMATION mbi;

			// check for invalid address.
			if ( !addr )
				return false;

			// check for invalid page protection.
			if ( !VirtualQuery( ( const void* ) addr , &mbi , sizeof( mbi ) ) )
				return false;

			// todo - dex; fix this, its wrong... check for rwe or something too
			if ( /*!( mbi.State & MEM_COMMIT ) ||*/ ( mbi.Protect & PAGE_NOACCESS ) || ( mbi.Protect & PAGE_GUARD ) )
				return false;

			return true;
		}

		// relative virtual address.
		template< typename t = address >
		static __forceinline t RVA( address base , size_t offset ) {
			return base.add( offset ).get< t >( );
		}
	};
}
