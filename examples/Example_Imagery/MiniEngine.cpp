#include "MiniEngine.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"


#include "engine/IGW3DSceneGraphContext.h"
#include "engine/IGW3DRasterLayerImageCollection.h"
#include "engine/IGW3DRasterTimeController.h"
#include "engine/IGW3DPlanetRasterTimePlayer.h"
#include "core/IGW3DRasterLayerImageCollectionFinalizationToken.h"
#include "engine/IGW3DRasterLayerEnvelopeStream.h"

#include "GeoWeb3dCore/SystemExports.h"

#include <deque>
#include <set>
#include <map>
#include <iostream>


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
	raster_drivers = sdk_context_->get_RasterDriverCollection();
	raster_repdrivers = sdk_context_->get_RasterLayerRepresentationDriverCollection();

	Geoweb3d::GW3DEnvelope env;
	Geoweb3d::GW3DResult res;
	//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
	//having to guess which data source driver is needed to open up a particular dataset.
	auto ds = sdk_context_->get_RasterDriverCollection()->auto_Open("../examples/media/DC_1ft_demo.ecw", res);

	if (!ds.expired())
	{
		Geoweb3d::IGW3DRasterLayerCollection* layer_collection = ds.lock()->get_RasterLayerCollection();
		auto imagery_rep_driver = sdk_context_->get_RasterLayerRepresentationDriverCollection()->get_Driver("Imagery");

		if (!imagery_rep_driver.expired() && layer_collection && layer_collection->count() > 0)
		{
			imagery_rep_props_ = imagery_rep_driver.lock()->get_PropertyCollection()->create_Clone();
			imagery_rep_props_->put_Property( Geoweb3d::Raster::ImageryProperties::SCREEN_RES_MODE, true );

			Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
			params.page_level = 6;
			params.representation_layer_activity = true;
			params.representation_default_parameters = imagery_rep_props_;

			auto layer = layer_collection->get_AtIndex(0);

			imgery_rep_ = imagery_rep_driver.lock()->get_RepresentationLayerCollection()->create(layer, params);

			env = layer.lock()->get_Envelope();
		}
	}


	camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);
	double longitude, latitude;
	longitude = (env.MinX + env.MaxX) * 0.5;
	latitude = (env.MinY + env.MaxY) * 0.5;
	camera_controller_->put_Location(longitude, latitude);
	camera_controller_->put_Elevation(300);
	camera_controller_->put_Rotation(80, 20, 0);

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
	switch (win_event.Type)
	{
		case win_event.KeyPressed:
		{
			switch (win_event.Key.code)
			{
				case Geoweb3d::Key::S:
				{
					auto rep_l = imgery_rep_.lock();
					if (rep_l)
					{
						screen_res_mode_ = !screen_res_mode_;

						imagery_rep_props_->put_Property(Geoweb3d::Raster::ImageryProperties::SCREEN_RES_MODE, screen_res_mode_);
						rep_l->put_PropertyCollection(imagery_rep_props_);

						std::string active = screen_res_mode_ ? std::string("ON") : std::string("OFF");

						printf("\nSCREEN_RES_MODE %s", active.c_str() );
					}
				}
				break;
				case Geoweb3d::Key::Z:
				{
					auto rep_l = imgery_rep_.lock();
					if (rep_l)
					{
						sampler_algorithm_index_ = (sampler_algorithm_index_ + 1) % 8;

						imagery_rep_props_->put_Property(Geoweb3d::Raster::ImageryProperties::SAMPLER_ALGO, sampler_algorithm_index_ );
						rep_l->put_PropertyCollection(imagery_rep_props_);

						std::string active = screen_res_mode_ ? std::string("ON") : std::string("OFF");

						const char* sampling_mode[] =
						{
							"Nearest Neighbor"
							,"Bilinear"
							,"Cubic"
							,"Cubic Spline"
							,"Lancos"
							,"Average"
							,"Mode"
							,"Gaussian"
						};

						printf( "\nSAMPLING ALGORITHM: %s", sampling_mode[sampler_algorithm_index_] );
					}
				}
				break;
				default:
				break;
			};//End of switch
		}
		break;
		default:
		break;
	};//End of switch

	navHelper_->ProcessEvent(win_event, window_);
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

