#include "netvar.hpp"
#include "../../includes.hpp"

std::unordered_map< std::string , int > sdk::netvar::m_client_ids;
std::unordered_map< std::string , std::unordered_map< std::string , sdk::netvar::netvar_data_t > > sdk::netvar::m_offsets;

bool sdk::netvar::init( )
{
	// first store client-ids
	if ( !store_client_ids( ) )
		return false;

	util::console::set_prefix( util::console::SDK );
	util::console::print( "searching netvars..\n" );

	if ( !sdk::interfaces::client ) {
		util::console::set_prefix( util::console::ERR );
		util::console::print( "sdk::interfaces::client == nullptr\n" );
		util::console::set_prefix( util::console::NONE );

		return false;
	}

	auto list = sdk::interfaces::client->GetAllClasses( );
	if ( !list ) {
		util::console::set_prefix( util::console::ERR );
		util::console::print( "client class list is nullptr\n" );
		util::console::set_prefix( util::console::NONE );

		return false;
	}

	for ( ; list != nullptr; list = list->m_pNext ) {
		store_table( list->m_pRecvTable->m_pNetTableName , list->m_pRecvTable );
	}

	util::console::print( "found %d netvars\n" , ( int ) m_offsets.size( ) );
	util::console::set_prefix( util::console::NONE );

	return true;
}

bool sdk::netvar::store_client_ids( ) {
	util::console::set_prefix( util::console::SDK );
	util::console::print( "searching client ids..\n" );

	if ( m_client_ids.size( ) )
		m_client_ids.clear( );

	if ( !sdk::interfaces::client ) {
		util::console::set_prefix( util::console::ERR );
		util::console::print( "sdk::interfaces::client == nullptr\n" );
		util::console::set_prefix( util::console::NONE );

		return false;
	}

	auto list = sdk::interfaces::client->GetAllClasses( );
	if ( !list ) {
		util::console::set_prefix( util::console::ERR );
		util::console::print( "client class list is nullptr\n" );
		util::console::set_prefix( util::console::NONE );

		return false;
	}

	for ( ; list != nullptr; list = list->m_pNext ) {
		m_client_ids [ list->m_pNetworkName ] = list->m_ClassID;
	}

	util::console::print( "found %d client ids\n" , ( int ) m_client_ids.size( ) );
	util::console::set_prefix( util::console::NONE );

	return true;
}

void sdk::netvar::store_table( const std::string& name , RecvTable* table , std::size_t offset ) {
	for ( int i { }; i < table->m_nProps; ++i ) {
		auto prop = &table->m_pProps [ i ];
		auto child = prop->m_pDataTable;

		// we have a child table, that contains props.
		if ( child && child->m_nProps > 0 )
			sdk::netvar::store_table( name , child , prop->m_Offset + offset );

		// insert if not present.
		if ( !m_offsets [ name ][ prop->m_pVarName ].m_offset ) {
			m_offsets [ name ][ prop->m_pVarName ].m_datamap_var = false;
			m_offsets [ name ][ prop->m_pVarName ].m_prop_ptr = prop;
			m_offsets [ name ][ prop->m_pVarName ].m_offset = static_cast< size_t >( prop->m_Offset + offset );
		}
	}
}

int sdk::netvar::get_client_id( const std::string& network_name ) {
	return m_client_ids [ network_name ];
}

int sdk::netvar::get( const std::string& table , const std::string& prop ) {
	return m_offsets [ table ][ prop ].m_offset;
}
