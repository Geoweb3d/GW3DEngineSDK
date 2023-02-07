#include "MiniEngine.h"


#include "engine/IGW3DRasterTimeController.h"
#include "engine/IGW3DPlanetRasterTimePlayer.h"

//struct to store information about selected features
struct FeatureInfo
{
	long	id;
	double	intersection_distance;

	FeatureInfo(long object_id, double distance = 0., int tex_prop_index = 0)
		: id(object_id), intersection_distance(distance)
	{}

	bool operator <(const FeatureInfo& rhs) const
	{
		return id < rhs.id;
	}

	bool operator ==(const FeatureInfo& rhs) const
	{
		return id == rhs.id;
	}

};

// Custom stream object for querying information about the selected feature(s)
class PathIntersectionTestStream : public Geoweb3d::IGW3DVectorLayerStream
{

public:

	PathIntersectionTestStream()
	{
		reset();
	}

	virtual ~PathIntersectionTestStream()
	{
		//cleanup
		feature_info_set.clear();
	}

	//insert selected features to be processed
	void insert(long object_id, double intersection_distance)
	{
		//store only the nearest intersection distance per feature
		auto existing = feature_info_set.find(object_id);
		if (existing != feature_info_set.end())
		{
			if (existing->intersection_distance <= intersection_distance)
			{
				return;
			}
		}
		FeatureInfo finfo(object_id);
		finfo.intersection_distance = intersection_distance;
		feature_info_set.insert(finfo);
	}

	//find FeatureInfo for a given object id
	const FeatureInfo& find(long object_id)
	{
		return *feature_info_set.find(object_id);
	}

	// When IGW3DVectorLayer::Stream is called with this class, the SDK will call OnStream for each requested feature
	virtual bool OnStream(Geoweb3d::IGW3DVectorLayerStreamResult* result)
	{
		// Request the attibute data for the current feature
		const Geoweb3d::IGW3DAttributeCollection* attribute_collection = result->get_AttributeCollection();

		// Query the object id (or feature id) for the current feature
		long object_id = result->get_ObjectID();

		FeatureInfo finfo = find(object_id);

		std::cout << " Feature Info: <id : " << object_id << " >  < Intersection Distance: " << finfo.intersection_distance << " >" << std::endl;
		return true;
	}

	virtual bool OnError()
	{
		return false;
	}

	virtual unsigned long count() const
	{
		return (long)feature_info_set.size();
	}

	virtual bool next(int64_t* ppVal)
	{
		if (nIndex_ >= count())
		{
			*ppVal = 0;
			reset();
			return false;
		}
		*ppVal = feature_info_set_iterator->id;
		feature_info_set_iterator++;
		return true;
	}

	virtual void reset()
	{
		nIndex_ = 0;
		feature_info_set_iterator = feature_info_set.begin();
	}

	virtual int64_t operator[](unsigned long index)
	{
		return get_AtIndex(index);
	}

	virtual int64_t get_AtIndex(unsigned long index)
	{
		if (index >= count())
		{
			//error
			return -1;
		}

		std::set< FeatureInfo >::iterator temp_feature_info_set_iterator = feature_info_set.begin();
		for (unsigned i = 0; i < index; ++i)
		{
			++temp_feature_info_set_iterator;
		}
		return temp_feature_info_set_iterator->id;
	}

private:

	unsigned nIndex_;
	std::set< FeatureInfo > feature_info_set;
	std::set< FeatureInfo >::iterator feature_info_set_iterator;
};



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

bool MiniEngine::LoadConfiguration(const char* example_name)
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

bool MiniEngine::LoadData(const char* rootpath)
{
	LoadDemoImageryDataset(sdk_context_);

	Geoweb3d::GW3DResult res;
	Geoweb3d::IGW3DVectorLayerWPtr layer;
	Geoweb3d::GW3DEnvelope env;

	if (CreateElevationRep(sdk_context_, "../examples/media/NED 10-meter DC.tif", 16, 0, true, 6.0, env).expired())
	{
		printf("could not load elevation data source\n");
		return false;
	}

	env.sort();
	
	{
		Geoweb3d::IGW3DRasterDriverCollection* raster_drivers = sdk_context_->get_RasterDriverCollection();
		Geoweb3d::IGW3DLayerHelperCollectionPtr layer_col = sdk_context_->create_LayerHelperCollection();

		//Populate the IGW3DLayerHelperCollection wil the layers of interest.
		//For this example use all dems that are loaded within the target bounds. 
		MyQuery raster_query;
		Geoweb3d::GW3DResult query_res = Geoweb3d::Raster::StreamRasterQuery(env, raster_query);
		if (Geoweb3d::Succeeded(query_res))
		{
			for (auto& filename : raster_query.file_names_)
			{
				Geoweb3d::IGW3DRasterDataSourceWPtr rdatasource2;
				Geoweb3d::GW3DResult result;
				//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
				//having to guess which data source driver is needed to open up a particular dataset.
				rdatasource2 = raster_drivers->auto_Open(filename, result);

				if (!rdatasource2.expired())
				{
					Geoweb3d::IGW3DRasterLayerCollection* layer_collection2 = rdatasource2.lock()->get_RasterLayerCollection();
					Geoweb3d::IGW3DRasterLayerWPtr raster_layer2 = layer_collection2->get_AtIndex(0);

					layer_col->add(raster_layer2);
				}
			}
		}

		
		layer_col->set_SortMode(Geoweb3d::IGW3DLayerHelperCollection::SortMode::RESOLUTION);

		
		Geoweb3d::IGW3DDataSourceCompositor* data_composite_util = sdk_context_->get_DataSourceCompositor();
		Geoweb3d::DataSourceCompositeParameters dsc_params;
		dsc_params.x_size = 900;
		dsc_params.y_size = 900;
		dsc_params.point_resolution = -1.0f;
		dsc_params.env = env;

		std::string ds_comp_name = "../examples/media/temp/composition.vrt";

		Geoweb3d::GW3DResult composite_result;
		Geoweb3d::IGW3DRasterDataSourceWPtr comp_ds = data_composite_util->create_Composite(
			ds_comp_name.c_str(),
			"EPSG:4267",
			layer_col,
			dsc_params,
			composite_result
		);


		if (Geoweb3d::Succeeded(composite_result))
		{
			std::string out_vectorized_contours = "../examples/media/temp/out_vectorized_contours.gpkg";
			Geoweb3d::IGW3DRasterDriverWPtr gpkg_driver = raster_drivers->get_Driver("GPKG");
			Geoweb3d::IGW3DContourUtility* contour_util = sdk_context_->get_ContourUtility();

			/*
			* //can use this api if not really interested in the intermediate data composition stage
			* 
				Geoweb3d::ContourCreationParameters contour_params;
				contour_params.elevation_interval = 20;
				contour_params.x_size = 900;
				contour_params.y_size = 900;
				contour_params.point_resolution = -1.0f;
				contour_params.env = env;
				contour_params.export_driver = gpkg_driver;
				Geoweb3d::IGW3DTaskPtr c_task = contour_util->create_Contours(out_vectorized_contours.c_str(), "EPSG:4267", layer_col, contour_params);
			*/

			Geoweb3d::IGW3DTaskPtr c_task = contour_util->create_Contours(out_vectorized_contours.c_str(), comp_ds, 15.0, gpkg_driver, nullptr);
			res = c_task->force_Completion();

			if (Geoweb3d::Succeeded(res))
			{

				Geoweb3d::GW3DResult open_contours_res;
				//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
				//having to guess which data source driver is needed to open up a particular dataset.
				Geoweb3d::IGW3DVectorDataSourceWPtr data_source = sdk_context_->get_VectorDriverCollection()->auto_Open(out_vectorized_contours.c_str(), open_contours_res);

				if (data_source.expired())
				{
					std::cout << "Could not load the following Vector Data Source\n";
				}
				else
				{
					Geoweb3d::IGW3DRasterDriverWPtr gtiff_driver = raster_drivers->get_Driver("GTiff");

					Geoweb3d::IGW3DVectorLayerCollection* vlyrcollection = data_source.lock()->get_VectorLayerCollection();
					vlyrcollection->reset();

					Geoweb3d::IGW3DVectorLayerWPtr layer;
					if (vlyrcollection->count())
					{
						layer = vlyrcollection->get_AtIndex(0);

						//rep the vectorized contours as colored-lines
						{
							Geoweb3d::IGW3DVectorRepresentationDriverWPtr colored_line_rep_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("ColoredLine");
							Geoweb3d::IGW3DPropertyCollectionPtr colored_line_properties = colored_line_rep_driver.lock()->get_PropertyCollection()->create_Clone();
							colored_line_properties->put_Property(Geoweb3d::Vector::ColoredLinesProperties::IndividualProperties::RED, 0.0f);
							colored_line_properties->put_Property(Geoweb3d::Vector::ColoredLinesProperties::IndividualProperties::GREEN, 0.0f);
							colored_line_properties->put_Property(Geoweb3d::Vector::ColoredLinesProperties::IndividualProperties::BLUE, 1.0f);

							Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
							params.page_level = 1;
							params.representation_default_parameters = colored_line_properties;
							Geoweb3d::IGW3DVectorRepresentationWPtr colored_line_rep_ = colored_line_rep_driver.lock()->get_RepresentationLayerCollection()->create(layer, params);
						}

						std::string out_rasterized_contours = "../examples/media/temp/out_rasterized_contours.tif";
						Geoweb3d::Vector::GW3DRasterizorParameters params;
						params.x_size = 600;
						params.y_size = 600;
						params.init_b = 128;
						params.no_data_r = 0;
						params.no_data_g = 0;
						params.no_data_b = 0;
						params.burn_r = 255;
						params.burn_g = 0;
						params.burn_b = 0;
						params.init_r = 0;
						params.init_g = 0;
						params.init_b = 0;
						params.export_driver = gtiff_driver;
						params.env = env;
						
						Geoweb3d::GW3DResult rasterization_result = layer.lock()->create_Rasterization(out_rasterized_contours.c_str(), params, nullptr);

						if (Geoweb3d::Succeeded(rasterization_result))
						{
							Geoweb3d::GW3DResult open_result;
							Geoweb3d::GW3DEnvelope rasterized_env;
							//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
							//having to guess which data source driver is needed to open up a particular dataset.
							Geoweb3d::IGW3DRasterDataSourceWPtr	 contour_raster_rep = sdk_context_->get_RasterDriverCollection()->auto_Open(out_rasterized_contours.c_str(), open_result);

							//check GW3DResult for details on a failure - however the rep being valid also signifies success
							auto rep_l = contour_raster_rep.lock();
							if (rep_l)
							{
								Geoweb3d::IGW3DRasterLayerCollection* layer_collection = contour_raster_rep.lock()->get_RasterLayerCollection();
								if (layer_collection && layer_collection->count() > 0)
								{
									auto imagery_layer = layer_collection->get_AtIndex(0);

									if (!imagery_layer.expired())
									{
										if (0) //show rasterized contours as imagery
										{
											if (0) //legacy way to show an overlay using "Imagery" representations - basically invalid value are not composited when no replacements are set
											{
												auto imagery_rep_driver = sdk_context_->get_RasterLayerRepresentationDriverCollection()->get_Driver("Imagery");
												if (!imagery_rep_driver.expired())
												{
													Geoweb3d::Imagery::ImageryLoadProperties imagery_properties;
													Geoweb3d::IGW3DPropertyCollectionPtr img_props = imagery_rep_driver.lock()->get_PropertyCollection()->create_Clone();
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_RED, 0.0f);
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_GREEN, 0.0f);
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_BLUE, 0.0f);
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::USER_DEFINED_INVALID_COLOR, true);
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::TOLERANCE_RANGE, 0.2f);

													Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
													params.representation_default_parameters = img_props;
													params.page_level = 5;
													params.priority = 3;
													params.representation_layer_activity = true;
													imagery_rep_driver.lock()->get_RepresentationLayerCollection()->create(imagery_layer, params);

													rasterized_env = imagery_layer.lock()->get_Envelope();
												}
												else
												{
													printf("Do Imagery driver - should never occur on a successful startup / initialization of the application\n");
												}
											}
											else
											{
												auto imagery_overlay_rep_driver = sdk_context_->get_RasterLayerRepresentationDriverCollection()->get_Driver("Imagery-Overlay");
												if (!imagery_overlay_rep_driver.expired())
												{
													//"Imagery-Overlay" was first designed to be primarily a time based overlay.  We will design out the need for always having to 
													//create a player for the case that a user intends the "Imagery-Overlay" to always just be on. 
													//For the time being a controller, and a player must be setup and enabled to visual a "Imagery-Overlay" representation
													Geoweb3d::IGW3DRasterTimeControllerWPtr controller = sdk_context_->get_GW3DRasterTimeController(imagery_layer, nullptr);
													Geoweb3d::IGW3DPlanetRasterTimePlayerWPtr player = sdk_context_->RegisterRasterTimeControllerWithPlanet(controller);
													player.lock()->put_Enabled(true);


													Geoweb3d::Imagery::ImageryLoadProperties imagery_properties;
													Geoweb3d::IGW3DPropertyCollectionPtr img_props = imagery_overlay_rep_driver.lock()->get_PropertyCollection()->create_Clone();
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::SCREEN_RES_MODE, true);
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::RESOLUTION_MAPPING, 1);
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_RED, 0.0f);
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_GREEN, 0.0f);
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_BLUE, 0.0f);
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::USER_DEFINED_INVALID_COLOR, true);
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::TOLERANCE_RANGE, 0.2f);
													img_props->put_Property(Geoweb3d::Raster::ImageryProperties::SAMPLER_ALGO, Geoweb3d::Raster::SamplerAlg::NEAREST_NEIGHBOR);

													Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
													params.representation_default_parameters = img_props;
													params.page_level = 5;
													params.priority = 0;
													params.representation_layer_activity = true;
													imagery_overlay_rep_driver.lock()->get_RepresentationLayerCollection()->create(imagery_layer, params);
												}
												else
												{
													printf("Do Imagery-Overlay driver - should never occur on a successful startup / initialization of the application\n");
												}
											}
										}
									}
									else
									{
										printf("layer collectiion valid but first layer is null - shouldn't ever occur with a valid data source.\n");
									}
								}
								else
								{
									printf("Empty layer collection - shouldn't ever occur with a valid data source.\n");
								}
							}
							else
							{
								printf("Could not represent Imagery data source\n");
							}
						}
					}
				}

				//This section does the data export of what was composited - outputs into a GTIFF format
				//note : example doesn't do anything with the output - just show it can be exported
				if (!comp_ds.lock())
				{
					std::cout << "Could not load the following Vector Data Source\n";
				}
				else
				{
					Geoweb3d::IGW3DRasterDriverWPtr gtiff_driver = raster_drivers->get_Driver("GTiff");

					std::string out_vrt_contours_tif = "../examples/media/temp/composition_out.tif";
					if (comp_ds.lock()->create_Copy(gtiff_driver, nullptr, out_vrt_contours_tif.c_str(), nullptr))
					{
						std::cout << "SUCCESS: outputted vrt to tif\n";
					}
					else
					{
						std::cout << "FAILURE: outputted vrt to tif succesfully\n";
					}
				}
			}
			else
			{
				printf("FAILED creating %s\n", out_vectorized_contours.c_str());
			}
		}
	}

	camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);
	double longitude, latitude;
	longitude = (env.MinX + env.MaxX) * 0.5;
	latitude = (env.MinY + env.MaxY) * 0.5;
	camera_controller_->put_Location(longitude, latitude);
	camera_controller_->put_Elevation(9000);
	camera_controller_->put_Rotation(0, 89, 0);

	navHelper_->put_HomePosition(camera_);
	return true;
}

Geoweb3d::IGW3DWindowWPtr MiniEngine::Create3DWindow(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback* window_events)
{
	Geoweb3d::IGW3DWindowCollection* wcol = sdk_context->get_WindowCollection();

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

	Geoweb3d::IGW3DCameraCollection* cameracollection = windowptr.lock()->get_CameraCollection();
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

void MiniEngine::OnDraw2D(const Geoweb3d::IGW3DCameraWPtr& camera) {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the pre draw work operation. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void MiniEngine::DoPreDrawWork()
{
	const Geoweb3d::GW3DPoint* mouse_geo = 0;
	if (navHelper_->getMousePixelLocation(&mouse_geo))
	{
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the post draw work operation. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void MiniEngine::DoPostDrawWork()
{
	if (!line_segment_.expired())
	{
		if (line_segment_.lock()->get_Enabled())
		{
			Geoweb3d::IGW3DLineSegmentIntersectionReportCollection* intersection_report_collection = line_segment_.lock()->get_IntersectionReportCollection();

			Geoweb3d::IGW3DLineSegmentIntersectionReport* intersection_report;

			intersection_report_collection->reset();

			PathIntersectionTestStream streamer;
			bool found_one = false;
			while (intersection_report_collection->next(&intersection_report))
			{
				Geoweb3d::IGW3DVectorRepresentationWPtr rep = intersection_report->get_VectorRepresentation();
				const char* vector_driver_name = intersection_report->get_VectorRepresentation().lock()->get_Driver().lock()->get_Name();
				const char* vector_layer_name = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer().lock()->get_Name();

				Geoweb3d::IGW3DVectorLayerWPtr icon_vector_layer = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer();

				Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection* details_collection = intersection_report->get_IntersectionDetailCollection();

				Geoweb3d::IGW3DLineSegmentIntersectionDetail* intersection_detail;

				details_collection->reset();

				//collect details for the intersected feature(s)
				while (details_collection->next(&intersection_detail))
				{
					printf("FID HIT %d, at DIST %5.5f\n", intersection_detail->get_ObjectID(), intersection_detail->get_IntersectionDistance());
					streamer.insert(intersection_detail->get_ObjectID(), intersection_detail->get_IntersectionDistance());

					Geoweb3d::GW3DPoint tri_pt0;
					Geoweb3d::GW3DPoint tri_pt1;
					Geoweb3d::GW3DPoint tri_pt2;
					intersection_detail->get_TriangleIntersected(tri_pt0, tri_pt1, tri_pt2);
					printf("TRI0 %6.6f, %6.6f, %6.6f\n", tri_pt0.get_X(), tri_pt0.get_Y(), tri_pt0.get_Z());

					//just showing we can also access the actual intersection point
					const Geoweb3d::GW3DPoint* hit_point = intersection_detail->get_IntersectionPoint();
				}

				if (streamer.count())
				{
					// process the results
					icon_vector_layer.lock()->Stream(&streamer);
					
				}
			}

			if (!found_one)
			{
				//unset any previous fid 
			}
		}

		//one shot
		line_segment_.lock()->put_Enabled(false);
	}

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

LRESULT CALLBACK MiniEngine::ProcessLowLevelEvent(OSWinHandle hWndParent, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	// We don't forward the WM_CLOSE message to prevent the internal SDK from closing us (click on X)
	if (message == WM_CLOSE)
	{
		printf("ProcessLowLevelEvent: WM_Close detected and prevented!\n");
		return 0;
	}
	return 1;
}

