#pragma once
#include <Windows.h>
#include "../util/include.hpp"

class Player;
class vec3_t;
class CUserCmd;

namespace globals {
	extern HWND csgo_window;
	extern Player* local_player;
	extern bool bSendPacket;
	extern vec3_t m_strafe_angles;
	extern CUserCmd* pCmd;
	extern int last_frame_stage;
}

namespace patterns {
	extern util::pattern surface_lock_cursor;
	extern util::pattern clientmodeshared_createmove;
	extern util::pattern override_view_sig;
	extern util::pattern modelrender_drawmodelexecute;
	extern util::pattern c_csplayer_get_eye_angles;
	extern util::pattern CL_Move;
	extern util::pattern CL_SendMove;
	extern util::pattern CCLCMsg_Move_Deconstructor;
	extern util::pattern CCLCMsg_Move_set_data;
	extern util::pattern DelayUnscope;
	//extern util::pattern GetSequenceActivity;
	extern util::pattern CCSGOPlayerAnimState_CacheSequences;
	//extern util::pattern CCSGOPlayerAnimState_DoProceduralFootPlant;
	//extern util::pattern CCSGOPlayerAnimState_ModifyEyePosition;
	extern util::pattern CCSGOPlayerAnimState_SetUpAimMatrix;
	extern util::pattern m_nAnimOverlay;
	extern util::pattern GetFirstSequenceAnimTag;
	extern util::pattern CBasePlayer_CalcViewBob;
	extern util::pattern CPredictedViewModel_AddViewModelBob;
	extern util::pattern SetupBones;
	extern util::pattern ProcessInterpolatedList;
	extern util::pattern Allow_Extrapolation;
	extern util::pattern player_animstate_offset;
	extern util::pattern player_bone_count_offset;
	extern util::pattern player_bone_cache_offset;
	extern util::pattern player_invalidate_physics_recursive;
	extern util::pattern player_lookup_bone;
	extern util::pattern player_spawn_time;
	extern util::pattern SetAbsAngles;
	extern util::pattern SetAbsOrigin;
	extern util::pattern IsLocalPlayer;
	extern util::pattern studioHdr;
	extern util::pattern BoneAccessor;
	extern int* m_nPredictionRandomSeed;
	extern Player* m_pPredictionPlayer;

	bool init( void );
}
