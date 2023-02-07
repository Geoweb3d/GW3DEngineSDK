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

namespace Geoweb3d
{
	namespace Vector
	{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Z-axis translation mode (i.e. elevation mode) </summary>
	///
	/// <remarks>	Vertical (Elevation) Offset Mode
	/// 			ABSOLUTE_MODE | IGNORE_VERTEX_Z:
	/// 			absolute (relative to sea level), and if the vertex has Z, ignore it
	/// 			RELATIVE_MODE | IGNORE_VERTEX_Z:
	/// 			relative to elevation data, and if the vertex has Z, ignore it
	/// 			ABSOLUTE_MODE:
	/// 			absolute (relative to sea level, or relative to the z coordinate if it exists)
	/// 			RELATIVE_MODE:
	/// 			relative to elevation data, and if the vertex has Z, use it as an additional
	/// 			offset from the elevation data. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum TRANSLATION_Z_OFFSET_MODE
	{
		/// <summary>	Absolute Vertical Offset Mode </summary>
		ABSOLUTE_MODE	= 0x0,
		/// <summary>	Relative Vertical Offset Mode </summary>
		RELATIVE_MODE	= 0x1,
		/// <summary>	Ignore Z Coordinate </summary>
		IGNORE_VERTEX_Z	= 0x2,
		/// <summary>	MAX - Do Not Use </summary>
		FF_TZ_MAX		= 0xFFFF
	};

	enum OITMode
	{
		ALPHA_MODE = 0,		//Basis Alpha composition mode
		STIPPLE_MODE = 1,	//Stipple mode
		OITModeMax = 0xFFFF
	};

	enum InterpolationAlgorithm
	{
		HAVERSINE = 0,
		VINCENTY = 1,
		/// <summary>	InterpolationAlgorithmMax - Can be used for looping over the modes. </summary>
		InterpolationAlgorithmMax,
		/// <summary>	InterpolationAlgorithmMax_DATA_SIZER - Do Not Use </summary>
		InterpolationAlgorithmMax_DATA_SIZER = 0xFFFF
	};

	enum OutlineMode
	{
		OUTLINE_OFF = 0,
		OUTLINE_ON = 1
	};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Field Filter Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct FieldFilterProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Properties that can only be set for representation layer defaults. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum LayerDefaultsOnlyProperties
	{
		/// <summary>	Display Last N Features Per Object ID </summary>
		DISPLAY_LAST_N	= 0,
		/// <summary>	MAX - Do Not Use </summary>
		FF_EL_MAX		= 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	MAX - Do Not Use </summary>
		LP_PARAM_MAX = 0xFFFF
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Light Point Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct LightPointProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Color - Red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Color - Green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Color - Blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Light intensity </summary>
		INTENSITY = 7,
		/// <summary>	Light falloff (meters) </summary>
		FALL_OFF  = 8,
		/// <summary>	Bulb radius (meters) </summary>
		BULB_RADIUS= 9,
		/// <summary>	Illumination radius (meters) </summary>
		ILLUMINATION_RADIUS_CUTOFF= 10,
		/// <summary>	MAX - Do Not Use </summary>
		LP_PARAM_MAX = 0xFFFF
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Spot Light Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct SpotLightProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Color - Red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Color - Green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Color - Blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Light intensity </summary>
		INTENSITY			= 7,
		/// <summary>	Light falloff (meters) </summary>
		FALL_OFF			= 8,
		/// <summary>	Bulb radius (meters) </summary>
		BULB_RADIUS			= 9,
		/// <summary>	Illumination radius (meters) </summary>
		ILLUMINATION_RADIUS_CUTOFF= 10,
		/// <summary>	Inner cutoff angle </summary>
		INNER_CUTOFF_ANGLE = 11,
		/// <summary>	Outer cutoff angle </summary>
		OUTER_CUTOFF_ANGLE = 12,
		/// <summary>	Model heading (degrees) </summary>
		HEADING				= 13,
		/// <summary>	Model pitch (degrees) </summary>
		PITCH				= 14,
		/// <summary>	Model roll (degrees) </summary>
		ROLL				= 15,
		/// <summary>	MAX - Do Not Use </summary>
		LP_PARAM_MAX = 0xFFFF,		
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Model Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct ModelProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Model scale along the X axis (1.0 is native scale)</summary>
		SCALE_X				= 4,
		/// <summary>	Model scale along the Y axis (1.0 is native scale) </summary>
		SCALE_Y				= 5,
		/// <summary>	Model scale along the Z axis (1.0 is native scale) </summary>
		SCALE_Z				= 6,
		/// <summary>	Model heading (degrees) </summary>
		HEADING				= 7,
		/// <summary>	Model pitch (degrees) </summary>
		PITCH				= 8,
		/// <summary>	Model roll (degrees) </summary>
		ROLL				= 9,
		/// <summary>	Model file url/path </summary>
		URL					= 10,
		/// <summary>	Model alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 11,
		/// <summary>	MAX - Do Not Use </summary>
		MP_IP_MAX			= 0xFFFF
	};

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Colored Lines Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct ColoredLinesProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Display modes for colored line representations. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum DisplayModes
	{
		/// <summary>	Normal extruded mode </summary>
		NORMAL_MODE		= 0x0,
		/// <summary>	Draped mode (elevation must remain 0.0)</summary>
		DRAPED			= 0x1,
		/// <summary>	MAX - Do Not Use </summary>
		DISPLAY_MODES_MAX= 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Line color - Red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Line color - Green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Line color - Blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Line alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 7,
		/// <summary>	MAX - Do Not Use </summary>
		CL_IP_MAX			= 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Properties that can only be set for representation layer defaults. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum LayerDefaultsOnlyProperties
	{
		/// <summary>	Line width (pixels) </summary>
		LINE_WIDTH = 8,
		/// <summary>	Disable depth writing (Set to true for cases when you want to draw lines 
		/// 			in the scene but don't want them to influence the depth buffer.  They will
		/// 			still be depth tested. </summary>
		DISABLE_DEPTH_WRITING = 9,
		DISPLAY_MODE  = 10,
		/// <summary>	MAX - Do Not Use </summary>
		CL_LP_MAX			= 0xFFFF
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Curtain Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct CurtainProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		TRANSLATION_OFFSET_HEADING = 0,
		TRANSLATION_OFFSET_DISTANCE = 1,
		TRANSLATION_Z_OFFSET = 2,
		TRANSLATION_Z_OFFSET_MODE = 3,
		FRONT_CURTAIN_RED = 4,
		FRONT_CURTAIN_GREEN = 5,
		FRONT_CURTAIN_BLUE = 6,
		FRONT_CURTAIN_ALPHA = 7,
		BACK_CURTAIN_RED = 8,
		BACK_CURTAIN_GREEN = 9,
		BACK_CURTAIN_BLUE = 10,
		BACK_CURTAIN_ALPHA = 11,
		DRAW_POST = 12,
		POST_RESERVED = 13,
		POST_RED = 14,
		POST_GREEN = 15,
		POST_BLUE = 16,
		POST_ALPHA = 17,
		ALLOW_SEGMENTATION = 18,
		EL_IP_MAX = 0xFFFF
	};

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Box Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct BoxProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Box color - Red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Box color - Green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Box color - Blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Box alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 7,
		/// <summary>	Order independent transparency mode </summary>
		OIT = 8,
		/// <summary>	Texture palette index </summary>
		TEXTURE_PALETTE_INDEX	= 9,
		/// <summary>	Heading (degrees) </summary>
		HEADING				= 10,
		/// <summary>	Pitch (degrees) </summary>
		PITCH				= 11,
		/// <summary>	Roll (degrees) </summary>
		ROLL				= 12,
		/// <summary>	Extents along the X axis (meters) </summary>
		X_EXTENT			= 13,
		/// <summary>	Extents along the Y axis (meters) </summary>
		Y_EXTENT			= 14,
		/// <summary>	Extents along the Z axis (meters) </summary>
		Z_EXTENT			= 15,
		/// <summary>	MAX - Do Not Use </summary>
		BP_IP_MAX			= 0xFFFF
	};

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Cone Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct ConeProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Cone color - Red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Cone color - Green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Cone color - Blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Cone alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 7,
		/// <summary>	Order independent transparency mode </summary>
		OIT = 8,
		/// <summary>	Texture palette index </summary>
		TEXTURE_PALETTE_INDEX	= 9,
		/// <summary>	Heading (degrees) </summary>
		HEADING				= 10,
		/// <summary>	Pitch (degrees) </summary>
		PITCH				= 11,
		/// <summary>	Roll (degrees) </summary>
		ROLL				= 12,
		/// <summary>	Cone height (meters) </summary>
		HEIGHT				= 13,
		/// <summary>	Cone radius (meters) </summary>
		RADIUS				= 14,
		/// <summary>	Radial samples (simplification) </summary>
		RADIAL_SAMPLES		= 15,
		/// <summary>	Axis samples (simplification) </summary>
		AXIS_SAMPLES		= 16,
		/// <summary>	MAX (Do Not Use) </summary>
		CP_IP_MAX			= 0xFFFF
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Cylinder Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct CylinderProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Cylinder color - Red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Cylinder color - Green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Cylinder color - Blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Cylinder alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 7,
		/// <summary>	Order independent transparency mode </summary>
		OIT = 8,
		/// <summary>	Texture palette index </summary>
		TEXTURE_PALETTE_INDEX	= 9,
		/// <summary>	Heading (degrees) </summary>
		HEADING				= 10,
		/// <summary>	Pitch (degrees) </summary>
		PITCH				= 11,
		/// <summary>	Roll (degrees) </summary>
		ROLL				= 12,
		/// <summary>	Height of cylinder (meters) </summary>
		HEIGHT				= 13,
		/// <summary>	Radius of cylinder (meters) </summary>
		RADIUS				= 14,
		/// <summary>	Slices (horizontal simplification) </summary>
		SLICES				= 15,
		/// <summary>	Stacks (vertical simplification) </summary>
		STACKS				= 16,
		/// <summary>	Cylinder caps (set to true to render caps at ends of cylinder) </summary>
		CAPS				= 17,
		/// <summary>	MAX - Do Not Use </summary>
		CYP_IP_MAX			= 0xFFFF
	};

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Projected Image Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct ProjectedImageProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Projected image color bias - Red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Projected image color bias - Green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Projected image color bias - Blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Projected image alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 7,
		/// <summary>	Order independent transparency mode </summary>
		OIT = 8,
		/// <summary>	Texture palette index </summary>
		TEXTURE_PALETTE_INDEX	= 9,
		/// <summary>	Heading (degrees) </summary>
		HEADING				= 10,
		/// <summary>	Pitch (degrees) </summary>
		PITCH				= 11,
		/// <summary>	Roll (degrees) </summary>
		ROLL				= 12,
		/// <summary>	Camera field of view angle (degrees) </summary>
		CAMERA_FIELD_OF_VIEW	= 13,
		/// <summary>	Camera near plane distance from origin point (meters) </summary>
		CAMERA_NEAR_PLANE		= 14,
		/// <summary>	Camera far plane distance from origin point (meters) </summary>
		CAMERA_FAR_PLANE		= 15,
		/// <summary>	Draw Frustum (set to true to draw an outline of the camera frustum) </summary>
		DRAW_FRUSTUM			= 16,
		/// <summary>	MAX - Do Not Use </summary>
		PIP_IP_MAX			= 0xFFFF
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Plane Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct PlaneProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for individual features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Plane color - Red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Plane color - Green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Plane color - Blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Plane alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 7,
		/// <summary>	Order independent transparency mode </summary>
		OIT = 8,
		/// <summary>	Texture palette index </summary>
		TEXTURE_PALETTE_INDEX	= 9,
		/// <summary>	Heading (degrees) </summary>
		HEADING				= 10,
		/// <summary>	Pitch (degrees) </summary>
		PITCH				= 11,
		/// <summary>	Roll (degrees) </summary>
		ROLL				= 12,
		/// <summary>	Extents along the X axis (meters) </summary>
		X_EXTENT			= 13,
		/// <summary>	Extents along the Y axis (meters) </summary>
		Y_EXTENT			= 14,
		/// <summary>	Samples along the X axis </summary>
		X_SAMPLES			= 15,
		/// <summary>	Samples along the Y axis </summary>
		Y_SAMPLES			= 16,
		/// <summary>	MAX - Do Not Use </summary>
		PP_IP_MAX			= 0xFFFF
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Pyramid Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct PyramidProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Pyramid color - Red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Pyramid color - Green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Pyramid color - Blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Pyramid alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 7,
		/// <summary>	Order independent transparency mode </summary>
		OIT = 8,
		/// <summary>	Heading (degrees) </summary>
		HEADING				= 9,
		/// <summary>	Pitch (degrees) </summary>
		PITCH				= 10,
		/// <summary>	Roll (degrees) </summary>
		ROLL				= 11,
		/// <summary>	Extents along the X axis (meters) </summary>
		X_EXTENT			= 12,
		/// <summary>	Extents along the Y axis (meters) </summary>
		Y_EXTENT			= 13,
		/// <summary>	Extents along the Z axis (meters) </summary>
		Z_EXTENT			= 14,
		/// <summary>	MAX - Do Not Use </summary>
		PYP_IP_MAX			= 0xFFFF
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sphere Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct SphereProperties
{
	enum FillMode
	{
		//GW3D added due to name collisions with other sdks
		GW3DSOLID = 0,		//fill only
		GW3DOUTLINE = 1,	//lines only, no fill. This is not supported for representations with DRAPED display mode.
		GW3DBOTH = 2,		//render filled with lines.
		FillModeMax = 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Sphere color - Red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Sphere color - Green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Sphere color - Blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Sphere alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 7,
		/// <summary>	Order independent transparency mode </summary>
		OIT = 8,
		/// <summary>	Texture palette index </summary>
		TEXTURE_PALETTE_INDEX	= 9,
		/// <summary>	Heading (degrees) </summary>
		HEADING				= 10,
		/// <summary>	Pitch (degrees) </summary>
		PITCH				= 11,
		/// <summary>	Roll (degrees) </summary>
		ROLL				= 12,
		/// <summary>	Sphere radius (meters) </summary>
		RADIUS				= 13,
		/// <summary>	Slices (horizontal simplification), if -1, will auto generate a good quality based on radius</summary>
		SLICES				= 14,
		/// <summary>	Stacks (vertical simplification), if -1, will auto generate a good quality based on radius </summary>
		STACKS				= 15,
		/// <summary>	Fill Mode, draw as filled, wireframe (lines), or both, See Fill_Mode enum FillMode </summary>
		FILL_MODE			= 16,
		/// <summary>	MAX - Do Not Use </summary>
		SHP_IP_MAX			= 0xFFFF
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Draped Ring Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct DrapedRingProperties
{
	enum DrapedRingLightingMode
	{
		UNSHADED = 0,
		SHADED = 1,
		/// <summary>	DrapedRingLightingModeMax - Can be used for looping over the modes. </summary>
		DrapedRingLightingModeMax,
		/// <summary>	DrapedRingLightingModeMax_DATA_SIZER - Do Not Use </summary>
		DrapedRingLightingModeMax_DATA_SIZER = 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Color - Red (0.0 to 1.0) </summary>
		RED = 0,
		/// <summary>	Color - Green (0.0 to 1.0) </summary>
		GREEN = 1,
		/// <summary>	Color - Blue (0.0 to 1.0) </summary>
		BLUE = 2,
		/// <summary>	Sphere alpha transparency (0.0 to 1.0)(NOT SUPPORTED YET) </summary>
		ALPHA = 3,
		/// <summary>	Texture palette index </summary>
		TEXTURE_PALETTE_INDEX = 4,
		/// <summary>	Texture blend factor( only texture (1.0) to only color (0.0) ).</summary>
		TEXTURE_TO_COLOR_BLEND = 5,
		/// <summary>	Inner Radius. (meters) 
		/// Inner radius will be clamped at the current outer radius. </summary>
		INNER_RAD = 6,
		/// <summary>	Outer Radius. (meters) </summary>
		OUTER_RAD = 7,
		/// <summary>	Minimum line width in screen space (pixels). (0.0 to 32.0) 
		/// Line width is only applied if the metric width is too small to cover the pixels. 
		/// Line width set to 0 will deactivate this capability as an optimization. </summary>
		MIN_SCREEN_SPACE_EXTENT = 8,
		/// <summary> Uses an outlining texture for contrast. </summary>
		OUTLINE_MODE = 9,
		/// <summary>	Lighting mode. (Turn shading on/off) </summary>
		LIGHTING_MODE = 10,
		/// <summary>	MAX - Do Not Use </summary>
		DR_PARAM_MAX = 0xFFFF
	};

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Draped Ring Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct DrapedLineProperties
{
	enum DrapedLineLightingMode
	{
		UNSHADED = 0,
		SHADED = 1,
		/// <summary>	DrapedLineLightingModeMax - Can be used for looping over the modes. </summary>
		DrapedLineLightingModeMax,
		/// <summary>	DrapedLineLightingModeMax_DATA_SIZER - Do Not Use </summary>
		DrapedLineLightingModeMax_DATA_SIZER = 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Color - Red (0.0 to 1.0) </summary>
		RED = 0,
		/// <summary>	Color - Green (0.0 to 1.0) </summary>
		GREEN = 1,
		/// <summary>	Color - Blue (0.0 to 1.0) </summary>
		BLUE = 2,
		/// <summary>	Sphere alpha transparency (0.0 to 1.0)(NOT SUPPORTED YET) </summary>
		ALPHA = 3,
		/// <summary>	Texture palette index </summary>
		TEXTURE_PALETTE_INDEX = 4,
		/// <summary>	Texture blend factor( only texture (1.0) to only color (0.0) ).</summary>
		TEXTURE_TO_COLOR_BLEND = 5,
		/// <summary>	Width extent. (meters) </summary>
		WIDTH = 6,
		/// <summary>	Minimum line width in screen space (pixels). (0 to 32) 
		/// Line width is only applied if the metric width is too small to cover the pixels. 
		/// Line width set to 0 will deactivate this capability as an optimization. </summary>
		LINE_WIDTH = 7,
		/// <summary> Uses an outlining texture for contrast. </summary>
		OUTLINE_MODE = 8,
		/// <summary> In development </Summary>
		//LABEL = 11,
		/// <summary>	MAX - Do Not Use </summary>
		DR_PARAM_MAX = 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Properties that can only be set for representation layer defaults. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum LayerDefaultsOnlyProperties
	{
		/// <summary>	Order independent transparency mode </summary>
		OIT = 9,
		LIGHTING_MODE = 10,
		/// <summary>	MAX - Do Not Use </summary>
		LDO_MAX = 0xFFFF
	};
};

struct RingProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Color bias - red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Color bias - green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Color bias - blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 7,
		/// <summary>	Order independent transparency mode </summary>
		OIT = 8,
		/// <summary>	Heading (degrees) </summary>
		HEADING				= 9,
		/// <summary>	Pitch (degrees) </summary>
		PITCH				= 10,
		/// <summary>	Roll (degrees) </summary>
		ROLL				= 11,
		/// <summary>	Number of bearing lines (MUST be at least one).  The first bearing line always points north if heading is zero. </summary>
		NUMBER_OF_BEARING_LINES = 12, 
		/// <summary>	Width of the bearing lines in screen pixels. </summary>
		BEARING_LINE_WIDTH = 13, 
		/// <summary>	Bearing line color - red (0.0 to 1.0) </summary>
		BEARING_LINE_RED   = 14,
		/// <summary>	Bearing line color - green (0.0 to 1.0) </summary>
		BEARING_LINE_GREEN = 15,
		/// <summary>	Bearing line color - blue (0.0 to 1.0) </summary>
		BEARING_LINE_BLUE	= 16,
		/// <summary>	Bearing line alpha transparency (0.0 to 1.0) </summary>
		BEARING_LINE_ALPHA	= 17,
		/// <summary>	Set to 0 do draw rings as outlines, or 1 to draw rings as filled bands. </summary>
		DRAW_RING_TYPE = 18,
		/// <summary>	Line width of the rings in screen pixels. </summary>
		RING_LINE_WIDTH = 19,
		/// <summary>	The inner radius of the ring 0 (0 to have first ring start in the center). </summary>
		RING_0_INNER_RADIUS = 20,
		/// <summary>	Distance from inner radius of ring 0 to the outer radius of the ring 0. </summary>
		RING_0_INTERVAL = 21,
		/// <summary>	Number of additional rings beyond the first (ring 0). Each must have its own configuration.
		///				See CreateNumberOfAdditionalRings in 'include\Geoweb3d\misc\GW3DRangeRingHelper.h' as a helper
		///				to modify the property definition to support additional ring configurations </summary>
		NUMBER_OF_ADDITIONAL_RINGS = 22,
		/// <summary>	Distance from outer radius of ring 0 to outer radius of ring 1. </summary>
		RING_1_INTERVAL     = 23, 
		/// <summary>	Ring 1 color - red (0.0 to 1.0) </summary>
		RING_1_RED			= 24,
		/// <summary>	Ring 1 color - green (0.0 to 1.0) </summary>
		RING_1_GREEN		= 25,
		/// <summary>	Ring 1 color - blue (0.0 to 1.0) </summary>
		RING_1_BLUE			= 26,
		/// <summary>	Ring 1 alpha transparency (0.0 to 1.0) </summary>
		RING_1_ALPHA		= 27,
		/// <summary>	MAX - Do Not Use. </summary>
		RP_IP_MAX			= 0xFFFF
	};
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Surface Polygon Representation Properties </summary>
//////////////////////////////////////////////////////////////////////////////////////////////////////

struct SurfacePolygonProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Color bias - red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Color bias - green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Color bias - blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 7,
		/// <summary>	MAX - Do Not Use. </summary>
		SFP_SU_MAX			= 0xFFFF
	};

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Extruded Polygon Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct ExtrudedPolygonProperties
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Display modes for polygon representations. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum DisplayModes
	{
		/// <summary>	Normal extruded mode </summary>
		NORMAL_MODE		= 0x0,
		/// <summary>	Draped mode (extrusion height and elevation must remain 0.0)</summary>
		DRAPED			= 0x1,
		/// <summary>	MAX - Do Not Use </summary>
		DISPLAY_MODES_MAX= 0xFFFF
	};

	enum FillMode
	{
		//GW3D added due to name collisions with other sdks
		GW3DSOLID = 0,		//fill only
		GW3DOUTLINE = 1,	//lines only, no fill. This is not supported for representations with DRAPED display mode.
		GW3DBOTH = 2,		//render filled with lines. This is not supported for representations with DRAPED display mode.
		FillModeMax= 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Top texture mapping modes for polygon representations. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum TopTextureMappingModes
	{
		/// <summary>	Maps the texture coordinates based on the the bounding box</summary>
		BOUNDINGBOX		= 0x0,  
		/// <summary>	Maps the texture coordinates based on the longest edge of the bounding box</summary>
		LONGEST_EDGE	= 0x1,  
		/// <summary>	MAX - Do Not Use </summary>
		MAPMODE_MAPPING_MAX			= 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Properties that can only be set for representation layer defaults. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum LayerDefaultsOnlyProperties
	{
		/// <summary>	Display Mode </summary>
		DISPLAY_MODE		= 18,
		FILL_MODE			= 20,
		OUTLINE_COLOR_BIAS	= 21,
		OUTLINE_LINE_WIDTH	= 22,
		/// <summary>	Order independent transparency mode </summary>
		OIT		= 23,
		/// <summary>	MAX - Do Not Use </summary>
		EPL_MAX				= 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Properties that only take effect on construction or paging </summary>
	///
	/// <remarks>	These properties currently only get applied when the data is being paged to the 
	/// 			renderer. This means either they should be constructed on representation creation,
	/// 			or the representation should be forced to page back in (i.e. move the eye to 
	/// 			outer space and back, or force the paging distance to change). </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum ConstructionProperties
	{
		/// <summary>	Number of times to repeat the top texture along the X axis (0 is invalid) </summary>
		TEXTURE_TOP_REPEAT_N_TIMES_X	= 10,
		/// <summary>	Number of times to repeat the top texture along the Y axis(0 is invalid) </summary>
		TEXTURE_TOP_REPEAT_N_TIMES_Y	= 11,
		/// <summary>	Number of times to repeat the side texture horizontally (0 is invalid) </summary>
		TEXTURE_SIDE_REPEAT_N_TIMES_X	= 12,
		/// <summary>	Number of times to repeat the side texture vertically (0 is invalid) </summary>
		TEXTURE_SIDE_REPEAT_N_TIMES_Y	= 13,
		/// <summary>	Mode for mapping the top texture </summary>
		TEXTURE_TOP_MAPPING_MODE	= 14,
		/// <summary>	Simplification tolerance factor </summary>
		SIMPLIFICATION_THRESHOLD	= 17,
		/// <summary>	MAX - Do Not Use </summary>
		EPP_EPP_CP_MAX				= 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Unsuppported </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Unsuppported </summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Polygon color - Red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Polygon color - Green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Polygon color - Blue (0.0 to 1.0) </summary>
		BLUE				= 6,
		/// <summary>	Polygon alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 7,
		/// <summary>	Extrusion height (meters) </summary>
		EXTRUSION_HEIGHT	= 8,
		/// <summary>	Blend between color and texture </summary>
		/// <remarks>	Controls the amount of blend between color and texture.  Valid values
		///				range from 0.0 to 1.0, where 0.5 would result in a 50/50 blend( only texture (1.0) to only color (0.0) ).
		TEXTURE_TO_COLOR_BLEND = 9,
		/// <summary>	Texture palette index  for the texture to map on the sides of the extruded polygon </summary>
		TEXTURE_SIDE_PALETTE_INDEX	= 15,
		/// <summary>	Texture palette index  for the texture to map on the top of the extruded polygon </summary>
		TEXTURE_TOP_PALETTE_INDEX	= 16,
		/// <summary>	Enable textured polygon mode </summary>
		TEXTURE_MODE_ENABLE			= 19,
		/// <summary>	Enable the rendering of the bottom of the extrusion.  </summary>
		RENDER_BOTTOM = 24,
		/// <summary>	MAX - Do Not Use </summary>
		EPP_EPP_MAX			= 0xFFFF
	};

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Extruded Path Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct ExtrudedPathProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Render Mode</summary>
	///
	/// <remarks>	LINE_BACKGROUND typically is used for contrast so the foreground line will stand out.
	///  			LINE_BACKGROUND will also apply to the extruded path to give it contrast; however,
	///				it will not be noticed with a texture unless the TEXTURE_TO_COLOR_BLEND is set 
	///				appropriately. If LINE_FOREGROUND is also set the EXTRUDED_PATH will seek to 
	///				honor the ratio of these values for its underlying coloring. 
	/// 
	///				The Curtain currently does not have any contrasting border associated with these values. </remarks>	
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum RenderMode
	{
		EXTRUDED_PATH = 0x1,
		LINE = 0x2,
		CURTAIN = 0x4,
		DRAPED_LINE = 0x8,
		/// <summary>	RenderModeMax - Can be used for looping over the modes. </summary>
		RenderModeMax,
		/// <summary>	RenderModeMax_DATA_SIZER - Do Not Use </summary>
		RenderModeMax_DATA_SIZER = 0xFFFF
	};

	enum LightingMode
	{
		NONE_SHADED = 0x0,
		EXTRUDED_PATH_SHADED = 0x1,
		CURTAIN_SHADED = 0x2,
		DRAPED_LINE_SHADED = 0x4,
		/// <summary>	LightingModeMax - Can be used for looping over the modes. </summary>
		LightingModeMax,
		/// <summary>	LightingModeMax_DATA_SIZER - Do Not Use </summary>
		LightingModeMax_DATA_SIZER = 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Properties that can only be set for representation layer defaults. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum LayerDefaultsOnlyProperties
	{
		/// <summary>	Order independent transparency mode </summary>
		OIT = 13,
		RENDER_MODE = 14,
		LIGHTING_MODE = 20,
		/// <summary>	MAX - Do Not Use </summary>
		LDO_MAX = 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Properties that only take effect on construction or paging </summary>
	///
	/// <remarks>	These properties currently only get applied when the data is being paged to the 
	/// 			renderer. This means either they should be constructed on representation creation,
	/// 			or the representation should be forced to page back in (i.e. move the eye to 
	/// 			outer space and back, or force the paging distance to change). </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum ConstructionProperties
	{
		/// <summary>	Distance to Segment at. </summary>
		/// <remarks>   This stores vertices in memory.
		///				Caution should be taken to understand the extend of the data
		///				that will be produced and the associated load on the system. 
		///				Minimum allowed effective value is 1 meter. 
		///				A value of 0 will disable this setting. </remarks>
		SEGMENTATION_DISTANCE = 18,
		/// <summary>	Segments are produced using Haversine's Formula, but are dynamically
		///				created when needed on the GPU.  This setting works in conjunction with 
		///				the SEGMENTATION_DISTANCE setting.
		///				Note: In order to attempt to achieve the DESIRED_RESOLUTION the SEGMENTATION_DISTANCE
		///				value may need to be overridden to a finer precision. 
		/// <remarks>   Caution should be taken to understand the extend of the data
		///				that will be produced and the associated load on the system.
		///				The GPU based tessellator allows for 64 levels of segmentation.
		///				if the desired resolution cannot be reached with 64 segments
		///				then the SEGMENTATION_DISTANCE will be adjusted.
		///				So to reiterate; if 64 times DESIRED_RESOLUTION is greater than SEGMENTATION_DISTANCE
		///				then SEGMENTATION_DISTANCE will now equal 64 times DESIRED_RESOLUTION.
		///				Minimum allowed effective value is 1 meter.
		///				A value of 0 will disable this setting. </remarks>
		DESIRED_RESOLUTION = 19,
		/// <summary>	Use previous and next geometry in multi list to determine joint angles.  </summary>
		MULTI_GEOMETRY_CONNECTION = 21,
		/// <summary>	Determines the Interpolation Algorithm to be use via SEGMENTATION_DISTANCE </summary>
		SEGMENTATION_INTERPOLATION = 22,
		/// <summary>	MAX - Do Not Use </summary>
		CP_MAX = 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Unsuppported </summary>
		TRANSLATION_OFFSET_HEADING = 0,
		/// <summary>	Unsuppported </summary>
		TRANSLATION_OFFSET_DISTANCE = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Polygon color - Red (0.0 to 1.0) </summary>
		RED = 4,
		/// <summary>	Polygon color - Green (0.0 to 1.0) </summary>
		GREEN = 5,
		/// <summary>	Polygon color - Blue (0.0 to 1.0) </summary>
		BLUE = 6,
		/// <summary>	Polygon alpha transparency (0.0 to 1.0) </summary>
		ALPHA = 7,
		/// <summary>	Blend between color and texture </summary>
		/// <remarks>	Controls the amount of blend between color and texture.  Valid values
		///				range from 0.0 to 1.0, where 0.5 would result in a 50/50 blend( only texture (1.0) to only color (0.0) ).
		///				A value of -1 disables blending. </remarks>
		TEXTURE_TO_COLOR_BLEND = 8,
		/// <summary>	Texture palette index for the texturing the Extruded Path</summary>
		TEXTURE_PALETTE_INDEX = 9,
		/// <summary>	Enable textured polygon mode </summary>
		TEXTURE_MODE_ENABLE = 10,
		/// <summary>	Left distance of Extruded Path in Meters </summary>
		LEFT_DISTANCE = 11,
		/// <summary>	Left distance of Extruded Path in Meters </summary>
		RIGHT_DISTANCE = 12,
		CURTAIN_COLOR_BIAS = 15,
		/// <summary>	Minimum line width in screen space (pixels). (0 to 32) 
		/// The renderings are stretched to the screen space Line width if the metric width is too small to cover the pixels. 
		///  </summary>
		LINE_WIDTH = 16,
		/// <summary> Uses an outlining texture for contrast. </summary>
		OUTLINE_MODE = 17,
		DRAPED_LINE_COLOR_BIAS = 23,
		CURTAIN_ALPHA = 24,
		DRAPED_LINE_ALPHA = 25,
		/// <summary>	MAX - Do Not Use </summary>
		EPP_EPP_MAX = 0xFFFF
	};

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Point Cloud Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct PointCloudProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Point Cloud Render Modes </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum PointCloudRenderModes
	{
		/// <summary>	Display the native color </summary>
		COLOR_MODE	= 0x0,
		/// <summary>	Color points based on classification number. </summary>
		CLASSIFICATION_MODE		= 0x1,
		/// <summary>	Color points based on return number. </summary>
		RETURN_MODE				= 0x2,
		/// <summary>	Color points based on altitude. </summary>
		HEIGHT_SHADING			= 0x3,
		/// <summary>	Display the intensity </summary>
		INTENSITY_MODE			= 0x4,
		/// <summary>	MAX - Do Not Use. </summary>
		PC_RM_MAX = 0xFFFF
	};

	enum PointCloudPixelSizeModes
	{
		/// <summary> Employs and adaptive pixel size algorithm	 </summary>
		ADAPTIVE_DEFAUT_SPACING_MODE = 0x0,
		/// <summary> Employs and adaptive pixel size algorithm	 </summary>
		ADAPTIVE_USE_USER_SPACING_MODE = 0x1,
		/// <summary> Uses the Min Max Pixel setting values	 </summary>
		USE_MIN_MAX_DIST_MODE = 0x2,
		/// <summary>	MAX - Do Not Use. </summary>
		PC_PSM_MAX = 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Properties that can only be set for representation layer defaults. </summary>
	///
	/// <remarks>	Visible distance values these are really ranges: depth 0 starts becoming visible at
	///				D0 and becomes fully visible at D1, where depth 1 starts at D1 and becomes fully 
	///				visible at D2, etc. As such, you need to define N + 1 where N = OCTREE_DEPTH to 
	///				cover all the cases (currently supports up to, but not including 9 levels) </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum LayerDefaultsOnlyProperties
	{
		/// <summary>	Render mode. </summary>
		RENDER_MODE = 16,
		/// <summary>	If rendering an intensity map, saet to true to normalize the intensity values. </summary>
		APPLY_INTENSITY_NORMALIZATION = 17,
		/// <summary>	Set the color for each classification.
		///				Implemented as a hex triplet for color with the first byte as 0 for don't render and anything else for render
		///				I.e. : 0x red, ff green, ff blue, ff render/or not. </summary>
		CLASSIFICATION_DATA_0 = 18,
		/// <summary>	Further classifications. </summary>
		CLASSIFICATION_DATA_31 = CLASSIFICATION_DATA_0 + 31,
		/// <summary>	Set the color for each return.
		///				Implemented as a hex triplet for color with the first byte as 0 for don't render and anything else for render
		///				I.e. : 0x red, ff green, ff blue, ff render/or not. </summary>
		RETURN_DATA_0 = CLASSIFICATION_DATA_31 + 1,
		/// <summary>	Further returns. </summary>
		RETURN_DATA_7 = RETURN_DATA_0 + 7,
		/// <summary>	If rendering with height shading, set minimum height above sea level for color ramp. </summary>
		HEIGHT_SHADING_START_HEIGHT = RETURN_DATA_7 + 1,
		/// <summary>	If rendering with height shading, set maximum height above sea level for color ramp. </summary>
		HEIGHT_SHADING_END_HEIGHT = HEIGHT_SHADING_START_HEIGHT + 1,
		/// <summary>	Will start rendering nodes when they surpase this screen space size. [30 to 500]  </summary>
		MIN_NODE_SIZE = HEIGHT_SHADING_END_HEIGHT + 1,
		/// <summary>	Pixel Size Mode </summary>
		PIXEL_SIZE_MODE = MIN_NODE_SIZE + 1,
		/// <summary>	Render down to this LoD level [0 to 50] </summary>
		MAX_RENDER_LEVEL = PIXEL_SIZE_MODE + 1,
		/// <summary>	This is generally the minimum spacing expected for points in the root node of the dataset
		/// PoTree datasets will usually have this set. Set PIXEL_SIZE_MODE to ADAPTIVE_DEFAUT_SPACING_MODE
		/// to use the default of the data set. Set PIXEL_SIZE_MODE to ADAPTIVE_USE_USER_SPACING_MODE to use the set 
		/// value here. </summary>
		USER_MIN_SPACING = MAX_RENDER_LEVEL + 1,
		/// <summary>	MAX - Do Not Use </summary>
		PC_LYR_MAX = 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Distance at which terrain tree depth 0 start~s becoming visible. </summary>
		VISIBLE_DISTANCE_D0				= 4,
		/// <summary>	Distance at which terrain tree depth 1 starts becoming visible, and terrain tree
		///				depth 0 becomes completely visible. </summary>
		VISIBLE_DISTANCE_D1				= 5,
		/// <summary>	Distance at which terrain tree depth 2 starts becoming visible, and terrain tree
		///				depth 1 becomes completely visible. </summary>
		VISIBLE_DISTANCE_D2				= 6,
		/// <summary>	Distance at which terrain tree depth 3 starts becoming visible, and terrain tree
		///				depth 2 becomes completely visible. </summary>
		VISIBLE_DISTANCE_D3				= 7,
		/// <summary>	Distance at which terrain tree depth 4 starts becoming visible, and terrain tree
		///				depth 3 becomes completely visible. </summary>
		VISIBLE_DISTANCE_D4				= 8,
		/// <summary>	Distance at which terrain tree depth 5 starts becoming visible, and terrain tree
		///				depth 4 becomes completely visible. </summary>
		VISIBLE_DISTANCE_D5				= 9,
		/// <summary>	Distance at which terrain tree depth 6 starts becoming visible, and terrain tree
		///				depth 5 becomes completely visible. </summary>
		VISIBLE_DISTANCE_D6				= 10,
		/// <summary>	Distance at which terrain tree depth 7 starts becoming visible, and terrain tree
		///				depth 6 becomes completely visible. </summary>
		VISIBLE_DISTANCE_D7				= 11,
		/// <summary>	Depth to cache on the GPU. </summary>
		CACHE_DEPTH						= 12,
		/// <summary>	Minimum point size. </summary>
		MIN_POINT_SIZE					= 13,
		/// <summary>	Maximum point size. </summary>
		MAX_POINT_SIZE					= 14,
		/// <summary>	Distance over which to scale points from the min point size to the max point size, 
		///				where at distance 0 points are displayed at the max size. </summary>
		POINT_SCALE_DIST				= 15,
		/// <summary>	MAX - Do Not Use. </summary>
		PC_PARAM_MAX = 0xFFFF
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Custom Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct CustomProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>  radius(meters) to assist in culling as well as how many pixels in screen space when rendered</summary>
		RADIUS = 4,		
		/// <summary>	MAX - Do Not Use </summary>
		PCP_IP_MAX			= 0xFFFF
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Water Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct WaterProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Mode defining how water height is interpreted. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum WaterHeightModes
	{
		/// <summary>	Absolute height mode. </summary>
		ABSOLUTE_MODE			= 0x0,
		/// <summary>	Relative height mode. </summary>
		RELATIVE_MODE			= 0x1,
		/// <summary>	Relative height from minimum mode. </summary>
		RELATIVE_FROM_MIN_MODE	= 0x2,		
		/// <summary>	Relative height from maximum mode. </summary>
		RELATIVE_FROM_MAX_MODE	= 0x4,
		/// <summary>	MAX - Do Not Use. </summary>
		WATER_HEIGHT_MODE_MAX = 0xFFFF
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Direction in which to horizontally offset representations of features </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Distance to horizontally offset representations of features (meters)</summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Water color bias (Red) </summary>
		RED								= 4,
		/// <summary>	Water color bias (Green)  </summary>
		GREEN							= 5,
		/// <summary>	Water color bias (Blue)  </summary>
		BLUE							= 6,
		/// <summary>	Water height (meters) </summary>
		WATER_HEIGHT					= 7,
		/// <summary>	Water height mode </summary>
		WATER_HEIGHT_MODE				= 8,
		/// <summary>	Water flow heading (degrees) </summary>
		FLOW_HEADING					= 9,
		/// <summary>	Water flow speed </summary>
		FLOW_SPEED						= 10,
		/// <summary>	Water wave height </summary>
		MAX_WAVE_HEIGHT					= 11,
		/// <summary>	Reflection displacement </summary>
		REFLECTION_DISPLACEMENT			= 12,
		/// <summary>	Reflection error tolerance </summary>
		REFLECTION_ERROR_TOLERANCE		= 13,
		/// <summary>	Water opacity </summary>
		WATER_OPACITY_SCALE				= 14,
		/// <summary>	MAX - Do Not Use </summary>
		WP_PARAM_MAX = 0xFFFF
	};

	//A representation's CameraProperties are set via the GW3DRepresentation::put_Properties API
	enum CameraProperties
	{
		//include these when rendering
		REFLECT_TERRAIN				= 0,
		REFLECT_SKY					= 1,
		REFLECT_EXTRUDED_POLYGONS	= 2,
		REFLECT_COLORED_LINES		= 3,
		REFLECT_POINT_CLOUD			= 4,
		REFLECT_MODELS				= 5,
		REFLECT_BOX					= 6,
		REFLECT_CONE				= 7,
		REFLECT_CYLINDER			= 8,
		REFLECT_PLANE				= 9,
		REFLECT_PYRAMID				= 10,
		REFLECT_SPHERE				= 11,		
		//general parameters
		AUTO_ADJUST_LOCAL_PLANE		= 12,
		//if auto_adjust_local_plane = false, then it will use this value:
		REFLECTION_PLANE_HEIGHT		= 13,
		CONSIDER_AREA_WITH_AUTO_ADJUST	= 14,
		AUTO_ADJUST_AREA_CUTOFF		= 15,
		RO_MAX = 0xFFFF
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Billboard Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct BillboardProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Representation properties that can be set for unique features. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum IndividualProperties
	{
		/// <summary>	Unsuppported  </summary>
		TRANSLATION_OFFSET_HEADING	  = 0,
		/// <summary>	Unsuppported </summary>
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		/// <summary>	Distance to vertically offset representations of features (meters) </summary>
		TRANSLATION_Z_OFFSET	  = 2,
		/// <summary>	Mode in which to interpret the vertical offset representations of features </summary>
		TRANSLATION_Z_OFFSET_MODE = 3,
		/// <summary>	Color - Red (0.0 to 1.0) </summary>
		RED					= 4,
		/// <summary>	Color - Green (0.0 to 1.0) </summary>
		GREEN				= 5,
		/// <summary>	Color - Blue (0.0 to 1.0) </summary>
		BLUE				= 6,
			/// <summary>	Color - Alpha transparency (0.0 to 1.0) </summary>
		ALPHA				= 7,	
		/// <summary>	Texture palette index </summary>
		TEXTURE_PALETTE_INDEX	= 8,
		/// <summary>	Billboard  size in the x dimension in pixels.</summary>
		SIZE_X = 9,
		/// <summary>	Billboard  size in the y dimension in pixels.</summary>
		SIZE_Y = 10,
		/// <summary>	Texture blend factor( only texture (1.0) to only color (0.0) ). -1.0 = To modulate the texture color by the color properties.</summary>
		TEXTURE_TO_COLOR_BLEND = 11,
		/// <summary>	Leader line Color - Red (0.0 to 1.0) </summary>
		LEADER_RED  = 12,
		/// <summary>	Leader line Color - Green (0.0 to 1.0) </summary>
		LEADER_GREEN = 13,
		/// <summary>	Leader line Color - Blue (0.0 to 1.0) </summary>
		LEADER_BLUE = 14,
		/// <summary>	Leader line Color - Alpha transparency (0.0 to 1.0) </summary>
		LEADER_ALPHA = 15,
		/// <summary>	Leader line Texture palette index </summary>
		LEADER_TEXTURE_PALETTE_INDEX = 16,
		/// <summary>	Leader line  size in the x dimension in pixels.</summary>
		LEADER_SIZE_X = 17,
		/// <summary>	Leader line top y extend percent (0.0 to 100.0). Some images have an alpha surrounding area; adjust this value to 
		///get the top of the leader line to be closer to the actual rendered billboard pixels. The percent is of the radius of the billboard; 
		///so 100% wll extend the leader line to the center of the billboard. </summary>
		LEADER_TOP_Y_EXTEND_PERCENT = 18,
		/// <summary>	Billboard color/texture blend factor( only texture (1.0) to only color (0.0) ).</summary>
		LEADER_TEXTURE_TO_COLOR_BLEND = 19,
		/// <summary> </Summary>
		LABEL = 28,
		/// <summary> </Summary>
		LABEL_ANCHOR_POS = 29,
		/// <summary>	Additional Texture palette index. 
		/// Currently uses the same anchor position of the texture found at the TEXTURE_PALETTE_INDEX.  
		/// Currently no blending (as to not effect primary texture fidelity)- if the texture referenced by TEXTURE_PALETTE_INDEX left any fully transparent
		/// pixels then this secondary texture will be able to write to them.</summary>
		SECONDARY_TEXTURE_PALETTE_IDX = 30,
		/// <summary>	MAX - Do Not Use </summary>
		BBP_PARAM_MAX = 0xFFFF
	};

	/// Used to control Leader Lines on Billboards.
	enum BILLBOARD_LEADER_MODE
	{
		/// <summary>	No Leader Line is rendered. </summary>
		LEADER_LINE_OFF = 0,
		/// <summary>	A Leader Line is rendered from the billboard anchor position to the "Z" value of the point. </summary>
		LEADER_LINE_ON,
		/// <summary>	A Leader Line is rendered from the billboard anchor position to the "Z" value of the point. Another dashed
		/// leader line is rendered from the "Z" value of the point to the terrain surface.</summary>
		LEADER_LINE_ON_EXTENDED
	};

	/// Mask Used to control how occluded portions of billboards and leader lines are to be rendered.
	enum BILLBOARD_EFFECT_MASK
	{
		/// <summary>	The Occlusion Effect is disabled </summary>
		BILLBOARD_EFFECT_OFF = 0x0,
		/// <summary>	The Occlusion Effect is applied to billboards</summary>
		APPLY_EFFECT_TO_BILLBOARDS = 0x1,
		/// <summary>	The Occlusion Effect is applied to  leader lines </summary>
		APPLY_EFFECT_TO_LEADER_LINES = 0x2,
		/// <summary>	Helper to enable the effect on billboards and leader lines </summary>
		APPLY_EFFECT_TO_FEATURE = APPLY_EFFECT_TO_BILLBOARDS | APPLY_EFFECT_TO_LEADER_LINES,
		/// <summary>	MAX - Do Not Use </summary>
		BB_HBM_MAX = 0xFFFF
	};

	/// Used to control the look and feel of occluded billboards. 
	enum BILLBORD_EFFECT_RENDER_MODE
	{
		/// <summary>	The rendering of occluded billboards remains unchanged  </summary>
		OCCLUDED_BILLBOARDS_RENDER_UNCHANGED = 0,
		/// <summary>	Occluded Billboards are rendered with a dashed outline and leader lines get dashed. </summary>
		OCCLUDED_BILLBOARDS_RENDER_DASHED = 1,
		/// <summary>	Occluded Billboards and leader lines are rendered as stippled. </summary>
		OCCLUDED_BILLBOARDS_RENDER_SHADED = 2,
		/// <summary>	MAX - Do Not Use </summary>
		BB_ERM_MAX
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Properties that can only be set for representation layer defaults. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum LayerDefaultsOnlyProperties
	{
		/// <summary>	Allow internal algorithms to configure the billboard offset </summary>
		AUTOHEIGHT = 20,
		/// <summary>	Render Leader Lines, 0 - Off, 1 - On, 2 - Extended </summary>
		LEADER_LINES_ENABLED = 21,
		/// <summary>	Disable depth (Set to true for cases when you want to billboards in the scene but
		///				don't want them to influence the depth buffer or be depth tested. </summary>
		DISABLE_DEPTH = 22,
		/// <summary>	Use BILLBOARD_EFFECT_MASK to enable a per-pixel effect to occluded portions of billboards.
		///				Occluded pixels will get rendered on top of occluders using the render effect specified by
		///				the OCCLUDED_BILLBOARD_RENDER_MODE property. </summary>
		OCCLUDED_PER_PIXEL_EFFECT = 23,
		/// <summary>	Use BILLBOARD_EFFECT_MASK to enable a per-object effect to occluded billboards. Features that
		///				fail an occlusion test will get rendered using the render effect specified by
		///				the OCCLUDED_BILLBOARD_RENDER_MODE property. Occlusion is determined based on whether or not 
		///				the billboard's actual position can be seen (where leader lines point to when enabled otherwise
		///				the configured anchor point of the billboard).  </summary>
		OCCLUDED_PER_OBJECT_EFFECT = 24,
		/// <summary>	USE BILLBORD_EFFECT_RENDER_MODE to control the look and feel of billboards when occlusion
		///				effects are enabled.  </summary>
		OCCLUDED_BILLBOARD_RENDER_MODE = 25,
		/// <summary>	When OCCLUDED_BILLBOARD_RENDER_MODE is set to OCCLUDED_BILLBOARDS_RENDER_SHADED, gives control 
		///				over the amount of stippling that gets applied. 0.0 to 1.0.</summary>
		///				 </summary>
		OCCLUDED_BILLBOARD_SHADING_BLEND_AMOUNT = 26,
		/// <summary> </Summary>
		STACK_ORDER = 27,
		/// <summary>	MAX - Do Not Use </summary>
		BB_LP_MAX		= 0xFFFF
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Smoke Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct SmokeProperties
{
	enum IndividualProperties
	{
		TRANSLATION_OFFSET_HEADING	  = 0,
		TRANSLATION_OFFSET_DISTANCE	  = 1,
		TRANSLATION_Z_OFFSET	  = 2,
		TRANSLATION_Z_OFFSET_MODE = 3,
		HEADING	= 4,		
		PITCH = 5,		
		ROLL = 6,
		PLUME_HEIGHT = 7,
		PLUME_MAX_RADIUS = 8,
		PLUME_MIN_RADIUS = 9,
		PLUME_THRUST = 10,
		PLUME_HEAT = 11, 
		PLUME_LIFE_SPAN = 12, 
		RED = 13, 
		GREEN = 14, 
		BLUE = 15, 
		ALPHA = 16,
		SIMULATE = 17,
		PLUME_VORTEX_VELOCITY = 18
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Model Region Representation Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct ModelRegionProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Properties that can only be set for representation layer defaults. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum LayerDefaultsOnlyProperties
	{
		/// <summary>	 </summary>
		SSE_THRESHOLD = 0,
		/// <summary>	MAX - Do Not Use </summary>
		MR_PARAM_MAX = 0xFFFF
	};

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Label Properties </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct LabelProperties
{
	enum IndividualProperties
	{
		/// <summary>	Foreground Color - Red (0.0 to 1.0) </summary>
		FOREGROUND_COLOR_RED   = 0,
		/// <summary>	Foreground Color - Green (0.0 to 1.0) </summary>
		FOREGROUND_COLOR_GREEN = 1,
		/// <summary>	Foreground Color - Bue (0.0 to 1.0) </summary>
		FOREGROUND_COLOR_BLUE  = 2,
		/// <summary>	Foreground Color - Alpha (0.0 to 1.0) </summary>
		FOREGROUND_COLOR_ALPHA = 3,
		/// <summary>	Background Color - Red (0.0 to 1.0) </summary>
		BACKGROUND_COLOR_RED   = 4,
		/// <summary>	Background Color - Green (0.0 to 1.0) </summary>
		BACKGROUND_COLOR_GREEN = 5,
		/// <summary>	Background Color - Blue (0.0 to 1.0) </summary>
		BACKGROUND_COLOR_BLUE  = 6,
		/// <summary>	Background Color - Alpha (0.0 to 1.0) </summary>
		BACKGROUND_COLOR_ALPHA = 7
	};
};

}
}