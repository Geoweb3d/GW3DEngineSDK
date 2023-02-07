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
#include "core/GW3DGUIInterFace.h"

/* Primary namespace */
namespace Geoweb3d
{
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of intersection reports from an intersection test. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUILineSegmentIntersectionReportCollection : public IGW3DCollection< IGW3DGUILineSegmentIntersectionReport* >
	{
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	An intersection report for a single representation. </summary>
	///
	/// <remarks>	The intersection report contains details for one or more intersection points with 
	/// 			features in the given representation. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUILineSegmentIntersectionReport
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Values that represent the class of representation in this intersection. </summary>
		///
		/// <remarks>	Use this enum to determine which function to use to retrieve the representation
		/// 			that was intersection. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum IntersectionRepresentationClass
		{
			/// <summary>	A Vector Representation. </summary>
			VectorRepresentation = 0,
			/// <summary>	A Basic Vector Representation. </summary>
			VectorRepresentationBasic,
			/// <summary>	Do Not Use. </summary>
			IRS_MAX = 0xff
		};

		virtual IntersectionRepresentationClass get_RepresentationClass() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets vector layer representation. </summary>
		///
		/// <remarks>	Gets the vector layer representation described by this intersection. </remarks>
		///
		/// <returns>	The vector layer representation if the intersected representation class is 
		/// 			VectorRepresentation, else an expired pointer. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorRepresentationWPtr get_VectorRepresentation() = 0;

	
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets vector layer basic representation. </summary>
		///
		/// <remarks>	Gets the basic vector layer representation described by this intersection. </remarks>
		///
		/// <returns>	The vector basic layer representation if the intersected representation class is 
		/// 			VectorRepresentationBasic, else an expired pointer. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorRepresentationBasicWPtr get_VectorRepresentationBasic() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets intersection detail collection. </summary>
		///
		/// <remarks>	Provides details on each feature intersection in the given representation. </remarks>
		///
		/// <returns>	the collection of intersection details. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DLineSegmentIntersectionDetailCollection *get_IntersectionDetailCollection() = 0;
	};


}
}