#include "MiniEngine.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/GW3DRaster.h"
#include "engine/GW3DVector.h"
#include "Geoweb3dCore/LayerParameters.h"

#include "GeoWeb3dCore/SystemExports.h"

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <param name="sdk_context">	Context for the sdk. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	MiniEngine::MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context) 
		: sdk_context_(sdk_context) 
		, navHelper_(new NavigationHelper())
		, selected_fid_(-1)
	{

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Destructor. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	MiniEngine::~MiniEngine() 
	{
		delete navHelper_;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Updates this object. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
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
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
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

	void MiniEngine::SetExtrudedPolyDefaultProps(Geoweb3d::IGW3DPropertyCollectionPtr defaults)
	{
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::TRANSLATION_Z_OFFSET, 20.0);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::EXTRUSION_HEIGHT, 20.0);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::RENDER_BOTTOM, true);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::RED, 0.0);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::GREEN, 0.0);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::BLUE, 1.0);

	};

	void MiniEngine::SetColoredLineDefaultProps(Geoweb3d::IGW3DPropertyCollectionPtr defaults)
	{
		defaults->put_Property(Geoweb3d::Vector::ColoredLinesProperties::IndividualProperties::TRANSLATION_Z_OFFSET, 1.0);
		defaults->put_Property(Geoweb3d::Vector::ColoredLinesProperties::IndividualProperties::RED, 0.0);
		defaults->put_Property(Geoweb3d::Vector::ColoredLinesProperties::IndividualProperties::GREEN, 0.0);
		defaults->put_Property(Geoweb3d::Vector::ColoredLinesProperties::IndividualProperties::BLUE, 1.0);

	};


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Loads a data. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <param name="rootpath">	The rootpath. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool MiniEngine::LoadData(const char *rootpath)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		///
		///	Open a shapefile - we will stream back out the data of this shape file to populate a spatial 
		///	data structure.  This data structure will then be used as a data source for a different homespun datasource
		/// that the you will manage with the use of special callbacks (i.e. IGW3DVectorPipelineCallbacks)
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////

		//auto open a shape file (will auto know to use the shape file driver)
		Geoweb3d::GW3DResult res;

		//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
		//having to guess which data source driver is needed to open up a particular dataset.
		Geoweb3d::IGW3DVectorDataSourceWPtr data_source = sdk_context_->get_VectorDriverCollection()->auto_Open("../examples/media/Building Footprints.shp", res);

		if (data_source.expired())
		{
			std::cout << "Could not load the following Vector Data Source\n";
			return false;
		}

		Geoweb3d::IGW3DVectorLayerCollection* vlyrcollection_ds = data_source.lock()->get_VectorLayerCollection(true);
		Geoweb3d::IGW3DVectorLayerWPtr ds_layer;
		Geoweb3d::IGW3DDefinitionCollectionPtr field_definition;
		Geoweb3d::GeometryType geom_type;

		if (vlyrcollection_ds->next(&ds_layer))
		{
			//Here we will stream the shape file layer into our database to be used later. 
			ds_layer.unsafe_get()->Stream(reinterpret_cast<Geoweb3d::IGW3DVectorLayerStream*>(&spatial_db_));

			geom_type = ds_layer.unsafe_get()->get_GeometryType();
			field_definition = ds_layer.unsafe_get()->get_AttributeDefinitionCollection(true)->create_Clone();
			env_.merge(ds_layer.unsafe_get()->get_Envelope());
			//don't forget to set the bounds for IGW3DVectorPipelineCallbacks - as this acts as an optimization
			spatial_db_.SetBounds(env_);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		///
		///	At this point we can close the shapefile used to populate our database, as we no longer need it. 
		/// Or we can show how we can represent an 'internal' datasource as well.
		/// We will represent this shapefile as 'colored lines'
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool represent_internal_datasource_as_well = true;
		if (!represent_internal_datasource_as_well)
		{
			close_VectorDataSource(data_source);
		}

		
		//because we here are creating a datasource we need to pick the driver ourselves
		//this will be the datasource were we supply our own data when the callback requests features within it's bounds.
		Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context_->get_VectorDriverCollection(true)->get_Driver("Geoweb3d_Datasource");
		Geoweb3d::IGW3DVectorDataSourceWPtr data_source_callback = driver.unsafe_get()->get_VectorDataSourceCollection()->open("callback datasource", res);

		if (data_source_callback.expired())
		{
			std::cout << "Could not load the following Vector Data Source\n";
			return false;
		}

		Geoweb3d::IGW3DVectorLayerCollection* vlyrcollection_ds_w_cb = data_source_callback.lock()->get_VectorLayerCollection(true);


		////////////////////////////////////////////////////////////////////////////////////////////////////
		///
		///	When we create a layer we give the create proxy call the callback object
		/// Here we also supply the layer attributes - that can be used to do attribute mappings later. 
		/// Will use the attribute definition that the original shape file had (but can be anything).
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////
		callback_layer_ = vlyrcollection_ds_w_cb->create_Proxy("callback layer", geom_type, field_definition, &spatial_db_);
		
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		///
		/// Now our datasource is set - time to choose a represenation 
		/// Remember that some representation only work with certain layer datatypes. 
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Geoweb3d::IGW3DVectorRepresentationDriverWPtr extruded_poly_rep_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("ExtrudedPolygon");

		Geoweb3d::GW3DResult validitychk;

		if (!Geoweb3d::Succeeded(validitychk = extruded_poly_rep_driver.lock()->get_CapabilityToRepresent(callback_layer_)))
		{
			printf("not able to mix this geometry type with the rep..\n");
			return false;
		}

		
		//get the default representation properties and set them to some reasonable defaults. (if we don't want the system defaults). 
		Geoweb3d::IGW3DPropertyCollectionPtr properties_ = extruded_poly_rep_driver.lock()->get_PropertyCollection()->create_Clone();
		SetExtrudedPolyDefaultProps(properties_);
		Geoweb3d::IGW3DVectorLayerWPtr ds_w_cb_layer;

		if (vlyrcollection_ds_w_cb->next(&ds_w_cb_layer))
		{
			Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
			params.representation_default_parameters = properties_;
			params.page_level = 3;
			params.cb = &attribute_mapping_callback_; // used for attribute mapping
			extruded_poly_rep_ = extruded_poly_rep_driver.lock()->get_RepresentationLayerCollection()->create(ds_w_cb_layer, params);

		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		///
		///	He we do the colored line representation of the original 'internal' datasource 
		/// We show this just as a reference.  As this example app is mainly to show the architecture
		/// of a 'call back' datasource. 
		/// (i.e. note picking, tooltip, and context menue are not hooked up to work with this representation).
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////
		if (represent_internal_datasource_as_well)
		{

			Geoweb3d::IGW3DVectorRepresentationDriverWPtr colored_line_rep_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("ColoredLine");
			Geoweb3d::GW3DResult validitychk;

			if (!Geoweb3d::Succeeded(validitychk = colored_line_rep_driver.lock()->get_CapabilityToRepresent(callback_layer_)))
			{
				printf("not able to mix this geometry type with the rep..\n");
				return S_FALSE;
			}


			//get the default representation properties and set them to some reasonable defaults. (if we don't want the system defaults). 
			Geoweb3d::IGW3DPropertyCollectionPtr properties_ = colored_line_rep_driver.lock()->get_PropertyCollection()->create_Clone();
			SetColoredLineDefaultProps(properties_);

			Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
			params.representation_default_parameters = properties_;
			params.page_level = 3;
			line_rep_ = colored_line_rep_driver.lock()->get_RepresentationLayerCollection()->create(ds_layer, params);


		}

		if (!extruded_poly_rep_.expired())
		{
			line_segment_ = sdk_context_->get_LineSegmentIntersectionTestCollection()->create();
			if (!line_segment_.expired())
			{
				line_segment_.lock()->get_VectorRepresentationCollection()->add(extruded_poly_rep_);
			}
		}


		camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);
		double longitude, latitude;
		longitude = (env_.MinX + env_.MaxX) * 0.5;
		latitude = (env_.MinY + env_.MaxY) * 0.5;
		camera_controller_->put_Location(longitude, latitude);
		camera_controller_->put_Elevation(3000);
		camera_controller_->put_Rotation(0, 80, 0);

		navHelper_->put_HomePosition(camera_);
		return true;
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

		auto lock_cam = camera.lock();
		if (lock_cam)
		{
			Geoweb3d::IGW3DEnvironmentEffects* env_effects = lock_cam->get_EnvironmentEffects();
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
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void MiniEngine::DoPreDrawWork()
	{
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Executes the post draw work operation. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void MiniEngine::DoPostDrawWork()
	{
		if (!line_segment_.expired())
		{
			if (line_segment_.lock()->get_Enabled())
			{

				ClearSelected();

				Geoweb3d::IGW3DLineSegmentIntersectionReportCollection* intersection_report_collection = line_segment_.lock()->get_IntersectionReportCollection();

				Geoweb3d::IGW3DLineSegmentIntersectionReport* intersection_report;

				intersection_report_collection->reset();

				while (intersection_report_collection->next(&intersection_report))
				{
					const char* vector_driver_name = intersection_report->get_VectorRepresentation().lock()->get_Driver().lock()->get_Name();
					const char* vector_layer_name = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer().lock()->get_Name();

					//Geoweb3d::IGW3DVectorLayerWPtr icon_vector_layer = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer();

					Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection* details_collection = intersection_report->get_IntersectionDetailCollection();

					Geoweb3d::IGW3DLineSegmentIntersectionDetail* intersection_detail;

					details_collection->reset();

					//collect details for the intersected feature(s)
					while (details_collection->next(&intersection_detail))
					{
						/*std::cout << " Feature Info: <id : " << intersection_detail->get_ObjectID() << " > "
							<< " < Intersection Distance: " << intersection_detail->get_IntersectionDistance() << " >" << std::endl;*/

						SelectById(intersection_detail->get_ObjectID());

						//only allowing one selection (if more are needed - need to track history or rep properties - in order to delect back to there previous states).
						break;
						//just showing we can also access the actual intersection point
						//const Geoweb3d::GW3DPoint* hit_point = intersection_detail->get_IntersectionPoint();
					}
				}
			}

			//one shot
			line_segment_.lock()->put_Enabled(false);
		}
	}

	void MiniEngine::ClearSelected()
	{
		if (!extruded_poly_rep_.expired())
		{
			if(selected_fid_ > 0)
			{
				if (previous_rep_props_)
				{
					extruded_poly_rep_.lock()->put_PropertyCollection(selected_fid_, previous_rep_props_);
				}
				else
				{
					extruded_poly_rep_.lock()->put_PropertyCollection(selected_fid_, nullptr);
				}
			}
			selected_fid_ = -1;
		}
	}

	void MiniEngine::SelectById(long fid)
	{
		ClearSelected();

		if (!extruded_poly_rep_.expired())
		{
			const Geoweb3d::IGW3DPropertyCollection* prev_props = extruded_poly_rep_.lock()->get_PropertyCollection(fid);
			if (prev_props)
			{
				previous_rep_props_ = prev_props->create_Clone();
			}
			else
			{
				previous_rep_props_.reset();
			}

			Geoweb3d::IGW3DPropertyCollectionPtr props = extruded_poly_rep_.lock()->get_PropertyCollection()->create_Clone();

			if (props)
			{
				props->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::RED, 1.0);
				props->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::BLUE, 0.0);
				props->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::TRANSLATION_Z_OFFSET, 30.0);
				extruded_poly_rep_.lock()->put_PropertyCollection(fid, props);

				BaseDataPtr datum = spatial_db_.get_DataItemRefereence(fid);

				Geoweb3d::IGW3DStringPtr gw3d_str;
				printf("FID: %d\n", fid);
				spatial_db_.get_Name(fid, gw3d_str);
				printf("NAME: %s\n", gw3d_str->c_str());
				spatial_db_.get_Function(fid, gw3d_str);
				printf("Function: %s\n", gw3d_str->c_str());
				

				selected_fid_ = fid;
			}

			
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	OS event system, basically a helper off ProcessLowLevelEvent. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <param name="win_event">	The window event. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void MiniEngine::ProcessEvent(const Geoweb3d::WindowEvent& win_event)
	{

		bool filter_out = false;

		switch (win_event.Type)
		{
			case win_event.MouseButtonPressed:
			{
				switch (win_event.MouseButton.button)
				{
					case Geoweb3d::Mouse::Left:
					{
						//configure the pick ray
						Geoweb3d::GW3DPoint ray_start;
						Geoweb3d::GW3DPoint ray_end;

						int mouse_x = win_event.MouseButton.x;
						int mouse_y = win_event.MouseButton.y;

						Geoweb3d::IGW3DCameraController* camera_controller = camera_.lock()->get_CameraController();

						camera_controller->get_Ray(mouse_x, mouse_y, ray_start, ray_end);

						const Geoweb3d::GW3DPoint* cam_pos = camera_controller->get_Location();

						double cam_x = cam_pos->get_X();
						double cam_y = cam_pos->get_Y();
						double cam_z = cam_pos->get_Z();

						double r_startx = ray_start.get_X();
						double r_starty = ray_start.get_Y();
						double r_startz = ray_start.get_Z();

						double r_endx = ray_end.get_X();
						double r_endy = ray_end.get_Y();
						double r_endz = ray_end.get_Z();

						line_segment_.lock()->put_StartEnd(*cam_pos, ray_end);

						line_segment_.lock()->put_Enabled(true); //only do a one shot

						filter_out = false;
					}
					break;

					default:
					break;
				};//End of Switch
			}
			break;

			case win_event.KeyPressed:
			{
				switch (win_event.Key.code)
				{
					case Geoweb3d::Key::A:
					{
					}
					break;

					case Geoweb3d::Key::S:
					{
						
					}
					break;

					case Geoweb3d::Key::D:
					{
						
					}
					break;

					case Geoweb3d::Key::F:
					{
						
					}
					break;

					default:
					break;
				};//End of switch
			}
			break;

			default:
			break;
		};//End of Switch

		if (!filter_out)
		{
			navHelper_->ProcessEvent(win_event, window_);
		}

	}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	OS event system, raw data directly from the windows message pump. </summary>
		///
		/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
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

