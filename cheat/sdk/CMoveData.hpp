#pragma once
#include "../math/Vector.hpp"
#include "basehandle.hpp"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/shared/igamemovement.h
#if 0
class CMoveData
{
public:
	bool			m_bFirstRunOfFunctions : 1;
	bool			m_bGameCodeMovedPlayer : 1;

	CBaseHandle	    m_nPlayerHandle;	// edict index on server, client entity handle on client

	int				m_nImpulseCommand;	// Impulse command issued.
	vec3_t			m_vecViewAngles;	// Command view angles (local space)
	vec3_t			m_vecAbsViewAngles;	// Command view angles (world space)
	int				m_nButtons;			// Attack buttons.
	int				m_nOldButtons;		// From host_client->oldbuttons;
	float			m_flForwardMove;
	float			m_flSideMove;
	float			m_flUpMove;

	float			m_flMaxSpeed;
	float			m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	vec3_t			m_vecVelocity;		// edict::velocity		// Current movement direction.
	vec3_t			m_vecAngles;		// edict::angles
	vec3_t			m_vecOldAngles;

	// Output only
	float			m_outStepHeight;	// how much you climbed this move
	vec3_t			m_outWishVel;		// This is where you tried 
	vec3_t			m_outJumpVel;		// This is your jump velocity

	// Movement constraints	(radius 0 means no constraint)
	vec3_t			m_vecConstraintCenter;
	float			m_flConstraintRadius;
	float			m_flConstraintWidth;
	float			m_flConstraintSpeedFactor;

	vec3_t			m_vecAbsOrigin;		// edict::origin
};
#endif

class CMoveData
{
	//public:
	//    char pad_0x00[183];
public:
	bool            m_bFirstRunOfFunctions : 1;
	bool            m_bGameCodeMovedPlayer : 1;
	bool            m_bNoAirControl : 1;

	CBaseHandle     m_nPlayerHandle;    // edict index on server, client entity handle on client

	int             m_nImpulseCommand;    // Impulse command issued.
	vec3_t          m_vecViewAngles;    // Command view angles (local space)
	vec3_t          m_vecAbsViewAngles;    // Command view angles (world space)
	int             m_nButtons;            // Attack buttons.
	int             m_nOldButtons;        // From host_client->oldbuttons;
	float            m_flForwardMove;
	float            m_flSideMove;
	float            m_flUpMove;

	float            _m_flMaxSpeed;
	float            m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	vec3_t            m_vecVelocity_;        // edict::velocity        // Current movement direction.
	vec3_t            m_vecOldVelocity;
	float            somefloat;
	vec3_t            m_vecAngles;        // edict::angles
	vec3_t            m_vecOldAngles;

	// Output only
	float            m_outStepHeight;    // how much you climbed this move
	vec3_t            m_outWishVel;        // This is where you tried 
	vec3_t            m_outJumpVel;        // This is your jump velocity

										   // Movement constraints    (radius 0 means no constraint)
	vec3_t            m_vecConstraintCenter;
	float            m_flConstraintRadius;
	float            m_flConstraintWidth;
	float            m_flConstraintSpeedFactor;
	bool            m_bConstraintPastRadius;        ///< If no, do no constraining past Radius.  If yes, cap them to SpeedFactor past radius

	void            SetAbsOrigin( const vec3_t& vec );
	const vec3_t& GetAbsOrigin( ) const;

public:
	vec3_t            _m_vecAbsOrigin;        // edict::origin

};

