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
	/// <summary>	Configuration of analysis results for individual features </summary>
	///
	/// <remarks>	When enabled, representation will provide feedback on individual features / objects
	///				in the analysis.  Basic information will always be provided, but information on 
	///				each feature allowed by the IGW3DContentsFilter will only be provided as long as 
	///				the IGW3DFeatureFeedbackConfiguration is enabled. </remarks>
	///	
	///	<see cref="IGW3DFrustumAnalysis"/>
	///	<see cref="IGW3DVectorRepresentationDriverCollection::Stream"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DFeatureFeedbackConfiguration
	{
		virtual ~IGW3DFeatureFeedbackConfiguration() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the contents filter that controls what features are reported back. </summary>
		///
		/// <remarks>	You will always get basic information back on representations, but if this 
		///				feature feedback configuration is enabled, details about all features that qualify 
		///				against the constraints of the contents filter will be reported. If the 
		///				IGW3DContentsFilter is disabled, it will not filter out any features. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DContentsFilter* get_ContentsFilter() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the contents filter that controls what features are reported back. </summary>
		///
		/// <remarks>	You will always get basic information back on representations, but if this 
		///				feature feedback configuration is enabled, details about all features that qualify 
		///				against the constraints of the contents filter will be reported. If the 
		///				IGW3DContentsFilter is disabled, it will not filter out any features. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DContentsFilter* get_ContentsFilter() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query whether the reporting of feature-specific details is enabled. </summary>
		///
		/// <remarks>	When this is enabled, you will get information about what features were found, 
		///				within the constraints of your IGW3DContentsFilter configuration.
		///
		///				This is disabled by default. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Enabled() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enable/disable the reporting of feature-specific details. </summary>
		///
		/// <remarks>	When this is enabled, you will get information about what features were found, 
		///				within the constraints of your IGW3DContentsFilter configuration.
		///
		///				This is disabled by default. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_Enabled(bool enable) = 0;

		//SKIP-CODE-BEGIN

		enum FIELD_OF_VIEW_MODE
		{
			CAMERA_DEFAULT,		//use the fov of the camera
			CAMERA_RELATIVE,	//extend the fov of the camera.
			FIELD_OF_VIEW_MODE_MAX = 0xFFFF
		};

		//if the camera fov is 45 degrees, and if you set this to 10, this analysis's  report will 
		//contain information containing a 55 degree field of view. One use case is if you want an 
		//imaginary 2D screen coordinate where a point feature would be rendered if the screen was 
		//large enough. (imaginary as its offscreen) In the results, you can query if the feature 
		//ID was within this FOV as well as if within the camera's field of view.  This is useful
		//if you want to know if a feature is just outside the fov of the camera, but within this 
		//fov. default is CAMERA_DEFAULT
		//virtual void put_FieldOfViewMode( FIELD_OF_VIEW_MODE mode ) = 0;
		//virtual void put_FieldOfViewValue( float value ) = 0;

		//virtual FIELD_OF_VIEW_MODE get_FieldOfViewMode( ) const = 0;
		//virtual float  get_FieldOfViewValue( ) const = 0;

//SKIP-CODE-END

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the configuration of the 2D pixel location of features. </summary>
		///
		/// <remarks>	This is where you configure whether 2D pixel locations of each feature loaded in 
		///				in the 3D scene should be reported in the IGW3DFeatureID2DPixelLocationResult.
		///
		///				Note that if you set put_FieldOfViewvalue to a positive value, this location can 
		///				be imaginary and have coordinates outside your viewport/window.
		///
		///				ONLY supported in representations: model, custom, all shapes, range ring. </remarks>
		///
		///	<see cref="IGW3DFeatureID2DPixelLocationResult"/>
		///	<see cref="IGW3DFeatureFeedbackResult::get_FeatureID2DPixelLocationResult"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DFeatureID2DPixelLocationConfiguration* get_FeatureID2DPixelLocationConfiguration() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the configuration of the 2D pixel location of features. </summary>
		///
		/// <remarks>	This is where you configure whether 2D pixel locations of each feature loaded in 
		///				in the 3D scene should be reported in the IGW3DFeatureID2DPixelLocationResult.
		///
		///				Note that if you set put_FieldOfViewvalue to a positive value, this location can 
		///				be imaginary and have coordinates outside your viewport/window.
		///
		///				ONLY supported in representations: model, custom, all shapes, range ring. </remarks>
		///
		///	<see cref="IGW3DFeatureID2DPixelLocationResult"/>
		///	<see cref="IGW3DFeatureFeedbackResult::get_FeatureID2DPixelLocationResult"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DFeatureID2DPixelLocationConfiguration* get_FeatureID2DPixelLocationConfiguration() const = 0;
	};
}