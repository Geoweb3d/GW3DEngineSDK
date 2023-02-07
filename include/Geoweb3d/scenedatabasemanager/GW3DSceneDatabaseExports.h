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

#ifdef _ATL_ATTRIBUTES
#	define GW3D_SCENE_DATABASE_API  
#elif GW3D_SCENE_DATABASE_EXPORTS
#	define GW3D_SCENE_DATABASE_API __declspec(dllexport)
#else
#	define GW3D_SCENE_DATABASE_API __declspec(dllimport)
#endif
