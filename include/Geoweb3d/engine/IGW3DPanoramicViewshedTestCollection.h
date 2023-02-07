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
    /// <summary>	Collection of 3D viewshed tests. </summary>
    ///
    /// <remarks>	This collection contains all 3D viewshed tests, and is also where new viewsheds can
    /// 			be created. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DPanoramicViewshedTestCollection : public IGW3DCollection< IGW3DPanoramicViewshedTestWPtr >
    {
        virtual ~IGW3DPanoramicViewshedTestCollection() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a viewshed. </summary>
        ///
        /// <remarks>	Creates a new viewshed.  Note that the viewshed will not be visible until it is
        /// 			positioned and one or more cameras have been added to it. </remarks>
        ///
        /// <returns>	A viewshed test. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DPanoramicViewshedTestWPtr create() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enables/disables all the tests. </summary>
        ///
        /// <remarks>	Globally enables or disables all the viewshed tests. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_EnableAll(bool enable) = 0;

    //SKIP-CODE-BEGIN

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets results. </summary>
        ///
        /// <remarks>	Geoweb3d,  04/16/2013. </remarks>
        ///
        /// <returns>	null if it fails, else the  results. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //virtual IGW3DViewshedTestResultsCollection * get_ResultsCollection() = 0;

    //SKIP-CODE-END

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Closes the given viewshed. </summary>
        ///
        /// <remarks>	Closes the given viewshed and removes it from the collection.
        ///
        /// 			To close a viewshed: IGW3DPanoramicViewshedTestCollection::close( vtest );
        ///
        ///				IGW3DPanoramicViewshedTestWPtr will become expired. </remarks>
        ///
        /// <param name="vtest"> the viewshed test </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static void close(IGW3DPanoramicViewshedTestWPtr& vtest)
        {
            close_PanoramicViewshedTest(vtest);
        }
    };
}