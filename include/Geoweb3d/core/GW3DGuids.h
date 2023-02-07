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
#include <Geoweb3d/core/GW3DGuidsHelper.h>

#ifdef _MSC_VER
#include <windows.h>
#endif

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

//{3833EEBF-289C-41C0-BBB9-DBF985145605}
GW3D_DEFINE_GUID(GUID_GEOWEB3D_SDK,   0x3833eebf, 0x289c, 0x41c0, 0xbb, 0xb9, 0xdb, 0xf9, 0x85, 0x14, 0x56, 0x5);

//{BD825961-0BD2-4814-9522-E455CF42D153}
GW3D_DEFINE_GUID(GUID_GEOWEB3D_FUSIONSERVER_SDK, 0xbd825961, 0xbd2, 0x4814, 0x95, 0x22, 0xe4, 0x55, 0xcf, 0x42, 0xd1, 0x53);

// {9505E122-7A48-4123-8CDB-8E4426EA4A25}
GW3D_DEFINE_GUID( GUID_GEOWEB3DDESKTOP_SDK, 0x9505e122, 0x7a48, 0x4123, 0x8c, 0xdb, 0x8e, 0x44, 0x26, 0xea, 0x4a, 0x25);

// {2AB14817-FCB2-44ae-BB12-D510FDABDFD3}
GW3D_DEFINE_GUID( GUID_GEOWEB3DDESKTOP_REBRANDING_TOKEN, 0x2ab14817, 0xfcb2, 0x44ae, 0xbb, 0x12, 0xd5, 0x10, 0xfd, 0xab, 0xdf, 0xd3);

// {52F58F47-1FA0-4748-AB71-5FD77C570830}
GW3D_DEFINE_GUID(GUID_DYNAMIC_VECTOR, 0x52f58f47, 0x1fa0, 0x4748, 0xab, 0x71, 0x5f, 0xd7, 0x7c, 0x57, 0x8, 0x30);

GW3D_DEFINE_GUID(GUID_GEOWEB3D_SDK_INTEGRATED_BRIDGE_ARBITER, 0x53f58f47, 0x1fa0, 0x4728, 0xac, 0x72, 0x5f, 0xd3, 0x7c, 0x57, 0x8, 0x30);

GW3D_DEFINE_GUID(GUID_GEOWEB3D_SDK_INTEGRATED_BRIDGE_CLIENT, 0x52f58f47, 0x1fa0, 0x4728, 0xac, 0x72, 0x5f, 0xd3, 0x7c, 0x57, 0x8, 0x30);
