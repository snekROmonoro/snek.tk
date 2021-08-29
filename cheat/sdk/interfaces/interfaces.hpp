#pragma once
#include "../include.hpp"
#include <d3d9.h>
#include <d3dx9.h>

namespace sdk {
	namespace interfaces {
		extern IDirect3DDevice9* d3d_device;
		extern IBaseClientDLL* client;
		extern CGameUI* game_ui;
		extern IVModelInfo* model_info;
		extern IClientEntityList* entity_list;
		extern ICvar* cvar;
		extern IVEngineClient* engine;
		extern IInputSystem* input_system;
		extern CInput* input;
		extern CPrediction* prediction;
		extern IEngineVGui* engine_vgui;
		extern IPanel* panel;
		extern ISurface* surface;
		extern IVDebugOverlay* debug_overlay;
		extern CGameMovement* game_movement;
		extern IClientMode* client_mode;
		extern IClientShadowMgr* shadow_manager;
		extern IMaterialSystem* material_system;
		extern CStudioRenderContext* studio_render;
		extern IVModelRender* model_render;
		extern IVRenderView* render_view;
		extern CViewRender* view_render;
		extern IPhysicsSurfaceProps* physics_surface_props;
		extern IEngineTrace* engine_trace;
		extern IGameEventManager2* event_manager;

		bool init( void );
	}
}
