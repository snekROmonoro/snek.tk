#pragma once
#include <string>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

namespace renderer {
	struct position_t {
		position_t( ) {
			this->x = this->y = 0;
		}

		template< typename T1 = int , typename T2 = int >
		position_t( T1 x , T2 y ) {
			this->x = static_cast< int >( x );
			this->y = static_cast< int >( y );
		}

		bool operator==( position_t b ) {
			return ( this->x == b.x && this->y == b.y );
		}

		bool operator!=( position_t b ) {
			return ( this->x != b.x || this->y != b.y );
		}

		position_t operator+( position_t b ) {
			return position_t( this->x + b.x , this->y + b.y );
		}

		position_t operator+=( position_t b ) {
			this->x += b.x;
			this->y += b.y;
			return *this;
		}

		position_t operator-( position_t b ) {
			return position_t( this->x - b.x , this->y - b.y );
		}

		position_t operator-=( position_t b ) {
			this->x -= b.x;
			this->y -= b.y;
			return *this;
		}

		position_t operator+( int n ) {
			return position_t( this->x + n , this->y + n );
		}

		position_t operator+=( int n ) {
			this->x += n;
			this->y += n;
			return *this;
		}

		position_t operator-( int n ) {
			return position_t( this->x - n , this->y - n );
		}

		position_t operator-=( int n ) {
			this->x -= n;
			this->y -= n;
			return *this;
		}

		position_t operator/( int n ) {
			return position_t( this->x / n , this->y / n );
		}

		position_t operator/=( int n ) {
			this->x /= n;
			this->y /= n;
			return *this;
		}

		position_t operator*( int n ) {
			return position_t( this->x * n , this->y * n );
		}

		position_t operator*=( int n ) {
			this->x *= n;
			this->y *= n;
			return *this;
		}

		position_t operator/( float n ) {
			return position_t(
				( int ) ( ( ( float ) this->x ) / n ) ,
				( int ) ( ( ( float ) this->y ) / n )
			);
		}

		position_t operator/=( float n ) {
			this->x = ( int ) ( ( ( float ) this->x ) / n );
			this->y = ( int ) ( ( ( float ) this->y ) / n );
			return *this;
		}

		position_t operator*( float n ) {
			return position_t(
				( int ) ( ( ( float ) this->x ) * n ) ,
				( int ) ( ( ( float ) this->y ) * n )
			);
		}

		position_t operator*=( float n ) {
			this->x = ( int ) ( ( ( float ) this->x ) * n );
			this->y = ( int ) ( ( ( float ) this->y ) * n );
			return *this;
		}

		int x , y;
	};

	struct dimensions_t {
		dimensions_t( ) {
			this->w = this->h = 0;
		}

		template< typename T1 = int , typename T2 = int >
		dimensions_t( T1 w , T2 h ) {
			this->w = static_cast< int >( w );
			this->h = static_cast< int >( h );
		}

		bool operator==( dimensions_t b ) {
			return ( this->w == b.w && this->h == b.h );
		}

		bool operator!=( dimensions_t b ) {
			return ( this->w != b.w || this->h != b.h );
		}

		dimensions_t operator+( dimensions_t b ) {
			return dimensions_t( this->w + b.w , this->h + b.h );
		}

		dimensions_t operator+=( dimensions_t b ) {
			this->w += b.w;
			this->h += b.h;
			return *this;
		}

		dimensions_t operator-( dimensions_t b ) {
			return dimensions_t( this->w - b.w , this->h - b.h );
		}

		dimensions_t operator-=( dimensions_t b ) {
			this->w -= b.w;
			this->h -= b.h;
			return *this;
		}

		dimensions_t operator/( int n ) {
			return dimensions_t( this->w / n , this->h / n );
		}

		dimensions_t operator/=( int n ) {
			this->w /= n;
			this->h /= n;
			return *this;
		}

		dimensions_t operator*( int n ) {
			return dimensions_t( this->w * n , this->h * n );
		}

		dimensions_t operator*=( int n ) {
			this->w *= n;
			this->h *= n;
			return *this;
		}

		dimensions_t operator/( float n ) {
			return dimensions_t(
				( int ) ( ( ( float ) this->w ) / n ) ,
				( int ) ( ( ( float ) this->h ) / n )
			);
		}

		dimensions_t operator/=( float n ) {
			this->w = ( int ) ( ( ( float ) this->w ) / n );
			this->h = ( int ) ( ( ( float ) this->h ) / n );
			return *this;
		}

		dimensions_t operator*( float n ) {
			return dimensions_t(
				( int ) ( ( ( float ) this->w ) * n ) ,
				( int ) ( ( ( float ) this->h ) * n )
			);
		}

		dimensions_t operator*=( float n ) {
			this->w = ( int ) ( ( ( float ) this->w ) * n );
			this->h = ( int ) ( ( ( float ) this->h ) * n );
			return *this;
		}

		int w , h;
	};

	struct rect_t {
		rect_t( ) {
			this->x = this->y = this->w = this->h = 0;
		}

		template< typename T = int >
		rect_t( T x , T y , T w , T h ) {
			this->x = static_cast< int >( x );
			this->y = static_cast< int >( y );
			this->w = static_cast< int >( w );
			this->h = static_cast< int >( h );
		}

		rect_t( position_t pos , dimensions_t dim ) {
			this->x = pos.x;
			this->y = pos.y;
			this->w = dim.w;
			this->h = dim.h;
		}

		bool operator==( rect_t b ) {
			return ( this->x == b.x && this->y == b.y
				&& this->w == b.w && this->h == b.h );
		}

		bool operator!=( rect_t b ) {
			return ( this->x != b.x || this->y != b.y
				|| this->w != b.w || this->h != b.h );
		}

		position_t get_pos( ) {
			return position_t( this->x , this->y );
		}

		dimensions_t get_dim( ) {
			return dimensions_t( this->w , this->h );
		}

		rect_t separate_in_rect( int pixels ) {
			return rect_t( this->x + pixels , this->y + pixels , this->w - ( pixels * 2 ) , this->h - ( pixels * 2 ) );
		}

		rect_t separate_out_rect( int pixels ) {
			return rect_t( this->x - pixels , this->y - pixels , this->w + ( pixels * 2 ) , this->h + ( pixels * 2 ) );
		}

		static rect_t separate_in_rect( rect_t rect , int pixels ) {
			return rect.separate_in_rect( pixels );
		}

		static rect_t separate_out_rect( rect_t rect , int pixels ) {
			return rect.separate_out_rect( pixels );
		}

		int x , y , w , h;
	};

	inline position_t operator+( position_t Pos , dimensions_t Dim ) {
		return position_t( Pos.x + Dim.w , Pos.y + Dim.h );
	}

	inline position_t operator-( position_t Pos , dimensions_t Dim ) {
		return position_t( Pos.x - Dim.w , Pos.y - Dim.h );
	}

	inline dimensions_t operator+( dimensions_t Dim , position_t DimAsPos ) {
		return dimensions_t( Dim.w + DimAsPos.y , Dim.h + DimAsPos.y );
	}

	inline dimensions_t operator-( dimensions_t Dim , position_t DimAsPos ) {
		return dimensions_t( Dim.w - DimAsPos.y , Dim.h - DimAsPos.y );
	}

	// https://cplusplus.com/forum/beginner/206462/#msg977025
	void rotate_point( int& x , int& y , float rotation );
	void rotate_point( position_t& point , float rotation );

	// https://cplusplus.com/forum/beginner/206462/#msg977025
	void rotate_triangle( position_t points [ 3 ] , float rotation );

	class font_t {
	public:
		ID3DXFont* m_pFont;
		std::string m_szFamily;
		int m_nHeight;
		int m_nWeight;
		bool m_bCreated;

		__forceinline font_t( ) : m_szFamily { } , m_pFont { } , m_nHeight { } , m_nWeight { } , m_bCreated { false } { }
		__forceinline font_t( const std::string& szFamily , const int& nHeight , const int& nWeight ) : m_szFamily { szFamily } , m_pFont { } , m_nHeight { nHeight } , m_nWeight { nWeight } , m_bCreated { false } { }

		// create font.
		void create( );
		void create( const std::string& szFamily , const int& nHeight , const int& nWeight );

		// draw text with font.
		void draw_text( int nX , int nY , const std::string& szText , DWORD col );
		void draw_text( position_t sPos , const std::string& szText , DWORD col );

		// draw bolded text with font.
		void draw_bolded_text( int nX , int nY , const std::string& szText , DWORD col );
		void draw_bolded_text( position_t sPos , const std::string& szText , DWORD col );

		// get text size.
		dimensions_t text_size( const std::string& szText );

		// release font.
		void release( );
	};

	inline DWORD to_d3d( int r , int g , int b , int a = 255 ) {
		return ( ( D3DCOLOR ) ( ( ( ( a ) & 0xff ) << 24 ) | ( ( ( r ) & 0xff ) << 16 ) | ( ( ( g ) & 0xff ) << 8 ) | ( ( b ) & 0xff ) ) );
	}

	enum fonts_t : uint32_t {
		FONT_DEFAULT = 0 ,
		FONT_HUD ,
		FONT_ESP ,
		FONT_ESP_HEALTH ,
		FONT_ESP_SMALL ,
		FONT_MAX
	};

	class c_renderer {
	private:
		struct RenderStatesBackup_t {
			IDirect3DStateBlock9* d3d9_state_block;
			bool setup = false;
		} s_RenderStatesBackup;

		IDirect3DDevice9* m_pDevice;
		bool m_bInitialized;
		D3DVIEWPORT9 m_OriginalViewport;
		std::vector< RECT > m_vecClipRectStack;
		std::vector< font_t > m_fonts;
	public:
		void init( IDirect3DDevice9* pDevice );

		void on_lost_device( );
		void on_reset_device( IDirect3DDevice9* pDevice );

		dimensions_t get_screen_size( ) {
			if ( !m_bInitialized )
				return dimensions_t( );

			return dimensions_t( m_OriginalViewport.Width , m_OriginalViewport.Height );
		}

		void setup_render_states( );
		void restore_render_states( );

		void draw_polygon( std::vector< position_t > Vertices , DWORD col , bool bFilled );
		void draw_polygon_multi_color( std::vector< position_t > Vertices , std::vector< DWORD > Colors , bool bFilled );

		void create_font( ID3DXFont** pFont , const std::string& szFamily , int nHeight , int nWeight );
		dimensions_t text_size( ID3DXFont* pFont , const std::string& szText );
		void draw_text( int nX , int nY , ID3DXFont* pFont , const std::string& szText , DWORD col );

		void draw_filled_rect( int nX , int nY , int nW , int nH , DWORD col );
		void draw_filled_rect( position_t sPos , dimensions_t sDim , DWORD col );
		void draw_filled_rect( rect_t sRect , DWORD col );

		void draw_outlined_rect( int nX , int nY , int nW , int nH , DWORD col );
		void draw_outlined_rect( position_t sPos , dimensions_t sDim , DWORD col );
		void draw_outlined_rect( rect_t sRect , DWORD col );

		void draw_multi_color_rect( int nX , int nY , int nW , int nH , DWORD col1 , DWORD col2 , DWORD col3 , DWORD col4 );
		void draw_multi_color_rect( position_t sPos , dimensions_t sDim , DWORD col1 , DWORD col2 , DWORD col3 , DWORD col4 );
		void draw_multi_color_rect( rect_t sRect , DWORD col1 , DWORD col2 , DWORD col3 , DWORD col4 );

		void draw_pos_line( int nX1 , int nY1 , int nX2 , int nY2 , DWORD col );
		void draw_pos_line( position_t sPos1 , position_t sPos2 , DWORD col );

		void draw_line( int nX , int nY , int nW , int nH , DWORD col );
		void draw_line( position_t sPos , dimensions_t sDim , DWORD col );
		void draw_line( rect_t sRect , DWORD col );

		void draw_filled_triangle( int nX1 , int nY1 , int nX2 , int nY2 , int nX3 , int nY3 , DWORD col );
		void draw_filled_triangle( position_t sPos1 , position_t sPos2 , position_t sPos3 , DWORD col );

		void draw_outlined_triangle( int nX1 , int nY1 , int nX2 , int nY2 , int nX3 , int nY3 , DWORD col );
		void draw_outlined_triangle( position_t sPos1 , position_t sPos2 , position_t sPos3 , DWORD col );

		void draw_filled_circle( int nX , int nY , float flRadius , DWORD col );
		void draw_filled_circle( position_t sPos , float flRadius , DWORD col );

		void draw_outlined_circle( int nX , int nY , float flRadius , DWORD col );
		void draw_outlined_circle( position_t sPos , float flRadius , DWORD col );

		RECT push_clip( int nX , int nY , int nW , int nH );
		RECT push_clip( position_t sPos , dimensions_t sDim );
		RECT push_clip( rect_t sRect );
		RECT pop_clip( );

		font_t get_font( uint32_t idx ) {
			if ( m_fonts.empty( ) )
				return font_t( );

			if ( idx < 0 || idx > m_fonts.size( ) )
				return font_t( );

			return m_fonts [ idx ];
		}
	};
}

extern renderer::c_renderer g_renderer;
