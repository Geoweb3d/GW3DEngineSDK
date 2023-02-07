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

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Intersection detail. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DLineSegmentIntersectionDetail
    {
        virtual  ~IGW3DLineSegmentIntersectionDetail() {};

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the feature/object id. </summary>
        ///
        /// <returns>	The id. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const int64_t get_ObjectID() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets intersection distance from the start point. </summary>
        ///
        /// <returns>	The intersection distance. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual double get_IntersectionDistance() const = 0; //false is descending

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the intersection point. </summary>
        ///
        /// <returns>	null if it fails, else the intersection point. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const GW3DPoint* get_IntersectionPoint() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Get the intersected triangle. </summary>
        ///
        /// <remarks>	The triangle at this point.  This is the real triangle of the intersected geometry,
        /// 			and	is not clipped or modified in any way. </remarks>
        ///
        /// <param name="vert_a">	[in,out] Vertex a of the triangle. </param>
        /// <param name="vert_b">	[in,out] Vertex b of the triangle. </param>
        /// <param name="vert_c">	[in,out] Vertex c of the triangle. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_TriangleIntersected(GW3DPoint& vert_a, GW3DPoint& vert_b, GW3DPoint& vert_c) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The normal at the point of intersection. </summary>
        ///
        /// <remarks>	This gives a point that defines the normal relative to the intersection point. </remarks>
        ///
        /// <returns>	the normal of intersection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const GW3DPoint* get_NormalOfIntersection() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	These reflect the same as on the GW3DLineSegmentIntersection
        ///				as they are here for when developers want to expose results but not
        ///				the configuration of the line segment api itself. </summary>
        ///
        /// <remarks>	This gives a point that defines the normal relative to the intersection point. </remarks>
        ///
        /// <returns>	the normal of intersection. </returns>
        //////////////////////////////////////////////////////////////////////////////////////////////////// 
        virtual bool get_Enabled() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_FrameTimeStamp() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool	get_IsValid() const = 0;
    };
}