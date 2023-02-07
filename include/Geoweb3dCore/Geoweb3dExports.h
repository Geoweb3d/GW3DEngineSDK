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

#ifndef GW3D_DLL
	#if defined(_MSC_VER) && !defined(GW3D_DISABLE_DLL)
	#  ifdef GW3D_COMPILATION
	#    define GW3D_DLL __declspec(dllexport)
	#  else
	#    define GW3D_DLL
	#  endif
	#  define GW3D_INTERNAL
	#else
	#  if defined(USE_GCC_VISIBILITY_FLAG)
	#    define GW3D_DLL     __attribute__ ((visibility("default")))
	#    if !defined(__MINGW32__)
	#        define GW3D_INTERNAL __attribute__((visibility("hidden")))
	#    else
	#        define GW3D_INTERNAL
	#    endif
	#  else
	#    define GW3D_DLL
	#    define GW3D_INTERNAL
	#  endif
#endif

#endif

//#if defined(_WIN64) || defined(_WIN32)
//	#if GEOWEB3DENGINE_EXPORTS
//	#	define GW3D_DLL __declspec(dllexport)
//	#else
//	#	define GW3D_DLL __declspec(dllimport)
//	#endif
//#elif defined(__ANDROID__) || defined(__linux__)
//	#if GEOWEB3DENGINE_EXPORTS
//	#	define GW3D_DLL __attribute__((visibility("default")))
//	#else 
//	#	define GW3D_DLL
//	#endif
//#else
//	#define GW3D_DLL
//	#pragma warning Unknown dynamic link import/export semantics.
//#endif
