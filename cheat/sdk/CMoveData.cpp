#include "CMoveData.hpp"

inline const vec3_t& CMoveData::GetAbsOrigin( ) const
{
	return _m_vecAbsOrigin;
}

// This is implemented with a more exhaustive test in gamemovement.cpp.  We check if the origin being requested is
//  inside solid, which it never should be
inline void CMoveData::SetAbsOrigin( const vec3_t& vec )
{
	_m_vecAbsOrigin = vec;
}
