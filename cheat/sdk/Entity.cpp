#include "Entity.hpp"
#include "../includes.hpp"

void animstate_pose_param_cache_t::SetValue( Player* pPlayer , float flValue )
{
	if ( !pPlayer ) {
		return;
	}

	// Not an initalized pose.
	if ( this->m_nIndex == -1 ) {
		return;
	}

	pPlayer->m_flPoseParameter( ) [ this->m_nIndex ] = flValue;
}
