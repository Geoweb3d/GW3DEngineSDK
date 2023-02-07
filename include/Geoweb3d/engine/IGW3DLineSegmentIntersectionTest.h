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
#include "IGW3DAnalysis.h"
#include <Geoweb3dCore/GW3DResults.h>

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	A line segment intersection test. </summary>
    ///
    /// <remarks>	A line segment intersection test analyzes a user-defined line segment and
    /// 			reports information on any vector representations that it intersects. This
    /// 			includes the unique IDs of any features/objects the line segment intersects, as
    /// 			well as details on the exact points of intersection.
    ///
    /// 			Some representations do not support the line intersection test, including point
    /// 			cloud, colored line, and custom representations.
    ///
    /// 			The line intersection test will only detect representations that are added to
    /// 			its collection, and it will only detect representations that are enabled (visible).
    ///
    /// 			As long as the line segment intersection test is enabled, its results will be
    /// 			recalculated every frame, and may therefore be evaluated after each time the
    /// 			scene draws. </remarks>
    ///
    /// <see cref="IGW3DLineSegmentIntersectionTestResultsCollection"/>
    /// <see cref="IGW3DLineSegmentIntersectionDetailCollection"/>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  IGW3DLineSegmentIntersectionTest : public IGW3DAnalysis
    {
        virtual  ~IGW3DLineSegmentIntersectionTest() {};

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the enabled state of the line segment intersection test. </summary>
        ///
        /// <remarks>	Sets whether the line segment intersection test is enabled or disabled. When
        /// 			disabled, the analysis will be suspended and the results will not be updated.
        /// 			Therefore it is important to let the scene draw once after enabling the line
        /// 			segment intersection test, otherwise the results will not be current. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_Enabled(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the enabled state of the line segment intersection test. </summary>
        ///
        /// <returns>	true enabled, false if disabled. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_Enabled() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the start and end points of the line segment. </summary>
        ///
        /// <remarks>	Sets both the start and end points of the line segment used in the analysis. The
        /// 			next time and every subsequent time the scene draws, the test results will reflect
        /// 			this line segment.
        ///
        /// 			To use the line segment intersection test for picking (i.e. a mouse query) it is
        /// 			recommended to use the eye position as the start point and the far plane
        /// 			mouse intersection as the end point.  Depending on the use case, any results
        /// 			a further distance than the terrain mouse intersection could be ignored. </remarks>
        ///
        /// <param name="startpt">	The start point. </param>
        /// <param name="endpt">  	The end point. </param>
        ///
        /// <see cref="IGW3DCameraController::get_Ray"/>
        /// <see cref="IGW3DWindowCoordinateToGeodeticQuery"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_StartEnd(const GW3DPoint& startpt, const GW3DPoint& endpt) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the start point of the line segment. </summary>
        ///
        /// <remarks>	Sets the start point of the line segment used in the analysis. The next time and
        /// 			every subsequent time the scene draws, the test results will reflect the line
        /// 			segment defined by the end point and this new start point. </remarks>
        ///
        /// <param name="startpt">	The start point. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_Start(const GW3DPoint& startpt) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the end point of the line segment. </summary>
        ///
        /// <remarks>	Sets the end point of the line segment used in the analysis. The next time and
        /// 			every subsequent time the scene draws, the test results will reflect the line
        /// 			segment defined by the start point and this new end point. </remarks>
        ///
        /// <param name="endpt">	The end point. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_End(const GW3DPoint& endpt) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the start and end points of the line segment. </summary>
        ///
        /// <remarks>	SeGetsts both the start and end points of the line segment used in the analysis. </remarks>
        ///
        /// <param name="startpt">	The start point. </param>
        /// <param name="endpt">  	The end point. </param>
        ///
        /// <see cref="IGW3DCameraController::get_Ray"/>
        /// <see cref="IGW3DWindowCoordinateToGeodeticQuery"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_StartEnd(GW3DPoint& startpt, GW3DPoint& endpt) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the start point of the line segment. </summary>
        ///
        /// <remarks>	Gets the start point of the line segment used in the analysis. </remarks>
        ///
        /// <param name="startpt">	The start point. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_Start(GW3DPoint& startpt) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the end point of the line segment. </summary>
        ///
        /// <remarks>	Gets the end point of the line segment used in the analysis. </remarks>
        ///
        /// <param name="endpt">  	The end point. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_End(GW3DPoint& endpt) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The collection defining all participating vector representations. </summary>
        ///
        /// <remarks>	A collection defining all vector representations that participate in the analysis.
        /// 			All of the member representations will be tested for intersection with the line
        /// 			segment.  If representations are added to or removed from the collection, this
        /// 			change will be reflected in the results after the next draw call.</remarks>
        ///
        /// <param name="reset_next_iterator"> reset the next iterator on the collection. </param>
        ///
        /// <returns>	The vector layer representation collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorRepresentationHelperCollection* get_VectorRepresentationCollection(bool reset_next_iterator = false) = 0;

    //SKIP-CODE-BEGIN

        //if you want to draw results or the ray itself on a camera (or both), you can do such with this
        //api

        //struct  GW3D_DLL IGW3DAnalyisRepresentationDriversCollection()
        //{
        //	//can have just a line/ray (for things like stereo)
        //	//can have what we have today for results (depth fail makes a red dotted)
        //	//can have detail info rep about each intersection point/text dots lines etc.
        //}

        //struct  GW3D_DLL IGW3DAnalyisRepresentationCollection
        //{
        //	IGW3DCamera
        //	IGW3DAnalyisRepresentation
        //}

        //virtual IGW3DAnalyisRepresentationHelperCollection * get_AnalyisRepresentationCollection() = 0;

    //SKIP-CODE-END

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The collection of intersection test analysis results. </summary>
        ///
        /// <remarks>	The collection of the intersections for all representations found to intersect with
        /// 			the line segment.  This collection is updated every time the scene is drawn, and
        /// 			reflects the intersection test's configuration at the time of the draw call.
        /// 			</remarks>
        ///
        /// <param name="reset_next_iterator"> reset the next iterator on the collection. </param>
        ///
        /// <returns>	the intersection collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DLineSegmentIntersectionReportCollection* get_IntersectionReportCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets frame time stamp of the frame the intersection results were last updated. </summary>
        ///
        /// <remarks>	Each frame that the intersection test result are updated, this value is updated to
        /// 			the current frame's time stamp.  Therefore, if the frame time stamp of the
        /// 			intersection test matches the last frame time stamp, the test results are up to
        /// 			date.  If the test is disabled, this will reflect the last frame the test was
        /// 			updated prior to the test being disabled. </remarks>
        ///
        /// <returns>	The frame time stamp. </returns>
        ///
        /// <see cref="Geoweb3d::SDK_Statistics::GetMasterFrameCount"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_FrameTimeStamp() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query validity of the intersection test. </summary>
        ///
        /// <remarks>	If the line segment intersection test results are invalid, this will return false. </remarks>
        ///
        /// <returns>	true if valid, false if invalid. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool	get_IsValid() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Clear results of the query, instead of waiting for the next draw. </summary>
        ///
        /// <remarks>	sometimes it may be required to clear the results instead of waiting for the next 
        ///				frame. </remarks>
        ///
        /// <returns>	void. </returns>
        //////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void	force_ClearResults() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the default properties for this test.
        /// 			</summary>
        ///
        /// <returns>	the properties. </returns>
        ///
        /// <see cref="Geoweb3d::LineSegmentParameters"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DPropertyCollection* get_PropertyCollection() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the  properties for a given test. </summary>
        ///
        /// <param name="p">  	The const IGW3DPropertyCollection* to process. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ///
        /// <see cref="Geoweb3d::LineSegmentParameters"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_PropertyCollection(const IGW3DPropertyCollection* p) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the  properties for a given test. </summary>
        ///
        /// <param name="p">	The const IGW3DPropertyCollectionPtr to process. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ///
        /// <see cref="Geoweb3d::LineSegmentParameters"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_PropertyCollection(const IGW3DPropertyCollectionPtr& p) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the envelope of the intersections tests. </summary>
        ///
        /// <param name="psEnvelope">	the envelope. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_Envelope(GW3DEnvelope* psEnvelope) const = 0;
    };
}