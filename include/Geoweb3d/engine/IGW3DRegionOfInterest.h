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
#include "IGW3DPositionOrientation.h"
//#include "IGW3DViewshedFrustum.h"
//#include "IGW3DAnalysis.h"
#include "../../Geoweb3dCore/GW3DResults.h"

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A Region of Interest. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  IGW3DRegionOfInterest : public IGW3DPositionOrientation
	{
		virtual ~IGW3DRegionOfInterest() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Values that represent a viewshed's LOD policy. </summary>
		///
		/// <remarks>	The LOD policy defines how terrain will be paged in within the viewshed
		/// 			frustum with will then following the paging rules defined for the vector and raster representations.
		///				Note ViewOnly modes will generally be used for more static based viewshed, that do not need to
		///				anticipate highly accurate data being behind them. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum LODPolicy
		{
			/// <summary>	The none option. </summary>
			/// <remarks>	Does not pull in any extra LOD. 
			///				The ROI for this mode is none. 
			/// </remarks>
			None = 0,

			/// <summary>	The normal option. </summary>
			/// <remarks>	Pulls in data as a normal camera would. This is logarithmically 
			///				based and concentrically from the viewshed location.
			///	</remarks>
			Normal = 1,

			/// <summary>	The maximum LOD option. </summary>
			/// <remarks>	Pulls in data as a normal camera would. This is logarithmically 
			///				based and concentrically from the viewshed location and also
			///				pulls in data at the maximum LOD level within ROI (A view for example).
			///				FullIn will use significantly more CPU and GPU 
			///				memory but will result in a more accurate data.
			/// </remarks>
			FullIn = 2,

			/// <summary>	The normal option for the ROI only (The ROI being within the view itself).</summary>
			/// <remarks>	Pulls in terrain data as in normal mode but only within the view boundaries.
			/// </remarks>
			Normal_ViewOnly = 3,

			/// <summary>	The maximum LOD option for within the ROI only. (The ROI being within the view itself).</summary>
			/// <remarks>	Pulls in data at the maximum LOD level to a radial distance from the viewshed location
			///				that will encompass the entire viewshed AND only within the view boundaries.
			///	</remarks>
			FullIn_ViewOnly = 4,

			/// <summary>	This mode follows the same methodology as the FullIn mode but with the addition quality clamp algorithm.
			///</summary>	Quality clamping is explained more in the put_QualityTargetValues API, but essentially it clamps the
			///				full in mode from going all the way down to the maximum LOD based on a defined resource limit.
			/// <remarks>	
			///	</remarks>
			FullIn_QualityClamp = 5,

			/// <summary>	This mode follows the same methodology as the FullIn_ViewOnly mode but with the addition quality clamp algorithm.
			///</summary>	Quality clamping is explained more in the put_QualityTargetValues API, but essentially it clamps the
			///				full in mode from going all the way down to the maximum LOD based on a defined resource limit.
			/// <remarks>	
			///	</remarks>
			FullIn_ViewOnly_QualityClamp = 6,


			/// <summary>	Do Not Use. </summary>
			MAX_LOD_POLICY = 0xFFFF
		};

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
		/// <summary>	Enables/disables the ROI. </summary>
		///
		/// <remarks>	While disabled, the ROI will not be visible and data within the frustum will
		/// 			not be paged in. </remarks>
		///
		/// <param name="enable">	true to enable, false to disable. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_Enabled(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the enabled state of the ROI. </summary>
		///
		/// <returns>	true if enabled, false if disabled. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Enabled() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the terrain LOD policy. </summary>
		///
		/// <remarks>	Sets the policy by which terrain inside the ROI frustum is paged in. </remarks>
		///
		/// <param name="LODPolicy"> the policy. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_LODPolicy(LODPolicy) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the terrain LOD policy. Default is 'Normal'.</summary>
		///
		/// <returns>	The policy. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual LODPolicy get_LODPolicy()const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the frame time stamp of the last frame the ROI was last analyzed. </summary>
		///
		/// <returns>	The frame time stamp. </returns>
		///
		/// <see cref="Geoweb3d::SDK_Statistics::GetMasterFrameCount"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned get_FrameTimeStamp() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query validity of the ROI test. </summary>
		///
		/// <remarks>	If the ROI test results are invalid, this will return false. </remarks>
		///
		/// <returns>	true if valid, false if invalid. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_IsValid() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the target resolution values. </summary>
		///
		/// <remarks>	This is a setting to fine tune usability of a viewshed when using a Full In LODPolicy.  It gives the user the ability
		///				to trade performance for accuracy. By setting a desired resolution at a the target view range we will
		///				compute a limit metric to determine the resources that a user is willing to allot to a viewshed. </remarks>
		///
		/// <param name="distance">		The target distance. (meters > 0.0) </param>
		/// <param name="resolution">	The target resolution. (meters > 0.0)</param>
		///
		/// <returns>	GW3D_sOk means the values were within appropriate ranges.. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_QualityTargetValues(double distance, double resolution) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the target resolution values. </summary>
		///
		/// <remarks>	Accessor to receive the target resolution values. 
		///				Default values are for a 30 miles range at a 200m posting. </remarks>
		///
		/// <param name="distance">		The target distance.(meters)</param>
		/// <param name="resolution">	The target resolution.(meters)</param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_QualityTargetValues(double& distance, double& resolution) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the minimum resolution at a radial distance for a specified quality percentage.  </summary>
		///
		/// <remarks>	This API works in conjunction with the put_QualityTargetValues API.  Default is
		///				for a 30 mile extent at a desired resolution of 200m posting. 
		///				This allows a user to check the the resolutions at different quality percentages. </remarks>
		///
		/// <param name="quality_percentage">	The quality percentage. </param>
		/// <param name="min_resolution"> The minimus posting resolution attained under this criteria. </param>
		/// <param name="up_to_distance">	The target distance. (meters) </param>
		///
		/// <returns>	The minimum resolution a viewshed can expect.(meters) </returns>
		/// <returns>	The minimum resolution a viewshed can expect to this maximum extent.(meters) </returns>
		/// <returns>	GW3D_sOk means the values were within appropriate ranges.. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult get_MinResolution(double distance, double quality_percentage, double& min_resolution, double& up_to_distance) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the quality percentage. </summary>
		///
		/// <remarks>	This API works in conjunction with the put_QualityTargetValues API.  
		///				Default is for a 30 mile extent at a desired resolution of 200m posting. 
		///				The quality percentage is applied to the limit metric that was computed by the
		///				put_QualityTargetValues.
		///				At some point, a high enough percentage will give the same results as a Full In mode
		///				viewshed. </remarks>
		///
		/// <param name="quality_percentage">	The qualitative percentage. Default is 100.0 (> 0.0)</param>
		///
		/// <returns>	GW3D_sOk means the values we're within appropriate ranges.. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_QualityPercentage(double quality_percentage) = 0;

	};
}