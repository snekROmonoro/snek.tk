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

typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;

enum Hitboxes_t : int {
	HITBOX_HEAD = 0 ,
	HITBOX_NECK ,
	HITBOX_PELVIS ,
	HITBOX_BODY ,
	HITBOX_THORAX ,
	HITBOX_CHEST ,
	HITBOX_UPPER_CHEST ,
	HITBOX_R_THIGH ,
	HITBOX_L_THIGH ,
	HITBOX_R_CALF ,
	HITBOX_L_CALF ,
	HITBOX_R_FOOT ,
	HITBOX_L_FOOT ,
	HITBOX_R_HAND ,
	HITBOX_L_HAND ,
	HITBOX_R_UPPER_ARM ,
	HITBOX_R_FOREARM ,
	HITBOX_L_UPPER_ARM ,
	HITBOX_L_FOREARM ,
	HITBOX_MAX
};

enum PlayerAnimEvent_t
{
	PLAYERANIMEVENT_FIRE_GUN_PRIMARY = 0 ,
	PLAYERANIMEVENT_FIRE_GUN_PRIMARY_OPT , // an optional primary attack for variation in animation. For example, the knife toggles between left AND right slash animations.
	PLAYERANIMEVENT_FIRE_GUN_PRIMARY_SPECIAL1 ,
	PLAYERANIMEVENT_FIRE_GUN_PRIMARY_OPT_SPECIAL1 , // an optional primary special attack for variation in animation.
	PLAYERANIMEVENT_FIRE_GUN_SECONDARY ,
	PLAYERANIMEVENT_FIRE_GUN_SECONDARY_SPECIAL1 ,
	PLAYERANIMEVENT_GRENADE_PULL_PIN ,
	PLAYERANIMEVENT_THROW_GRENADE ,
	PLAYERANIMEVENT_JUMP ,
	PLAYERANIMEVENT_RELOAD ,
	PLAYERANIMEVENT_RELOAD_START ,	///< w_model partial reload for shotguns
	PLAYERANIMEVENT_RELOAD_LOOP ,	///< w_model partial reload for shotguns
	PLAYERANIMEVENT_RELOAD_END ,		///< w_model partial reload for shotguns
	PLAYERANIMEVENT_CLEAR_FIRING ,	///< clear animations on the firing layer
	PLAYERANIMEVENT_DEPLOY ,			///< Used to play deploy animations on third person models.
	PLAYERANIMEVENT_SILENCER_ATTACH ,
	PLAYERANIMEVENT_SILENCER_DETACH ,

	// new events
	PLAYERANIMEVENT_THROW_GRENADE_UNDERHAND ,
	PLAYERANIMEVENT_CATCH_WEAPON ,
	PLAYERANIMEVENT_COUNT
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

enum BoneFlags_t : int {
	BONE_USED_BY_ANYTHING = 0x0007FF00 ,
	BONE_USED_BY_HITBOX = 0x00000100 , // bone (or child) is used by a hit box
	BONE_USED_BY_ATTACHMENT = 0x00000200 , // bone (or child) is used by an attachment point
	BONE_USED_BY_VERTEX_MASK = 0x0003FC00 ,
	BONE_USED_BY_VERTEX_LOD0 = 0x00000400 , // bone (or child) is used by the toplevel model via skinned vertex
	BONE_USED_BY_VERTEX_LOD1 = 0x00000800 ,
	BONE_USED_BY_VERTEX_LOD2 = 0x00001000 ,
	BONE_USED_BY_VERTEX_LOD3 = 0x00002000 ,
	BONE_USED_BY_VERTEX_LOD4 = 0x00004000 ,
	BONE_USED_BY_VERTEX_LOD5 = 0x00008000 ,
	BONE_USED_BY_VERTEX_LOD6 = 0x00010000 ,
	BONE_USED_BY_VERTEX_LOD7 = 0x00020000 ,
	BONE_USED_BY_BONE_MERGE = 0x00040000
};

enum CSWeaponType : int {
	WEAPONTYPE_UNKNOWN = -1 ,
	WEAPONTYPE_KNIFE = 0 ,
	WEAPONTYPE_PISTOL = 1 ,
	WEAPONTYPE_SUBMACHINEGUN = 2 ,
	WEAPONTYPE_RIFLE = 3 ,
	WEAPONTYPE_SHOTGUN = 4 ,
	WEAPONTYPE_SNIPER_RIFLE = 5 ,
	WEAPONTYPE_MACHINEGUN = 6 ,
	WEAPONTYPE_C4 = 7 ,
	WEAPONTYPE_TASER = 8 ,
	WEAPONTYPE_GRENADE = 9 ,
	WEAPONTYPE_HEALTHSHOT = 11
};

enum Teams_t : int {
	TEAM_NOTEAM = 0 ,
	TEAM_SPECTATOR ,
	TEAM_TERRORISTS ,
	TEAM_COUNTERTERRORISTS
};

enum Effects_t : int {
	EF_BONEMERGE = 0x001 ,	// Performs bone merge on client side
	EF_BRIGHTLIGHT = 0x002 ,	// DLIGHT centered at entity origin
	EF_DIMLIGHT = 0x004 ,	// player flashlight
	EF_NOINTERP = 0x008 ,	// don't interpolate the next frame
	EF_NOSHADOW = 0x010 ,	// Don't cast no shadow
	EF_NODRAW = 0x020 ,	// don't draw entity
	EF_NORECEIVESHADOW = 0x040 ,	// Don't receive no shadow
	EF_BONEMERGE_FASTCULL = 0x080 ,	// For use with EF_BONEMERGE. If this is set, then it places this ent's origin at its
	EF_ITEM_BLINK = 0x100 ,	// blink an item so that the user notices it.
	EF_PARENT_ANIMATES = 0x200 ,	// always assume that the parent entity is animating
	EF_MAX_BITS = 10
};

enum InvalidatePhysicsBits_t : int {
	POSITION_CHANGED = 0x1 ,
	ANGLES_CHANGED = 0x2 ,
	VELOCITY_CHANGED = 0x4 ,
	ANIMATION_CHANGED = 0x8 ,
	BOUNDS_CHANGED = 0x10 ,
	SEQUENCE_CHANGED = 0x20
};

enum DataUpdateType_t : int {
	DATA_UPDATE_CREATED = 0 ,
	DATA_UPDATE_DATATABLE_CHANGED ,
};

enum LifeStates_t : int {
	LIFE_ALIVE = 0 ,
	LIFE_DYING ,
	LIFE_DEAD ,
	LIFE_RESPAWNABLE ,
	LIFE_DISCARDBODY ,
};

enum PlayerFlags_t : int {
	FL_ONGROUND = ( 1 << 0 ) ,
	FL_DUCKING = ( 1 << 1 ) ,
	FL_WATERJUMP = ( 1 << 3 ) ,
	FL_ONTRAIN = ( 1 << 4 ) ,
	FL_INRAIN = ( 1 << 5 ) ,
	FL_FROZEN = ( 1 << 6 ) ,
	FL_ATCONTROLS = ( 1 << 7 ) ,
	FL_CLIENT = ( 1 << 8 ) ,
	FL_FAKECLIENT = ( 1 << 9 ) ,
	FL_INWATER = ( 1 << 10 ) ,
};

enum MoveType_t : int {
	MOVETYPE_NONE = 0 ,
	MOVETYPE_ISOMETRIC ,
	MOVETYPE_WALK ,
	MOVETYPE_STEP ,
	MOVETYPE_FLY ,
	MOVETYPE_FLYGRAVITY ,
	MOVETYPE_VPHYSICS ,
	MOVETYPE_PUSH ,
	MOVETYPE_NOCLIP ,
	MOVETYPE_LADDER ,
	MOVETYPE_OBSERVER ,
	MOVETYPE_CUSTOM ,
	MOVETYPE_LAST = MOVETYPE_CUSTOM ,
	MOVETYPE_MAX_BITS = 4 ,
};

enum Weapons_t : int
{
	DEAGLE = 1 ,
	ELITE = 2 ,
	FIVESEVEN = 3 ,
	GLOCK = 4 ,
	AK47 = 7 ,
	AUG = 8 ,
	AWP = 9 ,
	FAMAS = 10 ,
	G3SG1 = 11 ,
	GALIL = 13 ,
	M249 = 14 ,
	M4A4 = 16 ,
	MAC10 = 17 ,
	P90 = 19 ,
	ZONE_REPULSOR = 20 ,
	MP5_SD = 23 ,
	UMP45 = 24 ,
	XM1014 = 25 ,
	BIZON = 26 ,
	MAG7 = 27 ,
	NEGEV = 28 ,
	SAWEDOFF = 29 ,
	TEC9 = 30 ,
	ZEUS = 31 ,
	P2000 = 32 ,
	MP7 = 33 ,
	MP9 = 34 ,
	NOVA = 35 ,
	P250 = 36 ,
	SHIELD = 37 ,
	SCAR20 = 38 ,
	SG553 = 39 ,
	SSG08 = 40 ,
	KNIFEGG = 41 ,
	KNIFE_CT = 42 ,
	FLASHBANG = 43 ,
	HEGRENADE = 44 ,
	SMOKE = 45 ,
	MOLOTOV = 46 ,
	DECOY = 47 ,
	INCGRENADE = 48 ,
	C4 = 49 ,
	HEALTHSHOT = 57 ,
	KNIFE_T = 59 ,
	M4A1S = 60 ,
	USPS = 61 ,
	TRADEUPCONTRACT = 62 ,
	CZ75A = 63 ,
	REVOLVER = 64 ,
	TAGRENADE = 68 ,
	FISTS = 69 ,
	BREACHCHARGE = 70 ,
	TABLET = 72 ,
	MELEE = 74 ,
	AXE = 75 ,
	HAMMER = 76 ,
	SPANNER = 78 ,
	KNIFE_GHOST = 80 ,
	FIREBOMB = 81 ,
	DIVERSION = 82 ,
	FRAG_GRENADE = 83 ,
	SNOWBALL = 84 ,
	BUMPMINE = 85 ,
	KNIFE_BAYONET = 500 ,
	KNIFE_CSS = 503 ,
	KNIFE_FLIP = 505 ,
	KNIFE_GUT = 506 ,
	KNIFE_KARAMBIT = 507 ,
	KNIFE_M9_BAYONET = 508 ,
	KNIFE_HUNTSMAN = 509 ,
	KNIFE_FALCHION = 512 ,
	KNIFE_BOWIE = 514 ,
	KNIFE_BUTTERFLY = 515 ,
	KNIFE_SHADOW_DAGGERS = 516 ,
	KNIFE_CORD = 517 ,
	KNIFE_CANIS = 518 ,
	KNIFE_URSUS = 519 ,
	KNIFE_GYPSY_JACKKNIFE = 520 ,
	KNIFE_OUTDOOR = 521 ,
	KNIFE_STILETTO = 522 ,
	KNIFE_WIDOWMAKER = 523 ,
	KNIFE_SKELETON = 525 ,
	STUDDED_BROKENFANG_GLOVES = 4725 ,
	STUDDED_BLOODHOUND_GLOVES = 5027 ,
	T_GLOVES = 5028 ,
	CT_GLOVES = 5029 ,
	SPORTY_GLOVES = 5030 ,
	SLICK_GLOVES = 5031 ,
	LEATHER_HANDWRAPS = 5032 ,
	MOTORCYCLE_GLOVES = 5033 ,
	SPECIALIST_GLOVES = 5034 ,
	STUDDED_HYDRA_GLOVES = 5035 ,
};

struct RenderableInstance_t {
	uint8_t m_alpha;
	RenderableInstance_t( ) : m_alpha { 255ui8 } {}
};

class IHandleEntity {
public:
	virtual ~IHandleEntity( ) {}
	virtual void SetRefEHandle( const CBaseHandle& handle ) = 0;
	virtual const unsigned long& GetRefEHandle( ) const = 0;
};

class IClientRenderable;

class IClientUnknown : public IHandleEntity {
public:
	virtual void* GetCollideable( ) = 0;
	virtual void* GetClientNetworkable( ) = 0;
	virtual IClientRenderable* GetClientRenderable( ) = 0;
	virtual void* GetIClientEntity( ) = 0;
	virtual void* GetBaseEntity( ) = 0;
	virtual void* GetClientThinkable( ) = 0;
	virtual void* GetClientAlphaProperty( ) = 0;
};

class IClientRenderable
{
public:
	virtual IClientUnknown* GetIClientUnknown( ) = 0;
	virtual vec3_t const& GetRenderOrigin( void ) = 0;
	virtual vec3_t const& GetRenderAngles( void ) = 0;
	virtual bool                       ShouldDraw( void ) = 0;
	virtual int                        GetRenderFlags( void ) = 0; // ERENDERFLAGS_xxx
	virtual void                       Unused( void ) const {}
	virtual ClientShadowHandle_t       GetShadowHandle( ) const = 0;
	virtual ClientRenderHandle_t& RenderHandle( ) = 0;
	virtual const model_t* GetModel( ) const = 0;
	virtual int                        DrawModel( int flags , const int /*RenderableInstance_t*/& instance ) = 0;
	virtual int                        GetBody( ) = 0;
	virtual void                       GetColorModulation( float* color ) = 0;
	virtual bool                       LODTest( ) = 0;
	virtual bool                       SetupBones( matrix3x4_t* pBoneToWorldOut , int nMaxBones , int boneMask , float currentTime ) = 0;
	virtual void                       SetupWeights( const matrix3x4_t* pBoneToWorld , int nFlexWeightCount , float* pFlexWeights , float* pFlexDelayedWeights ) = 0;
	virtual void                       DoAnimationEvents( void ) = 0;
	virtual void* /*IPVSNotify*/       GetPVSNotifyInterface( ) = 0;
	virtual void                       GetRenderBounds( vec3_t& mins , vec3_t& maxs ) = 0;
	virtual void                       GetRenderBoundsWorldspace( vec3_t& mins , vec3_t& maxs ) = 0;
	virtual void                       GetShadowRenderBounds( vec3_t& mins , vec3_t& maxs , int /*ShadowType_t*/ shadowType ) = 0;
	virtual bool                       ShouldReceiveProjectedTextures( int flags ) = 0;
	virtual bool                       GetShadowCastDistance( float* pDist , int /*ShadowType_t*/ shadowType ) const = 0;
	virtual bool                       GetShadowCastDirection( vec3_t* pDirection , int /*ShadowType_t*/ shadowType ) const = 0;
	virtual bool                       IsShadowDirty( ) = 0;
	virtual void                       MarkShadowDirty( bool bDirty ) = 0;
	virtual IClientRenderable* GetShadowParent( ) = 0;
	virtual IClientRenderable* FirstShadowChild( ) = 0;
	virtual IClientRenderable* NextShadowPeer( ) = 0;
	virtual int /*ShadowType_t*/       ShadowCastType( ) = 0;
	virtual void                       CreateModelInstance( ) = 0;
	virtual ModelInstanceHandle_t      GetModelInstance( ) = 0;
	virtual const matrix3x4_t& RenderableToWorldTransform( ) = 0;
	virtual int                        LookupAttachment( const char* pAttachmentName ) = 0;
	virtual   bool                     GetAttachment( int number , vec3_t& origin , vec3_t& angles ) = 0;
	virtual bool                       GetAttachment( int number , matrix3x4_t& matrix ) = 0;
	virtual float* GetRenderClipPlane( void ) = 0;
	virtual int                        GetSkin( ) = 0;
	virtual void                       OnThreadedDrawSetup( ) = 0;
	virtual bool                       UsesFlexDelayedWeights( ) = 0;
	virtual void                       RecordToolMessage( ) = 0;
	virtual bool                       ShouldDrawForSplitScreenUser( int nSlot ) = 0;
	virtual uint8_t                    OverrideAlphaModulation( uint8_t nAlpha ) = 0;
	virtual uint8_t                    OverrideShadowAlphaModulation( uint8_t nAlpha ) = 0;
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

	POFFSET( IClientRenderable* , renderable , 0x4 );
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
		using SetAbsAngles_t = void( __thiscall* )( void* , const vec3_t& );
		patterns::SetAbsAngles.get< SetAbsAngles_t >( )( this , angles );
	}

	void SetAbsOrigin( const vec3_t& origin ) {
		using SetAbsOrigin_t = void( __thiscall* )( void* , const vec3_t& );
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

class Weapon;

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

	CUserCmd& m_PlayerCommand( ) {
		return get< CUserCmd >( 0x326C );
	}

	CUserCmd*& m_pCurrentCommand( ) {
		return get< CUserCmd* >( 0x3314 );
	}

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
		using BuildTransformations_t = void( __thiscall* )( void* , CStudioHdr* , vec3_t* , quaternion* , matrix3x4a_t const& , int , uint8_t* );
		return util::get_virtual_function< BuildTransformations_t >( this , 189 )( this , hdr , pos , q , transform , mask , computed );
	}

	void StandardBlendingRules( CStudioHdr* hdr , vec3_t* pos , quaternion* q , float time , int mask ) {
		using StandardBlendingRules_t = void( __thiscall* )( void* , CStudioHdr* , vec3_t* , quaternion* , float , int );
		return util::get_virtual_function< StandardBlendingRules_t >( this , 205 )( this , hdr , pos , q , time , mask );
	}

	float GetFOV( ) {
		return util::get_virtual_function< float( __thiscall* )( void* ) >( this , 326 )( this );
	}

	const vec3_t& WorldSpaceCenter( ) {
		return util::get_virtual_function< const vec3_t& ( __thiscall* )( void* ) >( this , 78 )( this );
	}

	void EyePosition( vec3_t* pos ) {
		util::get_virtual_function< float* ( __thiscall* )( Player* , vec3_t* ) >( this , 168 )( this , pos );
	}

	Weapon* GetActiveWeapon( );
	Entity* GetObserverTarget( );
	Entity* GetGroundEntity( );

	void SetAnimLayers( C_AnimationLayer* layers ) {
		std::memcpy( m_AnimOverlay( ) , layers , sizeof( C_AnimationLayer ) * 13 );
	}

	void GetAnimLayers( C_AnimationLayer* layers ) {
		std::memcpy( layers , m_AnimOverlay( ) , sizeof( C_AnimationLayer ) * 13 );
	}

	void SetPoseParameters( float* poses ) {
		std::memcpy( m_flPoseParameter( ) , poses , sizeof( float ) * 24 );
	}

	void GetPoseParameters( float* poses ) {
		std::memcpy( poses , m_flPoseParameter( ) , sizeof( float ) * 24 );
	}

	bool alive( ) {
		return m_lifeState( ) == LIFE_ALIVE;
	}

};

class WeaponInfo {
public:
	virtual ~WeaponInfo( ) = default;

	//char pad_0000[4]; //0x0000
	char* consoleName;				// 0x0004
	char		pad_0008 [ 8 ];				// 0x0008
	void* m_pWeaponDef;  //0x0010
	int			iMaxClip1;					// 0x0014
	int			iMaxClip2;					// 0x0018
	int			iDefaultClip1;				// 0x001C
	int			iDefaultClip2;				// 0x0020
	char		pad_0024 [ 8 ];				// 0x0024
	char* szWorldModel;				// 0x002C
	char* szViewModel;				// 0x0030
	char* szDroppedModel;				// 0x0034
	char		pad_0038 [ 4 ];				// 0x0038
	char* N0000023E;					// 0x003C
	char		pad_0040 [ 56 ];				// 0x0040
	char* szEmptySound;				// 0x0078
	char		pad_007C [ 4 ];				// 0x007C
	char* szBulletType;				// 0x0080
	char		pad_0084 [ 4 ];				// 0x0084
	char* szHudName;					// 0x0088
	char* szWeaponName;				// 0x008C
	char		pad_0090 [ 60 ];				// 0x0090
	int 		WeaponType;					// 0x00C8
	int			iWeaponPrice;				// 0x00CC
	int			iKillAward;					// 0x00D0
	char* szAnimationPrefix;			// 0x00D4
	float		flCycleTime;				// 0x00D8
	float		flCycleTimeAlt;				// 0x00DC
	float		flTimeToIdle;				// 0x00E0
	float		flIdleInterval;				// 0x00E4
	bool		bFullAuto;					// 0x00E8
	char		pad_0x00E5 [ 3 ];			// 0x00E9
	int			iDamage;					// 0x00EC
	float		flArmorRatio;				// 0x00F0
	int			iBullets;					// 0x00F4
	float		flPenetration;				// 0x00F8
	float		flFlinchVelocityModifierLarge;	// 0x00FC
	float		flFlinchVelocityModifierSmall;	// 0x0100
	float		flRange;					// 0x0104
	float		flRangeModifier;			// 0x0108
	float		flThrowVelocity;			// 0x010C
	char		pad_0x010C [ 16 ];			// 0x0110
	bool		bHasSilencer;				// 0x011C
	char		pad_0x0119 [ 3 ];			// 0x011D
	char* pSilencerModel;				// 0x0120
	int			iCrosshairMinDistance;		// 0x0124
	float		flMaxPlayerSpeed;	        // 0x0128
	float		flMaxPlayerSpeedAlt;		// 0x012C
	char		pad_0x0130 [ 4 ];		    // 0x0130
	float		flSpread;					// 0x0134
	float		flSpreadAlt;				// 0x0138
	float		flInaccuracyCrouch;			// 0x013C
	float		flInaccuracyCrouchAlt;		// 0x0140
	float		flInaccuracyStand;			// 0x0144
	float		flInaccuracyStandAlt;		// 0x0148
	float		flInaccuracyJumpInitial;	// 0x014C
	float		flInaccuracyJump;			// 0x0150
	float		flInaccuracyJumpAlt;		// 0x0154
	float		flInaccuracyLand;			// 0x0158
	float		flInaccuracyLandAlt;		// 0x015C
	float		flInaccuracyLadder;			// 0x0160
	float		flInaccuracyLadderAlt;		// 0x0164
	float		flInaccuracyFire;			// 0x0168
	float		flInaccuracyFireAlt;		// 0x016C
	float		flInaccuracyMove;			// 0x0170
	float		flInaccuracyMoveAlt;		// 0x0174
	float		flInaccuracyReload;			// 0x0178
	int			iRecoilSeed;				// 0x017C
	float		flRecoilAngle;				// 0x0180
	float		flRecoilAngleAlt;			// 0x0184
	float		flRecoilAngleVariance;		// 0x0188
	float		flRecoilAngleVarianceAlt;	// 0x018C
	float		flRecoilMagnitude;			// 0x0190
	float		flRecoilMagnitudeAlt;		// 0x0194
	float		flRecoilMagnitudeVariance;	// 0x0198
	float		flRecoilMagnitudeVarianceAlt;	// 0x019C
	float		flRecoveryTimeCrouch;		// 0x01A0
	float		flRecoveryTimeStand;		// 0x01A4
	float		flRecoveryTimeCrouchFinal;	// 0x01A8
	float		flRecoveryTimeStandFinal;	// 0x01AC
	int			iRecoveryTransitionStartBullet;	// 0x01B0 
	int			iRecoveryTransitionEndBullet;	// 0x01B4
	bool		bUnzoomAfterShot;			// 0x01B8
	bool		bHideViewModelZoomed;		// 0x01B9
	char		pad_0x01B5 [ 2 ];			// 0x01BA
	char		iZoomLevels [ 3 ];			// 0x01BC
	int			iZoomFOV [ 2 ];				// 0x01C0
	float		fZoomTime [ 3 ];				// 0x01C4
	char* szWeaponClass;				// 0x01D4
	float		flAddonScale;				// 0x01D8
	char		pad_0x01DC [ 4 ];			// 0x01DC
	char* szEjectBrassEffect;			// 0x01E0
	char* szTracerEffect;				// 0x01E4
	int			iTracerFrequency;			// 0x01E8
	int			iTracerFrequencyAlt;		// 0x01EC
	char* szMuzzleFlashEffect_1stPerson;	// 0x01F0
	char		pad_0x01F4 [ 4 ];				// 0x01F4
	char* szMuzzleFlashEffect_3rdPerson;	// 0x01F8
	char		pad_0x01FC [ 4 ];			// 0x01FC
	char* szMuzzleSmokeEffect;		// 0x0200
	float		flHeatPerShot;				// 0x0204
	char* szZoomInSound;				// 0x0208
	char* szZoomOutSound;				// 0x020C
	float		flInaccuracyPitchShift;		// 0x0210
	float		flInaccuracySoundThreshold;	// 0x0214
	float		flBotAudibleRange;			// 0x0218
	char		pad_0x0218 [ 8 ];			// 0x0220
	char* pWrongTeamMsg;				// 0x0224
	bool		bHasBurstMode;				// 0x0228
	char		pad_0x0225 [ 3 ];			// 0x0229
	bool		bIsRevolver;				// 0x022C
	bool		bCannotShootUnderwater;		// 0x0230
};

class Weapon : public Entity {
public:
	NETVAR( short , m_iItemDefinitionIndex , "DT_BaseAttributableItem" , "m_iItemDefinitionIndex" );
	NETVAR( int , m_iClip1 , "DT_BaseCombatWeapon" , "m_iClip1" );
	NETVAR( int , m_iPrimaryReserveAmmoCount , "DT_BaseCombatWeapon" , "m_iPrimaryReserveAmmoCount" );
	NETVAR( int , m_Activity , "CBaseCombatWeapon" , "m_Activity" );
	NETVAR( float , m_fFireDuration , "CBaseCombatWeapon" , "m_fFireDuration" );
	NETVAR( int , m_iBurstShotsRemaining , "CWeaponCSBaseGun" , "m_iBurstShotsRemaining" );
	NETVAR( float , m_fNextBurstShot , "CWeaponCSBaseGun" , "m_fNextBurstShot" );
	NETVAR( float , m_flNextPrimaryAttack , "DT_BaseCombatWeapon" , "m_flNextPrimaryAttack" );
	NETVAR( float , m_flNextSecondaryAttack , "DT_BaseCombatWeapon" , "m_flNextSecondaryAttack" );
	NETVAR( float , m_flThrowStrength , "DT_BaseCSGrenade" , "m_flThrowStrength" );
	NETVAR( bool , m_bPinPulled , "DT_BaseCSGrenade" , "m_bPinPulled" );
	NETVAR( float , m_fThrowTime , "DT_BaseCSGrenade" , "m_fThrowTime" );
	NETVAR( int , m_zoomLevel , "CWeaponCSBaseGun" , "m_zoomLevel" );
	NETVAR( float , m_flRecoilIndex , "DT_WeaponNegev" , "m_flRecoilIndex" );
	NETVAR( int , m_weaponMode , "CWeaponCSBase" , "m_weaponMode" );
	NETVAR( int , m_nFallbackPaintKit , "DT_BaseAttributableItem" , "m_nFallbackPaintKit" );
	NETVAR( int , m_nFallbackStatTrak , "DT_BaseAttributableItem" , "m_nFallbackStatTrak" );
	NETVAR( int , m_nFallbackSeed , "DT_BaseAttributableItem" , "m_nFallbackSeed" );
	NETVAR( int , m_flFallbackWear , "DT_BaseAttributableItem" , "m_flFallbackWear" );
	NETVAR( int , m_iAccountID , "DT_BaseCombatWeapon" , "m_iAccountID" );
	NETVAR( EHANDLE , m_hWeapon , "DT_PredictedViewModel" , "m_hWeapon" );
	NETVAR( EHANDLE , m_hWeaponWorldModel , "DT_PredictedViewModel" , "m_hWeaponWorldModel" );
	NETVAR( EHANDLE , m_hOwnerEntity , "DT_CSPlayer" , "m_hOwnerEntity" );
	NETVAR( float , m_flConstraintRadius , "DT_CSPlayer" , "m_flConstraintRadius" );
	NETVAR( float , m_fLastShotTime , "DT_WeaponCSBase" , "m_fLastShotTime" );

};
