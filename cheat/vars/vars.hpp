#pragma once
#include <vector>
#include <string>
#include <map>
#include "../renderer/color/color.hpp"

namespace vars {
	class CValue
	{
	public:
		CValue( ) : m_value { 0.0 } , m_name { "" } { }

		template < typename T >
		T get( ) {
			return static_cast< T >( m_value );
		}

		color get_color( ) {
			D3DCOLOR d3dcol = static_cast< D3DCOLOR >( m_value );
			return color(
				D3DColorExtractRed( d3dcol ) ,
				D3DColorExtractGreen( d3dcol ) ,
				D3DColorExtractBlue( d3dcol ) ,
				D3DColorExtractAlpha( d3dcol )
			);
		}

		void set_color( color col ) {
			m_value = static_cast< double >( col.to_d3d( ) );
		}

		template < typename T >
		void set( T in ) {
			m_value = static_cast< double >( in );
		}

		std::string get_name( ) {
			return m_name;
		}

		void set_name( std::string name ) {
			m_name = name;
		}

	private:
		double m_value = 0.0;
		std::string m_name = "";
	};

	// m_vars[ hashed_string ] = CValue
	inline std::map< unsigned int , CValue > m_vars;

	CValue& find_var( std::string var_name );

	bool init( );
	void reset_default( ); // reset vars settings to default

	namespace config {
		bool save( std::string config_name );
		bool load( std::string config_name );
		std::vector< std::string > get_configs( ); // Doesn't return names with extension!!

		std::string get_configs_pre_folder( );
		std::string get_configs_folder( );
	}

}
