/**
* Property of Geoweb3d: GIS SDK
* --------------------------------
* Copyright 2008-2019
* Author: Vincent A. Autieri, Geoweb3d
* Geoweb3d SDK is not free software: you cannot redistribute it and/or modify
* it under any terms unless we have a written agreement between us.
* Geoweb3d SDK and example applications are distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.
**/

#include "stdafx.h"
#include <windows.h>
#include <deque>
#include <string>
#include <locale>
#include <algorithm>
#include <set>
#include <map>
#include <ctype.h>
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DRaster.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "GeoWeb3dCore/SystemExports.h"

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

void my_fatal_function(const char *msg );
void my_info_function(const char *msg);
void SetInformationHandling();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the application operation. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">	Context for the sdk. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void RunApplication(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context)
{

	Geoweb3d::IGW3DWindowCollection * wcol = sdk_context->get_WindowCollection();

	Geoweb3d::IGW3DWindowWPtr window = wcol->create_3DWindow("Moving Specific Features", GW3D_OVERLAPPED, 50, 50, 800, 600, 0, Geoweb3d::IGW3DStereoDriverPtr(), nullptr);

	if (window.expired())
	{
		printf("Error Creating window\n");

		return;
	}

	Geoweb3d::IGW3DCameraCollection *ccol = window.lock()->get_CameraCollection();

	Geoweb3d::IGW3DCameraWPtr camera = ccol->create("A-Camera");

	/////////////////////////////////
	// lets create our entities! 
	////////////////////////////////

	Geoweb3d::IGW3DVectorDriverWPtr datasource_driver = sdk_context->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");

	if (datasource_driver.expired())
	{
		printf("Data source driver not found, you can print out all the datasource driver names by looping over the get_VectorDriverCollection\n");
		return;
	}
	
	//so what attributes do we want to store about our moving features? You don't have to do this, but
	//it *may* help you attribute map later on, but nothing is stooping you from using our own structures
	//to attribute map.  What I mean is, if you have a feature with Name "ABC123", and you have this represented
	//as a sphere in the 3D scene, you can have all sphere's green by default, but for "ABC123" make it red.  You 
	//do not need to "stream" to do attribute mapping either, but why not..makes it easy, and you can move your geometry
	//at the same time you stream.....
	Geoweb3d::IGW3DDefinitionCollectionPtr fdcol_header_title_whatever(Geoweb3d::IGW3DDefinitionCollection::create());
	fdcol_header_title_whatever->add_Property("SomethingIWantToStoreSuchAs_NAME", Geoweb3d::PROPERTY_STR, "entity_name");

	//so now we created our datasource, but a datasource can have 1 through n layers in it, which is whatever the spec of the datasource driver is!  A geoweb3d datasource is unlimited
	Geoweb3d::IGW3DVectorDataSourceCollection *dcol = datasource_driver.lock()->get_VectorDataSourceCollection();

	//Geoweb3d_Datasource can take any name... I'm using .gw3d just to help drive home the point if you used a shapefile driver for example.
	Geoweb3d::IGW3DVectorDataSourceWPtr dynamic_features_datasource = dcol->create("MovingFeatures_Entities_Whatever.gw3d"); //if this was a "disk" based datasource driver, you will see the filename created at that path given


	//describe our layer that we will now populate.  You will see it has a name, the geometry type, and the description(definition/attributes) of each feature
	Geoweb3d::IGW3DVectorLayerWPtr entity_layer = dynamic_features_datasource.lock()->get_VectorLayerCollection()->create("MovingEntitiesLayer", Geoweb3d::gtPOINT_25D, fdcol_header_title_whatever /*So we just described our layer*/);

	//moving billboards, only  moving billboards when it comes to point type reps needs to have all the features created
	//up front at the time of this example. This is a short term limitation with billboards which will be addressed in the future.
	//NOTE, if you do not add all up front, then you will need to call representation refresh on the layer after you add
	//new features, but you may see a frame flicker in the 3D scene (this may be acceptable to some vs creating all the features up front)
	
	//so now from our definition/attributes we want to store along with the feature's location..
	Geoweb3d::IGW3DPropertyCollectionPtr fcolfeature_attributes = fdcol_header_title_whatever->create_PropertyCollection();
	fcolfeature_attributes->put_Property(fcolfeature_attributes->get_DefinitionCollection()->get_IndexByName("SomethingIWantToStoreSuchAs_NAME"), "feature_id_A_NAME_is_BUDDY");

	//its geometry
	Geoweb3d::GW3DPoint starting_location(-75.9833, 42.2167, 3);

	Geoweb3d::GW3DResult retval;
	long feature_id_A = entity_layer.lock()->create_Feature(fcolfeature_attributes.get(), &starting_location, retval);
	//so you can think of feature_id_A as a handle, as its how control this guy in both the vector layer, as well as the 
	//representation in the 3D map to give it unique appearances.

	//error check against retval!

	fcolfeature_attributes->put_Property(fcolfeature_attributes->get_DefinitionCollection()->get_IndexByName("SomethingIWantToStoreSuchAs_NAME"), "feature_id_B_NAME_is_NUTTY");

	starting_location.put_X(-75.9832);
	starting_location.put_Y(42.2167);
	starting_location.put_Z(2); 

	long feature_id_B = entity_layer.lock()->create_Feature(fcolfeature_attributes.get(), &starting_location, retval);
	//error check against retval!
	//so you can think of feature_id_B as a handle, as its how control this guy in both the vector layer, as well as the 
	//representation in the 3D map to give it unique appearances.

	fcolfeature_attributes->put_Property(fcolfeature_attributes->get_DefinitionCollection()->get_IndexByName("SomethingIWantToStoreSuchAs_NAME"), "feature_id_B_NAME_is_BAR");

	starting_location.put_X(-75.9832);
	starting_location.put_Y(42.2166);
	starting_location.put_Z(2);

	long feature_id_C = entity_layer.lock()->create_Feature(fcolfeature_attributes.get(), &starting_location,retval);
	//error check against retval!
	//so you can think of feature_id_C as a handle, as its how control this guy in both the vector layer, as well as the 
	//representation in the 3D map to give it unique appearances.


	//ok, so we also want to put the engine in an optimization mode for moving things around vs static data...
	//we do this by setting the vector layer to an editable mode.  If you don't do this, you don't have 
	//a way to move features already inserted as well.
	entity_layer.lock()->put_GeometryEditableMode(true);

	//now lets add the vectorlayer created above to the 3D map...  NOTE you can have multiple map representations off
	//the same vector layer.  One use case may be where you have a sphere as the real world location, but then went
	//a billboard also at that location but offset a bit from the sphere (billboard is more of a screenspace visual)
	Geoweb3d::Vector::RepresentationLayerCreationParameter construction_defaults;
	
	Geoweb3d::IGW3DVectorRepresentationWPtr entities_in_3Dmap =
		sdk_context->get_VectorRepresentationDriverCollection()->get_Driver("BillBoard").lock()
		->get_RepresentationLayerCollection()->create( entity_layer , construction_defaults);


	//lets put the camera directly over the data above...  I'm familiar witht he clamp distance defaults
	//and page distance defaults, which is why I'm only going up 1000 meters, as I know for an example
	//app they will see the 3 things, be it boxes, planes, spheres, models, billboards, etc.
	camera.lock()->get_CameraController()->put_Location(starting_location.get_X(), starting_location.get_Y());
	camera.lock()->get_CameraController()->put_Elevation(1000, Geoweb3d::IGW3DPositionOrientation::Relative);

	while (sdk_context->draw( window ) == Geoweb3d::GW3D_sOk)
	{
		//class here to keep it close to you for learning how things work
		class FeatureAKAEntityController : public Geoweb3d::IGW3DVectorLayerStream, public Geoweb3d::IGW3DGeometryVisitor
		{
		public:
			FeatureAKAEntityController(long feature_to_control) : feature_to_control_(feature_to_control) {}
			//Geoweb3d::IGW3DVectorLayerStream
			virtual bool OnStream(Geoweb3d::IGW3DVectorLayerStreamResult *result) override
			{
				Geoweb3d::GW3DGeometry * geometry = result->get_EditableGeometry();

				if (!geometry) //must be we are not in edit mode????
					return false;

				//process with the visitor pattern so we don't need to cast.
				geometry->accept(this);

				return true;
			}
			virtual bool OnError() override
			{
				return true;
			}

			// These are required to be implemented!  *NOTE* if
			// [count] returns 0, then no other of APIs to index
			// into selection_set_fids_ will get called.  What this
			// means is if you are streaming a whole layer, its safe to
			// return 0 for everything.

			virtual unsigned long count() const
			{
				return 1; //moving only 1 feature
			}

			virtual bool next(int64_t *ppVal)
			{
				*ppVal = feature_to_control_;
				return true;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Resets this object. </summary>
			///
			/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			virtual void reset()
			{

			}

			virtual int64_t operator[](unsigned long index)
			{
				return feature_to_control_;
			}
			virtual int64_t get_AtIndex(unsigned long index)
			{
				return feature_to_control_;
			}

			//Geoweb3d::IGW3DGeometryVisitor
			virtual void visit(Geoweb3d::GW3DPoint* pt) override
			{
				if (pt->is_3D())
				{
					pt->put_Z(pt->get_Z() + 1);
				}

				pt->put_X( pt->get_X() + .00001);
			}
			virtual void visit(Geoweb3d::GW3DLineString*) override
			{
			
			}
			virtual void visit(Geoweb3d::GW3DLinearRing*) override
			{
			
			}
			virtual void visit(Geoweb3d::GW3DPolygon*) override
			{
			
			}
			virtual void visit(Geoweb3d::GW3DMultiPoint*)override
			{
			
			}
			virtual void visit(Geoweb3d::GW3DMultiLineString*)override
			{
			
			}
			virtual void visit(Geoweb3d::GW3DMultiPolygon*)override
			{
			
			}

			long feature_to_control_;
		};

		static int moveflipcounter = 0;

		if (moveflipcounter < 60)
		{
			FeatureAKAEntityController move_and_changer_stream(feature_id_A);
			entity_layer.lock()->Stream(&move_and_changer_stream);
		}
		else if (moveflipcounter < 60 * 2)
		{
			FeatureAKAEntityController move_and_changer_stream(feature_id_B);
			entity_layer.lock()->Stream(&move_and_changer_stream);
		}
		else if (moveflipcounter < 60 * 3)
		{
			FeatureAKAEntityController move_and_changer_stream(feature_id_C);
			entity_layer.lock()->Stream(&move_and_changer_stream);
		}
		else
		{
			moveflipcounter = 0;
		}

		moveflipcounter++;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	SetInformationHandling();

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

	sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();

	return 0;
}


/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	My fatal function. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="msg">	The message. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	My fatal function. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="msg">	The message. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void my_fatal_function(const char *msg )
{
	printf("Fatal Info: %s", msg);
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function(const char *msg )
{
	printf("General Info: %s", msg);
}

void SetInformationHandling()
{
	/*! Tell the engine about our error function handling */
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Information, my_info_function);
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Warning, my_info_function);
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Fatal, my_fatal_function);
}
