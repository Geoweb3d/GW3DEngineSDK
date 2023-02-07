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
#include "IGW3DRegionOfInterest.h"
#include "IGW3DViewshedFrustum.h"
#include "IGW3DAnalysis.h"

/* Primary namespace */
namespace Geoweb3d
{

	class GW3DPolygon;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Stream of a viewshed's contents. </summary>
	///
	/// <remarks>	This interface is inherited by the SDK user, and receives all visible polygons 
	/// 			within the viewshed frustum when passed into IGW3DViewshedTest::stream_Results. </remarks>
	/// 
	/// <see cref="IGW3DViewshedTest::stream_Results"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DViewshedContentsStream
	{
		virtual ~IGW3DViewshedContentsStream() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the stream action. </summary>
		///
		/// <remarks>	This function gets called for each visible polygon surface in the viewshed 
		/// 			frustum. </remarks>
		///
		/// <param name="polygon">	the polygon. </param>
		///
		/// <returns>	true to continue streaming the next polygon, false to abort stream. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnStream(GW3DPolygon* polygon) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Callback reporting that an error occurred. </summary>
		///
		/// <remarks>	TODO. </remarks>
		///
		/// <param name="viewshed"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnError(IGW3DViewshedTest* viewshed) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A viewshed analysis. </summary>
	///
	/// <remarks>	The Geoweb3d viewshed provides a visual display of surfaces that are visible and/or
	/// 			non-visible from a defined position, orientation and field of view. The display of
	/// 			viewshed is divided into two regions: the visible and the hidden region.  The
	/// 			visible region represents all terrain and geometry surfaces that are visible from
	/// 			the viewshed origin, whereas the hidden region represents all terrain and
	/// 			geometry surfaces that are not visible from the viewshed origin. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  IGW3DViewshedTest
		: public IGW3DRegionOfInterest
#ifndef SWIGGING
		, public IGW3DAnalysis
#endif
	{
		virtual ~IGW3DViewshedTest() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets whether an outline of the frustum should be drawn. </summary>
		///
		/// <remarks>	When enabled, an outline of the frustum will be drawn, helping to identify its
		/// 			extents in 3D space. </remarks>
		///
		/// <param name="enable">	true to enable, false to disable. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_DrawFrustum(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if the frustum outline is being drawn. </summary>
		///
		/// <returns>	true if it is being drawn, false if it is not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_DrawFrustum() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets whether to draw the visible region of the viewshed. </summary>
		///
		/// <remarks>	When enabled, the visible region of the viewshed will be displayed on top of all
		/// 			surfaces within the frustum that are visible from the viewshed origin. </remarks>
		///
		/// <param name="enable">	true to enable, false to disable. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_DrawVisibleRegion(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if visible region is being drawn. </summary>
		///
		/// <returns>	true if it is being drawn, false if it is not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_DrawVisibleRegion() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets whether to draw the hidden region of the viewshed. </summary>
		///
		/// <remarks>	When enabled, the hidden region of the viewshed will be displayed on top of all
		/// 			surfaces within the frustum that are not visible from the viewshed origin.
		/// 			</remarks>
		///
		/// <param name="enable">	true to enable, false to disable. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_DrawHiddenRegion(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if hidden region is being drawn. </summary>
		///
		/// <returns>	true if it is being drawn, false if it is not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_DrawHiddenRegion() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Shade the visible region according to the normal to the viewshed origin.</summary>
		///
		/// <remarks>	Shade the visible region of the viewshed according to the normal of the surface
		/// 			relative to the camera origin.  For example, surfaces facing the origin will be
		/// 			colored differently than surfaces at an angle to the origin. This will override the
		/// 			color set for the visible region.  </remarks>
		///
		/// <param name="enable">	true to shade by normals, false to display solid color. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_VisibleNormals(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if visible surfaces are shaded according to the normal to the origin. </summary>
		///
		/// <returns>	true if normals are displayed, false if they are not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_VisibleNormals() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Shade the hidden region according to the normal to the camera origin.</summary>
		///
		/// <remarks>	Shade the hidden region of the viewshed according to the normal of the surface
		/// 			relative to the viewshed origin.  For example, surfaces facing directly away from the
		/// 			origin will be colored differently than surfaces at an angle to the origin. This
		/// 			will override the color set for the visible region.  </remarks>
		///
		/// <param name="enable">	true to shade by normals, false to display solid color. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_HiddenNormals(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if hidden surfaces are shaded according to the normal to the origin. </summary>
		///
		/// <returns>	true if normals are displayed, false if they are not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_HiddenNormals() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the color of the visible region surfaces. </summary>
		///
		/// <remarks>	Sets the color of the visible surfaces. All surfaces that are visible from the
		/// 			viewshed origin will be overlaid with this color. </remarks>
		///
		/// <param name="r">	The red component of the color (0.0 to 1.0). </param>
		/// <param name="g">	The green component of the color (0.0 to 1.0). </param>
		/// <param name="b">	The blue component of the color (0.0 to 1.0). </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_VisibleGridColor(float r, float g, float b) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the color of the visible region. </summary>
		///
		/// <param name="r">	The red component of the color (0.0 to 1.0). </param>
		/// <param name="g">	The green component of the color (0.0 to 1.0). </param>
		/// <param name="b">	The blue component of the color (0.0 to 1.0). </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_VisibleGridColor(float& r, float& g, float& b) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the blend factor for the visible region color. </summary>
		///
		/// <remarks>	Sets the degree to which the visible region's color should be blended onto terrain
		/// 			and geometry surfaces.  The higher the number the more the more dominant the color
		/// 			will be versus the native color/texture of the surface.  At 0.0 the color will be
		/// 			completely transparent, and at 1.0 the color will completely opaque, such that the
		/// 			native color/texture of the surfaces will not be visible. </remarks>
		///
		/// <param name="val">	The blend factor (0.0 to 1.0). </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_VisibleGridBlendFactor(float val) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets visible region blend factor. </summary>
		///
		/// <remarks>	Gets the degree to which the visible region's color is blended onto terrain and
		/// 			geometry surfaces. </remarks>
		///
		/// <returns>	The blend factor (0.0 to 1.0). </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual float get_VisibleGridBlendFactor() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the color of the hidden region surfaces. </summary>
		///
		/// <remarks>	Sets the color of the hidden surfaces. All surfaces that are not visible from the
		/// 			viewshed origin will be overlaid with this color. </remarks>
		///
		/// <param name="r">	The red component of the color (0.0 to 1.0). </param>
		/// <param name="g">	The green component of the color (0.0 to 1.0). </param>
		/// <param name="b">	The blue component of the color (0.0 to 1.0). </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_HiddenGridColor(float r, float g, float b) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the color of the hidden region. </summary>
		///
		/// <param name="r">	The red component of the color (0.0 to 1.0). </param>
		/// <param name="g">	The green component of the color (0.0 to 1.0). </param>
		/// <param name="b">	The blue component of the color (0.0 to 1.0). </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_HiddenGridColor(float& r, float& g, float& b) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the blend factor for the hidden region color. </summary>
		///
		/// <remarks>	Sets the degree to which the hidden region's color should be blended onto terrain
		/// 			and geometry surfaces.  The higher the number the more the more dominant the color
		/// 			will be versus the native color/texture of the surface.  At 0.0 the color will be
		/// 			completely transparent, and at 1.0 the color will completely opaque, such that the
		/// 			native color/texture of the surfaces will not be visible. </remarks>
		///
		/// <param name="val">	The blend factor (0.0 to 1.0). </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_HiddenGridBlendFactor(float val) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets hidden region blend factor. </summary>
		///
		/// <remarks>	Gets the degree to which the hidden region's color is blended onto terrain and
		/// 			geometry surfaces. </remarks>
		///
		/// <returns>	The blend factor (0.0 to 1.0). </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual float get_HiddenGridBlendFactor() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the grid line width. </summary>
		///
		/// <remarks>	The drawn regions of the viewshed are overlaid with a regular grid to help show the
		/// 			distribution of the viewshed overlay.  This setting controls the width of the lines
		/// 			that compose the grid. </remarks>
		///
		/// <param name="val">	The line width. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_GridLineWidth(float val) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the grid line width. </summary>
		///
		/// <remarks>	Gets the width of the lines that compose the grid displayed on drawn regions of the
		/// 			viewshed. </remarks>
		///
		/// <returns>	The line width. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual float get_GridLineWidth()const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the grid size. </summary>
		///
		/// <remarks>	Sets the size of the grid, where the higher the number the smaller the grid cells
		/// 			and therefore the denser the grid. </remarks>
		///
		/// <param name="val">	The size. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_GridSize(float val) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets  the grid size. </summary>
		///
		/// <returns>	The size. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual float get_GridSize()const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Stream the viewshed results. </summary>
		///
		/// <remarks>	Stream polygons of all the visible surfaces within the viewshed. </remarks>
		///
		/// <param name="streamobj">	A stream object (implemented by the SDK user). </param>
		///
		/// <returns>	GW3D_sOk is succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult stream_Results(IGW3DViewshedContentsStream* streamobj) = 0;

		//SKIP-CODE-BEGIN

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the viewshed report collection. </summary>
		///
		/// <remarks>	Details of the visible surfaces within the viewshed frustum.
		/// 			
		/// 			This API is not yet supported. </remarks>
		///
		/// <param name="reset_next_iterator">	(optional) reset the next iterator. </param>
		///
		/// <returns>	the viewshed report collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		//virtual IGW3DViewshedReportCollection * get_ViewshedReportCollection( bool reset_next_iterator = false ) = 0;

		//SKIP-CODE-END

#ifdef SWIGGING

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a camera to the analysis (like viewshed). </summary>
		///
		/// <remarks>	Adds a camera to the analysis, causing visual results to be rendered on this
		///				camera.  No results  will be visible in the 3D scene unless a camera is added.
		///				TODO: this will probably become a collection so that one can query which camera
		///				as were added. </remarks>
		///
		/// <param name="p">	The camera to add. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_AddCamera(IGW3DCameraWPtr camera) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Removes a camera from the viewshed. </summary>
		///
		/// <remarks>	Removes a camera from the viewshed, causing it to no longer be rendered for that
		/// 			camera. </remarks>
		///
		/// <param name="p">	The camera to remove. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_RemoveCamera(IGW3DCameraWPtr camera) = 0;
#endif

	};
}