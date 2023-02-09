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
#include <string>

class callback : public Geoweb3d::Vector::IGW3DConstructionCallback
{
public:
	callback() {};

	ConstructionCallbackReturnType onInsert(long feature_id, const Geoweb3d::IGW3DPropertyCollection *feature_attributes, Geoweb3d::Vector::IGW3DConstructionCallbackRepresentationSettings *rep_settings)
	{
		//static const unsigned layer_model_url_idx = feature_attributes->get_DefinitionCollection()->get_IndexByName("FullPath");
		static const unsigned layer_scale_x_idx = feature_attributes->get_DefinitionCollection()->get_IndexByName("Scale_X");
		static const unsigned layer_scale_y_idx = feature_attributes->get_DefinitionCollection()->get_IndexByName("Scale_Y");
		static const unsigned layer_scale_z_idx = feature_attributes->get_DefinitionCollection()->get_IndexByName("Scale_Z");
		static const unsigned lod_idx = feature_attributes->get_DefinitionCollection()->get_IndexByName("LOD_Tile");

		//static const unsigned rep_model_url_idx = rep_settings->representation_properties->get_DefinitionCollection()->get_IndexByName("URL");
		static const unsigned rep_scale_x_idx = rep_settings->representation_properties->get_DefinitionCollection()->get_IndexByName("SCALE_X");
		static const unsigned rep_scale_y_idx = rep_settings->representation_properties->get_DefinitionCollection()->get_IndexByName("SCALE_Y");
		static const unsigned rep_scale_z_idx = rep_settings->representation_properties->get_DefinitionCollection()->get_IndexByName("SCALE_Z");
		static const unsigned rep_red_idx = rep_settings->representation_properties->get_DefinitionCollection()->get_IndexByName("RED");
		static const unsigned rep_green_idx = rep_settings->representation_properties->get_DefinitionCollection()->get_IndexByName("GREEN");
		static const unsigned rep_blue_idx = rep_settings->representation_properties->get_DefinitionCollection()->get_IndexByName("BLUE");
		static const unsigned rep_z_offset_idx = rep_settings->representation_properties->get_DefinitionCollection()->get_IndexByName("TRANSLATION_Z_OFFSET");

		static const unsigned rep_radius_idx = rep_settings->representation_properties->get_DefinitionCollection()->get_IndexByName("RADIUS");

		//rep_settings->put_VectorRepresentationProperty(rep_model_url_idx, feature_attributes->get_Property(layer_model_url_idx));
		rep_settings->put_VectorRepresentationProperty(rep_scale_x_idx, feature_attributes->get_Property(layer_scale_x_idx));
		rep_settings->put_VectorRepresentationProperty(rep_scale_y_idx, feature_attributes->get_Property(layer_scale_y_idx));
		rep_settings->put_VectorRepresentationProperty(rep_scale_z_idx, feature_attributes->get_Property(layer_scale_z_idx));
		//rep_settings->put_VectorRepresentationProperty(rep_radius_idx, 2.0);

		float red = 0.0;
		float green = 0.0;
		float blue = 0.0;
		int lod = feature_attributes->get_Property(lod_idx);
		double z_offset = 0.0;

		switch (lod)
		{
		case 0:
		{
			red = 166.0; green = 206.0; blue = 227.0;
			z_offset = 0.0;
		}
		break;
		case 1:
		{
			red = 31.0; green = 120.0; blue = 180.0;
			z_offset = 1.0;
		}
		break;
		case 2:
		{
			red = 178.0; green = 223.0; blue = 138.0;
			z_offset = 2.0;
		}
		break;
		case 3:
		{
			red = 51.0; green = 160.0; blue = 44.0;
			z_offset = 3.0;
		}
		break;
		case 4:
		{
			red = 251.0; green = 154.0; blue = 153.0;
			z_offset = 4.0;
		}
		break;
		case 5:
		{
			red = 227.0; green = 26.0; blue = 28.0;
			z_offset = 5.0;
		}
		break;
		case 6:
		{
			red = 253.0; green = 191.0; blue = 111.0;
			z_offset = 6.0;
		}
		break;
		case 7:
		{
			red = 255.0; green = 127.0; blue = 0.0;
			z_offset = 7.0;
		}
		break;
		case 8:
		{
			red = 202.0; green = 178.0; blue = 214.0;
			z_offset = 8.0;
		}
		break;
		case 9:
		{
			red = 106.0; green = 61.0; blue = 154.0;
			z_offset = 9.0;
		}
		break;
		default:
			break;
		}

		rep_settings->put_VectorRepresentationProperty(rep_red_idx, red / 255.0);
		rep_settings->put_VectorRepresentationProperty(rep_green_idx, green / 255.0);
		rep_settings->put_VectorRepresentationProperty(rep_blue_idx, blue / 255.0);

		rep_settings->put_VectorRepresentationProperty(rep_z_offset_idx, z_offset + 10.0);
		return KEEP_AND_SAVE;
	}

	void onRemove(long feature_id) {};

};

callback cb_;

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

	PathIntersectionTestStream(Geoweb3d::IGW3DPropertyCollectionPtr props)
	{
		props_ = props;
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
		// Request the attribute data for the current feature
		const Geoweb3d::IGW3DAttributeCollection* attribute_collection = result->get_AttributeCollection();

		// Query the object id (or feature id) for the current feature
		long object_id = result->get_ObjectID();

		Geoweb3d::IGW3DStringPtr full_path;
		attribute_collection->get_Property(attribute_collection->get_DefinitionCollection()->get_IndexByName("Full_Path")).to_string(full_path);
		bool ahgt = attribute_collection->get_Property(attribute_collection->get_DefinitionCollection()->get_IndexByName("AHGT"));
		double ao1 = attribute_collection->get_Property(attribute_collection->get_DefinitionCollection()->get_IndexByName("AO1"));
		double scale_x = attribute_collection->get_Property(attribute_collection->get_DefinitionCollection()->get_IndexByName("Scale_X"));
		double scale_y = attribute_collection->get_Property(attribute_collection->get_DefinitionCollection()->get_IndexByName("Scale_Y"));
		double scale_z = attribute_collection->get_Property(attribute_collection->get_DefinitionCollection()->get_IndexByName("Scale_Z"));
		std::int64_t location_id = attribute_collection->get_Property(attribute_collection->get_DefinitionCollection()->get_IndexByName("Location_ID"));
		int lod = attribute_collection->get_Property(attribute_collection->get_DefinitionCollection()->get_IndexByName("LOD_Tile"));

		FeatureInfo finfo = find(object_id);

		std::string full_path_str = full_path->c_str();
		std::cout << "Feature Info: <id : " << object_id << " >  < Intersection Distance: " << finfo.intersection_distance << " >" << std::endl;
		std::cout << "\t Full Path: " << full_path_str << std::endl; 
		std::cout << "\t AHGT: " << (ahgt ? "true" : "false") << std::endl;
		std::cout << "\t AO1: " << ao1 << std::endl;
		std::cout << "\t Scale_X: " << scale_x << std::endl;
		std::cout << "\t Scale_Y: " << scale_y << std::endl;
		std::cout << "\t Scale_Z: " << scale_z << std::endl;
		std::cout << "\t Location_ID: " << location_id << std::endl;
		std::cout << "\t LOD_Tile: " << lod << std::endl;

		static const unsigned rep_model_url_idx = props_->get_DefinitionCollection()->get_IndexByName("URL");
		static const unsigned rep_heading_idx = props_->get_DefinitionCollection()->get_IndexByName("HEADING");
		static const unsigned rep_scale_x_idx = props_->get_DefinitionCollection()->get_IndexByName("SCALE_X");
		static const unsigned rep_scale_y_idx = props_->get_DefinitionCollection()->get_IndexByName("SCALE_Y");
		static const unsigned rep_scale_z_idx = props_->get_DefinitionCollection()->get_IndexByName("SCALE_Z");

		props_->put_Property(rep_model_url_idx, full_path->c_str());
		props_->put_Property(rep_heading_idx, ao1);
		props_->put_Property(rep_scale_x_idx, scale_x);
		props_->put_Property(rep_scale_y_idx, scale_y);
		props_->put_Property(rep_scale_z_idx, scale_z);

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
	Geoweb3d::IGW3DPropertyCollectionPtr props_;
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
	current_model_fid_ = -1;
	current_sphere_fid_ = -1;

	/*struct prop_state
	{
		float red_;
		float green_;
		float blue_;
		float z_offset;
		float radius;
	};*/

	sphere_default_state_ = {
		1.0,//red
		0.0,
		0.0,
		1.0,
		5.0
	};

	sphere_specific_state_ = {
		1.0,//red
		0.0,
		0.0,
		1.0,
		5.0
	};

	model_default_state_ = {
		0.0,
	};

	model_specific_state_ = {
		0.0,
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

void MiniEngine::PopulateCurrentStateProps(Geoweb3d::IGW3DVectorRepresentationWPtr rep,const long current_fid)
{
	const Geoweb3d::IGW3DPropertyCollection* current_props = rep.lock()->get_PropertyCollection(current_fid);

	if (current_props)
	{
		//specific_state_.fill_mode_ = (Geoweb3d::Vector::SphereProperties::FillMode) ((int)current_props->get_Property(Geoweb3d::Vector::SphereProperties::FILL_MODE));
		sphere_specific_state_.red_ = current_props->get_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::RED);
		sphere_specific_state_.green_ = current_props->get_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::GREEN);
		sphere_specific_state_.blue_ = current_props->get_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::BLUE);
		sphere_specific_state_.z_offset_ = current_props->get_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::TRANSLATION_Z_OFFSET);
		sphere_specific_state_.radius_ = current_props->get_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::RADIUS);
	}
	else
	{
		//specific_state_.fill_mode_ = default_state_.fill_mode_;
		sphere_specific_state_.red_ =		sphere_default_state_.red_;
		sphere_specific_state_.green_ =		sphere_default_state_.green_;
		sphere_specific_state_.blue_ =		sphere_default_state_.blue_;
		sphere_specific_state_.z_offset_ =	sphere_default_state_.z_offset_;
		sphere_specific_state_.radius_ =	sphere_default_state_.radius_;
	}
}

void MiniEngine::SetCurrentProps(Geoweb3d::IGW3DPropertyCollectionPtr current)
{
	current->put_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::RED, sphere_specific_state_.red_);
	current->put_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::GREEN, sphere_specific_state_.green_);
	current->put_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::BLUE, sphere_specific_state_.blue_);
	current->put_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::TRANSLATION_Z_OFFSET, sphere_specific_state_.z_offset_);
	current->put_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::RADIUS, sphere_specific_state_.radius_);

};

void MiniEngine::SetDefaultProps(Geoweb3d::IGW3DPropertyCollectionPtr defaults)
{
	//defaults->put_Property(Geoweb3d::Vector::SphereProperties::FILL_MODE, default_state_.fill_mode_);
	defaults->put_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::RED, sphere_default_state_.red_);
	defaults->put_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::GREEN, sphere_default_state_.green_);
	defaults->put_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::BLUE, sphere_default_state_.blue_);
	defaults->put_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::TRANSLATION_Z_OFFSET, sphere_default_state_.z_offset_);
	defaults->put_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::RADIUS, sphere_default_state_.radius_);

	defaults->put_Property(Geoweb3d::Vector::SphereProperties::IndividualProperties::ALPHA, 0.5);

};

void MiniEngine::PopulateCurrentModelStateProps(Geoweb3d::IGW3DVectorRepresentationWPtr rep, const long current_fid)
{
	const Geoweb3d::IGW3DPropertyCollection* current_props = rep.lock()->get_PropertyCollection(current_fid);

	if (current_props)
	{
		model_specific_state_.z_offset_ = current_props->get_Property(Geoweb3d::Vector::ModelProperties::IndividualProperties::TRANSLATION_Z_OFFSET);
	}
	else
	{
		model_specific_state_.z_offset_ = model_default_state_.z_offset_;
	}
}

void MiniEngine::SetCurrentModelProps(Geoweb3d::IGW3DPropertyCollectionPtr current)
{
	current->put_Property(Geoweb3d::Vector::ModelProperties::IndividualProperties::TRANSLATION_Z_OFFSET, model_specific_state_.z_offset_);

};

void MiniEngine::SetDefaultModelProps(Geoweb3d::IGW3DPropertyCollectionPtr defaults)
{
	defaults->put_Property(Geoweb3d::Vector::ModelProperties::IndividualProperties::TRANSLATION_Z_OFFSET, model_default_state_.z_offset_);

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
	//also add a web service for the rest of the world:
	const char* map_service = "http://app.geoweb3d.com/dashboard/data/frmt_wms_virtualearth_hybrid.xml";

	//https://earth.app.goo.gl/8MNfJR
	//	printf("Please wait: loading %s\n", map_service);

	Geoweb3d::IGW3DRasterDriverCollection *raster_drivers = sdk_context_->get_RasterDriverCollection();

	Geoweb3d::GW3DResult res;

	//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
	//having to guess which data source driver is needed to open up a particular dataset.
	Geoweb3d::IGW3DRasterDataSourceWPtr rdatasource = raster_drivers->auto_Open(map_service, res);

	Geoweb3d::IGW3DRasterRepresentationDriverCollection* raster_repdrivers = sdk_context_->get_RasterLayerRepresentationDriverCollection();
	Geoweb3d::IGW3DRasterRepresentationDriverWPtr imagery_driver = raster_repdrivers->get_Driver("Imagery");

	Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context_->get_VectorDriverCollection()->get_Driver("CDB_Datasource");

	if (driver.expired())
	{
		printf("Could  not find the driver requested!  For a list of supported drivers, run the Print Capabilities example");
		return false;
	}

	//Place user specific CDB database in the following folder ".. /examples/media/"
	Geoweb3d::IGW3DVectorDataSourceWPtr  data_source = driver.lock()->get_VectorDataSourceCollection()->open(".. /examples/media/", res);

	if (data_source.expired())
	{
		std::cout << "Could not load the following Vector Data Source\n";
	}

	Geoweb3d::IGW3DVectorLayerCollection *vlyrcollection = data_source.lock()->get_VectorLayerCollection();

	Geoweb3d::IGW3DVectorLayerWPtr layer;
	Geoweb3d::GW3DEnvelope env;
	Geoweb3d::Vector::IGW3DLODPagingPolicy lod_policy;
	lod_policy.page_level_start = 12;

	//NOTE: Using auto_Open allows the system to select the default, built-in representation corresponding to the 
	//the specified data source. Certain data sources have their specific internal representation that needs to be
	//enforced and aut_Open is the means by which to do so. This will ensure that the most suitable representation for
	//the datasource is selected.
	Geoweb3d::IGW3DVectorRepresentationHelperCollectionPtr rep_collect = sdk_context_->get_VectorRepresentationDriverCollection()->auto_Open(data_source, &lod_policy);

	Geoweb3d::IGW3DVectorRepresentationDriverWPtr model_rep_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("Model");
	model_properties_ = model_rep_driver.lock()->get_PropertyCollection()->create_Clone();
	SetDefaultModelProps(model_properties_);

	Geoweb3d::IGW3DVectorRepresentationDriverWPtr sphere_rep_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("Sphere");
	Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
	params.page_level = 12;
	sphere_properties_ = sphere_rep_driver.lock()->get_PropertyCollection()->create_Clone();
	SetDefaultProps(sphere_properties_);
	params.representation_default_parameters = sphere_properties_;
	params.cb = &cb_;

	


	while (vlyrcollection->next(&layer))
	{
		Geoweb3d::IGW3DVectorRepresentationWPtr rep = sphere_rep_driver.lock()->get_RepresentationLayerCollection()->create(layer, params);
		reps_.push_back(rep);
		//current_rep_ = rep;		
		Geoweb3d::GW3DEnvelope temp = layer.lock()->get_Envelope();
	}

	line_segment_ = sdk_context_->get_LineSegmentIntersectionTestCollection()->create();

	Geoweb3d::IGW3DVectorRepresentationWPtr rep;
	while(rep_collect->next(&rep)) 
	{
		if (!line_segment_.expired())
		{
			line_segment_.lock()->get_VectorRepresentationCollection()->add(rep);
		}
	}


	camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);
	double longitude, latitude;
	
	longitude = 45.005564472622083;
	latitude = 12.791623550637965;
	camera_controller_->put_Location(longitude, latitude);
	camera_controller_->put_Elevation(155);
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

			PathIntersectionTestStream streamer(model_properties_);
			bool found_one = false;
			while (intersection_report_collection->next(&intersection_report))
			{
				Geoweb3d::IGW3DVectorLayerWPtr vector_layer = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer();

				current_rep_ = intersection_report->get_VectorRepresentation();
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
					vector_layer.lock()->Stream(&streamer);

					//should dig in and get the closest but using first one in the set for now
					current_model_fid_ = streamer.get_AtIndex(0);
					found_one = true;
					PopulateCurrentModelStateProps(current_rep_, current_model_fid_);
				}

			}
			if (!found_one)
			{
				current_model_fid_ = -1;
			}

			//one shot
			line_segment_.lock()->put_Enabled(false);
		}


		if (model_props_dirty_)
		{

			if (current_model_fid_ >= 0)
			{
				SetCurrentModelProps(model_properties_);
				if (!current_rep_.expired())
					current_rep_.lock()->put_PropertyCollection(current_model_fid_, model_properties_);
			}
			else
			{
				SetDefaultModelProps(model_properties_);
				if (!current_rep_.expired())
					current_rep_.lock()->put_PropertyCollection(model_properties_);
			}
			model_props_dirty_ = false;
		}
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
		model_prop_state* state;
		if (current_model_fid_ >= 0)
		{
			state = &model_specific_state_;
		}
		else
		{
			state = &model_default_state_;
		}

		switch (win_event.Key.code)
		{
			case Geoweb3d::Key::A:
			{
				if (win_event.Key.shift)
				{
					state->z_offset_-= 10.0f;
					if (state->z_offset_ < 0.0f)
						state->z_offset_ = 0.0f;
				}
				else
				{
					state->z_offset_ += 10.0f;
					if (state->z_offset_ > 200.0f)
						state->z_offset_ = 200.0f;
				}
				printf("Z_OFFSET: %1.1f\n", state->z_offset_);
				model_props_dirty_ = true;
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

LRESULT CALLBACK MiniEngine::ProcessLowLevelEvent(OSWinHandle hWndParent, UINT message, WPARAM wParam, LPARAM lParam)
{
	// We don't forward the WM_CLOSE message to prevent the internal SDK from closing us (click on X)
	if (message == WM_CLOSE)
	{
		printf("ProcessLowLevelEvent: WM_Close detected and prevented!\n");
		return 0;
	}
	return 1;
}

