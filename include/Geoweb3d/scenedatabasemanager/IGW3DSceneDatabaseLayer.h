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
#include "GW3DInterFace.h"

/* Primary namespace */
namespace Geoweb3d
{
    class IGW3DSceneDatabaseLayer;
    _GW3D_SMARTPTR_TYPEDEF      ( IGW3DSceneDatabaseLayer );
    _GW3D_WEAK_SMARTPTR_TYPEDEF ( IGW3DSceneDatabaseLayer );

    class IGW3DSceneDatabaseLayerCollection;
    _GW3D_SMARTPTR_TYPEDEF      ( IGW3DSceneDatabaseLayerCollection );
    _GW3D_WEAK_SMARTPTR_TYPEDEF ( IGW3DSceneDatabaseLayerCollection );

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Database Layer which encapsulates some path 
    ///           information to an engine loadable layer.    </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct IGW3DSceneDatabaseLayer
    {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will return filename of the layer. </summary>
        ///
        /// <returns> The database layer's file name.         </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_LayerFileName() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will return file path of the layer. </summary>
        ///
        /// <returns> The database layer's file path.          </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_LayerFilePath() const = 0;
    };
}