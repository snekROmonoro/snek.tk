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

	// render start was not called.
	if ( globals::last_frame_stage == FRAME_NET_UPDATE_END ) {
		int start = sdk::interfaces::client_state->last_command_ack;
		int stop = sdk::interfaces::client_state->lastoutgoingcommand + sdk::interfaces::client_state->chokedcommands;

		// call CPrediction::Update.
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

	sdk::interfaces::prediction->m_bInPrediction = true;

	static CMoveData data { };
	//memset( &data , 0 , sizeof( CMoveData ) );

	*patterns::m_nPredictionRandomSeed = globals::pCmd->random_seed;
	patterns::m_pPredictionPlayer = globals::local_player;

	// CPrediction::StartCommand
	globals::local_player->m_pCurrentCommand( ) = globals::pCmd;
	globals::local_player->m_PlayerCommand( ) = *globals::pCmd;

	sdk::interfaces::global_vars->curtime = globals::local_player->m_nTickBase( ) * sdk::interfaces::global_vars->interval_per_tick;
	sdk::interfaces::global_vars->frametime = sdk::interfaces::prediction->m_bEnginePaused ? 0.f : sdk::interfaces::global_vars->interval_per_tick;

	sdk::interfaces::move_helper->SetHost( globals::local_player );
	sdk::interfaces::game_movement->StartTrackPredictionErrors( globals::local_player );

	sdk::interfaces::prediction->SetupMove( globals::local_player , globals::pCmd , sdk::interfaces::move_helper , &data );

	sdk::interfaces::game_movement->ProcessMovement( globals::local_player , &data );
	sdk::interfaces::prediction->FinishMove( globals::local_player , globals::pCmd , &data );
	sdk::interfaces::game_movement->FinishTrackPredictionErrors( globals::local_player );

	sdk::interfaces::move_helper->SetHost( nullptr );
}

void features::prediction::restore( )
{
	if ( !globals::pCmd || !globals::local_player )
		return;

	sdk::interfaces::prediction->m_bInPrediction = false;

	*patterns::m_nPredictionRandomSeed = -1;
	patterns::m_pPredictionPlayer = nullptr;

	sdk::interfaces::global_vars->curtime = m_stored_variables::m_curtime;
	sdk::interfaces::global_vars->frametime = m_stored_variables::m_frametime;
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
