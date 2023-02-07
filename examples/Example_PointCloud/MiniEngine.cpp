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
	int64_t	id;
	double	intersection_distance;

	FeatureInfo()
		:id(-1), intersection_distance(0.0)
	{}

	FeatureInfo(int64_t object_id, double distance = 0., int tex_prop_index = 0)
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
	void insert(int64_t object_id, double intersection_distance)
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
	bool find(int64_t object_id, FeatureInfo& info)
	{
		auto& itr = feature_info_set.find(object_id);
		if (itr != feature_info_set.end())
		{
			info = *itr;
			return true;
		}
		return false;
	}

	// When IGW3DVectorLayer::Stream is called with this class, the SDK will call OnStream for each requested feature
	virtual bool OnStream(Geoweb3d::IGW3DVectorLayerStreamResult* result)
	{
		// Request the attibute data for the current feature
		const Geoweb3d::IGW3DAttributeCollection* attribute_collection = result->get_AttributeCollection();

		if (!attribute_collection)
		{
			printf(" IGW3DAttributeCollection is null\n");
			return false;
		}
		// Query the object id (or feature id) for the current feature
		int64_t object_id = result->get_ObjectID();

		FeatureInfo info;
		if (find(object_id, info))
		{
			int64_t node_id = object_id / 10000000;
			int64_t pt_id = object_id - node_id * 10000000;

			std::cout << " Feature Info: <id : " << object_id << " >  < Intersection Distance: " << info.intersection_distance << " >" << std::endl;
			std::cout << " Node index: <id : " << node_id << " >  < Point index: " << pt_id << " >" << std::endl;

			static const unsigned red_idx = attribute_collection->get_DefinitionCollection()->get_IndexByName("ColorRed");
			static const unsigned green_idx = attribute_collection->get_DefinitionCollection()->get_IndexByName("ColorGreen");
			static const unsigned blue_idx = attribute_collection->get_DefinitionCollection()->get_IndexByName("ColorBlue");
			static const unsigned intensity_idx = attribute_collection->get_DefinitionCollection()->get_IndexByName("Intensity");
			static const unsigned return_idx = attribute_collection->get_DefinitionCollection()->get_IndexByName("Return");
			static const unsigned classification_idx = attribute_collection->get_DefinitionCollection()->get_IndexByName("Classification");

			const float color_r = attribute_collection->get_Property(red_idx);
			const float color_g = attribute_collection->get_Property(green_idx);
			const float color_b = attribute_collection->get_Property(blue_idx);
			const float intensity = attribute_collection->get_Property(intensity_idx);
			const int return_num = attribute_collection->get_Property(return_idx);
			const int classification = attribute_collection->get_Property(classification_idx);

			std::cout << " Red: " << color_r << std::endl;
			std::cout << " Green: " << color_g << std::endl;
			std::cout << " Blue: " << color_b << std::endl;
			std::cout << " Intensity: " << intensity << std::endl;
			std::cout << " Return: " << return_num << std::endl;
			std::cout << " Classification: " << classification << std::endl;
		}
		else
		{
			printf("The feature being streamed I did not request\n");
		}
		return true;
	}

	virtual bool OnError()
	{
		return false;
	}

	virtual unsigned long count() const
	{
		return (unsigned long)feature_info_set.size();
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



Geoweb3d::IGW3DVectorRepresentationWPtr MiniEngine::RepresentAsPointCloud(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer, Geoweb3d::IGW3DPropertyCollectionPtr defaults)
{
	Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver("PointCloud");

	// prints the values the layers has to help the user know what they can attribute map into the representation
	const Geoweb3d::IGW3DDefinitionCollection* def = layer.lock()->get_AttributeDefinitionCollection();

	if (def)
	{
		printf("Attribute Names within the layer: [%s]: \n", layer.lock()->get_Name());
		for (unsigned i = 0; i < def->count(); ++i)
		{
			printf("\t[%s]\n", def->get_AtIndex(i)->property_name);
		}
	}

	Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
	params.representation_default_parameters = defaults;
	params.page_level = 8;
	return driver.lock()->get_RepresentationLayerCollection()->create(layer, params);

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

	classifications_ = 
	{
		(int)0xFF0000FF,(int)0xFF8800FF,(int)0xFFFF00FF,(int)0x80FF00FF,
		(int)0x00FF00FF,(int)0x00FF80FF,(int)0x00FFFFFF,(int)0x0080FFFF,
		(int)0x0000FFFF,(int)0x8000FFFF,(int)0xFF00FFFF,(int)0xFF0080FF,
		(int)0xFF0000FF,(int)0xFF8800FF,(int)0xFFFF00FF,(int)0x80FF00FF,
		(int)0x00FF00FF,(int)0x00FF80FF,(int)0x00FFFFFF,(int)0x0080FFFF,
		(int)0x0000FFFF,(int)0x8000FFFF,(int)0xFF00FFFF,(int)0xFF0080FF,
		(int)0xFF0000FF,(int)0xFF8800FF,(int)0xFFFF00FF,(int)0x80FF00FF,
		(int)0x00FF00FF,(int)0x00FF80FF,(int)0x00FFFFFF,(int)0x0080FFFF
	};

	returns_ =
	{
		(int)0xFF0000FF,
		(int)0x880000FF,
		(int)0x00FF00FF,
		(int)0x008800FF,
		(int)0x0000FFFF,
		(int)0x000088FF,
		(int)0xFFFFFFFF,
		(int)0x888888FF
	};

	default_state_ = {
	/*MIN_POINT_SIZE			*/	1.25f,
	/*MAX_POINT_SIZE			*/	5.0f,
	/*POINT_SCALE_DIST			*/	10.0f,
	/*RENDER_MODE				*/	Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes::COLOR_MODE,
	/*HEIGHT_SHADING_START_HEIGHT*/	0.0f,
	/*HEIGHT_SHADING_END_HEIGHT	*/	500.0f,
	/*TRANSLATION_Z_OFFSET		*/	-360.0f,
	/*PIXEL_SIZE_MODE			*/	Geoweb3d::Vector::PointCloudProperties::PointCloudPixelSizeModes::ADAPTIVE_DEFAUT_SPACING_MODE,
	/*MIN_NODE_SIZE				*/	150.0f,
	/*MAX_RENDER_LEVEL			*/	10,
	/*USER_MIN_SPACING			*/	12.0
	};

	specific_state_ = default_state_;

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
void MiniEngine::PopulateCurrentStateProps(const long current_fid)
{
	const Geoweb3d::IGW3DPropertyCollection* current_props = point_cloud_.lock()->get_PropertyCollection(current_fid_);

	if (current_props)
	{
		specific_state_.min_pt_size = current_props->get_Property(Geoweb3d::Vector::PointCloudProperties::IndividualProperties::MIN_POINT_SIZE);
		specific_state_.max_pt_size = current_props->get_Property(Geoweb3d::Vector::PointCloudProperties::IndividualProperties::MAX_POINT_SIZE);
		specific_state_.point_scale_dist_ = current_props->get_Property(Geoweb3d::Vector::PointCloudProperties::IndividualProperties::POINT_SCALE_DIST);
		specific_state_.render_mode = (Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes)((int)current_props->get_Property(Geoweb3d::Vector::PointCloudProperties::RENDER_MODE));
		specific_state_.min_shade_height = current_props->get_Property(Geoweb3d::Vector::PointCloudProperties::HEIGHT_SHADING_START_HEIGHT);
		specific_state_.max_shade_height = current_props->get_Property(Geoweb3d::Vector::PointCloudProperties::HEIGHT_SHADING_END_HEIGHT);
		specific_state_.z_offset = current_props->get_Property(Geoweb3d::Vector::PointCloudProperties::TRANSLATION_Z_OFFSET);
		specific_state_.pixel_mode = (Geoweb3d::Vector::PointCloudProperties::PointCloudPixelSizeModes)((int)current_props->get_Property(Geoweb3d::Vector::PointCloudProperties::PIXEL_SIZE_MODE));
		specific_state_.min_node_size = current_props->get_Property(Geoweb3d::Vector::PointCloudProperties::MIN_NODE_SIZE);
		specific_state_.max_render_level = current_props->get_Property(Geoweb3d::Vector::PointCloudProperties::MAX_RENDER_LEVEL);
		specific_state_.user_min_spacing = current_props->get_Property(Geoweb3d::Vector::PointCloudProperties::USER_MIN_SPACING);
	}
	else
	{
		specific_state_ = default_state_;
	}
}

void MiniEngine::SetProps(Geoweb3d::IGW3DPropertyCollectionPtr props, prop_state prop_s)
{
	props->put_Property(Geoweb3d::Vector::PointCloudProperties::IndividualProperties::MIN_POINT_SIZE, prop_s.min_pt_size);
	props->put_Property(Geoweb3d::Vector::PointCloudProperties::IndividualProperties::MAX_POINT_SIZE, prop_s.max_pt_size);
	props->put_Property(Geoweb3d::Vector::PointCloudProperties::IndividualProperties::POINT_SCALE_DIST, prop_s.point_scale_dist_);
	props->put_Property(Geoweb3d::Vector::PointCloudProperties::RENDER_MODE, prop_s.render_mode);
	props->put_Property(Geoweb3d::Vector::PointCloudProperties::HEIGHT_SHADING_START_HEIGHT, prop_s.min_shade_height);
	props->put_Property(Geoweb3d::Vector::PointCloudProperties::HEIGHT_SHADING_END_HEIGHT, prop_s.max_shade_height);
	props->put_Property(Geoweb3d::Vector::PointCloudProperties::TRANSLATION_Z_OFFSET, prop_s.z_offset);
	props->put_Property(Geoweb3d::Vector::PointCloudProperties::PIXEL_SIZE_MODE, prop_s.pixel_mode);
	props->put_Property(Geoweb3d::Vector::PointCloudProperties::MIN_NODE_SIZE, prop_s.min_node_size);
	props->put_Property(Geoweb3d::Vector::PointCloudProperties::MAX_RENDER_LEVEL, prop_s.max_render_level);
	props->put_Property(Geoweb3d::Vector::PointCloudProperties::USER_MIN_SPACING, prop_s.user_min_spacing);
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

bool MiniEngine::LoadData(const char* rootpath)
{
	LoadDemoImageryDataset(sdk_context_);

	Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("PointCloud");
	properties_ = driver.lock()->get_PropertyCollection()->create_Clone();

	SetProps(properties_, default_state_);
	for (int i = 0; i < 32; i++)
	{
		properties_->put_Property(Geoweb3d::Vector::PointCloudProperties::CLASSIFICATION_DATA_0 + i, classifications_.c[i]);
	}
	for (int i = 0; i < 8; i++)
	{
		properties_->put_Property(Geoweb3d::Vector::PointCloudProperties::RETURN_DATA_0 + i, returns_.r[i]);
	}

	line_segment_ = sdk_context_->get_LineSegmentIntersectionTestCollection()->create();
	Geoweb3d::GW3DEnvelope env;
	Geoweb3d::GW3DResult res;

	Geoweb3d::IGW3DVectorDriverWPtr	lidar_vector_driver_ = sdk_context_->get_VectorDriverCollection(true)->get_Driver("Liblas");
	Geoweb3d::IGW3DVectorDataSourceWPtr data_source = lidar_vector_driver_.lock()->get_VectorDataSourceCollection()->open("../examples/media/lidar/0629.las", res);

	if (data_source.expired())
	{
		std::cout << "Could not load the following Vector Data Source\n";
	}

	Geoweb3d::IGW3DVectorLayerCollection* vlyrcollection = data_source.lock()->get_VectorLayerCollection();
	vlyrcollection->reset();

	Geoweb3d::IGW3DVectorLayerWPtr layer;

	while (vlyrcollection->next(&layer))
	{
		//for a shapefile, we only have 1 layer.. but.. just beware this
		//is would overwrite if it has more than 1 layer
		point_cloud_ = RepresentAsPointCloud(sdk_context_, layer, properties_);
		env.merge(layer.lock()->get_Envelope());
	}

	if (!point_cloud_.expired())
	{
		line_segment_ = sdk_context_->get_LineSegmentIntersectionTestCollection()->create();

		if (!line_segment_.expired())
		{
			line_segment_.lock()->get_VectorRepresentationCollection()->add(point_cloud_);
		}
	}

	camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);
	double longitude, latitude;
	longitude = (env.MinX + env.MaxX) * 0.5;
	latitude = (env.MinY + env.MaxY) * 0.5;

	camera_controller_->put_Location(longitude, latitude);
	camera_controller_->put_Elevation(3000);
	camera_controller_->put_Rotation(0, 89.0, 0);

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

void MiniEngine::OnDraw2D(const Geoweb3d::IGW3DCameraWPtr& camera) {};

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

				Geoweb3d::IGW3DVectorLayerWPtr point_cloud_vector_layer = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer();

				Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection* details_collection = intersection_report->get_IntersectionDetailCollection();

				Geoweb3d::IGW3DLineSegmentIntersectionDetail* intersection_detail;

				details_collection->reset();
				details_collection->do_Sort(Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection::Ascending);

				int limit = 1;
				int running_count = 0;
				printf("\nNUM HIT %d\n", details_collection->count());
				//collect details for the intersected feature(s)
				while (details_collection->next(&intersection_detail))
				{
					//int64_t fid = intersection_detail->get_ObjectID();
					//int64_t node_id = fid / 10000000;
					//int64_t pt_id = fid - node_id * 10000000;
					//printf("Node ID %d, Point ID %d at DIST %5.5f\n", (int)node_id, (int)pt_id, intersection_detail->get_IntersectionDistance());

					streamer.insert(intersection_detail->get_ObjectID(), intersection_detail->get_IntersectionDistance());
					running_count++;
					if (running_count >= limit)
					{
						break;
					}
				}

				if (streamer.count())
				{
					
					// process the results
					point_cloud_vector_layer.lock()->Stream(&streamer);

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
			SetProps(properties_, specific_state_);
			if (!point_cloud_.expired())
				point_cloud_.lock()->put_PropertyCollection(current_fid_, properties_);
		}
		else
		{
			SetProps(properties_, default_state_);
			if (!point_cloud_.expired())
			{
				point_cloud_.lock()->put_PropertyCollection(properties_);
				//point_cloud_.lock()->force_Refresh();
			}
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

	//so we don't do an analytic when just trying to navigate. 
	static bool mouse_mouse_after_down_click = false;
	switch (win_event.Type)
	{
	case win_event.MouseButtonPressed:
	{
		mouse_mouse_after_down_click = false;
	}
	break;
	case win_event.MouseMoved:
	{
		mouse_mouse_after_down_click = true;
	}
	break;
	case win_event.MouseButtonReleased:
	{
		switch (win_event.MouseButton.button)
		{
		case Geoweb3d::Mouse::Left:
		{
			if (!line_segment_.expired() && !mouse_mouse_after_down_click)
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
		case Geoweb3d::Key::Q:
		{
			if (win_event.Key.shift)
			{
				state->pixel_mode = (Geoweb3d::Vector::PointCloudProperties::PointCloudPixelSizeModes)(state->pixel_mode - 1);
				if (state->pixel_mode < Geoweb3d::Vector::PointCloudProperties::PointCloudPixelSizeModes::ADAPTIVE_DEFAUT_SPACING_MODE)
				{
					state->pixel_mode = Geoweb3d::Vector::PointCloudProperties::PointCloudPixelSizeModes::USE_MIN_MAX_DIST_MODE;
				}
			}
			else
			{
				state->pixel_mode = (Geoweb3d::Vector::PointCloudProperties::PointCloudPixelSizeModes)(state->pixel_mode + 1);
				if (state->pixel_mode > Geoweb3d::Vector::PointCloudProperties::PointCloudPixelSizeModes::USE_MIN_MAX_DIST_MODE)
				{
					state->pixel_mode = Geoweb3d::Vector::PointCloudProperties::PointCloudPixelSizeModes::ADAPTIVE_DEFAUT_SPACING_MODE;
				}
			}

			if (state->pixel_mode == Geoweb3d::Vector::PointCloudProperties::PointCloudPixelSizeModes::ADAPTIVE_DEFAUT_SPACING_MODE)
			{
				printf("PIXEL_SIZE_MODE: ADAPTIVE_DEFAUT_SPACING_MODE\n");
			}
			else if (state->pixel_mode == Geoweb3d::Vector::PointCloudProperties::PointCloudPixelSizeModes::ADAPTIVE_USE_USER_SPACING_MODE)
			{
				printf("PIXEL_SIZE_MODE: ADAPTIVE_USE_USER_SPACING_MODE\n");
			}
			else if (state->pixel_mode == Geoweb3d::Vector::PointCloudProperties::PointCloudPixelSizeModes::USE_MIN_MAX_DIST_MODE)
			{
				printf("PIXEL_SIZE_MODE: USE_MIN_MAX_DIST_MODE\n");
			}

			props_dirty_ = true;
			filter_out = true;
		}
		break;
		case Geoweb3d::Key::W:
		{
			if (win_event.Key.shift)
			{
				state->min_pt_size -= 1.0f;
				if (state->min_pt_size < 0.0f)
					state->min_pt_size = 0.0f;
			}
			else
			{
				if (state->min_pt_size < state->max_pt_size)
				{
					state->min_pt_size += 1.0f;
					if (state->min_pt_size > 30.0f)
						state->min_pt_size = 30.0f;
				}
			}
			printf("MIN_POINT_SIZE: %3.1f\n", state->min_pt_size);
			props_dirty_ = true;
			filter_out = true;
		}
		break;
		case Geoweb3d::Key::E:
		{
			if (win_event.Key.shift)
			{
				state->max_pt_size -= 1.0f;
				if (state->max_pt_size < 0.0f)
				{
					state->max_pt_size = 0.0f;
				}

				if (state->min_pt_size > state->max_pt_size)
				{
					state->min_pt_size = state->max_pt_size;
				}
			}
			else
			{
				state->max_pt_size += 1.0f;
				if (state->max_pt_size > 30.0f)
					state->max_pt_size = 30.0f;
			}
			printf("MAX_POINT_SIZE: %3.1f\n", state->max_pt_size);
			props_dirty_ = true;
			filter_out = true;
		}
		break;
		case Geoweb3d::Key::R:
		{
			if (win_event.Key.shift)
			{
				state->point_scale_dist_ -= 10.0f;
				if (state->point_scale_dist_ < 0.0f)
				{
					state->point_scale_dist_ = 0.0f;
				}
			}
			else
			{
				state->point_scale_dist_ += 10.0f;
				if (state->point_scale_dist_ > 10000.0f)
				{
					state->point_scale_dist_ = 10000.0f;
				}
			}

			printf("POINT_SCALE_DIST: %3.1f\n", state->point_scale_dist_);
			props_dirty_ = true;
			filter_out = true;
		}
		break;
		case Geoweb3d::Key::A:
		{
			if (win_event.Key.shift)
			{
				state->render_mode = (Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes)(state->render_mode - 1);
				if (state->render_mode < -0x01)
				//if (state->render_mode < Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes::COLOR_MODE)
				{
					state->render_mode = Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes::INTENSITY_MODE;
				}
			}
			else
			{
				state->render_mode = (Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes)(state->render_mode + 1);
				if (state->render_mode > Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes::INTENSITY_MODE)
				{
					//state->render_mode = Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes::COLOR_MODE;
					state->render_mode = (Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes )-0x01;
				}
			}

			if (state->render_mode == -0x01)
			{
				printf("RENDER_MODE: SECRET LOD MODE\n");
			}
			else if (state->render_mode == Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes::COLOR_MODE)
			{
				printf("RENDER_MODE: COLOR_MODE\n");
			}
			else if(state->render_mode == Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes::CLASSIFICATION_MODE)
			{
				printf("RENDER_MODE: CLASSIFICATION_MODE\n");
			}
			else if (state->render_mode == Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes::RETURN_MODE)
			{
				printf("RENDER_MODE: RETURN_MODE\n");
			}
			else if (state->render_mode == Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes::HEIGHT_SHADING)
			{
				printf("RENDER_MODE: HEIGHT_SHADING\n");
			}
			else if (state->render_mode == Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes::INTENSITY_MODE)
			{
				printf("RENDER_MODE: INTENSITY_MODE\n");
			}

			props_dirty_ = true;
			filter_out = true;
		}
		break;

		case Geoweb3d::Key::S:
		{
			if (win_event.Key.shift)
			{
				state->min_shade_height -= 10.0f;
				if (state->min_shade_height < 0.0f)
					state->min_shade_height = 0.0f;
			}
			else
			{
				if (state->min_shade_height < state->max_shade_height)
				{
					state->min_shade_height += 10.0f;
					if (state->min_shade_height > 10000.0f)
						state->min_shade_height = 10000.0f;
				}
			}
			printf("HEIGHT_SHADING_START_HEIGHT: %3.1f\n", state->min_shade_height);
			props_dirty_ = true;
			filter_out = true;
		}
		break;

		case Geoweb3d::Key::D:
		{
			if (win_event.Key.shift)
			{
				state->max_shade_height -= 10.0f;
				if (state->max_shade_height < 0.0f)
				{
					state->max_shade_height = 0.0f;
				}

				if (state->min_shade_height > state->max_shade_height)
				{
					state->min_shade_height = state->max_shade_height;
				}
			}
			else
			{
				state->max_shade_height += 10.0f;
				if (state->max_shade_height > 10000.0f)
					state->max_shade_height = 10000.0f;
			}
			printf("HEIGHT_SHADING_END_HEIGHT: %3.1f\n", state->max_shade_height);
			props_dirty_ = true;
			filter_out = true;
		}
		break;
		case Geoweb3d::Key::Z:
		{
			if (win_event.Key.shift)
			{
				state->z_offset -= 10.0f;
				if (state->z_offset < -10000.0f)
					state->z_offset = -10000.0f;
			}
			else
			{
				state->z_offset += 10.0f;
				if (state->z_offset > 10000.0f)
					state->z_offset = 10000.0f;
			}
			printf("TRANSLATION_Z_OFFSET: %6.1f\n", state->z_offset);
			props_dirty_ = true;
			filter_out = true;
		}
		break;
		case Geoweb3d::Key::X:
		{
			if (win_event.Key.shift)
			{
				state->min_node_size -= 10.0f;
				if (state->min_node_size < 30.0f)
					state->min_node_size = 30.0;
			}
			else
			{
				state->min_node_size += 10.0f;
				if (state->min_node_size > 300.0f)
					state->min_node_size = 300.0f;
			}
			printf("MIN_NODE_SIZE: %2.2f\n", state->min_node_size);
			props_dirty_ = true;
			filter_out = true;
		}
		break;

		case Geoweb3d::Key::C:
		{
			if (win_event.Key.shift)
			{
				state->max_render_level -= 1;
				if (state->max_render_level < 0)
				{
					state->max_render_level = 0;
				}
			}
			else
			{
				state->max_render_level += 1;
				if (state->max_render_level > 50)
					state->max_render_level = 50;
			}
			printf("MAX_RENDER_LEVEL: %d\n", state->max_render_level);
			props_dirty_ = true;
			filter_out = true;
		}
		break;

		case Geoweb3d::Key::V:
		{
			if (win_event.Key.shift)
			{
				state->user_min_spacing -= 0.1f;
				if (state->user_min_spacing < 0.1f)
				{
					state->user_min_spacing = 0.1f;
				}
			}
			else
			{
				state->user_min_spacing += 0.1f;
				if (state->user_min_spacing > 1000.0f)
					state->user_min_spacing = 1000.0f;
			}
			printf("USER_MIN_SPACING: %3.1f\n", state->user_min_spacing);
			props_dirty_ = true;
			filter_out = true;
		}
		break;

		case Geoweb3d::Key::H:
		{
			printf("Changeable Properties: ");
			printf("\n"
				"Q - PointCloudPixelSizeModes (W/SHIFT DECREASE)				\n"
				"W - MIN_POINT_SIZE (W/SHIFT DECREASE)			\n"
				"E - MAX_POINT_SIZE (W/SHIFT DECREASE)			\n"
				"R - POINT_SCALE_DIST (W/SHIFT DECREASE)		\n"

				"A - PointCloudRenderModes	(W/SHIFT DECREASE)					\n"
				"S - HEIGHT_SHADING_START_HEIGHT				\n"
				"D - HEIGHT_SHADING_END_HEIGHT					\n"

				"Z - TRANSLATION_Z_OFFSET						\n"
				"X - MIN_NODE_SIZE								\n"
				"C - MAX_RENDER_LEVEL							\n"
				"V - USER_MIN_SPACING							\n"

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

