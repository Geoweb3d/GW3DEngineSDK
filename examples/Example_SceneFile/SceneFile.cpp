
#include "../Common/NavigationHelper.h"

#include <Geoweb3d/core/GW3DInterface.h>
#include <Geoweb3d/core/GW3DGPUConfig.h>
#include <Geoweb3d/common/IGW3DString.h>

#include <Geoweb3d/engine/IGW3DGeoweb3dSDK.h>
#include <Geoweb3d/engine/IGW3DEventStream.h>
#include <Geoweb3d/engine/GW3DCamera.h>
#include <Geoweb3d/engine/GW3DWindow.h>

#include <Geoweb3d/engine/IGW3DVectorRepresentation.h>
#include <Geoweb3d/engine/IGW3DVectorRepresentationDriver.h>
#include <Geoweb3d/engine/IGW3DEnvironmentEffects.h>
#include <Geoweb3d/engine/IGW3DEnvironmentLabs.h>

#include <Geoweb3d/attributemapper/IGW3DSceneLoadingCallbacks.h>
#include <Geoweb3d/attributemapper/IGW3DSceneCollection.h>
#include <Geoweb3d/attributemapper/IGW3DScene.h>

#include <GeoWeb3dCore/SystemExports.h>
 
#include <deque>

void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();

class MiniEngine;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Pre post draw tasks. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct PrePostDrawTasks
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPreDraw() = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the post draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPostDraw() =0;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Mini engine. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class MiniEngine 
	: public Geoweb3d::IGW3DEventStream
	, public Geoweb3d::IGW3DWindowCallback
	, public Geoweb3d::IGW3DSceneLoadingCallbacks
{
public:

	Geoweb3d::IGW3DCameraWPtr current_camera_;
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Constructor. </summary>
    ///
    /// <param name="sdk_context">	Context for the sdk. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, bool load_projects_at_startup ) 
		: sdk_context_( sdk_context ) 
		, nav_helper_ ( new NavigationHelper() )
		, load_projects_at_startup_( load_projects_at_startup )
	{}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~MiniEngine()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Updates this object. </summary>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Update()
    {
		if( sdk_context_->draw( window_ ) == Geoweb3d::GW3D_sOk )
        {
            return true;
        }

        //return that we did not draw. (app closing?)
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Loads a configuration. </summary>
    ///
    /// <param name="example_name">	Name of the example. </param>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool LoadConfiguration(const char *example_name )
    {
		window_ = create_Window( sdk_context_, example_name, this );
		if(window_.expired())return false;

		camera_ = create_Camera(window_,"1 Camera");
		camera_.lock()->get_CameraController()->put_Location( -74.0197, 40.6975 );
		camera_.lock()->get_CameraController()->put_Elevation( 554.754, Geoweb3d::IGW3DPositionOrientation::Relative );
		camera_.lock()->get_CameraController()->put_Rotation( 32.1832f, 26.9841f, 0.0f );

		camera_.lock()->get_LatitudeLongitudeGrid()->put_Enabled( false );
		camera_.lock()->get_LatitudeLongitudeGrid()->put_GeofenceGridEnabled( false );

		auto camera_controller_ = camera_.lock()->get_CameraController();
		camera_controller_->put_NavigationRestrictionMode( Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY );
		camera_controller_->put_PitchClamp( Geoweb3d::IGW3DPositionOrientation::PitchClampMode::PITCH_90 );

		//Add cameras to the navigation helper
		nav_helper_->add_Camera(camera_);
		return true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Loads the data. </summary>
    ///
    /// <param name="rootpath">	The rootpath. </param>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

	void LoadProject_( const std::string& path )
	{
		Geoweb3d::IGW3DSceneCollection* collection = sdk_context_->get_SceneCollection();
		if ( !collection )
			return;

		auto iter = std::find_if( scenes_.begin(), scenes_.end(), [ &path ]( Geoweb3d::IGW3DSceneWPtr& wscene )
		{
			Geoweb3d::GW3DSharedPtr<Geoweb3d::IGW3DScene> scene = wscene.lock();
			if ( scene )
			{
				return strcmp( path.c_str(), scene->get_Filename() ) == 0;
			}

			return false;
		} );

		if ( iter == scenes_.end() )
		{
			printf( "\nLoading Project: %s \n", path.c_str() );
			scenes_.push_back( collection->Load( path.c_str(), this ) );
		}
	}

    bool LoadProjects( const char *rootpath )
    {
		Geoweb3d::IGW3DSceneCollection* collection = sdk_context_->get_SceneCollection();
		if ( !collection || !scenes_.empty() )
			return false;

		sample_root_path_ = rootpath;
		active_scene_filename_ = sample_root_path_ + "/DC.scene";

		if ( load_projects_at_startup_ )
		{
			LoadProject_( active_scene_filename_ );

			auto locked_camera = camera_.lock();
			auto iter = scenes_.begin();

			if ( locked_camera && iter != scenes_.end() )
			{
				auto locked_scene = iter->lock();
				if ( locked_scene )
				{
					auto locked_scene_camera = locked_scene->get_Camera().lock();
					if ( locked_scene_camera )
					{
						double elevation = 0.0;
						Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode = Geoweb3d::IGW3DPositionOrientation::Relative;
						locked_scene_camera->get_Elevation( elevation, mode );

						locked_camera->get_CameraController()->put_Location( locked_scene_camera->get_Location()->get_X(), locked_scene_camera->get_Location()->get_Y() );
						locked_camera->get_CameraController()->put_Elevation( elevation, mode );
						locked_camera->get_CameraController()->put_Rotation( locked_scene_camera->get_Heading(), locked_scene_camera->get_Pitch(), locked_scene_camera->get_Roll() );

						// reset the home position
						nav_helper_->put_HomePosition( camera_ );
					}
					else
					{
						Geoweb3d::GW3DEnvelope env = locked_scene->get_Envelope();
						if ( !env.get_IsNull() )
						{
							double lon = env.MaxX - (( env.MaxX - env.MinX ) * 0.5);
							double lat = env.MaxY - (( env.MaxY - env.MinY ) * 0.5);

							locked_camera->get_CameraController()->put_Location( lon, lat );
							locked_camera->get_CameraController()->put_Elevation( 3048.0, Geoweb3d::IGW3DPositionOrientation::Relative );
							locked_camera->get_CameraController()->put_Rotation( 0.0f, 90.0f, 0.0f );
						}
					}
				}
			}

			return true;
		}

		return false;
    }

	void RemoveProject_( const std::string& path )
	{
		Geoweb3d::IGW3DSceneCollection* collection = sdk_context_->get_SceneCollection();
		if ( !collection )
			return;

		auto iter = std::find_if( scenes_.begin(), scenes_.end(), [ &path ]( Geoweb3d::IGW3DSceneWPtr& wscene )
		{
			Geoweb3d::GW3DSharedPtr<Geoweb3d::IGW3DScene> scene = wscene.lock();
			if ( scene )
			{
				return strcmp( path.c_str(), scene->get_Filename() ) == 0;
			}

			return false;
		} );

		if ( iter != scenes_.end() )
		{
			printf( "\nUnLoading Project: %s \n", path.c_str() );
			collection->Destroy( *iter );
			scenes_.erase( iter );
		}
	}

	void RemoveProjects()
	{
		Geoweb3d::IGW3DSceneCollection* collection = sdk_context_->get_SceneCollection();
		if ( !collection )
			return;

		for ( Geoweb3d::IGW3DSceneWPtr scene : scenes_ )
		{
			if ( !scene.expired() )
			{
				printf( "\nUnLoading Project: %s \n", scene.lock()->get_Filename() );
				collection->Destroy( scene );
			}
		}

		scenes_.clear();
	}


private:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	call back for 2d drawing/client side. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="camera">	The camera. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera ){}
	virtual void OnCreate( ) {}
	virtual void OnDrawBegin( ) {}
	virtual void OnDrawEnd( ) {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	OS event system, basically a helper off ProcessLowLevelEvent. </summary>
    ///
    /// <param name="win_event">	The window event. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void ProcessEvent( const Geoweb3d::WindowEvent& win_event )
    {
		switch ( win_event.Type )
		{
		case Geoweb3d::WindowEvent::KeyPressed:
		{
			const bool nomods = !win_event.Key.shift && !win_event.Key.alt && !win_event.Key.control;
			const bool alt = 	win_event.Key.alt;

			printf( "code: %i, nomods: %s, alt flag: %s, control: %s, shift: %s, alt: %s \n", (int)win_event.Key.code, (nomods ? "true" : "false"), (alt ? "true" : "false"), (win_event.Key.control ? "true" : "false"), (win_event.Key.shift ? "true" : "false"), (win_event.Key.alt ? "true" : "false") );

			switch ( win_event.Key.code )
			{
			case Geoweb3d::Key::A:
			{
				SetFogActivity( !GetFogActivity() );
				break;
			}

			case Geoweb3d::Key::Numpad7 :
			{
				if ( nomods )	DecreaseHazeVisibility();
				else if ( alt )	IncreaseHazeTransition();
			}
			break;

			case Geoweb3d::Key::Numpad9:
			{
				if ( nomods )	IncreaseHazeVisibility();
				else if ( alt )	DecreaseHazeTransition();
			}
			break;

			case Geoweb3d::Key::Numpad4:
			{
				if ( nomods )	DecreaseFogVisibility();
				else if ( alt )	IncreaseFogTransition();
			}
			break;

			case Geoweb3d::Key::Numpad6:
			{
				if ( nomods )	IncreaseFogVisibility();
				else if ( alt )	DecreaseFogTransition();
			}
			break;

			case Geoweb3d::Key::Numpad8:
			{
				IncreaseFogHeight();
			}
			break;

			case Geoweb3d::Key::Numpad5:
			{
				DecreaseFogHeight();
			}
			break;

			case Geoweb3d::Key::Num0:
				camera_.lock()->get_CameraController()->put_Location( 0.0, 0.0 );
				camera_.lock()->get_CameraController()->put_Elevation( 500.0, Geoweb3d::IGW3DPositionOrientation::Relative );
				camera_.lock()->get_CameraController()->put_Rotation( 0.0f, 15.0f, 0.0f );
				break;

			case Geoweb3d::Key::Num1:
				camera_.lock()->get_CameraController()->put_Location( -77.034280769760812, 38.889779819860564 );
				camera_.lock()->get_CameraController()->put_Elevation( 500.0, Geoweb3d::IGW3DPositionOrientation::Relative );
				camera_.lock()->get_CameraController()->put_Rotation( 0.0f, 35.0f, 0.0f );
				break;

			case Geoweb3d::Key::Num2:
				camera_.lock()->get_CameraController()->put_Location( -116.277504778, 34.5570926026 );
				camera_.lock()->get_CameraController()->put_Elevation( 516.624, Geoweb3d::IGW3DPositionOrientation::Relative );
				camera_.lock()->get_CameraController()->put_Rotation( 268.288f, 23.375, 0.0f );
				break;

			case Geoweb3d::Key::T:
			{
				if ( !win_event.Key.shift )
				{
					LoadProject_( active_scene_filename_ );
				}
				else
				{
					RemoveProject_( active_scene_filename_ );
				}
			}
			break;

			case Geoweb3d::Key::R:
			{
				RemoveProjects();
			}
			break;
			case Geoweb3d::Key::PageUp:
			{
				hours_ = (hours_ + 1) % 24;

				Geoweb3d::IGW3DDateTime* dt = camera_.lock()->get_DateTime();
				dt->put_Time(hours_, minutes_);
			}
			break;
			case Geoweb3d::Key::PageDown:
			{
				hours_ = std::max<int>( 0, (hours_ - 1) ) % 24;
				Geoweb3d::IGW3DDateTime* dt = camera_.lock()->get_DateTime();
				dt->put_Time(hours_, minutes_);
			}
			break;
			default:
				nav_helper_->ProcessEvent( win_event, window_ );
				break;
			};
		}
		break;

		default:
		{
			nav_helper_->ProcessEvent( win_event, window_ );
			//float heading = camera_.lock()->get_CameraController()->get_Heading();
			//float pitch = camera_.lock()->get_CameraController()->get_Pitch();
			//printf( "Heading: %lf, Pitch: %lf \n", heading, pitch );
		}
		break;
		}
	}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	OS event system, raw data directly from the windows message pump. </summary>
    ///
    /// <param name="hWndParent">	The window parent. </param>
    /// <param name="message">   	The message. </param>
    /// <param name="wParam">	 	The wParam field of the message. </param>
    /// <param name="lParam">	 	The lParam field of the message. </param>
    ///
    /// <returns>	. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual LRESULT CALLBACK ProcessLowLevelEvent(OSWinHandle hWndParent, UINT message, WPARAM wParam, LPARAM lParam )
    {
        // We don't forward the WM_CLOSE message to prevent the internal SDK from closing us (click on X)
        if (message == WM_CLOSE)
        {
            printf("ProcessLowLevelEvent: WM_Close detected and prevented!\n");
            return 0;
        }
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Pipeline messages </summary>
    ///
    /// <remarks> Because we are a globe and can load data everywhere, we are never *done*
    ///			  loading anything as if you move away and come back, its going to load the area again.
	///			  </remarks>
    ///
    /// <param name="representation">	The representation. </param>
    /// <param name="etype">		 	The etype. </param>
    /// <param name="unique_id">	 	Unique identifier. </param>
    /// <param name="bounds">		 	The bounds. </param>
    /// <param name="value">		 	The value. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void OnPagingEvent( Geoweb3d::IGW3DVectorRepresentationWPtr representation, PagingEventType etype, unsigned unique_id, const Geoweb3d::GW3DEnvelope& bounds, unsigned int value ) override
    {
        if(etype ==  Geoweb3d::IGW3DEventStream::QuickCacheCreateFinished)
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

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates window . </summary>
	///
	/// <param name="sdk_context">  	Context for the sdk. </param>
	/// <param name="title">			The title. </param>
	/// <param name="window_events">	(optional) [in,out] If non-null, the window events. </param>
	///
	/// <returns>	The new window creation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Geoweb3d::IGW3DWindowWPtr create_Window( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events = 0 )
	{
		Geoweb3d::IGW3DWindowCollection* wcol = sdk_context->get_WindowCollection(  );

		Geoweb3d::IGW3DWindowWPtr window = wcol->create_3DWindow(title,GW3D_OVERLAPPED,50,50,1280,600,0,Geoweb3d::IGW3DStereoDriverPtr(),window_events);
		window.lock()->put_Quality(/*0 nvidia has a driver but with 0*/ 1 );


		if(window.expired())
		{
			printf("Error Creating window: [%s]!\n", title );

			return Geoweb3d::IGW3DWindowWPtr();
		}
		return window;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates a camera. </summary>
	///
	/// <param name="windowptr">  	The window. </param>
	/// <param name="cameratitle">	The camera title. </param>
	///
	/// <returns>	The new camera. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	 

	Geoweb3d::IGW3DCameraWPtr create_Camera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle )
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


			Geoweb3d::IGW3DDateTime* dt = camera.lock()->get_DateTime();
			dt->put_isFollowCameraLocationForReference( true );
			dt->put_Time(hours_,minutes_);
			dt->put_DateFromUnDelimitedString("2023125");

			//go back to using the date and time of this computer
			//camera.lock()->get_DateTime()->put_isUseComputerDate(true);
			//camera.lock()->get_DateTime()->put_isUseComputerTime(true);
		}

		return camera;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> IGW3DSceneLoadingCallbacks </summary>
	///
	/// <remarks>  </remarks>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void represented_VectorLayer(Geoweb3d::IGW3DVectorLayerWPtr layer, Geoweb3d::IGW3DVectorRepresentationWPtr rep) override
	{
		auto lyr_l = layer.lock();
		auto rep_l = rep.lock();
		if (lyr_l && rep_l)
		{
			printf("Callback : Layer: %s -> Representation: %s\n", lyr_l->get_Name(), rep_l->get_Driver().lock()->get_Name());
		}
	}

	void SetFogActivity (bool active)
	{
		fog_enabled_ = active;

		if (!camera_.expired())
		{
			camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->put_FogActivity(active);
		}

	}

	bool GetFogActivity () const
	{
		return fog_enabled_;
	}

	void IncreaseHazeVisibility()
	{
		if (!camera_.expired())
		{
			Geoweb3d::GW3DFoggingParameters params = camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->get_FogParameters();
			params.haze_visibility_range = (std::max)( params.haze_visibility_range + primary_increment_value_, 0.0f );
			camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->put_FogParameters( params );
			std::cout << "Haze Visibility set to: " << params.haze_visibility_range << std::endl;
		}
	}

	void DecreaseHazeVisibility()
	{
		if (!camera_.expired())
		{
			Geoweb3d::GW3DFoggingParameters params = camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->get_FogParameters();
			params.haze_visibility_range = (std::max)( params.haze_visibility_range - primary_increment_value_, 0.0f );
			camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->put_FogParameters( params );
			std::cout << "Haze Visibility set to: " << params.haze_visibility_range << std::endl;
		}
	}

	void IncreaseFogVisibility()
	{
		if (!camera_.expired())
		{
			Geoweb3d::GW3DFoggingParameters params = camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->get_FogParameters();
			params.fog_visibility_range = (std::max)( params.fog_visibility_range + primary_increment_value_, 0.0f );
			camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->put_FogParameters( params );
			std::cout << "Fog Visibility set to: " << params.fog_visibility_range << std::endl;
		}
	}

	void DecreaseFogVisibility()
	{
		if (!camera_.expired())
		{
			Geoweb3d::GW3DFoggingParameters params = camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->get_FogParameters();
			params.fog_visibility_range = (std::max)( params.fog_visibility_range - primary_increment_value_, 0.0f );
			camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->put_FogParameters( params );
			std::cout << "Fog Visibility set to: " << params.fog_visibility_range << std::endl;
		}
	}

	void IncreaseFogHeight()
	{
		if (!camera_.expired())
		{
			Geoweb3d::GW3DFoggingParameters params = camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->get_FogParameters();
			params.fog_height = (std::max)( params.fog_height + 1.0f, 0.0f );
			camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->put_FogParameters( params );
			std::cout << "Fog Height set to: " << params.fog_height << std::endl;
		}
	}

	void DecreaseFogHeight()
	{
		if (!camera_.expired())
		{
			Geoweb3d::GW3DFoggingParameters params = camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->get_FogParameters();
			params.fog_height = (std::max)( params.fog_height - 1.0f, 0.0f );
			camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->put_FogParameters( params );
			std::cout << "Fog Height set to: " << params.fog_height << std::endl;
		}
	}

	void IncreaseFogTransition()
	{
		if (!camera_.expired())
		{
			Geoweb3d::GW3DFoggingParameters params = camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->get_FogParameters();
			params.fog_transition_range = (std::max)( params.fog_transition_range + secondary_increment_value_, 0.0f );
			camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->put_FogParameters( params );
			std::cout << "Fog Transition set to: " << params.fog_transition_range << std::endl;
		}
	}

	void DecreaseFogTransition()
	{
		if (!camera_.expired())
		{
			Geoweb3d::GW3DFoggingParameters params = camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->get_FogParameters();
			params.fog_transition_range = (std::max)( params.fog_transition_range - secondary_increment_value_, 0.0f );
			camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->put_FogParameters( params );
			std::cout << "Fog Transition set to: " << params.fog_transition_range << std::endl;
		}
	}

	void IncreaseHazeTransition()
	{
		if (!camera_.expired())
		{
			Geoweb3d::GW3DFoggingParameters params = camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->get_FogParameters();
			params.haze_transition_range = (std::max)( params.haze_transition_range + secondary_increment_value_, 0.0f );
			camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->put_FogParameters( params );
			std::cout << "Haze transition range set to: " << params.haze_transition_range << std::endl;
		}
	}

	void DecreaseHazeTransition()
	{
		if (!camera_.expired())
		{
			Geoweb3d::GW3DFoggingParameters params = camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->get_FogParameters();
			params.haze_transition_range = (std::max)( params.haze_transition_range - secondary_increment_value_, 0.0f );
			camera_.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->put_FogParameters( params );
			std::cout << "Haze transition range set to: " << params.haze_transition_range << std::endl;
		}
	}

private:

    /// <summary>	Context for the sdk. </summary>
    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;

	/// <summary>	All Loaded Scene Files. </summary>
	std::deque<Geoweb3d::IGW3DSceneWPtr>		scenes_;

	bool load_projects_at_startup_;
	std::string sample_root_path_;
	std::string active_scene_filename_;

    /// <summary>	The window. </summary>
    Geoweb3d::IGW3DWindowWPtr window_;
	Geoweb3d::IGW3DCameraWPtr camera_;

	NavigationHelper* nav_helper_;

	//Fog & Haze 
	bool fog_enabled_ = true;
	const float primary_increment_value_ = 250.0;
	const float secondary_increment_value_ = 10.0;

	//Fog
	float fog_visibility_range_ = 500.0f;
	float fog_transition_range_ = 100.0f;
	float fog_height_ = -1000000000000.0f;
	
	//Haze
	float haze_visibility_range_ = 2000.0f;
	float haze_transition_range_ = 2000.0f;

	int hours_ = 22;
	int minutes_ = 0;
}; //engine end of class


///////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the application operation. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">	Context for the sdk. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
	const bool load_projects_now = true;
    MiniEngine *pengine = new MiniEngine( sdk_context, load_projects_now );

    if(pengine->LoadConfiguration("Scene File Example (2017)"))
    {
		// download the Geoweb3d DC sample project here: http://www.geoweb3d.com/download/sample-projects/

        ////******* CHANGE THIS TO YOUR SAMPLE DATA LOCATION ******/
        const char * root_path = "../examples/media";

        if( pengine->LoadProjects( root_path ) )
        {

        }
        else
        {
            printf("Could not load projects, you probably need to setup the path to your data\n");
        }

        //the engine loaded all its data ok

        while( pengine->Update() )
        {
            //could do other app stuff here
        }

		pengine->RemoveProjects();
    }

    delete pengine;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Main entry-point for this application. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="argc">	The argc. </param>
/// <param name="argv">	[in,out] If non-null, the argv. </param>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
    SetInformationHandling( );

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if(sdk_context)
    {
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets information handling. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SetInformationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}