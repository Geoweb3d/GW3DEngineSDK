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
    /// <summary>	Collection of all line segment intersection tests. </summary>
    ///
    /// <remarks>	Collection of all the line segment intersection tests that exist in the
    /// 			application. This is also where new intersection tests are created. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DLineSegmentIntersectionTestCollection : public IGW3DCollection< IGW3DLineSegmentIntersectionTestWPtr >
    {
        virtual  ~IGW3DLineSegmentIntersectionTestCollection() {};

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a line segment intersection test </summary>
        ///
        /// <remarks>	Creates a new line segment intersection test.  Note, currently the SDK only supports
        /// 			up to 10 segments, though this may change in the future. </remarks>
        ///
        /// <returns>	A line segment intersection test. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DLineSegmentIntersectionTestWPtr create() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enables/disables all the tests. </summary>
        ///
        /// <remarks>	Globally enables or disables all the line segment intersection tests. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_EnableAll(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets intersection results for the most recent frame. </summary>
        ///
        /// <remarks>	This collection contains just the intersection tests that produced results the
        /// 			last time the scene was drawn. </remarks>
        ///
        /// <param name="reset_next_iterator"> TODO </param>
        ///
        /// <returns>	Collection of the intersection results. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DLineSegmentIntersectionTestResultsCollection* get_IntersectionResultsCollection(bool reset_next_iterator = false) = 0;

        //SKIP-CODE-BEGIN

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets if a rep driver supports the line segment test. </summary>
        ///
        /// <remarks>	not all representation currently support a line segment test.  Others
        ///				just don't make sense.  This provided an easy way to know. </remarks>
        ///
        /// <param name="rep"> TODO </param>
        ///
        /// <returns>	true if its supported. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //virtual bool get_CapabilityToAnalize( IGW3DVectorRepresentationDriver *rep ) = 0;

        //SKIP-CODE-END

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Closes the given line segment intersection test. </summary>
        ///
        /// <remarks>	Closes the given line segment intersection test and removes it from the
        /// 			collection.
        ///
        /// 			To close an intersection test:
        /// 			IGW3DLineSegmentIntersectionTestCollection::close( window );
        ///
        ///				IGW3DLineSegmentIntersectionTestWPtr will become expired. </remarks>
        ///
        /// <param name="ltest">	The test. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static void close(IGW3DLineSegmentIntersectionTestWPtr& ltest)
        {
            close_LineSegmentIntersectionTest(ltest);
        }
    };
}