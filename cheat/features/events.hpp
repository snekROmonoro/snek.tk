#pragma once
#include "../sdk/include.hpp"

class EventListener : public IGameEventListener2 {
	using game_event_t = void( __cdecl* )( IGameEvent* );

private:
	std::map< std::string , std::vector< game_event_t > > m_events;

public:
	EventListener( ) : m_events { } {
		m_debug_id = 42;
	}

	void init( );

	void add( const std::string& name , game_event_t fn ) {
		m_events [ name ].push_back( fn );
	}

	void register_events( ) {
		if ( m_events.empty( ) )
			return;

		for ( const auto& e : m_events ) {
			sdk::interfaces::event_manager->AddListener( this , e.first.data( ) , false );

			util::console::set_prefix( util::console::HOOK );
			util::console::print( "hooked event: %s\n" , e.first.data( ) );
			util::console::set_prefix( util::console::NONE );
		}
	}

	void remove_events( ) {
		sdk::interfaces::event_manager->RemoveListener( this );

		util::console::set_prefix( util::console::WARN );
		util::console::print( "removed event listener\n" );
		util::console::set_prefix( util::console::NONE );
	}

	void FireGameEvent( IGameEvent* evt ) override {
		const std::string name = evt->GetName( );

		if ( m_events.count( name ) != 0 ) {
			const auto& events = m_events [ name ];
			for ( const auto& e : events )
				e( evt );
		}
	}
};

extern EventListener g_event_listener;

namespace events {
	void player_hurt( IGameEvent* evt );
}
