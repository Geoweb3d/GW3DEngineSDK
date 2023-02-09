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
		, current_type_(Weather_Type::NONE)
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
		Geoweb3d::Imagery::ImageryLoadProperties temp_imagery_properties;
		//TODO - switch these to the CPP interface
		temp_imagery_properties.priority = 1;
		temp_imagery_properties.screen_res_mode_ = true;
		temp_imagery_properties.resolution_mapping_ = 2;
		temp_imagery_properties.sampler_algo_ = Geoweb3d::Raster::BILINEAR;
		temp_imagery_properties.activity = true;

		current_raster_idx = 0;
		if (CreateImageryRep(sdk_context_, "../examples/media/test_service_xmls/servers/Bing/Hybrid.xml", 6, 0, true, temp_imagery_properties).expired())
		{
			printf("CreateImageryRep Error\n");
		}

		temp_imagery_properties.activity = false;
		if (CreateImageryRep(sdk_context_, "../examples/media/test_service_xmls/servers/Bing/Road Map.xml", 6, 0, false, temp_imagery_properties).expired())
		{
			printf("CreateImageryRep Error\n");
		}

		if (CreateImageryRep(sdk_context_, "../examples/media/test_service_xmls/servers/Google/Hybrid.xml", 6, 0, false, temp_imagery_properties).expired())

		{
			printf("CreateImageryRep Error\n");
		}

		if (CreateImageryRep(sdk_context_, "../examples/media/test_service_xmls/servers/Google/Road Map.xml", 6, 0, false, temp_imagery_properties).expired())
		{
			printf("CreateImageryRep Error\n");
		}

		if (CreateImageryRep(sdk_context_, "../examples/media/test_service_xmls/servers/ArcGIS/World Topo Map.xml", 6, 0, false, temp_imagery_properties).expired())
		{
			printf("CreateImageryRep Error\n");
		}

		if (CreateImageryRep(sdk_context_, "../examples/media/test_service_xmls/servers/NRL Tileserver/FAA Sectional Charts.xml", 6, 0, false, temp_imagery_properties).expired())
		{
			printf("CreateImageryRep Error\n");
		}

		if (CreateImageryRep(sdk_context_, "../examples/media/test_service_xmls/servers/USGS/Imagery Only.xml", 6, 0, false, temp_imagery_properties).expired())
		{
			printf("CreateImageryRep Error\n");
		}


		raster_drivers = sdk_context_->get_RasterDriverCollection();
		raster_repdrivers = sdk_context_->get_RasterLayerRepresentationDriverCollection();
		imagery_driver = raster_repdrivers->get_Driver("Imagery");
		imagery_overlay_driver = raster_repdrivers->get_Driver("Imagery-Overlay");
		rep_col_ = imagery_overlay_driver.lock()->get_RepresentationLayerCollection();

		//default 
		if(!LoadGEOMET(Weather_Type::WIND_SPEED))
		{
			printf("ERROR Loading GEOMET RADAR\n");
		}
		
		//------------------------------------------------------------------------------------------------------------------------------------------------------------
		//simulate another layer
		//test code to show how multiple imagery-overlay rep can co-exist
		if (0)
		{
			Geoweb3d::Imagery::ImageryLoadProperties imagery_properties;
			imagery_properties.invalid_blue_ = 0.0;
			imagery_properties.invalid_green_ = 0.0;
			imagery_properties.invalid_red_ = 0.0;
			imagery_properties.priority = 1;
			imagery_properties.sampler_algo_ = Geoweb3d::Raster::BILINEAR;
			imagery_properties.screen_res_mode_ = true;
			imagery_properties.resolution_mapping_ = 1;

			if( CreateImageryRep( sdk_context_,"../examples/media/CADRGTPC500000.tif", 6, 0,true, imagery_properties ).expired() )
			{
				printf("CreateImageryRep Error\n");
			}

			if (CreateImageryRep(sdk_context_, "../examples/media/CADRGGNC5000000.tif", 6, 0, true, imagery_properties).expired())
			{
				printf("CreateImageryRep Error\n");
			}
		}
		//------------------------------------------------------------------------------------------------------------------------------------------------------------

		if( CreateElevationRep( sdk_context_, "../examples/media/NED 10-meter DC.tif", 6, 0, true, 6.0).expired() )
		{
			printf("CreateElevationRep Error\n");
		}

		camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);
		double longitude, latitude;
		longitude = -85.6232494;
		latitude = 35.2590292;
		camera_controller_->put_Location(longitude, latitude);
		camera_controller_->put_Rotation(0, 89, 0);

		cam_mid_page_level_elev_.push_back(13174623); //dist we break into next page level
		cam_mid_page_level_elev_.push_back(6818515);
		cam_mid_page_level_elev_.push_back(3537953);
		cam_mid_page_level_elev_.push_back(1841209);
		cam_mid_page_level_elev_.push_back(961540);
		cam_mid_page_level_elev_.push_back(504236);
		cam_mid_page_level_elev_.push_back(265755);
		cam_mid_page_level_elev_.push_back(140938);
		cam_mid_page_level_elev_.push_back(75334);
		cam_mid_page_level_elev_.push_back(40685);
		cam_mid_page_level_elev_.push_back(22284);
		cam_mid_page_level_elev_.push_back(12457);
		cam_mid_page_level_elev_.push_back(7192);
		cam_mid_page_level_elev_.push_back(4404);
		cam_mid_page_level_elev_.push_back(3114);
		cam_mid_page_level_elev_.push_back(1557);
		cam_mid_page_level_elev_.push_back(778);
		cam_mid_page_level_elev_.push_back(550);
		cam_mid_page_level_elev_.push_back(337);

		camera_controller_->put_Elevation(cam_mid_page_level_elev_[0] + cam_mid_page_level_elev_[0] * 0.25f);

		navHelper_->put_HomePosition(camera_);
		return true;
	}

	bool MiniEngine::TurnOnNextMapService(bool increment)
	{
		int prev_index = current_raster_idx;
		if (increment)
		{
			current_raster_idx++;

			if (current_raster_idx == imagery_representations_.size())
			{
				current_raster_idx = 0;
			}
		}
		else
		{
			current_raster_idx--;

			if (current_raster_idx < 0)
			{
				current_raster_idx = imagery_representations_.size() - 1;
			}
		}

		auto previous_imagery_rep = imagery_representations_[prev_index].lock();
		if (previous_imagery_rep != nullptr)
		{
			previous_imagery_rep->put_Enabled(false);
		}

		auto current_imagery_rep = imagery_representations_[current_raster_idx].lock();
		if (current_imagery_rep != nullptr)
		{
			current_imagery_rep->put_Enabled(true);
		}

		return true;
	}

	bool MiniEngine::BuildRasterLayerImageCollection(const char* datasource_name, Geoweb3d::IGW3DRasterLayerImageCollectionPtr raster_layer_image_col)
	{
		Geoweb3d::GW3DResult res;
		//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
		//having to guess which data source driver is needed to open up a particular dataset.
		Geoweb3d::IGW3DRasterDataSourceWPtr  ds = raster_drivers->auto_Open(datasource_name, res);
		if (!ds.expired())
		{
			//going to assume all these will end up using the same driver - used to cycle out this dataset on keyboard input
			current_raster_driver_ = ds.lock()->get_Driver();
			current_raster_datasource_col_ = current_raster_driver_.lock()->get_RasterDataSourceCollection();
			data_sources_.push_back(ds);

			Geoweb3d::IGW3DRasterLayerCollection* lyr_col = ds.lock()->get_RasterLayerCollection();
			Geoweb3d::IGW3DRasterLayerWPtr lyr = lyr_col->get_AtIndex(0);

			if (!lyr.expired())
			{
				raster_layer_image_col->add(lyr);
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
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

	bool MiniEngine::LoadNOAAPrecipiation6Hour()
	{
		bool success = true;
		Geoweb3d::Imagery::ImageryLoadProperties imagery_properties;
			
		raster_layer_image_collection_ = sdk_context_->get_SceneGraphContext()->create_RasterLayerImageCollection();

		Geoweb3d::IGW3DPropertyCollectionPtr img_props = imagery_overlay_driver.lock()->get_PropertyCollection()->create_Clone();
		img_props->put_Property(Geoweb3d::Raster::ImageryProperties::SCREEN_RES_MODE, true);
		img_props->put_Property(Geoweb3d::Raster::ImageryProperties::RESOLUTION_MAPPING, 1);
		img_props->put_Property(Geoweb3d::Raster::ImageryProperties::SAMPLER_ALGO, Geoweb3d::Raster::SamplerAlg::BILINEAR);

		/*
			Here we see how to build up a IGW3DRasterLayerImageCollection that we will subsequently represent
		*/
		success &= BuildRasterLayerImageCollection("../examples/media/test_service_xmls/NOAA Weather/NOAA_Precip_Hour6_Layer33.xml",  raster_layer_image_collection_);
		success &= BuildRasterLayerImageCollection("../examples/media/test_service_xmls/NOAA Weather/NOAA_Precip_Hour12_Layer29.xml", raster_layer_image_collection_);
		success &= BuildRasterLayerImageCollection("../examples/media/test_service_xmls/NOAA Weather/NOAA_Precip_Hour18_Layer25.xml", raster_layer_image_collection_);
		success &= BuildRasterLayerImageCollection("../examples/media/test_service_xmls/NOAA Weather/NOAA_Precip_Hour24_Layer21.xml", raster_layer_image_collection_);
		success &= BuildRasterLayerImageCollection("../examples/media/test_service_xmls/NOAA Weather/NOAA_Precip_Hour30_Layer17.xml", raster_layer_image_collection_);
		success &= BuildRasterLayerImageCollection("../examples/media/test_service_xmls/NOAA Weather/NOAA_Precip_Hour36_Layer13.xml", raster_layer_image_collection_);
		success &= BuildRasterLayerImageCollection("../examples/media/test_service_xmls/NOAA Weather/NOAA_Precip_Hour42_Layer9.xml",  raster_layer_image_collection_);


		success &= LoadDatasourceAndRepresent(raster_layer_image_collection_.get(), img_props, 7200);

		return success;
	}

	
	bool MiniEngine::LoadGEOMET(Weather_Type type)
	{
		std::string file;
		switch (type)
		{
		case Weather_Type::RADAR:
			file = "../examples/media/test_service_xmls/GeoMet Weather/Weather Radar (NorthAmerica).xml";
			break;
		case Weather_Type::CLOUD:
			file = "../examples/media/test_service_xmls/GeoMet Weather/Total Cloud Cover (Global).xml";
			break;
		case Weather_Type::PRECIP:
			file = "../examples/media/test_service_xmls/GeoMet Weather/Precipitation Accumulation (Global).xml";
			break;
		case Weather_Type::WIND_SPEED:
			file = "../examples/media/test_service_xmls/GeoMet Weather/Wind Speed - Surface (Global).xml";
			break;
		case Weather_Type::WIND_VEL:
			file = "../examples/media/test_service_xmls/GeoMet Weather/Wind Velocity - Surface (Global).xml";
			break;
		default:
			return false;

		}
		bool success = false;
		Geoweb3d::Imagery::ImageryLoadProperties imagery_properties;
		Geoweb3d::IGW3DPropertyCollectionPtr img_props = imagery_overlay_driver.lock()->get_PropertyCollection()->create_Clone();
		img_props->put_Property(Geoweb3d::Raster::ImageryProperties::SCREEN_RES_MODE, true);
		img_props->put_Property(Geoweb3d::Raster::ImageryProperties::RESOLUTION_MAPPING, 1);
		img_props->put_Property(Geoweb3d::Raster::ImageryProperties::SAMPLER_ALGO, Geoweb3d::Raster::SamplerAlg::BILINEAR);

		Geoweb3d::GW3DResult res;
		//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
		//having to guess which data source driver is needed to open up a particular dataset.
		Geoweb3d::IGW3DRasterDataSourceWPtr  ds = raster_drivers->auto_Open(file.c_str(), res);
		Geoweb3d::IGW3DRasterLayerWPtr lyr;

		if (!ds.expired())
		{
			//going to assume all these will end up using the same driver - used to cycle out this dataset on keyboard input
			current_raster_driver_ = ds.lock()->get_Driver();
			current_raster_datasource_col_ = current_raster_driver_.lock()->get_RasterDataSourceCollection();
			data_sources_.push_back(ds);

			Geoweb3d::IGW3DRasterLayerCollection* lyr_col = ds.lock()->get_RasterLayerCollection();
			lyr = lyr_col->get_AtIndex(0);

			success = LoadDatasourceAndRepresent(lyr, img_props);
		}

		return success;
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


			if (!camera.lock()->get_DateTime()->put_DateFromDelimitedString("2002-6-21"))
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

			Geoweb3d::IGW3DEnvironmentEffects* env_effects = camera.lock()->get_EnvironmentEffects();
			if (env_effects)
			{
				Geoweb3d::IGW3DLightingConfiguration* light_config = env_effects->get_LightingConfiguration();
				if (light_config)
				{
					light_config->put_EnableLightingOverride(true);
				}
			}
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
				case Geoweb3d::Key::Num1:
				{
					if (current_type_ != Weather_Type::RADAR_NOOA)
					{
						current_type_ = Weather_Type::RADAR_NOOA;
						CloseCurrentDataSource();
						if (!LoadNOAAPrecipiation6Hour())
						{
							printf("ERROR Loading RADAR_NOOA\n");
						}
						else
						{
							printf("Loading RADAR_NOOA\n");
						}
					}

					filter_out = true;
				}
				break;

				case Geoweb3d::Key::Num2:
				{
					if (current_type_ != Weather_Type::RADAR)
					{
						current_type_ = Weather_Type::RADAR;
						CloseCurrentDataSource();
						if (!LoadGEOMET(Weather_Type::RADAR))
						{
							printf("ERROR Loading GEOMENT RADAR\n");
						}
						else
						{
							printf("Loading GEOMET RADAR\n");
						}
					}

					filter_out = true;
				}
				break;

				case Geoweb3d::Key::Num3:
				{
					if (current_type_ != Weather_Type::PRECIP)
					{
						current_type_ = Weather_Type::PRECIP;
						CloseCurrentDataSource();
						if (!LoadGEOMET(Weather_Type::PRECIP))
						{
							printf("ERROR Loading GEOMET PRECIP\n");
						}
						else
						{
							printf("Loading GEOMET PRECIP\n");
						}
					}

					filter_out = true;
				}
				break;

				case Geoweb3d::Key::Num4:
				{
					if (current_type_ != Weather_Type::CLOUD)
					{
						current_type_ = Weather_Type::CLOUD;
						CloseCurrentDataSource();
						if (!LoadGEOMET(Weather_Type::CLOUD))
						{
							printf("ERROR Loading GEOMET CLOUD\n");
						}
						else
						{
							printf("Loading GEOMET CLOUD\n");
						}
					}

					filter_out = true;
				}
				break;

				case Geoweb3d::Key::Num5:
				{
					if (current_type_ != Weather_Type::WIND_SPEED)
					{
						current_type_ = Weather_Type::WIND_SPEED;
						CloseCurrentDataSource();
						if (!LoadGEOMET(Weather_Type::WIND_SPEED))
						{
							printf("ERROR Loading GEOMET WIND_SPEED\n");
						}
						else
						{
							printf("Loading GEOMET WIND_SPEED\n");
						}
					}

					filter_out = true;
				}
				break;

				case Geoweb3d::Key::Num6:
				{
					if (current_type_ != Weather_Type::WIND_VEL)
					{
						current_type_ = Weather_Type::WIND_VEL;
						CloseCurrentDataSource();
						if (!LoadGEOMET(Weather_Type::WIND_VEL))
						{
							printf("ERROR Loading GEOMET WIND_VEL\n");
						}
						else
						{
							printf("Loading GEOMET WIND_VEL\n");
						}
					}

					filter_out = true;
				}
				break;

				//PLAY
				case Geoweb3d::Key::A:
				{
					if (player_.lock())
					{
						if (win_event.Key.shift)
						{
							player_.lock()->put_PlayerState(Geoweb3d::IGW3DPlanetRasterTimePlayer::PlanetRasterTimePlayerState::PLAY_AND_REPEAT_COVERAGE);
							printf("PLAY WITH REPEAT COVERAGE\n");
						}
						else if (win_event.Key.control)
						{
							player_.lock()->put_PlayerState(Geoweb3d::IGW3DPlanetRasterTimePlayer::PlanetRasterTimePlayerState::PLAY_AND_REPEAT);
							printf("PLAY WITH REPEAT\n");
						}
						else
						{
							player_.lock()->put_PlayerState(Geoweb3d::IGW3DPlanetRasterTimePlayer::PlanetRasterTimePlayerState::PLAY);
							printf("PLAY\n");
						}
						filter_out = true;
					}
				}
				break;

				//PAUSE
				case Geoweb3d::Key::S:
				{
					if (player_.lock())
					{
						player_.lock()->put_PlayerState(Geoweb3d::IGW3DPlanetRasterTimePlayer::PlanetRasterTimePlayerState::PAUSE);
						printf("PAUSE\n");
						filter_out = true;
					}
				}
				break;

				////STOP
				case Geoweb3d::Key::D:
				{
					if (player_.lock())
					{
						player_.lock()->put_PlayerState(Geoweb3d::IGW3DPlanetRasterTimePlayer::PlanetRasterTimePlayerState::STOP);
						printf("STOP\n");
						filter_out = true;
					}
				}
				break;

				//SPEED
				case Geoweb3d::Key::F:
				{
					if (player_.lock())
					{
						if (win_event.Key.shift)
						{
							player_speed_ -= 5;
							player_speed_ = std::clamp(player_speed_, 0, 200);

						}
						else
						{
							player_speed_ += 5;
							player_speed_ = std::clamp(player_speed_, 0, 200);
						}

						Geoweb3d::GW3DResult res = player_.lock()->put_Speed(player_speed_);
						printf("SPEED FACTOR: %d\n", player_speed_);
						filter_out = true;
					}
				}
				break;

				//SEEK
				case Geoweb3d::Key::W:
				{
					if (player_.lock())
					{
						player_.lock()->put_SeekForward();
						printf("SEEK FORWARD\n");
						filter_out = true;
					}
				}
				break;

				//SEEK
				case Geoweb3d::Key::E:
				{
					if (player_.lock())
					{
						player_.lock()->put_SeekBackwards();
						printf("SEEK BACKWARDS\n");
						filter_out = true;
					}
				}
				break;

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
					if (win_event.Key.shift)
					{
						TurnOnNextMapService(false);
					}
					else
					{
						TurnOnNextMapService(true);
					}
					filter_out = true;
				}
				break;

				//CAM
				case Geoweb3d::Key::R:
				{
					if (win_event.Key.shift)
					{
						if (current_lod_ > 0)
						{
							current_lod_--;
						}
					}
					else
					{
						if (current_lod_ < cam_mid_page_level_elev_.size())
						{
							current_lod_++;
						}
					}

					if (current_lod_ == 0)
					{
						camera_controller_->put_Elevation(cam_mid_page_level_elev_[0] + cam_mid_page_level_elev_[0]*0.25f);
					}
					else if (current_lod_ == cam_mid_page_level_elev_.size())
					{
						camera_controller_->put_Elevation(cam_mid_page_level_elev_[cam_mid_page_level_elev_.size()-1] - cam_mid_page_level_elev_[cam_mid_page_level_elev_.size()-1] * 0.25f);
					}
					else
					{
						camera_controller_->put_Elevation((cam_mid_page_level_elev_[current_lod_] + cam_mid_page_level_elev_[current_lod_+1]) * 0.5f);
					}

					printf("CURRENT LOD %d\n", current_lod_);
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::H:
				{
					printf("Changeable Properties:\n");
					printf("\n"
						"Num 1 - NOAA RADAR\n"				
						"Num 2 - GEOMET RADAR\n"
						"Num 3 - GEOMET PRECIP\n"
						"Num 4 - GEOMET CLOUD\n"
						"Num 5 - GEOMET WIND_SPEED\n"
						"Num 6 - GEOMET WIND_VEL\n"
						"Q - ALPHA (W/SHIFT DECREASE)	\n"
						"W - SEEK FORWARD	\n"
						"E - SEEK BACKWARDS						\n"
						"R - DESCEND CAM TO NEXT LOD (W/SHIFT ASCEND))	\n"
						"A - PLAY MODE (W/SHIFT - PLAY_AND_REPEAT_COVERAGE, W/CTRL PLAY_AND_REPEAT						\n"
						"S - PAUSE						\n"
						"D - STOP			\n"
						"F - SPEED FACTOR (W/SHIFT DECREASE)				\n"
						"Z - CHANGE BASEMAP SERVICE\n"
					);


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

