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
#include "core/GW3DInterFace.h"

/* Primary namespace */
namespace Geoweb3d
{
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Clamp radius. </summary>
	///
	/// <remarks>	The clamp radius controls the minimum and maximum visibility of features in a layer.
	/// 			It provides control over the minimum and maximum distance at which features will be
	/// 			visible. Note that this can be used to create LODs, by setting different 
	/// 			representations of the same layer to have mutually exclusive visibility distance 
	/// 			ranges (for example, displaying icons at a distance and 3D models up close).</remarks>
	/// 
	/// <see cref="IGW3DGUIVectorRepresentation::get_ClampRadiusFilter"/>
	/// <see cref="IGW3DGUIVectorRepresentationBasic::get_ClampRadiusFilter"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIClampRadius
	{   

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set the enabled state of visibility clamping. </summary>
		///
		/// <remarks>	When set to false, visibility is not clamped based on distance, and the values
		/// 			set via put_MinRadius and put_MaxRadius are ignored.  When set to true, visibility 
		/// 			is clamped per feature based on distance from the eye.
		/// 			
		/// 			Note that some representations do not support visibility clamping, such as polygons.
		///				Also note that visibility can be limited by other factors, including paging level
		///				and screenspace culling.</remarks>
		///
		/// <param name="enable">	true to enable visibility clamping, false to disable it. </param>
		/// 
		/// <see cref="IGW3DGUIVectorRepresentation::get_PagingLevel"/>
		/// <see cref="IGW3DGUIVectorRepresentationBasic::get_PagingLevel/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void  put_Enabled( bool enable )= 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Check if visibility clamping is enabled. </summary>
		///
		/// <remarks>	If visibility clamping is enabled, the minimum and maximum visibility distance 
		/// 			may be controlled via the put_MinRadius and put_MaxRadius functions.
		/// 			
		/// 			Note that some representations do not support visibility clamping, such as polygons.
		///				Also note that visibility can be limited by other factors, including paging level 
		///				and screnspace culling. </remarks>
		///
		/// <returns>	true if visibility clamping is enabled, false if it is disabled. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool  get_Enabled( ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set the minimum visible radius. </summary>
		///
		/// <remarks>	Set the minimum distance from the eye that features will be visible.
		/// 			As long as visibility clamping is enabled, any features that are less than
		/// 			this distance away from the eye will not be visible.
		/// 			
		/// 			Note that for a viewshed analysis, features of a layer will always be visible,
		/// 			regardless of this setting.  This is because the distance from the user's Eye
		/// 			is arbitrary to the viewshed from the origin point. </remarks>
		///
		/// <param name="radius">	Inner radius of the visible area. 
		/// 						Set to zero to see all features within the max radius. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void  put_MinRadius( float radius ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set the maximum visible radius. </summary>
		///
		/// <remarks>	Set the maximum distance from the eye that features will be visible.
		/// 			As long as visibility clamping is enabled, any features that are more than
		/// 			this distance away from the eye will not be visible.
		/// 			
		/// 			Note that for a viewshed analysis, features of a layer will always be visible,
		/// 			regardless of this setting.  This is because the distance from the user's Eye
		/// 			is arbitrary to the viewshed from the origin point. </remarks>
		///
		/// <param name="radius">	Outer radius of the visible area. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void  put_MaxRadius( float radius ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the minimum visibility for features in the representation. </summary>
		///
		/// <remarks>	This setting is ignored if visibility clamping is not enabled. </remarks>
		///
		/// <returns>	Minimum distance from the eye that features will be visible. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual float get_MinRadius( ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the maximum visibility for features in the representation. </summary>
		///
		/// <remarks>	This setting is ignored if visibility clamping is not enabled. </remarks>
		///
		/// <returns>	Maximum distance from the eye that features will be visible. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual float get_MaxRadius( ) const = 0;
	};

}
}