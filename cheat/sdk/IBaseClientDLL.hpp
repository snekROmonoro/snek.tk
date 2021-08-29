#pragma once
#include "../util/vfunc/vfunc.hpp"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/cdll_int.h

enum ClientFrameStage_t : int
{
	FRAME_UNDEFINED = -1 ,			// (haven't run any frames yet)
	FRAME_START ,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START ,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START ,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END ,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END ,

	// We're about to start rendering the scene
	FRAME_RENDER_START ,
	// We've finished rendering the scene.
	FRAME_RENDER_END ,

	FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};

class ClientClass;

class IBaseClientDLL {
public:

	// request a pointer to the list of client datatable classes
	ClientClass* GetAllClasses( ) {
		using o_fn = ClientClass * ( __thiscall* )( void* );
		return util::get_virtual_function< o_fn >( this , 8 )( this );
	}
};

class CGameUI {
public:
	 void CreateCommandMessageBox( const char* title , const char* message , bool showOk = true , bool showCancel = false , const char* okCommand = nullptr , const char* cancelCommand = nullptr , const char* closedCommand = nullptr , const char* legend = nullptr , void* unknown = nullptr ) {
		return util::get_virtual_function< void( __thiscall* )( decltype( this ) , const char* , const char* , bool , bool , const char* , const char* , const char* , const char* , void* )>( this , 20 )( this , title , message , showOk , showCancel , okCommand , cancelCommand , closedCommand , legend , unknown );
	}
};
