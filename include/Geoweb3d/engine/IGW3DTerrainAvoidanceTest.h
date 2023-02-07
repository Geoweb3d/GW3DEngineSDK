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
#include "IGW3DAnalysis.h"

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A Terrain Avoidance analysis. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  IGW3DTerrainAvoidanceTest : public IGW3DAnalysis
	{
		virtual ~IGW3DTerrainAvoidanceTest() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum ReferenceAltitudeMode
		{
			//Will only use the reference altitude value
			Value = 0,
			//Will only use the attached camera altitude
			Camera = 1,
			//Will use the attached camera altitude plus the reference altitude
			//so basically the use case is an offset from the camera.
			CameraAndValue = 2
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum IncreasedDetailMode
		{
			//Uses the default terrain settings of that are set the on camera viewing the analytic. 
			NoExtraDetail = 0,
			//Will apply the TerrainLOD value to pull in more terrain nodes
			UseTerrainLODValue = 1,
			//Will apply the TerrainLOD value to pull in more terrain nodes and sacrifice frame rate
			//to pull all such nodes into the scene faster. 
			UseTerrainLODValueAndFastLoad = 2,
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the reference altitude for the Terrain Avoidance calculation. </summary>
		///
		/// <param name="altitude"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_ReferenceAltitude( double altitude) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the reference altitude for the Terrain Avoidance calculation. </summary>
		///
		/// <returns>	The reference altitude in meters. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_ReferenceAltitude() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the reference altitude mode for the Terrain Avoidance calculation. </summary>
		///
		/// <param name="ref_alt_mode"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_ReferenceAltitudeMode(ReferenceAltitudeMode ref_alt_mode) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the reference altitude mode for the Terrain Avoidance calculation. </summary>
		///
		/// <returns>	The reference altitude mode. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual ReferenceAltitudeMode get_ReferenceAltitudeMode() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>Put the color that will display when no elevation data is found.</summary>
		///
		/// <param name="red"> TODO </param>
		/// <param name="green"> TODO </param>
		/// <param name="blue"> TODO </param>
		/// <param name="alpha"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_NoDataColor(const float red, const float green, const float blue, const float alpha) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>Get the current no data color that is set. </summary>
		///
		/// <param name="red"> TODO </param>
		/// <param name="green"> TODO </param>
		/// <param name="blue"> TODO </param>
		/// <param name="alpha"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_NoDataColor(float& red, float& green, float& blue, float& alpha) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assign a color lookup table finalization token. </summary>
		///
		/// <remarks>	This function assigns a color lookup table finalization token as the color lookup 
		///				table that is utilized by this IGW3DTerrainAvoidanceTest. A color lookup finalization
		///				token is a finalized configuration of an IGW3DColorLookupTablePtr, which is created 
		///				using the IGW3DSceneGraphContext interface. Needs to be assigned in order to visualize
		///				the test. </remarks>
		///
		/// <param name="color_lut">	The color lookup table. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		/// 
		/// <see cref="IGW3DSceneGraphContext, IGW3DColorLookupTablePtr"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_GW3DFinalizationToken(IGW3DColorLookupTableFinalizationTokenPtr color_lut) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Values that represent data considered in the ROI. </summary>
		///
		/// <remarks>	Options include terrain only and terrain plus all other geometry.  </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum SceneFilterType
		{
			/// <summary>	terrain only. </summary>
			TERRAIN_ONLY = 0,
			/// <summary>	 terrain and any scene geometry. </summary>
			TERRAIN_AND_ALL_OTHER_GEOMETRY = 1,
			/// <summary>	Do Not Use. </summary>
			ViewshedFilterType = 0xFFFF
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set the filter. </summary>
		///
		/// <remarks>	Sets the filter that determines what gets blocked via the ROI. </remarks>
		///
		/// <param name="type">	The filter type. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_SceneFilterType(SceneFilterType type) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the filter type. </summary>
		///
		/// <returns>	The query filter type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual SceneFilterType get_SceneFilterType() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enables/disables the Analytic. </summary>
		///
		/// <remarks>	While disabled, the Analytic will not be visible and extra data within the camera will
		/// 			not be paged in. </remarks>
		///
		/// <param name="enable">	true to enable, false to disable. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_Enabled(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the increased detail mode of the Analytic. </summary>
		///
		/// <remarks>	Default value is UseTerrainLODValue.	</remarks>
		/// 
		/// <returns>	The current increased detail mode. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IncreasedDetailMode get_IncreasedDetailMode() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enables/disables the increased detail mode of the Analytic. </summary>
		///
		/// <remarks>	Default value is UseTerrainLODValue. This will enable a mode that will pull in extra detail within 
		/// the camera frustum, with also the option to load faster.  </remarks>
		///
		/// <param name="detail_mode">	The increased detail mode. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_IncreasedDetailMode(IncreasedDetailMode detail_mode) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the enabled state of the ROI. </summary>
		///
		/// <returns>	true if enabled, false if disabled. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Enabled() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set the terrain linear LOD. </summary>
		///
		/// <remarks>	Default value is 8.0.  Higher values increase the terrain detail at greater
		///				distances. Make sure IncreasedDetailMode is set appropriately for this to 
		///				take effect. </remarks>
		///
		/// <param name="val">	The LOD. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_TerrainLinearLOD(double val) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the terrain linear LOD. </summary>
		///
		/// <remarks>	Default value is 8.0.  Higher values increase the terrain detail at greater
		///				distances. Make sure IncreasedDetailMode is set appropriately for this to 
		///				take effect. </remarks>
		/// 
		/// <returns>	The terrain linear LOD. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_TerrainLinearLOD() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The collection defining all participating vector representations. </summary>
		///
		/// <remarks>	A collection defining all vector representations that participate in the analysis.
		/// 			All of the member representations will be tested for intersection with the line
		/// 			segment.  If representations are added to or removed from the collection, this
		/// 			change will be reflected in the results after the next draw call.</remarks>
		///
		/// <param name="reset_next_iterator"> TODO </param>
		///
		/// <returns>	The vector layer representation collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DVectorRepresentationHelperCollection* get_VectorRepresentationCollection(bool reset_next_iterator = false) = 0;

	};
}