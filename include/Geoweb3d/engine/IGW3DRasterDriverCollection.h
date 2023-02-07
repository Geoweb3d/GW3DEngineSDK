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

namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of raster drivers. </summary>
    /// 
    /// <remarks>	Note: The C++ raster API has more capabilities than the legacy C-style API, however
    ///				in other capabilities it does not yet have equivalency.  See 
    /// 			RasterExports.h and DemExports.h to evaluate which capabilities are important to
    ///				your use case before selecting which raster API to use. </remarks>
    ///
    ///	<see cref="LoadDEM"/>
    ///	<see cref="LoadImagery"/>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DRasterDriverCollection : public IGW3DCollection< IGW3DRasterDriverWPtr >
    {
        virtual ~IGW3DRasterDriverCollection() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Array indexer operator. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DRasterDriverWPtr operator[](unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets at index. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DRasterDriverWPtr get_AtIndex(unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets a driver by name. </summary>
        ///
        /// <remarks>	Gets a raster driver by name. </remarks>
        ///
        /// <param name="name">	The name. </param>
        ///
        /// <returns>	The driver, or an expired pointer if there is no driver by the given name. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterDriverWPtr get_Driver(const char* name) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Auto-open a raster datasource. </summary>
        ///
        /// <remarks>	Automatically determines the correct driver for the data and opens it with that
        /// 			driver.
        ///
        /// 			If you do not want to open a datasource through IGW3DRasterDriver, if you don't
        /// 			know what driver to use to open a datasource, or if you are just not sure if the
        /// 			SDK can open something, you can use this API where the sdk will do its best to
        ///				automatically find an IGW3DRasterDriver and open the datasource for you, returning a
        ///				IGW3DRasterDataSource. If you want to see what driver ended up getting used, that
        ///				can be queried from the IGW3DRasterDataSource.
        ///
        ///				pszName can be any string but is typically a filepath or url.
        /// 
        ///				Note that when opening a WMS service it behooves you to configure any max resolution
        ///				parameters so the 3D engine will know when to stop requesting higher res more data. 
        ///				For example a layer xml file that may have max x size, y size listed.
        ///				Also it is good to be aware of the max number of time slices if this layer will be represented
        ///				as a "imagery-overlay" </remarks>
        ///
        /// <param name="pszName">		The name (i.e. filepath or url). </param>
        /// <param name="result">		Returns whether the returned datasource was already opened.  </param>
        /// <param name="properties">	(Optional) the properties. </param>
        ///
        /// <returns>	The raster datasource. </returns>
        ///
        /// <see cref="IGW3DRasterDataSourceCollection::open"/>
        /// <see cref="IGW3DGeoweb3dSDK::get_GW3DRasterTimeController"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterDataSourceWPtr auto_Open( const char *  pszName, GW3DResult& result, const IGW3DPropertyCollectionPtr properties = IGW3DPropertyCollectionPtr()  ) = 0;

    };
}
