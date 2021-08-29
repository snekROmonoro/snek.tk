#pragma once
#include "include.hpp"
#include "../globals/globals.hpp"
#include "interfaces/interfaces.hpp"

#define NETVAR_ADDITIVE( t, func, table, prop, off ) \
t& func( ) { \
	static auto offset = sdk::netvar::get( table, prop ); \
	return *( t* ) ( std::uintptr_t( this ) + offset + off ); \
}

#define NETVAR( t, func, table, prop ) \
t& func( ) { \
	static auto offset = sdk::netvar::get( table, prop ); \
	return *( t* ) ( std::uintptr_t( this ) + offset ); \
}

#define PNETVAR( t, func, table, prop ) \
t func( ) { \
	static auto offset = sdk::netvar::get( table, prop ); \
	return ( t ) ( std::uintptr_t( this ) + offset ); \
}

#define OFFSET( t, func, offset ) \
t& func( ) { \
	return *( t* ) ( std::uintptr_t( this ) + offset ); \
}

#define POFFSET( t, func, offset ) \
t func( ) { \
	return ( t ) ( std::uintptr_t( this ) + offset ); \
}

struct RenderableInstance_t {
	uint8_t m_alpha;
	RenderableInstance_t( ) : m_alpha { 255ui8 } {}
};

enum RenderFlags_t : uint32_t {
	STUDIO_NONE = 0x00000000 ,
	STUDIO_RENDER = 0x00000001 ,
	STUDIO_VIEWXFORMATTACHMENTS = 0x00000002 ,
	STUDIO_DRAWTRANSLUCENTSUBMODELS = 0x00000004 ,
	STUDIO_TWOPASS = 0x00000008 ,
	STUDIO_STATIC_LIGHTING = 0x00000010 ,
	STUDIO_WIREFRAME = 0x00000020 ,
	STUDIO_ITEM_BLINK = 0x00000040 ,
	STUDIO_NOSHADOWS = 0x00000080 ,
	STUDIO_WIREFRAME_VCOLLIDE = 0x00000100 ,
	STUDIO_NOLIGHTING_OR_CUBEMAP = 0x00000200 ,
	STUDIO_SKIP_FLEXES = 0x00000400 ,
	STUDIO_DONOTMODIFYSTENCILSTATE = 0x00000800 ,
	STUDIO_TRANSPARENCY = 0x80000000 ,
	STUDIO_SHADOWDEPTHTEXTURE = 0x40000000 ,
	STUDIO_SHADOWTEXTURE = 0x20000000 ,
	STUDIO_SKIP_DECALS = 0x10000000
};

enum DataUpdateType_t : int {
	DATA_UPDATE_CREATED = 0 ,
	DATA_UPDATE_DATATABLE_CHANGED ,
};

class IHandleEntity {
public:
	virtual ~IHandleEntity( ) {}
	virtual void SetRefEHandle( const CBaseHandle& handle ) = 0;
	virtual const unsigned long& GetRefEHandle( ) const = 0;
};

class IClientUnknown : public IHandleEntity {
public:
	virtual void* GetCollideable( ) = 0;
	virtual void* GetClientNetworkable( ) = 0;
	virtual void* GetClientRenderable( ) = 0;
	virtual void* GetIClientEntity( ) = 0;
	virtual void* GetBaseEntity( ) = 0;
	virtual void* GetClientThinkable( ) = 0;
	virtual void* GetClientAlphaProperty( ) = 0;
};

class Entity {
public:
	template< typename T >
	T& get( size_t offset ) {
		return *( T* ) ( ( uintptr_t ) this + offset );
	}

	template< typename T >
	void set( size_t offset , const T& val ) {
		*( T* ) ( ( uintptr_t ) this + offset ) = val;
	}

	template< typename T >
	T as( ) {
		return ( T ) this;
	}

	NETVAR( vec3_t , m_vecOrigin , "DT_BasePlayer" , "m_vecOrigin" );
	OFFSET( vec3_t , m_vecOldOrigin , 0x3A8 );
	NETVAR( vec3_t , m_vecVelocity , "DT_CSPlayer" , "m_vecVelocity[0]" );
	NETVAR( vec3_t , m_vecMins , "DT_BaseEntity" , "m_vecMins" );
	NETVAR( vec3_t , m_vecMaxs , "DT_BaseEntity" , "m_vecMaxs" );
	NETVAR( int , m_iTeamNum , "DT_BaseEntity" , "m_iTeamNum" );
	NETVAR( int , m_nSequence , "DT_PredictedViewModel" , "m_nSequence" );
	NETVAR( float , m_flCycle , "DT_CSPlayer" , "m_flCycle" );
	NETVAR( float , m_flAnimTime , "DT_CSPlayer" , "m_flAnimTime" );

	NETVAR( EHANDLE , m_hBombDefuser , "DT_PlantedC4" , "m_hBombDefuser" );
	NETVAR( float , m_flC4Blow , "DT_PlantedC4" , "m_flC4Blow" );
	NETVAR( float , m_flTimerLength , "DT_PlantedC4" , "m_flTimerLength" );
	NETVAR( float , m_flDefuseCountDown , "DT_PlantedC4" , "m_flDefuseCountDown" );
	NETVAR( bool , m_bBombTicking , "DT_PlantedC4" , "m_bBombTicking" );

	OFFSET( int , m_fEffects , 0xF0 );
	NETVAR( int , m_nModelIndex , "DT_BasePlayer" , "m_nModelIndex" );
	OFFSET( bool , m_bReadyToDraw , 0x274 );

	POFFSET( void* , renderable , 0x4 );
	POFFSET( void* , networkable , 0x8 );

	IClientUnknown* GetIClientUnknown( ) {
		return util::get_virtual_function< IClientUnknown* ( __thiscall* )( void* ) >( networkable( ) , 0 )( networkable( ) );
	}

	vec3_t& GetRenderOrigin( ) {
		return util::get_virtual_function< vec3_t& ( __thiscall* )( void* ) >( renderable( ) , 1 )( renderable( ) );
	}

	vec3_t& GetRenderAngles( ) {
		return util::get_virtual_function< vec3_t& ( __thiscall* )( void* ) >( renderable( ) , 2 )( renderable( ) );
	}

	const model_t* GetModel( ) {
		return util::get_virtual_function< const model_t* ( __thiscall* )( void* ) >( renderable( ) , 8 )( renderable( ) );
	}

	void DrawModel( int flags = STUDIO_RENDER , const RenderableInstance_t& instance = { } ) {
		return util::get_virtual_function< void( __thiscall* )( void* , int , const RenderableInstance_t& )>( renderable( ) , 9 )( renderable( ) , flags , instance );
	}

	bool SetupBones( matrix3x4_t* out , int max , int mask , float time ) {
		return util::get_virtual_function< bool( __thiscall* )( void* , matrix3x4_t* , int , int , float )>( renderable( ) , 13 )( renderable( ) , out , max , mask , time );
	}

	void Release( ) {
		return util::get_virtual_function< void( __thiscall* )( void* ) >( networkable( ) , 1 )( networkable( ) );
	}

	ClientClass* GetClientClass( ) {
		return util::get_virtual_function< ClientClass* ( __thiscall* )( void* ) >( networkable( ) , 2 )( networkable( ) );
	}

	void OnDataChanged( DataUpdateType_t type ) {
		return util::get_virtual_function< void( __thiscall* )( void* , DataUpdateType_t ) >( networkable( ) , 5 )( networkable( ) , type );
	}

	void PreDataUpdate( DataUpdateType_t type ) {
		return util::get_virtual_function< void( __thiscall* )( void* , DataUpdateType_t ) >( networkable( ) , 6 )( networkable( ) , type );
	}

	void PostDataUpdate( DataUpdateType_t type ) {
		return util::get_virtual_function< void( __thiscall* )( void* , DataUpdateType_t ) >( networkable( ) , 7 )( networkable( ) , type );
	}

	bool dormant( ) {
		return util::get_virtual_function< bool( __thiscall* )( void* ) >( networkable( ) , 9 )( networkable( ) );
	}

	int index( ) {
		return util::get_virtual_function< int( __thiscall* )( void* ) >( networkable( ) , 10 )( networkable( ) );
	}

	void SetDestroyedOnRecreateEntities( ) {
		return util::get_virtual_function< void( __thiscall* )( void* ) >( networkable( ) , 13 )( networkable( ) );
	}

	const vec3_t& GetAbsOrigin( ) {
		return util::get_virtual_function< const vec3_t& ( __thiscall* )( void* ) >( this , 10 )( this );
	}

	const vec3_t& GetAbsAngles( ) {
		return util::get_virtual_function< const vec3_t& ( __thiscall* )( void* ) >( this , 11 )( this );
	}

	void SetAbsAngles( const vec3_t& angles ) {
		using SetAbsAngles_t = void( __thiscall* )( decltype( this ) , const vec3_t& );
		patterns::SetAbsAngles.get< SetAbsAngles_t >( )( this , angles );
	}

	void SetAbsOrigin( const vec3_t& origin ) {
		using SetAbsOrigin_t = void( __thiscall* )( decltype( this ) , const vec3_t& );
		patterns::SetAbsOrigin.get< SetAbsOrigin_t >( )( this , origin );
	}

};

enum animstate_layer_t
{
	ANIMATION_LAYER_AIMMATRIX = 0 ,
	ANIMATION_LAYER_WEAPON_ACTION ,
	ANIMATION_LAYER_WEAPON_ACTION_RECROUCH ,
	ANIMATION_LAYER_ADJUST ,
	ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL ,
	ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB ,
	ANIMATION_LAYER_MOVEMENT_MOVE ,
	ANIMATION_LAYER_MOVEMENT_STRAFECHANGE ,
	ANIMATION_LAYER_WHOLE_BODY ,
	ANIMATION_LAYER_FLASHED ,
	ANIMATION_LAYER_FLINCH ,
	ANIMATION_LAYER_ALIVELOOP ,
	ANIMATION_LAYER_LEAN ,
	ANIMATION_LAYER_COUNT ,
};

enum animstate_pose_param_idx_t
{
	PLAYER_POSE_PARAM_FIRST = 0 ,
	PLAYER_POSE_PARAM_LEAN_YAW = PLAYER_POSE_PARAM_FIRST ,
	PLAYER_POSE_PARAM_SPEED ,
	PLAYER_POSE_PARAM_LADDER_SPEED ,
	PLAYER_POSE_PARAM_LADDER_YAW ,
	PLAYER_POSE_PARAM_MOVE_YAW ,
	PLAYER_POSE_PARAM_RUN ,
	PLAYER_POSE_PARAM_BODY_YAW ,
	PLAYER_POSE_PARAM_BODY_PITCH ,
	PLAYER_POSE_PARAM_DEATH_YAW ,
	PLAYER_POSE_PARAM_STAND ,
	PLAYER_POSE_PARAM_JUMP_FALL ,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_IDLE ,
	PLAYER_POSE_PARAM_AIM_BLEND_CROUCH_IDLE ,
	PLAYER_POSE_PARAM_STRAFE_DIR ,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_WALK ,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_RUN ,
	PLAYER_POSE_PARAM_AIM_BLEND_CROUCH_WALK ,
	PLAYER_POSE_PARAM_MOVE_BLEND_WALK ,
	PLAYER_POSE_PARAM_MOVE_BLEND_RUN ,
	PLAYER_POSE_PARAM_MOVE_BLEND_CROUCH_WALK ,
	//PLAYER_POSE_PARAM_STRAFE_CROSS,
	PLAYER_POSE_PARAM_COUNT ,
};

class Player;

struct animstate_pose_param_cache_t
{
	uint8_t pad_0x0 [ 0x4 ]; //0x0
	int m_nIndex;
	const char* m_szName;

	void SetValue( Player* pPlayer , float flValue );
};

class CCSGOPlayerAnimState {
public:
	char _pad0 [ 0x4 ];
	bool m_bFirstRunSinceInit;
	char _pad1 [ 0x5b ];
	Player* m_pPlayer;
	void* m_pWeapon;
	void* m_pWeaponLast;
	float m_flLastUpdateTime;
	int m_nLastUpdateFrame;
	float m_flLastUpdateIncrement;
	float m_flEyeYaw;
	float m_flEyePitch;
	float m_flFootYaw;
	float m_flFootYawLast;
	float m_flMoveYaw;
	float m_flMoveYawIdeal;
	float m_flMoveYawCurrentToIdeal;
	float m_flTimeToAlignLowerBody;
	float m_flPrimaryCycle;
	float m_flMoveWeight;
	float m_flMoveWeightSmoothed;
	float m_flAnimDuckAmount;
	float m_flDuckAdditional;
	float m_flRecrouchWeight;
	vec3_t m_vecPositionCurrent;
	vec3_t m_vecPositionLast;
	vec3_t m_vecVelocity;
	vec3_t m_vecVelocityNormalized;
	vec3_t m_vecVelocityNormalizedNonZero;
	float m_flVelocityLengthXY;
	float m_flVelocityLengthZ;
	float m_flSpeedAsPortionOfRunTopSpeed;
	float m_flSpeedAsPortionOfWalkTopSpeed;
	float m_flSpeedAsPortionOfCrouchTopSpeed;
	float m_flDurationMoving;
	float m_flDurationStill;
	bool m_bOnGround;
	bool m_bLanding;
	char _pad2 [ 0x12 ];
	float m_flWalkToRunTransition;
	char _pad3 [ 0x4 ];
	float m_flInAirSmoothValue;
	bool m_bOnLadder;
	char _pad4 [ 0x3 ];
	float m_flLadderWeight;
	char _pad5 [ 0x2c ];
	vec3_t m_vecVelocityLast;
	char _pad6 [ 0x45 ];
	bool m_bPlayerIsAccelerating;
	char _pad7 [ 0x2 ];
	animstate_pose_param_cache_t m_tPoseParamMappings [ PLAYER_POSE_PARAM_COUNT ];
	float m_flDurationMoveWeightIsTooHigh;
	float m_flStaticApproachSpeed;
	int m_nPreviousMoveState;
	float m_flStutterStep;
	char _pad8 [ 0x80 ];
	float m_flAimYawMin;
	float m_flAimYawMax;
	float m_flAimPitchMin;
	float m_flAimPitchMax;
	int m_nAnimstateModelVersion;
};

class CStudioHdr {
public:
	class mstudioposeparamdesc_t {
	public:
		int sznameindex;
		char* const name( void ) const { return ( ( char* ) this ) + sznameindex; }
		int flags;
		float start;
		float end;
		float loop;
	};

	studiohdr_t* m_pStudioHdr;
	void* m_pVModel;
};

class C_AnimationLayer {
public:
	float   m_anim_time;			// 0x0000
	float   m_fade_out_time;		// 0x0004
	int     m_flags;				// 0x0008
	int     m_activty;				// 0x000C
	int     m_priority;				// 0x0010
	int     m_order;				// 0x0014
	int     m_sequence;				// 0x0018
	float   m_prev_cycle;			// 0x001C
	float   m_weight;				// 0x0020
	float   m_weight_delta_rate;	// 0x0024
	float   m_playback_rate;		// 0x0028
	float   m_cycle;				// 0x002C
	Entity* m_owner;				// 0x0030
	int     m_bits;					// 0x0034
}; // size: 0x0038

class CBoneAccessor {
public:
	void* m_pAnimating;
	BoneArray* m_pBones;
	int m_ReadableBones;
	int m_WritableBones;
};

class CBoneCache {
public:
	BoneArray* m_pCachedBones;
	PAD( 0x8 );
	int m_CachedBoneCount;
};

struct VarMapEntry_t {
	unsigned short type;
	unsigned short m_bNeedsToInterpolate;
	void* data;
	void* watcher;
};

struct VarMapping_t {
	VarMapEntry_t* m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};

class Player : public Entity {
public:
	OFFSET( vec3_t , m_vecAbsVelocity , 0x94 );
	POFFSET( VarMapping_t* , GetVarMapping , 0x94 );

	NETVAR( int , m_lifeState , "DT_BasePlayer" , "m_lifeState" );
	NETVAR( int , m_fFlags , "DT_BasePlayer" , "m_fFlags" );
	NETVAR( int , m_MoveType , "C_BaseEntity" , "m_MoveType" );
	NETVAR( int , m_iHealth , "DT_CSPlayer" , "m_iHealth" );
	NETVAR( int , m_iAccount , "DT_CSPlayer" , "m_iAccount" );
	NETVAR( int , m_hViewModel , "DT_BasePlayer" , "m_hViewModel[0]" );
	NETVAR( bool , m_bHasDefuser , "DT_CSPlayer" , "m_bHasDefuser" );
	NETVAR( int , m_nHitboxSet , "DT_BaseAnimating" , "m_nHitboxSet" );
	NETVAR( vec3_t , m_angAbsRotation , "C_BaseEntity" , "m_angAbsRotation" );
	NETVAR( vec3_t , m_angRotation , "C_BaseEntity" , "m_angRotation" );
	NETVAR( vec3_t , m_angNetworkAngles , "C_BaseEntity" , "m_angNetworkAngles" );

	bool m_bIsLocalPlayer( ) {
		return get< bool >( patterns::IsLocalPlayer.get( ) );
	}

	CCSGOPlayerAnimState* m_PlayerAnimState( ) {
		return get< CCSGOPlayerAnimState* >( patterns::player_animstate_offset.get( ) );
	}

	void SetPlayerAnimState( CCSGOPlayerAnimState* state ) {
		set< CCSGOPlayerAnimState* >( patterns::player_animstate_offset.get( ) , state );
	}

	CStudioHdr* m_studioHdr( ) {
		return get< CStudioHdr* >( patterns::studioHdr.get( ) );
	}

	NETVAR( int , m_nTickBase , "DT_CSPlayer" , "m_nTickBase" );
	NETVAR( float , m_flNextAttack , "DT_CSPlayer" , "m_flNextAttack" );
	NETVAR( float , m_flDuckAmount , "DT_BasePlayer" , "m_flDuckAmount" );
	NETVAR( float , m_flDuckSpeed , "DT_BasePlayer" , "m_flDuckSpeed" );
	NETVAR( float , m_flSimulationTime , "DT_BaseEntity" , "m_flSimulationTime" );

	float& m_flOldSimulationTime( ) {
		static auto m_flOldSimulationTime = sdk::netvar::get( "DT_BaseEntity" , "m_flSimulationTime" ) + sizeof( float );
		return get< float >( m_flOldSimulationTime );
	}

	matrix3x4_t& m_pCoordFrame( ) {
		static auto m_pCoordFrame = sdk::netvar::get( "DT_BaseEntity" , "m_CollisionGroup" ) - 0x30;
		return get< matrix3x4_t >( m_pCoordFrame );
	}

	NETVAR( float , m_flLowerBodyYawTarget , "DT_CSPlayer" , "m_flLowerBodyYawTarget" );
	NETVAR( float , m_fImmuneToGunGameDamageTime , "DT_CSPlayer" , "m_fImmuneToGunGameDamageTime" );
	NETVAR( bool , m_bGunGameImmunity , "DT_CSPlayer" , "m_bGunGameImmunity" );
	NETVAR( bool , m_bHasHelmet , "DT_CSPlayer" , "m_bHasHelmet" );
	NETVAR( bool , m_bClientSideAnimation , "DT_CSPlayer" , "m_bClientSideAnimation" );
	NETVAR( bool , m_bHasHeavyArmor , "DT_CSPlayer" , "m_bHasHeavyArmor" );
	NETVAR( bool , m_bIsScoped , "DT_CSPlayer" , "m_bIsScoped" );
	NETVAR( bool , m_bDucking , "DT_CSPlayer" , "m_bDucking" );
	OFFSET( int , m_nNextThinkTick , 0xFC );
	NETVAR( bool , m_bSpotted , "DT_BaseEntity" , "m_bSpotted" );
	OFFSET( int , m_iOcclusionFlags , 0xA28 );
	OFFSET( int , m_iOcclusionFramecount , 0xA30 );
	NETVAR( int , m_iObserverMode , "DT_BasePlayer" , "m_iObserverMode" );
	NETVAR( int , m_ArmorValue , "DT_BasePlayer" , "m_ArmorValue" );
	NETVAR( float , m_flMaxspeed , "DT_BasePlayer" , "m_flMaxspeed" );
	NETVAR( float , m_surfaceFriction , "C_BasePlayer" , "m_surfaceFriction" );
	OFFSET( float , m_flFlashBangTime , 0xA2E8 );
	NETVAR( float , m_flFlashDuration , "DT_CSPlayer" , "m_flFlashDuration" );
	NETVAR( float , m_flFlashAlpha , "DT_CSPlayer" , "m_flFlashAlpha" );
	NETVAR( vec3_t , m_angEyeAngles , "DT_CSPlayer" , "m_angEyeAngles" );
	NETVAR( vec3_t , m_aimPunchAngle , "DT_CSPlayer" , "m_aimPunchAngle" );
	NETVAR( vec3_t , m_viewPunchAngle , "DT_CSPlayer" , "m_viewPunchAngle" );
	NETVAR( vec3_t , m_aimPunchAngleVel , "DT_CSPlayer" , "m_aimPunchAngleVel" );
	NETVAR( vec3_t , m_vecViewOffset , "DT_CSPlayer" , "m_vecViewOffset" );
	OFFSET( int , m_iEFlags , 0xE8 );
	PNETVAR( float* , m_flPoseParameter , "DT_CSPlayer" , "m_flPoseParameter" );
	OFFSET( uint32_t , GetReadableBones , 0x26AC );
	OFFSET( uint32_t , GetWriteableBones , 0x26B0 );
	POFFSET( matrix3x4_t* , GetBoneArrayForWrite , 0x26A8 );
	OFFSET( int , m_BoneCacheCount , 0x2910 + 0xC );
	OFFSET( CBoneCache , m_BoneCache , 0x2910 );
	POFFSET( matrix3x4_t** , m_BoneCache2 , 0x2910 );
	NETVAR( EHANDLE , m_hObserverTarget , "DT_CSPlayer" , "m_hObserverTarget" );
	NETVAR( EHANDLE , m_hActiveWeapon , "DT_BaseCombatCharacter" , "m_hActiveWeapon " );
	NETVAR( EHANDLE , m_hGroundEntity , "DT_CSPlayer" , "m_hGroundEntity" );
	PNETVAR( CBaseHandle* , m_hMyWeapons , "DT_CSPlayer" , "m_hMyWeapons" );

	C_AnimationLayer* m_AnimOverlay( ) {
		return get< C_AnimationLayer* >( patterns::m_nAnimOverlay.get( ) );
	}

	float& m_flSpawnTime( ) {
		return get< float >( patterns::player_spawn_time.get( ) );
	}

	CBoneAccessor& m_BoneAccessor( ) {
		return get< CBoneAccessor >( patterns::BoneAccessor.get( ) );
	}

	OFFSET( float , m_flVelocityModifier , 0xA38C );

	unsigned long GetRefEHandle( ) {
		if ( !this->GetIClientUnknown( ) )
			return NULL;

		return this->GetIClientUnknown( )->GetRefEHandle( );
	}

	void BuildTransformations( CStudioHdr* hdr , vec3_t* pos , quaternion* q , const matrix3x4a_t& transform , int mask , uint8_t* computed ) {
		using BuildTransformations_t = void( __thiscall* )( decltype( this ) , CStudioHdr* , vec3_t* , quaternion* , matrix3x4a_t const& , int , uint8_t* );
		return util::get_virtual_function< BuildTransformations_t >( this , 189 )( this , hdr , pos , q , transform , mask , computed );
	}

	void StandardBlendingRules( CStudioHdr* hdr , vec3_t* pos , quaternion* q , float time , int mask ) {
		using StandardBlendingRules_t = void( __thiscall* )( decltype( this ) , CStudioHdr* , vec3_t* , quaternion* , float , int );
		return util::get_virtual_function< StandardBlendingRules_t >( this , 205 )( this , hdr , pos , q , time , mask );
	}

	float GetFOV( ) {
		return util::get_virtual_function< float( __thiscall* )( decltype( this ) ) >( this , 326 )( this );
	}

	const vec3_t& WorldSpaceCenter( ) {
		return util::get_virtual_function< const vec3_t& ( __thiscall* )( void* ) >( this , 78 )( this );
	}

	void EyePosition( vec3_t* pos ) {
		util::get_virtual_function< float* ( __thiscall* )( Player* , vec3_t* ) >( this , 168 )( this , pos );
	}

};
