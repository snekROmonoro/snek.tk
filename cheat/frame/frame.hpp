#pragma once
#include <cstdint>

namespace frame {
	inline uint64_t m_iLastRenderMs = 0;
	inline uint64_t m_iCurrRenderMs = 0;
	inline float m_flFrameTime = 0.f;

	void update( );
}
