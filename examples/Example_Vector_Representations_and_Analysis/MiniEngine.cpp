#include "MiniEngine.h"

#include "RepresentationHelpers.h"
#include "BuildingClassifier.h"
#include "CameraFrustumContentsTest.h"
#include "IntersectionTestHandler.h"
#include "ViewshedHandler.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/IGW3DClampRadius.h"
#include "engine/IGW3DMediaCenter.h"
#include "engine/IGW3DImage.h"

#include "engine/IGW3DPanoramicViewshedTest.h"

#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/LayerParameters.h"

#include <deque>
#include <map>
#include <iostream>

	MiniEngine::MiniEngine( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context ) 
		: sdk_context_( sdk_context )
		, intersection_test_handler_( new IntersectionTestHandler( sdk_context ) )
		, navHelper_ (new NavigationHelper())
		, viewshed_handler_( new ViewshedHandler( sdk_context ) )
	{}

	MiniEngine::~MiniEngine()
	{
		delete navHelper_;
		delete intersection_test_handler_;
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
        //you would probably want to keep your vectordatasources in your own
        //container so you don't have to always loop through the dozens of drivers
        //to find the one you are looking for
        std::deque<std::string> vectordata;

        vectordata.push_back( rootpath );
        vectordata.back().append( "/Building Footprints.shp" );
        vectordata.push_back( rootpath );
        vectordata.back().append( "/Utility Poles.shp" );

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


        //imagery has yet to be brought over to the C++ api.
        std::string imagerypath( rootpath );
        imagerypath += "/DC_1ft_demo.ecw";

        Geoweb3d::GW3DEnvelope env;
        if ( CreateImageryRep(sdk_context_, imagerypath.c_str(), 6, 0, true, env ).expired())
		{
			std::cout<< "CreateImageryRep Error" << std::endl;
		}
		else
		{
            double center_lon = (env.MaxX + env.MinX) / 2.0;
            double center_lat = (env.MaxY + env.MinY) / 2.0;
            camera_.lock()->get_CameraController()->put_Location(center_lon, center_lat);
            camera_.lock()->get_CameraController()->put_Elevation(500.0, Geoweb3d::IGW3DPositionOrientation::Relative);
		}

        //lets try to get a starting point for the camera by looking at all the vector data
        //and zoom to the center area of its bounds. This is more of an SDK test too, as
        //we go through all the vector drivers and gather all the layers etc. to find
        //the overall 2D bounding box.

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

                        //**** REPRESENTATION ASSUMPTIONS ON LAYER NAMES ****
                        // Obviously this example is tryint go be generic and this is not the right way
                        // to do this in the real world.

                        //if layer name has footprints anywhere, we will assume its extruded polygons, as well
                        //as the sample dataset geoweb3d provided (to attribute map)

                        std::string layername (layer.lock()->get_Name());

                        if( DoesStringContain( std::string("Footprints"), layername) )
                        {
                            printf("Geoweb3d Data Detected the footprints data(most likely) and will both represent it and attribute map it\n");
                            extruded_representation_ = RepresentAsExtruded(sdk_context_, layer);
                        }
                        else if(DoesStringContain( std::string("Poles"), layername) )
                        {
                            printf("Geoweb3d Data Detected the Misc points data(most likely) and will both represent it and attribute map it\n");
                            light_layer =  layer;
                            lightpoint_representation_ = RepresentAsLights( sdk_context_, layer );
                            
                            //Show the Frustum Intersection, mainly used for drawing things like ICONs.
                            //Note, a custom rep does not place anything in the 3D scene.  Any "Shape Type"
                            //or "Model" Representation will work with a frustum query.  The SDK has yet to
                            //support things like lines, water, and extruded polygon for instance.
                            //can represent a model and do a frustum contents query
                            custom_representation_ = RepresentAsCustom( sdk_context_, layer );
                            custom_representation_.lock()->get_FrameAnalysisContentsConfiguration()->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->put_Enabled( true );
							custom_representation_.lock()->get_FrameAnalysisContentsConfiguration()->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->get_ContentsFilter()->put_Enabled( true );//we want to use paramters different than what we use for rendering
                            //we want to knwo where on the window the point is
                            custom_representation_.lock()->get_FrameAnalysisContentsConfiguration()->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->get_FeatureID2DPixelLocationConfiguration()->put_Enabled( true );

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
        sdk_context_->run_EventStream( *this );
    }

    void MiniEngine::DoPostDrawWork()
    {

		//frame_tasks_.clear(); //all tasks for this frame completed

		intersection_test_handler_->DoPostDrawWork();

        //all pre and post draw tasks ran, now lets do any updates
        //to the representations within the 3d scene.

        if(!extruded_representation_.expired())
        {


            if(! (Geoweb3d::SDK_Statistics::GetMasterFrameCount() % 60) ) //toggle every few seconds/frames
            {
                static bool toggle_ = true;

                if(!toggle_)
                {
                    //put all back to the default settings
                    extruded_representation_.lock()->reset_Properties();


                }
                else
                {
                    BuildingClassifier classifierStream( extruded_representation_ );
                    //now get the layer of this representation belongs to and stream
                    //the changes
                    extruded_representation_.lock()->get_VectorLayer().lock()->Stream( &classifierStream );

                    static double height = 25;
					if (height <= 100)
					{
						UpdateDefaultPolygonExtrusion(extruded_representation_,height+=5 );
					}


                }

                toggle_ = !toggle_;
            }
		}

        if(! (Geoweb3d::SDK_Statistics::GetMasterFrameCount() % 120) ) //toggle every few seconds/frames
        {
            if(!lightpoint_representation_.expired())
            {

                static bool toggle_ = true;

                if(!toggle_)
                {
                    //put all back to the default settings of the represetnation driver
                    lightpoint_representation_.lock()->put_PropertyCollection( 0 );
                }
                else
                {
                    //don't use the defaults of the driver, make the defaults
                    //to use this layers unique defaults.
                    SetGlobalLightColor();
                }

                toggle_ = !toggle_;

            }
        }
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
        CameraFrustumContentsTest frustum_contents;
        sdk_context_->get_VectorRepresentationDriverCollection()->Stream( &frustum_contents );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Linear interpolation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="startvalue">	The startvalue. </param>
    /// <param name="endvalue">  	The endvalue. </param>
    /// <param name="value">	 	The value. </param>
    ///
    /// <returns>	. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    double MiniEngine::linearInterpolation( double startvalue, double endvalue, double value )
    {
        if ( value < startvalue ) return startvalue;
        if ( value > endvalue )  return endvalue;

        return ( endvalue - value ) * startvalue + value * endvalue;
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
						case Geoweb3d::Key::S:
						{
							Geoweb3d::IGW3DCameraStereoSettings *settings = camera_.lock()->get_CameraStereoSettings();
							//just toggle stereo on/off
							settings->put_Enable( !settings->get_Enable() );
						}
						break;
						case Geoweb3d::Key::V:
						{
							viewshed_handler_->ToggleViewshed(camera_);
						}
						break;
						case Geoweb3d::Key::E:
						{      
							viewshed_handler_->RunViewshedReport();
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
						default:
						break;
					};
				}
			}
			break;
			default:
			break;
        };

		// Enable to move viewshed with the camera, otherwise it will stay where you placed it
		bool move_viewshed_with_eye = false;
		
        if(move_viewshed_with_eye && viewshed_handler_ && !viewshed_handler_->viewshed_test_.expired() )
        {
            //make sure our camera is added to the analysis
            //note, we should try to be smarter and only
            //add when needed, as well as only query the camera
            //position once per frame
            viewshed_handler_->viewshed_test_.lock()->put_AddCamera( camera_ );
            const Geoweb3d::GW3DPoint *cloc = camera_controller_->get_Location( );

            float yaw,pitch,roll;
            camera_controller_->get_Rotation( yaw,pitch,roll);

            //put the viewshed just below the camera, pointing down a bit.
            double lon = cloc->get_X();
            double lat = cloc->get_Y();

            viewshed_handler_->viewshed_test_.lock()->put_Location( lon,lat );
            viewshed_handler_->viewshed_test_.lock()->put_Elevation( cloc->get_Z() - 1.0 );
            viewshed_handler_->viewshed_test_.lock()->put_Rotation( yaw, 45.0f ,roll);

        }

    }

    LRESULT CALLBACK MiniEngine::ProcessLowLevelEvent( OSWinHandle hWndParent, unsigned int message, WPARAM wParam, LPARAM lParam )
    {
        // We don't forward the WM_CLOSE message to prevent the internal SDK from closing us (click on X)
        if (message == WM_CLOSE)
        {
            printf("ProcessLowLevelEvent: WM_Close detected and prevented!\n");
            return 0;
        }
        return 1;
    }

    void MiniEngine::OnPagingEvent( Geoweb3d::IGW3DVectorRepresentationWPtr representation, PagingEventType etype, unsigned unique_id, const Geoweb3d::GW3DEnvelope& bounds, unsigned int value )
    {
        if(etype ==  IGW3DEventStream::QuickCacheCreateFinished)
        {
            printf("Cache for this Envelope/unique_id finished : %s\n", representation.lock()->get_Driver().lock()->get_Name());
        }
        else if(etype ==  Geoweb3d::IGW3DEventStream::DataSourceFinishedLoading)
        {
            printf("Loading for this Envelope/unique_id finished : %s\n", representation.lock()->get_Driver().lock()->get_Name());
        }
        else if(etype ==  Geoweb3d::IGW3DEventStream::DataSourceWorkEstimationComplete || etype ==  Geoweb3d::IGW3DEventStream::DataSourceWorkEstimationComplete  )
        {
            printf("Percent for this area [%s] %d \n", representation.lock()->get_Driver().lock()->get_Name(), value );
        }
    }

    void MiniEngine::OnStereoEvent( Geoweb3d::IGW3DStereoDriverWPtr stereo_driver, StereoEventType etype )
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

	void MiniEngine::SetGlobalLightColor()
	{
		if(!lightpoint_representation_.expired())
		{
			//variants are expensive to create and destroy, so resuse the object when you can.
			Geoweb3d::IGW3DVariant scratchpad;
			Geoweb3d::IGW3DPropertyCollectionPtr properties = lightpoint_representation_.lock()->get_PropertyCollection()->create_Clone();
			scratchpad = 1.0;
			properties->put_Property(  Geoweb3d::Vector::LightPointProperties::RED, scratchpad );
			scratchpad = 0.0;
			properties->put_Property(  Geoweb3d::Vector::LightPointProperties::GREEN, scratchpad );
			scratchpad = 1.0;
			properties->put_Property( Geoweb3d::Vector::LightPointProperties::BLUE, scratchpad );
			//the put here is the "commit" transaction in that this will apply the settings to the 3D representation.
			lightpoint_representation_.lock()->put_PropertyCollection( properties );
		}
	}