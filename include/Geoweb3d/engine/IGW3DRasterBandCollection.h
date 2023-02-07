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
#include "../../Geoweb3dCore/Geoweb3dTypes.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of Raster Bands. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DRasterBandCollection : public IGW3DCollection< IGW3DRasterBandPtr >
    {
        virtual ~IGW3DRasterBandCollection() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Array indexer operator. </summary>
        ///
        /// <param name="index">	Zero-based index of the raster band. </param>
        ///
        /// <returns>	The indexed value. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DRasterBandPtr operator[](unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets at index. </summary>
        ///
        /// <param name="index">	Zero-based index of the raster band. </param>
        ///
        /// <returns>	at index. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DRasterBandPtr get_AtIndex(unsigned long index) const = 0;

    //SKIP-CODE-BEGIN

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the raster data source. </summary>
        ///
        /// <returns>	The raster data source. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //virtual IGW3DRasterDataSourceWPtr get_RasterDataSource() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Typical error and warning that will be returned is GW3D_eBandProjectionError (error) and
        /// GW3D_sProjectionGuessed (warning). </summary>
        ///
        /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
        ///
        /// <returns>	The is valid for Raster Band representation. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //virtual GW3DResult get_IsValidForRasterBandRepresentation() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Collection of representations on this Band. </summary>
        ///
        /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
        ///
        /// <returns>	null if it fails, else the representation Band collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //virtual IGW3DRasterBandRepresentationCollection *get_RepresentationBandCollection( ) = 0;

    //SKIP-CODE-END
    };
}