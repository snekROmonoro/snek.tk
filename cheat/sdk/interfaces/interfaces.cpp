#include "interfaces.hpp"
#include "../../includes.hpp"

class interface_reg
{
private:
	using instantiate_interface_fn = void* ( * )( );

public:
	instantiate_interface_fn create_fn;
	const char* name;

	interface_reg* next;
};

#if 0
interface_reg* get_interfaces_list( const char* mod ) {
	DWORD interface_fn = reinterpret_cast< DWORD >( GetProcAddress( GetModuleHandleA( mod ) , "CreateInterface" ) ); // we need to use util::modules for this but ok
	if ( !interface_fn )
		return nullptr;

	unsigned int jmp_start = ( unsigned int ) ( interface_fn ) +4;
	unsigned int jmp_target = jmp_start + *( unsigned int* ) ( jmp_start + 1 ) + 5;
	interface_reg* interface_reg_list = **reinterpret_cast< interface_reg*** >( jmp_target + 6 );

	return interface_reg_list;
}

template < typename t >
t create_interface( const char* mod , const char* iname ) {
	t ret = ( t ) nullptr;
	interface_reg* interface_reg_list = get_interfaces_list( mod );

	for ( interface_reg* cur = interface_reg_list; cur; cur = cur->next ) {
		if ( strcmp( cur->name , iname ) == 0 ) {
			ret = reinterpret_cast< t >( cur->create_fn( ) );

			util::console::set_prefix( util::console::SDK );
			util::console::print( "found %s at: 0x%p\n" , cur->name , ret );
			util::console::set_prefix( util::console::NONE );
		}
	}

	return reinterpret_cast< t >( ret );
}

template < typename t >
t create_interface_nover( const char* mod , const char* iname ) {
	t ret = ( t ) nullptr;
	interface_reg* interface_reg_list = get_interfaces_list( mod );

	int part_match_len = strlen( iname );
	for ( interface_reg* cur = interface_reg_list; cur; cur = cur->next ) {
		if ( !strncmp( cur->name , iname , part_match_len ) && std::atoi( cur->name + part_match_len ) > 0 ) {
			ret = reinterpret_cast< t >( cur->create_fn( ) );

			util::console::set_prefix( util::console::SDK );
			util::console::print( "found %s at: 0x%p\n" , cur->name , ret );
			util::console::set_prefix( util::console::NONE );
		}
	}

	return reinterpret_cast< t >( ret );
}
#endif

// < interface_name , interfaces_address >
std::vector< std::pair< std::pair< std::string , std::string > , uintptr_t > > get_all_interfaces( ) {
	std::vector< util::modules::Module > modules;
	if ( !util::modules::GetAllModules( modules ) )
		return { };

	std::vector< std::pair< std::pair< std::string , std::string > , uintptr_t > > output = { };

	for ( const auto& m : modules ) {
		// get CreateInterface export.
		util::address ci = util::modules::GetExport( m , "CreateInterface" );
		if ( !util::address::valid( ci.get( ) ) )
			continue;

		// there is a jump to the real CreateInterface function.
		if ( ci.add( 0x4 ).deref( ).get< uint8_t >( ) == 0xe9 ) {
			// displaced jump - take it
			ci = ci.rel32( 0x5 );

			// we are now in the true CreateInterface function.

			// check for ESI register at 5th instruction
			// ptr to linked list is after the MOV ESI instruction.
			if ( ci.add( 0x5 ).deref( ).get< uint8_t >( ) == 0x35 ) {

				// get ptr to linked list.
				interface_reg* reg = ci.add( 0x6 ).deref( ).deref( ).get< interface_reg* >( );

				// iterate list.
				for ( interface_reg* cur = reg; cur; cur = cur->next ) {
					if ( !cur->name )
						continue;

					output.push_back( { { std::string( m.GetName( ) ) , std::string( cur->name ) } , ( uintptr_t ) cur->create_fn( ) } );
				}
			}
		}

		// check for ESI register at 2nd instruction
		// ptr to linked list is after the MOV ESI instruction.
		else if ( ci.add( 0x2 ).deref( ).get< uint8_t >( ) == 0x35 ) {

			// get ptr to linked list.
			interface_reg* reg = ci.add( 0x3 ).deref( ).deref( ).get< interface_reg* >( );

			// iterate list.
			for ( interface_reg* cur = reg; cur; cur = cur->next ) {
				if ( !cur->name )
					continue;

				output.push_back( { { std::string( m.GetName( ) ) , std::string( cur->name ) } , ( uintptr_t ) cur->create_fn( ) } );
			}
		}
	}

	return output;
}

template < typename t >
t get_interface_by_name( const char* iname , bool nover = false , int skip = 0 ) {
	t ret = ( t ) nullptr;

	static std::vector< std::pair< std::pair< std::string , std::string > , uintptr_t > > m_interfaces = { };

	// get interfaces
	if ( m_interfaces.empty( ) ) {
		m_interfaces = get_all_interfaces( );
	}

	// check if it's empty
	if ( m_interfaces.empty( ) ) {
		return ret;
	}

	int part_match_len = strlen( iname );
	for ( const auto& interf : m_interfaces ) {
		const char* module_name = interf.first.first.c_str( );
		const char* interf_name = interf.first.second.c_str( );
		uintptr_t interf_addr = interf.second;

		// it is found twice and has different address?
		// so check for module name
		if ( strstr( interf_name , "VGUI_Surface031" ) && !strstr( module_name , "vguimatsurface.dll" ) ) {
			continue;
		}

		if ( strstr( interf_name , "GameMovement001" ) && !strstr( module_name , "client.dll" ) ) {
			continue;
		}

		bool exact = false;
		if ( nover ) {
			exact = !strncmp( interf_name , iname , part_match_len ) && std::atoi( interf_name + part_match_len ) > 0;
		}
		else {
			exact = strcmp( interf_name , iname ) == 0;
		}

		// make sure it's exact interface
		if ( !exact )
			continue;

		if ( skip > 0 ) {
			skip--;
			continue;
		}

		bool changed = ret != reinterpret_cast< t >( interf_addr );

		// if it changed the interface address from one to another
		//if ( ret && changed )
			//continue;

		ret = reinterpret_cast< t >( interf_addr );

		if ( changed ) {
			util::console::set_prefix( util::console::SDK );
			util::console::print( "found %s at: 0x%p\n" , interf_name , ret );
			util::console::set_prefix( util::console::NONE );
		}
	}

	return reinterpret_cast< t >( ret );
}

IMemAlloc* g_pMemAlloc = nullptr;

IDirect3DDevice9* sdk::interfaces::d3d_device = nullptr;
IBaseClientDLL* sdk::interfaces::client = nullptr;
CGlobalVarsBase* sdk::interfaces::global_vars = nullptr;
CGameUI* sdk::interfaces::game_ui = nullptr;
IVModelInfo* sdk::interfaces::model_info = nullptr;
IClientEntityList* sdk::interfaces::entity_list = nullptr;
ICvar* sdk::interfaces::cvar = nullptr;
IVEngineClient* sdk::interfaces::engine = nullptr;
CClientState* sdk::interfaces::client_state = nullptr;
IInputSystem* sdk::interfaces::input_system = nullptr;
CInput* sdk::interfaces::input = nullptr;
IMoveHelper* sdk::interfaces::move_helper = nullptr;
CPrediction* sdk::interfaces::prediction = nullptr;
IEngineVGui* sdk::interfaces::engine_vgui = nullptr;
IPanel* sdk::interfaces::panel = nullptr;
ISurface* sdk::interfaces::surface = nullptr;
IVDebugOverlay* sdk::interfaces::debug_overlay = nullptr;
IGameMovement* sdk::interfaces::game_movement = nullptr;
IClientMode* sdk::interfaces::client_mode = nullptr;
IClientShadowMgr* sdk::interfaces::shadow_manager = nullptr;
IMaterialSystem* sdk::interfaces::material_system = nullptr;
CStudioRenderContext* sdk::interfaces::studio_render = nullptr;
IVModelRender* sdk::interfaces::model_render = nullptr;
IVRenderView* sdk::interfaces::render_view = nullptr;
CViewRender* sdk::interfaces::view_render = nullptr;
IPhysicsSurfaceProps* sdk::interfaces::physics_surface_props = nullptr;
IEngineTrace* sdk::interfaces::engine_trace = nullptr;
IGameEventManager2* sdk::interfaces::event_manager = nullptr;

bool sdk::interfaces::init( void )
{
	//g_pMemAlloc = *( IMemAlloc** ) ( GetProcAddress( GetModuleHandleA( "tier0.dll" ) , "g_pMemAlloc" ) );
	g_pMemAlloc = util::modules::GetExport( util::modules::GetModule( "tier0.dll" ) , "g_pMemAlloc" ).deref( ).get< IMemAlloc* >( );
	util::console::set_prefix( util::console::SDK );
	util::console::print( "g_pMemAlloc: 0x%p\n" , g_pMemAlloc );
	util::console::set_prefix( util::console::NONE );
	if ( !g_pMemAlloc )
		return false;

	sdk::interfaces::d3d_device = util::pattern::search( "shaderapidx9.dll" , "A1 ? ? ? ? 50 8B 08 FF 51 0C" , true ).add( 1 ).deref( ).deref( ).get< IDirect3DDevice9* >( );
	util::console::set_prefix( util::console::SDK );
	util::console::print( "sdk::interfaces::d3d_device: 0x%p\n" , sdk::interfaces::d3d_device );
	util::console::set_prefix( util::console::NONE );
	if ( !sdk::interfaces::d3d_device )
		return false;

	//sdk::interfaces::client = create_interface< IBaseClientDLL* >( "client.dll" , "VClient018" );
	sdk::interfaces::client = get_interface_by_name< IBaseClientDLL* >( "VClient" , true );
	if ( !sdk::interfaces::client )
		return false;

	sdk::interfaces::global_vars = util::address( util::get_virtual_function< void* >( sdk::interfaces::client , 0 ) ).add( 0x1f ).deref( ).deref( ).get< CGlobalVarsBase* >( );
	util::console::set_prefix( util::console::SDK );
	util::console::print( "sdk::interfaces::global_vars: 0x%p\n" , sdk::interfaces::global_vars );
	util::console::set_prefix( util::console::NONE );
	if ( !sdk::interfaces::global_vars )
		return false;

	//sdk::interfaces::game_ui = create_interface< CGameUI* >( "client.dll" , "GameUI011" );
	sdk::interfaces::game_ui = get_interface_by_name< CGameUI* >( "GameUI" , true );
	if ( !sdk::interfaces::game_ui )
		return false;

	//sdk::interfaces::model_info = create_interface< IVModelInfo* >( "engine.dll" , "VModelInfoClient004" );
	sdk::interfaces::model_info = get_interface_by_name< IVModelInfo* >( "VModelInfoClient" , true );
	if ( !sdk::interfaces::model_info )
		return false;

	//sdk::interfaces::entity_list = create_interface< IClientEntityList* >( "client.dll" , "VClientEntityList003" );
	sdk::interfaces::entity_list = get_interface_by_name< IClientEntityList* >( "VClientEntityList" , true );
	if ( !sdk::interfaces::entity_list )
		return false;

	//sdk::interfaces::cvar = create_interface< ICvar* >( "vstdlib.dll" , "VEngineCvar007" );
	sdk::interfaces::cvar = get_interface_by_name< ICvar* >( "VEngineCvar" , true );
	if ( !sdk::interfaces::cvar )
		return false;

	sdk::interfaces::engine = get_interface_by_name< IVEngineClient* >( "VEngineClient" , true );
	if ( !sdk::interfaces::engine )
		return false;

	//sdk::interfaces::client_state = util::pattern::search( "engine.dll" , "B9 ? ? ? ? 56 FF 50 14 8B 34 85" , true ).add( 0x1 ).deref( ).get< CGlobalState* >( )->m_client_state;
	sdk::interfaces::client_state = **( CClientState*** ) ( ( ( *( uintptr_t** ) sdk::interfaces::engine ) [ 12 ] ) + 0x10 );
	util::console::set_prefix( util::console::SDK );
	util::console::print( "sdk::interfaces::client_state: 0x%p\n" , sdk::interfaces::client_state );
	util::console::set_prefix( util::console::NONE );
	if ( !sdk::interfaces::client_state )
		return false;

	sdk::interfaces::input_system = get_interface_by_name< IInputSystem* >( "InputSystemVersion" , true );
	if ( !sdk::interfaces::input_system )
		return false;

	sdk::interfaces::input = util::address( util::get_virtual_function< void* >( sdk::interfaces::client , 16 /*INACTIVATEMOUSE*/ ) ).add( 0x1 ).deref( ).get< CInput* >( );
	util::console::set_prefix( util::console::SDK );
	util::console::print( "sdk::interfaces::input: 0x%p\n" , sdk::interfaces::input );
	util::console::set_prefix( util::console::NONE );
	if ( !sdk::interfaces::input )
		return false;

	//sdk::interfaces::move_helper = util::pattern::search( "client.dll" , "8B 0D ? ? ? ? 8B 46 08 68" , true ).add( 0x2 ).deref( ).deref( ).get< IMoveHelper* >( );
	sdk::interfaces::move_helper = util::pattern::search( "client.dll" , "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" , true ).add( 0x2 ).deref( ).deref( ).get< IMoveHelper* >( );
	util::console::set_prefix( util::console::SDK );
	util::console::print( "sdk::interfaces::move_helper: 0x%p\n" , sdk::interfaces::move_helper );
	util::console::set_prefix( util::console::NONE );
	if ( !sdk::interfaces::move_helper )
		return false;

	sdk::interfaces::prediction = get_interface_by_name< CPrediction* >( "VClientPrediction" , true );
	if ( !sdk::interfaces::prediction )
		return false;

	sdk::interfaces::engine_vgui = get_interface_by_name< IEngineVGui* >( "VEngineVGui" , true );
	if ( !sdk::interfaces::engine_vgui )
		return false;

	sdk::interfaces::panel = get_interface_by_name< IPanel* >( "VGUI_Panel" , true );
	if ( !sdk::interfaces::panel )
		return false;

	sdk::interfaces::surface = get_interface_by_name< ISurface* >( "VGUI_Surface" , true ); // VGUI_Surface031
	if ( !sdk::interfaces::surface )
		return false;

	sdk::interfaces::debug_overlay = get_interface_by_name< IVDebugOverlay* >( "VDebugOverlay" , true );
	if ( !sdk::interfaces::debug_overlay )
		return false;

	sdk::interfaces::game_movement = get_interface_by_name< IGameMovement* >( "GameMovement001" , false );
	if ( !sdk::interfaces::game_movement )
		return false;

	sdk::interfaces::client_mode = util::address( util::get_virtual_function< void* >( sdk::interfaces::client , 10 /*HUDPROCESSINPUT*/ ) ).add( 0x5 ).deref( ).deref( ).get< IClientMode* >( );
	util::console::set_prefix( util::console::SDK );
	util::console::print( "sdk::interfaces::client_mode: 0x%p\n" , sdk::interfaces::client_mode );
	util::console::set_prefix( util::console::NONE );
	if ( !sdk::interfaces::client_mode )
		return false;

	sdk::interfaces::shadow_manager = util::pattern::search( "client.dll" , "A1 ? ? ? ? FF 90 ? ? ? ? 6A 00 6A 00" , true ).add( 1 ).deref( ).get< IClientShadowMgr* >( );
	util::console::set_prefix( util::console::SDK );
	util::console::print( "sdk::interfaces::shadow_manager: 0x%p\n" , sdk::interfaces::shadow_manager );
	util::console::set_prefix( util::console::NONE );
	if ( !sdk::interfaces::shadow_manager )
		return false;

	sdk::interfaces::material_system = get_interface_by_name< IMaterialSystem* >( "VMaterialSystem" , true );
	if ( !sdk::interfaces::material_system )
		return false;

	sdk::interfaces::studio_render = get_interface_by_name< CStudioRenderContext* >( "VStudioRender" , true );
	if ( !sdk::interfaces::studio_render )
		return false;

	sdk::interfaces::model_render = get_interface_by_name< IVModelRender* >( "VEngineModel" , true );
	if ( !sdk::interfaces::model_render )
		return false;

	sdk::interfaces::render_view = get_interface_by_name< IVRenderView* >( "VEngineRenderView" , true );
	if ( !sdk::interfaces::render_view )
		return false;

	sdk::interfaces::view_render = util::pattern::search( "client.dll" , "8B 0D ? ? ? ? 57 8B 01 FF 50 14 E8 ? ? ? ? 5F" , true ).add( 0x2 ).deref( ).deref( ).get< CViewRender* >( );
	util::console::set_prefix( util::console::SDK );
	util::console::print( "sdk::interfaces::view_render: 0x%p\n" , sdk::interfaces::view_render );
	util::console::set_prefix( util::console::NONE );
	if ( !sdk::interfaces::view_render )
		return false;

	sdk::interfaces::physics_surface_props = get_interface_by_name< IPhysicsSurfaceProps* >( "VPhysicsSurfaceProps" , true );
	if ( !sdk::interfaces::physics_surface_props )
		return false;

	sdk::interfaces::engine_trace = get_interface_by_name< IEngineTrace* >( "EngineTraceClient" , true );
	if ( !sdk::interfaces::engine_trace )
		return false;

	sdk::interfaces::event_manager = get_interface_by_name< IGameEventManager2* >( "GAMEEVENTSMANAGER" , true , 1 );
	if ( !sdk::interfaces::event_manager )
		return false;

	return true;
}
