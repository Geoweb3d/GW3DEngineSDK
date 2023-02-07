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
#include <Geoweb3d/core/GW3DInterFace.h>
#include <Geoweb3d/engine/IGW3DGeoFenceIntruderCollection.h>
#include <Geoweb3dCore/GeometryExports.h>

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum GEOFENCE_INTRUDER_CALLBACK_TYPE : unsigned
	{
		GEOFENCE_INTRUDER_GRID = 0,
		GEOFENCE_INTRUDER_VIEWPORT,
		GEOFENCE_INTRUDER_TILE
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DGeofenceListener
	{
		virtual ~IGW3DGeofenceListener() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum InsertionCallbackReturnType
		{
			/// Ignore this tile in visualization
			DISCARD = 0,
			/// Keep this tile for visualization
			KEEP = 1
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="tile_id"> TODO </param>
		/// <param name="env"> TODO </param>
		/// <param name="intruder_data"> TODO </param>
		/// <param name="tile_data"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual InsertionCallbackReturnType OnTaskInsert(uint64_t tile_id, GEOFENCE_INTRUDER_CALLBACK_TYPE cb_type, unsigned int px_width, unsigned int px_height, const GW3DEnvelope& env, IGW3DClientUserDataPtr intruder_data, IGW3DClientUserDataPtr& task_data, const char* layer_name) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="tile_id"> TODO </param>
		/// <param name="threaded_task_data"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnTaskComplete(uint64_t tile_id, IGW3DClientUserDataPtr task_data, const char* layer_name) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="tile_id"> TODO </param>
		/// <param name="threaded_task_data"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnTaskCancel(uint64_t tile_id, IGW3DClientUserDataPtr task_data, const char* layer_name) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="tile_id"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnTileComplete(uint64_t tile_id, const GW3DEnvelope& env) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnAllTasksComplete() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="env"> TODO </param>
		/// <param name="geo_fence_layer_name"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnGeoFenceRefresh(const GW3DEnvelope& env, const char* layer_name) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="task_data"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void GW3DTHREADED_SignalTaskStart(IGW3DClientUserDataPtr task_data) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="task_data"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void GW3DTHREADED_SignalTaskStop(IGW3DClientUserDataPtr task_data) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DGeoFenceIntruder
	{
		virtual ~IGW3DGeoFenceIntruder() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_Name() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="env"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Envelope(const GW3DEnvelope& env) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="dpp_lon"> TODO </param>
		/// <param name="dpp_lat"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_DegreesPerPixel(double dpp_lon, double dpp_lat) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="user_data"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_UserData(IGW3DClientUserDataPtr user_data) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="width"> TODO </param>
		/// <param name="height"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Viewport(const unsigned int& width, const unsigned int& height) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult check_TileProgress() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_IsWorkPending() const = 0;
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Means that the scale of the tiles is expected to change - vs. just a put_viewport 
		///				alone which could signify just a window resize. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void refresh() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="listener"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void    add_GlobalListener(IGW3DGeofenceListener* listener) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="listener"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void remove_GlobalListener(IGW3DGeofenceListener* listener) = 0;
	};
}