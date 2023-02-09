#include "MiniEngine.h"

#include "TerrainAvoidanceHandler.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/IGW3DClampRadius.h"
#include "engine/IGW3DMediaCenter.h"
#include "engine/IGW3DImage.h"
#include "engine/GW3DRaster.h"

//interfaces that will be depricated
#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/LayerParameters.h"

#include <deque>
#include <map>
#include <iostream>

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsExtruded( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer )
{

    Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "ExtrudedPolygon" );


    Geoweb3d::GW3DResult validitychk;

    if(!Geoweb3d::Succeeded( validitychk =  driver.lock()->get_CapabilityToRepresent( layer ) ))
    {
        printf("Asked to represent a vector layer with a representation, but the layer is not compatible!\n");
    }
    else
    {
        // prints the values the layers has to help the user know what they can attribute map into the representation
        const Geoweb3d::IGW3DDefinitionCollection* def = layer.lock()->get_AttributeDefinitionCollection();

        printf("Attribute Names within the layer: [%s] you can map to the a extruded polygon property: \n", layer.lock()->get_Name() );
        for(unsigned i = 0; i < def->count(); ++i )
        {
            printf("\t[%s]\n", def->get_AtIndex(i)->property_name );
        }


		Geoweb3d::IGW3DPropertyCollectionPtr defaults = driver.lock()->get_PropertyCollection()->create_Clone();
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::RED, 1.0);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::GREEN, 0.5);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::BLUE, 1.0);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::EXTRUSION_HEIGHT, 100.0);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::LayerDefaultsOnlyProperties::OIT, Geoweb3d::Vector::OITMode::STIPPLE_MODE);

        //good to go!
        Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
		params.representation_default_parameters = defaults;
        return driver.lock()->get_RepresentationLayerCollection()->create( layer, params );
    }

    return Geoweb3d::IGW3DVectorRepresentationWPtr();
}

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsPointCloud(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer)
{

	Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver("PointCloud");

	//Geoweb3d::GW3DResult validitychk;

	//NOTE - not hooked up yet for point cloud
	//if (!Geoweb3d::Succeeded(validitychk = driver.lock()->get_CapabilityToRepresent(layer)))
	//{
	//	printf("Asked to represent a vector layer with a representation, but the layer is not compatible!\n");
	//}
	//else
	{

		Geoweb3d::IGW3DPropertyCollectionPtr defaults = driver.lock()->get_PropertyCollection()->create_Clone();
		defaults->put_Property(Geoweb3d::Vector::PointCloudProperties::TRANSLATION_Z_OFFSET, 100.0);

		//good to go!
		Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
		params.representation_default_parameters = defaults;
		return driver.lock()->get_RepresentationLayerCollection()->create(layer, params);
	}

	return Geoweb3d::IGW3DVectorRepresentationWPtr();
}

	MiniEngine::MiniEngine( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context ) 
		: sdk_context_( sdk_context )
		, navHelper_ (new NavigationHelper())
		, terrain_avoidance_handler_( new TerrainAvoidanceHandler( sdk_context ) )
		, elevation_driver(sdk_context->get_RasterLayerRepresentationDriverCollection()->get_Driver( "Elevation" ))
		, detail_mode_(Geoweb3d::IGW3DTerrainAvoidanceTest::IncreasedDetailMode::UseTerrainLODValue)
	{}

	MiniEngine::~MiniEngine()
	{
		delete navHelper_;
	}

    bool MiniEngine::Update()
    {
        DoPreDrawWork();

		if( sdk_context_->draw( window_ ) == Geoweb3d::GW3D_sOk )
        {
            DoPostDrawWork();
            return true;
        }

        //return that we did not draw. (app closing?)
        return false;
    }

    bool MiniEngine::LoadConfiguration( const char* example_name )
    {
        window_ = Create3DWindow( sdk_context_, example_name, this );

        if( window_.expired() )return false;

        camera_ = CreateCamera( window_, "Camera 1" );
        camera_controller_ = camera_.lock()->get_CameraController();

        if( camera_.expired() )
			return false;

        sdk_context_->put_EnableEventStream( true );

        return true;
    }

    bool MiniEngine::LoadData( const char* rootpath )
    {

        //just loading up vector data so I can try to make it easy
        //or someone to load additional or other data.  In a real application
        //you would probably want to keep your vector datasources in your own
        //container so you don't have to always loop through the dozens of drivers
        //to find the one you are looking for
        std::deque<std::string> vectordata;

        vectordata.push_back( rootpath );
        vectordata.back().append( "/Building Footprints.shp" );

		Geoweb3d::GW3DEnvelope env;

        for( unsigned i = 0; i < vectordata.size(); ++i )
        {
			Geoweb3d::GW3DResult res;
			//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
			//having to guess which data source driver is needed to open up a particular dataset.
            Geoweb3d::IGW3DVectorDataSourceWPtr data_source = sdk_context_->get_VectorDriverCollection()->auto_Open( vectordata[i].c_str(), res );
			
			//testing a failure case
			if (data_source.expired())
			{
				std::cout << "Could not load the following Vector Data Source: " << vectordata[i].c_str();
			}
			else
			{
				Geoweb3d::IGW3DVectorLayerCollection* vlyrcollection = data_source.lock()->get_VectorLayerCollection();
				Geoweb3d::IGW3DVectorLayerWPtr layer;
				
				while (vlyrcollection->next(&layer))
				{
					extruded_representation_ = RepresentAsExtruded(sdk_context_, layer);

					env.merge(layer.lock()->get_Envelope());
				}
			}
        }


        // Now load the elevation data
		std::string elevationpath( rootpath );
        elevationpath += "/NED 10-meter DC.tif";

		Geoweb3d::IGW3DRasterDriverCollection *raster_drivers = sdk_context_->get_RasterDriverCollection();
		Geoweb3d::GW3DResult res;

		//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
		//having to guess which data source driver is needed to open up a particular dataset.
		Geoweb3d::IGW3DRasterDataSourceWPtr rdatasource = raster_drivers->auto_Open(elevationpath.c_str(), res);

		if (CreateElevationRep(sdk_context_, elevationpath.c_str(), 6, 0, true, 6.0).expired())
		{
			printf("CreateElevationRep Error\n");
		}

		//add rings and models into the scene
		Geoweb3d::IGW3DVectorDataSourceWPtr dc_lines_data_source = sdk_context_->get_VectorDriverCollection()->auto_Open("../examples/media/DC_LINES.shp", res);
		if (dc_lines_data_source.expired())
		{
			std::cout << "Could not load the following Vector Data Source\n";
		}
		else
		{

			Geoweb3d::IGW3DVectorLayerCollection* vlyrcollection = dc_lines_data_source.lock()->get_VectorLayerCollection();
			Geoweb3d::IGW3DVectorLayerWPtr layer;
			Geoweb3d::IGW3DVectorRepresentationDriverWPtr draped_ring_driver;
			Geoweb3d::IGW3DVectorRepresentationDriverWPtr model_driver;

			while (vlyrcollection->next(&layer))
			{
				draped_ring_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("DrapedRing");
				model_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("Model");
				Geoweb3d::IGW3DPropertyCollectionPtr draped_ring_properties_ = draped_ring_driver.lock()->get_PropertyCollection()->create_Clone();
				struct prop_state
				{
					int ss_outline_width_;
					int ss_width_;
					float alpha_;
					float inner_rad_;
					float outer_rad_;
					float texture_blend_;
					float red_;
					float green_;
					float blue_;
					Geoweb3d::Vector::DrapedRingProperties::DrapedRingLightingMode lighting_;
				};
				prop_state default_state_draped_ring_prop_ = {
					2,// outline_width_
					10,// ss_width_
					1.0,// alpha_
					50.0,// inner_rad_
					100.0,// outer_rad_
					0.0,// texture_blend_
					1.0,// red_
					0.0,// green_
					0.0,// blue_
					Geoweb3d::Vector::DrapedRingProperties::DrapedRingLightingMode::UNSHADED
				};
				draped_ring_properties_->put_Property(Geoweb3d::Vector::DrapedRingProperties::IndividualProperties::MIN_SCREEN_SPACE_EXTENT, default_state_draped_ring_prop_.ss_width_);
				draped_ring_properties_->put_Property(Geoweb3d::Vector::DrapedRingProperties::IndividualProperties::OUTLINE_MODE, default_state_draped_ring_prop_.ss_outline_width_);
				draped_ring_properties_->put_Property(Geoweb3d::Vector::DrapedRingProperties::IndividualProperties::LIGHTING_MODE, default_state_draped_ring_prop_.lighting_);
				draped_ring_properties_->put_Property(Geoweb3d::Vector::DrapedRingProperties::IndividualProperties::INNER_RAD, default_state_draped_ring_prop_.inner_rad_);
				draped_ring_properties_->put_Property(Geoweb3d::Vector::DrapedRingProperties::IndividualProperties::OUTER_RAD, default_state_draped_ring_prop_.outer_rad_);
				draped_ring_properties_->put_Property(Geoweb3d::Vector::DrapedRingProperties::IndividualProperties::ALPHA, default_state_draped_ring_prop_.alpha_);
				draped_ring_properties_->put_Property(Geoweb3d::Vector::DrapedRingProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND, default_state_draped_ring_prop_.texture_blend_);
				draped_ring_properties_->put_Property(Geoweb3d::Vector::DrapedRingProperties::IndividualProperties::RED, default_state_draped_ring_prop_.red_);
				draped_ring_properties_->put_Property(Geoweb3d::Vector::DrapedRingProperties::IndividualProperties::GREEN, default_state_draped_ring_prop_.green_);
				draped_ring_properties_->put_Property(Geoweb3d::Vector::DrapedRingProperties::IndividualProperties::BLUE, default_state_draped_ring_prop_.blue_);
				draped_ring_properties_->put_Property(Geoweb3d::Vector::DrapedRingProperties::IndividualProperties::TEXTURE_PALETTE_INDEX, -1);

				Geoweb3d::GW3DResult validitychk;

				if (!Geoweb3d::Succeeded(validitychk = draped_ring_driver.lock()->get_CapabilityToRepresent(layer)))
				{
					printf("not able to mix this geometry type with the draped line rep..\n");
				}
				else
				{
					Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
					params.page_level = 1;
					params.representation_default_parameters = draped_ring_properties_;
					draped_ring_rep_ = draped_ring_driver.lock()->get_RepresentationLayerCollection()->create(layer, params);

					draped_ring_rep_.lock()->get_ClampRadiusFilter()->put_Enabled(true);
					draped_ring_rep_.lock()->get_ClampRadiusFilter()->put_MaxRadius(100000.0);
					draped_ring_rep_.lock()->get_ClampRadiusFilter()->put_MinRadius(0.0);
					draped_ring_rep_.lock()->get_ClampRadiusFilter()->put_MaxRadiusTransitionRange(30000.0);
				}

				if (!Geoweb3d::Succeeded(validitychk = draped_ring_driver.lock()->get_CapabilityToRepresent(layer)))
				{
					printf("not able to mix this geometry type with the model rep..\n");
				}
				else
				{
					Geoweb3d::IGW3DPropertyCollectionPtr model_properties_ = model_driver.lock()->get_PropertyCollection()->create_Clone();
					model_properties_->put_Property(Geoweb3d::Vector::ModelProperties::SCALE_X, 25.0);
					model_properties_->put_Property(Geoweb3d::Vector::ModelProperties::SCALE_Y, 25.0);
					model_properties_->put_Property(Geoweb3d::Vector::ModelProperties::SCALE_Z, 25.0);

					Geoweb3d::Vector::RepresentationLayerCreationParameter  params2;
					params2.page_level = 4;
					params2.representation_default_parameters = model_properties_;
					model_rep_ = model_driver.lock()->get_RepresentationLayerCollection()->create(layer, params2);
				}
			}

		}

		//a more interesting view
		camera_.lock()->get_CameraController()->put_Location(-76.952323363131512, 38.847788035597276);
		camera_.lock()->get_CameraController()->put_Elevation(1744.1317362794653, Geoweb3d::IGW3DPositionOrientation::Absolute);
		camera_.lock()->get_CameraController()->put_Rotation(-46.f, 23.f, 0.0f);

		//look at precision
		//camera_.lock()->get_CameraController()->put_Rotation(46.0000153f, 46.8000031f, 0.0f);
		//camera_.lock()->get_CameraController()->put_Location(-77.033454513278670, 38.899037823198611);
		//camera_.lock()->get_CameraController()->put_Elevation(4006.639947173185647, Geoweb3d::IGW3DPositionOrientation::Relative);
		camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);
		navHelper_->put_HomePosition(camera_);
		terrain_avoidance_handler_->ToggleTerrainAvoidance(camera_);
        return true;
    }

	Geoweb3d::IGW3DWindowWPtr MiniEngine::Create3DWindow( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events)
	{
		Geoweb3d::IGW3DWindowCollection * wcol = sdk_context->get_WindowCollection(  );

		Geoweb3d::IGW3DWindowWPtr window = wcol->create_3DWindow(title, GW3D_OVERLAPPED, 10, 10, 1280,720, 0, Geoweb3d::IGW3DStereoDriverPtr(), window_events);

		if(window.expired())
		{
			printf("Error Creating window: [%s]!\n", title );

			return Geoweb3d::IGW3DWindowWPtr();
		}

		return window;
	}

	Geoweb3d::IGW3DCameraWPtr MiniEngine::CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle )
	{

		Geoweb3d::IGW3DCameraCollection *cameracollection = windowptr.lock()->get_CameraCollection();
		Geoweb3d::IGW3DCameraWPtr camera =	cameracollection->create( cameratitle );

		if(!camera.expired())
		{
			printf("DelimitedString: %s\n",   camera.lock()->get_DateTime()->get_DateFromDelimitedString()->c_str());
			printf("UnDelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromUnDelimitedString()->c_str());

			camera.lock()->get_DateTime()->put_isUseComputerDate(false);
			camera.lock()->get_DateTime()->put_isUseComputerTime(false);


			if(!camera.lock()->get_DateTime()->put_DateFromDelimitedString("2002-1-25"))
			{
				printf("Error with put_DateFromDelimitedString\n");
			}

			if(!camera.lock()->get_DateTime()->put_DateFromUnDelimitedString("2002125"))
			{
				printf("Error with put_DateFromUnDelimitedString\n");
			}


			printf("User Set: DelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromDelimitedString()->c_str());
			printf("User Set: UnDelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromUnDelimitedString()->c_str());


			camera.lock()->get_DateTime()->put_Time(12,0);
			//go back to using the date and time of this computer
			// camera.lock()->get_DateTime()->put_isUseComputerDate(true);
			// camera.lock()->get_DateTime()->put_isUseComputerTime(true);

			navHelper_->add_Camera(camera);
		}

		return camera;
	} 

	Geoweb3d::IGW3DCameraWPtr MiniEngine::GetCamera()
	{
		return camera_;
	}

    void MiniEngine::DoPreDrawWork()
    {
        //process all the vector pipeline events
    }

    void MiniEngine::DoPostDrawWork()
    {

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	call back for 2d drawing/client side. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="camera">	The camera. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void MiniEngine::OnDraw2D(const Geoweb3d::IGW3DCameraWPtr &camera)
    {
        // Draw icons in 2D or just to find out whats in view. If you are drawing in OpenGL, at this
        // point we have an OpenGL 3.3 render context inside the SDK.
     //   CameraFrustumContentsTest frustum_contents;
      //  sdk_context_->get_VectorRepresentationDriverCollection()->Stream( &frustum_contents );
    }


    void MiniEngine::ProcessEvent( const Geoweb3d::WindowEvent& win_event )
    {
		bool filter_out = false;


		switch( win_event.Type )
        {
			case win_event.KeyPressed:
			{
				if( !camera_.expired() )
				{
					float heading, pitch, roll;
					camera_controller_->get_Rotation( heading, pitch, roll );

					switch(	win_event.Key.code )
					{
						case Geoweb3d::Key::Z:
						{
							static bool toggle_rep_in_analytic = true;
							terrain_avoidance_handler_->TurnOnRepInAnalytic(draped_ring_rep_, toggle_rep_in_analytic);
							terrain_avoidance_handler_->TurnOnRepInAnalytic(extruded_representation_, toggle_rep_in_analytic);
							terrain_avoidance_handler_->TurnOnRepInAnalytic(model_rep_, toggle_rep_in_analytic);
							terrain_avoidance_handler_->TurnOnRepInAnalytic(point_cloud_rep_, toggle_rep_in_analytic);
							
							printf("Representations %s for TA Mask\n", toggle_rep_in_analytic ? "Included" : "Excluded");

							toggle_rep_in_analytic = !toggle_rep_in_analytic;
							filter_out = true;
						}
						break;

						case Geoweb3d::Key::V:
						{
							terrain_avoidance_handler_->ToggleTerrainAvoidance(camera_);
							filter_out = true;
						}
						break;

						case Geoweb3d::Key::C:
						{
							static bool toggle_ref_alt = true;
							toggle_ref_alt = !toggle_ref_alt;
							if (toggle_ref_alt)
							{
								terrain_avoidance_handler_->put_ReferenceAltitudeMode(Geoweb3d::IGW3DTerrainAvoidanceTest::ReferenceAltitudeMode::Value);
								printf("ReferenceAltitudeMode is Value\n");
							}
							else
							{
								terrain_avoidance_handler_->put_ReferenceAltitudeMode(Geoweb3d::IGW3DTerrainAvoidanceTest::ReferenceAltitudeMode::Camera);
								printf("ReferenceAltitudeMode is Camera\n");
							}
							filter_out = true;
						}
						break;

						case Geoweb3d::Key::X:
						{
							static double ref_alt = 0.f;
							if (win_event.Key.shift)
							{
								terrain_avoidance_handler_->get_ReferenceAltitude(ref_alt);
								ref_alt -= 10.0;
								if (ref_alt < 0.0)
								{
									ref_alt = 0.0;
								}
							}
							else
							{
								terrain_avoidance_handler_->get_ReferenceAltitude(ref_alt);
								ref_alt += 10.0;
							}

							terrain_avoidance_handler_->put_ReferenceAltitude(ref_alt);
							terrain_avoidance_handler_->get_ReferenceAltitude(ref_alt);
							printf("Reference Alt Value is: %7.1f\n", ref_alt);

							filter_out = true;
						}
						break;

						case Geoweb3d::Key::B:
						{
							static double linear_lod = 8.0;
							if (win_event.Key.shift)
							{
								linear_lod -= 0.5;
							}
							else
							{
								linear_lod += 0.5;
							}
							linear_lod = std::clamp(linear_lod, 4.0, 12.0);
							terrain_avoidance_handler_->put_TerrainLinearLOD(linear_lod);
							printf("Linear LOD %2.1f\n", linear_lod);
							filter_out = true;
						}
						break;

						case Geoweb3d::Key::N:
						{
							if (detail_mode_ == Geoweb3d::IGW3DTerrainAvoidanceTest::IncreasedDetailMode::NoExtraDetail)
							{
								detail_mode_ = Geoweb3d::IGW3DTerrainAvoidanceTest::IncreasedDetailMode::UseTerrainLODValue;
								printf("IncreasedDetailMode %s\n", "UseTerrainLODValue");
							}
							else if (detail_mode_ == Geoweb3d::IGW3DTerrainAvoidanceTest::IncreasedDetailMode::UseTerrainLODValue)
							{
								detail_mode_ = Geoweb3d::IGW3DTerrainAvoidanceTest::IncreasedDetailMode::UseTerrainLODValueAndFastLoad;
								printf("IncreasedDetailMode %s\n", "UseTerrainLODValueAndFastLoad");
							}
							else
							{
								detail_mode_ = Geoweb3d::IGW3DTerrainAvoidanceTest::IncreasedDetailMode::NoExtraDetail;
								printf("IncreasedDetailMode %s\n", "NoExtraDetail");
							}

							terrain_avoidance_handler_->put_IncreasedDetailMode(detail_mode_);
							filter_out = true;
						}
						break;

						default:
						break;
					};
				}
			}
			break;
			default:
			break;
        };

		if (!filter_out)
		{
			navHelper_->ProcessEvent(win_event, window_);
		}


    }

    LRESULT CALLBACK MiniEngine::ProcessLowLevelEvent(OSWinHandle hWndParent, unsigned int message, WPARAM wParam, LPARAM lParam )
    {
        // We don't forward the WM_CLOSE message to prevent the internal SDK from closing us (click on X)
        if (message == WM_CLOSE)
        {
            printf("ProcessLowLevelEvent: WM_Close detected and prevented!\n");
            return 0;
        }
        return 1;
    }

	bool MiniEngine::DoesStringContain( std::string &searchingfor,  std::string &stringofinterest )
	{
		std::transform(searchingfor.begin(), searchingfor.end(), searchingfor.begin(), toupper );
		std::transform(stringofinterest.begin(), stringofinterest.end(), stringofinterest.begin(), toupper );

		if(stringofinterest.find( searchingfor.c_str() )!=std::string::npos)
		{
			return true;
		}

		return false;
	}

