#include "events.hpp"
#include "../includes.hpp"

// https://wiki.alliedmods.net/Counter-Strike:_Global_Offensive_Events

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
	Player* local = sdk::interfaces::entity_list->GetClientEntity< Player* >( sdk::interfaces::engine->GetLocalPlayer( ) );
	if ( !attacker || !victim || !local )
		return;

	if ( attacker != local || victim == local )
		return;

	sdk::interfaces::debug_overlay->AddTextOverlay( victim->GetAbsOrigin( ) , 2.f , "HIT" );
}
