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

/* @file Analytic Parameters.h */

/* Primary namespace */
namespace Geoweb3d
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Line segment analysis property parameters. </summary>
///
/// <remarks>	Contains the enum describing the definition of the property group used in 
/// 			configuring the rendered line in a line segment intersection test. </remarks>
/// 
/// <see cref="Geoweb3d::IGW3DLineSegmentIntersectionTest::get_PropertyCollection"/>
/// <see cref="Geoweb3d::IGW3DLineSegmentIntersectionTest::put_PropertyCollection"/>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct LineSegmentParameters
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualParameters
	{
		/// <summary>	An enum constant representing the translation offset heading option. </summary>
		TRANSLATION_OFFSET_HEADING			= 0,
		/// <summary>	An enum constant representing the translation offset distance option. </summary>
		TRANSLATION_OFFSET_DISTANCE			= 1,
		/// <summary>	An enum constant representing the translation z coordinate offset option. </summary>
		TRANSLATION_Z_OFFSET				= 2,
		/// <summary>	An enum constant representing the translation z coordinate offset mode option. </summary>
		TRANSLATION_Z_OFFSET_MODE			= 3,
		/// <summary>	An enum constant representing the red option. </summary>
		RED									= 4,
		/// <summary>	An enum constant representing the green option. </summary>
		GREEN								= 5,
		/// <summary>	An enum constant representing the blue option. </summary>
		BLUE								= 6,
		/// <summary>	An enum constant representing the alpha option. </summary>
		ALPHA								= 7,
		/// <summary>	An enum constant representing the line width option. </summary>
		LINE_WIDTH							= 8,

		/// <summary>	An enum constant representing the render option (true to render the line). </summary>
		RENDER								= 9,

		/// <summary>	An enum constant representing the still render when inactive option (if RENDER is true, makes line visible even when analysis is inactive). </summary>
		STILL_RENDER_WHEN_INACTIVE			= 10,

		/// <summary>	An enum constant representing the clamp start to focal option. When using stereo mode, this forces the start of the line to clamp to the 
		/// 			focal plane used, so that no excessive negative parallax occurs when it extends to or past the eye. </summary>
		CLAMP_START_TO_FOCAL				= 11,

		/// <summary>	An enum constant representing the clamp start to focal top scalar option. When CLAMP_START_TO_FOCAL is true, a 1 - -1 scalar where 1 places 
		/// 			the beginning of the rendered line at the top of the focal plane, 0 in the center, and -1 at the bottom.</summary>
		CLAMP_START_TO_FOCAL_TOP_SCALAR		= 12,

		/// <summary>	An enum constant representing the clamp start to focal right scalar option. When CLAMP_START_TO_FOCAL is true, a 1 - -1 scalar where 1 places 
		/// 			the beginning of the rendered line at the right of the focal plane, 0 in the center, and -1 at the left */</summary>
		CLAMP_START_TO_FOCAL_RIGHT_SCALAR	= 13,

		/// <summary>	Do Not Use. </summary>
		LineSegmentA_MAX					= 0xff
	};	
};

typedef LineSegmentParameters OpenCLRayAnalysisParameters;

}