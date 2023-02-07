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

namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of the raster representation drivers. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DRasterRepresentationDriverCollection : public IGW3DCollection< IGW3DRasterRepresentationDriverWPtr >
    {
        virtual ~IGW3DRasterRepresentationDriverCollection() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Array indexer operator. </summary>
        ///
        /// <param name="index">	Zero-based index of the raster representation driver. </param>
        ///
        /// <returns>	The indexed value. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DRasterRepresentationDriverWPtr operator[](unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets at index. </summary>
        ///
        /// <param name="index">	Zero-based index of the raster representation driver. </param>
        ///
        /// <returns>	at index. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DRasterRepresentationDriverWPtr get_AtIndex(unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets a driver by name. </summary>
        ///
        /// <param name="name">	The name. </param>
        ///
        /// <returns>	The driver. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterRepresentationDriverWPtr get_Driver(const char* name) = 0;

        //SKIP-CODE-BEGIN

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Streams the given streamobj. </summary>
            ///
            /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
            ///
            /// <param name="streamobj">	[in,out] If non-null, the streamobj. </param>
            ////////////////////////////////////////////////////////////////////////////////////////////////////

            //virtual void Stream( IGW3DFrustumAnalysisStream *streamobj ) = 0;

        //SKIP-CODE-END
    };
}