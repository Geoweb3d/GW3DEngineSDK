#include "stdafx.h"
#include "MiniEngine.h"

#include "FannedViewshedHandler.h"
#include "PerspectiveViewshedHandler.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/IGW3DClampRadius.h"
#include "engine/IGW3DMediaCenter.h"
#include "engine/IGW3DImage.h"
#include "engine/GW3DRaster.h"

#include "engine/IGW3DPerspectiveViewshedTest.h"

#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/LayerParameters.h"

#include <deque>
#include <map>
#include <iostream>

// Note: The Perspective Viewshed has not been wired to the hotkeys for modifications yet. 
const bool use_panoramic_viewshed = true;
const bool insert_projected_image_too = false;

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
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::RED, 0.0);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::GREEN, 0.0);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::IndividualProperties::BLUE, 1.0);

        //good to go!
        Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
		params.representation_default_parameters = defaults;
        return driver.lock()->get_RepresentationLayerCollection()->create( layer, params );
    }

    return Geoweb3d::IGW3DVectorRepresentationWPtr();
}

	MiniEngine::MiniEngine( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context ) 
		: sdk_context_( sdk_context )
		//, intersection_test_handler_( new IntersectionTestHandler( sdk_context ) )
		, navHelper_ (new NavigationHelper())
		, fanned_viewshed_handler_( new FannedViewshedHandler( sdk_context ) )
		, perspective_viewshed_handler_( new PerspectiveViewshedHandler( sdk_context ) )
		, elevation_driver(sdk_context->get_RasterLayerRepresentationDriverCollection()->get_Driver( "Elevation" ))
	{}

	MiniEngine::~MiniEngine()
	{
		delete navHelper_;
		//delete intersection_test_handler_;
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

        for( unsigned i = 0; i < vectordata.size(); ++i )
        {
			Geoweb3d::GW3DResult res;
			//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
			//having to guess which data source driver is needed to open up a particular dataset.
            Geoweb3d::IGW3DVectorDataSourceWPtr data_source = sdk_context_->get_VectorDriverCollection()->auto_Open( vectordata[i].c_str(), res);
			
			//testing a failure case
            if( data_source.expired() )
				std::cout << "Could not load the following Vector Data Source: " << vectordata[i].c_str();
        }

	//	w_env.merge(t_data_source.lock()->get_VectorLayerCollection()->get_AtIndex(ii).lock()->get_Envelope());			


        // Now load the elevation data
		std::string elevationpath( rootpath );
        elevationpath += "/NED 10-meter DC.tif";

		Geoweb3d::IGW3DRasterDriverCollection *raster_drivers = sdk_context_->get_RasterDriverCollection();

		Geoweb3d::GW3DResult res;
		//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
		//having to guess which data source driver is needed to open up a particular dataset.
		Geoweb3d::IGW3DRasterDataSourceWPtr rdatasource = raster_drivers->auto_Open(elevationpath.c_str(), res);

		if(!rdatasource.expired())
		{
			Geoweb3d::IGW3DRasterLayerCollection* layer_collection = rdatasource.lock()->get_RasterLayerCollection();
			Geoweb3d::IGW3DRasterLayerWPtr layer = layer_collection->get_AtIndex(0);

			if (!layer.expired())
			{
				Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
				params.page_level = 0;
				params.priority = 0;
				params.representation_layer_activity = true;
				Geoweb3d::IGW3DRasterRepresentationWPtr imagery_rep = elevation_driver.lock()->get_RepresentationLayerCollection()->create(layer, params);

				const Geoweb3d::IGW3DPropertyCollection* prop_col = imagery_rep.lock()->get_PropertyCollection();
				if (prop_col)
				{

					const Geoweb3d::IGW3DDefinitionCollection* def_col = prop_col->get_DefinitionCollection();
					if (def_col)
					{
						static unsigned scale_index = imagery_rep.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("VERTICAL_SCALE");

						Geoweb3d::IGW3DPropertyCollectionPtr defaultsettings = elevation_driver.lock()->get_PropertyCollection()->create_Clone();

						defaultsettings->put_Property(scale_index, 2.0);
						imagery_rep.lock()->put_PropertyCollection(defaultsettings);
					}
				}
			}
		}

        //lets try to get a starting point for the camera by looking at all the vector data
        //and zoom to the center area of its bounds. This is more of an SDK test too, as
        //we go through all the vector drivers and gather all the layers etc. to find
        //the overall 2D bounding box.
        Geoweb3d::GW3DEnvelope env;

        Geoweb3d::IGW3DVectorDriverWPtr vdriver;
        sdk_context_->get_VectorDriverCollection()->reset();

        while( sdk_context_->get_VectorDriverCollection()->next( &vdriver ) )
        {
            // Note, going through a lock() all the time creates alot of extra work
            // Avoid this when we are in a tight loop, like the while loop below
            Geoweb3d::IGW3DVectorDataSourceCollection* vdscollection = vdriver.lock()->get_VectorDataSourceCollection( true );

            Geoweb3d::IGW3DVectorDataSourceWPtr vdatasource;

            while( vdscollection->next( &vdatasource ) )
            {
                Geoweb3d::IGW3DVectorLayerCollection* vlyrcollection = vdatasource.lock()->get_VectorLayerCollection( true );

				Geoweb3d::IGW3DVectorLayerWPtr layer;

				std::cout<< "\nVector File's Layers:"<<std::endl;

                while( vlyrcollection->next( &layer ) )
                {
					        
					std::cout<< "\tName: "<<layer.lock()->get_Name()<<", LayerType: " << layer.lock()->get_Type() << std::endl;
          
                    Geoweb3d::GW3DResult retval = layer.lock()->get_IsValidForVectorRepresentation();

                    if( Geoweb3d::Succeeded( retval ) )
                    {
						switch( retval )
						{
							case Geoweb3d::GW3D_sAlreadyInserted:
								//this is a warning, and it just means you have the same pointer
            					//so something already inserted.
								std::cout<< "Warning: [ "<<layer.lock()->get_Name()<< "] You have the same pointer, so something already inserted." << std::endl;
								break;
							case Geoweb3d::GW3D_sProjectionGuessed:
								printf("Warning: [%s] Has valid WGS84 bounds, but it must not have a spatial reference.\n", layer.lock()->get_Name() );
								break;
							default:
								break;
						}

                        env.merge( layer.lock()->get_Envelope() );


						double center_lon = (env.MaxX + env.MinX) / 2.0;
						double center_lat = (env.MaxY + env.MinY) / 2.0;
						camera_.lock()->get_CameraController()->put_Location(center_lon, center_lat);
						camera_.lock()->get_CameraController()->put_Elevation( 3000.0, Geoweb3d::IGW3DPositionOrientation::Relative );
						camera_.lock()->get_CameraController()->put_Rotation(0.f, 89.f, 0.f);


						Geoweb3d::IGW3DLatitudeLongitudeGrid* grid = camera_.lock()->get_LatitudeLongitudeGrid();
						grid->put_Enabled(false);
                        //**** REPRESENTATION ASSUMPTIONS ON LAYER NAMES ****
                        // Obviously this example is trying to be generic and this is not the right way
                        // to do this in the real world.

                        //if layer name has footprints anywhere, we will assume its extruded polygons, as well
                        //as the sample dataset geoweb3d provided (to attribute map)

                        std::string layername (layer.lock()->get_Name());

                        if( DoesStringContain( std::string("Footprints"), layername) )
                        {
                            printf("Geoweb3d Data Detected the footprints data(most likely) and will both represent it and attribute map it\n");
                            extruded_representation_ = RepresentAsExtruded(sdk_context_, layer);

                        }
            
                    }
                    else
					{
						switch(retval)
						{
							case Geoweb3d::GW3D_eLayerProjectionError:
            					//this is an error means you will have the opportunity to set projection information,
            					//and call get_isValidToRepresentIn3D again....until you get it right
            					printf("\n ** Vector data was loaded, but you will not be able to represent it into the 3d scene due to unknown projection! **\n\n");
            					break;
							default:
            					//something wrong with this layer and we cannot represent it!
            					printf("\n ** Vector data was loaded, but you will not be able to represent it into the 3d scene due to its incompatibility! **\n\n");
            					break;
						}
					}
                }
            }
        }

		camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);

		navHelper_->put_HomePosition(camera_);

		if ( use_panoramic_viewshed )
		{
			fanned_viewshed_handler_->ToggleViewshed( camera_, insert_projected_image_too );
		}
		else
		{
			perspective_viewshed_handler_->ToggleViewshed( camera_, insert_projected_image_too );
		}
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
		navHelper_->ProcessEvent(win_event, window_);

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
						//Test Span 
						case Geoweb3d::Key::Num0:
						{
							double hangle_span = fanned_viewshed_handler_->GetViewshed_HorizontalAngle();
							hangle_span+=10;
							if(hangle_span > 360)
								hangle_span = 360;
							fanned_viewshed_handler_->SetViewshed_HorizontalAngle(hangle_span);
						}
						break;
						case Geoweb3d::Key::Num9:
						{
							double hangle_span = fanned_viewshed_handler_->GetViewshed_HorizontalAngle();
							hangle_span-=10;
							if(hangle_span < 1.0)
								hangle_span = 1.0;
							fanned_viewshed_handler_->SetViewshed_HorizontalAngle(hangle_span);
						}
						break;
						case Geoweb3d::Key::Num8:
						{
							double vangle_span = fanned_viewshed_handler_->GetViewshed_VerticalAngle();
							vangle_span+=10;
							if(vangle_span > 360)
								vangle_span = 360;
							fanned_viewshed_handler_->SetViewshed_VerticalAngle(vangle_span);
						}
						break;
						case Geoweb3d::Key::Num7:
						{
							double vangle_span = fanned_viewshed_handler_->GetViewshed_VerticalAngle();
							vangle_span-=10;
							if(vangle_span < 1.0)
								vangle_span = 1.0;
							fanned_viewshed_handler_->SetViewshed_VerticalAngle(vangle_span);
						}
						break;
						case Geoweb3d::Key::Num6:
						{
							double inner_rad = fanned_viewshed_handler_->GetViewshed_InnerRadius();
							inner_rad+=10;
							fanned_viewshed_handler_->SetViewshed_InnerRadius(inner_rad);
						}
						break;
						case Geoweb3d::Key::Num5:
						{
							double inner_rad = fanned_viewshed_handler_->GetViewshed_InnerRadius();
							inner_rad-=10;
							fanned_viewshed_handler_->SetViewshed_InnerRadius(inner_rad);
						}
						break;

						case Geoweb3d::Key::Num4:
						{
							double outer_rad = fanned_viewshed_handler_->GetViewshed_OuterRadius();
							outer_rad+=10;
							fanned_viewshed_handler_->SetViewshed_OuterRadius(outer_rad);
						}
						break;
						case Geoweb3d::Key::Num3:
						{
							double outer_rad = fanned_viewshed_handler_->GetViewshed_OuterRadius();
							outer_rad-=10;
							fanned_viewshed_handler_->SetViewshed_OuterRadius(outer_rad);
						}
						break;
						case Geoweb3d::Key::Dash:
						{
							float pitch = fanned_viewshed_handler_->GetViewshed_Pitch();
							pitch-=10;
							fanned_viewshed_handler_->SetViewshed_Pitch(pitch);
						}
						break;
						case Geoweb3d::Key::Equal:
						{
							float pitch = fanned_viewshed_handler_->GetViewshed_Pitch();
							pitch+=10;
							fanned_viewshed_handler_->SetViewshed_Pitch(pitch);
						}
						break;
						case Geoweb3d::Key::LBracket:
						{
							double elevation = fanned_viewshed_handler_->GetViewshed_Elevation();
							elevation-=10;
							fanned_viewshed_handler_->SetViewshed_Elevation(elevation);
						}
						break;
						case Geoweb3d::Key::RBracket:
						{
							double elevation = fanned_viewshed_handler_->GetViewshed_Elevation();
							elevation+=10;
							fanned_viewshed_handler_->SetViewshed_Elevation(elevation);
						}
						break;

						//Test terrain detail policies
						case Geoweb3d::Key::Num1:
						{
							static int pres_policy = 0;
							pres_policy++ ;
							if( pres_policy > Geoweb3d::IGW3DViewshedTest::LODPolicy::FullIn_ViewOnly_QualityClamp)
							{
								pres_policy = 0;
							}
							switch(pres_policy)
							{
								case 0:
								{
									printf("LOD policy is None\n");
									break;
								}
								case 1:
								{
									printf("LOD policy is Normal\n");
									break;
								}
								case 2:
								{
									printf("LOD policy is FullIn\n");
									break;
								}
								case 3:
								{
									printf("LOD policy is Normal_ViewOnly\n");
									break;
								}
								case 4:
								{
									printf("LOD policy is FullIn_ViewOnly\n");
									break;
								}
								case 5:
								{
									printf("LOD policy is FullIn_QualityClamp\n");
									break;
								}
								case 6:
								{
									printf("LOD policy is FullIn_ViewOnly_QualityClamp\n");
									break;
								}
							}
							fanned_viewshed_handler_->SetViewshed_LODPolicy((Geoweb3d::IGW3DViewshedTest::LODPolicy) pres_policy);
						}
						break;
						
						case Geoweb3d::Key::V:
						{
							if ( use_panoramic_viewshed )
							{
								fanned_viewshed_handler_->ToggleViewshed( camera_, insert_projected_image_too );
							}
							else
							{
								perspective_viewshed_handler_->ToggleViewshed( camera_, insert_projected_image_too );
							}
						}
						break;

						case Geoweb3d::Key::P:
						{
							const int width = 4000; //try a 4kx4k snapshot 
							const int height= 4000;

							Geoweb3d::IGW3DMediaCenter *medcenter = sdk_context_->get_MediaCenter();
							//save as a lossless format as we are interested in a poster.
							Geoweb3d::IGW3DMediaCenterTaskPtr ctask = medcenter->create_Image(window_,width,height,"C:/temp/test_deferred.bmp");
							//note, when the task is completed, I want to have to go to disk,  Note that
							//this may get written now or 10 minutes from now.. if you really want to be
							//sure the task is completed and the file is written, you need to call wait_ForCompletion.

							//note, we can force the event and or/wait
							//if(ctask->get_Status() == Geoweb3d::GW3D_sPending)
							{
								medcenter->wait_ForCompletion(ctask, -1 );

								ctask->get_Image().lock()->put_FlipVertical();
								ctask->get_Image().lock()->put_ToDisk("C:/temp/vertical_flip.png" );

								//be careful @as this might take a while to scale
								ctask->get_Image().lock()->put_ScaleImage(6000,6000);
								ctask->get_Image().lock()->put_ToDisk("C:/temp/scale_up.png" );

								ctask->get_Image().lock()->put_ScaleImage(32,32);
								ctask->get_Image().lock()->put_ToDisk("C:/temp/scale_down.png" );
							}
						}
						break;

						case Geoweb3d::Key::Tilde:
						{
							static bool top_down_toggle = false;
							if(!top_down_toggle)
							{
								fanned_viewshed_handler_->SetViewshed_Elevation( 1100);
								fanned_viewshed_handler_->SetViewshed_Pitch( 90);
								top_down_toggle = true;
							}
							else
							{
								fanned_viewshed_handler_->SetViewshed_Elevation( 0);
								fanned_viewshed_handler_->SetViewshed_Pitch(0);
								top_down_toggle = false;
							}
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

		// Enable to move fanned_viewshed with the camera, otherwise it will stay where you placed it
		bool move_fanned_viewshed_with_eye = false;
		
        if(move_fanned_viewshed_with_eye && fanned_viewshed_handler_ && !fanned_viewshed_handler_->viewshed_test_.expired() )
        {
            //make sure our camera is added to the analysis
            //note, we should try to be smarter and only
            //add when needed, as well as only query the camera
            //position once per frame
            fanned_viewshed_handler_->viewshed_test_.lock()->put_AddCamera( camera_ );
            const Geoweb3d::GW3DPoint *cloc = camera_controller_->get_Location( );

            float yaw,pitch,roll;
            camera_controller_->get_Rotation( yaw,pitch,roll);

            //put the fanned_viewshed just below the camera, pointing down a bit.
            double lon = cloc->get_X();
            double lat = cloc->get_Y();

            fanned_viewshed_handler_->viewshed_test_.lock()->put_Location( lon,lat );
            fanned_viewshed_handler_->viewshed_test_.lock()->put_Elevation( cloc->get_Z() - 1.0 );
            fanned_viewshed_handler_->viewshed_test_.lock()->put_Rotation( yaw, 45.0f ,roll);

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

