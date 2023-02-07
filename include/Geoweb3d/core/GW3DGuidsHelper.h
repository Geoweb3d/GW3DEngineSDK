//////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// Geoweb3d SDK license under which the SDK provided.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Geoweb3dCore/Geoweb3dTypes.h>

#ifdef __cplusplus

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines gw3d extern c. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define GW3DEXTERN_C    extern "C"
#else
#define GW3DEXTERN_C    extern
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines gw3d define unique identifier. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="name">	The name. </param>
/// <param name="l">   	The void to process. </param>
/// <param name="w1">  	The first w value. </param>
/// <param name="w2">  	The second w value. </param>
/// <param name="b1">  	The first void. </param>
/// <param name="b2">  	The second void. </param>
/// <param name="b3">  	The third void. </param>
/// <param name="b4">  	The fourth void. </param>
/// <param name="b5">  	The fifth void. </param>
/// <param name="b6">  	The b 6. </param>
/// <param name="b7">  	The b 7. </param>
/// <param name="b8">  	The b 8. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#define GW3D_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        GW3DEXTERN_C const Geoweb3d::GW3DGUID DECLSPEC_SELECTANY name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#else
//Android was reporting multiple definitions with the above msc_ver technique
#define GW3D_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        const Geoweb3d::_GW3D_GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }; \
        GW3DEXTERN_C  const Geoweb3d::_GW3D_GUID __attribute__((visibility("default"))) name;
#endif

//#define GW3D_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
//GW3DEXTERN_C const GW3DGUID name

__inline bool IsEqualGW3DGUID( const Geoweb3d::GW3DGUID &rguid1, const Geoweb3d::GW3DGUID &rguid2 )
{
	return memcmp( &rguid1, &rguid2, sizeof(Geoweb3d::GW3DGUID ) ) == 0;
}

__inline bool IsLessThanGW3DGUID( const Geoweb3d::GW3DGUID &rguid1, const Geoweb3d::GW3DGUID &rguid2 )
{
	return memcmp( &rguid1, &rguid2, sizeof(Geoweb3d::GW3DGUID ) ) < 0;
}

