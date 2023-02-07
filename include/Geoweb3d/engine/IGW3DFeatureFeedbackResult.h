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

namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///	<summary>	A feature ID 2D pixel location result. </summary>
	///
	///	<remarks>	Provides information about the 2D window coordinate locations of features
	///				reported in the IGW3DFeatureFeedbackResult. </remarks>
	///
	/// <see cref="IGW3DFeatureID2DPixelLocationConfiguration"/>
	///	<see cref="IGW3DFeatureFeedbackResult::get_FeatureID2DPixelLocationResult"/>
	//////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DFeatureID2DPixelLocationResult
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		///	<summary>	Query whether the IGW3DFeatureID2DPixelLocationConfiguration was enabled. </summary>
		///
		///	<remarks>	If this returns false, the IGW3DFeatureID2DPixelLocationConfiguration was not set 
		///				to true, and therefore this IGW3DFeatureID2DPixelLocationResult will not contain 
		///				valid results. </remarks>
		///
		/// <see cref="IGW3DFeatureID2DPixelLocationConfiguration"/>
		///	<see cref="IGW3DFeatureFeedbackConfiguration::get_FeatureID2DPixelLocationConfiguration"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_WasEnabled() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the window coordinate of the feature. </summary>
		///
		/// <param name="pixel_x">	[out] The pixel x coordinate. </param>
		/// <param name="pixel_y">	[out] The pixel y coordinate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_WindowCoordinate(int& pixel_x, int& pixel_y) const = 0; //coordinate on the window to read back the Geodetic information

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the normalized coordinate of the feature. </summary>
		///
		/// <param name="nx">	[out] The normalized x coordinate. </param>
		/// <param name="ny">	[out] The normalized y coordinate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_NormalizedCoordinate(float& nx, float& ny) const = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	The feature feedback result. </summary>
	///
	/// <remarks>	The feature feedback results of a IGW3DFrustumAnalysisStream, which reports details
	///				for individual features in a representation that appear within the camera frustum.
	///				</remarks>
	///
	/// <see cref="IGW3DFrustumAnalysisStream"/>
	/// <see cref="IGW3DVectorRepresentationDriverCollection::Stream"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DFeatureFeedbackResult
	{
		virtual ~IGW3DFeatureFeedbackResult() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The object id/feature id of the feature. </summary>
		///
		/// <returns>	The object identifier. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual long get_ObjectID() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query whether this object was rendered in the 3D scene. </summary>
		///
		/// <returns>	True if object was occluded by globe, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Rendered() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query whether this object was occluded by the globe.	</summary>
		///
		/// <returns>	This returns true only when the globe itself occludes the feature. It does not 
		///				take terrain data into consideration. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_OccludedByGlobe() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The distance between this feature and the camera for which this is being reported. </summary>
		///
		/// <returns>	The distance from camera (in meters). </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_DistanceFromCamera() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		///	<summary>	Gets the approximate number of pixels the feature takes up on the screen </summary>
		///
		/// <remarks>	Gets the approximate number of pixels that this feature takes up on the screen for 
		///				the representation returned by get_VectorRepresentation. </remarks>
		///
		/// <returns>	The number of pixels. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual float get_ApproximateNumPixelsOnScreen() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the 2D pixel location result for this feature. </summary>
		///
		/// <remarks>	Gets the 2D pixel location result for this feature, relative to the window and
		///				and camera.
		///
		///				This result is only valid if IGW3DFeatureID2DPixelLocationConfiguration::put_Enabled 
		///				was set to true prior to the rendering of the last frame. </remarks>
		///
		///	<see cref="IGW3DFeatureFeedbackConfiguration"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DFeatureID2DPixelLocationResult* get_FeatureID2DPixelLocationResult() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the 2D pixel location result for this feature. </summary>
		///
		/// <remarks>	Gets the 2D pixel location result for this feature, relative to the window and
		///				and camera.
		///
		///				This result is only valid if IGW3DFeatureID2DPixelLocationConfiguration::put_Enabled 
		///				was set to true prior to the rendering of the last frame. </remarks>
		///
		///	<see cref="IGW3DFeatureFeedbackConfiguration"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DFeatureID2DPixelLocationResult* get_FeatureID2DPixelLocationResult() const = 0;
	};
}