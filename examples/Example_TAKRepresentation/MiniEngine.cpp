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
#include <chrono>
#include <string>

static const std::string IsContactPropertyName	= "IsContact"; //bool
static const std::string CallSignPropertyName	= "Callsign"; //string 
static const std::string UidPropertyName		= "EventUid"; //string
static const std::string ActiveStatePropertyName = "Active";

using clock_type = std::chrono::steady_clock;

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

	PathIntersectionTestStream()
	{
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

		FeatureInfo finfo = find(object_id);

		
		std::cout << " Feature Info: <id : " << object_id << " >  < Intersection Distance: " << finfo.intersection_distance << " >" << std::endl;

		 std::string prop_name;
		Geoweb3d::IGW3DStringPtr prop_value;
		bool feature_is_contact = false;

		for (int i = 0; i < attribute_collection->count(); i++)
		{
			prop_name = attribute_collection->get_DefinitionCollection()->get_AtIndex(i)->property_name;

			//if(feature_is_contact)
			{
				if(prop_name == CallSignPropertyName )
				{
					auto callsign = attribute_collection->get_AtIndex(i);
					if(callsign->is_string() )
					{
						selected_contact_callsign_ = callsign->raw_string();
					}
				}

				if(prop_name == UidPropertyName )
				{
					auto uid = attribute_collection->get_AtIndex(i);
					if(uid->is_string())
					{
						selected_contact_uid_ = uid->raw_string();
					}
				}
			}

			if(prop_name == IsContactPropertyName)
			{
				auto is_contact = attribute_collection->get_AtIndex(i);
				if(is_contact->is_bool() && is_contact)
				{
					feature_is_contact = is_contact;
				}
			}

			attribute_collection->get_AtIndex(i)->to_string(prop_value);
			std::cout << " " << prop_name.c_str() << "  :  " << prop_value->c_str() << " " << std::endl;
		}
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

	virtual bool next(int64_t* ppVal)
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

	void get_SelectedContact(std::string& callsign, std::string& uid)
	{
		callsign = selected_contact_callsign_;
		uid = selected_contact_uid_;
	}

private:
	std::string		selected_contact_uid_;
	std::string		selected_contact_callsign_;

	unsigned nIndex_;
	std::set< FeatureInfo > feature_info_set;
	std::set< FeatureInfo >::iterator feature_info_set_iterator;
};


// Contact feature streamer
class ContactFeatureStream : public Geoweb3d::IGW3DVectorLayerStream
{
public:

	ContactFeatureStream(Geoweb3d::IGW3DCollection<long>& contact_fids)
		: contact_fids_(contact_fids)
	{
		reset();
	}

	virtual ~ContactFeatureStream()
	{
	}

	// When IGW3DVectorLayer::Stream is called with this class, the SDK will call OnStream for each requested feature
	virtual bool OnStream(Geoweb3d::IGW3DVectorLayerStreamResult* result)
	{
		// Request the attribute data for the current feature
		const Geoweb3d::IGW3DAttributeCollection* attribute_collection = result->get_AttributeCollection();

		// Query the object id (or feature id) for the current feature
		long object_id = result->get_ObjectID();

		std::string prop_name;
		Geoweb3d::IGW3DStringPtr prop_value;
		bool feature_is_contact = false;
		bool is_active = false;

		std::string contact_callsign, contact_uid;

		for(int i = 0; i < attribute_collection->count(); i++)
		{
			prop_name = attribute_collection->get_DefinitionCollection()->get_AtIndex(i)->property_name;

			if(prop_name == CallSignPropertyName)
			{
				auto callsign = attribute_collection->get_AtIndex(i);
				if(callsign->is_string())
				{
					contact_callsign = callsign->raw_string();
				}
			}

			if(prop_name == UidPropertyName)
			{
				auto uid = attribute_collection->get_AtIndex(i);
				if(uid->is_string())
				{
					contact_uid = uid->raw_string();
				}
			}

			if(prop_name == ActiveStatePropertyName)
			{
				auto active = attribute_collection->get_AtIndex(i);
				if(active->is_bool())
				{
					is_active = active->to_bool();
				}
			}

			if(prop_name == IsContactPropertyName)
			{
				auto is_contact = attribute_collection->get_AtIndex(i);
				if(is_contact->is_bool() && is_contact)
				{
					feature_is_contact = is_contact;
				}
			}
		}

		std::string state = is_active ? std::string(" Added") : std::string(" Removed");

#ifdef SHOW_CONTACT_MESSAGES
		std::cout << "Contact CallSign: " << contact_callsign.c_str() << "\tContact UID: " << contact_uid.c_str()
			     << state.c_str() << std::endl;
#endif

		return true;
	}

	virtual bool OnError()
	{
		return false;
	}

	virtual unsigned long count() const
	{
		return contact_fids_.count();
	}

	virtual bool next(int64_t* ppVal)
	{
		long val;
		bool found = contact_fids_.next( &val );
		*ppVal = val;

		return found;
	}

	virtual void reset()
	{
		contact_fids_.reset();
	}

	virtual int64_t operator[](unsigned long index)
	{
		return get_AtIndex(index);
	}

	virtual int64_t get_AtIndex(unsigned long index)
	{
		return contact_fids_.get_AtIndex( index );
	}


private:
	Geoweb3d::IGW3DCollection<long>& contact_fids_;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">	Context for the sdk. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

MiniEngine::MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const CommandLineArgs& args)
	: sdk_context_(sdk_context)
	, navHelper_(new NavigationHelper())
{
	font_ = Geoweb3d::IGW3DFont::create("Arial.ttf", 10, Geoweb3d::IGW3DFont::NORMAL, false);

	//NOTE: Add a few generic messages that can be sent to a selected contact via the chat 'menu' option.
	chat_messages_[0] = "RGR";
	chat_messages_[1] = "@Brch";
	chat_messages_[2] = "@LCC";
	chat_messages_[3] = "@VOD";

	//Add a few different callsign to give the user the ability to change the callsign. If a callsign is specified 
	//on the command line( line 124, main.cpp) this will be used as the first callsign in the array.
	callsigns_[0] = args.callsign.empty() ? std::string("Geoweb3d_CallSign#2") : args.callsign;

	callsigns_[1] = "Geoweb3d_CallSign#3";
	callsigns_[2] = "Geoweb3d_CallSign#4";
	callsigns_[3] = "Geoweb3d_CallSign#5";

	roles_ =
	{
		Geoweb3d::IGW3DTAKClientContext::Role::Team_Member,
		Geoweb3d::IGW3DTAKClientContext::Role::Team_Lead,
		Geoweb3d::IGW3DTAKClientContext::Role::HQ,
		Geoweb3d::IGW3DTAKClientContext::Role::Sniper,
		Geoweb3d::IGW3DTAKClientContext::Role::Medic,
		Geoweb3d::IGW3DTAKClientContext::Role::Forward_Observer,
		Geoweb3d::IGW3DTAKClientContext::Role::RTO,
		Geoweb3d::IGW3DTAKClientContext::Role::K9
	};

	teams_ =
	{
		Geoweb3d::IGW3DTAKClientContext::Team::White,
		Geoweb3d::IGW3DTAKClientContext::Team::Yellow,
		Geoweb3d::IGW3DTAKClientContext::Team::Orange,
		Geoweb3d::IGW3DTAKClientContext::Team::Magenta,
		Geoweb3d::IGW3DTAKClientContext::Team::Red,
		Geoweb3d::IGW3DTAKClientContext::Team::Maroon,
		Geoweb3d::IGW3DTAKClientContext::Team::Purple,
		Geoweb3d::IGW3DTAKClientContext::Team::Dark_Blue,
		Geoweb3d::IGW3DTAKClientContext::Team::Blue,
		Geoweb3d::IGW3DTAKClientContext::Team::Cyan,
		Geoweb3d::IGW3DTAKClientContext::Team::Teal,
		Geoweb3d::IGW3DTAKClientContext::Team::Green,
		Geoweb3d::IGW3DTAKClientContext::Team::Dark_Green,
		Geoweb3d::IGW3DTAKClientContext::Team::Brown
	};

	//Assign the server url. The default is the FreeTAKServer. If a server ip/host name is
	//specified on the command line( line 111, main.cpp) it will be used as the actually
	//server connection instead of the default.
	server_url_ = args.server_ip.empty() ? std::string("204.48.30.216") : args.server_ip;
	server_port_ = args.server_port;

	tak_vector_driver_ = sdk_context_->get_VectorDriverCollection(true)->get_Driver("TAK_COT");
	data_driver_props_ = tak_vector_driver_.lock()->get_PropertyCollection()->create_Clone();

	if(!args.user_name.empty())
	{
		//If a user name is specified on the commandline ( line 131, main.cpp ) we use it to set the corresponding
		//vector driver property. This will allow us to pass the credential to the TAK server when connecting.
		data_driver_props_->put_Property(data_driver_props_->get_DefinitionCollection()->get_IndexByName("username"), args.user_name.c_str());
	}

	if(!args.password.empty())
	{
		//If a password is specified on the commandline ( line 138, main.cpp ) we use it to set the corresponding
		//vector driver property. This will allow us to pass the credential to the TAK server when connecting.
		data_driver_props_->put_Property(data_driver_props_->get_DefinitionCollection()->get_IndexByName("password"), args.password.c_str());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Destructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

MiniEngine::~MiniEngine()
{
	UnLoadTAK();

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

bool MiniEngine::LoadConfiguration(const char* example_name)
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


bool MiniEngine::LoadTAK()
{
	Geoweb3d::GW3DResult result;
	
	if (tak_vector_ds_.expired())
	{
		tak_vector_ds_ = tak_vector_driver_.lock()->get_VectorDataSourceCollection()->open( server_url_.c_str(), result, data_driver_props_);

		if (tak_vector_ds_.expired())
		{
			std::cout << "Could not load the following Vector Data Source\n";
			return false;
		}

		tak_client_context_ = Geoweb3d::RegisterTAKClientCallback(tak_vector_ds_.lock().get(), this);
	}

	if (!rep_collect_)
	{
		//NOTE: Using auto_Open allows the system to select the default, built-in representation corresponding to the 
		//the specified data source. Certain data sources have their specific internal representation that needs to be
		//enforced and aut_Open is the means by which to do so. This will ensure that the most suitable representation for
		//the datasource is selected.
		rep_collect_ = sdk_context_->get_VectorRepresentationDriverCollection()->auto_Open(tak_vector_ds_);

		if (rep_collect_)
		{
			line_segment_ = sdk_context_->get_LineSegmentIntersectionTestCollection()->create();

			for (int i = 0; i < rep_collect_->count(); i++)
			{
				Geoweb3d::IGW3DVectorRepresentationWPtr rep_w = rep_collect_->get_AtIndex(i);
				auto rep_l = rep_w.lock();
				if (rep_l)
				{
					if (!line_segment_.expired())
					{
						line_segment_.lock()->get_VectorRepresentationCollection()->add(rep_l);
					}
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool MiniEngine::UnLoadTAK()
{
	for (int i = 0; i < rep_collect_->count(); i++)
	{
		Geoweb3d::IGW3DVectorRepresentationWPtr temp = rep_collect_->get_AtIndex(i);
		close_VectorRepresentation(temp);
	}
	rep_collect_.reset();

	Geoweb3d::UnRegisterTAKClientCallback(tak_vector_ds_.lock().get(), this);

	close_VectorDataSource(tak_vector_ds_);

	tak_client_context_ = nullptr;

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

bool MiniEngine::LoadData(const char* rootpath)
{

	if (CreateImageryRep(sdk_context_, "../examples/media/test_service_xmls/servers/Bing/Hybrid.xml", 1, 1, true).expired())
	{
		printf("CreateImageryRep Error\n");
	}

	if (CreateImageryRep(sdk_context_, "../examples/media/DC_1ft_demo.ecw", 6, 2, true).expired())
	{
		printf("CreateImageryRep Error\n");
	}

	Geoweb3d::GW3DResult res;

	data_driver_props_->put_Property(data_driver_props_->get_DefinitionCollection()->get_IndexByName("callsign"), callsigns_[0].c_str() );

	if(server_port_ != -1)
	{
		data_driver_props_->put_Property(data_driver_props_->get_DefinitionCollection()->get_IndexByName("port"), server_port_ );
	}

	LoadTAK();

	if (CreateElevationRep(sdk_context_, "../examples/media/NED 10-meter DC.tif", 6, 0, true, 6.0).expired())
	{
		printf("CreateElevationRep Error\n");
	}

	camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);
	double longitude, latitude, elevation;
	longitude = -76.154545799999994;// (env.MinX + env.MaxX) * 0.5;
	latitude = 42.069567300000003;// (env.MinY + env.MaxY) * 0.5;
	
	camera_controller_->put_Location(longitude, latitude);
	camera_controller_->put_Elevation(1000.0);
	camera_controller_->put_Rotation(0, 80, 0);

	navHelper_->put_HomePosition(camera_);

	if(tak_client_context_ != nullptr)
	{
		Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode;
		camera_controller_->get_Elevation(elevation, mode );

		tak_client_context_->put_Position( longitude, latitude, 9999999.0/*elevation*/, Geoweb3d::IGW3DTAKClientContext::AltitudeType::AGL );
	}
	return true;
}

Geoweb3d::IGW3DWindowWPtr MiniEngine::Create3DWindow(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback* window_events)
{
	Geoweb3d::IGW3DWindowCollection* wcol = sdk_context->get_WindowCollection();

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

	Geoweb3d::IGW3DCameraCollection* cameracollection = windowptr.lock()->get_CameraCollection();
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

void MiniEngine::OnDraw2D(const Geoweb3d::IGW3DCameraWPtr& camera) 
{

}

void MiniEngine::OnDraw2D(const Geoweb3d::IGW3DCameraWPtr& camera, Geoweb3d::IGW3D2DDrawContext* drawcontext)
{

	static std::chrono::time_point<clock_type> start_time = clock_type::now();
	static std::chrono::time_point<clock_type> end_time	= clock_type::now();

	start_time = end_time;
	end_time = clock_type::now();

	auto dt_ms = std::chrono::duration_cast<std::chrono::milliseconds>( end_time - start_time).count();

	if(font_ != nullptr)
	{
		const int start_x = 10;
		const int start_y = 10;

		int x = start_x;
		int y = start_y;

		int row_height = font_->get_CellHeight();

		std::string server_status = "Server Connection: " + server_url_ + ":" + std::to_string( server_port_ ) + " -> " + (tak_server_connection_active_.load() ? std::string( "Active" ): std::string( "InActive" ) );

		const float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		const float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
		const float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
		const float cyan[4] = { 0.0f, 1.0f, 1.0f, 1.0f };

		const float* active_color = tak_server_connection_active_.load() ? green : red ;

		drawcontext->draw_Text(font_, x, y, active_color[0], active_color[1], active_color[2], active_color[3]
			                  , server_status.c_str() );
		y += row_height;

		if(!selected_contact_uid_.empty() && !selected_contact_callsign_.empty())
		{
			std::string contact_text = "Selected Contact<callsign:uid> : " + selected_contact_callsign_ + " : " + selected_contact_uid_;

			drawcontext->draw_Text(font_, x, y, green[0], green[1], green[2], green[3], contact_text.c_str());
			y += row_height;

			if( tak_client_context_ != nullptr  )
			{
				std::string callsign = tak_client_context_->get_Callsign();
				std::string uid = tak_client_context_->get_Uid();

				if(selected_contact_callsign_ != callsign && selected_contact_uid_ != uid)
				{
					drawcontext->draw_Text(font_, x, y, green[0], green[1], green[2], green[3], "Send Chat Message [1-4]");
					y += row_height;

					if(chat_message_index_ >= 0 && chat_message_index_ < NUM_CHAT_MESSAGE)
					{
						last_message_index_ = chat_message_index_;
						std::string text = callsign + " : " + chat_messages_[last_message_index_];
						chat_msg_queue_.emplace_back(std::make_pair(true, text));
					}

					if(chat_message_index_ >= 0)
					{
						tak_client_context_->sendChatMessage(selected_contact_callsign_.c_str()
							, selected_contact_uid_.c_str(), chat_messages_[last_message_index_].c_str());
					}

					chat_message_index_ = -1;

					drawcontext->draw_Text(font_, x, y, green[0], green[1], green[2], green[3], "Send Mission Package[5]");
					y += row_height;

					std::string text = std::string("Enable/Disable Mission Package Downloads[6] : ") + (accept_file_transfers_ ? std::string("Enabled") : std::string("Disabled"));
					drawcontext->draw_Text(font_, x, y, green[0], green[1], green[2], green[3], text.c_str());
					y += row_height;
				}
			}
		}

		drawcontext->draw_Text(font_, x, y, active_color[0], active_color[1], active_color[2], active_color[3], "Change Call Sign[7]");
		y += row_height;

		drawcontext->draw_Text(font_, x, y, active_color[0], active_color[1], active_color[2], active_color[3], "Change Role[8]");
		y += row_height;

		drawcontext->draw_Text(font_, x, y, active_color[0], active_color[1], active_color[2], active_color[3], "Change Team[9]");
		y += row_height;

		//
		auto window = window_.lock();
		int win_x, win_y;
		unsigned int win_width, win_height;
		window->get_WindowSize(win_x, win_y, win_width, win_height);

		unsigned int num_rows = static_cast<unsigned int>(chat_msg_queue_.size());
		unsigned int messages_height_span = num_rows * static_cast<unsigned int>( row_height );
		unsigned int _y = static_cast<unsigned int>(y);

		while(( _y + messages_height_span) >= win_height)
		{
			if(!chat_msg_queue_.empty())
			{
				chat_msg_queue_.pop_front();
			}

			num_rows = static_cast<unsigned int>(chat_msg_queue_.size());
			messages_height_span = num_rows * static_cast<unsigned int>( row_height );
		}

		for(size_t i = 0; i < chat_msg_queue_.size(); ++i)
		{
			bool self = chat_msg_queue_[i].first;
			const std::string& text = chat_msg_queue_[i].second;

			const float* msg_color = self ? blue : cyan;

			drawcontext->draw_Text(font_, x, y, msg_color[0], msg_color[1], msg_color[2], msg_color[3], text.c_str());
			y += row_height;
		}

		//File transfer notification.
		if( file_download_in_progress_.load() || file_upload_in_progress_.load() )
		{
			float alpha = 1.0f;

			if(file_download_completed_.load() || file_upload_completed_.load())
			{
				float dt = dt_ms / 1000.0f;
				static float fade_timer = 0.0f;
				float fade_time_seconds = static_cast<float>(fade_time_second_);

				alpha = (fade_time_seconds - fade_timer) / fade_time_seconds;

				fade_timer += dt;

				if(fade_timer >= fade_time_seconds)
				{
					fade_timer = 0.0f;
					alpha = 0.0f;
					if(file_download_in_progress_)
					{
						file_download_in_progress_.store(false);
						file_download_completed_.store(false);
					}

					if(file_upload_in_progress_)
					{
						file_upload_in_progress_.store(false);
						file_upload_completed_.store(false);
					}
				}
			}

			std::string transfer_notification = "File Transfer in progress...";

			int num_chars = static_cast<int>(transfer_notification.size());

			x = static_cast<int>(win_width) - ((num_chars + 1) * font_->get_CellWidth());
			y = static_cast<int>(win_height) - (2 * row_height);

			if( x > 0 && y > 0 )
			{
				drawcontext->draw_Text(font_, x, y, cyan[0], cyan[1], cyan[2], alpha, transfer_notification.c_str());
			}
		}
	}
}

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

			PathIntersectionTestStream streamer;
			bool found_one = false;
			while (intersection_report_collection->next(&intersection_report))
			{
				Geoweb3d::IGW3DVectorRepresentationWPtr rep = intersection_report->get_VectorRepresentation();
				const char* vector_driver_name = intersection_report->get_VectorRepresentation().lock()->get_Driver().lock()->get_Name();
				const char* vector_layer_name = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer().lock()->get_Name();

				Geoweb3d::IGW3DVectorLayerWPtr icon_vector_layer = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer();

				Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection* details_collection = intersection_report->get_IntersectionDetailCollection();

				Geoweb3d::IGW3DLineSegmentIntersectionDetail* intersection_detail;

				details_collection->reset();

				//collect details for the intersected feature(s)
				while (details_collection->next(&intersection_detail))
				{
					printf("FID HIT %d, at DIST %5.5f\n", intersection_detail->get_ObjectID(), intersection_detail->get_IntersectionDistance());
					streamer.insert(intersection_detail->get_ObjectID(), intersection_detail->get_IntersectionDistance());

					Geoweb3d::GW3DPoint tri_pt0;
					Geoweb3d::GW3DPoint tri_pt1;
					Geoweb3d::GW3DPoint tri_pt2;
					intersection_detail->get_TriangleIntersected(tri_pt0, tri_pt1, tri_pt2);
					printf("TRI0 %6.6f, %6.6f, %6.6f\n", tri_pt0.get_X(), tri_pt0.get_Y(), tri_pt0.get_Z());

					//just showing we can also access the actual intersection point
					const Geoweb3d::GW3DPoint* hit_point = intersection_detail->get_IntersectionPoint();
				}

				if (streamer.count())
				{
					// process the results
					icon_vector_layer.lock()->Stream(&streamer);

					streamer.get_SelectedContact(selected_contact_callsign_, selected_contact_uid_);
				}
			}
		}

		//one shot
		line_segment_.lock()->put_Enabled(false);
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
	case win_event.MouseMoved:
	{
		navHelper_->setMousePixelLocation(win_event.MouseMove.x, win_event.MouseMove.y);
	}
	break;
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

		switch (win_event.Key.code)
		{

		case Geoweb3d::Key::P:
		{
			static bool tak_loaded = true;
			if (tak_loaded)
			{
				UnLoadTAK();
			}
			else
			{
				LoadTAK();
			}
			tak_loaded = !tak_loaded;
			filter_out = true;
		}
		break;
		case Geoweb3d::Key::Num1:
		case Geoweb3d::Key::Numpad1:
		{
			if(!tak_server_connection_active_.load())
			{
				std::cout << "\nClient is currently not connected to a server.Please wait until connection is active." << std::endl;
				return;
			}

			if(selected_contact_uid_.empty() || selected_contact_callsign_.empty())
			{
				std::cout << "\nPlease select a message recipient." << std::endl;
				return;
			}

			if(chat_message_index_ == -1)
			{
				chat_message_index_ = 0;
			}
		}
		break;
		case Geoweb3d::Key::Num2:
		case Geoweb3d::Key::Numpad2:
		{
			if(!tak_server_connection_active_.load())
			{
				std::cout << "\nClient is currently not connected to a server.Please wait until connection is active." << std::endl;
				return;
			}

			if(selected_contact_uid_.empty() || selected_contact_callsign_.empty())
			{
				std::cout << "\nPlease select a message recipient." << std::endl;
				return;
			}

			if(chat_message_index_ == -1)
			{
				chat_message_index_ = 1;
			}
		}
		break;
		case Geoweb3d::Key::Num3:
		case Geoweb3d::Key::Numpad3:
		{
			if(!tak_server_connection_active_.load())
			{
				std::cout << "\nClient is currently not connected to a server.Please wait until connection is active." << std::endl;
				return;
			}

			if(selected_contact_uid_.empty() || selected_contact_callsign_.empty())
			{
				std::cout << "\nPlease select a message recipient." << std::endl;
				return;
			}

			if(chat_message_index_ == -1)
			{
				chat_message_index_ = 2;
			}
		}
		break;
		case Geoweb3d::Key::Num4:
		case Geoweb3d::Key::Numpad4:
		{
			if(!tak_server_connection_active_.load())
			{
				std::cout << "\nClient is currently not connected to a server.Please wait until connection is active." << std::endl;
				return;
			}

			if(selected_contact_uid_.empty() || selected_contact_callsign_.empty())
			{
				std::cout << "\nPlease select a message recipient." << std::endl;
				return;
			}

			if(chat_message_index_ == -1)
			{
				chat_message_index_ = 3;
			}
		}
		break;
		case Geoweb3d::Key::Num5:
		case Geoweb3d::Key::Numpad5:
		{
			static bool file_transfer_active = false;

			if(file_transfer_active)
			{
				return;
			}

			if(!tak_server_connection_active_.load())
			{
				std::cout << "\nClient is currently not connected to a server.Please wait until connection is active." << std::endl;
				return;
			}

			if(selected_contact_uid_.empty() || selected_contact_callsign_.empty())
			{
				std::cout << "\nPlease select the recipient of the mission package by clicking on a contact." << std::endl;
				return;
			}

			if(tak_client_context_ != nullptr)
			{
				file_transfer_active = true;
				
				file_upload_in_progress_.store(true);
				file_upload_completed_.store(false);

				//"../examples/media/test_images/test_images.zip"
				auto result = tak_client_context_->sendMissionPackage(selected_contact_callsign_.c_str()
					, selected_contact_uid_.c_str()
					, "../examples/media/test_images/test_images.zip"
					, "Test Images");

				if(Geoweb3d::GW3DResult::GW3D_sOk != result)
				{
					std::cout << "\nMission Package Transfer Error" << std::endl;
				}

				file_transfer_active = false;
			}
		}
		break;
		case Geoweb3d::Key::Num6:
		case Geoweb3d::Key::Numpad6:
		{
			accept_file_transfers_ = !accept_file_transfers_;
		}
		break;
		case Geoweb3d::Key::Num7:
		case Geoweb3d::Key::Numpad7:
		{
			callsign_index_ = (callsign_index_ + 1) % NUM_CALLSIGNS;

			if(tak_client_context_ != nullptr)
			{
				tak_client_context_->put_Callsign(callsigns_[callsign_index_].c_str() );
			}
		}
		break;
		case Geoweb3d::Key::Num8:
		case Geoweb3d::Key::Numpad8:
		{
			role_index_ = (role_index_ + 1) % NUM_ROLES;
			if(tak_client_context_ != nullptr)
			{
				tak_client_context_->put_Role(roles_[role_index_]);
			}
		}
		break;
		case Geoweb3d::Key::Num9:
		case Geoweb3d::Key::Numpad9:
		{
			team_index_ = (team_index_ + 1) % NUM_TEAMS;
			if(tak_client_context_ != nullptr)
			{
				tak_client_context_->put_Team(teams_[team_index_] );
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


//----Geoweb3d::IGW3DTAKClientCallback callback implementation.

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	 
/// Invoked whenever a TAK contact is added or removed from the server.
///</summary>
/// <remarks>  
/// </remarks>
///
////////////////////////////////////////////////////////////////////////////////////////////////////

void MiniEngine::onLayerContactChanged(const Geoweb3d::IGW3DTAKClientContext* context, Geoweb3d::IGW3DCollection<long>& modified_features, Geoweb3d::IGW3DVectorLayer* layer)
{
#ifdef SHOW_CONTACT_MESSAGES
	std::cout << "\n********" << modified_features.count() << " Contacts Modified********" << std::endl;
#endif

	ContactFeatureStream streamer(modified_features);

	layer->Stream(&streamer);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	 
/// Invoked whenever a TAK Cursor on Target(CoT) chat message is received.
///</summary>
/// <remarks>  
/// </remarks>
///
////////////////////////////////////////////////////////////////////////////////////////////////////

void MiniEngine::onChatMessageReceived(const Geoweb3d::IGW3DTAKClientContext* context, const char* sender_callsign, const char* sender_uid, const char* message)
{
	chat_src_contact_callsign_ = sender_callsign;
	chat_src_contact_uid_ = sender_uid;

	last_chat_message_ = message;

	std::string text = chat_src_contact_callsign_ + " : " + last_chat_message_;
	chat_msg_queue_.emplace_back( std::make_pair( false, text ) );
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Invoked whenever a file transfer to the client is initiated. 
///</summary>
/// <remarks>  
/// Implementer should return true to confirm/accept the transfer, or false to cancel the transfer.
/// </remarks>
///
////////////////////////////////////////////////////////////////////////////////////////////////////
bool MiniEngine::onFileTransferRequested(const Geoweb3d::IGW3DTAKClientContext* context, const char* sender_callsign, const char* local_file_path)
{
	std::string _local_file_path = local_file_path != nullptr ? local_file_path : "";
	if(!_local_file_path.empty())
	{
		file_download_completed_.store(false);
		file_download_in_progress_.store(accept_file_transfers_);

		return accept_file_transfers_;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	 
/// Invoked whenever an accepted file transfer is completed.
///</summary>
/// <remarks>  
/// local_file_path is the absolute path the received mission package on the local file system.
/// </remarks>
///
////////////////////////////////////////////////////////////////////////////////////////////////////
void MiniEngine::onFileTransferCompleted(const Geoweb3d::IGW3DTAKClientContext* context, const char* local_file_path)
{

	if(file_download_in_progress_.load())
	{
		mp_filepath_ = local_file_path;
	}
	file_download_completed_.store(true);
}

void MiniEngine::onFileTransferFailed(const Geoweb3d::IGW3DTAKClientContext* context, const char* local_file_path, const Geoweb3d::GW3DResult reason)
{
	file_download_completed_.store(false);
}

void MiniEngine::onFileTransferSendCompleted(const Geoweb3d::IGW3DTAKClientContext* context, const char* recipient_uid)
{
	file_upload_completed_.store(true);
}

void MiniEngine::onFileTransferSendFailed(const Geoweb3d::IGW3DTAKClientContext* context, const char* recipient_uid, const Geoweb3d::GW3DResult reason)
{
	file_upload_completed_.store(false);
}




////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	 
/// Invoked whenever connection to the server is active.
///</summary>
/// <remarks>  
///	Communication with the server should only occur when the server connection is active. 
/// </remarks>
///
////////////////////////////////////////////////////////////////////////////////////////////////////
void MiniEngine::onServerConnectionActive(const Geoweb3d::IGW3DTAKClientContext* context)
{
	tak_server_connection_active_.store(true);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Invoked whenever connection to the server is inactive or otherwise disabled.
///</summary>
/// <remarks>  
///	Communication with the server should only occur when the server connection is active. 
/// </remarks>
///
////////////////////////////////////////////////////////////////////////////////////////////////////
void MiniEngine::onServerConnectionInActive(const Geoweb3d::IGW3DTAKClientContext* context)
{
	tak_server_connection_active_.store(false);
}
