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

Weapon* Player::GetActiveWeapon( )
{
	return sdk::interfaces::entity_list->GetClientEntityFromHandle< Weapon* >( m_hActiveWeapon( ) );
}

Entity* Player::GetObserverTarget( ) {
	return sdk::interfaces::entity_list->GetClientEntityFromHandle( m_hObserverTarget( ) );
}

Entity* Player::GetGroundEntity( ) {
	return sdk::interfaces::entity_list->GetClientEntityFromHandle( m_hGroundEntity( ) );
}

