#pragma once
#include <cmath>
#include <algorithm>

#define FastSqrt(x)	(sqrt)(x)

typedef float vec_t;
struct vec2_t;
struct vec3_t;

struct vec2_t {
	vec_t x , y;
	vec2_t( void ) {
		this->x = this->y = 0;
	}

	vec2_t( vec_t x , vec_t y ) {
		this->x = x;
		this->y = y;
	}

	bool is_valid( ) {
		return std::isfinite( this->x ) && this->x > 1.f && std::isfinite( this->y ) && this->y > 1.f;
	}

	bool is_zero( ) {
		return vec2_t( this->x , this->y ) == vec2_t( 0.0f , 0.0f );
	}

	float length( ) const {
		return std::sqrtf( this->x * this->x + this->y * this->y );
	}

	float dist_to( const vec2_t& vec ) const {
		vec2_t delta;
		delta.x = this->x - vec.x;
		delta.y = this->y - vec.y;

		return delta.length( );
	}

	vec2_t normalized( ) const {
		auto res = *this;
		auto l = res.length( );

		if ( l != 0.0f )
			res /= l;
		else
			res.x = res.y = 0.0f;

		return res;
	}

	/* operators */
	bool operator==( const vec2_t& src ) const {
		return ( src.x == this->x ) && ( src.y == this->y );
	}

	bool operator!=( const vec2_t& src ) const {
		return ( src.x != this->x ) || ( src.y != this->y );
	}

	float& operator[]( int i ) {
		return ( ( float* ) this ) [ i ];
	}

	float operator[]( int i ) const {
		return ( ( float* ) this ) [ i ];
	}

	vec2_t& operator+=( const vec2_t& v ) {
		this->x += v.x; this->y += v.y;
		return *this;
	}

	vec2_t& operator-=( const vec2_t& v ) {
		this->x -= v.x; this->y -= v.y;
		return *this;
	}

	vec2_t& operator*=( float fl ) {
		this->x *= fl;
		this->y *= fl;

		return *this;
	}

	vec2_t& operator*=( const vec2_t& v ) {
		this->x *= v.x;
		this->y *= v.y;

		return *this;
	}

	vec2_t& operator/=( const vec2_t& v ) {
		this->x /= v.x;
		this->y /= v.y;

		return *this;
	}

	vec2_t& operator+=( float fl ) {
		this->x += fl;
		this->y += fl;

		return *this;
	}

	vec2_t& operator/=( float fl ) {
		this->x /= fl;
		this->y /= fl;

		return *this;
	}

	vec2_t& operator-=( float fl ) {
		this->x -= fl;
		this->y -= fl;

		return *this;
	}

	vec2_t& operator+( vec2_t v ) {
		vec2_t temp;
		temp.x = this->x + v.x;
		temp.y = this->y + v.y;

		return temp;
	}

	vec2_t& operator*( vec2_t v ) {
		vec2_t temp;
		temp.x = this->x * v.x;
		temp.y = this->y * v.y;

		return temp;
	}

	vec2_t& operator*( float v ) {
		vec2_t temp;
		temp.x = this->x * v;
		temp.y = this->y * v;

		return temp;
	}

	vec2_t& operator-( vec2_t v ) {
		vec2_t temp;
		temp.x = this->x - v.x;
		temp.y = this->y - v.y;

		return temp;
	}

	vec2_t& operator/( vec2_t v ) {
		vec2_t temp;
		temp.x = this->x / v.x;
		temp.y = this->y / v.y;

		return temp;
	}

	vec2_t& operator/( float v ) {
		vec2_t temp;
		temp.x = this->x / v;
		temp.y = this->y / v;

		return temp;
	}

};

struct vec3_t {
	vec_t x , y , z;

	vec3_t( void ) {
		this->x = this->y = this->z = 0;
	}

	/*vec3_t( vec_t x , vec_t y , vec_t z ) {
		this->x = x;
		this->y = y;
		this->z = z;
	}*/

	template< typename T1 = vec_t , typename T2 = vec_t , typename T3 = vec_t >
	vec3_t( T1 x , T2 y , T3 z ) {
		this->x = static_cast< vec_t >( x );
		this->y = static_cast< vec_t >( y );
		this->z = static_cast< vec_t >( z );
	}

	vec3_t( vec_t v [ 3 ] ) {
		this->x = v [ 0 ];
		this->y = v [ 1 ];
		this->z = v [ 2 ];
	}

	void init( vec_t x = 0.f , vec_t y = 0.f , vec_t z = 0.f ) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void zero( ) {
		this->x = 0.f;
		this->y = 0.f;
		this->z = 0.f;
	}

	vec2_t to_2d( ) {
		return vec2_t( this->x , this->y );
	}

	bool is_valid( ) {
		return std::isfinite( this->x ) && std::isfinite( this->y ) && std::isfinite( this->z );
	}

	bool is_zero( ) {
		return vec3_t( this->x , this->y , this->z ) == vec3_t( 0.0f , 0.0f , 0.0f );
	}

	float length( void ) const {
		float root = 0.0f;
		float sqsr = x * x + y * y + z * z;
		root = sqrt( sqsr );
		return root;
	}

	vec3_t to_vectors( vec3_t* side = nullptr , vec3_t* up = nullptr );

	vec3_t clamp( ) {

		if ( this->x < -89.0f )
			this->x = -89.0f;

		if ( this->x > 89.0f )
			this->x = 89.0f;

		while ( this->y < -180.0f )
			this->y += 360.0f;

		while ( this->y > 180.0f )
			this->y -= 360.0f;

		this->z = 0.0f;

		return *this;
	}

	vec3_t clamped( ) {
		vec3_t clamped = *this;
		clamped.clamp( );
		return clamped;
	}

	float length_2d( void ) const {
		float root = 0.0f;
		float sqst = x * x + y * y;
		root = sqrt( sqst );
		return root;
	}

	float length_sqr( void ) const {
		return ( x * x + y * y + z * z );
	}

	float length_2d_sqr( void ) const {
		return ( x * x + y * y );
	}

	vec3_t normalize( ) {
		vec3_t vector;
		float length = this->length( );

		if ( length != 0 )
		{
			vector.x = x / length;
			vector.y = y / length;
			vector.z = z / length;
		}
		else
		{
			vector.x = vector.y = 0.0f; vector.z = 1.0f;
		}

		return vector;
	}

	float normalize_place( ) {
		float radius = FastSqrt( x * x + y * y + z * z );

		/* FLT_EPSILON is added to the radius to eliminate the possibility of divide by zero. */
		float iradius = 1.f / ( radius + FLT_EPSILON );

		x *= iradius;
		y *= iradius;
		z *= iradius;

		return radius;
	}

	vec3_t normalized( ) {
		vec3_t vec = *this;
		vec.normalize_place( );
		return vec;
	}

	float dist_to( const vec3_t& other ) const {
		vec3_t delta;

		delta.x = x - other.x;
		delta.y = y - other.y;
		delta.z = z - other.z;

		return delta.length( );
	}

	float dist_to_sqr( const vec3_t& other ) const {
		vec3_t delta;

		delta.x = x - other.x;
		delta.y = y - other.y;
		delta.z = z - other.z;

		return delta.length_sqr( );
	}

	float dot( const vec3_t& other ) const {
		const vec3_t& a = *this;
		return( a.x * other.x + a.y * other.y + a.z * other.z );
	}

	vec3_t cross( const vec3_t& vec ) const {
		return vec3_t( this->y * vec.z - this->z * vec.y , this->z * vec.x - this->x * vec.z , this->x * vec.y - this->y * vec.x );
	}

	void multiply_add( const vec3_t& a , const vec3_t& b , float scalar ) {
		x = a.x + b.x * scalar;
		y = a.y + b.y * scalar;
		z = a.z + b.z * scalar;
	}

	float* base( ) {
		return ( float* ) this;
	}

	float const* base( ) const {
		return ( float const* ) this;
	}

	/* operators */
	float operator[]( int i ) const {
		return ( ( float* ) this ) [ i ];
	}

	float& operator[]( int i ) {
		return ( ( float* ) this ) [ i ];
	}

	bool operator==( const vec3_t& src ) const {
		return ( src.x == x ) && ( src.y == y ) && ( src.z == z );
	}

	bool operator!=( const vec3_t& src ) const {
		return ( src.x != x ) || ( src.y != y ) || ( src.z != z );
	}

	vec3_t& operator=( const vec3_t& vec ) {
		this->x = vec.x; this->y = vec.y; this->z = vec.z;

		return *this;
	}

	vec3_t operator-( ) const {
		return vec3_t( -this->x , -this->y , -this->z );
	}

	vec3_t operator+( const vec3_t& v ) const {
		return vec3_t( this->x + v.x , this->y + v.y , this->z + v.z );
	}

	vec3_t operator-( const vec3_t& v ) const {
		return vec3_t( this->x - v.x , this->y - v.y , this->z - v.z );
	}

	vec3_t operator*( float fl ) const {
		return vec3_t( this->x * fl , this->y * fl , this->z * fl );
	}

	vec3_t operator*( const vec3_t& v ) const {
		return vec3_t( this->x * v.x , this->y * v.y , this->z * v.z );
	}

	vec3_t operator/( float fl ) const {
		return vec3_t( this->x / fl , this->y / fl , this->z / fl );
	}

	vec3_t operator/( const vec3_t& v ) const {
		return vec3_t( this->x / v.x , this->y / v.y , this->z / v.z );
	}

	vec3_t& operator+=( const vec3_t& v ) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	}

	vec3_t& operator-=( const vec3_t& v ) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		return *this;
	}

	vec3_t& operator*=( float fl ) {
		this->x *= fl;
		this->y *= fl;
		this->z *= fl;
		return *this;
	}

	vec3_t& operator*=( const vec3_t& v ) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;
		return *this;
	}

	vec3_t& operator+=( float fl ) {
		this->x += fl;
		this->y += fl;
		this->z += fl;
		return *this;
	}

	vec3_t operator+( float fl )
	{
		return vec3_t( this->x + fl , this->y + fl , this->z + fl );
	}

	vec3_t& operator-=( float fl )
	{
		this->x -= fl;
		this->y -= fl;
		this->z -= fl;
		return *this;
	}

	vec3_t operator-( float fl )
	{
		return vec3_t( this->x - fl , this->y - fl , this->z - fl );
	}

	vec3_t& operator/=( float fl ) {
		float oofl = 1.0f / fl;
		this->x *= oofl;
		this->y *= oofl;
		this->z *= oofl;
		return *this;
	}

	vec3_t operator/( float fl )
	{
		return vec3_t( this->x / fl , this->y / fl , this->z / fl );
	}

	vec3_t& operator/=( const vec3_t& v )
	{
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;
		return *this;
	}

};

__forceinline vec3_t operator*( float lhs , const vec3_t& rhs ) {
	return rhs * lhs;
}

__forceinline vec3_t operator/( float lhs , const vec3_t& rhs ) {
	return rhs / lhs;
}

class quaternion
{
public:
	inline quaternion( void ) {}
	inline quaternion( float ix , float iy , float iz , float iw ) : x( ix ) , y( iy ) , z( iz ) , w( iw ) {}

	inline void init( float ix = 0.0f , float iy = 0.0f , float iz = 0.0f , float iw = 0.0f ) { x = ix; y = iy; z = iz; w = iw; }

	float* base( ) { return ( float* ) this; }
	const float* base( ) const { return ( float* ) this; }

	float x , y , z , w;
};

class __declspec( align( 16 ) ) vec_aligned_t : public vec3_t
{

public:

	inline vec_aligned_t( void ) {};
	inline vec_aligned_t( float x , float y , float z )
	{
		init( x , y , z );
	}

public:

	explicit vec_aligned_t( const vec3_t& other )
	{
		init( other.x , other.y , other.z );
	}

	vec_aligned_t& operator=( const vec3_t& other )
	{
		init( other.x , other.y , other.z );
		return *this;
	}

	vec_aligned_t& operator=( const vec_aligned_t& other )
	{
		init( other.x , other.y , other.z );
		return *this;
	}

	float w;
};

