#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/GW3DRaster.h"
#include "engine/GW3DFrameAnalysis.h"

#include "engine/IGW3DEventStream.h"
#include "engine/IGW3DWindowCallback.h"
#include "engine/IGW3DVectorRepresentationCollection.h"
#include "engine/IGW3DSceneGraphContext.h"
#include "engine/IGW3DImageCollection.h"
#include "engine/IGW3DImage.h"
#include "core/GW3DSharedPtr.h"

//The Geoweb3dCore APIs are from the previous C-like API
//and will eventualy be fully replaced in the C++ API and deprecated
#include "Geoweb3dCore/GeometryExports.h"
#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/LayerParameters.h"

// A simple example of navigation control
#include "../Common/NavigationHelper.h"

#include <deque>
#include <set>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <tchar.h>

#define GLEW_STATIC
#include <GL/glew.h>

//#include <winsock2.h>
//#include <ws2tcpip.h>

#pragma comment( lib, "GW3DEngineSDK.lib" )
#pragma comment( lib, "GW3DCommon.lib" )
#pragma comment( lib, "Opengl32.lib" )
#pragma comment( lib, "glew32s.lib" )


void my_fatal_function( const char* msg );
void my_info_function( const char *msg );
void SetInformationHandling();

//struct to store information about selected features
struct FeatureInfo
{
	long	id;
	double	intersection_distance;
	int		texture_prop_index;

	FeatureInfo (long object_id, double distance = 0., int tex_prop_index = 0)
		: id (object_id), intersection_distance(distance), texture_prop_index (tex_prop_index)
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
class IconIntersectionTestStream : public Geoweb3d::IGW3DVectorLayerStream
{

	public:

		IconIntersectionTestStream( )
		{
			reset();
		}

		virtual ~IconIntersectionTestStream( )
		{
			//cleanup
			feature_info_set.clear();
		}

		//insert selected features to be processed
		void insert (long object_id, double intersection_distance, int tex_palette_index)
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
			finfo.texture_prop_index = tex_palette_index;
			finfo.intersection_distance = intersection_distance;
			feature_info_set.insert(finfo);
		}

		//find FeatureInfo for a given object id
		const FeatureInfo& find (long object_id)
		{
			return *feature_info_set.find(object_id);
		}

		// When IGW3DVectorLayer::Stream is called with this class, the SDK will call OnStream for each requested feature
		virtual bool OnStream( Geoweb3d::IGW3DVectorLayerStreamResult* result )
		{
			// Request the attibute data for the current feature
			const Geoweb3d::IGW3DAttributeCollection* attribute_collection = result->get_AttributeCollection();

			// Query the object id (or feature id) for the current feature
			long object_id = result->get_ObjectID();

			FeatureInfo finfo = find(object_id);

			int texture_palette_index = attribute_collection->get_Property( finfo.texture_prop_index );

			std::cout<<" Feature Info: <id : "<< object_id << " > "<< "< #: "<<texture_palette_index+1<<" > "
					     <<" < Intersection Distance: "<< finfo.intersection_distance<< " >"<<std::endl;
			return true;
		}

		virtual bool OnError( ) 
		{
			return false;
		}

		virtual unsigned long count() const
		{
			return (long)feature_info_set.size();
		}

		virtual bool next( int64_t *ppVal )
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
			return get_AtIndex( index );
		}

		virtual int64_t get_AtIndex( unsigned long index )
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

	private:

		unsigned nIndex_;
		std::set< FeatureInfo > feature_info_set;
		std::set< FeatureInfo >::iterator feature_info_set_iterator;
};

class IconDrawEventTask
{
	public:

		IconDrawEventTask()
		{}

		IconDrawEventTask( Geoweb3d::IGW3DGeoweb3dSDKPtr& sdk_context, Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr line_segment, int tex_palette_index )
			: m_sdk_context ( sdk_context )
			, m_line_segment( line_segment )
			, tex_palette_index_( tex_palette_index )
		{}

		~IconDrawEventTask() {}

		void OnPreDraw()
		{
			m_line_segment.lock()->put_Enabled ( true ); //only do a one shot
		}

		void OnPostDraw( bool draw_succeeded )
		{
			if( !draw_succeeded )
				return;

			Geoweb3d::IGW3DLineSegmentIntersectionReportCollection* intersection_report_collection = m_line_segment.lock()->get_IntersectionReportCollection();

			Geoweb3d::IGW3DLineSegmentIntersectionReport* intersection_report;

			intersection_report_collection->reset();
					
			IconIntersectionTestStream streamer;	

			while( intersection_report_collection->next( &intersection_report ) )
			{
				const char* vector_driver_name = intersection_report->get_VectorRepresentation().lock()->get_Driver().lock()->get_Name();
				const char* vector_layer_name  = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer().lock()->get_Name();

				Geoweb3d::IGW3DVectorLayerWPtr icon_vector_layer = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer();
				   
				Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection* details_collection = intersection_report->get_IntersectionDetailCollection();

				Geoweb3d::IGW3DLineSegmentIntersectionDetail* intersection_detail;

				details_collection->reset();

				//collect details for the intersected feature(s)
				while( details_collection->next( &intersection_detail ) )
				{
					streamer.insert(intersection_detail->get_ObjectID(), intersection_detail->get_IntersectionDistance(), tex_palette_index_);

					//just showing we can also access the actual intersection point
					const Geoweb3d::GW3DPoint* hit_point = intersection_detail->get_IntersectionPoint();
				}

				if (streamer.count())
				{
					// process the results
					icon_vector_layer.lock()->Stream( &streamer );
				}

  			}

			//one shot
			m_line_segment.lock()->put_Enabled( false );
		}

	public:

		int tex_palette_index_;
		Geoweb3d::IGW3DGeoweb3dSDKPtr m_sdk_context;
		Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr m_line_segment;
};

class App : public Geoweb3d::IGW3DVectorLayerStream, public Geoweb3d::IGW3DWindowCallback
{
	public:

		App( const Geoweb3d::IGW3DGeoweb3dSDKPtr& sdk_context )
			: sdk_context_( sdk_context )
			, image_texpalette_prop_index_( 3 )
			, raster_layer_name_index_( 0 )
			, draw_task_active_( false )
			, wire_frame_( false )
			, navHelper_(new NavigationHelper())
			, anchor_(Geoweb3d::IGW3DImage::ANCHOR_POSITION::MIDDLE_CENTER)
		{
		}

		~App()
		{
			delete navHelper_;
			images_.clear();
		}

		//IGW3DWindowCallback
		void OnCreate( ){}

		void OnDrawBegin( ){}

		void OnDrawEnd( ) {}

		void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr& camera ){}
		
		void ProcessEvent( const Geoweb3d::WindowEvent& win_event )
		{
			bool filter_out = false;

			switch( win_event.Type )
			{
				case win_event.MouseButtonReleased:
				{
					switch( win_event.MouseButton.button )
					{
						case Geoweb3d::Mouse::Left:
						{
							if( !line_segment_.expired() )
							{
								//configure the pick ray
								Geoweb3d::GW3DPoint ray_start;
								Geoweb3d::GW3DPoint ray_end;

								int mouse_x = win_event.MouseButton.x;
								int mouse_y = win_event.MouseButton.y;

								Geoweb3d::IGW3DCameraController* camera_controller = camera_.lock()->get_CameraController();

								camera_controller->get_Ray( mouse_x, mouse_y, ray_start, ray_end );

								const Geoweb3d::GW3DPoint* cam_pos =camera_controller->get_Location();

								double cam_x = cam_pos->get_X();
								double cam_y = cam_pos->get_Y();
								double cam_z = cam_pos->get_Z();

								double r_startx = ray_start.get_X();
								double r_starty = ray_start.get_Y();
								double r_startz = ray_start.get_Z();

								double r_endx = ray_end.get_X();
								double r_endy = ray_end.get_Y();
								double r_endz = ray_end.get_Z();

								line_segment_.lock()->put_StartEnd( *cam_pos, ray_end );

								frame_task_.push_back( Geoweb3d::GW3DSharedPtr< IconDrawEventTask >( new IconDrawEventTask( sdk_context_, line_segment_, image_texpalette_prop_index_ ) ) );
								draw_task_active_ = true;
							}
						}
						break;

						default:
						break;
					};
				}
				break;
				
				case win_event.KeyPressed:
				{
					switch (win_event.Key.code)
					{
					case Geoweb3d::Key::C:
					{
						anchor_ = (Geoweb3d::IGW3DImage::ANCHOR_POSITION)(anchor_ + 1);
						if (anchor_ == Geoweb3d::IGW3DImage::ANCHOR_POSITION::APM_MAX)
						{
							anchor_ = Geoweb3d::IGW3DImage::ANCHOR_POSITION::TOP_LEFT;
						}

						if (anchor_ == Geoweb3d::IGW3DImage::ANCHOR_POSITION::TOP_LEFT)
						{
							printf("ANCHOR MODE: TOP_LEFT \n");
						}
						else if (anchor_ == Geoweb3d::IGW3DImage::ANCHOR_POSITION::TOP_CENTER)
						{
							printf("ANCHOR MODE: TOP_CENTER \n");
						}
						else if (anchor_ == Geoweb3d::IGW3DImage::ANCHOR_POSITION::TOP_RIGHT)
						{
							printf("ANCHOR MODE: TOP_RIGHT \n");
						}
						else if (anchor_ == Geoweb3d::IGW3DImage::ANCHOR_POSITION::MIDDLE_LEFT)
						{
							printf("ANCHOR MODE: MIDDLE_LEFT \n");
						}
						else if (anchor_ == Geoweb3d::IGW3DImage::ANCHOR_POSITION::MIDDLE_CENTER)
						{
							printf("ANCHOR MODE: MIDDLE_CENTER \n");
						}
						else if (anchor_ == Geoweb3d::IGW3DImage::ANCHOR_POSITION::MIDDLE_RIGHT)
						{
							printf("ANCHOR MODE: MIDDLE_RIGHT \n");
						}
						else if (anchor_ == Geoweb3d::IGW3DImage::ANCHOR_POSITION::BOTTOM_LEFT)
						{
							printf("ANCHOR MODE: BOTTOM_LEFT \n");
						}
						else if (anchor_ == Geoweb3d::IGW3DImage::ANCHOR_POSITION::BOTTOM_CENTER)
						{
							printf("ANCHOR MODE: BOTTOM_CENTER \n");
						}
						else if (anchor_ == Geoweb3d::IGW3DImage::ANCHOR_POSITION::BOTTOM_RIGHT)
						{
							printf("ANCHOR MODE: BOTTOM_RIGHT \n");
						}

						for ( Geoweb3d::IGW3DImageWPtr& image : images_ )
						{
							if ( !image.expired() )
								image.lock()->put_AnchorPosition( anchor_ );
						}

						if ( !current_rep_.expired() )
							current_rep_.lock()->put_GW3DFinalizationToken( image_collection_->create_FinalizeToken() );

						filter_out = true;
					}
					break;

					default:
						break;
					};

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

		bool AddBillboardLayer( const char* layer_name, double lon, double lat, double elev )
		{
			layer_ = CreateLayer( layer_name, lon, lat, elev, 0.0, 0.0, 0.0, 1.0 );

			if( layer_.expired() )
			{
				std::cout<<"Error creating "<< layer_name <<std::endl;
				return false;
			}

			Geoweb3d::GW3DEnvelope envelope = layer_.lock()->get_Envelope();

			double cam_lon = ( envelope.MaxX + envelope.MinX ) * 0.5;
			double cam_lat = ( envelope.MaxY + envelope.MinY ) * 0.5;
			const double cam_elev = elev;//( envelope.MaxZ + envelope.MinZ ) * 0.5;

			//The camera is position at the center of the layer so 
			//layer creation need to happen prior to this.
			//UpdateCamera( cam_lon, cam_lat, cam_elev );

			if( !CreateBillboardRepresentation( layer_ ) )
			{
				std::cout<<"Error creating "<< layer_name << " representation." <<std::endl;
				return false;
			}

			layer_.unsafe_get()->Stream( reinterpret_cast< Geoweb3d::IGW3DVectorLayerStream* >( this ) );

			if( !line_segment_.expired() )
			{
				line_segment_.lock()->get_VectorRepresentationCollection()->add( current_rep_ );
			}

			return true;
		}

		bool Initialize()
		{
			window_ = sdk_context_->get_WindowCollection()->create_3DWindow( "Billboard Example", GW3D_OVERLAPPED, 10, 10, 1280,720, 0, Geoweb3d::IGW3DStereoDriverPtr(), this ); 
			Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context_->get_VectorDriverCollection()->get_Driver( "Geoweb3d_Datasource" );

			glewInit();

			if( driver.expired() )
			{
				std::cout << "Could not find the requested driver! For a list of supported drivers, run the Print Capabilites example" << std::endl;
				return false;
			}

			if( !driver.lock()->get_IsCapable( "CreateDataSource" ) )
			{
				std::cout << "This driver is not capable of creating new datasources!" << std::endl;
				return false;
			}

			data_source_ = driver.lock()->get_VectorDataSourceCollection()->create( "TestDataSource" );

			if( data_source_.expired() )
			{
				std::cout << "Could not create data source." << std::endl;
				return false;
			}

			//Raster
			CreateElevationLayer();
			Geoweb3d::GW3DEnvelope raster_envelope = raster_layer_.lock()->get_Envelope();

			CreateImages();

			CreateFieldDefinition();

			//line segment
			line_segment_ = sdk_context_->get_LineSegmentIntersectionTestCollection()->create();

			double lon = -77.0493;//( raster_envelope.MaxX + raster_envelope.MinX ) * 0.5;
			double lat = 38.92323;//( raster_envelope.MaxY + raster_envelope.MinY ) * 0.5;
			double elev = 1000.0;

			UpdateCamera( lon, lat, elev );

			AddBillboardLayer( "Billboards", lon, lat, 0.0 );

			return true;
		}

		void Run()
		{
			bool valid = true;

			while( valid )
			{
				DoPreDrawTask();

				valid = ( sdk_context_->draw( window_ ) == Geoweb3d::GW3D_sOk );

				DoPostDrawTask( valid );
			}
		}

		void DoPreDrawTask()
		{
			if( frame_task_.empty() )
				return;

			for( size_t i = 0; i < frame_task_.size(); ++i )
			{
				frame_task_[i]->OnPreDraw();
			}

			draw_task_active_ = false;
		}

		void DoPostDrawTask( bool draw_successful )
		{
			if( frame_task_.empty() || draw_task_active_ )
				return;

			for( size_t i = 0; i < frame_task_.size(); ++i )
			{
				frame_task_[i]->OnPostDraw( draw_successful );
			}

			frame_task_.clear();
		}

		//IGW3DVectorLayerStream overrides
		virtual bool OnStream( Geoweb3d::IGW3DVectorLayerStreamResult* result )
		{
			const Geoweb3d::IGW3DAttributeCollection* attribute_collection = result->get_AttributeCollection();
		    const Geoweb3d::IGW3DPropertyCollection* representation_properties = result->get_VectorRepresentationProperties( current_rep_ );
			Geoweb3d::IGW3DPropertyCollectionPtr feature_prop;

			const Geoweb3d::IGW3DVariant color_r = attribute_collection->get_Property( color_r_prop_index_ );
			const Geoweb3d::IGW3DVariant color_g = attribute_collection->get_Property( color_g_prop_index_ );
			const Geoweb3d::IGW3DVariant color_b = attribute_collection->get_Property( color_b_prop_index_ );
			const Geoweb3d::IGW3DVariant color_a = attribute_collection->get_Property( color_a_prop_index_ );

			const Geoweb3d::IGW3DVariant img_property_collection_id = attribute_collection->get_Property( image_texpalette_prop_index_ );

			//if( representation_properties )
			//{
			//	//const Geoweb3d::IGW3DDefinitionCollection* rep_def = representation_properties->get_DefinitionCollection();
			//	result->put_VectorRepresentationProperty( current_rep_, rep_img_collection_prop_index_, img_property_collection_id );
			//}
			//else
			//{
			//	result->put_VectorRepresentationProperty( current_rep_, rep_img_collection_prop_index_, img_property_collection_id );
			//}

			result->put_VectorRepresentationProperty( current_rep_, rep_img_collection_prop_index_, img_property_collection_id );

			result->put_VectorRepresentationProperty( current_rep_, rep_color_r_prop_index_, color_r );
			result->put_VectorRepresentationProperty( current_rep_, rep_color_g_prop_index_, color_g );
			result->put_VectorRepresentationProperty( current_rep_, rep_color_b_prop_index_, color_b );
			result->put_VectorRepresentationProperty( current_rep_, rep_color_a_prop_index_, color_a );

			return true;
		}

		virtual bool OnError( ) 
		{
			return false;
		}

		virtual unsigned long count() const
		{
			return 0;
		}

		virtual bool next( int64_t *ppVal )
		{
			return true;
		}

		virtual void reset()
		{

		}

		virtual int64_t operator[](unsigned long index)
		{
			return 0;
		}

		virtual int64_t get_AtIndex( unsigned long index )
		{
			return 0;
		}  

		void UpdateCamera( double lon, double lat, double elev )
		{
			camera_lon_ = lon;
			camera_lat_ = lat;
			camera_elev_= elev;

			double cam_lon = lon;
			double cam_lat = lat;
			const double cam_elev = elev;

			float pitch = 0.0f;

			if( camera_.expired() )
			{
				camera_ = window_.lock()->get_CameraCollection()->create( "Main Camera" );
				navHelper_->add_Camera(camera_);
			}

			camera_.lock()->get_CameraController()->put_Location( cam_lon, cam_lat );
			camera_.lock()->get_CameraController()->put_Elevation( cam_elev, Geoweb3d::IGW3DPositionOrientation::Relative );
			camera_.lock()->get_CameraController()->put_Rotation( 0.0f, pitch, 0.0f );
		}

		Geoweb3d::IGW3DVectorLayerWPtr CreateLayer( const char* layer_name, double layer_lon, double layer_lat, double elev, double r, double g, double b, double a )
		{
			Geoweb3d::IGW3DVectorLayerWPtr layer = data_source_.lock()->get_VectorLayerCollection()->create( layer_name, Geoweb3d::gtPOINT_25D, field_definition_ );

			if( !layer.expired() )
			{
				const unsigned int NUM_ROWS = static_cast<unsigned int>( images_.size() * 20 );
				const unsigned int NUM_COLS = static_cast<unsigned int>( images_.size() * 20 );
				const unsigned int NUM_FEATURES = NUM_ROWS * NUM_COLS;

				const double inter_feature_gap = 0.02;

				double lon = layer_lon - ( NUM_ROWS * inter_feature_gap * 0.5 );
				double lat = layer_lat - ( NUM_COLS * inter_feature_gap * 0.5 );
				//double elev = 2000.0;

				const double start_lon = lon;
				const double start_lat = lat;

				Geoweb3d::GW3DResult result;

				Geoweb3d::IGW3DPropertyCollectionPtr temp_collection = field_definition_->create_PropertyCollection();

				temp_collection->put_Property( color_r_prop_index_, r );
				temp_collection->put_Property( color_g_prop_index_, g );
				temp_collection->put_Property( color_b_prop_index_, b );
				temp_collection->put_Property( color_a_prop_index_, a );

				temp_collection->put_Property( icon_width_index_, 128.0 );
				temp_collection->put_Property( icon_height_index_, 128.0 );

				unsigned int image_index = 0;

				for( unsigned int i = 0; i < NUM_FEATURES; ++i )
				{
					if( ( i > 0 ) )
					{
						if( ( i % NUM_ROWS == 0 ) )
						{
							lon = start_lon;
						}

						if( ( i % NUM_COLS == 0 ) )
						{
							lat+=inter_feature_gap;
						}
					}

					image_index = i % images_.size();

					temp_collection->put_Property( image_texpalette_prop_index_, images_[image_index].unsafe_get()->get_PropertyCollectionID() );

					Geoweb3d::GW3DPoint p( lon, lat, elev );
					layer.unsafe_get()->create_Feature( temp_collection, &p, result );

					lon+=inter_feature_gap;

				}
			}

			return layer;
		}
		
		void CreateElevationLayer()
		{
			if( raster_data_source_.expired() )
			{
				Geoweb3d::GW3DResult res;

				//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
				//having to guess which data source driver is needed to open up a particular dataset.
				raster_data_source_ = sdk_context_->get_RasterDriverCollection()->auto_Open( "../examples/media/NED 10-meter DC.tif", res );

				Geoweb3d::IGW3DRasterLayerCollection* layer_collection = raster_data_source_.lock()->get_RasterLayerCollection();
				raster_layer_ = layer_collection->get_AtIndex(0);
			}

			if( elevation_rep_driver_.expired() )
			{
				elevation_rep_driver_ = sdk_context_->get_RasterLayerRepresentationDriverCollection()->get_Driver( "Elevation" );
			}

			if( !elevation_rep_driver_.expired() )
			{
				Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
				params.page_level = 6;
				params.priority = 0;
				params.representation_layer_activity = true;
				Geoweb3d::IGW3DRasterRepresentationWPtr raster_rep = elevation_rep_driver_.lock()->get_RepresentationLayerCollection()->create(raster_layer_, params);
			}

		}

		bool CreateBillboardRepresentation( Geoweb3d::IGW3DVectorLayerWPtr layer )
		{
			Geoweb3d::IGW3DVectorRepresentationDriverWPtr rep_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver( "BillBoard" );

			if( rep_driver.expired() )
			{
				std::cout<< "Error creating vector representation driver." <<std::endl;
				return false;
			}

			Geoweb3d::GW3DResult can_represent;
			can_represent = rep_driver.unsafe_get()->get_CapabilityToRepresent( layer );

			if( !Geoweb3d::Succeeded( can_represent ) )
			{
				std::cout << "Error: The selected representation driver cannot represent this layer." << std::endl;
				return false;
			}

			//create some default properties for the layer
			Geoweb3d::IGW3DPropertyCollectionPtr default_properties = rep_driver.lock()->get_PropertyCollection()->create_Clone();

			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_Z_OFFSET" ), 0.0 );

			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "RED" ),   1.0 );
			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "GREEN" ), 1.0 );
			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "BLUE" ),  1.0 );
			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "ALPHA" ),  0.25 );
			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "TEXTURE_TO_COLOR_BLEND" ),  1.0 );
			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "TEXTURE_PALETTE_INDEX" ),  0 );

			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "SIZE_X" ),  40.0 );
			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "SIZE_Y" ),  40.0 );

			Geoweb3d::Vector::RepresentationLayerCreationParameter rep_layer_creation_params;

			rep_layer_creation_params.page_level = 8;

			rep_layer_creation_params.finalization_token = image_collection_->create_FinalizeToken();

			rep_layer_creation_params.representation_default_parameters = default_properties;

			current_rep_ = rep_driver.lock()->get_RepresentationLayerCollection()->create( layer, rep_layer_creation_params );

			current_rep_.lock()->get_ClampRadiusFilter()->put_Enabled( true );
			current_rep_.lock()->get_ClampRadiusFilter()->put_MaxRadius( 30000.0 );


			//representation property indices.
			rep_img_collection_prop_index_ = current_rep_.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName( "TEXTURE_PALETTE_INDEX" );
			rep_color_r_prop_index_ = current_rep_.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("RED" );
			rep_color_g_prop_index_ = current_rep_.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("GREEN" );
			rep_color_b_prop_index_ = current_rep_.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("BLUE" );
			rep_color_a_prop_index_ = current_rep_.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("ALPHA" );

			return true;
		}

		void CreateFieldDefinition()
		{
			field_definition_ = Geoweb3d::IGW3DDefinitionCollection::create();

			color_r_prop_index_ = field_definition_->add_Property( "COLOR_R", Geoweb3d::PROPERTY_DOUBLE, "" );
			color_g_prop_index_ = field_definition_->add_Property( "COLOR_G", Geoweb3d::PROPERTY_DOUBLE, "" );
			color_b_prop_index_ = field_definition_->add_Property( "COLOR_B", Geoweb3d::PROPERTY_DOUBLE, "" );
			color_a_prop_index_ = field_definition_->add_Property( "COLOR_A", Geoweb3d::PROPERTY_DOUBLE, "" );

			field_definition_->add_Property( "BLEND_FACTOR", Geoweb3d::PROPERTY_DOUBLE, "" );

			icon_width_index_  = field_definition_->add_Property( "ICON_WIDTH", Geoweb3d::PROPERTY_DOUBLE, "" );
			icon_height_index_ = field_definition_->add_Property( "ICON_HEIGHT", Geoweb3d::PROPERTY_DOUBLE, "" );

			image_texpalette_prop_index_ = field_definition_->add_Property( "TEXTURE_ARRAY_INDEX", Geoweb3d::PROPERTY_INT, "" );
		}

		void CreateImages()
		{
			if(!image_collection_ )
			{
				image_collection_ = sdk_context_->get_SceneGraphContext()->create_ImageCollection();
			}
	
			images_.push_back( image_collection_->create( "../examples/media/test_images/one.png" ) );
			images_.push_back( image_collection_->create( "../examples/media/test_images/two.png" ) );
			images_.push_back( image_collection_->create( "../examples/media/test_images/three.png" ) );
			images_.push_back( image_collection_->create( "../examples/media/test_images/four.png" ) );
			images_.push_back( image_collection_->create( "../examples/media/test_images/five.png" ) );
			images_.push_back( image_collection_->create( "../examples/media/test_images/six.png" ) );
		}

	private:

		Geoweb3d::IGW3DGeoweb3dSDKPtr			sdk_context_;
		Geoweb3d::IGW3DVectorDataSourceWPtr		data_source_; 
		Geoweb3d::IGW3DRasterLayerWPtr			raster_layer_;
		Geoweb3d::IGW3DRasterDataSourceWPtr		raster_data_source_;
		Geoweb3d::IGW3DWindowWPtr				window_;
		Geoweb3d::IGW3DVectorLayerWPtr			layer_;
		Geoweb3d::IGW3DVectorLayerWPtr			elevation_layer_;

		Geoweb3d::IGW3DDefinitionCollectionPtr	field_definition_;
		Geoweb3d::IGW3DVectorRepresentationWPtr	current_rep_;
		Geoweb3d::IGW3DImageCollectionPtr		image_collection_;
		Geoweb3d::IGW3DCameraWPtr				camera_;

		Geoweb3d::IGW3DRasterRepresentationDriverWPtr elevation_rep_driver_;
		Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr line_segment_;

		std::vector< Geoweb3d::IGW3DImageWPtr >	images_;
		Geoweb3d::IGW3DImage::ANCHOR_POSITION anchor_;

		std::deque< Geoweb3d::GW3DSharedPtr< IconDrawEventTask > > frame_task_;

		double camera_lon_;
		double camera_lat_;
		double camera_elev_;

		int color_r_prop_index_;
		int color_g_prop_index_;
		int color_b_prop_index_;
		int color_a_prop_index_;

		int icon_width_index_;
		int icon_height_index_;

		int image_texpalette_prop_index_;
		int raster_layer_name_index_;

		int rep_img_collection_prop_index_;
		int rep_color_r_prop_index_;
		int rep_color_g_prop_index_;
		int rep_color_b_prop_index_;
		int rep_color_a_prop_index_;

		bool draw_task_active_;
		bool wire_frame_;

		NavigationHelper* navHelper_;


};

//Application driver function.
void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
	App app( sdk_context );

	if( app.Initialize() )
	{
		app.Run();
	}
}


int _tmain( int argc, _TCHAR* argv[] )
{
    SetInformationHandling();

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if( sdk_context )
    {
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
        {
            RunApplication( sdk_context );
        }
        else
        {
			std::cout << "\nError creating SDK context! Press any key to continue...\n" << std::endl;
            getchar();
        }
    }
    else
    {
        return -1;
    }

   //Example showing control over SDK destruction.  This will invalidate all
    //the pointers the SDK owned! ( xxx.expired() == true )
    sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();

    return 0;
}


/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function(const char *msg )
{
	std::cout<< "Fatal Info: " << msg;
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function( const char* msg )
{
	std::cout<< "General Info: " << msg;
}

/*! Information handling is not required */
void SetInformationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}

