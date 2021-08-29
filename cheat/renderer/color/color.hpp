#pragma once
#include <d3dx9.h>
#include <algorithm>

#define D3DColorExtractAlpha(x) (int)((x>>24)&0xff)
#define D3DColorExtractRed(x) (int)((x>>16)&0xff)
#define D3DColorExtractGreen(x) (int)((x>>8)&0xff)
#define D3DColorExtractBlue(x) (int)(x&0xff)

class color {
public:
	struct Hsv_t {
		float Hue , Saturation , Value;
	};

	int r , g , b , a;
	color( ) : r { 255 } , g { 255 } , b { 255 } , a { 255 } { }
	color( int r , int g , int b , int a = 255 ) : r { std::clamp( r , 0 , 255 ) } , g { std::clamp( g , 0 , 255 ) } , b { std::clamp( b , 0 , 255 ) } , a { std::clamp( a , 0 , 255 ) } { }

	// Returns the same color with a different alpha
	color override_alpha( int alpha = 255 ) {
		return color( r , g , b , std::clamp( alpha , 0 , 255 ) );
	}

	color clamped( ) {
		return color(
			std::clamp( r , 0 , 255 ) ,
			std::clamp( g , 0 , 255 ) ,
			std::clamp( b , 0 , 255 ) ,
			std::clamp( a , 0 , 255 )
		);
	}

	DWORD to_d3d( ) {
		return ( ( DWORD ) ( ( ( ( this->a ) & 0xff ) << 24 ) | ( ( ( this->r ) & 0xff ) << 16 ) | ( ( ( this->g ) & 0xff ) << 8 ) | ( ( this->b ) & 0xff ) ) );
	}

	static color lerp( color a , color b , float t , bool bAlpha = true ) {
		auto _lerp = [ ] ( int a , int b , float t ) {
			return ( int ) ( a + ( int ) ( ( float ) ( b - a ) * std::clamp( t , 0.f , 1.f ) ) );
		};

		return color(
			_lerp( a.r , b.r , t ) ,
			_lerp( a.g , b.g , t ) ,
			_lerp( a.b , b.b , t ) ,
			bAlpha ? _lerp( a.a , b.a , t ) : a.a
		);
	}

	color lerp( color b , float t , bool bAlpha ) {
		return color::lerp( *this , b , t , bAlpha );
	}

	static color HSVtoRGB( float hue , float sat , float val ) {
		float red = 0.f , grn = 0.f , blu = 0.f;
		float i , f , p , q , t;
		color result;

		if ( val == 0 ) {
			red = 0.f;
			grn = 0.f;
			blu = 0.f;
		}
		else {
			hue /= 60.f;
			i = floor( hue );
			f = hue - i;
			p = val * ( 1.f - sat );
			q = val * ( 1.f - ( sat * f ) );
			t = val * ( 1.f - ( sat * ( 1.f - f ) ) );
			if ( i == 0.f ) {
				red = val;
				grn = t;
				blu = p;
			}
			else if ( i == 1.f ) {
				red = q;
				grn = val;
				blu = p;
			}
			else if ( i == 2.f ) {
				red = p;
				grn = val;
				blu = t;
			}
			else if ( i == 3.f ) {
				red = p;
				grn = q;
				blu = val;
			}
			else if ( i == 4.f ) {
				red = t;
				grn = p;
				blu = val;
			}
			else if ( i == 5.f ) {
				red = val;
				grn = p;
				blu = q;
			}
		}

		return color( ( int ) ( red * 255.f ) , ( int ) ( grn * 255.f ) , ( int ) ( blu * 255.f ) );
	}

	static Hsv_t RGBtoHSV( color a ) {
		float hue , sat , val;
		float x , f , i;
		Hsv_t result;

		float r = 0.f , g = 0.f , b = 0.f;
		r = ( float ) a.r / 255.f;
		g = ( float ) a.g / 255.f;
		b = ( float ) a.b / 255.f;

		x = std::min< float >( std::min< float >( r , g ) , b );
		val = std::max< float >( std::max< float >( r , g ) , b );
		if ( x == val ) {
			hue = 0.f;
			sat = 0.f;
		}
		else {
			f = ( r == x ) ? g - b : ( ( g == x ) ? b - r : r - g );
			i = ( r == x ) ? 3.f : ( ( g == x ) ? 5.f : 1.f );
			hue = fmodf( ( i - f / ( val - x ) ) * 60.f , 360.f );
			sat = ( ( val - x ) / val );
		}

		result.Hue = hue;
		result.Saturation = sat;
		result.Value = val;

		return result;
	}

	Hsv_t RGBtoHSV( ) {
		return color::RGBtoHSV( *this );
	}

	static color black( int Alpha = 255 )
	{
		return color( 0 , 0 , 0 ).override_alpha( Alpha );
	}

	static color white( int Alpha = 255 )
	{
		return color( 255 , 255 , 255 ).override_alpha( Alpha );
	}

	static color grey( int Alpha = 255 )
	{
		return color( 127 , 127 , 127 ).override_alpha( Alpha );
	}

	static color dark_grey( int Alpha = 255 )
	{
		return color( 27 , 27 , 27 ).override_alpha( Alpha );
	}

	static color light_grey( int Alpha = 255 )
	{
		return color( 211 , 211 , 211 ).override_alpha( Alpha );
	}

	static color red( int Alpha = 255 )
	{
		return color( 255 , 0 , 0 ).override_alpha( Alpha );
	}

	static color green( int Alpha = 255 )
	{
		return color( 0 , 255 , 0 ).override_alpha( Alpha );
	}

	static color yellow( int Alpha = 255 )
	{
		return color( 255 , 255 , 0 ).override_alpha( Alpha );
	}

	static color blue( int Alpha = 255 )
	{
		return color( 0 , 0 , 255 ).override_alpha( Alpha );
	}

	static color light_blue( int Alpha = 255 )
	{
		return color( 173 , 216 , 230 ).override_alpha( Alpha );
	}

	static color dark_blue( int Alpha = 255 )
	{
		return color( 0 , 0 , 139 ).override_alpha( Alpha );
	}

	static color light_sky_blue( int Alpha = 255 )
	{
		return color( 135 , 206 , 250 ).override_alpha( Alpha );
	}

	static color sky_blue( int Alpha = 255 )
	{
		return color( 135 , 206 , 235 ).override_alpha( Alpha );
	}

	static color deep_sky_blue( int Alpha = 255 )
	{
		return color( 0 , 191 , 255 ).override_alpha( Alpha );
	}

	static color pink( int Alpha = 255 ) {
		return color( 255 , 192 , 203 ).override_alpha( Alpha );
	}

	static color light_pink( int Alpha = 255 ) {
		return color( 255 , 182 , 193 ).override_alpha( Alpha );
	}

	static color hot_pink( int Alpha = 255 ) {
		return color( 255 , 105 , 180 ).override_alpha( Alpha );
	}
};

inline bool operator==( color a , color b ) {
	return (
		a.r == b.r &&
		a.g == b.g &&
		a.b == b.b &&
		a.a == b.a
		);
}

inline bool operator!=( color a , color b ) {
	return (
		a.r != b.r ||
		a.g != b.g ||
		a.b != b.b ||
		a.a != b.a
		);
}
