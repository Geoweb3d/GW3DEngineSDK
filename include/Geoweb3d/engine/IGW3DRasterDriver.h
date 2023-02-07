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

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Raster driver. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DRasterDriver : public IGW3DDriver
    {
        virtual ~IGW3DRasterDriver() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the default properties </summary>
        ///
        /// <remarks>	Gets the default properties for opening a datasource with this driver.  Check for
        /// 			null, as some drivers may not have any properties. </remarks>
        ///
        /// <returns>	The properties. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DPropertyCollectionPtr  get_PropertyCollection() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the raster datasource collection. </summary>
        ///
        /// <remarks>	Gets the collection of all raster datasources opened using this driver.  Raster
        /// 			datasources are also opened in this collection. </remarks>
        ///
        /// <returns>	the raster data source collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterDataSourceCollection* get_RasterDataSourceCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the name. </summary>
        ///
        /// <remarks>	Gets the name of the driver. </remarks>
        ///
        /// <returns>	the name. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_Name() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the file extension. </summary>
        ///
        /// <remarks>	Gets the file extension, such as .ecw or .tif </remarks>
        ///
        /// <returns>	The extension. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DStringPtr get_Extension()   const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the description. </summary>
        ///
        /// <remarks>	Get the description of the file format. </remarks>
        ///
        /// <returns>	The description. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DStringPtr get_Description() const = 0;

        //SKIP-CODE-BEGIN

            //get the help topic (if any) of the file format.
            //virtual const IGW3DStringPtr get_HelpTopic() const = 0;

            //get the mime type  (if any) of the file format, such as "image/png"
            //virtual const IGW3DStringPtr get_MimeType() const = 0;

        //SKIP-CODE-END
    };
}