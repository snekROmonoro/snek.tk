#include "prediction.hpp"
#include "../includes.hpp"

/**
  * CPrediction::RunCommand
  * https://github.com/perilouswithadollarsign/cstrike15_src/blob/master/game/client/prediction.cpp#L1037
  * @client.dll - 55 8B EC 83 E4 C0 83 EC 34 53 56 8B 75 08
*/

void features::prediction::update( )
{
	bool valid = sdk::interfaces::client_state->m_nDeltaTick > 0;

	if ( m_stored_variables::m_velocity_modifier < 1.0f ) {
		*( bool* ) ( ( uintptr_t ) sdk::interfaces::prediction + 0x24 ) = true;
	}

	if ( sdk::interfaces::client_state->m_nDeltaTick >= 0 ) {
		int start = sdk::interfaces::client_state->last_command_ack;
		int stop = sdk::interfaces::client_state->lastoutgoingcommand + sdk::interfaces::client_state->chokedcommands;

		sdk::interfaces::prediction->Update( sdk::interfaces::client_state->m_nDeltaTick , valid , start , stop );
	}
}

void features::prediction::predict( )
{
	if ( !globals::pCmd || !globals::local_player )
		return;

	// backup variables.
	m_stored_variables::m_tickbase = globals::local_player->m_nTickBase( );
	m_stored_variables::m_velocity_modifier = globals::local_player->m_flVelocityModifier( );
	m_stored_variables::m_velocity = globals::local_player->m_vecVelocity( );
	m_stored_variables::m_origin = globals::local_player->m_vecOrigin( );
	m_stored_variables::m_flags = globals::local_player->m_fFlags( );
	m_stored_variables::m_curtime = sdk::interfaces::global_vars->curtime;
	m_stored_variables::m_frametime = sdk::interfaces::global_vars->frametime;
	m_stored_variables::m_bInPrediction = sdk::interfaces::prediction->m_bInPrediction;
	m_stored_variables::m_bFirstTimePredicted = sdk::interfaces::prediction->m_bFirstTimePredicted;

	sdk::interfaces::prediction->m_bInPrediction = true;
	sdk::interfaces::prediction->m_bFirstTimePredicted = false;

	static CMoveData data { };
	//memset( &data , 0 , sizeof( CMoveData ) );

	// StartCommand
	globals::local_player->m_pCurrentCommand( ) = globals::pCmd;
	globals::local_player->m_LastCmd( ) = *globals::pCmd; // should we also do this?
	*patterns::m_nPredictionRandomSeed = globals::pCmd->random_seed;
	patterns::m_pPredictionPlayer = globals::local_player;

	// Set globals appropriately
	sdk::interfaces::global_vars->curtime = globals::local_player->m_nTickBase( ) * sdk::interfaces::global_vars->interval_per_tick;
	sdk::interfaces::global_vars->frametime = sdk::interfaces::prediction->m_bEnginePaused ? 0 : sdk::interfaces::global_vars->interval_per_tick;

	// Add and subtract buttons we're forcing on the player
	globals::pCmd->buttons |= globals::local_player->m_afButtonForced( );
	globals::pCmd->buttons &= ~globals::local_player->m_afButtonDisabled( );

	sdk::interfaces::move_helper->SetHost( globals::local_player );
	sdk::interfaces::game_movement->StartTrackPredictionErrors( globals::local_player );

	// Do weapon selection
	/*if ( ucmd->weaponselect != 0 )
	{
		C_BaseCombatWeapon* weapon = ToBaseCombatWeapon( CBaseEntity::Instance( ucmd->weaponselect ) );
		if ( weapon )
		{
			player->SelectItem( weapon->GetName( ) , ucmd->weaponsubtype );
		}
	}*/

	// vehicles?

	if ( globals::pCmd->impulse )
		globals::local_player->m_nImpulse( ) = globals::pCmd->impulse;

	// Get button states
	globals::local_player->UpdateButtonState( globals::pCmd->buttons );

	sdk::interfaces::prediction->CheckMovingGround( globals::local_player , sdk::interfaces::global_vars->frametime );

	// Copy from command to player unless game .dll has set angle using fixangle
	//player->SetLocalViewAngles( ucmd->viewangles );
	sdk::interfaces::prediction->SetLocalViewAngles( globals::pCmd->viewangles );

	// Call standard client pre-think
	//RunPreThink( player );
	if ( globals::local_player->PhysicsRunThink( 0 /*THINK_FIRE_ALL_FUNCTIONS*/ ) ) {
		globals::local_player->PreThink( );
	}

	// Call Think if one is set
	//RunThink( player , TICK_INTERVAL );
	const auto m_nNextTick = globals::local_player->get< int* >( 0xFC );
	if ( *m_nNextTick > 0 && *m_nNextTick <= globals::local_player->m_nTickBase( ) )
	{
		globals::local_player->SetNextThink( -1 ); // TICK_NEVER_THINK
		globals::local_player->Think( );
	}

	// Setup input.
	sdk::interfaces::prediction->SetupMove( globals::local_player , globals::pCmd , sdk::interfaces::move_helper , &data );

	// RUN MOVEMENT
	sdk::interfaces::game_movement->ProcessMovement( globals::local_player , &data );

	sdk::interfaces::prediction->FinishMove( globals::local_player , globals::pCmd , &data );

	sdk::interfaces::move_helper->ProcessImpacts( );

	// RunPostThink( player );

	sdk::interfaces::game_movement->FinishTrackPredictionErrors( globals::local_player );
}

void features::prediction::restore( )
{
	if ( !globals::pCmd || !globals::local_player )
		return;

	// FinishCommand
	globals::local_player->m_pCurrentCommand( ) = NULL;
	*patterns::m_nPredictionRandomSeed = NULL;
	patterns::m_pPredictionPlayer = NULL;

	sdk::interfaces::game_movement->Reset( );

	if ( !sdk::interfaces::prediction->m_bEnginePaused && sdk::interfaces::global_vars->frametime > 0 ) {
		globals::local_player->m_nTickBase( )++;
	}

	// restore globals.
	sdk::interfaces::global_vars->curtime = m_stored_variables::m_curtime;
	sdk::interfaces::global_vars->frametime = m_stored_variables::m_frametime;

	sdk::interfaces::prediction->m_bInPrediction = m_stored_variables::m_bInPrediction;
	sdk::interfaces::prediction->m_bFirstTimePredicted = m_stored_variables::m_bFirstTimePredicted;

	sdk::interfaces::move_helper->SetHost( nullptr );
}

void features::prediction::correct_viewmodel_data( )
{
	if ( !globals::local_player || !globals::local_player->alive( ) )
		return;

	if ( globals::local_player->m_hViewModel( ) != 0xFFFFFFFF ) {
		Entity* view_model = sdk::interfaces::entity_list->GetClientEntityFromHandle< Entity* >( globals::local_player->m_hViewModel( ) );
		if ( view_model ) {
			view_model->m_flCycle( ) = m_stored_viewmodel_variables::m_viewmodel_cycle;
			view_model->m_flAnimTime( ) = m_stored_viewmodel_variables::m_viewmodel_anim_time;
		}
	}
}

void features::prediction::update_viewmodel_data( )
{
	if ( !globals::local_player || !globals::local_player->alive( ) )
		return;

	if ( globals::local_player->m_hViewModel( ) != 0xFFFFFFFF ) {
		Entity* view_model = sdk::interfaces::entity_list->GetClientEntityFromHandle< Entity* >( globals::local_player->m_hViewModel( ) );
		if ( view_model ) {
			m_stored_viewmodel_variables::m_viewmodel_cycle = view_model->m_flCycle( );
			m_stored_viewmodel_variables::m_viewmodel_anim_time = view_model->m_flAnimTime( );
		}
	}
}
