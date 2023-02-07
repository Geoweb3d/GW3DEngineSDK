/**
 * Property of Geoweb3d: GIS SDK
 * --------------------------------
 * Copyright 2008-2014
 * Author: Vincent A. Autieri, Geoweb3d
 * Geoweb3d SDK is not free software: you cannot redistribute it and/or modify
 * it under any terms unless we have a written agreement between us.
 * Geoweb3d SDK and example applications are distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
**/


#include "stdafx.h"
#include <math.h>
#include "ImagePaletteCreator.h"
#include "OpenGLPaletteCreator.h"
#include "BirdAttributeMapper.h"
#include "OpenGLAttributeMapper.h"

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "core/IGW3DOpenGLTextureFinalizationToken.h"
//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"

#include "engine/IGW3DCameraManipulator.h"
#include "engine/IGW3DImageCollection.h"
#include "engine/IGW3DImage.h"

//interfaces that will be going away.
#include "Geoweb3dCore/GeometryExports.h"
#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/LayerParameters.h"

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")


void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();


/**
 *  The purpose of this example application is to create a vector layer in memory with user
 * defined points.
 */

Geoweb3d::IGW3DVectorDataSourceWPtr create_Geoweb3dDataSource(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context,  const char *datasourcename )
{
    Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");

    if(!driver.expired())
    {
        Geoweb3d::IGW3DVectorDataSourceCollection *datasource_collection = driver.lock()->get_VectorDataSourceCollection();

        return datasource_collection->create( datasourcename );
    }
    else
    {
        printf("SDK Bug Detected\n");
    }

    return Geoweb3d::IGW3DVectorDataSourceWPtr();
}


void RunApplication(  Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
    Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");

    if(driver.expired())
    {
        printf("Could  not find the driver requested!  For a list of supported drivers, run the Print Capabilites example");
        return;
    }

	if (!driver.lock()->get_IsCapable(isCreateDataSourceSupported))
	{
		printf("This driver is not capable of creating new datasources!\n");
		return;
	}

    Geoweb3d::IGW3DVectorDataSourceWPtr created_datasource = create_Geoweb3dDataSource(sdk_context, "Dynamic_Features.geoweb3d");

	//Showing a little bit of attribute mapping from the datasource too.  So we create our custom datasource, where in the field
	//we add the texture to use when we represent this feature.
    Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());
    field_definition->add_Property("Texture File or url",Geoweb3d::PROPERTY_STR, "Description of the Object's texture");
	field_definition->add_Property("OpenGLTextureID",Geoweb3d::PROPERTY_INT, "Description opengl texture");
	field_definition->add_Property("USE IP CAMERA",Geoweb3d::PROPERTY_BOOL, "Use Ip camera when true, else loads texture through the image collection");
	field_definition->add_Property("Feature Id",Geoweb3d::PROPERTY_BOOL, "unique id to test indexing out of bounds with the Image Collection");

	// create some layers
    Geoweb3d::IGW3DVectorLayerWPtr birds	= created_datasource.lock()->get_VectorLayerCollection()->create( "Birds", Geoweb3d::gtPOINT, field_definition );


	// create some features in one of the layers

    Geoweb3d::IGW3DPropertyCollectionPtr field_values (field_definition->create_PropertyCollection());

    Geoweb3d::GW3DPoint p1(-77.02000,38.89000, 0);

	Geoweb3d::GW3DResult result;


	//showing we will be able to load off disk
	//field_values->put_Property(0,"http://plazacam.studentaffairs.duke.edu/mjpg/video.mjpg");
	//field_values->put_Property( 0, "../examples/media/test_images/one.png" );
	//field_values->put_Property( 0, R"(C:\dev_nas\mobile\MMPS_UI\MMPS_UI\Resources\drawable-xxxhdpi\Airports_24.png)");
	field_values->put_Property( 0, R"(C:\dev_nas\geoweb3d_trunk_clean\geoweb3d\distribution\examples\media\Texture\bullseye_white.png)");
	field_values->put_Property( 1, 1 /*We will use 1 to say its red*/ );
	field_values->put_Property( 2, false );
	field_values->put_Property( 3, 0 );

	printf("Point FeatureID created: %d \n", birds.lock()->create_Feature(field_values, &p1, result) );

	p1.put_X(-77.02000 );
	p1.put_Y( 38.89050 );

	//showing we will be able to load from a url
    //field_values->put_Property(0,"http://217.22.201.135/mjpg/video.mjpg");
	//field_values->put_Property( 0, "../examples/media/test_images/two.png" );
	//field_values->put_Property( 0, R"(C:\dev_nas\mobile\MMPS_UI\MMPS_UI\Resources\drawable-xxxhdpi\Airways_24.png)" );
	field_values->put_Property( 0, R"(C:\dev_nas\geoweb3d_trunk_clean\geoweb3d\distribution\examples\media\Texture\bullseye_white.png)");
	field_values->put_Property( 1, 2 /*We will use 2 to say its green*/ );
	field_values->put_Property( 2, false );
	field_values->put_Property( 3, 1 );

    printf("Point FeatureID created: %d \n", birds.lock()->create_Feature(field_values, &p1, result) );

	ImagePaletteCreator palette(sdk_context, 0,2);
	birds.lock()->Stream( &palette );

	Geoweb3d::IGW3DFinalizationTokenPtr collection_token = palette.GetPalletToken();

	Geoweb3d::IGW3DVectorRepresentationDriverWPtr cylinder = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "Cylinder" );
	Geoweb3d::IGW3DVectorRepresentationDriverWPtr sphere = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "Sphere" );
	Geoweb3d::IGW3DVectorRepresentationDriverWPtr projected = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "ProjectedImage" );
	Geoweb3d::IGW3DVectorRepresentationDriverWPtr box = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "Box" );
	Geoweb3d::IGW3DVectorRepresentationDriverWPtr billboard = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "BillBoard" );
	Geoweb3d::IGW3DVectorRepresentationDriverWPtr path = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "ExtrudedPath" );
	Geoweb3d::IGW3DVectorRepresentationDriverWPtr epoly; // = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "ExtrudedPolygon" );
	Geoweb3d::IGW3DVectorRepresentationDriverWPtr epolyex = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "ExtrudedPolygonEx" );

	if ( !cylinder.expired() )
	{
		Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
		params.representation_default_parameters = cylinder.lock()->get_PropertyCollection()->create_Clone();
		params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_Z_OFFSET" ), 10.0 );
		params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_OFFSET_HEADING" ), 45.0 );
		params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_OFFSET_DISTANCE" ), 15.0 );
		Geoweb3d::IGW3DVectorRepresentationWPtr rep = cylinder.lock()->get_RepresentationLayerCollection()->create( birds, params );
		rep.lock()->put_GW3DFinalizationToken( collection_token );
		BirdAttributeMapper attribmap( palette.imagepalette, 0, 3, rep );
		birds.lock()->Stream( &attribmap );
	}

	if ( !sphere.expired() )
	{
		Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
		params.representation_default_parameters = sphere.lock()->get_PropertyCollection()->create_Clone();
		params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_Z_OFFSET" ), 4.0 );
		Geoweb3d::IGW3DVectorRepresentationWPtr rep = sphere.lock()->get_RepresentationLayerCollection()->create( birds, params );
		rep.lock()->put_GW3DFinalizationToken( collection_token );
		BirdAttributeMapper attribmap( palette.imagepalette, 0, 3, rep );
		birds.lock()->Stream( &attribmap );
	}

	Geoweb3d::IGW3DVectorRepresentationWPtr proj_rep;
	if ( !projected.expired() )
	{
		Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
		params.representation_default_parameters = projected.lock()->get_PropertyCollection()->create_Clone();
		params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_Z_OFFSET" ), 20.0 );
		params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "PITCH" ), 30.0 );
		params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "HEADING" ), 270.0 );
		proj_rep = projected.lock()->get_RepresentationLayerCollection()->create( birds, params );
		proj_rep.lock()->put_GW3DFinalizationToken( collection_token );
		BirdAttributeMapper attribmap( palette.imagepalette, 0, 3, proj_rep );
		birds.lock()->Stream( &attribmap );
	}

	if ( !box.expired() )
	{
		Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
		params.representation_default_parameters = box.lock()->get_PropertyCollection()->create_Clone();
		params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_Z_OFFSET" ), 12 );
		params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "HEADING" ), 30.0 );
		Geoweb3d::IGW3DVectorRepresentationWPtr rep = box.lock()->get_RepresentationLayerCollection()->create( birds, params );
		rep.lock()->put_GW3DFinalizationToken( collection_token );
		BirdAttributeMapper attribmap( palette.imagepalette, 0, 3, rep );
		birds.lock()->Stream( &attribmap );
	}

	if ( !billboard.expired() )
	{
		Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
		params.representation_default_parameters = billboard.lock()->get_PropertyCollection()->create_Clone();
		params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_Z_OFFSET" ), 30 );
		params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "AUTOHEIGHT" ), false );

		if ( palette.imagepalette )
		{
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "SIZE_X" ), palette.imagepalette->get_CurrentWidth() / 2 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "SIZE_Y" ), palette.imagepalette->get_CurrentHeight() / 2 );
		}

		Geoweb3d::IGW3DVectorRepresentationWPtr rep = billboard.lock()->get_RepresentationLayerCollection()->create( birds, params );
		rep.lock()->put_GW3DFinalizationToken( collection_token );

		BirdAttributeMapper attribmap( palette.imagepalette, 0, 3, rep );
		birds.lock()->Stream( &attribmap );
	}

    Geoweb3d::GW3DPoint p3( -77.02020, 38.89020, 0.0 );
	Geoweb3d::GW3DPoint p4( -77.02020, 38.89070, 0.0 );
	Geoweb3d::GW3DPoint p5( -77.02070, 38.89070, 0.0 );
	Geoweb3d::GW3DPoint p6( -77.02070, 38.89020, 0.0 );
	{
		Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());
		Geoweb3d::IGW3DPropertyCollectionPtr field_values (field_definition->create_PropertyCollection());

		Geoweb3d::GW3DLineString line;
		line.add_Point( &p3 );
		line.add_Point( &p4 );
		line.add_Point( &p5 );
		line.add_Point( &p6 );

		Geoweb3d::IGW3DVectorLayerWPtr lines = created_datasource.lock()->get_VectorLayerCollection()->create( "Line Features", Geoweb3d::gtLINE_STRING, field_definition );

		printf("Line FeatureID created: %d \n", lines.lock()->create_Feature(field_values, &line, result) );

		if ( !path.expired() )
		{
			Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
			params.representation_default_parameters = path.lock()->get_PropertyCollection()->create_Clone();
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_Z_OFFSET" ), 30 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_PALETTE_INDEX" ), 0 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_MODE_ENABLE" ), true );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "ALPHA" ), 0.5 );

			Geoweb3d::IGW3DVectorRepresentationWPtr rep = path.lock()->get_RepresentationLayerCollection()->create( lines, params );
			rep.lock()->put_GW3DFinalizationToken( collection_token );
		}
	}

	{
		Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());
		Geoweb3d::IGW3DPropertyCollectionPtr field_values (field_definition->create_PropertyCollection());

		Geoweb3d::GW3DPolygon polygon;
		Geoweb3d::GW3DLinearRing ring;

		ring.add_Point( &p3 );
		ring.add_Point( &p4 );
		ring.add_Point( &p5 );
		ring.add_Point( &p6 );
		ring.add_Point( &p3 );

		polygon.add_Ring( &ring );

		Geoweb3d::IGW3DVectorLayerWPtr polygons = created_datasource.lock()->get_VectorLayerCollection()->create( "Polygon Features", Geoweb3d::gtPOLYGON, field_definition );
		printf("Polygon FeatureID created: %d \n", polygons.lock()->create_Feature(field_values, &polygon, result) );

		if ( !epoly.expired() )
		{
			Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
			params.representation_default_parameters = epoly.lock()->get_PropertyCollection()->create_Clone();
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_Z_OFFSET" ), 15 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "EXTRUSION_HEIGHT" ), 8 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_MODE_ENABLE" ), true );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_SIDE_PALETTE_INDEX" ), 0 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_TOP_PALETTE_INDEX" ), 1 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_TOP_REPEAT_N_TIMES_X" ), 2.0 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_TOP_REPEAT_N_TIMES_Y" ), 2.0 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_SIDE_REPEAT_N_TIMES_X" ), 1.0 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_SIDE_REPEAT_N_TIMES_Y" ), 1.0  );

			Geoweb3d::IGW3DVectorRepresentationWPtr rep = epoly.lock()->get_RepresentationLayerCollection()->create( polygons, params );
			rep.lock()->put_GW3DFinalizationToken( collection_token );
		}

		if ( !epolyex.expired() )
		{
			Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
			params.representation_default_parameters = epolyex.lock()->get_PropertyCollection()->create_Clone();
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_Z_OFFSET" ), 0 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "EXTRUSION_HEIGHT" ), 8 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_MODE_ENABLE" ), true );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_SIDE_PALETTE_INDEX" ), 0 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_TOP_PALETTE_INDEX" ), 1 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_TOP_REPEAT_N_TIMES_X" ), 2.0 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_TOP_REPEAT_N_TIMES_Y" ), 2.0 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_SIDE_REPEAT_N_TIMES_X" ), 1.0 );
			params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_SIDE_REPEAT_N_TIMES_Y" ), 1.0  );

			Geoweb3d::IGW3DVectorRepresentationWPtr rep = epolyex.lock()->get_RepresentationLayerCollection()->create( polygons, params );
			rep.lock()->put_GW3DFinalizationToken( collection_token );
		}
	}


	Geoweb3d::IGW3DWindowWPtr window = sdk_context->get_WindowCollection(  )->create_3DWindow("Textured Representations", GW3D_OVERLAPPED, 10, 10, 1280,720, 0, Geoweb3d::IGW3DStereoDriverPtr(), 0);
	
    sdk_context->draw( window );

	Geoweb3d::IGW3DCameraWPtr camera1 = window.lock()->get_CameraCollection()->create("Main Camera");

	//figure out where to zoom

    Geoweb3d::IGW3DVectorLayerCollection *vlayercollection=  created_datasource.lock()->get_VectorLayerCollection();

    Geoweb3d::IGW3DVectorLayerWPtr tester;
	Geoweb3d::GW3DEnvelope env;
    while(vlayercollection->next( &tester)) //just to see
    {
        printf("Vector Layer: [%s]\n",tester.lock()->get_Name());
		env.merge( tester.lock()->get_Envelope() );
		
    }

  
    double longitude, latitude;
    longitude = (env.MinX + env.MaxX ) * 0.5;
    latitude  = (env.MinY + env.MaxY ) * 0.5;
	
	
	Geoweb3d::IGW3DBearingHelperPtr bearinghelder = Geoweb3d::IGW3DBearingHelper::create();
	bearinghelder->put_TargetLocation(p1.get_X( ),p1.get_Y( ),0);

	float camele = 60;
	camera1.lock()->get_CameraController()->put_Elevation(camele, Geoweb3d::IGW3DPositionOrientation::Relative );

	int use_raw_openglcount = 1;

	float counter = 0;
	while( sdk_context->draw( window )==Geoweb3d::GW3D_sOk )
	{
	   //rotate around the target
	   double dummyele;
	   double dummylat;
	   double dummylon;
	   bearinghelder->get_TargetLocation(dummylon,dummylat,dummyele);
	   double camlongitude = dummylon  + .001 * cos( counter );
	   double camlatitude  = dummylat  + .001 * sin( counter );
	  
	   camera1.lock()->get_CameraController()->put_Location( camlongitude, camlatitude);
	   bearinghelder->put_SourceLocation(camlongitude, camlatitude,camele);
	 
	   counter+=.01f;
	  

	   double bearing,pitch,distance;
	   bearinghelder->calculate_result(bearing,pitch,distance);
	   camera1.lock()->get_CameraController()->put_Rotation( static_cast<float>(bearing), static_cast<float>(pitch), 0.0f );
	
	   --use_raw_openglcount;

	   if ( use_raw_openglcount == 0 )
	   {
		   // we should be in an opengl context of the
		   //sdk here... as well as in the 2D callback.
		   //its just not safe to draw to the opengl buffers
		   //at this point.
		   Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
		   if ( box.expired() )
		   {
			   printf( "Box representation has expired!\n" );
		   }

		   const Geoweb3d::IGW3DPropertyCollection* box_default_properties = box.lock()->get_PropertyCollection();
		   params.representation_default_parameters = box_default_properties->create_Clone();
		   params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_Z_OFFSET" ), 20 );
		   Geoweb3d::IGW3DVectorRepresentationWPtr rep = box.lock()->get_RepresentationLayerCollection()->create( birds, params );

		   OpenGLPaletteCreator opengl_palette( sdk_context, 1/*one is the color property index in the datasource layer*/ );
		   birds.lock()->Stream( &opengl_palette );

		   rep.lock()->put_GW3DFinalizationToken( opengl_palette.GetPalletToken() ); 
		   //proj_rep.lock()->put_GW3DFinalizationToken( opengl_palette.GetPalletToken() );

		   OpenGLAttributeMapper attribmap( opengl_palette.palette, 1/*one is the color property index in the datasource layer*/, rep );
		   birds.lock()->Stream( &attribmap );
	   }
   }
}

int _tmain(int argc, _TCHAR* argv[])
{

    SetInformationHandling( );

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if(sdk_context)
    {
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
		sdk_init->put_ESRILicenseCheckout(false); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
        {
            RunApplication( sdk_context );
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

    //example to show if you want to control its when the sdk context gets destroyed.  This will invalidate all
    // the pointers the SDK owned!  (xxx.expired() ==true)
    sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();

    return 0;
}


/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function(const char *msg )
{
    printf("Fatal Info: %s", msg);
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function(const char *msg )
{
    printf("General Info: %s", msg);
}

/*! Information handling is not required */
void SetInformationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}
