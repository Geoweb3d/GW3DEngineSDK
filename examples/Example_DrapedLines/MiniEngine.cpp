#include "MiniEngine.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/IGW3DClampRadius.h"
#include "engine/IGW3DMediaCenter.h"

#include "engine/IGW3DSceneGraphContext.h"
#include "engine/IGW3DImageCollection.h"
#include "engine/IGW3DImage.h"
#include "engine/GW3DRaster.h"

#include "GeoWeb3dCore/SystemExports.h"

#include <deque>
#include <set>
#include <map>
#include <iostream>
#include "core/IGW3DFinalizationToken.h"

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

	virtual bool next(int64_t *ppVal)
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



Geoweb3d::IGW3DVectorRepresentationWPtr MiniEngine::RepresentAsDrapedLine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer, Geoweb3d::IGW3DPropertyCollectionPtr defaults, int default_top_texture_id)
{
	Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver("DrapedLine");

	Geoweb3d::GW3DResult validitychk;

	if (!Geoweb3d::Succeeded(validitychk = driver.lock()->get_CapabilityToRepresent(layer)))
	{
		printf("Asked to represent a vector layer with a representation, but the layer is not compatible!\n");
	}
	else
	{
		// prints the values the layers has to help the user know what they can attribute map into the representation
		const Geoweb3d::IGW3DDefinitionCollection* def = layer.lock()->get_AttributeDefinitionCollection();

		printf("Attribute Names within the layer: [%s] you can map to the a draped line propery: \n", layer.lock()->get_Name());
		for (unsigned i = 0; i < def->count(); ++i)
		{
			printf("\t[%s]\n", def->get_AtIndex(i)->property_name);
		}
		
		defaults->put_Property(defaults->get_DefinitionCollection()->get_IndexByName("TEXTURE_PALETTE_INDEX"), default_top_texture_id);
		//defaults->put_Property(defaults->get_DefinitionCollection()->get_IndexByName("LABEL"), "TEST");

		Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
		params.representation_default_parameters = defaults;
		params.page_level = 8;
		return driver.lock()->get_RepresentationLayerCollection()->create(layer, params);
	}

	return Geoweb3d::IGW3DVectorRepresentationWPtr();
}


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
		current_fid_ = -1;

		/*struct prop_state
		{
			float width;
			float texture_blend_;
			float red_;
			float green_;
			float blue_;
			int line_width;
			Geoweb3d::Vector::OutlineMode outline_mode;
		};*/

		default_state_ = {
			100.0,
			1.0,
			0.0,//red
			0.5,
			0.0,
			4,
			Geoweb3d::Vector::OutlineMode::OUTLINE_ON,
		};

		specific_state_ = {
			100.0,
			1.0,
			0.0,//red
			0.5,
			0.0,
			4,
			Geoweb3d::Vector::OutlineMode::OUTLINE_ON,
		};
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
	void MiniEngine::PopulateCurrentStateProps(const long current_fid)
	{
		const Geoweb3d::IGW3DPropertyCollection* current_props = draped_line_.lock()->get_PropertyCollection(current_fid_);

		if (current_props)
		{
			specific_state_.width_ = current_props->get_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::WIDTH);
			specific_state_.texture_blend_ = current_props->get_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND);
			specific_state_.red_ = current_props->get_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::RED);
			specific_state_.green_= current_props->get_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::GREEN);
			specific_state_.blue_= current_props->get_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::BLUE);
			specific_state_.line_width = current_props->get_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::LINE_WIDTH);
			specific_state_.outline_mode = (Geoweb3d::Vector::OutlineMode)( (int)current_props->get_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::OUTLINE_MODE));
		}
		else
		{
			specific_state_.width_						= default_state_.width_;
			specific_state_.texture_blend_				= default_state_.texture_blend_;
			specific_state_.red_						= default_state_.red_;
			specific_state_.green_						= default_state_.green_;
			specific_state_.blue_						= default_state_.blue_;
			specific_state_.line_width					= default_state_.line_width;
			specific_state_.outline_mode				= default_state_.outline_mode;
		}
	}

	void MiniEngine::SetCurrentProps(Geoweb3d::IGW3DPropertyCollectionPtr current)
	{
		current->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::WIDTH, specific_state_.width_);
		current->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND, specific_state_.texture_blend_);
		current->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::RED, specific_state_.red_);
		current->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::GREEN, specific_state_.green_);
		current->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::BLUE, specific_state_.blue_);
		current->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::LINE_WIDTH, specific_state_.line_width);
		current->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::OUTLINE_MODE, specific_state_.outline_mode);

	};

	void MiniEngine::SetDefaultProps(Geoweb3d::IGW3DPropertyCollectionPtr defaults)
	{
		defaults->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::WIDTH, default_state_.width_);
		defaults->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND, default_state_.texture_blend_);
		defaults->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::RED, default_state_.red_);
		defaults->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::GREEN, default_state_.green_);
		defaults->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::BLUE, default_state_.blue_);
		defaults->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::LINE_WIDTH, default_state_.line_width);
		defaults->put_Property(Geoweb3d::Vector::DrapedLineProperties::IndividualProperties::OUTLINE_MODE, default_state_.outline_mode);
		
	};

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

		Geoweb3d::GW3DResult res;
		//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
		//having to guess which data source driver is needed to open up a particular dataset.
		Geoweb3d::IGW3DVectorDataSourceWPtr data_source = sdk_context_->get_VectorDriverCollection()->auto_Open("../examples/media/DC_LINES.shp", res );

		if (data_source.expired())
		{
			std::cout << "Could not load the following Vector Data Source\n";
		}

		Geoweb3d::IGW3DVectorLayerCollection *vlyrcollection = data_source.lock()->get_VectorLayerCollection();
		vlyrcollection->reset();

		Geoweb3d::IGW3DVectorLayerWPtr layer;
		Geoweb3d::GW3DEnvelope env;

		int roof_propertycollection_id = -1;

		//Order is not preserved.
		Geoweb3d::IGW3DImageCollectionPtr imagepalette = sdk_context_->get_SceneGraphContext()->create_ImageCollection();

		{
			//If you add a duplicate, it will return the same IGW3DImage
			Geoweb3d::IGW3DImageWPtr image = imagepalette->create("../examples/media/Texture/Roads/road_texture.jpg");
			roof_propertycollection_id = image.lock()->get_PropertyCollectionID();
		}

		Geoweb3d::IGW3DImageWPtr imagetest;
		imagepalette->reset();
		while (imagepalette->next(&imagetest))
		{
			printf("file: [%s], userdata: [%i]\n", imagetest.lock()->get_ImageName(), static_cast<int>( reinterpret_cast<std::int64_t>(imagetest.lock()->get_UserData() ) ) );
		}

		Geoweb3d::IGW3DFinalizationTokenPtr reptoken = imagepalette->create_FinalizeToken();

		while (vlyrcollection->next(&layer))
		{
			Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("DrapedLine");
			properties_ = driver.lock()->get_PropertyCollection()->create_Clone();

			SetDefaultProps(properties_);
			//for a shapefile, we only have 1 layer.. but.. just beware this
			//is would overwrite if it has more than 1 layer
			draped_line_ = RepresentAsDrapedLine(sdk_context_, layer, properties_, roof_propertycollection_id);

			env.merge(layer.lock()->get_Envelope());
		}

		if (!draped_line_.expired())
		{
			draped_line_.lock()->put_GW3DFinalizationToken(reptoken);

			line_segment_ = sdk_context_->get_LineSegmentIntersectionTestCollection()->create();

			if (!line_segment_.expired())
			{
				line_segment_.lock()->get_VectorRepresentationCollection()->add(draped_line_);
			}

			/*billboard_font_ = Geoweb3d::IGW3DFont::create("Arial.ttf", 12, Geoweb3d::IGW3DFont::NORMAL, false);
			draped_line_.lock()->put_Font(billboard_font_);

			const Geoweb3d::IGW3DPropertyCollection* label_props = draped_line_.lock()->get_LabelProperties();
			if (label_props)
			{
				Geoweb3d::IGW3DPropertyCollectionPtr updated_label_props = label_props->create_Clone();
				updated_label_props->put_Property(Geoweb3d::Vector::LabelProperties::FOREGROUND_COLOR_RED, 1.0);
				updated_label_props->put_Property(Geoweb3d::Vector::LabelProperties::FOREGROUND_COLOR_GREEN, 1.0);
				updated_label_props->put_Property(Geoweb3d::Vector::LabelProperties::FOREGROUND_COLOR_BLUE, 0.0);
				updated_label_props->put_Property(Geoweb3d::Vector::LabelProperties::FOREGROUND_COLOR_ALPHA, 1.0);
				updated_label_props->put_Property(Geoweb3d::Vector::LabelProperties::BACKGROUND_COLOR_RED, 0.0);
				updated_label_props->put_Property(Geoweb3d::Vector::LabelProperties::BACKGROUND_COLOR_GREEN, 0.0);
				updated_label_props->put_Property(Geoweb3d::Vector::LabelProperties::BACKGROUND_COLOR_BLUE, 0.0);
				updated_label_props->put_Property(Geoweb3d::Vector::LabelProperties::BACKGROUND_COLOR_ALPHA, 1.0);
				draped_line_.lock()->put_LabelProperties(updated_label_props);
			}*/
		}

		if( CreateElevationRep( sdk_context_, "../examples/media/NED 10-meter DC.tif", 6, 0, true, 6.0 ).expired())
		{
			printf("CreateElevationRep Error\n");
		}


		camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);
		double longitude, latitude;
		longitude = (env.MinX + env.MaxX) * 0.5;
		latitude = (env.MinY + env.MaxY) * 0.5;
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
			//go back to using the date and time of this computer
			 //camera.lock()->get_DateTime()->put_isUseComputerDate(true);
			//camera.lock()->get_DateTime()->put_isUseComputerTime(true);

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
					const char* vector_driver_name = intersection_report->get_VectorRepresentation().lock()->get_Driver().lock()->get_Name();
					const char* vector_layer_name = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer().lock()->get_Name();

					Geoweb3d::IGW3DVectorLayerWPtr icon_vector_layer = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer();

					Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection* details_collection = intersection_report->get_IntersectionDetailCollection();

					Geoweb3d::IGW3DLineSegmentIntersectionDetail* intersection_detail;

					details_collection->reset();

					//collect details for the intersected feature(s)
					while (details_collection->next(&intersection_detail))
					{
						streamer.insert(intersection_detail->get_ObjectID(), intersection_detail->get_IntersectionDistance());

						//just showing we can also access the actual intersection point
						const Geoweb3d::GW3DPoint* hit_point = intersection_detail->get_IntersectionPoint();
					}

					if (streamer.count())
					{
						// process the results
						icon_vector_layer.lock()->Stream(&streamer);

						//should dig in and get the closest but using first one in the set for now
						current_fid_ = streamer.get_AtIndex(0);
						found_one = true;
						PopulateCurrentStateProps(current_fid_);
					}

				}
				if (!found_one)
				{
					current_fid_ = -1;
				}
			}

			//one shot
			line_segment_.lock()->put_Enabled(false);
		}

		if (props_dirty_)
		{

			if (current_fid_ >= 0)
			{
				SetCurrentProps(properties_);
				if(!draped_line_.expired())
					draped_line_.lock()->put_PropertyCollection(current_fid_, properties_);
			}
			else
			{
				SetDefaultProps(properties_);
				if (!draped_line_.expired())
					draped_line_.lock()->put_PropertyCollection(properties_);
			}
			props_dirty_ = false;
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

		switch (win_event.Type)
		{
			case win_event.MouseButtonPressed:
			{
				switch (win_event.MouseButton.button)
				{
					case Geoweb3d::Mouse::Left:
					{
						if (!line_segment_.expired())
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
					}
					break;

					default:
						break;
				};//End of Switch
			}
			break;

			case win_event.KeyPressed:
			{

				prop_state* state;
				if (current_fid_ >= 0)
				{
					state = &specific_state_;
				}
				else
				{
					state = &default_state_;
				}

				switch (win_event.Key.code)
				{
					case Geoweb3d::Key::A:
					{
						if (win_event.Key.shift)
						{
							state->width_ -= 1.0f;
							if (state->width_ < 0.0f)
								state->width_ = 0.0f;
						}
						else
						{
							state->width_ += 1.0f;
							if (state->width_ > 100.0f)
								state->width_ = 100.0f;
						}
						printf("WIDTH: %3.1f\n", state->width_);
						props_dirty_ = true;
						filter_out = true;
					}
					break;

					case Geoweb3d::Key::S:
					{
						if (win_event.Key.shift)
						{
							state->texture_blend_ -= 0.1f;
							if (state->texture_blend_ < 0.0f)
								state->texture_blend_ = 0.0f;
						}
						else
						{
							state->texture_blend_ += 0.1f;
							if (state->texture_blend_ > 1.0f)
								state->texture_blend_ = 1.0f;
						}
						printf("TEXTURE_TO_COLOR_BLEND: %1.1f\n", state->texture_blend_);
						props_dirty_ = true;
						filter_out = true;
					}
					break;

					case Geoweb3d::Key::D:
					{
						if (win_event.Key.shift)
						{
							state->green_ -= 0.1f;
							if (state->green_ < 0.0f)
								state->green_ = 0.0f;
						}
						else
						{
							state->green_ += 0.1f;
							if (state->green_ > 1.0f)
								state->green_ = 1.0f;
						}
						printf("GREEN COMPONENT: %1.1f\n", state->green_);
						props_dirty_ = true;
						filter_out = true;
					}
					break;

					case Geoweb3d::Key::F:
					{
						if (win_event.Key.shift)
						{
							state->line_width -= 1;
							if (state->line_width < 0)
								state->line_width = 0;
						}
						else
						{
							state->line_width += 1;
							if (state->line_width > 32)
								state->line_width = 32;
						}
						printf("LINE_WIDTH: %d\n", state->line_width);
						props_dirty_ = true;
						filter_out = true;
					}
					break;

					case Geoweb3d::Key::G:
					{
						if (state->outline_mode == Geoweb3d::Vector::OutlineMode::OUTLINE_ON)
						{
							state->outline_mode = Geoweb3d::Vector::OutlineMode::OUTLINE_OFF;
							printf("OUTLINE_OFF\n");
						}
						else
						{
							state->outline_mode = Geoweb3d::Vector::OutlineMode::OUTLINE_ON;
							printf("OUTLINE_ON\n");
						}

						props_dirty_ = true;
						filter_out = true;
					}
					break;

					case Geoweb3d::Key::H:
					{
						printf("Changeable Properties: Click off a feature to change the defaults properties. Note: once an individual feature property has changed\n");
						printf("the defaults will not apply to that feature. ");
						printf("\n"
							"A - INCREASE WIDTH (W/SHIFT DECREASE)		\n"
							"S - INCREASE TEXTURE_TO_COLOR_BLEND (W/SHIFT DECREASE)	\n"
							"D - INCREASE GREEN COMPONENT (W/SHIFT DECREASE)						\n"
							"F - INCREASE SCREEN SPACE LINE WIDTH (W/SHIFT DECREASE)						\n"
							"G - TOGGLE OUTLINE CONTRAST BORDER						\n"
							"H - THIS MENU \n"
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
		};//End of Switch

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

