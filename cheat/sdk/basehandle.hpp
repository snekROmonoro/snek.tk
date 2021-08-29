#pragma once

class Entity;

#define NUM_ENT_ENTRY_BITS			(11 + 2)
#define NUM_ENT_ENTRIES				(1 << NUM_ENT_ENTRY_BITS)
#define INVALID_EHANDLE_INDEX		0xFFFFFFFF
#define NUM_SERIAL_NUM_BITS			16
#define NUM_SERIAL_NUM_SHIFT_BITS	(32 - NUM_SERIAL_NUM_BITS)
#define ENT_ENTRY_MASK				(( 1 << NUM_SERIAL_NUM_BITS) - 1)

class CBaseHandle {
public:
	CBaseHandle( ) {
		m_Index = INVALID_EHANDLE_INDEX;
	}

	CBaseHandle( const CBaseHandle& other ) {
		m_Index = other.m_Index;
	}

	CBaseHandle( unsigned long value ) {
		m_Index = value;
	}

	CBaseHandle( int iEntry , int iSerialNumber ) {
		Init( iEntry , iSerialNumber );
	}

	void Init( int iEntry , int iSerialNumber ) {
		m_Index = iEntry | ( iSerialNumber << NUM_SERIAL_NUM_SHIFT_BITS );
	}

	void Term( ) {
		m_Index = INVALID_EHANDLE_INDEX;
	}

	bool IsValid( ) const {
		return m_Index != INVALID_EHANDLE_INDEX;
	}

	int GetEntryIndex( ) const {
		if ( !IsValid( ) )
			return NUM_ENT_ENTRIES - 1;

		return m_Index & ENT_ENTRY_MASK;
	}

	int GetSerialNumber( ) const {
		return m_Index >> NUM_SERIAL_NUM_SHIFT_BITS;
	}

	int ToInt( ) const {
		return ( int ) ( m_Index );
	}

	bool operator !=( const CBaseHandle& other ) const {
		return m_Index != other.m_Index;
	}

	bool operator ==( const CBaseHandle& other ) const {
		return m_Index == other.m_Index;
	}

	bool operator ==( const void* pEnt ) const {
		return Get( ) == pEnt;
	}

	bool operator !=( const void* pEnt ) const {
		return Get( ) != pEnt;
	}

	const CBaseHandle& operator=( const void* pEntity ) {
		return Set( pEntity );
	}

	const CBaseHandle& Set( const void* pEntity ) {
		if ( !pEntity )
			m_Index = INVALID_EHANDLE_INDEX;

		return *this;
	}

	void* Get( ) const {
		return nullptr;
	}

	unsigned long m_Index;
};

template< class t > class CHandle : public CBaseHandle {
public:
	CHandle( ) {}

	CHandle( int iEntry , int iSerialNumber ) {
		Init( iEntry , iSerialNumber );
	}

	CHandle( const CBaseHandle& handle ) : CBaseHandle( handle ) {}

	CHandle( t* pObj ) {
		Term( );
		Set( pObj );
	}

	CHandle< t > FromIndex( int index ) {
		CHandle< t > ret;
		ret.m_Index = index;
		return ret;
	}

	t* Get( ) const {
		return ( t* ) ( CBaseHandle::Get( ) );
	}

	operator t* ( ) {
		return Get( );
	}

	operator t* ( ) const {
		return Get( );
	}

	bool operator !( ) const {
		return !Get( );
	}

	bool operator==( t* val ) const {
		return Get( ) == val;
	}

	bool operator!=( t* val ) const {
		return Get( ) != val;
	}

	void Set( const t* pVal ) {
		CBaseHandle::Set( ( const void* ) ( pVal ) );
	}

	const CBaseHandle& operator=( const t* val ) {
		Set( val );
		return *this;
	}

	t* operator->( ) const {
		return Get( );
	}
};

using EHANDLE = CHandle< Entity >;
