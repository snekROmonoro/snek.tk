#pragma once

#define TICK_INTERVAL			(sdk::interfaces::global_vars->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )

#include "basehandle.hpp"
#include "CGameMovement.hpp"
#include "CGlobalVarsBase.hpp"
#include "checksum_crc.hpp"
#include "ClientClass.hpp"
#include "CMoveData.hpp"
#include "CUserCmd.hpp"
#include "dt_recv.hpp"
#include "IBaseClientDLL.hpp"
#include "IClientEntityList.hpp"
#include "IClientMode.hpp"
#include "ICvar.hpp"
#include "IEngineTrace.hpp"
#include "IGameEvent.hpp"
#include "IMemAlloc.hpp"
#include "input.hpp"
#include "IPrediction.hpp"
#include "ISurface.hpp"
#include "IVDebugOverlay.hpp"
#include "IVEngineClient.hpp"
#include "matrix.hpp"
#include "studio.hpp"
#include "studiorender.hpp"
#include "UtlVector.hpp"

#include "netvar/netvar.hpp"
#include "interfaces/interfaces.hpp"

#include "Entity.hpp"
