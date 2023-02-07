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
	namespace Raster
	{
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 /// <summary>	An elevation parameters. </summary>
	 ///
	 ////////////////////////////////////////////////////////////////////////////////////////////////////

	 struct GW3D_DLL ElevationParameters
	{
		 virtual  ~ElevationParameters() {}
		/* Comparison function for invalid elevation values (INVALID_VALUE) */
		enum OpType
		{
			DEM_EQ = 0, /* if( point == invalid_elevation_value ) discard; */
			DEM_LEQ	=1	/* if( point <= invalid_elevation_value ) discard; */
		};
	
		enum IndividualParameters
		{
			INVALID_OVERRIDE   = 0,
			INVALID_VALUE      = 1,
			INVALID_COMPARITOR = 2,
			VERTICAL_SCALE	   = 3,
			ENOUSE_MAX= 0xff
		};
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	An imagery parameters. </summary>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum SamplerAlg
	{
		NEAREST_NEIGHBOR = 0,
		BILINEAR = 1,
		CUBIC = 2,
		CUBIC_SPLINE = 3,
		LANCOS = 4,
		AVERAGE = 5,
		MODE = 6,
		GAUSS = 7,
		END_OF_SAMPLER_ALG_MODES = 0xff
	};

	namespace ImageryProperties
	{
		enum ImageryProperties
		{
			/// <summary>	If the image supplies invalid values, pay attention to those. </summary>
			USE_IMAGE_INVALID_COLOR = 0,
			/// <summary>	Use the invalid red, blue, green channels on top of image invalid color. </summary>
			USER_DEFINED_INVALID_COLOR = 1,
			/// <summary>	Amount from invalid_red/green/blue a color can vary and will still be considered invalid (range 0 - 1). </summary>
			TOLERANCE_RANGE = 2,
			/// <summary>	Normalized invalid red value (range 0 - 1). </summary>
			INVALID_RED = 3,
			/// <summary>	Normalized invalid green value (range 0 - 1). </summary>
			INVALID_BLUE = 4,
			/// <summary>	Normalized invalid blue value (range 0 - 1). </summary>
			INVALID_GREEN = 5,
			/// <summary> Raster resolution mode.	false - fixed size texture for each node in the terrain tree.
			///										true - node texture resolution based on screen space error. </summary>
			SCREEN_RES_MODE = 6,
			/// <summary>	resolution mapping takes effect only if the SCREEN_RES_MODE is set to true. 
			///				The value is a metric to determine a ratio between screen resolutions to native raster resolutions.
			///				Meaningful values may be [1 to 64]
			///				This value attempts to represents a worst case up-sampling from raster resolution to 
			///				screen resolution: [1 : auto_resolution_] pixel mapping.
			///				default is 4.				(range 1 - 64). </summary>
			RESOLUTION_MAPPING = 7,
			/// <summary>	Use the invalid replacement red, blue, green, alpha channels for when use_image_invalid_color_ and user_defined_invalid_color_ are accessed. </summary>
			USE_INVALID_REPLACEMENT_COLOR = 8,
			/// <summary>	Normalized invalid red value (range 0 - 1). </summary>
			INVALID_REPLACEMENT_RED = 9,
			/// <summary>	Normalized invalid blue value (range 0 - 1). </summary>
			INVALID_REPLACEMENT_BLUE = 10,
			/// <summary>	Normalized invalid green value (range 0 - 1). </summary>
			INVALID_REPLACEMENT_GREEN = 11,
			/// <summary>	Normalized invalid alpha value (range 0 - 1). </summary>
			INVALID_REPLACEMENT_ALPHA = 12,
			/// <summary> Sampling Algorithm.  </summary>
			SAMPLER_ALGO = 13,
			/// <summary>	MAX - Do Not Use </summary>
			IMAGERY_PROP_MAX = 0xFFFF

		};
	}

	
	}
}
