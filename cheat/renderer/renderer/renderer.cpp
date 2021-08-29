#include "renderer.hpp"
#include "../../includes.hpp"

struct Vertex_t {
	float x , y , z , rhw;
	DWORD color;
};

void renderer::rotate_point( int& x , int& y , float rotation )
{
	auto temp_x = x;
	auto temp_y = y;

	float theta = math::deg2rad( rotation );
	float c = cos( theta );
	float s = sin( theta );

	x = ( int ) ( ( ( float ) temp_x * c ) - ( ( float ) temp_y * s ) );
	y = ( int ) ( ( ( float ) temp_x * s ) + ( ( float ) temp_y * c ) );
}

void renderer::rotate_point( position_t& point , float rotation )
{
	return rotate_point( point.x , point.y , rotation );
}

void renderer::rotate_triangle( position_t points [ 3 ] , float rotation )
{
	position_t geometric_center = ( points [ 0 ] + points [ 1 ] + points [ 2 ] ) / 3;

	for ( int i = 0; i < 3; i++ ) {
		points [ i ] -= geometric_center;
		rotate_point( points [ i ] , rotation );
		points [ i ] += geometric_center;
	}

	/*const auto points_center = ( points [ 0 ] + points [ 1 ] + points [ 2 ] ) / 3;

	for ( int i = 0; i < 3; i++ )
	{
		auto& point = points [ i ];
		point -= points_center;

		const auto temp_x = point.x;
		const auto temp_y = point.y;

		const auto theta = math::deg2rad( rotation );
		const auto c = cosf( theta );
		const auto s = sinf( theta );

		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;

		point += points_center;
	}*/
}

using namespace renderer;
c_renderer g_renderer { };

void font_t::create( )
{
	if ( m_bCreated ) {
		return;
	}

	g_renderer.create_font( &this->m_pFont , this->m_szFamily , this->m_nHeight , this->m_nWeight );
	m_bCreated = this->m_pFont != nullptr;
}

void font_t::create( const std::string& szFamily , const int& nHeight , const int& nWeight )
{
	if ( m_bCreated ) {
		return;
	}

	this->m_szFamily = szFamily;
	this->m_nHeight = nHeight;
	this->m_nWeight = nWeight;
	this->create( );
}

void font_t::draw_text( int nX , int nY , const std::string& szText , DWORD col )
{
	if ( !m_bCreated ) {
		return;
	}

	if ( !szText.length( ) ) {
		return;
	}

	g_renderer.draw_text( nX , nY , this->m_pFont , szText , col );
}

void font_t::draw_text( position_t sPos , const std::string& szText , DWORD col )
{
	if ( !m_bCreated ) {
		return;
	}

	if ( !szText.length( ) ) {
		return;
	}

	draw_text( sPos.x , sPos.y , szText , col );
}

void font_t::draw_bolded_text( int nX , int nY , const std::string& szText , DWORD col )
{
	if ( !m_bCreated ) {
		return;
	}

	if ( !szText.length( ) ) {
		return;
	}

	draw_text( nX - 1 , nY + 1 , szText , col );
	draw_text( nX - 0 , nY + 1 , szText , col );
	draw_text( nX + 1 , nY + 1 , szText , col );

	draw_text( nX - 1 , nY + 0 , szText , col );
	draw_text( nX - 0 , nY + 0 , szText , col );
	draw_text( nX + 1 , nY + 0 , szText , col );

	draw_text( nX - 1 , nY - 1 , szText , col );
	draw_text( nX - 0 , nY - 1 , szText , col );
	draw_text( nX + 1 , nY - 1 , szText , col );
}

void font_t::draw_bolded_text( position_t sPos , const std::string& szText , DWORD col )
{
	if ( !m_bCreated ) {
		return;
	}

	if ( !szText.length( ) ) {
		return;
	}

	draw_bolded_text( sPos.x , sPos.y , szText , col );
}

dimensions_t font_t::text_size( const std::string& szText )
{
	if ( !m_bCreated ) {
		return dimensions_t( );
	}

	if ( !szText.length( ) ) {
		return dimensions_t( 0 , this->m_nHeight );
	}

	return g_renderer.text_size( this->m_pFont , szText );
}

void font_t::release( )
{
	if ( !m_bCreated ) {
		return;
	}

	this->m_pFont->Release( );
	this->m_pFont = nullptr;
	m_bCreated = false;
}

void c_renderer::init( IDirect3DDevice9* pDevice )
{
	if ( m_bInitialized )
		return;

	m_pDevice = pDevice;
	m_bInitialized = true;

	m_pDevice->GetViewport( &m_OriginalViewport );

	// do fonts..
	m_fonts.resize( FONT_MAX );
	m_fonts [ FONT_DEFAULT ].create( "Tahoma" , 16 , FW_DONTCARE );
	m_fonts [ FONT_HUD ].create(  "Tahoma"  , 12 , FW_NORMAL );
	m_fonts [ FONT_ESP ].create(  "Calibri"  , 13 , FW_NORMAL );
	m_fonts [ FONT_ESP_HEALTH ].create(  "Tahoma"  , 10 , FW_LIGHT );
	m_fonts [ FONT_ESP_SMALL ].create(  "Calibri"  , 12 , FW_LIGHT );
}

void c_renderer::on_lost_device( )
{
	// release fonts..
	for ( auto& font : m_fonts ) {
		font.release( );
	}

	m_bInitialized = false;
	m_pDevice = nullptr;
}

void c_renderer::on_reset_device( IDirect3DDevice9* pDevice )
{
	// Make sure we re-initialize
	m_bInitialized = false;
	init( pDevice );
}

void c_renderer::setup_render_states( )
{
	if ( !m_bInitialized ) {
		// maybe we're resetting smh?
		this->restore_render_states( );
		s_RenderStatesBackup.setup = false;
		s_RenderStatesBackup.d3d9_state_block = NULL;
		return;
	}

	if ( s_RenderStatesBackup.setup )
		return;

	if ( FAILED( m_pDevice->CreateStateBlock( D3DSBT_ALL , &s_RenderStatesBackup.d3d9_state_block ) ) )
		return;

	s_RenderStatesBackup.d3d9_state_block->Capture( );

	m_pDevice->SetVertexShader( nullptr );
	m_pDevice->SetPixelShader( nullptr );
	m_pDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	m_pDevice->SetRenderState( D3DRS_LIGHTING , FALSE );
	m_pDevice->SetRenderState( D3DRS_FOGENABLE , FALSE );
	m_pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_NONE );
	m_pDevice->SetRenderState( D3DRS_FILLMODE , D3DFILL_SOLID );

	m_pDevice->SetRenderState( D3DRS_ZENABLE , D3DZB_FALSE );
	m_pDevice->SetRenderState( D3DRS_SCISSORTESTENABLE , TRUE );
	m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE , FALSE );
	m_pDevice->SetRenderState( D3DRS_STENCILENABLE , FALSE );

	m_pDevice->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS , TRUE );
	m_pDevice->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE , TRUE );

	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
	m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE , FALSE );
	m_pDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE , TRUE );
	m_pDevice->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	m_pDevice->SetRenderState( D3DRS_SRCBLENDALPHA , D3DBLEND_INVDESTALPHA );
	m_pDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
	m_pDevice->SetRenderState( D3DRS_DESTBLENDALPHA , D3DBLEND_ONE );

	m_pDevice->SetRenderState( D3DRS_SRGBWRITEENABLE , FALSE );
	m_pDevice->SetRenderState( D3DRS_COLORWRITEENABLE , D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
		D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );

	s_RenderStatesBackup.setup = true;
}

void c_renderer::restore_render_states( )
{
	if ( !s_RenderStatesBackup.setup )
		return;

	// Restore DX9 state
	s_RenderStatesBackup.d3d9_state_block->Apply( );
	s_RenderStatesBackup.d3d9_state_block->Release( );

	s_RenderStatesBackup.setup = false;
}

void c_renderer::draw_polygon( std::vector< position_t > Vertices , DWORD col , bool bFilled )
{
	if ( !m_bInitialized )
		return;

	int VertSize = ( int ) Vertices.size( );

	Vertex_t* vtx = new Vertex_t [ bFilled ? VertSize : ( VertSize + 1 ) ];
	for ( auto i = 0; i < VertSize; i++ ) {
		vtx [ i ] = {
			( float ) Vertices [ i ].x ,
			( float ) Vertices [ i ].y ,
			0.0f ,
			1.0f ,
			col
		};
	}

	if ( !bFilled ) {
		vtx [ VertSize ] = vtx [ 0 ];
	}

	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , true );
	m_pDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	m_pDevice->SetTexture( 0 , nullptr );
	m_pDevice->DrawPrimitiveUP( bFilled ? D3DPT_TRIANGLEFAN : D3DPT_LINESTRIP , bFilled ? VertSize - 2 : VertSize , vtx , sizeof( Vertex_t ) );

	delete [ ] vtx;
}

void renderer::c_renderer::draw_polygon_multi_color( std::vector< position_t > Vertices , std::vector< DWORD > Colors , bool bFilled )
{
	if ( !m_bInitialized )
		return;

	int VertSize = ( int ) Vertices.size( );

	Vertex_t* vtx = new Vertex_t [ bFilled ? VertSize : ( VertSize + 1 ) ];
	for ( auto i = 0; i < VertSize; i++ ) {
		vtx [ i ] = {
			( float ) Vertices [ i ].x ,
			( float ) Vertices [ i ].y ,
			0.0f ,
			1.0f ,
			Colors [ i ] // i feel uncomfortable about this..
		};
	}

	if ( !bFilled ) {
		vtx [ VertSize ] = vtx [ 0 ];
	}

	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , true );
	m_pDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	m_pDevice->SetTexture( 0 , nullptr );
	m_pDevice->DrawPrimitiveUP( bFilled ? D3DPT_TRIANGLEFAN : D3DPT_LINESTRIP , bFilled ? VertSize - 2 : VertSize , vtx , sizeof( Vertex_t ) );

	delete [ ] vtx;
}

void c_renderer::create_font( ID3DXFont** pFont , const std::string& szFamily , int nHeight , int nWeight )
{
	if ( !m_bInitialized )
		return;

	ID3DXFont* pD3DFont = nullptr;
	D3DXCreateFontA( m_pDevice , nHeight , 0 , nWeight , 1 , false , DEFAULT_CHARSET , OUT_DEFAULT_PRECIS , 0 , DEFAULT_PITCH | FF_DONTCARE , szFamily.data( ) , &pD3DFont );
	*pFont = pD3DFont;
}

dimensions_t c_renderer::text_size( ID3DXFont* pFont , const std::string& szText )
{
	RECT mTextRect = RECT( );
	pFont->DrawTextA( nullptr , szText.data( ) , szText.length( ) , &mTextRect , DT_CALCRECT , D3DCOLOR_RGBA( 0 , 0 , 0 , 0 ) );

	return dimensions_t( ( int ) ( mTextRect.right - mTextRect.left ) , ( int ) ( mTextRect.bottom - mTextRect.top ) );
}

void renderer::c_renderer::draw_text( int nX , int nY , ID3DXFont* pFont , const std::string& szText , DWORD col )
{
	if ( !m_bInitialized )
		return;

	RECT mDrawTextRect = { ( long ) nX , ( long ) nY , ( long ) nX , ( long ) nY };
	pFont->DrawTextA( nullptr , szText.data( ) , szText.length( ) , &mDrawTextRect , DT_LEFT | DT_NOCLIP , col );
}

void c_renderer::draw_filled_rect( int nX , int nY , int nW , int nH , DWORD col )
{
	if ( !m_bInitialized )
		return;

	std::vector< position_t > Vertices = {
		position_t( nX , nY ) ,
		position_t( nX + nW , nY ) ,
		position_t( nX + nW , nY + nH ) ,
		position_t( nX , nY + nH )
	};

	draw_polygon( Vertices , col , true );
}

void c_renderer::draw_filled_rect( position_t sPos , dimensions_t sDim , DWORD col )
{
	draw_filled_rect( sPos.x , sPos.y , sDim.w , sDim.h , col );
}

void c_renderer::draw_filled_rect( rect_t sRect , DWORD col )
{
	draw_filled_rect( sRect.x , sRect.y , sRect.w , sRect.h , col );
}

void c_renderer::draw_outlined_rect( int nX , int nY , int nW , int nH , DWORD col )
{
	if ( !m_bInitialized )
		return;

	std::vector< position_t > Vertices = {
		position_t( nX , nY ) ,
		position_t( nX + nW , nY ) ,
		position_t( nX + nW , nY + nH ) ,
		position_t( nX , nY + nH )
	};

	draw_polygon( Vertices , col , false );
}

void c_renderer::draw_outlined_rect( position_t sPos , dimensions_t sDim , DWORD col )
{
	draw_outlined_rect( sPos.x , sPos.y , sDim.w , sDim.h , col );
}

void c_renderer::draw_outlined_rect( rect_t sRect , DWORD col )
{
	draw_outlined_rect( sRect.x , sRect.y , sRect.w , sRect.h , col );
}

void c_renderer::draw_multi_color_rect( int nX , int nY , int nW , int nH , DWORD col1 , DWORD col2 , DWORD col3 , DWORD col4 )
{
	if ( !m_bInitialized )
		return;

	std::vector< position_t > Vertices = {
		position_t( nX , nY ) ,
		position_t( nX + nW , nY ) ,
		position_t( nX + nW , nY + nH ) ,
		position_t( nX , nY + nH )
	};

	std::vector< DWORD > Colors = {
		col1 , col2 , col3 , col4
	};

	draw_polygon_multi_color( Vertices , Colors , true );
}

void c_renderer::draw_multi_color_rect( position_t sPos , dimensions_t sDim , DWORD col1 , DWORD col2 , DWORD col3 , DWORD col4 )
{
	draw_multi_color_rect( sPos.x , sPos.y , sDim.w , sDim.h , col1 , col2 , col3 , col4 );
}

void c_renderer::draw_multi_color_rect( rect_t sRect , DWORD col1 , DWORD col2 , DWORD col3 , DWORD col4 )
{
	draw_multi_color_rect( sRect.x , sRect.y , sRect.w , sRect.h , col1 , col2 , col3 , col4 );
}

void c_renderer::draw_pos_line( int nX1 , int nY1 , int nX2 , int nY2 , DWORD col )
{
	if ( !m_bInitialized )
		return;

	Vertex_t Vertices [ 2 ] = {
		{ ( float ) nX1 , ( float ) nY1 , 0.0f , 1.0f , col },
		{ ( float ) nX2 , ( float ) nY2 , 0.0f , 1.0f , col }
	};

	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , true );
	m_pDevice->SetTexture( 0 , nullptr );
	m_pDevice->SetPixelShader( nullptr );
	m_pDevice->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	m_pDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
	m_pDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST , 1 , &Vertices , sizeof( Vertex_t ) );
}

void c_renderer::draw_pos_line( position_t sPos1 , position_t sPos2 , DWORD col )
{
	draw_pos_line( sPos1.x , sPos1.y , sPos2.x , sPos2.y , col );
}

void c_renderer::draw_line( int nX , int nY , int nW , int nH , DWORD col )
{
	draw_pos_line( nX , nY , nX + nW , nY + nH , col );
}

void c_renderer::draw_line( position_t sPos , dimensions_t sDim , DWORD col )
{
	draw_line( sPos.x , sPos.y , sDim.w , sDim.h , col );
}

void c_renderer::draw_line( rect_t sRect , DWORD col )
{
	draw_line( sRect.x , sRect.y , sRect.w , sRect.h , col );
}

void c_renderer::draw_filled_triangle( int nX1 , int nY1 , int nX2 , int nY2 , int nX3 , int nY3 , DWORD col )
{
	if ( !m_bInitialized )
		return;

	Vertex_t Vertices [ 3 ] = {
		{ ( float ) ( nX1 ) , ( float ) ( nY1 ), 0.0f , 1.0f , col },
		{ ( float ) ( nX2 ) , ( float ) ( nY2 ), 0.0f , 1.0f , col },
		{ ( float ) ( nX3 ) , ( float ) ( nY3 ), 0.0f , 1.0f , col }
	};

	m_pDevice->SetTexture( 0 , nullptr );
	m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 1 , &Vertices , sizeof( Vertex_t ) );
}

void c_renderer::draw_filled_triangle( position_t sPos1 , position_t sPos2 , position_t sPos3 , DWORD col )
{
	draw_filled_triangle( sPos1.x , sPos1.y , sPos2.x , sPos2.y , sPos3.x , sPos3.y , col );
}

void c_renderer::draw_outlined_triangle( int nX1 , int nY1 , int nX2 , int nY2 , int nX3 , int nY3 , DWORD col )
{
	if ( !m_bInitialized )
		return;

	Vertex_t Vertices [ 4 ] = {
		{ ( float ) ( nX1 ) , ( float ) ( nY1 ) , 0.0f , 1.0f , col },
		{ ( float ) ( nX2 ) , ( float ) ( nY2 ) , 0.0f , 1.0f , col },
		{ ( float ) ( nX3 ) , ( float ) ( nY3 ) , 0.0f , 1.0f , col },
		{ ( float ) ( nX1 ) , ( float ) ( nY1 ) , 0.0f , 1.0f , col }
	};

	m_pDevice->SetTexture( 0 , nullptr );
	m_pDevice->DrawPrimitiveUP( D3DPT_LINESTRIP , 3 , &Vertices , sizeof( Vertex_t ) );
}

void c_renderer::draw_outlined_triangle( position_t sPos1 , position_t sPos2 , position_t sPos3 , DWORD col )
{
	draw_outlined_triangle( sPos1.x , sPos1.y , sPos2.x , sPos2.y , sPos3.x , sPos3.y , col );
}

void c_renderer::draw_filled_circle( int nX , int nY , float flRadius , DWORD col )
{
	if ( !m_bInitialized )
		return;

	int nResolution = static_cast< int >( flRadius * 2.f ) + 2;

	Vertex_t* Vertices = new Vertex_t [ nResolution + 1 ];

	for ( int i = 0; i <= nResolution; i++ ) {
		Vertices [ i ] = {
			( float ) nX - flRadius * std::cosf( D3DX_PI * ( ( i - 1 ) / ( ( float ) nResolution / 2.0f ) ) ) ,
			( float ) nY - flRadius * std::sinf( D3DX_PI * ( ( i - 1 ) / ( ( float ) nResolution / 2.0f ) ) ) ,
			0.0f ,
			1.0f ,
			col
		};
	}

	m_pDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN , nResolution , Vertices , sizeof( Vertex_t ) );
	delete [ ] Vertices;
}

void c_renderer::draw_filled_circle( position_t sPos , float flRadius , DWORD col )
{
	draw_filled_circle( sPos.x , sPos.y , flRadius , col );
}

void c_renderer::draw_outlined_circle( int nX , int nY , float flRadius , DWORD col )
{
	if ( !m_bInitialized )
		return;

	int nResolution = static_cast< int >( flRadius * 2.f ) + 2;

	Vertex_t* Vertices = new Vertex_t [ nResolution + 1 ];

	for ( int i = 0; i <= nResolution; i++ ) {
		Vertices [ i ] = {
			( float ) nX + flRadius * std::cosf( D3DX_PI * ( i / ( ( float ) nResolution / 2.0f ) ) ) ,
			( float ) nY + flRadius * std::sinf( D3DX_PI * ( i / ( ( float ) nResolution / 2.0f ) ) ) ,
			0.0f ,
			1.0f ,
			col
		};
	}

	m_pDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	m_pDevice->DrawPrimitiveUP( D3DPT_LINESTRIP , nResolution , Vertices , sizeof( Vertex_t ) );
	delete [ ] Vertices;
}

void c_renderer::draw_outlined_circle( position_t sPos , float flRadius , DWORD col )
{
	draw_outlined_circle( sPos.x , sPos.y , flRadius , col );
}

RECT c_renderer::push_clip( int nX , int nY , int nW , int nH )
{
	RECT rcRet = RECT( );
	RECT rcNewRect = RECT( );

	if ( !m_bInitialized )
		return rcRet;

	m_pDevice->GetScissorRect( &rcRet );

	rcNewRect = RECT { ( LONG ) nX , ( LONG ) nY , ( LONG ) ( nX + nW ) , ( LONG ) ( nY + nH ) };
	m_pDevice->SetScissorRect( &rcNewRect );

	m_vecClipRectStack.push_back( rcRet );
	return rcRet;
}

RECT c_renderer::push_clip( position_t sPos , dimensions_t sDim )
{
	return push_clip( sPos.x , sPos.y , sDim.w , sDim.h );
}

RECT c_renderer::push_clip( rect_t sRect )
{
	return push_clip( sRect.x , sRect.y , sRect.w , sRect.h );
}

RECT c_renderer::pop_clip( )
{
	RECT rcRet = RECT( );
	if ( !m_bInitialized )
		return rcRet;

	if ( !m_vecClipRectStack.size( ) )
		return rcRet;

	rcRet = m_vecClipRectStack.back( );
	m_vecClipRectStack.pop_back( );

	m_pDevice->SetScissorRect( &rcRet );
	return rcRet;
}
