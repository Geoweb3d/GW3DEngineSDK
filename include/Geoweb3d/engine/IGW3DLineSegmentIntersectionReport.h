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
#include "IGW3DLineSegmentIntersectionReportCollection.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	An intersection report for a single representation. </summary>
    ///
    /// <remarks>	The intersection report contains details for one or more intersection points with
    /// 			features in the given representation. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DLineSegmentIntersectionReport
    {
        virtual  ~IGW3DLineSegmentIntersectionReport() {};

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the vector layer representation. </summary>
        ///
        /// <remarks>	Gets the vector layer representation for this intersection report.  Note: the
        /// 			intersection may represent multiple features in the representation, and even
        /// 			multiple intersection points per feature (for example the front and back face of
        /// 			a model). </remarks>
        ///
        /// <returns>	The vector layer representation. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorRepresentationWPtr get_VectorRepresentation() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets intersection detail collection. </summary>
        ///
        /// <remarks>	Provides details of each intersection point for the associated representation. </remarks>
        ///
        /// <returns>	The intersection detail collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DLineSegmentIntersectionDetailCollection* get_IntersectionDetailCollection(bool reset_next_iterator = false) = 0;
    };
}