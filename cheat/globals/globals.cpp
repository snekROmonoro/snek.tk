#include "globals.hpp"
#include "../includes.hpp"

HWND globals::csgo_window = 0;
Player* globals::local_player = nullptr;
bool globals::bSendPacket = true;
vec3_t globals::m_strafe_angles = vec3_t( );
CUserCmd* globals::pCmd = nullptr;
int globals::last_frame_stage = ClientFrameStage_t::FRAME_UNDEFINED;

util::pattern patterns::surface_lock_cursor = util::pattern( );
util::pattern patterns::clientmodeshared_createmove = util::pattern( );
util::pattern patterns::override_view_sig = util::pattern( );
util::pattern patterns::modelrender_drawmodelexecute = util::pattern( );
util::pattern patterns::c_csplayer_get_eye_angles = util::pattern( );
util::pattern patterns::CL_Move = util::pattern( );
util::pattern patterns::CL_SendMove = util::pattern( );
util::pattern patterns::CCLCMsg_Move_Deconstructor = util::pattern( );
util::pattern patterns::CCLCMsg_Move_set_data = util::pattern( );
util::pattern patterns::DelayUnscope = util::pattern( );
//util::pattern patterns::GetSequenceActivity = util::pattern( );
util::pattern patterns::CCSGOPlayerAnimState_CacheSequences = util::pattern( );
//util::pattern patterns::CCSGOPlayerAnimState_DoProceduralFootPlant = util::pattern( );
//util::pattern patterns::CCSGOPlayerAnimState_ModifyEyePosition = util::pattern( );
util::pattern patterns::CCSGOPlayerAnimState_SetUpAimMatrix = util::pattern( );
util::pattern patterns::m_nAnimOverlay = util::pattern( );
util::pattern patterns::GetFirstSequenceAnimTag = util::pattern( );
util::pattern patterns::CBasePlayer_CalcViewBob = util::pattern( );
util::pattern patterns::CPredictedViewModel_AddViewModelBob = util::pattern( );
util::pattern patterns::SetupBones = util::pattern( );
util::pattern patterns::ProcessInterpolatedList = util::pattern( );
util::pattern patterns::Allow_Extrapolation = util::pattern( );
util::pattern patterns::player_animstate_offset = util::pattern( );
util::pattern patterns::player_bone_count_offset = util::pattern( );
util::pattern patterns::player_bone_cache_offset = util::pattern( );
util::pattern patterns::player_invalidate_physics_recursive = util::pattern( );
util::pattern patterns::player_lookup_bone = util::pattern( );
util::pattern patterns::player_spawn_time = util::pattern( );
util::pattern patterns::m_LastCmd = util::pattern( );
util::pattern patterns::m_afButtonForced = util::pattern( );
util::pattern patterns::m_afButtonLast = util::pattern( );
util::pattern patterns::SetAbsAngles = util::pattern( );
util::pattern patterns::SetAbsOrigin = util::pattern( );
util::pattern patterns::IsLocalPlayer = util::pattern( );
util::pattern patterns::studioHdr = util::pattern( );
util::pattern patterns::BoneAccessor = util::pattern( );
int patterns::PreThink_idx = 0;
int patterns::Think_idx = 0;
util::pattern patterns::player_SetNextThink = util::pattern( );
util::pattern patterns::player_PhysicsRunThink = util::pattern( );
int* patterns::m_nPredictionRandomSeed = nullptr;
Player* patterns::m_pPredictionPlayer = nullptr;

bool patterns::init( void )
{
	patterns::surface_lock_cursor = util::pattern::search( "vguimatsurface.dll" , "80 3D ? ? ? ? 00 8B 91 A4 02 00 00 8B 0D ? ? ? ? C6 05 ? ? ? ? 01" );
	if ( !patterns::surface_lock_cursor.get( ) )
		return false;

	patterns::clientmodeshared_createmove = util::pattern::search( "client.dll" , "55 8B EC 8B 4D 04 8B C1" );
	if ( !patterns::clientmodeshared_createmove.get( ) )
		return false;

	patterns::override_view_sig = util::pattern::search( "client.dll" , "55 8B EC 83 E4 F8 8B 4D 04 83 EC 58" );
	if ( !patterns::override_view_sig.get( ) )
		return false;

	patterns::modelrender_drawmodelexecute = util::pattern::search( "engine.dll" , "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 89 4C 24 10" );
	if ( !patterns::modelrender_drawmodelexecute.get( ) )
		return false;

	patterns::c_csplayer_get_eye_angles = util::pattern::search( "client.dll" , "56 8B F1 85 F6 74 32" );
	if ( !patterns::c_csplayer_get_eye_angles.get( ) )
		return false;

	patterns::CL_Move = util::pattern::search( "engine.dll" , "55 8B EC 81 EC ? ? ? ? 53 56 8A F9 F3 0F 11 45 ? 8B 4D 04" );
	if ( !patterns::CL_Move.get( ) )
		return false;

	patterns::CL_SendMove = util::pattern::search( "engine.dll" , "E8 ? ? ? ? 84 DB 0F 84 ? ? ? ? 8B 8F ? ? ? ?" ).resolve_rip( );
	if ( !patterns::CL_SendMove.get( ) )
		return false;

	patterns::CCLCMsg_Move_Deconstructor = util::pattern::search( "engine.dll" , "53 8B D9 56 8D 73 24 57 C7 03 ? ? ? ?" );
	if ( !patterns::CCLCMsg_Move_Deconstructor.get( ) )
		return false;

	//patterns::CCLCMsg_Move_set_data = util::pattern::search( "engine.dll" , "E8 ? ? ? ? 8D 7E 18" ).resolve_rip( );
	patterns::CCLCMsg_Move_set_data = util::pattern::search( "engine.dll" , "55 8B EC 53 8B 5D 08 56 8B F1 85 DB 74 57" );
	if ( !patterns::CCLCMsg_Move_set_data.get( ) )
		return false;

	patterns::DelayUnscope = util::pattern::search( "client.dll" , "55 8B EC A1 ? ? ? ? 57 8B F9 B9 ? ? ? ? FF 50 ? 85 C0 75 ? 32 C0" );
	if ( !patterns::DelayUnscope.get( ) )
		return false;

	//	patterns::GetSequenceActivity = util::pattern::search( "client.dll" , "55 8B EC 53 8B 5D 08 56 8B F1 83 FB FF 0F 84 ? ? ? ?" );
	//	if ( !patterns::GetSequenceActivity.get( ) )
	//		return false;

	patterns::CCSGOPlayerAnimState_CacheSequences = util::pattern::search( "client.dll" , "55 8B EC 83 E4 F8 83 EC 34 53 56 8B F1 57 8B 46 60 85 C0 75 09 32 C0" );
	if ( !patterns::CCSGOPlayerAnimState_CacheSequences.get( ) )
		return false;

	//	patterns::CCSGOPlayerAnimState_DoProceduralFootPlant = util::pattern::search( "client.dll" , "55 8B EC 83 E4 F0 83 EC 78 56 8B F1 57 8B 56 60 85 D2 0F 84 ? ? ? ?" );
	//	if ( !patterns::CCSGOPlayerAnimState_DoProceduralFootPlant.get( ) )
	//		return false;

	//	patterns::CCSGOPlayerAnimState_ModifyEyePosition = util::pattern::search( "client.dll" , "55 8B EC 83 E4 F8 83 EC 5C 53 8B D9 56 57 83 7B 60 00 0F 84 ? ? ? ?" );
	//	if ( !patterns::CCSGOPlayerAnimState_ModifyEyePosition.get( ) )
	//		return false;

	patterns::CCSGOPlayerAnimState_SetUpAimMatrix = util::pattern::search( "client.dll" , "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D ? ? ? ?" );
	if ( !patterns::CCSGOPlayerAnimState_SetUpAimMatrix.get( ) )
		return false;

	//patterns::m_nAnimOverlay = util::pattern::search( "client.dll" , "8B 80 ? ? ? ? 03 C1 74 15" );
	patterns::m_nAnimOverlay = util::pattern::search( "client.dll" , "8B 80 ? ? ? ? 8D 34 C8" ).add( 0x2 ).deref( );
	if ( !patterns::m_nAnimOverlay.get( ) )
		return false;

	patterns::GetFirstSequenceAnimTag = util::pattern::search( "client.dll" , "E8 ? ? ? ? F3 0F 11 86 ? ? ? ? 0F 57 DB" ).resolve_rip( );
	if ( !patterns::GetFirstSequenceAnimTag.get( ) )
		return false;

	patterns::CBasePlayer_CalcViewBob = util::pattern::search( "client.dll" , "55 8B EC A1 ? ? ? ? 83 EC 10 56 8B F1 B9" );
	if ( !patterns::CBasePlayer_CalcViewBob.get( ) )
		return false;

	patterns::CPredictedViewModel_AddViewModelBob = util::pattern::search( "client.dll" , "55 8B EC A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 34 85 C0 74 35" );
	if ( !patterns::CPredictedViewModel_AddViewModelBob.get( ) )
		return false;

	patterns::SetupBones = util::pattern::search( "client.dll" , "55 8B EC 83 E4 F0 B8 D8" );
	if ( !patterns::SetupBones.get( ) )
		return false;

	patterns::ProcessInterpolatedList = util::pattern::search( "client.dll" , "53 0F B7 1D ? ? ? ? 56" );
	if ( !patterns::ProcessInterpolatedList.get( ) )
		return false;

	patterns::Allow_Extrapolation = util::pattern::search( "client.dll" , "A2 ? ? ? ? 8B 45 E8" );
	if ( !patterns::Allow_Extrapolation.get( ) )
		return false;

	patterns::player_animstate_offset = util::pattern::search( "client.dll" , "8B 8E ? ? ? ? F3 0F 10 48 04 E8 ? ? ? ? E9" ).add( 2 ).deref( );
	if ( !patterns::player_animstate_offset.get( ) )
		return false;

	patterns::player_bone_count_offset = util::pattern::search( "client.dll" , "8B 87 ? ? ? ? 8B 4D 0C" ).add( 2 ).deref( );
	if ( !patterns::player_bone_count_offset.get( ) )
		return false;

	patterns::player_bone_cache_offset = util::pattern::search( "client.dll" , "FF B7 ? ? ? ? 52" ).add( 2 ).deref( );
	if ( !patterns::player_bone_cache_offset.get( ) )
		return false;

	patterns::player_invalidate_physics_recursive = util::pattern::search( "client.dll" , "55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3 56" );
	if ( !patterns::player_invalidate_physics_recursive.get( ) )
		return false;

	patterns::player_lookup_bone = util::pattern::search( "client.dll" , "55 8B EC 53 56 8B F1 57 83 BE ? ? ? ? ? 75 14" );
	if ( !patterns::player_lookup_bone.get( ) )
		return false;

	patterns::player_spawn_time = util::pattern::search( "client.dll" , "F3 0F 5C 88 ? ? ? ? 0F" ).add( 0x4 ).deref( );
	if ( !patterns::player_spawn_time.get( ) )
		return false;

	patterns::m_LastCmd = util::pattern::search( "client.dll" , "8D 8E ? ? ? ? 89 5C 24 3C" );
	if ( !patterns::m_LastCmd.get( ) )
		return false;

	patterns::m_afButtonForced = util::pattern::search( "client.dll" , "8B 86 ? ? ? ? 09 47 30" );
	if ( !patterns::m_afButtonForced.get( ) )
		return false;

	//patterns::m_afButtonLast = util::pattern::search( "client.dll" , "33 CA 89 86 ? ? ? ?" );
	patterns::m_afButtonLast = 0x3208;
	if ( !patterns::m_afButtonLast.get( ) )
		return false;

	patterns::SetAbsAngles = util::pattern::search( "client.dll" , "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8 ?" );
	if ( !patterns::SetAbsAngles.get( ) )
		return false;

	patterns::SetAbsOrigin = util::pattern::search( "client.dll" , "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ?" );
	if ( !patterns::SetAbsOrigin.get( ) )
		return false;

	patterns::IsLocalPlayer = util::pattern::search( "client.dll" , "74 ? 8A 83 ? ? ? ? 88" ).add( 0x4 ).deref( );
	if ( !patterns::IsLocalPlayer.get( ) )
		return false;

	patterns::studioHdr = util::pattern::search( "client.dll" , "8B 86 ? ? ? ? 89 44 24 10 85 C0" ).add( 0x2 ).deref( );
	if ( !patterns::studioHdr.get( ) )
		return false;

	patterns::BoneAccessor = util::pattern::search( "client.dll" , "8D 81 ? ? ? ? 50 8D 84 24" ).add( 0x2 ).deref( );
	if ( !patterns::BoneAccessor.get( ) )
		return false;

	util::pattern sPreThink = util::pattern::search( "client.dll" , "FF 90 ? ? ? ? 8B 86 ? ? ? ? 83 F8 FF" ).add( 0x2 );
	if ( !sPreThink.get( ) )
		return false;

	patterns::PreThink_idx = *( int* ) ( sPreThink.get( ) ) / 4;

	util::pattern sThink = util::pattern::search( "client.dll" , "FF 90 ? ? ? ? FF 35 ? ? ? ? 8B 4C 24 3C" ).add( 0x2 );
	if ( !sThink.get( ) )
		return false;

	patterns::Think_idx = *( int* ) ( sThink.get( ) ) / 4;

	patterns::player_SetNextThink = util::pattern::search( "client.dll" , "55 8B EC 56 57 8B F9 8B B7 E8" );
	if ( !patterns::player_SetNextThink.get( ) )
		return false;

	patterns::player_PhysicsRunThink = util::pattern::search( "client.dll" , "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87 ? ? ? ? C1 E8 16" );
	if ( !patterns::player_PhysicsRunThink.get( ) )
		return false;

	patterns::m_nPredictionRandomSeed = util::address( util::get_virtual_function< void* >( sdk::interfaces::prediction , 19 ) ).add( 0x30 ).deref( ).get< int* >( );
	patterns::m_pPredictionPlayer = util::address( util::get_virtual_function< void* >( sdk::interfaces::prediction , 19 ) ).add( 0x54 ).deref( ).get< Player* >( );

	//patterns::m_nPredictionRandomSeed = util::pattern::search( "client.dll" , "8B 47 40 A3" ).add( 4 ).deref( ).get< int* >( );
	//patterns::m_pPredictionPlayer = util::pattern::search( "client.dll" , "0F 5B C0 89 35" ).add( 5 ).deref( ).get< Player* >( );

	return true;
}
