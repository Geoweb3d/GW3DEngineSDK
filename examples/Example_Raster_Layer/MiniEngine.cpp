#include "MiniEngine.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/GW3DRaster.h"
#include "engine/IGW3DBuffer.h"
#include "engine/IGW3DAsyncStatus.h"
#include "common/IGW3DDataBuffer.h"

#include "GeoWeb3dCore/SystemExports.h"

#include <iostream>       
#include <thread>         
#include <chrono>  
#include <fstream>
#include <string>
#include <iostream>
#include <deque>
#include <filesystem>

/*
	TODO 
	- add picking on the raster at a lon lat location
	- add an enc layer with higher priority

*/

std::atomic<bool> invalidate = true;

CaptureStream::CaptureStream(unsigned raster_width, unsigned  raster_height)
{
}

bool CaptureStream::OnStream(Geoweb3d::IGW3DRasterLayerEnvelopeStreamResult* result)
{
	if (result->get_Initializing())
	{
		async_status_ = result->get_AsyncStatus();
	}
	return true;
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
		, my_service_("myservice.xml")
	{
		raster_capture_ = CaptureStreamPtr(new CaptureStream());
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

	//

	class MyXMLConfigurator : public Geoweb3d::IG3DRasterServiceConfigVisitor
	{
	public:
		MyXMLConfigurator(const char* service_caps_path) :
			service_caps_path_(service_caps_path)
		{};

		virtual void visit(Geoweb3d::IGW3DWebServiceConfiguration * serviceconfig)
		{
			serviceconfig->put_CachePath("C:\\mycacheloc");

			Geoweb3d::IGW3DStringPtr desc = serviceconfig->get_CapabilitiesUrl();
			
			if(desc && desc->c_str())
			{
				//if you want the a wmts service to work in the future without an internet connection
				//you will need to save out a local copy of the capabilites of the wmts service 
				//and change the capabilities path to this local copy
				Geoweb3d::GW3DResult result;
				Geoweb3d::IGW3DDataBufferPtr data_buffer = Geoweb3d::OS_Helpers::GetHTTPFetch(desc->c_str(), result);
				if (data_buffer && result == Geoweb3d::GW3D_sOk)
				{
					data_buffer->WriteToFile(service_caps_path_, "WMTSCapabilities.xml");
					serviceconfig->put_CapabilitiesUrl((std::string(service_caps_path_) + "\\WMTSCapabilities.xml").c_str());
				}
			}
		}

		const char* service_caps_path_ = nullptr;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Preloads the data. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="rootpath">	The rootpath. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
#undef USE_DEFAULT_XML
#undef STRESS_TEST

	bool MiniEngine::PreLoadData(const char* rootpath)
	{

#ifdef USE_DEFAULT_XML
		std::string imagerypath(rootpath);
		my_service_ = imagerypath +"test_service_xmls/myservice.xml";

		
		return true;
#endif

		Geoweb3d::OS_Helpers::Allow_UntrustedSSL(true);
		///////////////////////////////////////////////////////////////////////////////////////
		///	WMS Testing
		///////////////////////////////////////////////////////////////////////////////////////
		
		const char* wms_service_full_service_url = "https://geoint.nrlssc.navy.mil/nrltileserver/wms?REQUEST=GetCapabilities&VERSION=1.1.1&SERVICE=WMS";
		
		///////////////////////////////////////////////////////////////////////////////////////
		///	WMTS Testing
		///////////////////////////////////////////////////////////////////////////////////////

		/*
			Datasource options

			(1) A url to the main WMTS capabilities
				: This will open a raster datasoure with raster layers for all the WMTS service layers
			(2) A url to capabilites of a specific layer
				: This will open a raster datasouce with 1 raster layer
			(3) An on disc Capabilites XML
				Will work the same way as 1 & 2 respectively
		*/

		//nrltileserver - good -----------------------------------------------------------------------------------------------
		const char* wmts_service_full_service_url = "https://geoint.nrlssc.navy.mil/nrltileserver/wmts2?REQUEST=GetCapabilities&VERSION=1.0.0&SERVICE=WMTS";
		const char* wmts_service_layer_url = "https://geoint.nrlssc.navy.mil/nrltileserver/wmts2?REQUEST=GetCapabilities&VERSION=1.0.0&SERVICE=WMTS,layer=BlueMarble_AUTO,tilematrixset=GlobalCRS84Pixel";
		const char* wmts_service_layer_xml = "bluemarble.xml"; 

		const char* filename = wmts_service_full_service_url;

		Geoweb3d::IGW3DRasterDriverCollection* raster_drivers = sdk_context_->get_RasterDriverCollection();
		Geoweb3d::IGW3DRasterDataSourceWPtr rdatasource;

		////////////////////////////////////////////////////////////
		//	First step is to open the datasource 
		//	Using auto open will try to find the correct driver
		////////////////////////////////////////////////////////////
		Geoweb3d::IGW3DRasterDriverWPtr temp_driver;

		bool auto_open = true;
		if (auto_open)
		{
			Geoweb3d::GW3DResult res;
			//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
			//having to guess which data source driver is needed to open up a particular dataset.
			rdatasource = raster_drivers->auto_Open(filename, res);
		}
		else
		{
			Geoweb3d::IGW3DRasterDriverWPtr wmts_Driver = raster_drivers->get_Driver("WMTS");
			Geoweb3d::GW3DResult res;
			rdatasource = wmts_Driver.lock()->get_RasterDataSourceCollection()->open(filename, res);
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		//	Loop thru the layers of the datasource to see what the individual layers are. 
		////////////////////////////////////////////////////////////////////////////////////////////
		int target_index = -1;
		std::string target_layer_name;

		if (!rdatasource.expired())
		{
			Geoweb3d::IGW3DRasterLayerCollection* layer_collection = rdatasource.lock()->get_RasterLayerCollection();
			unsigned total_layers = layer_collection->count();

			for (unsigned i = 0; i < total_layers; ++i)
			{
				auto locked_layer = layer_collection->get_AtIndex(i).lock();
				printf("Layer[%d] Name: %s \n", i, locked_layer->get_Name());

				//Testing code to  just look out for a target layer
				const char* result = strstr(locked_layer->get_Name(), "BlackMarble");
				if (result)
				{
					target_index = i;
					target_layer_name = locked_layer->get_Name();
				}
			}

			int layer_index = -1;
			if (total_layers == 1)
			{
				layer_index = 0;
			}
			else
			{
				printf("\n");
				printf("----------------------------------------------------------------------------\n");
				printf("Current Default layer is: %s\n", target_layer_name.c_str());
				printf("\n");
				printf("Enter a layer index (click enter to chose the default): ");

				while (layer_index < 0 || layer_index >= static_cast<int>( total_layers ))
				{
					std::string in;
					std::getline(std::cin, in);
					int res_int = std::atoi(in.c_str());
					if (std::strcmp(in.c_str() , "") == 0)
					{
						layer_index = target_index;
						break;
					}
					else
					{	
						layer_index = res_int;
					}
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////
			//	At this point we know what layer we are interested in based on the index we chose
			////////////////////////////////////////////////////////////////////////////////////////////

 			Geoweb3d::IGW3DRasterLayerWPtr layer = layer_collection->get_AtIndex(layer_index).lock();
			auto locked_layer = layer.lock();
			if (locked_layer)
			{
				//Lets see what details we can get about this layer. 

				printf("Constructing XML for Layer[%d] Name: %s \n", layer_index, locked_layer->get_Name());
				Geoweb3d::IGW3DStringPtr desc = locked_layer->get_Description();
				if (desc)
				{
					printf("Description: %s\n", desc->c_str());
				}
				Geoweb3d::GW3DEnvelope env = locked_layer->get_Envelope();
				printf("\t Env - MinX %f, MaxX %f, MaxY %f, MinY %f\n", env.MinX, env.MaxX, env.MaxY, env.MinY);
				printf("\t Width %d, Height %d\n", locked_layer->get_Width(), locked_layer->get_Height());
				printf("\t X DPP %f, Y DPP %f\n", locked_layer->get_PixelSizeX(), locked_layer->get_PixelSizeY());


				//Here we can write out the XML of the choosen layer (if we are not currently already working off a loaded XML);
				//(Again to note: we could have opened a layer XML from the start instead of URL )

				Geoweb3d::IG3DRasterServiceConfigurationPtr rconfig = layer.lock()->create_RasterServiceConfiguration();

				std::filesystem::path cwd = std::filesystem::current_path();

				MyXMLConfigurator mhyconfigvistor(cwd.string().c_str());
				rconfig->accept(&mhyconfigvistor);
				Geoweb3d::IGW3DStringPtr xmlstr = rconfig->get_SerializeToXMLString();

				if (!xmlstr->empty())
				{
					std::ofstream file(my_service_);
					std::string my_string = xmlstr->c_str();
					file << my_string;
					file.close();

					//Now that we have the desired XML configuration for this layer we will close this
					//datasource and open a new one 
				}
				else
				{
					printf("Unable to produce an XML for the service layer requested\n");
				}
			}

			close_RasterDataSource(rdatasource);
		}
		else
		{
			return false;
		}

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

		Geoweb3d::IGW3DRasterRepresentationDriverCollection* raster_repdrivers = sdk_context_->get_RasterLayerRepresentationDriverCollection();
		Geoweb3d::IGW3DRasterRepresentationDriverWPtr imagery_driver = raster_repdrivers->get_Driver("Imagery");
		
		Geoweb3d::GW3DEnvelope env;


		if (1)
		{
			Geoweb3d::IGW3DRasterDriverCollection* raster_drivers = sdk_context_->get_RasterDriverCollection();
			Geoweb3d::IGW3DRasterDataSourceWPtr rdatasource;

			////////////////////////////////////////////////////////////
			//	First step is to open the datasource 
			////////////////////////////////////////////////////////////
			bool auto_open = true;
			if (auto_open)
			{
				Geoweb3d::GW3DResult res;
				rdatasource = raster_drivers->auto_Open(my_service_.c_str(), res);
			}
			else
			{

				Geoweb3d::IGW3DRasterDriverWPtr wmts_Driver = raster_drivers->get_Driver("WMTS");
				Geoweb3d::GW3DResult res;
				rdatasource = wmts_Driver.lock()->get_RasterDataSourceCollection()->open(my_service_.c_str(), res);
			}

			if (!rdatasource.expired())
			{
				Geoweb3d::IGW3DRasterLayerCollection* layer_collection = rdatasource.lock()->get_RasterLayerCollection();

				raster_layer_ = layer_collection->get_AtIndex(0).lock();
				auto locked_layer = raster_layer_.lock();
				if (locked_layer)
				{
					printf("Layer Name: %s \n", locked_layer->get_Name());
					Geoweb3d::IGW3DStringPtr desc = locked_layer->get_Description();
					printf("Description: %s\n", desc->c_str());
					env = locked_layer->get_Envelope();
					printf("\t Env - MinX %f, MaxX %f, MaxY %f, MinY %f\n", env.MinX, env.MaxX, env.MaxY, env.MinY);
					printf("\t Width %d, Height %d\n", locked_layer->get_Width(), locked_layer->get_Height());
					printf("\t X DPP %f, Y DPP %f\n", locked_layer->get_PixelSizeX(), locked_layer->get_PixelSizeY());

					//-----------------Layer Query Utilities-------------//

#ifdef STRESS_TEST
					double start_lon = -100;
					double end_lon = -80;
					double start_lat = 40;
					double delta = 1.0; //geocell

					std::deque< Geoweb3d::IGW3DAsyncStatusWPtr> pasync_container;
					std::deque< Geoweb3d::IGW3DAsyncStatusWPtr>::iterator itr;
					static int counter = 0;

					int number_of_queries = fabs(end_lon - start_lon) * 2;

					printf("Doing %d queries\n", number_of_queries);

					while (1)
					{
						if (start_lon < end_lon)
						{
							Geoweb3d::IGW3DAsyncStatusPtr pasync;

							Geoweb3d::IGW3DRequest reg;
							reg.raster_width = 512;
							reg.raster_height = 512;
							Geoweb3d::GeometryUtility::set_WGS84Envelope(reg, start_lat + delta, start_lat, start_lon, start_lon + delta);
							raster_layer_.lock()->StreamEnvelopeAsync(&reg, raster_capture_);
							pasync_container.push_back(raster_capture_->async_status_);

							//A SECOND ROW UNDER THE ONE ABOVE
							Geoweb3d::GeometryUtility::set_WGS84Envelope(reg, start_lat, start_lat - delta, start_lon, start_lon + delta);
							raster_layer_.lock()->StreamEnvelopeAsync(&reg, raster_capture_);
							pasync_container.push_back(raster_capture_->async_status_);

							start_lon += delta;
						}
						else
						{
							break;
						}

					}

					while (1)
					{
						for (itr = pasync_container.begin(); itr != pasync_container.end();)
						{
							if ((*itr).lock() && (*itr).lock()->is_finished())
							{
								itr = pasync_container.erase(itr);
								printf("%d - \n ", number_of_queries--);
							}
							else
							{
								itr++;
							}
						}
					}
#else
					if (0)
					{
						double start_lon = -80;
						double start_lat = 0;
						double delta = 5.0; //geocell

						Geoweb3d::IGW3DAsyncStatusWPtr pasync;
						static int counter = 0;

						while (1)
						{
							if (invalidate)
							{
								invalidate = false;

								if (start_lat < 84 && start_lon < 84.0)
								{
									Geoweb3d::IGW3DRequest reg;
									reg.raster_width = 1064;
									reg.raster_height = 840;
									Geoweb3d::GeometryUtility::set_WGS84Envelope(reg, start_lat + delta, start_lat, start_lon, start_lon + delta);

									raster_layer_.lock()->StreamEnvelope(&reg, raster_capture_.get());
									raster_layer_.lock()->StreamEnvelopeAsync(&reg, raster_capture_);
									pasync = raster_capture_->async_status_;

									if (counter % 2 == 1)
									{
										start_lon += delta;
										start_lat += delta;
									}
								}
							}

							auto lock_ptr = pasync.lock();
							if (lock_ptr && lock_ptr->is_finished())
							{
								//increment x to get teh next data
								counter += 1;
								invalidate = true;
							}

							std::this_thread::sleep_for(std::chrono::seconds(1));
							printf(".");
						}

						close_RasterDataSource(rdatasource);
					}
#endif
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		///	Load and represent the Service
		///////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRESS_TEST

		if (CreateImageryRep(sdk_context_, my_service_.c_str(), 6, 0, true).expired())
		{
			printf("CreateImageryRep Error\n");
		}


		///////////////////////////////////////////////////////////////////////////////////////////
		///	Load and represent a simple 1 layers ECW file
		///////////////////////////////////////////////////////////////////////////////////////////
		if (CreateImageryRep(sdk_context_, "../examples/media/DC_1ft_demo.ecw", 6, 1, true, env).expired())
		{
			printf("CreateImageryRep Error\n");
		}
#endif
		///////////////////////////////////////////////////////////////////////////////////////////
		///	Configure the camera start up location
		///////////////////////////////////////////////////////////////////////////////////////////
		camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);

		double longitude, latitude;
		longitude = (env.MinX + env.MaxX) * 0.5;
		latitude = (env.MinY + env.MaxY) * 0.5;

		camera_controller_->put_Location(longitude, latitude);
		camera_controller_->put_Elevation(8000.0);
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

		bool filter_out = false;

		switch (win_event.Type)
		{
			case win_event.MouseButtonPressed:
			{
				switch (win_event.MouseButton.button)
				{
					case Geoweb3d::Mouse::Left:
					{
						
					}
					break;

					default:
					break;
				};//End of Switch
			}
			break;

			case win_event.KeyPressed:
			{
				switch (win_event.Key.code)
				{
					case Geoweb3d::Key::A:
					{
					}
					break;

					case Geoweb3d::Key::S:
					{
						
					}
					break;

					case Geoweb3d::Key::D:
					{
						
					}
					break;

					case Geoweb3d::Key::F:
					{
						
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

		LRESULT CALLBACK MiniEngine::ProcessLowLevelEvent( OSWinHandle hWndParent, unsigned int message, WPARAM wParam, LPARAM lParam)
		{
			// We don't forward the WM_CLOSE message to prevent the internal SDK from closing us (click on X)
			if (message == WM_CLOSE)
			{
				printf("ProcessLowLevelEvent: WM_Close detected and prevented!\n");
				return 0;
			}
			return 1;
		}

