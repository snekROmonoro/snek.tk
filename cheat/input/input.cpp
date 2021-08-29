#include "input.hpp"
#include "../util/console/console.hpp"

void input::init( HWND hWindow )
{
	if ( m_bInitialized )
		return;

	// Init default important keys.
	m_Keys [ VK_LBUTTON ] = Key_t( VK_LBUTTON );

	m_hWindow = hWindow;
	m_bInitialized = true;

	util::console::set_prefix( util::console::NONE );
	util::console::print( "initialized input\n" );

	update( );
}

void input::update( )
{
	if ( !( m_hWindow && m_hWindow == GetForegroundWindow( ) ) ) {
		// We can reset every key state to false, since we're not
		// using this application right now.
		// But still, keep them there
		return;
	}

	for ( auto& Key : m_Keys ) {
		if ( !Key.first )
			continue;

		Key.second.nKey = Key.first;

		Key.second.PrevKeyState = Key.second.KeyState;
		Key.second.KeyState = GetAsyncKeyState( Key.second.nKey );
	}
}

bool input::pressed( int nKey )
{
	if ( m_Keys.find( nKey ) == m_Keys.end( ) )
		m_Keys [ nKey ] = Key_t( nKey );

	return m_Keys [ nKey ].KeyState && !m_Keys [ nKey ].PrevKeyState;
}

bool input::down( int nKey )
{
	if ( m_Keys.find( nKey ) == m_Keys.end( ) )
		m_Keys [ nKey ] = Key_t( nKey );

	return m_Keys [ nKey ].KeyState && m_Keys [ nKey ].PrevKeyState;
}

bool input::released( int nKey )
{
	if ( m_Keys.find( nKey ) == m_Keys.end( ) )
		m_Keys [ nKey ] = Key_t( nKey );

	return !m_Keys [ nKey ].KeyState && m_Keys [ nKey ].PrevKeyState;
}

input::Key_t& input::get_key_data( int nKey ) {
	if ( m_Keys.find( nKey ) == m_Keys.end( ) )
		m_Keys [ nKey ] = Key_t( nKey );

	return m_Keys [ nKey ];
}