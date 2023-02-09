// LargerExample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>

#include "engine/GW3DCamera.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DVector.h"


#include "engine/IGW3DVectorRepresentationDriver.h"
#include "engine/IGW3DVectorRepresentationCollection.h"
#include "engine/IGW3DFrustumAnalysisStream.h"
#include "engine/IGW3DLightingConfiguration.h"

#include "engine/IGW3DEventStream.h"
#include "engine/IGW3DFeatureFeedbackResultCollection.h"
#include "engine/IGW3DFeatureFeedbackResult.h"
#include "engine/IGW3DContentsResult.h"
#include "engine/IGW3DFrameAnalysisContentsConfiguration.h"
#include "engine/IGW3DFrustumAnalysis.h"
#include "engine/IGW3DFeatureFeedbackConfiguration.h"
#include "engine/IGW3DFeatureID2DPixelLocationConfiguration.h"


#include "GeoWeb3dCore/SystemExports.h"
#include "GeoWeb3dCore/LayerParameters.h"
#include "GeoWeb3dCore/CoordinateExports.h"
#include "../Common/NavigationHelper.h"
#include "../Common/IdentifyVector.h"

#include "EntityController.h"

void my_fatal_function(const char *msg );
void my_info_function(const char *msg );

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

#define GLEW_STATIC
#include <GL/glew.h>

double closed_interval_rand(double x0, double x1)
{
	return x0 + (x1 - x0) * rand() / ((double)RAND_MAX);
}

////////////////////////////// help with threads if even really needed /////////////////

template <typename C>
struct ThreadInfo
{
	C* obj;	// and that object has a function that takes void* and returns DWORD
	DWORD(C::* function)(void*);	// and we have any amount of extra data that we might need.	

	void* data;

	// default copy c-tor, d-tor and operator= are fine	
	ThreadInfo(C* o, DWORD(C::*func)(void*), void* d) : obj(o), function(func), data(d)
	{
	}
};

template <typename C>
DWORD WINAPI runThreadFunction(void* data)
{
	std::auto_ptr<ThreadInfo<C> > ti((ThreadInfo<C>*)data);
	return ((ti->obj)->*(ti->function))(ti->data);
};

template <typename C>
void* makeThreadInfo(C* o, DWORD(C::* f)(void*), void* d)
{
	return (void*)new ThreadInfo<C>(o, f, d);
};


//Overlay2DHandler implementation
struct Overlay2DMetadata
{
	size_t v_index_start;
	size_t count;
};

struct Overlay2DVertex
{
	short x;
	short y;
	float u;
	float v;
};
struct Overlay2DHandler
{
	Overlay2DHandler()
		: texture(0)
	{
	}

	Overlay2DMetadata* addRep(Geoweb3d::IGW3DVectorRepresentationWPtr rep)
	{
		Overlay2DMetadata* metadata = &(rep_container[rep]);

		metadata->v_index_start = vertex_array.size();

		return metadata;
	}

	void ProcessFeatureFeedbackResult(Geoweb3d::IGW3DCameraWPtr active_camera, Geoweb3d::IGW3DFeatureFeedbackResult* feature_result)
	{
		float fx, fy;

		const int icon_width = 32;
		const int icon_height = 32;

		int px, py;

		//No sorting yet.
		feature_result->get_FeatureID2DPixelLocationResult()->get_WindowCoordinate(px, py);
		feature_result->get_FeatureID2DPixelLocationResult()->get_NormalizedCoordinate(fx, fy);

		unsigned int width, height;
		int x, y;

		active_camera.unsafe_get()->get_Window().unsafe_get()->get_CameraWindowCoordinates(active_camera, x, y, width, height);

		//
		Overlay2DVertex vtx;

		//v0
		vtx.x = px - icon_width / 2;
		vtx.y = py - icon_height / 2;
		vtx.u = 0.0f;
		vtx.v = 0.0f;
		vertex_array.push_back(vtx);

		//v1
		vtx.x = px + icon_width / 2;
		vtx.y = py - icon_height / 2;
		vtx.u = 1.0f;
		vtx.v = 0.0f;
		vertex_array.push_back(vtx);

		//v2
		vtx.x = px + icon_width / 2;
		vtx.y = py + icon_height / 2;
		vtx.u = 1.0f;
		vtx.v = 1.0f;
		vertex_array.push_back(vtx);

		//v3
		vtx.x = px - icon_width / 2;
		vtx.y = py + icon_height / 2;
		vtx.u = 0.0f;
		vtx.v = 1.0f;
		vertex_array.push_back(vtx);
	}

	void CreateTexture()
	{
		const unsigned int tex_width = 32;
		const unsigned int tex_height = 32;

		std::vector< unsigned char > tex_data(tex_width * tex_height * 4, 255);

		int c, index = 0;

		for (int i = 0; i < tex_height; ++i)
		{
			for (int j = 0; j < tex_width; ++j)
			{
				c = (((i & 0x8) == 0) ^ (((j & 0x8)) == 0)) * 255;

				index = (i * tex_width + j) * 4;

				tex_data[index] = (unsigned char)c;
				tex_data[index + 1] = (unsigned char)c;
				tex_data[index + 2] = (unsigned char)c;
			}
		}

		glGenTextures(1, &texture);

		glBindMultiTextureEXT(GL_TEXTURE0, GL_TEXTURE_2D, texture);

		glTextureParameteriEXT(texture, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteriEXT(texture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteriEXT(texture, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteriEXT(texture, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureImage2DEXT(texture, GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &tex_data[0]);

		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void Draw( Geoweb3d::IGW3DCameraWPtr active_camera )
	{
		unsigned int width, height;
		int x, y;

		float left = 0.0;
		float right = 1.0f;
		float top = 0.0f;
		float bottom = 1.0f;

		if (texture == 0)
		{
			glewInit();
			CreateTexture();
		}

		if (!active_camera.expired() && !vertex_array.empty())
		{
			active_camera.unsafe_get()->get_Window().unsafe_get()->get_CameraWindowCoordinates(active_camera, x, y, width, height);
			active_camera.unsafe_get()->get_NormalizeWindowCoordinates(left, right, top, bottom);

			const char* camera_name = active_camera.unsafe_get()->get_Name();

			glViewport(x, y, width, height);

			glMatrixLoadIdentityEXT(GL_PROJECTION);

			glMatrixOrthoEXT(GL_PROJECTION, 0, width, height, 0, 0.0, 1.0);

			//glMatrixOrthoEXT( GL_PROJECTION, left, right, bottom, top, 0.0, 1.0 );

			glMatrixLoadIdentityEXT(GL_MODELVIEW);

			glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);

			glEnable(GL_TEXTURE_2D);
			glBindMultiTextureEXT(GL_TEXTURE0, GL_TEXTURE_2D, texture);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glColor4f(1.0, 1.0, 1.0, 1.0);

			glVertexPointer(2, GL_SHORT, sizeof(Overlay2DVertex), &vertex_array[0].x);
			glTexCoordPointer(2, GL_FLOAT, sizeof(Overlay2DVertex), &vertex_array[0].u);

			glDrawArrays(GL_QUADS, 0, static_cast<GLsizei>( vertex_array.size() ));

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_DEPTH_TEST);

		}
	}

	std::map< Geoweb3d::IGW3DVectorRepresentationWPtr, Overlay2DMetadata > rep_container;
	std::vector< Overlay2DVertex > vertex_array;

	unsigned int texture;
};
//////////////////////////////////////////////////////////////////////
class MiniEngine : public Geoweb3d::IGW3DEventStream, public Geoweb3d::IGW3DWindowCallback, public Geoweb3d::IGW3DFrustumAnalysisStream, public IdentifyVector, public EntityController
{
	MiniEngine(): IdentifyVector(Geoweb3d::IGW3DGeoweb3dSDKPtr()) {};
	Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
	Geoweb3d::IGW3DWindowWPtr window_;
	Geoweb3d::IGW3DCameraWPtr pilot_camera_;

	//for the items moving around in the scene...
	Geoweb3d::IGW3DVectorLayerWPtr entity_layer_;
	Geoweb3d::IGW3DVectorRepresentationWPtr entity_models_;
	//Geoweb3d::IGW3DVectorRepresentationWPtr entity_icons_;

	//static data, but labels
	Geoweb3d::IGW3DVectorLayerWPtr roads_;

	// for lon lat position feedback
	//Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr windows_camera_to_longitude_latitude_;
	const Geoweb3d::GW3DPoint *plon_lat_readback_;

	//so we can at least move around by mouse if no udp messages for example.
	NavigationHelper nav_helper_;

	Overlay2DHandler overlay_handler_;


	//EntityController
	void RotateEntity(Geoweb3d::IGW3DVectorLayerStreamResult*, double hdg) override
	{

	}
	

	//EntityController
	Geoweb3d::IGW3DVectorRepresentationWPtr GetEntityRepresentation()  override
	{
		return Geoweb3d::IGW3DVectorRepresentationWPtr();
	}
public:

	MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr &sdk_context) :sdk_context_(sdk_context), plon_lat_readback_(0), IdentifyVector(sdk_context)
	{

	};

	bool LoadWindowingSystem()
	{
		window_ = 
			sdk_context_->get_WindowCollection()->create_3DWindow("Larger Hello World", GW3D_OVERLAPPED, 50, 50, 1280, 720, 0, Geoweb3d::IGW3DStereoDriverPtr(), /*wires up  Geoweb3d::IGW3DWindowCallback*/this);
		
		if (window_.expired())
			return false;

		pilot_camera_ = window_.lock()->get_CameraCollection()->create("pilot cam");

		//not interested in seeing the grid on the globe.
		pilot_camera_.lock()->get_LatitudeLongitudeGrid()->put_Enabled(false);
		pilot_camera_.lock()->get_EnvironmentEffects()->put_EnableLensFlare(false);

		// configure to be constant illum (always light out), yet keeping the sun moon and starts in the right places.
		pilot_camera_.lock()->get_EnvironmentEffects()->get_LightingConfiguration()->put_EnableLightingOverride(true);

		//need a lon lat readback!
		/* 
		note, we already get the lat long of the center position in the nav helper! so let be a little smart
		windows_camera_to_longitude_latitude_ = pilot_camera_.lock()->get_WindowCoordinateToGeodeticQueryCollection()->create();
		*/

		//so we can at least move around by mouse if no udp messages
		nav_helper_.add_Camera( pilot_camera_ );

		return true;

	}

	void SetupShellForDynamicData() //SetupShellForDynamicData, just make the database and describe No data is added here
	{
		/* Set up our attribute fields */
		Geoweb3d::IGW3DDefinitionCollectionPtr attribute_fields = Geoweb3d::IGW3DDefinitionCollection::create();
		attribute_fields->add_Property("Entity_Number", Geoweb3d::PROPERTY_INT, "My attribute for the ID of each entity");
		attribute_fields->add_Property("Label", Geoweb3d::PROPERTY_STR, "My label attribute for entities of interest", 255);
		attribute_fields->add_Property("Balloon", Geoweb3d::PROPERTY_STR, "My balloon attribute for entities", 255);

		/* Get the Geoweb3d In-Memory Datasource Driver */
		Geoweb3d::IGW3DVectorDriverCollection* v_drivers = sdk_context_->get_VectorDriverCollection();
		Geoweb3d::IGW3DVectorDriverWPtr memory_driver = v_drivers->get_Driver("Geoweb3d_Datasource");

		if (memory_driver.expired())
		{
			printf("The requested vector driver doesn't exist!");
			return;
		}

		if (!memory_driver.lock()->get_IsCapable(isCreateDataSourceSupported))
		{
			//Not really needed since we know we can create datasources with the Geoweb3d_Datasource
			printf("Failed test to see if driver supports creating a datasource!\n");
			return;
		}

		/* Create a new in-memory datasource (which can have many layers) */
		Geoweb3d::IGW3DVectorDataSourceCollection* datasources = memory_driver.lock()->get_VectorDataSourceCollection();
		Geoweb3d::IGW3DVectorDataSourceWPtr my_datasource = datasources->create("My_Entity_Datasource");

		if (my_datasource.expired())
		{
			printf("Failed to create my datasource!\n");
			return;
		}


		/* Create a new point layer for my entities */
		Geoweb3d::IGW3DVectorLayerCollection* my_opened_layers = my_datasource.lock()->get_VectorLayerCollection();
		entity_layer_ = my_opened_layers->create("My_Entity_Layer", Geoweb3d::gtPOINT_25D, attribute_fields);

		if (entity_layer_.expired())
		{
			printf("Failed to create my layer!\n");
			return;
		}

		//Make the layer editable, so we can create features after representing, and move them via the streamer
		entity_layer_.lock()->put_GeometryEditableMode(true, false);



		//////////////////////////// now represent the vector data into the map.


		Geoweb3d::IGW3DVectorRepresentationDriverWPtr model_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("Model");

		if (model_driver.expired())
		{
			printf("Bug: Couldn't get the Model driver!\n");
			return;
		}
		
		//start off with the driver defaults and tweak them
		Geoweb3d::IGW3DPropertyCollectionPtr model_properties = model_driver.lock()->get_PropertyCollection()->create_Clone();
		model_properties->put_Property(Geoweb3d::Vector::ModelProperties::URL, ".\\data\\sdk\\models\\Vehicles\\Kia_Rio\\blue_kia.flt");
		model_properties->put_Property(Geoweb3d::Vector::ModelProperties::TRANSLATION_Z_OFFSET, 0.0); //elevation
		model_properties->put_Property(Geoweb3d::Vector::ModelProperties::TRANSLATION_Z_OFFSET_MODE, Geoweb3d::Vector::RELATIVE_MODE | Geoweb3d::Vector::IGNORE_VERTEX_Z);  //Relative to ground + ignore Z coordinate (i.e. ground clamped)

		model_properties->put_Property(Geoweb3d::Vector::ModelProperties::SCALE_X, 1.0); //scale in X
		model_properties->put_Property(Geoweb3d::Vector::ModelProperties::SCALE_Y, 1.0); //scale in Y
		model_properties->put_Property(Geoweb3d::Vector::ModelProperties::SCALE_Z, 1.0); //scale in Z			

		Geoweb3d::Vector::RepresentationLayerCreationParameter  defaultparams_models;

		defaultparams_models.representation_default_parameters = model_properties;
		defaultparams_models.page_level = 7;

		printf("Will start paging into computer memory at distance: %f\n",Geoweb3d::Coordinates::PageLevelToMeterRadius(defaultparams_models.page_level));

		entity_models_ = model_driver.lock()->get_RepresentationLayerCollection()->create(entity_layer_, defaultparams_models);

		if (entity_models_.expired())
		{
			printf("Failed to represent entitiesW as models!\n");
			return;
		}

	

		printf("\nMODEL DEFAULT PROPERTIES:\n");
		Geoweb3d::IGW3DStringPtr string_val;
		for (unsigned long i = 0; i < model_properties->count(); i++)
		{
			model_properties->get_Property(i).to_string(string_val);
			printf("\tPROPERTY %d: %s   == %s\n", i, model_properties->get_DefinitionCollection()->get_AtIndex(i)->property_name, string_val->c_str());
		}

		/* Set up LODs: show only models when very close, only icons when far away */
		entity_models_.lock()->get_ClampRadiusFilter()->put_Enabled(true);
		entity_models_.lock()->get_ClampRadiusFilter()->put_MinRadius(0.0);
		entity_models_.lock()->get_ClampRadiusFilter()->put_MaxRadius(10000.0);

	

		//frustum analysis setup
		entity_models_.lock()->get_FrameAnalysisContentsConfiguration()->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->put_Enabled(true);
		entity_models_.lock()->get_FrameAnalysisContentsConfiguration()->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->get_FeatureID2DPixelLocationConfiguration()->put_Enabled(true);
		overlay_handler_.addRep(entity_models_);
	}

	void LoadStaticVectorData()
	{
		//open data source
		std::string filepath = "../examples/media/customerdata/gis_osm_roads_free_1.shp";

		Geoweb3d::GW3DResult res;
		Geoweb3d::IGW3DVectorDataSourceWPtr data_source = sdk_context_->get_VectorDriverCollection()->auto_Open(filepath.c_str(), res);

		if (data_source.expired())
		{
			std::cout << "Could not load the following Vector Data Source: " << filepath.c_str();
			return;
		}
		

		//shapefiles only  have 1 layer, assuming not playing in a shapefile workspace!
		 roads_ = data_source.lock()->get_VectorLayerCollection()->get_AtIndex(0);

		//so lets take this layer and place it into the 3d map.	
		Geoweb3d::IGW3DVectorRepresentationDriverWPtr line = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("ColoredLine");

		Geoweb3d::IGW3DPropertyCollectionPtr my_default_props = line.lock()->get_PropertyCollection()->create_Clone();
		
		//showing how you can also query and property or attribute by name instead of having to use the enum system
		my_default_props->put_Property(my_default_props->get_DefinitionCollection()->get_IndexByName("LINE_WIDTH"), 5.0);

		Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
		params.page_level = 10;
		params.representation_default_parameters = my_default_props;
		Geoweb3d::IGW3DVectorRepresentationWPtr rep = line.lock()->get_RepresentationLayerCollection()->create(roads_, params);



		// lets make the home position of the navigation helper at the center of the osm road data.
		const Geoweb3d::GW3DEnvelope envelope = roads_.lock()->get_Envelope();

		const double cam_lon = (envelope.MaxX + envelope.MinX) * 0.5;
		const double cam_lat = (envelope.MaxY + envelope.MinY) * 0.5;
		const double cam_elev = 120;

		//putting the cam at a relative location in case the customer adds elevation data and they are in mountains... as they 
		//could be underground if they are learning things here....
		nav_helper_.put_HomePosition(pilot_camera_, 0.0, 30.0, 0.0, cam_lon, cam_lat, cam_elev, Geoweb3d::IGW3DPositionOrientation::Relative);
		nav_helper_.reset_ToHomePosition(pilot_camera_);
	}
	void LoadGISData()
	{
 
		//we populate the shell from SetupShellForDynamicData bellow, just to point out shell vs data loading.
		//then incase a customer has a lot of overhead, positions are updated on the background thread
		SetupShellForDynamicData();
		
		LoadStaticVectorData();

		//we want to enable picking on all of these
		//NOTE this just creates a rest from the camera out, but
		//you can put these anywhere in the world.
		
		/*IdentifyVector::*/addAllVectorRepresentationToTest();


		//pretend we get an initial position here before we start the update thread..

		//lets get the attributes definitions that we already setup when we created the database shell and description
		const Geoweb3d::IGW3DDefinitionCollection* attribute_fields = entity_layer_.lock()->get_AttributeDefinitionCollection();

		Geoweb3d::IGW3DPropertyCollectionPtr field_values(attribute_fields->create_PropertyCollection());
		int Entity_Number_field = field_values->get_DefinitionCollection()->get_IndexByName("Entity_Number");
		int label_field = field_values->get_DefinitionCollection()->get_IndexByName("Label");
		int balloon_field = field_values->get_DefinitionCollection()->get_IndexByName("Balloon");

		



		if (roads_.expired())
		{
			printf("no roads layer..please fix that and the continue\n");
			return;
		}

		//lets scatter moving cards or whatever around the roads layer we loaded
		const Geoweb3d::GW3DEnvelope envelope = roads_.lock()->get_Envelope();

		const double center_lat = (envelope.MaxY + envelope.MinY) * 0.5;
		const double center_lon = (envelope.MaxX + envelope.MinX) * 0.5;
		const double center_elev = 2.0;

		srand(22);

		double R = .9;
		double Rsquared = R * R;
		double x, y, m;

		const unsigned number_of_entities = 5000;
		EntityInfo working_entityinfo;
		for (unsigned i = 0; i < number_of_entities; i++)
		{
			do
			{
				//make somewhat a uniform distribution
				x = R * (2 * closed_interval_rand(0, 1) - 1);
				y = R * (2 * closed_interval_rand(0, 1) - 1);
				m = x * x + y * y;
			} while (m > Rsquared);

			working_entityinfo.wgs84_location.put_X(center_lon - x);
			working_entityinfo.wgs84_location.put_Y(center_lat + y);
			working_entityinfo.wgs84_location.put_Z(center_elev);

			working_entityinfo.heading_ = 0;
			working_entityinfo.pitch_ = 0;
			working_entityinfo.roll_ = 0;

			/* Set the entity ID attribute value */
			field_values->put_Property(Entity_Number_field, (int)i);

			/* For the first entity, persistently display the balloon */
			if (i == 0)
			{
				field_values->put_Property(label_field, "I don't have a balloon!"); // Set the 'Label' attribute
				field_values->put_Property(balloon_field, "");
			}

			/* For the next four entities, always display a multi-line label */
			else if (i >= 1 && i < 40)
			{
				field_values->put_Property(label_field, "This is an example \\nof a label that uses \\nmulti-line text");	// Set the 'Label' attribute
				field_values->put_Property(balloon_field, "");
			}
			else if (i > 40 && i < 1000) 
			{
				field_values->put_Property(label_field, "click me - example of single line text");	// Set the 'Label' attribute
				field_values->put_Property(balloon_field, "<table border=\"1\"><tr><td>Row 1 Cell 1</td><td>Row 1 Cell 2</td></tr><tr><td>Row 2 Cell 1</td><td>Row 2 Cell 2</td></tr></table>");	// Set the 'Label' attribute
			}

			/* From here on out, set keep things the same */
			else 
			{
				field_values->put_Property(label_field, "Just an entity");
				field_values->put_Property(balloon_field, "");
			}

			//Geoweb3d::FieldValues and Geoweb3d::PropertyGroup will soon be removed completely from the API, so for now you can call get_PropertyCollection
			Geoweb3d::GW3DResult successornot;
			long feature_id = entity_layer_.lock()->create_Feature(field_values, &working_entityinfo.wgs84_location, successornot);

			entity_database_[feature_id] = working_entityinfo;
				
			/* we want to make sure we update all the fids at startup, so mark this guy as changed */
			dirty_fids_.insert(feature_id);

		}
	}
	
	void MoveCameraToVectorData()
	{

	}

	bool UpdateAndDraw()
	{
		if (sdk_context_->draw(window_) == Geoweb3d::GW3D_sOk)
		{
			//show what they picked in in the 3d Scene
			displayVectorRepresentationInterceptReport();

			//center point was updated on begin draw
			nav_helper_.getCentralPixelLocation(&plon_lat_readback_);

			static Geoweb3d::GW3DPoint last_point_for_declutter_;
			
			if (!plon_lat_readback_->equals(&last_point_for_declutter_))
			{
				last_point_for_declutter_ = *plon_lat_readback_;
				printf("New Center location Longitude Latitude Location Detected Longitude [%f], Latitude [%f] \n", last_point_for_declutter_.get_X(), last_point_for_declutter_.get_Y());
			}

			{
				//locked because our thread is pumping locations updates etc..
				CritSectEx::Scope scope(database_thread_protection_);			/* Now we stream, which prompts the Geoweb3d::IGW3DVectorLayerStream::OnStream callback to get called for each entity */
				entity_layer_.lock()->Stream(this);

				dirty_fids_.clear();
			}


			return true;
		}

		//return that we did not draw. (app closing?)
		return false;
	}

	void StartServices()
	{
		HANDLE threadhandle = CreateThread(NULL, 0, runThreadFunction<MiniEngine>,
			makeThreadInfo(this, &MiniEngine::ThreadFunction, NULL), 0, NULL);
	}
	
	//this here is a thread
	DWORD ThreadFunction(void *parm)
	{
		while (1)
		{

			if (entity_database_.empty())
				continue;

			//randomly pick an entity to update
			int minc = 0;
			int maxc = static_cast<int>( entity_database_.size() );
			std::size_t iteratoroffset = (rand() % (maxc - minc) + minc);

			CritSectEx::Scope scope(database_thread_protection_);

			EntityDatabaseType::iterator itr = entity_database_.begin();
			std::advance(itr, iteratoroffset);

			EntityInfo &entityinfo = itr->second;
			entityinfo.wgs84_location.put_X(entityinfo.wgs84_location.get_X() + .00001);
			entityinfo.wgs84_location.put_Y(entityinfo.wgs84_location.get_Y() + .00002);
			entityinfo.wgs84_location.put_Z(entityinfo.wgs84_location.get_Z() + .005);

			entityinfo.heading_ += 5.0;

			/* Now we make sure the sdk updates this entity, as it has changed */
			dirty_fids_.insert(itr->first);
		}

		return 0;
	}
protected:
	//Geoweb3d::IGW3DEventStream event
	void OnPagingEvent(Geoweb3d::IGW3DVectorRepresentationWPtr representation, PagingEventType etype, unsigned unique_id, const Geoweb3d::GW3DEnvelope& bounds, unsigned int value) override
	{
		//note, just showing off the unsafe_get as a small optimization.  Being representation cannot get deleted here, we mine as well not play with its ref count
		if (etype == IGW3DEventStream::QuickCacheCreateFinished)
		{
			printf("Cache for this Envelope/unique_id finished : %s\n", representation.unsafe_get()->get_Driver().lock()->get_Name());
		}
		else if (etype == Geoweb3d::IGW3DEventStream::DataSourceFinishedLoading)
		{
			printf("Loading for this Envelope/unique_id finished : %s\n", representation.unsafe_get()->get_Driver().lock()->get_Name());
		}
		else if (etype == Geoweb3d::IGW3DEventStream::DataSourceWorkEstimationComplete || etype == Geoweb3d::IGW3DEventStream::DataSourceWorkEstimationComplete)
		{
			printf("Percent for this area [%s] %d \n", representation.unsafe_get()->get_Driver().unsafe_get()->get_Name(), value);
		}
	}

	virtual void OnStereoEvent( Geoweb3d::IGW3DStereoDriverWPtr stereo_driver, StereoEventType etype ) override
	{
		if ( etype == Geoweb3d::IGW3DEventStream::HMD_CONNECTED )
		{
			printf( "A HMD has been connected. \n" );
		}
		else if ( etype == Geoweb3d::IGW3DEventStream::HMD_DISCONNECTED )
		{
			printf( "A HMD has been disconnected. \n" );
		}
	}

	//Gewoeb3d::IGW3DFrustumAnalysisStream
	bool OnStream(Geoweb3d::IGW3DContentsResult *contents, Geoweb3d::IGW3DFeatureFeedbackResultCollection *featurefeedback) override
	{

		if (featurefeedback->get_WasEnabled())
		{
			Geoweb3d::IGW3DVectorRepresentationWPtr rep = contents->get_VectorRepresentation();
			Overlay2DMetadata* metadata = overlay_handler_.addRep(rep);

			featurefeedback->reset();
			int feature_count = featurefeedback->count();

			metadata->count = static_cast<size_t>(feature_count);

			if (feature_count)
			{
				Geoweb3d::IGW3DFeatureFeedbackResult* feature_result;

				while (featurefeedback->next(&feature_result))
				{
					//if the feature is not occluded by the globe and was not rendered as a model, render its icon
					if (!feature_result->get_OccludedByGlobe() &&  !feature_result->get_Rendered() )
					{
						
						overlay_handler_.ProcessFeatureFeedbackResult(contents->get_Camera(), feature_result);
					}
				}
			}
		}

		return false;
	}

	bool OnError(void)
	{
		return true;
	}

	//Geoweb3d::IGW3DWindowCallback
	virtual void OnCreate() override {};
	virtual void OnDrawBegin() override 
	{
		//just override whatever the navhelper behavior is 
		//as we want to read this back the center location.
		nav_helper_.setCentralPixelLocation();
	};
	virtual void OnDrawEnd() override {};
	virtual void OnDraw2D(const Geoweb3d::IGW3DCameraWPtr &camera) override {};
	virtual void OnDraw2D(const Geoweb3d::IGW3DCameraWPtr &camera, Geoweb3d::IGW3D2DDrawContext *drawcontext) override
	{

		//just clear our vertex array for now( can be optimize to reuse via a tracking index )
		overlay_handler_.vertex_array.clear();

		//stream our reps for analysis and build our 'display list'
		sdk_context_->get_VectorRepresentationDriverCollection()->Stream(this);

		overlay_handler_.Draw(camera);
	}
	virtual void ProcessEvent(const Geoweb3d::WindowEvent& win_event) override
	{

		//pump the mouse movement helper with the window events
		/*current_camera_ =*/ nav_helper_.ProcessEvent(win_event, window_);

		//picking with the mouse 
		IdentifyVector::ProcessEvent(win_event, window_);
	}


};

void RunApplication(Geoweb3d::IGW3DGeoweb3dSDKPtr &sdk_context)
{
	MiniEngine hello_engine(sdk_context);

	hello_engine.LoadWindowingSystem();
	hello_engine.LoadGISData();
	hello_engine.MoveCameraToVectorData();

	hello_engine.StartServices();


	while (hello_engine.UpdateAndDraw())
	{
		//could do other app stuff here
	}

}

int main()
{
	/*! Tell the engine about our error function handling */
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Information, my_info_function);
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Warning, my_info_function);
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Fatal, my_fatal_function);

	Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context(Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface());

	if (sdk_context)
	{
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
		sdk_init->put_ESRILicenseCheckout(false); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
		if (Geoweb3d::Succeeded(sdk_context->InitializeLibrary("geoweb3dsdkdemo", sdk_init, 5, 0)))
		{
			RunApplication(sdk_context);
		}
		else
		{
			printf("\nSDK Context Creation Error! Press any key to continue...\n");
			getchar();
		}
	}
	else
	{
		return -1;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	My fatal function. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="msg">	The message. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function(const char *msg)
{
	std::cout << "Fatal Info: " << msg;
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function(const char* msg)
{
	std::cout << "General Info: " << msg;
}
