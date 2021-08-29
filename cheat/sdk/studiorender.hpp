#pragma once
#include "studio.hpp"
#include "../math/Vector.hpp"
#include "matrix.hpp"
#include "../renderer/color/color.hpp"
#include "IClientMode.hpp"

enum LightType_t {
	MATERIAL_LIGHT_DISABLE = 0 ,
	MATERIAL_LIGHT_POINT ,
	MATERIAL_LIGHT_DIRECTIONAL ,
	MATERIAL_LIGHT_SPOT ,
};

enum {
	STUDIORENDER_DRAW_ENTIRE_MODEL = 0 ,
	STUDIORENDER_DRAW_OPAQUE_ONLY = 0x01 ,
	STUDIORENDER_DRAW_TRANSLUCENT_ONLY = 0x02 ,
	STUDIORENDER_DRAW_GROUP_MASK = 0x03 ,
	STUDIORENDER_DRAW_NO_FLEXES = 0x04 ,
	STUDIORENDER_DRAW_STATIC_LIGHTING = 0x08 ,
	STUDIORENDER_DRAW_ACCURATETIME = 0x10 ,		// Use accurate timing when drawing the model.
	STUDIORENDER_DRAW_NO_SHADOWS = 0x20 ,
	STUDIORENDER_DRAW_GET_PERF_STATS = 0x40 ,
	STUDIORENDER_DRAW_WIREFRAME = 0x80 ,
	STUDIORENDER_DRAW_ITEM_BLINK = 0x100 ,
	STUDIORENDER_SHADOWDEPTHTEXTURE = 0x200
};

enum {
	MATERIAL_VAR_DEBUG = 1 ,
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = 2 ,
	MATERIAL_VAR_NO_DRAW = 4 ,
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = 8 ,
	MATERIAL_VAR_VERTEXCOLOR = 16 ,
	MATERIAL_VAR_VERTEXALPHA = 32 ,
	MATERIAL_VAR_SELFILLUM = 64 ,
	MATERIAL_VAR_ADDITIVE = 128 ,
	MATERIAL_VAR_ALPHATEST = 256 ,
	MATERIAL_VAR_ZNEARER = 1024 ,
	MATERIAL_VAR_MODEL = 2048 ,
	MATERIAL_VAR_FLAT = 4096 ,
	MATERIAL_VAR_NOCULL = 8192 ,
	MATERIAL_VAR_NOFOG = 16384 ,
	MATERIAL_VAR_IGNOREZ = 32768 ,
	MATERIAL_VAR_DECAL = 65536 ,
	MATERIAL_VAR_ENVMAPSPHERE = 131072 ,
	MATERIAL_VAR_ENVMAPCAMERASPACE = 524288 ,
	MATERIAL_VAR_BASEALPHAENVMAPMASK = 1048576 ,
	MATERIAL_VAR_TRANSLUCENT = 2097152 ,
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = 4194304 ,
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = 8388608 ,
	MATERIAL_VAR_OPAQUETEXTURE = 16777216 ,
	MATERIAL_VAR_ENVMAPMODE = 33554432 ,
	MATERIAL_VAR_SUPPRESS_DECALS = 67108864 ,
	MATERIAL_VAR_HALFLAMBERT = 134217728 ,
	MATERIAL_VAR_WIREFRAME = 268435456 ,
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = 536870912 ,
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = 1073741824 ,
};

struct LightDesc_t {
	LightType_t  m_Type;										//< MATERIAL_LIGHT_xxx
	vec3_t       m_Color;											//< color+intensity 
	vec3_t       m_Position;										//< light source center position
	vec3_t       m_Direction;										//< for SPOT, direction it is pointing
	float        m_Range;											//< distance range for light.0=infinite
	float        m_Falloff;										//< angular falloff exponent for spot lights
	float        m_Attenuation0;									//< constant distance falloff term
	float        m_Attenuation1;									//< linear term of falloff
	float        m_Attenuation2;									//< quadatic term of falloff
	float        m_Theta;											//< inner cone angle. no angular falloff 
	float        m_Phi;											//< outer cone angle
	float        m_ThetaDot;
	float        m_PhiDot;
	float        m_OneOverThetaDotMinusPhiDot;
	unsigned int m_Flags;
protected:
	float        m_RangeSquared;
};

struct studiomeshgroup_t {
	void* m_pMesh;
	int				 m_NumStrips;
	int				 m_Flags;
	void* m_pStripData;
	unsigned short* m_pGroupIndexToMeshIndex;
	int				 m_NumVertices;
	int* m_pUniqueTris;
	unsigned short* m_pIndices;
	bool			 m_MeshNeedsRestore;
	short			 m_ColorMeshID;
	void* m_pMorph;
};

struct studiomeshdata_t {
	int					m_NumGroup;
	studiomeshgroup_t* m_pMeshGroup;
};

class IMaterial;

struct studioloddata_t {
	studiomeshdata_t* m_pMeshData;
	float				 m_SwitchPoint;
	int					 numMaterials;
	IMaterial** ppMaterials;
	int* pMaterialFlags;
	int* m_pHWMorphDecalBoneRemap;
	int					 m_nDecalBoneCount;
};

struct studiohwdata_t {
	int					 m_RootLOD;
	int					 m_NumLODs;
	studioloddata_t* m_pLODs;
	int					 m_NumStudioMeshes;
};

struct DrawModelInfo_t {
	studiohdr_t* m_pStudioHdr;
	PAD( 0x8 );
	int				 m_Skin;
	int				 m_Body;
	int				 m_HitboxSet;
	void* m_pClientEntity;
	int				 m_Lod;
	void* m_pColorMeshes;
	bool			 m_bStaticLighting;
	vec3_t			 m_vecAmbientCube [ 6 ];
	int				 m_nLocalLightCount;
	LightDesc_t		 m_LocalLightDescs [ 4 ];
};

struct DrawModelState_t {
	studiohdr_t* m_pStudioHdr;
	studiohwdata_t* m_pStudioHWData;
	void* m_pRenderable;
	const matrix3x4_t* m_pModelToWorld;
	PAD( 0x4 );
	int					 m_drawFlags;
	int					 m_lod;
};

struct ModelRenderInfo_t {
	vec3_t				m_origin;			// 0
	vec3_t				m_angles;			// 12
	PAD( 0x4 );//chuj
	void* m_renderable;		// 24
	const model_t* m_model;			// 28
	const matrix3x4_t* m_model_to_world;	// 32
	const matrix3x4_t* m_lighting_offset;	// 36
	const vec3_t* m_lighting_origin;	// 40
	int					m_flags;			// 44
	int					m_index;			// 48
	int					m_skin;				// 52
	int					m_body;				// 56
	int					m_hitboxset;		// 60
	uint16_t			m_instance;			// 64

	ModelRenderInfo_t( ) : m_model_to_world {} , m_lighting_offset {} , m_lighting_origin {} {}
};

class IClientShadowMgr {
public:
};

class CMaterialVar {
public:
	void SetFloat( const float val ) {
		return util::get_virtual_function< void( __thiscall* )( void* , float ) >( this , 4 )( this , val );
	}

	void SetInt( const int val ) {
		return util::get_virtual_function< void( __thiscall* )( void* , int ) >( this , 5 )( this , val );
	}

	void SetChar( const char* val ) {
		return util::get_virtual_function< void( __thiscall* )( void* , const char* ) >( this , 6 )( this , val );
	}

	void SetVector( const float x , const float y , const float z ) {
		return util::get_virtual_function< void( __thiscall* )( void* , float , float , float ) >( this , 11 )( this , x , y , z );
	}
};

class IMaterial {
public:
	const char* GetName( ) {
		return util::get_virtual_function< const char* ( __thiscall* )( void* ) >( this , 0 )( this );
	}

	const char* GetTextureGroupName( ) {
		return util::get_virtual_function< const char* ( __thiscall* )( void* ) >( this , 1 )( this );
	}

	CMaterialVar* FindMaterialVar( const char* var , bool* found , bool complain = true ) {
		return util::get_virtual_function< CMaterialVar* ( __thiscall* )( void* , const char* , bool* , bool ) >( this , 11 )( this , var , found , complain );
	}

	void IncrementReferenceCount( ) {
		return util::get_virtual_function< void( __thiscall* )( void* ) >( this , 12 )( this );
	}

	void AlphaModulate( float alpha ) {
		return util::get_virtual_function< void( __thiscall* )( void* , float ) >( this , 27 )( this , alpha );
	}

	void ColorModulate( float r , float g , float b ) {
		return util::get_virtual_function< void( __thiscall* )( void* , float , float , float ) >( this , 28 )( this , r , g , b );
	}

	void ColorModulate( color col ) {
		ColorModulate( float( col.r ) / 255.f , float( col.g ) / 255.f , float( col.b ) / 255.f );
	}

	void SetFlag( int fl , bool set ) {
		return util::get_virtual_function< void( __thiscall* )( void* , int , bool ) >( this , 29 )( this , fl , set );
	}

	bool GetFlag( int fl ) {
		return util::get_virtual_function< bool( __thiscall* )( void* , int ) >( this , 30 )( this , fl );
	}

	bool IsErrorMaterial( ) {
		return util::get_virtual_function< bool( __thiscall* )( void* ) >( this , 42 )( this );
	}
};

class IMaterialSystem {
public:
	// IMaterial* CreateMaterial( const char* name, KeyValues* kv ) {
	//	return util::get_virtual_function< IMaterial*( __thiscall* )( void*, const char*, KeyValues* ) >( this, 83 )( this, name, kv );
	//}

	IMaterial* FindMaterial( const char* name , char* type ) {
		return  util::get_virtual_function< IMaterial* ( __thiscall* )( void* , const char* , char* , bool , void* ) >( this , 84 )( this , name , type , 1 , 0 );
	}

	uint16_t FirstMaterial( ) {
		return util::get_virtual_function< uint16_t( __thiscall* )( void* ) >( this , 86 )( this );
	}

	uint16_t NextMaterial( uint16_t handle ) {
		return util::get_virtual_function< uint16_t( __thiscall* )( void* , uint16_t ) >( this , 87 )( this , handle );
	}

	uint16_t InvalidMaterial( ) {
		return util::get_virtual_function< uint16_t( __thiscall* )( void* ) >( this , 88 )( this );
	}

	IMaterial* GetMaterial( uint16_t handle ) {
		return util::get_virtual_function< IMaterial* ( __thiscall* )( void* , uint16_t ) >( this , 89 )( this , handle );
	}

};

class CStudioRenderContext {
public:
	PAD( 0x250 );
	IMaterial* m_pForcedMaterial;

	void ForcedMaterialOverride( IMaterial* mat ) {
		return util::get_virtual_function< void( __thiscall* )( void* , IMaterial* , int , int ) >( this , 33 )( this , mat , 0 , 0 );
	}
};

class IVModelRender {
public:
	// dont use this, just a wrapper.
	// use the one from studiorender.
	// void ForcedMaterialOverride( IMaterial* mat ) {
	//	return util::get_virtual_function< void( __thiscall* )( void *, IMaterial *, int, int ) >( this, 1 )( this, mat, 0, 0 );
	//}

	bool IsForcedMaterialOverride( ) {
		return util::get_virtual_function< bool( __thiscall* )( void* ) >( this , 2 )( this );
	}
};

class IVRenderView {
public:
	void SetBlend( float blend ) {
		return util::get_virtual_function< void( __thiscall* )( void* , float )>( this , 4 )( this , blend );
	}

	void SetColorModulation( float* col ) {
		return util::get_virtual_function< void( __thiscall* )( void* , float* ) >( this , 6 )( this , col );
	}

	void SetColorModulation( color col ) {
		float fcol [ ] = { float( col.r ) / 255.f , float( col.g ) / 255.f , float( col.b ) / 255.f ,  1.f };
		return util::get_virtual_function< void( __thiscall* )( void* , float* ) >( this , 6 )( this , fcol );
	}

	color GetColorModulation( ) {
		float col [ 4 ];
		util::get_virtual_function< void( __thiscall* )( void* , float* ) >( this , 7 )( this , col );

		return color(
			( int ) ( col [ 0 ] * 255 ) ,
			( int ) ( col [ 1 ] * 255 ) ,
			( int ) ( col [ 2 ] * 255 ) ,
			( int ) ( col [ 3 ] * 255 )
		);
	}
};

class CViewRender {
public:
	PAD( 0x4 );
	CViewSetup m_view;

	const CViewSetup* GetPlayerViewSetup( int slot = -1 ) {
		return util::get_virtual_function< const CViewSetup* ( __thiscall* )( decltype( this ) , int ) >( this , 12 )( this , slot );
	}
};

struct MaterialVideoMode_t {
	int m_Width;
	int m_Height;
	int m_Format;
	int m_RefreshRate;
};

struct MaterialSystem_Config_t {
	MaterialVideoMode_t m_VideoMode;
	float               m_fMonitorGamma;
	float               m_fGammaTVRangeMin;
	float               m_fGammaTVRangeMax;
	float               m_fGammaTVExponent;
	bool                m_bGammaTVEnabled;
	bool                m_bTripleBuffered;
	int                 m_nAASamples;
	int                 m_nForceAnisotropicLevel;
	int                 m_nSkipMipLevels;
	int                 m_nDxSupportLevel;
	int                 m_nFlags;
	bool                m_bEditMode;
	char                m_nProxiesTestMode;
	bool                m_bCompressedTextures;
	bool                m_bFilterLightmaps;
	bool                m_bFilterTextures;
	bool                m_bReverseDepth;
	bool                m_bBufferPrimitives;
	bool                m_bDrawFlat;
	bool                m_bMeasureFillRate;
	bool                m_bVisualizeFillRate;
	bool                m_bNoTransparency;
	bool                m_bSoftwareLighting;
	bool                m_bAllowCheats;
	char                m_nShowMipLevels;
	bool                m_bShowLowResImage;
	bool                m_bShowNormalMap;
	bool                m_bMipMapTextures;
	char                m_nFullbright;
	bool                m_bFastNoBump;
	bool                m_bSuppressRendering;
	bool                m_bDrawGray;
	bool                m_bShowSpecular;
	bool                m_bShowDiffuse;
	int                 m_nWindowedSizeLimitWidth;
	int                 m_nWindowedSizeLimitHeight;
	int                 m_nAAQuality;
	bool                m_bShadowDepthTexture;
	bool                m_bMotionBlur;
	bool                m_bSupportFlashlight;
	bool                m_bPaintEnabled;
	PAD( 0xC );
};
