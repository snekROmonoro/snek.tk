#include "vars.hpp"
#include "../includes.hpp"

vars::CValue& vars::find_var( std::string var_name )
{
	unsigned int hash = std::hash< std::string >( )( var_name );
	if ( vars::m_vars.find( hash ) == vars::m_vars.end( ) ) {
		//MessageBoxA( NULL , std::string( "couldn't find var: " + var_name ).c_str( ) , "ERROR" , MB_OK | MB_ICONERROR );
		util::console::set_prefix( util::console::WARN );
		util::console::print( "couldn't find var: %s\n" , var_name.data( ) );
		util::console::set_prefix( util::console::NONE );

		// just insert default value at hash
		vars::CValue new_var; new_var.set_name( var_name );
		vars::m_vars.insert( std::make_pair( hash , new_var ) );
	}

	return m_vars [ hash ];
}

template< typename T >
void add_var( std::string var_name , T value )
{
	unsigned int hash = std::hash< std::string >( )( var_name );
	vars::CValue new_var;
	new_var.set( value );
	new_var.set_name( var_name );

	// only add in map if hasn't already
	if ( vars::m_vars.find( hash ) == vars::m_vars.end( ) ) {
		vars::m_vars.insert( std::make_pair( hash , new_var ) );
	}

	vars::m_vars [ hash ].set( value );
}

char documents_path [ MAX_PATH ];
std::string configs_pre_folder = "";
std::string configs_folder = "";

bool vars::init( )
{
	util::console::set_prefix( util::console::NONE );
	util::console::print( "initializing vars..\n" );

	if ( !( SUCCEEDED( SHGetFolderPathA( nullptr , CSIDL_PERSONAL , nullptr , SHGFP_TYPE_CURRENT , documents_path ) ) ) ) {
		util::console::set_prefix( util::console::ERR );
		util::console::print( "couldn't find documents path\n" );
		util::console::set_prefix( util::console::NONE );
		return false;
	}

	configs_pre_folder = std::string( documents_path ) + "\\snek.tk\\configs";
	CreateDirectoryA( ( std::string( documents_path ) + "\\snek.tk" ).data( ) , nullptr );

	configs_folder = std::string( documents_path ) + "\\snek.tk\\configs\\";
	CreateDirectoryA( ( std::string( documents_path ) + "\\snek.tk\\configs" ).data( ) , nullptr );

	util::console::set_prefix( util::console::NONE );
	util::console::print( "created folders, creating vars\n" );

	// if you look through the code you'd understand
	reset_default( );

	util::console::set_prefix( util::console::NONE );
	util::console::print( "initialized vars\n" );
	return true;
}

void vars::reset_default( )
{
	add_var( "menu.test" , false );
	add_var( "menu.test_color" , color::white( ).to_d3d( ) );
}

bool vars::config::save( std::string config_name )
{
	std::ofstream ofile( configs_folder + config_name );

	if ( !ofile.good( ) )
		return false;

	nlohmann::json j;
	for ( auto _var : m_vars ) {
		CValue& var = _var.second;
		j [ var.get_name( ) ] = var.get< double >( );
	}

	ofile << std::setw( 4 ) << j << std::endl;
	ofile.close( );
	return true;
}

bool vars::config::load( std::string config_name )
{
	std::ifstream ifile( configs_folder + config_name );

	if ( !ifile.good( ) )
		return false;

	// https://stackoverflow.com/questions/32939253/simple-output-from-json-file-in-c
	nlohmann::json j;

	try
	{
		ifile >> j;
	}
	catch ( ... )
	{
		util::console::set_prefix( util::console::ERR );
		util::console::print( "couldn't parse config: %s\n" , config_name.data( ) );
		util::console::set_prefix( util::console::NONE );

		ifile.close( );
		return false;
	}

	for ( auto& _var : m_vars ) {
		CValue& var = _var.second;
		if ( !j [ var.get_name( ) ].empty( ) ) {
			var.set< double >( j [ var.get_name( ) ].get< double >( ) );
		}
	}

	ifile.close( );
	return true;
}

std::vector< std::string > vars::config::get_configs( )
{
	auto sanitize_name = [ ] ( const std::string& dir ) {
		const auto dot = dir.find_last_of( "." );
		return dir.substr( 0 , dot );
	};

	std::vector< std::string > configs;
	for ( const auto& dir : std::filesystem::recursive_directory_iterator( configs_pre_folder ) ) {
		if ( dir.exists( ) && dir.is_regular_file( ) && ( dir.path( ).extension( ).string( ) == ".json" || dir.path( ).extension( ).string( ) == ".JSON" ) ) {
			const auto sanitized = sanitize_name( dir.path( ).filename( ).string( ) );
			configs.push_back( sanitized );
		}
	}

	return configs;
}

std::string vars::config::get_configs_pre_folder( ) {
	return configs_pre_folder;
}

std::string vars::config::get_configs_folder( ) {
	return configs_folder;
}
