#include "menu.hpp"
#include "../includes.hpp"

namespace gui
{
	const char* keys [ ] = {
		( "[-]" ) ,
		( "[M1]" ) ,
		( "[M2]" ) ,
		( "[CN]" ) ,
		( "[M3]" ) ,
		( "[M4]" ) ,
		( "[M5]" ) ,
		( "[-]" ) ,
		( "[BACK]" ) ,
		( "[TAB]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[BACK]" ) ,
		( "[ENT]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[SHIFT]" ) ,
		( "[CTRL]" ) ,
		( "[ALT]" ) ,
		( "[PAUSE]" ) ,
		( "[CAPS]" ) ,
		( "[KAN]" ) ,
		( "[-]" ) ,
		( "[JUN]" ) ,
		( "[FIN]" ) ,
		( "[KAN]" ) ,
		( "[-]" ) ,
		( "[ESC]" ) ,
		( "[CON]" ) ,
		( "[NCO]" ) ,
		( "[ACC]" ) ,
		( "[MAD]" ) ,
		( "[SPA]" ) ,
		( "[PGU]" ) ,
		( "[PGD]" ) ,
		( "[END]" ) ,
		( "[HOME]" ) ,
		( "[LEFT]" ) ,
		( "[UP]" ) ,
		( "[RIGH]" ) ,
		( "[DOWN]" ) ,
		( "[SEL]" ) ,
		( "[PRINT]" ) ,
		( "[EXE]" ) ,
		( "[PRINT]" ) ,
		( "[INS]" ) ,
		( "[DEL]" ) ,
		( "[HEL]" ) ,
		( "[0]" ) ,
		( "[1]" ) ,
		( "[2]" ) ,
		( "[3]" ) ,
		( "[4]" ) ,
		( "[5]" ) ,
		( "[6]" ) ,
		( "[7]" ) ,
		( "[8]" ) ,
		( "[9]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[A]" ) ,
		( "[B]" ) ,
		( "[C]" ) ,
		( "[D]" ) ,
		( "[E]" ) ,
		( "[F]" ) ,
		( "[G]" ) ,
		( "[H]" ) ,
		( "[I]" ) ,
		( "[J]" ) ,
		( "[K]" ) ,
		( "[L]" ) ,
		( "[M]" ) ,
		( "[N]" ) ,
		( "[O]" ) ,
		( "[P]" ) ,
		( "[Q]" ) ,
		( "[R]" ) ,
		( "[S]" ) ,
		( "[T]" ) ,
		( "[U]" ) ,
		( "[V]" ) ,
		( "[W]" ) ,
		( "[X]" ) ,
		( "[Y]" ) ,
		( "[Z]" ) ,
		( "[WIN]" ) ,
		( "[WIN]" ) ,
		( "[APP]" ) ,
		( "[-]" ) ,
		( "[SLE]" ) ,
		( "[NUM0]" ) ,
		( "[NUM1]" ) ,
		( "[NUM2]" ) ,
		( "[NUM3]" ) ,
		( "[NUM4]" ) ,
		( "[NUM5]" ) ,
		( "[NUM6]" ) ,
		( "[NUM7]" ) ,
		( "[NUM8]" ) ,
		( "[NUM9]" ) ,
		( "[*]" ) ,
		( "[+]" ) ,
		( "[|]" ) ,
		( "[-]" ) ,
		( "[.]" ) ,
		( "[/]" ) ,
		( "[F1]" ) ,
		( "[F2]" ) ,
		( "[F3]" ) ,
		( "[F4]" ) ,
		( "[F5]" ) ,
		( "[F6]" ) ,
		( "[F7]" ) ,
		( "[F8]" ) ,
		( "[F9]" ) ,
		( "[F10]" ) ,
		( "[F11]" ) ,
		( "[F12]" ) ,
		( "[F13]" ) ,
		( "[F14]" ) ,
		( "[F15]" ) ,
		( "[F16]" ) ,
		( "[F17]" ) ,
		( "[F18]" ) ,
		( "[F19]" ) ,
		( "[F20]" ) ,
		( "[F21]" ) ,
		( "[F22]" ) ,
		( "[F23]" ) ,
		( "[F24]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[NUMLOCK]" ) ,
		( "[SCR]" ) ,
		( "[=]" ) ,
		( "[MAS]" ) ,
		( "[TOY]" ) ,
		( "[OYA]" ) ,
		( "[OYA]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[-]" ) ,
		( "[SHIFT]" ) ,
		( "[SHIFT]" ) ,
		( "[CTRL]" ) ,
		( "[CTRL]" ) ,
		( "[ALT]" ) ,
		( "[ALT]" )
	};

	/**
	  * to have Combo/ListBox with std::vector<std::string>
	  * https://eliasdaler.github.io/using-imgui-with-sfml-pt2/#imgui-demo-window
	*/
	static auto vector_getter = [ ] ( void* vec , int idx , const char** out_text )
	{
		auto& vector = *static_cast< std::vector< std::string >* >( vec );
		if ( idx < 0 || idx >= static_cast< int >( vector.size( ) ) ) { return false; }
		*out_text = vector.at( idx ).c_str( );
		return true;
	};

	bool combo( const char* label , int* currIndex , std::vector<std::string>& values )
	{
		if ( values.empty( ) ) { return false; }
		return ImGui::Combo( label , currIndex , vector_getter ,
			static_cast< void* >( &values ) , values.size( ) );
	}

	bool listbox( const char* label , int* currIndex , std::vector<std::string>& values )
	{
		if ( values.empty( ) ) { return false; }
		return ImGui::ListBox( label , currIndex , vector_getter ,
			static_cast< void* >( &values ) , values.size( ) );
	}

	/* https://github.com/ocornut/imgui/blob/master/misc/cpp/imgui_stdlib.cpp */
	struct InputTextCallback_UserData
	{
		std::string* Str;
		ImGuiInputTextCallback  ChainCallback;
		void* ChainCallbackUserData;
	};

	static int InputTextCallback( ImGuiInputTextCallbackData* data )
	{
		InputTextCallback_UserData* user_data = ( InputTextCallback_UserData* ) data->UserData;
		if ( data->EventFlag == ImGuiInputTextFlags_CallbackResize )
		{
			std::string* str = user_data->Str;
			IM_ASSERT( data->Buf == str->c_str( ) );
			str->resize( data->BufTextLen );
			data->Buf = ( char* ) str->c_str( );
		}
		else if ( user_data->ChainCallback )
		{
			data->UserData = user_data->ChainCallbackUserData;
			return user_data->ChainCallback( data );
		}

		return 0;
	}

	bool input_text( const char* label , std::string* str , ImGuiInputTextFlags flags = 0 , ImGuiInputTextCallback callback = NULL , void* user_data = NULL )
	{
		IM_ASSERT( ( flags & ImGuiInputTextFlags_CallbackResize ) == 0 );
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;
		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;
		return ImGui::InputText( label , ( char* ) str->c_str( ) , str->capacity( ) + 1 , flags , InputTextCallback , &cb_user_data );
	}

	bool begin( const char* name , bool* p_open = NULL , ImGuiWindowFlags flags = 0 ) {
		return ImGui::Begin( name , p_open , flags );
	}

	void end( ) {
		return ImGui::End( );
	}

	bool begin_child( const char* str_id , const ImVec2& size = ImVec2( 0 , 0 ) , bool border = false , ImGuiWindowFlags flags = 0 ) {
		return ImGui::BeginChild( str_id , size , border , flags );
	}

	bool begin_child( ImGuiID id , const ImVec2& size = ImVec2( 0 , 0 ) , bool border = false , ImGuiWindowFlags flags = 0 ) {
		return ImGui::BeginChild( id , size , border , flags );
	}

	void end_child( ) {
		return ImGui::EndChild( );
	}

	void begin_group( ) {
		return ImGui::BeginGroup( );
	}

	void end_group( ) {
		return ImGui::EndGroup( );
	}

	void push_style_color( ImGuiCol idx , ImU32 col ) {
		return ImGui::PushStyleColor( idx , col );
	}

	void push_style_color( ImGuiCol idx , const ImVec4& col ) {
		return ImGui::PushStyleColor( idx , col );
	}

	void pop_style_color( int count = 1 ) {
		return ImGui::PopStyleColor( count );
	}

	void push_style_var( ImGuiStyleVar idx , float val ) {
		return ImGui::PushStyleVar( idx , val );
	}

	void push_style_var( ImGuiStyleVar idx , const ImVec2& val ) {
		return ImGui::PushStyleVar( idx , val );
	}

	void pop_style_var( int count = 1 ) {
		return ImGui::PopStyleVar( count );
	}

	ImVec2 get_window_pos( ) {
		return ImGui::GetWindowPos( );
	}

	ImVec2 get_window_size( ) {
		return ImGui::GetWindowSize( );
	}

	void set_next_window_pos( const ImVec2& pos , ImGuiCond cond = 0 , const ImVec2& pivot = ImVec2( 0 , 0 ) ) {
		return ImGui::SetNextWindowPos( pos , cond , pivot );
	}

	void set_next_window_size( const ImVec2& size , ImGuiCond cond = 0 ) {
		return ImGui::SetNextWindowSize( size , cond );
	}

	void same_line( float offset_from_start_x = 0.0f , float spacing = -1.0f ) {
		return ImGui::SameLine( offset_from_start_x , spacing );
	}

	void spacing( ) {
		return ImGui::Spacing( );
	}

	void push_font( ImFont* font ) {
		return ImGui::PushFont( font );
	}

	void pop_font( ) {
		return ImGui::PopFont( );
	}

	bool button( const char* label , const ImVec2& size = ImVec2( 0 , 0 ) ) {
		return ImGui::Button( label , size );
	}

	bool checkbox( const char* label , std::string var_name ) {
		auto& var = vars::find_var( var_name );
		bool v = var.get< bool >( );
		bool ret = ImGui::Checkbox( label , &v );
		var.set< bool >( v );
		return ret;
	}

	bool color_picker( const char* label , std::string var_name , bool alpha = true , bool sameline = true ) {
		bool ret = false;

		if ( sameline ) {
			ImGui::SameLine( );
		}

		auto& var = vars::find_var( var_name );
		color var_to_clr = var.get_color( );

		ImVec4 clr = ImVec4(
			var_to_clr.r / 255.0f ,
			var_to_clr.g / 255.0f ,
			var_to_clr.b / 255.0f ,
			var_to_clr.a / 255.0f
		);

		std::string id_colorbutton_new = label;
		id_colorbutton_new += "##color_button";

		std::string id_new = label;
		id_new += "##picker";

		bool open_popup = ImGui::ColorButton( id_colorbutton_new.c_str( ) , clr ,
			ImGuiColorEditFlags_NoSidePreview |
			ImGuiColorEditFlags_NoSmallPreview |
			ImGuiColorEditFlags_AlphaPreview |
			ImGuiColorEditFlags_NoTooltip /*, ImVec2( 20 , 0 )*/ );

		if ( open_popup )
		{
			ImGui::OpenPopup( id_new.c_str( ) );
		}

		if ( ImGui::BeginPopup( id_new.c_str( ) ) )
		{
			if ( ImGui::ColorPicker4( id_new.c_str( ) , &clr.x , ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_PickerHueBar | ( alpha ? ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar : 0 ) ) )
			{
				var_to_clr.r = ( int ) std::roundf( clr.x * 255.f );
				var_to_clr.g = ( int ) std::roundf( clr.y * 255.f );
				var_to_clr.b = ( int ) std::roundf( clr.z * 255.f );

				if ( alpha ) {
					var_to_clr.a = ( int ) std::roundf( clr.w * 255.f );
				}

				var.set_color( var_to_clr );
				ret = true;
			}

			ImGui::EndPopup( );
		}

		return ret;
	}

	// or actually: https://github.com/spirthack/CSGOSimple/blob/master/CSGOSimple/UI.cpp#L287
	bool keybind( const char* str_id , std::string current_key , std::string key_style = "" ) {
		ImGuiWindow* window = ImGui::GetCurrentWindow( );
		if ( window->SkipItems )
			return false;

		auto& current_key_var = vars::find_var( current_key );

		ImGuiContext& g = *GImGui;

		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID( str_id );
		ImGuiIO& io = g.IO;

		const ImVec2 str_id_size = ImGui::CalcTextSize( str_id , NULL , true );
		const ImVec2 label_size = ImGui::CalcTextSize( keys [ current_key_var.get< int >( ) ] );
		const ImRect frame_bb( window->DC.CursorPos , window->DC.CursorPos + label_size );
		const ImRect total_bb( window->DC.CursorPos + ImVec2( str_id_size.x + 4 , 0 ) , window->DC.CursorPos + ImVec2( frame_bb.Max.x + style.FramePadding.x + 4 , label_size.y ) );

		ImGui::ItemSize( total_bb , style.FramePadding.y );
		if ( !ImGui::ItemAdd( total_bb , id , &frame_bb ) )
			return false;

		const bool hovered = ImGui::IsItemHovered( );
		const bool edit_requested = hovered && io.MouseClicked [ 0 ];
		const bool style_requested = hovered && io.MouseClicked [ 1 ];

		static bool check_already_released_lbutton = false; // when we want to pick key and check for released, since we press lbutton to activate id, we need to wait till we release the key

		if ( edit_requested ) {
			if ( g.ActiveId != id ) {
				memset( io.MouseDown , 0 , sizeof( io.MouseDown ) );
				memset( io.KeysDown , 0 , sizeof( io.KeysDown ) );
				current_key_var.set< int >( 0 );
				check_already_released_lbutton = false;
			}

			ImGui::SetActiveID( id , window );
			ImGui::FocusWindow( window );
		}
		else if ( !hovered && io.MouseClicked [ 0 ] && g.ActiveId == id )
			ImGui::ClearActiveID( );

		bool value_changed = false;
		int key = current_key_var.get< int >( );

		if ( g.ActiveId == id ) {
			if ( input::released( VK_LBUTTON ) && !value_changed && check_already_released_lbutton ) {
				key = VK_LBUTTON;
				value_changed = true;
				ImGui::ClearActiveID( );
			}

			if ( !input::get_key_data( VK_LBUTTON ).PrevKeyState && !input::get_key_data( VK_LBUTTON ).KeyState ) {
				check_already_released_lbutton = true;
			}

			if ( input::released( VK_RBUTTON ) && !value_changed ) {
				key = VK_RBUTTON;
				value_changed = true;
				ImGui::ClearActiveID( );
			}

			if ( input::released( VK_MBUTTON ) && !value_changed ) {
				key = VK_MBUTTON;
				value_changed = true;
				ImGui::ClearActiveID( );
			}

			if ( input::released( VK_XBUTTON1 ) && !value_changed ) {
				key = VK_XBUTTON1;
				value_changed = true;
				ImGui::ClearActiveID( );
			}

			if ( input::released( VK_XBUTTON2 ) && !value_changed ) {
				key = VK_XBUTTON2;
				value_changed = true;
				ImGui::ClearActiveID( );
			}

			/*for ( auto i = 0; i < 5; i++ ) {
				if ( io.MouseDown [ i ] ) {
					switch ( i ) {
					case 0:
						key = VK_LBUTTON;
						break;
					case 1:
						key = VK_RBUTTON;
						break;
					case 2:
						key = VK_MBUTTON;
						break;
					case 3:
						key = VK_XBUTTON1;
						break;
					case 4:
						key = VK_XBUTTON2;
					}
					value_changed = true;
					ImGui::ClearActiveID( );
				}
			}*/

			if ( !value_changed ) {
				for ( auto i = VK_BACK; i <= VK_RMENU; i++ ) {
					if ( /*io.KeysDown [ i ]*/ input::released( i ) && !value_changed ) {
						key = i;
						value_changed = true;
						ImGui::ClearActiveID( );
					}
				}
			}

			if ( ImGui::IsKeyPressedMap( ImGuiKey_Escape ) ) {
				current_key_var.set< int >( 0 );
				ImGui::ClearActiveID( );
			}
			else {
				current_key_var.set< int >( key );
			}
		}
		else {
			if ( key_style.size( ) ) {
				auto& key_style_var = vars::find_var( key_style );

				bool popup_open = ImGui::IsPopupOpen( id );

				if ( style_requested && !popup_open )
					ImGui::OpenPopupEx( id );

				if ( popup_open ) {
					ImGui::SetNextWindowSize( ImVec2( 100 , 80 ) );
					ImGui::SetNextWindowPos( ImVec2( frame_bb.Min.x + style.FramePadding.x + str_id_size.x + 4 - 102 , frame_bb.Min.y - 2 ) );

					char name [ 16 ];
					ImFormatString( name , IM_ARRAYSIZE( name ) , "##Combo_%02d" , g.BeginPopupStack.Size ); // Recycle windows based on depth

#if 0 // sets the position on the way left of the child, so no.
				// Peak into expected window size so we can position it
					if ( ImGuiWindow* popup_window = FindWindowByName( name ) )
						if ( popup_window->WasActive )
						{
							ImVec2 size_expected = ImGui::CalcWindowExpectedSize( popup_window );
							ImRect r_outer = ImGui::GetWindowAllowedExtentRect( popup_window );
							ImVec2 pos = ImGui::FindBestWindowPosForPopupEx( frame_bb.GetBL( ) , size_expected , &popup_window->AutoPosLastDirection , r_outer , frame_bb , ImGuiPopupPositionPolicy_ComboBox );
							ImGui::SetNextWindowPos( pos );
						}
#endif

					// Horizontally align ourselves with the framed text
					ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
					ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding , ImVec2( -1 , 7 ) );
					bool ret = ImGui::Begin( name , NULL , window_flags );
					ImGui::PopStyleVar( );

					// Disabled, On Hold, On Toggle, Always On

					if ( ImGui::Selectable( "Disabled" , key_style_var.get< int >( ) == 0 ) )
						key_style_var.set< int >( 0 );

					if ( ImGui::Selectable( "On Hold" , key_style_var.get< int >( ) == 1 ) )
						key_style_var.set< int >( 1 );

					if ( ImGui::Selectable( "On Toggle" , key_style_var.get< int >( ) == 2 ) )
						key_style_var.set< int >( 2 );

					if ( ImGui::Selectable( "Always On" , key_style_var.get< int >( ) == 3 ) )
						key_style_var.set< int >( 3 );

					ImGui::EndPopup( );
				}
			}
		}

		char buf_display [ 64 ] = "[-]";

		if ( current_key_var.get< int >( ) != 0 && g.ActiveId != id )
			strcpy_s( buf_display , keys [ current_key_var.get< int >( ) ] );
		else if ( g.ActiveId == id )
			strcpy_s( buf_display , "[...]" );
		else
			strcpy_s( buf_display , "[-]" );

		ImGui::RenderText( ImVec2( frame_bb.Min.x + style.FramePadding.x + str_id_size.x + 4 , frame_bb.Min.y - 2 ) , buf_display );

		if ( str_id_size.x > 0 )
			ImGui::RenderText( ImVec2( frame_bb.Min.x , frame_bb.Min.y ) , str_id );

		return value_changed;
	}

	bool slider_int( const char* label , std::string var_name , int v_min , int v_max , const char* format = "%d" ) {
		auto& var = vars::find_var( var_name );
		int v = var.get< int >( );
		bool ret = ImGui::SliderInt( label , &v , v_min , v_max , format );
		var.set< int >( v );
		return ret;
	}

	bool slider_float( const char* label , std::string var_name , float v_min , float v_max , const char* format = "%.2f" , float power = 1.0f ) {
		auto& var = vars::find_var( var_name );
		float v = var.get< float >( );
		bool ret = ImGui::SliderFloat( label , &v , v_min , v_max , format , power );
		var.set< float >( v );
		return ret;
	}

	bool begin_listbox( const char* label , const ImVec2& size = ImVec2( 0 , 0 ) ) {
		return ImGui::ListBoxHeader( label , size );
	}

	bool begin_listbox( const char* label , int items_count , int height_in_items = -1 ) {
		return ImGui::ListBoxHeader( label , items_count , height_in_items );
	}

	void end_listbox( ) {
		return ImGui::ListBoxFooter( );
	}

	bool multi_combobox( const char* label , std::vector< std::string > variable , std::vector< std::string >& labels ) {
		static std::string preview_value = "None";

		// reset, cuz yeah
		preview_value = "None";

		// if it's empty or is not the same size, then rip my guy
		if ( variable.size( ) != labels.size( ) || !variable.size( ) )
			return false;

		for ( int i = 0 , j = 0; i < ( int ) variable.size( ); i++ ) {
			if ( vars::find_var( variable [ i ] ).get< bool >( ) ) {
				if ( j )
					preview_value += ", " + labels [ i ];
				else
					preview_value = labels [ i ];

				j++;
			}
		}

		bool variable_changed = false;
		if ( ImGui::BeginCombo( label , preview_value.c_str( ) ) ) {
			for ( int i = 0; i < ( int ) variable.size( ); i++ ) {
				auto& var = vars::find_var( variable [ i ] );
				bool v = var.get< bool >( );
				variable_changed = ImGui::Selectable( labels [ i ].c_str( ) , &v , ImGuiSelectableFlags_DontClosePopups );
				var.set< bool >( v );
			}

			ImGui::EndCombo( );
		}

		return variable_changed;
	}
}

bool menu::init( IDirect3DDevice9* pDevice , HWND hWindow )
{
	static bool bInitialized = false;
	if ( bInitialized ) {
		util::console::set_prefix( util::console::WARN );
		util::console::print( "multiple calls to menu::init()\n" );
		return true;
	}

	bInitialized = true;

	util::console::set_prefix( util::console::NONE );
	util::console::print( "initializing menu..\n" );

	ImGui::CreateContext( );

	menu::set_style( true );

	if ( !ImGui_ImplWin32_Init( hWindow ) )
		return false;

	if ( !ImGui_ImplDX9_Init( pDevice ) )
		return false;

	util::console::set_prefix( util::console::NONE );
	util::console::print( "initialized menu\n" );

	return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam );
LRESULT CALLBACK menu::wnd_proc( HWND hWnd , UINT message , WPARAM wParam , LPARAM lParam )
{
	if ( message == WM_KEYUP && wParam == VK_INSERT )
		menu::m_opened = !menu::m_opened;

	if ( !menu::m_opened ) {
		return false;
	}

	if ( !ImGui_ImplWin32_WndProcHandler( hWnd , message , wParam , lParam ) )
		return true;

	/* ImGui Wants Text Input so... */
	if ( ImGui::GetIO( ).WantTextInput )
		return true;

	/* block game input for some messages */
	switch ( message )
	{
		/* pressing mouse key */
	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
	case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:

		/* scrolling */
	case WM_MOUSEWHEEL:

		/* after releasing mouse key */
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:

		/* when moving mouse */
	case WM_MOUSEMOVE:

		/* don't allow game input */
		return true;
	}

	return false;
}

void menu::set_style( bool copy_original )
{
	static bool never_set_style_before = true;

	if ( never_set_style_before )
		m_original_style = ImGui::GetStyle( );

	if ( copy_original ) {
		memcpy( &m_style , &m_original_style , sizeof( ImGuiStyle ) );
	}

	ImGuiIO& io = ImGui::GetIO( );
	m_title_font = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Arial.ttf" , 24.f , NULL , io.Fonts->GetGlyphRangesDefault( ) );
	m_big_font = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Tahoma.ttf" , 34.f , NULL , io.Fonts->GetGlyphRangesDefault( ) );
	m_tabs_font = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Calibri.ttf" , 16.f , NULL , io.Fonts->GetGlyphRangesDefault( ) );
	m_font = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Verdana.ttf" , 14.f , NULL , io.Fonts->GetGlyphRangesDefault( ) );

	// btw make it re-sized with dpi

	m_style.Alpha = 1.f;
	m_style.WindowTitleAlign = ImVec2( 0.5f , 0.5f );
	m_style.ItemSpacing = ImVec2( 6.f , 3.f );
	m_style.ItemInnerSpacing = ImVec2( 6.f , 4.f );
	m_style.SelectableTextAlign = ImVec2( 0.5f , 0.5f );
	m_style.FrameRounding = 0.f;
	m_style.ColumnsMinSpacing = 0.f;
	m_style.GrabRounding = 2.f;
	m_style.AntiAliasedLines = true;
	m_style.WindowRounding = 0.f;
	m_style.FramePadding = ImVec2( 2.f , 2.f );
	m_style.ScrollbarSize = 6.f;
	m_style.ScrollbarRounding = 0.f;
	m_style.GrabMinSize = 6.f;
	m_style.TabRounding = 0.f;

	const auto MainMenuColor = ImColor(
		menu::get_main_color( ).r ,
		menu::get_main_color( ).g ,
		menu::get_main_color( ).b ,
		menu::get_main_color( ).a
	).Value;

	m_style.Colors [ ImGuiCol_Text ] = ImVec4( 0.90f , 0.90f , 0.90f , 1.00f );
	m_style.Colors [ ImGuiCol_TextDisabled ] = ImVec4( 0.50f , 0.50f , 0.50f , 1.00f );
	m_style.Colors [ ImGuiCol_WindowBg ] = ImColor( 9 , 9 , 9 , 240 ); // ImVec4( 0.04f, 0.04f, 0.04f /*12*/, 0.9f /*230*/ );
	m_style.Colors [ ImGuiCol_ChildBg ] = ImColor( 5 , 5 , 5 , 255 ); // ImVec4( 0.06f, 0.06f, 0.06f, 1.f );
	m_style.Colors [ ImGuiCol_PopupBg ] = ImVec4( 0.08f , 0.08f , 0.08f , 0.94f );
	m_style.Colors [ ImGuiCol_Border ] = ImVec4( 0.06f , 0.06f , 0.06f , 0.2f );//MainMenuColor;
	//m_style.Colors [ ImGuiCol_BorderShadow ] = ImVec4( 0.04f , 0.04f , 0.04f , 0.5f );//ImVec4( 0.06f, 0.06f, 0.06f, 0.01f );
	m_style.Colors [ ImGuiCol_BorderShadow ] = ImColor( 7 , 7 , 7 , 10 );
	m_style.Colors [ ImGuiCol_FrameBg ] = ImVec4( 0.10f , 0.10f , 0.10f , 0.71f );
	m_style.Colors [ ImGuiCol_FrameBgHovered ] = MainMenuColor;
	m_style.Colors [ ImGuiCol_FrameBgActive ] = MainMenuColor;
	m_style.Colors [ ImGuiCol_TitleBg ] = ImVec4( 0.07f , 0.07f , 0.07f , 0.48f );
	m_style.Colors [ ImGuiCol_TitleBgActive ] = ImVec4( 0.07f , 0.07f , 0.07f , 0.48f );
	m_style.Colors [ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.07f , 0.07f , 0.07f , 0.48f );
	m_style.Colors [ ImGuiCol_MenuBarBg ] = ImVec4( 0.10f , 0.10f , 0.10f , 0.66f );
	m_style.Colors [ ImGuiCol_ScrollbarBg ] = ImVec4( 0.02f , 0.02f , 0.02f , 0.00f );
	m_style.Colors [ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.31f , 0.31f , 0.31f , 1.00f );
	m_style.Colors [ ImGuiCol_ScrollbarGrabHovered ] = MainMenuColor;
	m_style.Colors [ ImGuiCol_ScrollbarGrabActive ] = MainMenuColor;
	m_style.Colors [ ImGuiCol_CheckMark ] = MainMenuColor;
	m_style.Colors [ ImGuiCol_SliderGrab ] = ImVec4( 0.29f , 0.29f , 0.29f , 1.00f );
	m_style.Colors [ ImGuiCol_SliderGrabActive ] = MainMenuColor;
	m_style.Colors [ ImGuiCol_Button ] = ImVec4( 0.01f , 0.01f , 0.01f , 1.00f );
	m_style.Colors [ ImGuiCol_ButtonHovered ] = MainMenuColor;
	m_style.Colors [ ImGuiCol_ButtonActive ] = MainMenuColor;

	m_style.Colors [ ImGuiCol_Header ] = MainMenuColor;
	m_style.Colors [ ImGuiCol_HeaderHovered ] = MainMenuColor;
	m_style.Colors [ ImGuiCol_HeaderActive ] = MainMenuColor;
	m_style.Colors [ ImGuiCol_ResizeGrip ] = ImVec4( 0.26f , 0.59f , 0.98f , 0.25f );
	m_style.Colors [ ImGuiCol_ResizeGripHovered ] = ImVec4( 0.26f , 0.59f , 0.98f , 0.67f );
	m_style.Colors [ ImGuiCol_ResizeGripActive ] = ImVec4( 0.26f , 0.59f , 0.98f , 0.95f );
	m_style.Colors [ ImGuiCol_PlotLines ] = ImVec4( 0.61f , 0.61f , 0.61f , 1.00f );
	m_style.Colors [ ImGuiCol_PlotLinesHovered ] = ImVec4( 1.00f , 1.00f , 1.00f , 1.00f );
	m_style.Colors [ ImGuiCol_PlotHistogram ] = ImVec4( 0.90f , 0.70f , 0.00f , 1.00f );
	m_style.Colors [ ImGuiCol_PlotHistogramHovered ] = ImVec4( 1.00f , 0.60f , 0.00f , 1.00f );
	m_style.Colors [ ImGuiCol_TextSelectedBg ] = MainMenuColor;
	m_style.Colors [ ImGuiCol_ModalWindowDarkening ] = ImVec4( 0.80f , 0.80f , 0.80f , 0.35f );

	// set gui style
	ImGui::GetStyle( ) = m_style;
	never_set_style_before = false;
}

void menu::on_end_scene( )
{
	// first frame this is called, toggle menu visible
	static bool first_frame_called = false;
	if ( !first_frame_called ) {
		menu::m_opened = true;
		first_frame_called = true;
	}

	ImGui_ImplDX9_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	// do menu only if it's opened
	if ( menu::m_opened ) {
		gui::push_font( m_font );

		//const ImVec2 window_size = ImVec2( 800 , 440 );
		const ImVec2 window_size = ImVec2(
			g_renderer.get_screen_size( ).w <= 800 ? g_renderer.get_screen_size( ).w - 20 : 800 ,
			g_renderer.get_screen_size( ).h <= 440 ? g_renderer.get_screen_size( ).h - 20 : 440 );

		gui::set_next_window_pos( ImVec2(
			( g_renderer.get_screen_size( ).w / 2 ) - ( window_size.x / 2 ) ,
			( g_renderer.get_screen_size( ).h / 2 ) - ( window_size.y / 2 )
		) , ImGuiCond_Appearing );

		gui::set_next_window_size( window_size , ImGuiCond_Always );

		gui::begin( "snek.tk##main" , nullptr , ImGuiWindowFlags_NoDecoration ); {
			// tip; main menu color should be a bright white
			const auto MainMenuColor = ImColor(
				menu::get_main_color( ).r ,
				menu::get_main_color( ).g ,
				menu::get_main_color( ).b ,
				menu::get_main_color( ).a
			).Value;

			enum tabs_t {
				TAB_INVALID = -1 ,
				TAB_RAGEBOT ,
				TAB_ANTIAIM ,
				TAB_VISUALS ,
				TAB_MISC ,
				TAB_CONFIG
			};

			const char* _vTabsList [ ] = {
				"Ragebot" ,
				"Anti-Aim" ,
				"Visuals" ,
				"Misc" ,
				"Config"
			};

			static int current_tab = TAB_INVALID;

			// Tab selector 
			gui::begin_group( ); {
				// Title
				gui::push_font( m_title_font );
				ImVec2 title_text_size = ImGui::CalcTextSize( "snek.tk |" );
				ImGui::TextColored( ImColor( 200 , 200 , 200 ).Value , "snek.tk |" );
				gui::pop_font( );

				gui::same_line( );
				gui::spacing( );
				gui::same_line( );
				gui::spacing( );
				gui::same_line( );

				ImGui::SetCursorPosY( ImGui::GetCursorPos( ).y + ( title_text_size.y / 2 ) - 10 );

				gui::push_font( m_tabs_font );
				for ( int i = 0; i < ARRAYSIZE( _vTabsList ); ++i ) {
					gui::push_style_color( ImGuiCol_Button , ImVec4( ) );
					gui::push_style_color( ImGuiCol_ButtonHovered , ImVec4( ) );
					gui::push_style_color( ImGuiCol_ButtonActive , ImVec4( ) );
					gui::push_style_color( ImGuiCol_Text , ( current_tab == i ) ? ImVec4( 0.90f , 0.90f , 0.90f , 1.f ) : MainMenuColor );

					float backup_padding_y = ImGui::GetStyle( ).FramePadding.y;
					ImGui::GetStyle( ).FramePadding.y = 0.0f;

					if ( gui::button( _vTabsList [ i ] , ImVec2( 0 , 20 ) ) ) {
						current_tab = i;
					}

					ImGui::GetStyle( ).FramePadding.y = backup_padding_y;

					gui::same_line( );

					gui::pop_style_color( 4 );
				}
				gui::pop_font( );

			} gui::end_group( );

			gui::begin_child( "##main_child" , ImVec2( -1 , -1 ) , true ); {
				/* push colors for new childs */
				gui::push_style_color( ImGuiCol_ChildBg , ImVec4( 0.04f , 0.04f , 0.04f , 1.f ) );
				gui::push_style_color( ImGuiCol_Border , ImVec4( 0.01f , 0.01f , 0.01f , 0.6f ) );

				auto get_perc = [ ] ( int perc ) {
					return ( ( ( float ) perc ) * 0.01f );
				};

				// do tabs
				switch ( current_tab ) {
				case TAB_INVALID: {
				} break;
				case TAB_RAGEBOT: {
					gui::checkbox( "Test Checkbox" , "menu.test" );
					gui::color_picker( "Test ColorPicker" , "menu.test_color" );
				} break;
				case TAB_ANTIAIM: {

				} break;
				case TAB_VISUALS: {

				} break;
				case TAB_MISC: {

				} break;
				case TAB_CONFIG: {
					// create 2 columns with offset
					ImGui::Columns( 2 , NULL , false );
					ImGui::SetColumnOffset( 1 , max( ( ImGui::GetWindowContentRegionWidth( ) - ImGui::GetStyle( ).ItemSpacing.x ) / 2.0f , 1.0f ) ); // grabbed the formula from some imgui issue, aka 3596

					static std::string ConfigName = "default";

					gui::begin_child( "List" , ImVec2( -1 , -1 ) , true ); {
						for ( auto& cfg : vars::config::get_configs( ) ) {
							bool is_current = cfg == ConfigName;

							gui::push_style_color( ImGuiCol_Text , is_current ? MainMenuColor : ImVec4( 0.90f , 0.90f , 0.90f , 1.f ) );
							gui::push_style_color( ImGuiCol_ButtonHovered , ImVec4( 0.01f , 0.01f , 0.01f , 1.00f ) );
							gui::push_style_color( ImGuiCol_ButtonActive , ImVec4( 0.01f , 0.01f , 0.01f , 1.00f ) );

							if ( gui::button( ( cfg + "##ConfigName" ).c_str( ) , ImVec2( -1 , 0 ) ) ) {
								ConfigName = cfg;
							}

							gui::pop_style_color( 3 );
						}

						gui::end_child( );
					}

					// jump to next column
					ImGui::NextColumn( );

					gui::begin_child( "Main" , ImVec2( -1 , -1 ) , true ); {
						gui::input_text( "Config Name" , &ConfigName , ImGuiInputTextFlags_CharsNoBlank );

						if ( gui::button( "Save Config" , ImVec2( -1 , 0 ) ) ) {
							bool succeded = vars::config::save( ConfigName + ".json" );
							if ( succeded ) {
								//features::misc::notifications::add( "saved config: " + ConfigName );
							}
							else {
								//features::misc::notifications::add( "failed to save config: " + ConfigName , color::red( ) );
							}
						}

						if ( gui::button( "Load Config" , ImVec2( -1 , 0 ) ) ) {
							bool succeded = vars::config::load( ConfigName + ".json" );
							if ( succeded ) {
								//features::misc::notifications::add( "loaded config: " + ConfigName );
							}
							else {
								//features::misc::notifications::add( "failed to load config: " + ConfigName , color::red( ) );
							}
						}

						/*if ( gui::button( "Import from Clipboard" , ImVec2( -1 , 0 ) ) ) {
							bool succeded = vars::config::import_from_clipboard( ConfigName + ".json" );
							if ( succeded ) {
								features::misc::notifications::add( "imported config: " + ConfigName );
							}
							else {
								features::misc::notifications::add( "failed to import config: " + ConfigName , color::red( ) );
							}
						}

						if ( gui::button( "Export to Clipboard" , ImVec2( -1 , 0 ) ) ) {
							bool succeded = vars::config::export_to_clipboard( ConfigName + ".json" );
							if ( succeded ) {
								features::misc::notifications::add( "exported config: " + ConfigName );
							}
							else {
								features::misc::notifications::add( "failed to export config: " + ConfigName , color::red( ) );
							}
						}*/

						if ( gui::button( "Delete Config" , ImVec2( -1 , 0 ) ) ) {
							remove( ( vars::config::get_configs_folder( ) + ConfigName + ".json" ).c_str( ) );
							//features::misc::notifications::add( "deleted config: " + ConfigName );
						}

						gui::end_child( );
					}

					// reset to 1 column back
					ImGui::Columns( 1 , NULL , false );
				} break;
				}

				/* pop colors */
				gui::pop_style_color( 2 );

				gui::end_child( );
			}

			gui::end( );
		}

		gui::pop_font( );
	}

	ImGui::EndFrame( );
	ImGui::Render( );
	ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
}

void menu::on_lost_device( )
{
	ImGui_ImplDX9_InvalidateDeviceObjects( );
}

void menu::on_reset_device( )
{
	ImGui_ImplDX9_CreateDeviceObjects( );
}

