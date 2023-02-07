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
#include <Geoweb3dCore/GW3DResults.h>
#include "GW3DExports.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of Raster data sources for a driver. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DRasterDataSourceCollection : public IGW3DCollection< IGW3DRasterDataSourceWPtr >
    {
        virtual ~IGW3DRasterDataSourceCollection() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Array indexer operator. </summary>
        ///
        /// <param name="index">	Zero-based index of the raster datasource. </param>
        ///
        /// <returns>	The indexed value. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DRasterDataSourceWPtr operator[](unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets at index. </summary>
        ///
        /// <param name="index">	Zero-based index of the raster datasource. </param>
        ///
        /// <returns>	at index. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DRasterDataSourceWPtr get_AtIndex(unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Deletes the datasource from disk (if capable) </summary>
        ///
        /// <param name="pszName">	The name to destroy. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult destroy(const char* pszName) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Opens a raster datasource using this driver. </summary>
        ///
        /// <remarks>	pszName can be any string supported by GDAL, but is typically a filepath or url. </remarks>
        ///
        /// <param name="pszName">   	The name (i.e. filepath or url). </param>
        /// <param name="result">		Returns whether the returned datasource was already opened.  </param>
        /// <param name="properties">	(optional) the properties. </param>
        ///
        /// <returns>	A raster datasource. </returns>
        ///
        /// <see cref="IGW3DRasterDriverCollection::auto_Open"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterDataSourceWPtr open(const char* pszName, GW3DResult& result, const IGW3DPropertyCollectionPtr properties = IGW3DPropertyCollectionPtr()) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="pszName"> TODO </param>
        /// <param name="properties"> TODO </param>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterDataSourceWPtr create(const char* pszName, const IGW3DPropertyCollectionPtr properties = IGW3DPropertyCollectionPtr()) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="pszName"> TODO </param>
        /// <param name="properties"> TODO </param>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterDataSourceWPtr create(const char* pszName, const IGW3DPropertyCollection* properties) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a new proxy layer. </summary>
        ///
        /// <remarks>	A proxy layer allows data to be queried out of a service or external datasource on
        ///				demand in multiple threads, via the IGW3DProxyRasterCallbacks. </remarks>
        /// 
        /// <param name="name">			   	The name. </param>
        /// <param name="properties">	   	(optional) raster creation properties. </param>
        ///
        /// <returns>	The new proxy layer. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterDataSourceWPtr create_Proxy(const char* name, const IGW3DPropertyCollectionPtr properties = IGW3DPropertyCollectionPtr(), IGW3DRasterPipelineCallbacks* cb = nullptr) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets datasource by name. </summary>
        ///
        /// <remarks>	Gets a datasource that was already opened with the same name. </remarks>
        ///
        /// <param name="name">	The name. </param>
        ///
        /// <returns>	The raster datasource. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterDataSourceWPtr get_ByName(const char* name) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the driver. </summary>
        ///
        /// <remarks>	Gets the raster driver that this collection belongs to. </remarks>
        ///
        /// <returns>	The driver. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterDriverWPtr get_Driver() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Closes the given datasource. </summary>
        ///
        /// <remarks>	Closes the given raster datasource and removes it from the collection.
        ///
        /// 			To close a datasource: IGW3DRasterDataSourceCollection::close( window );
        ///
        ///				IGW3DRasterDataSourceWPtr will become expired. </remarks>
        ///
        /// <param name="ds">	[in] The datasource. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static void close(IGW3DRasterDataSourceWPtr& ds)
        {
            close_RasterDataSource(ds);
        }
    };
}