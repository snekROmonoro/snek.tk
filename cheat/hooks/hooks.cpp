#include "hooks.hpp"

bool hooks::m_hooked = false;

util::hook::vmt_hook hooks::m_vmt_d3d;
util::hook::vmt_hook hooks::m_vmt_surface;
util::hook::vmt_hook hooks::m_vmt_client;
util::hook::vmt_hook hooks::m_vmt_clientmode;

bool hooks::init( )
{
	util::console::set_prefix( util::console::HOOK );
	util::console::print( "initializing MinHook..\n" );
	util::console::set_prefix( util::console::NONE );

	/* Handle MinHook initialization */ {
		MH_STATUS initialize_status = MH_Initialize( );
		if ( initialize_status != MH_OK ) {
			util::console::set_prefix( util::console::ERR );
			util::console::print( "failed to initialize minhook. status: %s \n" , MH_StatusToString( initialize_status ) );
			util::console::set_prefix( util::console::NONE );
			return false;
		}
	}

	util::console::set_prefix( util::console::HOOK );
	util::console::print( "initialized MinHook\n" );
	util::console::print( "starting to hook..\n" );

	oWndProc = ( WNDPROC ) SetWindowLongA( globals::csgo_window , GWLP_WNDPROC , ( long ) hooks::WndProc );
	util::console::print( "hooked WndProc\n" );

	util::console::set_prefix( util::console::NONE );

	// d3d
	if ( !m_vmt_d3d.initialize( sdk::interfaces::d3d_device ) )
		return false;

	if ( !m_vmt_d3d.hook_method( 42 , EndScene , &oEndScene ) )
		return false;

	if ( !m_vmt_d3d.hook_method( 16 , Reset , &oReset ) )
		return false;

	// surface
	if ( !m_vmt_surface.initialize( sdk::interfaces::surface ) )
		return false;

	if ( !m_vmt_surface.hook_method( 67 , LockCursor , &oLockCursor ) )
		return false;

	// client
	if ( !m_vmt_client.initialize( sdk::interfaces::client ) )
		return false;

	if ( !m_vmt_client.hook_method( 5 , LevelInitPreEntity , &oLevelInitPreEntity ) )
		return false;

	if ( !m_vmt_client.hook_method( 6 , LevelInitPostEntity , &oLevelInitPostEntity ) )
		return false;

	if ( !m_vmt_client.hook_method( 7 , LevelShutdown , &oLevelShutdown ) )
		return false;

	if ( !m_vmt_client.hook_method( 37 , FrameStageNotify , &oFrameStageNotify ) )
		return false;

	// client_mode
	if ( !m_vmt_clientmode.initialize( sdk::interfaces::client_mode ) )
		return false;

	/*if ( !m_vmt_clientmode.hook_method( 24 , CreateMove , &oCreateMove ) )
		return false;*/

	if ( !util::hook::hook_address( patterns::clientmodeshared_createmove.get< void* >( ) , CreateMove , &oCreateMove ) )
		return false;

	util::console::set_prefix( util::console::HOOK );
	util::console::print( "finished hooks\n" );
	util::console::set_prefix( util::console::NONE );

	m_hooked = true;
	return true;
}

bool hooks::release( )
{
	if ( !m_hooked )
		return false;

	MH_STATUS disable_hook_status = MH_DisableHook( MH_ALL_HOOKS );
	if ( disable_hook_status != MH_OK ) {
		util::console::set_prefix( util::console::ERR );
		util::console::print( "failed to disable hooks. status: %s \n" , MH_StatusToString( disable_hook_status ) );
		util::console::set_prefix( util::console::NONE );
		return false;
	}

	MH_STATUS uninitialize_minhook_status = MH_Uninitialize( );
	if ( MH_Uninitialize( ) != MH_OK ) {
		util::console::set_prefix( util::console::ERR );
		util::console::print( "failed to uninitialize MinHook. status: %s \n" , MH_StatusToString( uninitialize_minhook_status ) );
		util::console::set_prefix( util::console::NONE );

		return false;
	}

	return true;
}

WNDPROC hooks::oWndProc = 0;
long __stdcall hooks::WndProc( HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam )
{
	if ( menu::wnd_proc( hwnd , msg , wparam , lparam ) )
		return true;

	return CallWindowProcA( oWndProc , hwnd , msg , wparam , lparam );
}

long __fastcall hooks::EndScene( REG , IDirect3DDevice9* device )
{
	if ( !hooks::m_hooked )
		return oEndScene( REG_OUT , device );

	// since EndScene gets called twice in CSGO, we'll check for the address to be the same
	static auto ret = _ReturnAddress( );
	if ( ret != _ReturnAddress( ) )
		return oEndScene( REG_OUT , device );

	frame::update( );
	input::update( );

	g_renderer.setup_render_states( ); {
		g_renderer.draw_filled_rect( 20 , 20 , 40 , 40 , color::blue( ).to_d3d( ) );
		menu::on_end_scene( );
	} g_renderer.restore_render_states( );

	return oEndScene( REG_OUT , device );
}

long __fastcall hooks::Reset( REG , IDirect3DDevice9* device , D3DPRESENT_PARAMETERS* presentation_params )
{
	if ( !hooks::m_hooked )
		return oReset( REG_OUT , device , presentation_params );

	g_renderer.on_lost_device( );
	menu::on_lost_device( );

	auto hr = oReset( REG_OUT , device , presentation_params );
	if ( SUCCEEDED( hr ) ) {
		/* reset */
		g_renderer.on_reset_device( device );
		menu::on_reset_device( );
	}

	return hr;
}

void __fastcall hooks::LockCursor( REG ) {
	if ( !hooks::m_hooked )
		return oLockCursor( REG_OUT );

	if ( menu::m_opened ) {
		sdk::interfaces::surface->UnlockCursor( );
		return;
	}

	oLockCursor( REG_OUT );
}

void __fastcall hooks::LevelInitPreEntity( REG , const char* map )
{
	if ( !hooks::m_hooked )
		return oLevelInitPreEntity( REG_OUT , map );

	oLevelInitPreEntity( REG_OUT , map );
}

void __fastcall hooks::LevelInitPostEntity( REG )
{
	if ( !hooks::m_hooked )
		return oLevelInitPostEntity( REG_OUT );

	globals::local_player = sdk::interfaces::entity_list->GetClientEntity< Player* >( sdk::interfaces::engine->GetLocalPlayer( ) );

	oLevelInitPostEntity( REG_OUT );
}

void __fastcall hooks::LevelShutdown( REG )
{
	if ( !hooks::m_hooked )
		return oLevelShutdown( REG_OUT );

	globals::local_player = nullptr;

	oLevelShutdown( REG_OUT );
}

void __fastcall hooks::FrameStageNotify( REG , int stage )
{
	if ( !hooks::m_hooked )
		return oFrameStageNotify( REG_OUT , stage );

	if ( !sdk::interfaces::engine->IsInGame( ) )
		return oFrameStageNotify( REG_OUT , stage );

	globals::last_frame_stage = stage;
	globals::local_player = sdk::interfaces::entity_list->GetClientEntity< Player* >( sdk::interfaces::engine->GetLocalPlayer( ) );

	static auto draw_client_impacts = [ ] ( ) {
		if ( !globals::local_player )
			return;

		struct clientHitVerify_t
		{
			clientHitVerify_t( )
			{
				this->vecPosition = vec3_t( 0 , 0 , 0 );
				this->flTimestamp = 0;
				this->flExpireTime = -1;
			}

			clientHitVerify_t( vec3_t inPos , float flTimestamp , float flExpireTime )
			{
				this->vecPosition = inPos;
				this->flTimestamp = flTimestamp;
				this->flExpireTime = flExpireTime;
			}

			vec3_t vecPosition;
			float flTimestamp;
			float flExpireTime;
		};

		CUtlVector< clientHitVerify_t >& m_vecBulletVerifyListClient = globals::local_player->get< CUtlVector< clientHitVerify_t > >( 0x11C50 );
		static int last_count = 0;

		/*if ( g_cfg [ XOR( "misc_impact" ) ].get<bool>( ) ) {
			float time = g_csgo.sv_showimpacts_time->GetFloat( );
			for ( auto i = m_vecBulletVerifyListClient.Count( ); i > last_count; i-- ) {
				g_csgo.m_debug_overlay->AddBoxOverlay( m_vecBulletVerifyListClient [ i - 1 ].vecPosition , vec3_t( -2 , -2 , -2 ) , vec3_t( 2 , 2 , 2 ) , ang_t( 0 , 0 , 0 ) , 255 , 0 , 0 , 127 , time );
			}
		}*/

		for ( auto i = m_vecBulletVerifyListClient.Count( ); i > last_count; i-- ) {
			sdk::interfaces::debug_overlay->AddBoxOverlay( m_vecBulletVerifyListClient [ i - 1 ].vecPosition , vec3_t( -2 , -2 , -2 ) , vec3_t( 2 , 2 , 2 ) , vec3_t( 0 , 0 , 0 ) , 255 , 0 , 0 , 127 , 4.f );
		}

		if ( m_vecBulletVerifyListClient.Count( ) != last_count ) {
			last_count = m_vecBulletVerifyListClient.Count( );
		}
	};

	if ( stage == FRAME_RENDER_START ) {
		draw_client_impacts( );
	}

	oFrameStageNotify( REG_OUT , stage );
}

bool __fastcall hooks::CreateMove( REG , float time , CUserCmd* cmd )
{
	if ( !hooks::m_hooked )
		return oCreateMove( REG_OUT , time , cmd );

	bool original = oCreateMove( REG_OUT , time , cmd );

	if ( !cmd )
		return false;

	if ( original ) {
		sdk::interfaces::engine->SetViewAngles( cmd->viewangles );
		sdk::interfaces::prediction->SetLocalViewAngles( cmd->viewangles );
	}

	if ( !cmd->command_number )
		return false;

	globals::pCmd = cmd;
	globals::local_player = sdk::interfaces::entity_list->GetClientEntity< Player* >( sdk::interfaces::engine->GetLocalPlayer( ) );

	PVOID pebp;
	__asm mov pebp , ebp;
	bool* pbSendPacket = ( bool* ) ( *( DWORD* ) pebp - 0x1C );
	bool& bSendPacket = *pbSendPacket;
	bSendPacket = globals::bSendPacket = true;

	if ( globals::pCmd && globals::local_player ) {
		features::prediction::update( );

		globals::m_strafe_angles = cmd->viewangles;

		features::prediction::predict( );

		// ....

		features::prediction::restore( );
	}

	bSendPacket = globals::bSendPacket;
	return false;
}
