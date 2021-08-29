#include "events.hpp"
#include "../includes.hpp"

EventListener g_event_listener;

void EventListener::init( )
{
	add( "player_hurt" , events::player_hurt );

	register_events( );
}

void events::player_hurt( IGameEvent* evt ) {
	// wait for cheat to initialize
	if ( !hooks::m_hooked )
		return;

	Player* attacker = sdk::interfaces::entity_list->GetClientEntity< Player* >( sdk::interfaces::engine->GetPlayerForUserID( evt->m_keys->FindKey( "attacker" )->GetInt( ) ) );
	Player* victim = sdk::interfaces::entity_list->GetClientEntity< Player* >( sdk::interfaces::engine->GetPlayerForUserID( evt->m_keys->FindKey( "userid" )->GetInt( ) ) );

}
