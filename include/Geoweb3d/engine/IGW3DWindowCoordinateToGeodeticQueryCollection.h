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
    /// <summary>	Collection of geodetic queries. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DWindowCoordinateToGeodeticQueryCollection : public IGW3DCollection< IGW3DWindowCoordinateToGeodeticQueryWPtr >
    {
        virtual ~IGW3DWindowCoordinateToGeodeticQueryCollection() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a new query. </summary>
        ///
        /// <remarks>	Creates a new query, which reports the geodetic position at a given window
        /// 			coordinate. </remarks>
        ///
        /// <returns>	the new query. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DWindowCoordinateToGeodeticQueryWPtr create() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enables/disables all the queries. </summary>
        ///
        /// <remarks>	Globally enables or disables all the queries. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_EnableAll(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Closes the given query. </summary>
        ///
        /// <remarks>	Closes the query and removes it from the collection.
        ///
        /// 			To close a query: IGW3DWindowCoordinateToGeodeticQueryCollection::close( window );
        ///
        ///				IGW3DWindowCoordinateToGeodeticQueryWPtr will become expired. </remarks>
        ///
        /// <param name="query">	The query. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static void close(IGW3DWindowCoordinateToGeodeticQueryWPtr& query)
        {
            close_Query(query);
        }
    };
}