#pragma once
#include "../sdk/include.hpp"

namespace features {
	namespace prediction {
		void update( );
		void predict( );
		void restore( );

		void correct_viewmodel_data( );
		void update_viewmodel_data( );

		namespace m_stored_variables {
			inline float m_frametime;
			inline float m_curtime;

			inline int m_tickbase;
			inline float m_velocity_modifier;
			inline vec3_t m_velocity;
			inline vec3_t m_origin;
			inline int m_flags;

			inline bool m_bInPrediction , m_bFirstTimePredicted;
		}

		namespace m_stored_viewmodel_variables {
			inline float m_viewmodel_cycle;
			inline float m_viewmodel_anim_time;
		}
	}
}