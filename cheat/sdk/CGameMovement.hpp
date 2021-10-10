#pragma once
#include "../util/vfunc/vfunc.hpp"
#include "../math/Vector.hpp"

class Entity;
class CMoveData;

class IGameMovement
{
public:
	virtual			~IGameMovement( void ) {}

	virtual void	          ProcessMovement( Entity* pPlayer , CMoveData* pMove ) = 0;
	virtual void	          Reset( void ) = 0;
	virtual void	          StartTrackPredictionErrors( Entity* pPlayer ) = 0;
	virtual void	          FinishTrackPredictionErrors( Entity* pPlayer ) = 0;
	virtual void	          DiffPrint( char const* fmt , ... ) = 0;
	virtual vec3_t const& GetPlayerMins( bool ducked ) const = 0;
	virtual vec3_t const& GetPlayerMaxs( bool ducked ) const = 0;
	virtual vec3_t const& GetPlayerViewOffset( bool ducked ) const = 0;
	virtual bool		        IsMovingPlayerStuck( void ) const = 0;
	virtual Entity* GetMovingPlayer( void ) const = 0;
	virtual void		        UnblockPusher( Entity* pPlayer , Entity* pPusher ) = 0;
	virtual void            SetupMovementBounds( CMoveData* pMove ) = 0;
};

class CGameMovement
	: public IGameMovement
{
public:
	virtual ~CGameMovement( void ) {}
};
