#include "MiniEngine.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"


#include "engine/IGW3DSceneGraphContext.h"
#include "engine/IGW3DRasterLayerImageCollection.h"
#include "engine/IGW3DRasterTimeController.h"
#include "engine/IGW3DPlanetRasterTimePlayer.h"
#include "core/IGW3DRasterLayerImageCollectionFinalizationToken.h"
#include "engine/IGW3DRasterLayerEnvelopeStream.h"
#include "GeoWeb3dCore/SystemExports.h"

#include <deque>
#include <set>
#include <map>
#include <iostream>


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="sdk_context">	Context for the sdk. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	MiniEngine::MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context) 
		: sdk_context_(sdk_context) 
		, navHelper_(new NavigationHelper())
		, player_speed_(20)
		, transition_blending_ (false)
		, alpha_(1.0)
		, transition_blending_percent_(0.5)
	{

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Destructor. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	MiniEngine::~MiniEngine() 
	{
		delete navHelper_;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Updates this object. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool MiniEngine::Update()
	{
		DoPreDrawWork();

		if (sdk_context_->draw(window_) == Geoweb3d::GW3D_sOk)
		{
			DoPostDrawWork();
			return true;
		}

		//return that we did not draw. (app closing?)
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Loads a configuration. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="example_name">	Name of the example. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool MiniEngine::LoadConfiguration(const char *example_name)
	{
		window_ = Create3DWindow(sdk_context_, example_name, this);

		if (window_.expired())return false;

		camera_ = CreateCamera(window_, "Camera 1");
		camera_controller_ = camera_.lock()->get_CameraController();
		Geoweb3d::IGW3DLatitudeLongitudeGrid* grid = camera_.lock()->get_LatitudeLongitudeGrid();
		grid->put_Enabled(false);

		if (camera_.expired())return false;

		sdk_context_->put_EnableEventStream(true);

		return true;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Loads a data. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="rootpath">	The rootpath. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool MiniEngine::LoadData(const char *rootpath)
	{
		LoadDemoImageryDataset(sdk_context_);

		raster_drivers = sdk_context_->get_RasterDriverCollection();
		raster_repdrivers = sdk_context_->get_RasterLayerRepresentationDriverCollection();
		imagery_driver = raster_repdrivers->get_Driver("Imagery");
		imagery_overlay_driver = raster_repdrivers->get_Driver("Imagery-Overlay");
		rep_col_ = imagery_overlay_driver.lock()->get_RepresentationLayerCollection();

		
		bool success = false;
		Geoweb3d::Imagery::ImageryLoadProperties imagery_properties;
		Geoweb3d::IGW3DPropertyCollectionPtr img_props = imagery_overlay_driver.lock()->get_PropertyCollection()->create_Clone();
		img_props->put_Property(Geoweb3d::Raster::ImageryProperties::SCREEN_RES_MODE, true);
		img_props->put_Property(Geoweb3d::Raster::ImageryProperties::RESOLUTION_MAPPING, 1);
		img_props->put_Property(Geoweb3d::Raster::ImageryProperties::SAMPLER_ALGO, Geoweb3d::Raster::SamplerAlg::NEAREST_NEIGHBOR);

		//NOTE: Here we are using a raster of contours - you can replace this with a datasouce of your choosing. 
		std::string file = "../examples/media/DC_15m_contours_10m_rasterized.tif";
		Geoweb3d::GW3DResult res;
		//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
		//having to guess which data source driver is needed to open up a particular dataset.
		Geoweb3d::IGW3DRasterDataSourceWPtr  ds = raster_drivers->auto_Open(file.c_str(), res);
		Geoweb3d::IGW3DRasterLayerWPtr lyr;
		Geoweb3d::GW3DEnvelope env;

		if (!ds.expired())
		{
			//going to assume all these will end up using the same driver - used to cycle out this dataset on keyboard input
			current_raster_driver_ = ds.lock()->get_Driver();
			current_raster_datasource_col_ = current_raster_driver_.lock()->get_RasterDataSourceCollection();
			data_sources_.push_back(ds);

			Geoweb3d::IGW3DRasterLayerCollection* lyr_col = ds.lock()->get_RasterLayerCollection();
			lyr = lyr_col->get_AtIndex(0);

			success = LoadDatasourceAndRepresent(lyr, img_props);

			env.merge(lyr.lock()->get_Envelope());
		}

		if (CreateElevationRep(sdk_context_, "../examples/media/NED 10-meter DC.tif", 6, 0, true, 6.0).expired())
		{
			printf("CreateElevationRep Error\n");
		}

		camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);
		double longitude, latitude;
		longitude = (env.MinX + env.MaxX) * 0.5;
		latitude = (env.MinY + env.MaxY) * 0.5;
		camera_controller_->put_Location(longitude, latitude);
		camera_controller_->put_Elevation(3000);
		camera_controller_->put_Rotation(0, 89, 0);

		navHelper_->put_HomePosition(camera_);
		return true;
	}

	void MiniEngine::CloseCurrentDataSource()
	{
		for (int i = 0; i < img_representations_.size(); i++)
		{
			rep_col_->close(img_representations_[i]);
		}
		img_representations_.clear();

		for (int i = 0; i < data_sources_.size(); i++)
		{
			current_raster_datasource_col_->close(data_sources_[i]);
		}
		data_sources_.clear();
	}

	bool MiniEngine::LoadDatasourceAndRepresent(Geoweb3d::IGW3DRasterLayerWPtr raster_layer, Geoweb3d::IGW3DPropertyCollectionPtr& img_props)
	{
		controller_= sdk_context_->get_GW3DRasterTimeController(raster_layer, nullptr);
		controller_.lock()->put_AutoUpdateTimeExtentState(Geoweb3d::IGW3DRasterTimeController::AUTO_UPDATE_TIME_EXTENT_ON);
		player_= sdk_context_->RegisterRasterTimeControllerWithPlanet(controller_);
		player_.lock()->put_PlayerState(Geoweb3d::IGW3DPlanetRasterTimePlayer::PlanetRasterTimePlayerState::PLAY_AND_REPEAT_COVERAGE);
		printf("PLAY_AND_REPEAT_COVERAGE\n");
		Geoweb3d::GW3DResult res = player_.lock()->put_Speed(player_speed_);
		printf("SPEED FACTOR: %d\n", player_speed_);
		player_.lock()->put_Enabled(true);

		Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
		params.representation_default_parameters = img_props;
		params.page_level = 0;
		params.priority = 0;
		params.representation_layer_activity = true;
		Geoweb3d::IGW3DRasterRepresentationWPtr lyr_rep = rep_col_->create(raster_layer, params);
		auto lyr_rep_lock = lyr_rep.lock();
		if (lyr_rep_lock)
		{
			img_representations_.push_back(lyr_rep);
			return true;
		}
		else
		{
			return false;
		}

		return false;

	}

	bool MiniEngine::LoadDatasourceAndRepresent(Geoweb3d::IGW3DRasterLayerImageCollection* raster_layer_image_col, Geoweb3d::IGW3DPropertyCollectionPtr& img_props, int duration)
	{
		layers_token_ = raster_layer_image_col->create_FinalizeToken(duration);
		controller_ = sdk_context_->get_GW3DRasterTimeController(layers_token_, nullptr);
		controller_.lock()->put_AutoUpdateTimeExtentState(Geoweb3d::IGW3DRasterTimeController::AUTO_UPDATE_TIME_EXTENT_ON);
		player_ = sdk_context_->RegisterRasterTimeControllerWithPlanet(controller_);
		player_.lock()->put_PlayerState(Geoweb3d::IGW3DPlanetRasterTimePlayer::PlanetRasterTimePlayerState::PLAY_AND_REPEAT_COVERAGE);
		printf("PLAY_AND_REPEAT_COVERAGE\n");
		Geoweb3d::GW3DResult res = player_.lock()->put_Speed(player_speed_);
		printf("SPEED FACTOR: %d\n", player_speed_);
		player_.lock()->put_Enabled(true);

		Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
		params.representation_default_parameters = img_props;
		params.page_level = 0;
		params.priority = 0;
		params.representation_layer_activity = true;

		Geoweb3d::IGW3DRasterRepresentationWPtr lyr_rep = rep_col_->create(layers_token_, params);

		auto lyr_rep_lock = lyr_rep.lock();
		if (lyr_rep_lock)
		{
			img_representations_.push_back(lyr_rep);
			return true;
		}
		else
		{
			return false;
		}

		return false;
	
	}

	Geoweb3d::IGW3DWindowWPtr MiniEngine::Create3DWindow(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events)
	{
		Geoweb3d::IGW3DWindowCollection * wcol = sdk_context->get_WindowCollection();

		Geoweb3d::IGW3DWindowWPtr window = wcol->create_3DWindow(title, GW3D_OVERLAPPED, 10, 10, 1280, 720, 0, Geoweb3d::IGW3DStereoDriverPtr(), window_events);

		if (window.expired())
		{
			printf("Error Creating window: [%s]!\n", title);

			return Geoweb3d::IGW3DWindowWPtr();
		}

		return window;
	}

	Geoweb3d::IGW3DCameraWPtr MiniEngine::CreateCamera(Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle)
	{

		Geoweb3d::IGW3DCameraCollection *cameracollection = windowptr.lock()->get_CameraCollection();
		Geoweb3d::IGW3DCameraWPtr camera = cameracollection->create(cameratitle);

		if (!camera.expired())
		{
			printf("DelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromDelimitedString()->c_str());
			printf("UnDelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromUnDelimitedString()->c_str());

			camera.lock()->get_DateTime()->put_isUseComputerDate(false);
			camera.lock()->get_DateTime()->put_isUseComputerTime(false);


			if (!camera.lock()->get_DateTime()->put_DateFromDelimitedString("2002-1-25"))
			{
				printf("Error with put_DateFromDelimitedString\n");
			}

			if (!camera.lock()->get_DateTime()->put_DateFromUnDelimitedString("2002125"))
			{
				printf("Error with put_DateFromUnDelimitedString\n");
			}


			printf("User Set: DelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromDelimitedString()->c_str());
			printf("User Set: UnDelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromUnDelimitedString()->c_str());


			camera.lock()->get_DateTime()->put_Time(12, 0);

			navHelper_->add_Camera(camera);
		}

		return camera;
	}

	Geoweb3d::IGW3DCameraWPtr MiniEngine::GetCamera()
	{
		return camera_;
	}

	void MiniEngine::OnDraw2D(const Geoweb3d::IGW3DCameraWPtr &camera) {};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Executes the pre draw work operation. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void MiniEngine::DoPreDrawWork()
	{
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Executes the post draw work operation. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void MiniEngine::DoPostDrawWork()
	{
		
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	OS event system, basically a helper off ProcessLowLevelEvent. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="win_event">	The window event. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void MiniEngine::ProcessEvent(const Geoweb3d::WindowEvent& win_event)
	{

			bool filter_out = false;

			switch (win_event.Type)
			{

			case win_event.KeyPressed:
			{
				switch (win_event.Key.code)
				{
				//ALPHA
				case Geoweb3d::Key::Q:
				{
					if (player_.lock())
					{
						if (win_event.Key.shift)
						{
							Geoweb3d::GW3DResult res = player_.lock()->put_Alpha(alpha_ - 0.1f);
							if (Geoweb3d::Succeeded(res))
							{
								alpha_ = alpha_ - 0.1f;
							}
						}
						else
						{
							Geoweb3d::GW3DResult res = player_.lock()->put_Alpha(alpha_ + 0.1f);
							if (Geoweb3d::Succeeded(res))
							{
								alpha_ = alpha_ + 0.1f;
							}
						}
						printf("ALPHA %1.1f\n", alpha_);
						filter_out = true;
					}
				}
				break;

				//ALPHA
				case Geoweb3d::Key::Z:
				{
					toggle_raster_basemap_ = !toggle_raster_basemap_;
					filter_out = true;
				}
				break;

				default:
					break;
				};//End of switch

			}
			break;

			default:
				break;
			};//End of switch

		if (!filter_out)
		{
			navHelper_->ProcessEvent(win_event, window_);
		}

	}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	OS event system, raw data directly from the windows message pump. </summary>
		///
		/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
		///
		/// <param name="hWndParent">	The window parent. </param>
		/// <param name="message">   	The message. </param>
		/// <param name="wParam">	 	The wParam field of the message. </param>
		/// <param name="lParam">	 	The lParam field of the message. </param>
		///
		/// <returns>	. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		LRESULT CALLBACK MiniEngine::ProcessLowLevelEvent( OSWinHandle hWndParent, unsigned int message, WPARAM wParam, LPARAM lParam)
		{
			// We don't forward the WM_CLOSE message to prevent the internal SDK from closing us (click on X)
			if (message == WM_CLOSE)
			{
				printf("ProcessLowLevelEvent: WM_Close detected and prevented!\n");
				return 0;
			}
			return 1;
		}

