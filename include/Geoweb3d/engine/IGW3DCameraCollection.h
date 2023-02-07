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
#include "../core/GW3DInterFace.h"
#include "../core/IGW3DCollection.h"
#include "GW3DExports.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of cameras. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DCameraCollection : public IGW3DCollection< IGW3DCameraWPtr >
    {
#ifndef SWIGGING

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Retrieve the item at the given index. </summary>
        ///
        /// <param name="index">	Zero-based index of the item. </param>
        ///
        /// <returns>	The item. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DCameraWPtr operator[](unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Retrieve the item at the given index. </summary>
        ///
        /// <param name="index">	Zero-based index of the item. </param>
        ///
        /// <returns>	The item. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DCameraWPtr get_AtIndex(unsigned long index) const = 0;
#endif

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a camera. </summary>
        ///
        /// <param name="camera_name">	Name of the camera. </param>
        ///
        /// <returns>	The new camera. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DCameraWPtr create(const char* camera_name) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Closes the given camera. </summary>
        ///
        /// <remarks>	Closes the given camera and removes it from the collection.
        ///
        /// 			To close a camera: IGW3DCameraCollection::close( window );
        ///
        ///				IGW3DCameraWPtr will become expired. </remarks>
        ///
        /// <param name="camera">	[in] The camera. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static void close(IGW3DCameraWPtr& camera)
        {
            close_Camera(camera);
        }
    };
}