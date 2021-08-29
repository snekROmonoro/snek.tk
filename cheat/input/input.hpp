#pragma once
#include <windows.h>
#include <unordered_map>

namespace input {
	struct Key_t {
		Key_t( ) {
			nKey = 0;
			PrevKeyState = KeyState = false;
		}

		Key_t( int key ) {
			nKey = key;
			PrevKeyState = KeyState = false;
		}

		int nKey;
		bool KeyState;
		bool PrevKeyState;

		bool operator==( Key_t b ) {
			return nKey == b.nKey;
		}

		bool operator!=( Key_t b ) {
			return nKey != b.nKey;
		}
	};
	
	inline std::unordered_map< int , Key_t > m_Keys;

	// Initializes and Updates at the same time.
	void init( HWND hWindow );

	// Should Update every Tick/Frame !!!
	void update( );

	// We pressed the key
	bool pressed( int nKey );

	// We're pressing the key at this moment
	bool down( int nKey );

	// We were pressing the key but we aren't anymore
	bool released( int nKey );

	// Get Key data
	Key_t& get_key_data( int nKey );

	inline HWND m_hWindow;
	inline bool m_bInitialized = false;
}
