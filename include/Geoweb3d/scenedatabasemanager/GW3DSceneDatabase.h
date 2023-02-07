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
#include "GW3DSceneDatabaseExports.h"
#include "GW3DGuids.h"
#include "GW3DInterFace.h"
#include <Geoweb3dCore/Geoweb3dTypes.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Destroys the class instance described by p. </summary>
///
/// <param name="p">	[in,out] If non-null, the Geoweb3d::IGW3DIBaseObject * to process. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" GW3D_SCENE_DATABASE_API void DestroySceneDatabaseClassInstance( Geoweb3d::IGW3DIBaseObject * p );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates class instance. </summary>
///
/// <param name="class_type">	Type of the class. </param>
/// <param name="p">		 	[in,out] If non-null, the Geoweb3d::IGW3DCallback * to process. </param>
///
/// <returns>	null if it fails, else the new class instance. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" GW3D_SCENE_DATABASE_API Geoweb3d::IGW3DIBaseObject * CreateSceneDatabaseClassInstance( const Geoweb3d::GW3DGUID & class_type, Geoweb3d::IGW3DCallback * p );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Queries if class interface is supported class interface. </summary>
///
/// <param name="class_type">	Type of the class. </param>
///
/// <returns>	true if class interface supported, false if not. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" GW3D_SCENE_DATABASE_API bool IsSceneDatabaseClassInterfaceSupported( const Geoweb3d::GW3DGUID & class_type );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Deleter for IGW3DIBaseObject. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DSceneDatabaseInterfaceDeleter
{
    void operator()( Geoweb3d::IGW3DIBaseObject* p )
    {
        DestroySceneDatabaseClassInstance( p );
    }
};

template<class T, class C>
static T CreateSceneDatabaseInterfacePointer( const Geoweb3d::GW3DGUID& guid, Geoweb3d::IGW3DCallback* p = 0 )
{
    if ( IsSceneDatabaseClassInterfaceSupported( guid ) )
    {
        return T( dynamic_cast< C* >( CreateSceneDatabaseClassInstance( guid, p ) ), IGW3DSceneDatabaseInterfaceDeleter() );
    }
    else
    {
        //print error
    }

    return T();
}

