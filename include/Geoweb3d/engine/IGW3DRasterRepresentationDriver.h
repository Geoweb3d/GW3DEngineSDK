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
#include "../core/IGW3DDriver.h"

namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Raster representation driver. </summary>
    ///
    /// <remarks>	A raster representation driver defines a way of representing raster data in the
    /// 			3D scene, as well as the default properties for the representation. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DRasterRepresentationDriver : public IGW3DDriver
    {
        virtual ~IGW3DRasterRepresentationDriver() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the name of the driver. </summary>
        ///
        /// <returns>	the name. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_Name() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query whether the driver is capable of representing the given layer. </summary>
        ///
        /// <param name="ds">	The raster layer. </param>
        ///
        /// <returns>	GW3D_sOk if the driver can represent the given raster layer. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult get_CapabilityToRepresent(IGW3DRasterLayerWPtr ds) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the default properties that the sdk uses for new representations. </summary>
        ///
        /// <remarks>	When a new representation is created, if not given any other properties, it uses
        /// 			these default properties in the driver. </remarks>
        ///
        /// <returns>	the property collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

    	virtual const IGW3DPropertyCollection* get_PropertyCollection( ) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the representation collection. </summary>
        ///
        /// <remarks>	The collection of all representations for this driver. New representations are also
        /// 			created using the collection. </remarks>
        ///
        /// <returns>	the representation collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterRepresentationCollection* get_RepresentationLayerCollection(bool reset_next_iterator = false) = 0;
    };
}