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

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of 3D visibility dome tests. </summary>
    ///
    /// <remarks>	This collection contains all tests, and is also where new visibility domes can
    /// 			be created. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DVisibilityDomeTestCollection : public IGW3DCollection< IGW3DVisibilityDomeTestWPtr >
    {
        virtual ~IGW3DVisibilityDomeTestCollection() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a Visibility Dome. </summary>
        ///
        /// <remarks>	Creates a new visibility dome.  Note that the visibility dome will not be visible
        /// 			until it is positioned and one or more cameras have been added to it. </remarks>
        ///
        /// <returns>	A  test. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVisibilityDomeTestWPtr create() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enables/disables all the tests. </summary>
        ///
        /// <remarks>	Globally enables or disables all the viewshed tests. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_EnableAll(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Closes the given Visibility Dome. </summary>
        ///
        /// <remarks>	Closes the given visibility dome and removes it from the collection.
        ///
        /// 			To close a visibility dome: IGW3DVisibilityDomeTestCollection::close( vtest );
        ///
        ///				IGW3DVisibilityDomeTestWPtr will become expired.</remarks>
        ///
        /// <param name="vtest"> the viewshed test</param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static void close(IGW3DVisibilityDomeTestWPtr& vtest)
        {
            close_VisibilityDomeTest(vtest);
        }
    };
}