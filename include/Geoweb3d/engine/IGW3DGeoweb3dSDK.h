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
#include "../core/GW3DGuids.h"
#include "../../Geoweb3dCore/Geoweb3dEngine.h"

/* Primary namespace */
namespace Geoweb3d
{

    const int DISABLE_VERTICAL_SYNC = 0x10000000;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	The primary SDK context, through which most of the Geoweb3d Engine APIs may be
    /// 			accessed. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DGeoweb3dSDK : public IGW3DIBaseObject
    {
        virtual ~IGW3DGeoweb3dSDK() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates initialization configuration object. </summary>
        ///
        /// <returns>	The new initialization configuration. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DInitializationConfigurationPtr create_InitializationConfiguration() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Initialize the Geoweb3d Engine Library</summary>
        ///
        /// <remarks>	This must be called before calling any other APIs. </remarks>
        ///
        /// <param name="productname">		   	The product name. </param>
        /// <param name="config">			   	One time configurations of the SDK context. </param>
        /// <param name="productversion_major">	The major product version. </param>
        /// <param name="productversion_minor">	The minor product version. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult  InitializeLibrary(const char* productname, const IGW3DInitializationConfigurationPtr& config, int productversion_major, int productversion_minor) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Returns the result of initialization the Geoweb3d Engine Library. </summary>
        ///
        /// <remarks>	This will be the same result returned from the first
        ///          	InitializeLibrary call. </remarks>
        /// 
        /// <returns>	GW3D_sOk if initialization succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult get_LibraryInitializationResult() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the collection of all SDK windows. </summary>
        ///
        /// <returns>	The window collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DWindowCollection* get_WindowCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the collection of all Geofences. </summary>
        ///
        /// <returns>	The window collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DGeoFenceCollection* get_GeoFenceCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the collection of all Geofence Intruders. </summary>
        ///
        /// <returns>	The window collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DGeoFenceIntruderCollection* get_GeoFenceIntruderCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the scenegraph context. </summary>
        ///
        /// <returns>	The scenegraph context. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DSceneGraphContext* get_SceneGraphContext() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the display devices context. </summary>
        ///
        /// <returns>	The display devices context. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DDisplayDevices* get_DisplayDevices() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a Layer Helper Collection. </summary>
		///
		/// <returns>	The new layer helper collection. </returns>
		/// 
		/// <see cref="IGW3DLayerHelperCollection"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DLayerHelperCollectionPtr create_LayerHelperCollection() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the collection of all vector drivers. </summary>
        ///
        /// <remarks>	Vector data is created, loaded, and accessed through the vector drivers
        /// 			and vector driver collection. </remarks>
        ///
        /// <returns>	The vector driver collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorDriverCollection* get_VectorDriverCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the collection of all vector representation drivers. </summary>
        ///
        /// <remarks>	Representations of vector layers are created and accessed through the
        /// 			representation drivers. </remarks>
        ///
        /// <returns>	The representation driver collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorRepresentationDriverCollection* get_VectorRepresentationDriverCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the collection of all raster drivers. </summary>
        ///
        /// <remarks>	Raster data is loaded and accessed through the raster drivers
        /// 			and raster driver collection. </remarks>
        ///
        /// <returns>	The raster driver collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterDriverCollection* get_RasterDriverCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the collection of raster layer representation drivers. </summary>
        ///
        /// <remarks>	Representations of raster layers are created and accessed through the
        /// 			representation drivers. </remarks>
        ///
        /// <returns>	The raster representation driver collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterRepresentationDriverCollection* get_RasterLayerRepresentationDriverCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The Representation Layer Draw Collection (not yet implemented). </summary>
        ///
        /// <returns>	The representation layer draw collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorRepresentationHelperCollection* get_RepresentationLayerDrawCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The collection of line segment intersection tests. </summary>
        ///
        /// <remarks>	Line segment intersection tests allow the developer to query all intersections
        /// 			of a line segment with one or more 3D vector representations. This is useful for
        /// 			line of sight analysis, mouse queries, etc.</remarks>
        ///
        /// <returns>	The line segment intersection test collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DLineSegmentIntersectionTestCollection* get_LineSegmentIntersectionTestCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The collection of perspective viewshed analyses. </summary>
        ///
        /// <remarks>	viewshed tests allow the developer to view all intersections
        /// 			of visible and non visible things within a 3d scene. </remarks>
        ///
        /// <returns>	The panoramic viewshed test collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DPerspectiveViewshedTestCollection* get_PerspectiveViewshedTestCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The collection of panoramic viewshed analyses. </summary>
        ///
        /// <remarks>	viewshed tests allow the developer to view all intersections
        /// 			of visible and non visible things within a 3d scene.</remarks>
        ///
        /// <returns>	The perspective viewshed test collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DPanoramicViewshedTestCollection* get_PanoramicViewshedTestCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The collection of slope analyses. </summary>
        ///
        /// <remarks>	Slope Analyses color code the scene based on slope angle. </remarks>
        ///
        /// <returns>	The slope analysis collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DSlopeAnalysisCollection* get_SlopeAnalysisCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The collection of Visibility Dome tests. </summary>
        ///
        /// <returns>	The VisibilityDome test collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVisibilityDomeTestCollection* get_VisibilityDomeTestCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The collection of Terrain Avoidance tests. </summary>
        ///
        /// <returns>	The TerrainAvoidance test collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DTerrainAvoidanceTestCollection* get_TerrainAvoidanceTestCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The collection of 3D stereo drivers. </summary>
        ///
        /// <remarks> 3D Stereo Drivers.  Some drivers require specific hardware or hardware settings to
        /// 			property function. </remarks>
        ///
        /// <returns>	The stereo driver collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStereoDriverCollection* get_StereoDriverCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The collection of scenefiles </summary>
        ///
        /// <remarks> load, destroy, or get a scene that was loaded via a scenefile. </remarks>
        ///
        /// <returns>	The scene collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DSceneCollection* get_SceneCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the media center context. </summary>
        ///
        /// <remarks>	The media center is used for capturing snapshots/videos/broadcasts. </remarks>
        ///
        /// <returns>	The media center context. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DMediaCenter* get_MediaCenter() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets if the event stream is enabled. </summary>
        ///
        /// <returns>	true if it is enabled, false if it is not. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_IsEventStreamEnabled() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enables or disables the event stream. </summary>
        ///
        /// <remarks>	Enabling the event stream tells the SDK to store events so you can stream them out.
        /// 			
        /// 			Note: Disabling the event stream will flush any remaining events in the queue. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_EnableEventStream(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Executes the event stream operation. </summary>
        ///
        /// <remarks>	Initiates a stream of callbacks into your IGW3DEventStream with all events that
        /// 			have occurred since the	last stream.  All streamed events are then flushed. </remarks>
        ///
        /// <param name="stream">	[in] The stream. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void run_EventStream(IGW3DEventStream& stream) = 0;

    //SKIP-CODE-BEGIN

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	appends a directory to the plug-in search path. </summary>
        ///
        /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
        ///
        /// <param name="directory">	Pathname of the directory. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //virtual void addPluginDirectory(const char* directory) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	clears all directories from the plug-in search path. </summary>
        ///
        /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //virtual void resetPluginDirectories() = 0;

    //SKIP-CODE-END

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Updates a Geofence Intruder. </summary>
        ///
        /// <param name="g">	[in] The IGW3DGeoFenceIntruderWPtr to process. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult UpdateGeoFenceIntruders(IGW3DGeoFenceIntruderWPtr& g, bool fully_tiled = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Draws the given window. </summary>
        ///
        /// <param name="window">	[in] The IGW3DWindowWPtr to process. </param>
        /// <param name="vertical_sync_state">	[in] behavior of using the vertical sync </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult draw(IGW3DWindowWPtr& window, int vertical_sync_state = -1) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets length of daylight at a given location. </summary>
        ///
        /// <remarks>	THIS IS A POC, so use at your own risk.  Not fully tested. </remarks>
        ///
        /// <param name="latitude">   	The latitude. </param>
        /// <param name="day_of_year">	The day of year. </param>
        ///
        /// <returns>	The length of a day. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual float get_LengthOfDay(double latitude, int day_of_year) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the main animation controller object. </summary>
        ///
        /// <returns>	The animation controller interface. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DAnimationController* get_AnimationController() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates the main Geoweb3d Engine SDK interface. </summary>
        ///
        /// <returns>	The new interface. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static IGW3DGeoweb3dSDKPtr CreateInterface()
        {
            return CreateInterfacePtr< IGW3DGeoweb3dSDKPtr, IGW3DGeoweb3dSDK>(GUID_GEOWEB3D_SDK);
        }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the Time Controller of a Layer </summary>
		///
		/// <remarks>	A IGW3DRasterTimeController has the ability to monitor and change the current time extents 
		///				associated with the layer. Any representations will be automatically updated. 
		///				IGW3DRasterTimeUpdateCallback is a callback to trigger the client when any of the time meta data has 
		///				been changed. This will only happen when the layer is set to auto track the current system time. </remarks>
		///
		/// <returns>	The RasterTimeController Object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DRasterTimeControllerWPtr get_GW3DRasterTimeController(IGW3DRasterLayerWPtr layer, IGW3DRasterTimeControllerCallback* cb) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the Time Controller of a Layer Collection </summary>
		///
		/// <remarks>	IGW3DRasterTimeController has the ability to monitor and change the current time extents 
		///				associated with the layer collection. Any representations will be automatically updated. 
		///				IGW3DRasterTimeUpdateCallback is a callback to trigger the client when any of the time meta data has 
		///				been changed. This will only happen when the layer is set to auto track the current system time. </remarks>
		///
		/// <returns>	The RasterTimeController Object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		
		virtual IGW3DRasterTimeControllerWPtr get_GW3DRasterTimeController(IGW3DRasterLayerImageCollectionFinalizationTokenPtr collection, IGW3DRasterTimeControllerCallback* cb) = 0;
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Registers the Controller with the Planet. </summary>
		///
		/// <remarks>	This is a way to play the universal time of the planet overlay imagery outside of a camera date time. 
		///				This is will detach the layer times from being camera time dependent
		///				(IGW3DPlanetRasterTimePlayer must be enabled as well). </remarks>
		///
		/// <returns>	The PlanetRasterTimePlayer Object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		
		virtual IGW3DPlanetRasterTimePlayerWPtr RegisterRasterTimeControllerWithPlanet(IGW3DRasterTimeControllerWPtr time_controller) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the Contour Utility . </summary>
		///
		/// <returns>	The IGW3DContourUtility interface. </returns>
		/// 
		/// <see cref="IGW3DContourUtility"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DContourUtility* get_ContourUtility() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the Data Source Compositor . </summary>
		///
		/// <returns>	The IGW3DDataSourceCompositor interface. </returns>
		/// 
		/// <see cref="IGW3DDataSourceCompositor"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DDataSourceCompositor* get_DataSourceCompositor() = 0;
    };
}