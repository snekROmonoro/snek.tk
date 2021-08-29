#pragma once
#include <vector>
#include <map>
#include <string>
#include <unordered_map>

class RecvTable;
class RecvProp;

namespace sdk {
	namespace netvar {
		struct netvar_data_t {
			bool m_datamap_var; // we can't do proxies on stuff from datamaps :).
			RecvProp* m_prop_ptr;
			std::size_t m_offset;

			netvar_data_t( ) : m_datamap_var { } , m_prop_ptr { } , m_offset { } { }
		};

		extern std::unordered_map< std::string , int > m_client_ids;
		extern std::unordered_map< std::string , std::unordered_map< std::string , netvar_data_t > > m_offsets;

		bool init( );
		bool store_client_ids( );
		void store_table( const std::string& name , RecvTable* table , std::size_t offset = 0 );
		int get_client_id( const std::string& network_name );
		int get( const std::string& table , const std::string& prop );
	}
}
