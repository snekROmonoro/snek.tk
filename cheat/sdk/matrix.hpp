#pragma once
#include "../math/Vector.hpp"

class matrix3x4_t {
public:
	matrix3x4_t( ) {}

	matrix3x4_t( float m00 , float m01 , float m02 , float m03 , float m10 , float m11 , float m12 , float m13 , float m20 , float m21 , float m22 , float m23 ) {
		m_flMatVal [ 0 ][ 0 ] = m00;
		m_flMatVal [ 0 ][ 1 ] = m01;
		m_flMatVal [ 0 ][ 2 ] = m02;
		m_flMatVal [ 0 ][ 3 ] = m03;
		m_flMatVal [ 1 ][ 0 ] = m10;
		m_flMatVal [ 1 ][ 1 ] = m11;
		m_flMatVal [ 1 ][ 2 ] = m12;
		m_flMatVal [ 1 ][ 3 ] = m13;
		m_flMatVal [ 2 ][ 0 ] = m20;
		m_flMatVal [ 2 ][ 1 ] = m21;
		m_flMatVal [ 2 ][ 2 ] = m22;
		m_flMatVal [ 2 ][ 3 ] = m23;
	}

	void Init( const vec3_t& x , const vec3_t& y , const vec3_t& z , const vec3_t& origin ) {
		m_flMatVal [ 0 ][ 0 ] = x.x;
		m_flMatVal [ 0 ][ 1 ] = y.x;
		m_flMatVal [ 0 ][ 2 ] = z.x;
		m_flMatVal [ 0 ][ 3 ] = origin.x;
		m_flMatVal [ 1 ][ 0 ] = x.y;
		m_flMatVal [ 1 ][ 1 ] = y.y;
		m_flMatVal [ 1 ][ 2 ] = z.y;
		m_flMatVal [ 1 ][ 3 ] = origin.y;
		m_flMatVal [ 2 ][ 0 ] = x.z;
		m_flMatVal [ 2 ][ 1 ] = y.z;
		m_flMatVal [ 2 ][ 2 ] = z.z;
		m_flMatVal [ 2 ][ 3 ] = origin.z;
	}

	matrix3x4_t( const vec3_t& x , const vec3_t& y , const vec3_t& z , const vec3_t& origin ) {
		m_flMatVal [ 0 ][ 0 ] = x.x;
		m_flMatVal [ 0 ][ 1 ] = y.x;
		m_flMatVal [ 0 ][ 2 ] = z.x;
		m_flMatVal [ 0 ][ 3 ] = origin.x;
		m_flMatVal [ 1 ][ 0 ] = x.y;
		m_flMatVal [ 1 ][ 1 ] = y.y;
		m_flMatVal [ 1 ][ 2 ] = z.y;
		m_flMatVal [ 1 ][ 3 ] = origin.y;
		m_flMatVal [ 2 ][ 0 ] = x.z;
		m_flMatVal [ 2 ][ 1 ] = y.z;
		m_flMatVal [ 2 ][ 2 ] = z.z;
		m_flMatVal [ 2 ][ 3 ] = origin.z;
	}

	void SetOrigin( const vec3_t& p ) {
		m_flMatVal [ 0 ][ 3 ] = p.x;
		m_flMatVal [ 1 ][ 3 ] = p.y;
		m_flMatVal [ 2 ][ 3 ] = p.z;
	}

	vec3_t GetOrigin( ) {
		return { m_flMatVal [ 0 ][ 3 ], m_flMatVal [ 1 ][ 3 ], m_flMatVal [ 2 ][ 3 ] };
	}

	float* operator[]( int i ) {
		return m_flMatVal [ i ];
	}

	const float* operator[]( int i ) const {
		return m_flMatVal [ i ];
	}

	float* Base( ) {
		return &m_flMatVal [ 0 ][ 0 ];
	}

	const float* Base( ) const {
		return &m_flMatVal [ 0 ][ 0 ];
	}

public:
	float m_flMatVal [ 3 ][ 4 ];
};

class __declspec( align( 16 ) ) matrix3x4a_t : public matrix3x4_t {
public:
	matrix3x4a_t& operator=( const matrix3x4_t& src ) {
		std::memcpy( Base( ) , src.Base( ) , sizeof( float ) * 3 * 4 );
		return *this;
	};
};

class BoneArray : public matrix3x4a_t {
public:
	bool get_bone( vec3_t& out , int bone = 0 ) {
		if ( bone < 0 || bone >= 128 )
			return false;

		matrix3x4_t* bone_matrix = &this [ bone ];

		if ( !bone_matrix )
			return false;

		out = { bone_matrix->m_flMatVal [ 0 ][ 3 ], bone_matrix->m_flMatVal [ 1 ][ 3 ], bone_matrix->m_flMatVal [ 2 ][ 3 ] };

		return true;
	}
};

class VMatrix {
public:
	float* operator[]( int i ) {
		return m [ i ];
	}
	const float* operator[]( int i ) const {
		return m [ i ];
	}

	float* Base( ) {
		return &m [ 0 ][ 0 ];
	}
	const float* Base( ) const {
		return &m [ 0 ][ 0 ];
	}

public:
	float m [ 4 ][ 4 ];
};
