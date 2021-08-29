#include "frame.hpp"
#include <chrono>

void frame::update( )
{
	m_iLastRenderMs = m_iCurrRenderMs;
	m_iCurrRenderMs = std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now( ).time_since_epoch( ) ).count( );
	uint64_t m_iDeltaMs = m_iCurrRenderMs - m_iLastRenderMs;
	if ( m_iDeltaMs < 10000 )
		m_flFrameTime = static_cast< float > ( m_iDeltaMs ) / 1000.0f;
}
