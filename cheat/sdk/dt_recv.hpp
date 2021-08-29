#pragma once
#include "../util/vfunc/vfunc.hpp"

class RecvTable;
class RecvProp;

class RecvProp
{
	// This info comes from the receive data table.
public:
	char* m_pVarName;
	int m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray; // Set to true by the engine if this property sits inside an array.

	// Extra data that certain special property types bind to the property here.
	const void* m_pExtraData;
	
	// If this is an array (DPT_Array).
	RecvProp* m_pArrayProp;
	void* m_ArrayLengthProxy;

	void* m_ProxyFn;
	void* m_DataTableProxyFn; // For RDT_DataTable.

	RecvTable* m_pDataTable; // For RDT_DataTable.
	int m_Offset;
	
	int m_ElementStride;
	int m_nElements;

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char* m_pParentArrayPropName;
};

class RecvTable
{
public:
	// Properties described in a table.
	RecvProp* m_pProps;
	int				m_nProps;

	// The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
	// will have their own decoders that include props for all their children).
	void* m_pDecoder;

	char* m_pNetTableName;	// The name matched between client and server.

	PAD( 2 );
};
