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
#include "Geoweb3d/engine/IGW3DFont.h"

// A simple example of navigation control
#include "../Common/NavigationHelper.h"
#include "../Common/MiniEngineCore.h"

#include <deque>
#include <set>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <tchar.h>
#include <sstream>

#pragma comment( lib, "GW3DEngineSDK.lib" )
#pragma comment( lib, "GW3DCommon.lib" )


void my_fatal_function(const char* msg );
void my_info_function(const char *msg );
void SetInformationHandling();

//struct to store information about selected features
struct FeatureInfo
{
	long	id;
	double	intersection_distance;
	int		texture_prop_index;

	FeatureInfo(long object_id, double distance = 0., int tex_prop_index = 0)
		: id(object_id), intersection_distance(distance), texture_prop_index(tex_prop_index)
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

	IconIntersectionTestStream()
	{
		reset();
	}

	virtual ~IconIntersectionTestStream()
	{
		//cleanup
		feature_info_set.clear();
	}

	//insert selected features to be processed
	void insert(long object_id, double intersection_distance, int tex_palette_index)
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

		int texture_palette_index = attribute_collection->get_Property(finfo.texture_prop_index);

		std::cout << " Feature Info: <id : " << object_id << " > " << "< #: " << texture_palette_index + 1 << " > "
			<< " < Intersection Distance: " << finfo.intersection_distance << " >" << std::endl;
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

	virtual bool next(int64_t *ppVal)
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

private:

	unsigned nIndex_;
	std::set< FeatureInfo > feature_info_set;
	std::set< FeatureInfo >::iterator feature_info_set_iterator;
};

class IconDrawEventTask;

class App : public Geoweb3d::IGW3DVectorLayerStream, public Geoweb3d::IGW3DWindowCallback, public MiniEngineCore
{
public:

	App(const Geoweb3d::IGW3DGeoweb3dSDKPtr& sdk_context)
		: sdk_context_(sdk_context)
		, image_texpalette_prop_index_(3)
		, raster_layer_name_index_(0)
		, draw_task_active_(false)
		, wire_frame_(false)
		, navHelper_(new NavigationHelper())
		, anchor_(Geoweb3d::IGW3DImage::ANCHOR_POSITION::MIDDLE_LEFT) // initialized anchor to Middle Left so that the first change of anchor will center all images
	{
		current_fid_ = -1;

		/*struct prop_state
	{
		float texture_blend_;
		float b_red_;
		float b_green_;
		float b_blue_;
		float b_alpha_;
		float z_offset_;
		int z_offset_mmode_;
		bool auto_height_;
		float l_red_;
		float l_green_;
		float l_blue_;
		float l_alpha_;
		float size_x_;
		float size_y_;
		float leader_top_y_increase_percent_;
		int palette_;
	};*/

		default_state_ = {
			1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			30.0f,
			Geoweb3d::Vector::TRANSLATION_Z_OFFSET_MODE::RELATIVE_MODE, // | Geoweb3d::Vector::TRANSLATION_Z_OFFSET_MODE::IGNORE_VERTEX_Z,
			true,
			1.0f, 0.0f, 0.0f, 1.0f,
			40.0f, 40.0f,
			0.0f,
			-1
		};

		specific_state_ = {
			1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			30.0f,
			Geoweb3d::Vector::TRANSLATION_Z_OFFSET_MODE::RELATIVE_MODE, // | Geoweb3d::Vector::TRANSLATION_Z_OFFSET_MODE::IGNORE_VERTEX_Z,
			true,
			1.0f, 0.0f, 0.0f, 1.0f,
			40.0f, 40.0f,
			0.0f,
			-1
		};

	}

	~App()
	{
		delete navHelper_;
		images_.clear();
	}

	//IGW3DWindowCallback
	void OnCreate() {}

	void OnDrawBegin() {}

	void OnDrawEnd() {}

	void OnDraw2D(const Geoweb3d::IGW3DCameraWPtr& camera) {}

	void OnDraw2D(const Geoweb3d::IGW3DCameraWPtr& camera, Geoweb3d::IGW3D2DDrawContext *drawcontext) 
	{
		if ( font_ )
		{
			const int text_height = font_->get_CellHeight();
			const int x = font_->get_CellWidth();
			int y = 0;

			float txt_r = 0.25f;
			float txt_g = 0.25f;
			float txt_b = 0.25f;
			float txt_a = 1.0f;

			{
				y += text_height;
				std::stringstream ss;
				ss << "Key [6] - Depth Testing: " << (default_layer_props_.disable_depth_test ? "Disabled" : "Enabled") << std::endl;
				drawcontext->draw_Text( font_, x, y, txt_r, txt_g, txt_b, txt_a, ss.str().c_str() );
			}

			{
				y += text_height;
				std::stringstream ss;
				ss << "Key [0] - Leader Lines: ";
				if ( default_layer_props_.leaders_enabled == Geoweb3d::Vector::BillboardProperties::LEADER_LINE_OFF )
				{
					ss << "Off";
				}
				else if ( default_layer_props_.leaders_enabled == Geoweb3d::Vector::BillboardProperties::LEADER_LINE_ON )
				{
					ss << "On";
				}
				else if ( default_layer_props_.leaders_enabled == Geoweb3d::Vector::BillboardProperties::LEADER_LINE_ON_EXTENDED )
				{
					ss << "On ( Extended )";
				}
				drawcontext->draw_Text( font_, x, y, txt_r, txt_g, txt_b, txt_a, ss.str().c_str() );
			}

			{
				y += text_height;
				std::stringstream ss;
				ss << "Key [7] - Occlusion Per-Pixel Mode: ";
				if ( default_layer_props_.per_pixel_mode == Geoweb3d::Vector::BillboardProperties::APPLY_EFFECT_TO_BILLBOARDS )
				{
					ss << "Billboard Only";
				}
				else if ( default_layer_props_.per_pixel_mode == Geoweb3d::Vector::BillboardProperties::APPLY_EFFECT_TO_LEADER_LINES )
				{
					ss << "Leader Line Only";
				}
				else if ( default_layer_props_.per_pixel_mode == Geoweb3d::Vector::BillboardProperties::APPLY_EFFECT_TO_FEATURE )
				{
					ss << "Billboard and Leader Line";
				}
				else
				{
					ss << "Disabled";
				}
				drawcontext->draw_Text( font_, x, y, txt_r, txt_g, txt_b, txt_a, ss.str().c_str() );
			}

			{
				y += text_height;
				std::stringstream ss;
				ss << "Key [8] - Occlusion Per-Object Mode: ";
				if ( default_layer_props_.per_object_mode == Geoweb3d::Vector::BillboardProperties::APPLY_EFFECT_TO_BILLBOARDS )
				{
					ss << "Billboard Only";
				}
				else if ( default_layer_props_.per_object_mode == Geoweb3d::Vector::BillboardProperties::APPLY_EFFECT_TO_LEADER_LINES )
				{
					ss << "Leader Line Only";
				}
				else if ( default_layer_props_.per_object_mode == Geoweb3d::Vector::BillboardProperties::APPLY_EFFECT_TO_FEATURE )
				{
					ss << "Billboard and Leader Line";
				}
				else
				{
					ss << "Disabled";
				}
				drawcontext->draw_Text( font_, x, y, txt_r, txt_g, txt_b, txt_a, ss.str().c_str() );
			}

			{
				y += text_height;
				std::stringstream ss;
				ss << "Key [9] - Occlusion Render Mode: ";
				if ( default_layer_props_.occlusion_render_mode == Geoweb3d::Vector::BillboardProperties::OCCLUDED_BILLBOARDS_RENDER_DASHED )
				{
					ss << "Dashed";
				}
				else if ( default_layer_props_.occlusion_render_mode == Geoweb3d::Vector::BillboardProperties::OCCLUDED_BILLBOARDS_RENDER_SHADED )
				{
					ss << "Shaded ( " << default_layer_props_.shaded_blend_amount << " ) Keys [4-increase, 1-decrease]";
				}
				else if ( default_layer_props_.occlusion_render_mode == Geoweb3d::Vector::BillboardProperties::OCCLUDED_BILLBOARDS_RENDER_UNCHANGED )
				{
					ss << "Unchanged";
				}
				drawcontext->draw_Text( font_, x, y, txt_r, txt_g, txt_b, txt_a, ss.str().c_str() );
			}
		}
	}

	void SetupFont( const std::string& name, int size )
	{
		if ( !current_rep_.expired() )
		{
			billboard_font_ = Geoweb3d::IGW3DFont::create( name.c_str(), size, Geoweb3d::IGW3DFont::NORMAL, false );
			if ( billboard_font_ )
				current_rep_.lock()->put_Font( billboard_font_ );

			const Geoweb3d::IGW3DPropertyCollection* label_props = current_rep_.lock()->get_LabelProperties();
			if ( label_props )
			{
				Geoweb3d::IGW3DPropertyCollectionPtr updated_label_props = label_props->create_Clone();
				updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::FOREGROUND_COLOR_RED, 1.0 );
				updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::FOREGROUND_COLOR_GREEN, 1.0 );
				updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::FOREGROUND_COLOR_BLUE, 1.0 );
				updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::FOREGROUND_COLOR_ALPHA, 1.0 );
				updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::BACKGROUND_COLOR_RED, 0.0 );
				updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::BACKGROUND_COLOR_GREEN, 0.0 );
				updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::BACKGROUND_COLOR_BLUE, 0.0 );
				updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::BACKGROUND_COLOR_ALPHA, 1.0 );
				current_rep_.lock()->put_LabelProperties( updated_label_props );
			}
		}
	}

	void ProcessEvent(const Geoweb3d::WindowEvent& win_event)
	{
		bool filter_out = false;

		switch (win_event.Type)
		{
		case win_event.MouseButtonReleased:
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

					//frame_task_.push_back( Geoweb3d::GW3DSharedPtr< IconDrawEventTask >( new IconDrawEventTask( sdk_context_, line_segment_, image_texpalette_prop_index_, this ) ) );
					//draw_task_active_ = true;

					//filter_out = true;
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
			prop_state* state;
			if (current_fid_ >= 0)
			{
				state = &specific_state_;
			}
			else
			{
				state = &default_state_;
			}

			switch (win_event.Key.code)
			{
			case Geoweb3d::Key::W:
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

			case Geoweb3d::Key::A:
			{
				state->z_offset_mmode_ ^= 0x01;
				if (state->z_offset_mmode_ & 0x01)
				{
					printf("RELATIVE IS ON\n");
				}
				else
				{
					printf("RELATIVE IS OFF\n");
				}
				props_dirty_ = true;
				filter_out = true;
			}
			break;

			case Geoweb3d::Key::Q:
			{
				state->z_offset_mmode_ ^= 0x02;
				if (state->z_offset_mmode_ & 0x02)
				{
					printf("IGNORE Z IS ON\n");
				}
				else
				{
					printf("IGNORE Z IS OFF\n");
				}
				props_dirty_ = true;
				filter_out = true;
			}
			break;

			case Geoweb3d::Key::S:
			{
				state->auto_height_ = !state->auto_height_;
				if (state->auto_height_)
				{
					printf("AUTO HEIGHT IS ON\n");
				}
				else
				{
					printf("AUTO HEIGHT IS OFF\n");
				}
				props_dirty_ = true;
				filter_out = true;
			}
			break;

			case Geoweb3d::Key::D:
			{
				if (win_event.Key.shift)
				{
					state->z_offset_ -= 1.0f;
					if (state->z_offset_ < -100.0f)
						state->z_offset_ = -100.0f;
				}
				else
				{
					state->z_offset_ += 1.0f;
					if (state->z_offset_ > 100.0f)
						state->z_offset_ = 100.0f;
				}
				printf("Z OFFSET: %3.1f\n", state->z_offset_);
				props_dirty_ = true;
				filter_out = true;
			}
			break;

			case Geoweb3d::Key::B:
			{
				if (win_event.Key.shift)
				{
					state->b_green_ -= 0.1f;
					if (state->b_green_ < 0.0f)
						state->b_green_ = 0.0f;
				}
				else
				{
					state->b_green_ += 0.1f;
					if (state->b_green_ > 1.0f)
						state->b_green_ = 1.0f;
				}
				printf("GREEN COMPONENT: %1.1f\n", state->b_green_);
				props_dirty_ = true;
				filter_out = true;
			}
			break;

			case Geoweb3d::Key::E:
			{
				if (win_event.Key.shift)
				{
					state->size_x_ -= 1;
					if (state->size_x_ < 0)
						state->size_x_ = 0;
				}
				else
				{
					state->size_x_ += 1;
					if (state->size_x_ > 128)
						state->size_x_ = 128;
				}
				printf("SIZE X COMPONENT: %f\n", state->size_x_);
				props_dirty_ = true;
				filter_out = true;
			}
			break;

			case Geoweb3d::Key::R:
			{
				if (win_event.Key.shift)
				{
					state->size_y_ -= 1;
					if (state->size_y_ < 1)
						state->size_y_ = 1;
				}
				else
				{
					state->size_y_ += 1;
					if (state->size_y_ > 128)
						state->size_y_ = 128;
				}
				printf("SIZE Y COMPONENT: %f\n", state->size_y_);
				props_dirty_ = true;
				filter_out = true;
			}
			break;

			case Geoweb3d::Key::T:
			{
				if (win_event.Key.shift)
				{
					state->texture_blend_-= 0.1f;
					if (state->texture_blend_ < 0.0f)
					{
						state->texture_blend_ = -1.0f;
						//modulate the texture by the color (black stays black, white area become the target color)
						printf("COLOR MODULATION MODE \n");
					}
				}
				else
				{
					state->texture_blend_ += 0.1f;
					if (state->texture_blend_ < 0.0f)
					{
						state->texture_blend_ = 0.0f;
					}
					else if (state->texture_blend_ > 1.0f)
					{
						state->texture_blend_ = 1.0f;
					}
				}
				printf("TEXTURE TO COLOR BLEND: %1.1f\n", state->texture_blend_);
				props_dirty_ = true;
				filter_out = true;
			}
			break;


			case Geoweb3d::Key::Num0:
			case Geoweb3d::Key::Numpad0:
			{
				auto ToggleLeaderLines = []( Geoweb3d::IGW3DVectorRepresentationWPtr& rep )
				{
					if ( !rep.expired() )
					{
						const Geoweb3d::IGW3DPropertyCollection* props = rep.lock()->get_PropertyCollection();
						const Geoweb3d::IGW3DVariant mode_prop = props->get_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::LEADER_LINES_ENABLED );
						int mode = mode_prop.to_int();
						mode = ++mode % 3;

						printf( "Leader Lines Mode: %i \n", mode );

						auto new_props = props->create_Clone();
						new_props->put_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::LEADER_LINES_ENABLED, mode );
						rep.lock()->put_PropertyCollection( new_props );

						return mode;
					}

					return 0;
				};

				default_layer_props_.leaders_enabled = ToggleLeaderLines( current_rep_ );
				filter_out = true;
			}
			break;

			case Geoweb3d::Key::Num7:
			case Geoweb3d::Key::Numpad7:
			{
				auto ChangePerPixelMode = [&]( Geoweb3d::IGW3DVectorRepresentationWPtr& rep )
				{
					if ( !rep.expired() )
					{
						const Geoweb3d::IGW3DPropertyCollection* props = rep.lock()->get_PropertyCollection();
						const Geoweb3d::IGW3DVariant mode_prop = props->get_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_PER_PIXEL_EFFECT );
						int mode = mode_prop.to_int();
						mode = ++mode % 4;

						printf( "Using OCCLUDED_PER_PIXEL_EFFECT Mode: %i \n", mode );

						auto new_props = props->create_Clone();
						new_props->put_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_PER_PIXEL_EFFECT, mode );
						rep.lock()->put_PropertyCollection( new_props );

						return mode;
					}

					return static_cast<int>( Geoweb3d::Vector::BillboardProperties::BILLBOARD_EFFECT_OFF );
				};

				default_layer_props_.per_pixel_mode = ChangePerPixelMode( current_rep_ );
				filter_out = true;
			}
			break;

			case Geoweb3d::Key::Num4:
			case Geoweb3d::Key::Numpad4:
			{
				auto IncrementStippleAmount = []( Geoweb3d::IGW3DVectorRepresentationWPtr& rep )
				{
					if ( !rep.expired() )
					{
						const Geoweb3d::IGW3DPropertyCollection* props = rep.lock()->get_PropertyCollection();
						const Geoweb3d::IGW3DVariant prop = props->get_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_BILLBOARD_SHADING_BLEND_AMOUNT );
						double value = prop.to_double();
						value = fmin( 1.0, value + 0.05 );

						printf( "OCCLUDED_BILLBOARD_SHADING_BLEND_AMOUNT: %lf \n", value );

						auto new_props = props->create_Clone();
						new_props->put_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_BILLBOARD_SHADING_BLEND_AMOUNT, value );
						rep.lock()->put_PropertyCollection( new_props );

						return value;
					}

					return 1.0;
				};

				default_layer_props_.shaded_blend_amount = IncrementStippleAmount( current_rep_ );
				filter_out = true;
			}
			break;

			case Geoweb3d::Key::Num1:
			case Geoweb3d::Key::Numpad1:
			{
				auto DecrementStippleAmount = []( Geoweb3d::IGW3DVectorRepresentationWPtr& rep )
				{
					if ( !rep.expired() )
					{
						const Geoweb3d::IGW3DPropertyCollection* props = rep.lock()->get_PropertyCollection();
						const Geoweb3d::IGW3DVariant prop = props->get_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_BILLBOARD_SHADING_BLEND_AMOUNT );
						double value = prop.to_double();
						value = fmax( 0.0, value - 0.05 );

						printf( "OCCLUDED_BILLBOARD_SHADING_BLEND_AMOUNT: %lf \n", value );

						auto new_props = props->create_Clone();
						new_props->put_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_BILLBOARD_SHADING_BLEND_AMOUNT, value );
						rep.lock()->put_PropertyCollection( new_props );

						return value;
					}

					return 1.0;
				};

				default_layer_props_.shaded_blend_amount = DecrementStippleAmount( current_rep_ );
				filter_out = true;
			}
			break;


			case Geoweb3d::Key::Num8:
			case Geoweb3d::Key::Numpad8:
			{
				auto ChangePerPObjectMode = []( Geoweb3d::IGW3DVectorRepresentationWPtr& rep )
				{
					if ( !rep.expired() )
					{
						const Geoweb3d::IGW3DPropertyCollection* props = rep.lock()->get_PropertyCollection();
						const Geoweb3d::IGW3DVariant mode_prop = props->get_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_PER_OBJECT_EFFECT );
						int mode = mode_prop.to_int();
						mode = ++mode % 4;

						printf( "Using OCCLUDED_PER_OBJECT_EFFECT Mode: %i \n", mode );

						auto new_props = props->create_Clone();
						new_props->put_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_PER_OBJECT_EFFECT, mode );
						rep.lock()->put_PropertyCollection( new_props );

						return mode;
					}

					return static_cast<int>( Geoweb3d::Vector::BillboardProperties::BILLBOARD_EFFECT_OFF );
				};

				default_layer_props_.per_object_mode = ChangePerPObjectMode( current_rep_ );
				filter_out = true;
			}
			break;

			case Geoweb3d::Key::Num9:
			case Geoweb3d::Key::Numpad9:
			{
				auto ChangeOcclusionRenderMode = []( Geoweb3d::IGW3DVectorRepresentationWPtr& rep )
				{
					if ( !rep.expired() )
					{
						const Geoweb3d::IGW3DPropertyCollection* props = rep.lock()->get_PropertyCollection();
						const Geoweb3d::IGW3DVariant mode_prop = props->get_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_BILLBOARD_RENDER_MODE );
						int mode = mode_prop.to_int();

						mode = ++mode % 3;

						printf( "Using OCCLUDED_BILLBOARD_RENDER_MODE Mode: %i \n", mode );

						auto new_props = props->create_Clone();
						new_props->put_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_BILLBOARD_RENDER_MODE, mode );
						rep.lock()->put_PropertyCollection( new_props );

						return mode;
					}

					return static_cast<int>( Geoweb3d::Vector::BillboardProperties::OCCLUDED_BILLBOARDS_RENDER_UNCHANGED );
				};

				default_layer_props_.occlusion_render_mode = ChangeOcclusionRenderMode( current_rep_ );
				filter_out = true;
			}
			break;

			case Geoweb3d::Key::Num6:
			case Geoweb3d::Key::Numpad6:
			{
				auto ChangeDepthMode = []( Geoweb3d::IGW3DVectorRepresentationWPtr& rep )
				{
					if ( !rep.expired() )
					{
						const Geoweb3d::IGW3DPropertyCollection* props = rep.lock()->get_PropertyCollection();
						const Geoweb3d::IGW3DVariant enable_prop = props->get_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::DISABLE_DEPTH );
						bool enabled = enable_prop.to_bool();

						enabled = !enabled;
						printf( "Billboard Depth Disabled: %s \n", enabled ? "TRUE" : "FALSE" );

						auto new_props = props->create_Clone();
						new_props->put_Property( Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::DISABLE_DEPTH, enabled );
						rep.lock()->put_PropertyCollection( new_props );

						return enabled;
					}

					return false;
				};

				default_layer_props_.disable_depth_test = ChangeDepthMode( current_rep_ );
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

	bool AddBillboardLayer(const char* layer_name, double lon, double lat, double elev)
	{
		layer_ = CreateLayer(layer_name, lon, lat, elev, default_state_.b_red_, default_state_.b_green_, default_state_.b_blue_, default_state_.b_alpha_);

		if (layer_.expired())
		{
			std::cout << "Error creating " << layer_name << std::endl;
			return false;
		}

		Geoweb3d::GW3DEnvelope envelope = layer_.lock()->get_Envelope();

		double cam_lon = (envelope.MaxX + envelope.MinX) * 0.5;
		double cam_lat = (envelope.MaxY + envelope.MinY) * 0.5;
		const double cam_elev = elev;//( envelope.MaxZ + envelope.MinZ ) * 0.5;

		//The camera is position at the center of the layer so 
		//layer creation need to happen prior to this.
		//UpdateCamera( cam_lon, cam_lat, cam_elev );

		if (!CreateBillboardRepresentation(layer_))
		{
			std::cout << "Error creating " << layer_name << " representation." << std::endl;
			return false;
		}

		layer_.unsafe_get()->Stream(reinterpret_cast<Geoweb3d::IGW3DVectorLayerStream*>(this));

		if (!line_segment_.expired())
		{
			line_segment_.lock()->get_VectorRepresentationCollection()->add(current_rep_);
		}

		return true;
	}

	bool Initialize()
	{
		font_ = Geoweb3d::IGW3DFont::create( "Arial.ttf", 14, Geoweb3d::IGW3DFont::NORMAL, false );
		billboard_font_ = Geoweb3d::IGW3DFont::create( "Arial.ttf", 12, Geoweb3d::IGW3DFont::NORMAL, false );

		window_ = sdk_context_->get_WindowCollection()->create_3DWindow("Billboard Properties Example", GW3D_OVERLAPPED, 10, 10, 1280, 720, 0, Geoweb3d::IGW3DStereoDriverPtr(), this);
		Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context_->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");

		if (driver.expired())
		{
			std::cout << "Could not find the requested driver! For a list of supported drivers, run the Print Capabilities example" << std::endl;
			return false;
		}

		if (!driver.lock()->get_IsCapable("CreateDataSource"))
		{
			std::cout << "This driver is not capable of creating new datasources!" << std::endl;
			return false;
		}

		data_source_ = driver.lock()->get_VectorDataSourceCollection()->create("TestDataSource");

		if (data_source_.expired())
		{
			std::cout << "Could not create data source." << std::endl;
			return false;
		}

		LoadDemoImageryDataset( sdk_context_ );

		//Raster
		CreateElevationLayer();

		Geoweb3d::GW3DEnvelope raster_envelope = raster_layer_.lock()->get_Envelope();

		CreateImages();

		CreateFieldDefinition();

		//line segment
		line_segment_ = sdk_context_->get_LineSegmentIntersectionTestCollection()->create();

		double lon = (raster_envelope.MaxX + raster_envelope.MinX) * 0.5;
		double lat = (raster_envelope.MaxY + raster_envelope.MinY) * 0.5;
		double elev = 1000.0;

		UpdateCamera(lon, lat, elev);

		AddBillboardLayer("Billboards", lon, lat, 0.0);

		return true;
	}

	void Run()
	{
		bool valid = true;

		while (valid)
		{
			DoPreDrawTask();

			valid = (sdk_context_->draw(window_) == Geoweb3d::GW3D_sOk);

			DoPostDrawTask(valid);
		}
	}

	void DoPreDrawTask()
	{

	}

	void DoPostDrawTask(bool draw_successful)
	{
		if (!line_segment_.expired())
		{
			if (line_segment_.lock()->get_Enabled())
			{
				Geoweb3d::IGW3DLineSegmentIntersectionReportCollection* intersection_report_collection = line_segment_.lock()->get_IntersectionReportCollection();

				Geoweb3d::IGW3DLineSegmentIntersectionReport* intersection_report;

				intersection_report_collection->reset();

				IconIntersectionTestStream streamer;
				bool found_one = false;
				while (intersection_report_collection->next(&intersection_report))
				{
					const char* vector_driver_name = intersection_report->get_VectorRepresentation().lock()->get_Driver().lock()->get_Name();
					const char* vector_layer_name = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer().lock()->get_Name();

					Geoweb3d::IGW3DVectorLayerWPtr icon_vector_layer = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer();

					Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection* details_collection = intersection_report->get_IntersectionDetailCollection();

					Geoweb3d::IGW3DLineSegmentIntersectionDetail* intersection_detail;

					details_collection->reset();

					//collect details for the intersected feature(s)
					while (details_collection->next(&intersection_detail))
					{
						streamer.insert(intersection_detail->get_ObjectID(), intersection_detail->get_IntersectionDistance(), image_texpalette_prop_index_);

						//just showing we can also access the actual intersection point
						const Geoweb3d::GW3DPoint* hit_point = intersection_detail->get_IntersectionPoint();
					}

					if (streamer.count())
					{
						// process the results
						icon_vector_layer.lock()->Stream(&streamer);


						//should dig in and get the closest but using first one in the set for now
						long hit_fid_ = streamer.get_AtIndex(0);
						if (hit_fid_ != current_fid_)
						{
							if (current_fid_ >= 0)
							{
								SetCurrentProps(properties_, false);
							}

							current_fid_ = hit_fid_;
							found_one = true;
							PopulateCurrentStateProps(current_fid_);
							SetCurrentProps(properties_, true);
						}
					}

				}
				if (!found_one)
				{
					if (current_fid_ >= 0)
					{
						printf("FID %d Unselected\n");
						SetCurrentProps(properties_, false);
					}
					current_fid_ = -1;
				}
			}

			//one shot
			line_segment_.lock()->put_Enabled(false);
		}

		if (props_dirty_)
		{
			if (current_fid_ >= 0)
			{
				SetCurrentProps(properties_, true);
			}
			else
			{
				SetDefaultProps(properties_);
				if (!current_rep_.expired())
					current_rep_.lock()->put_PropertyCollection(properties_);
			}
			props_dirty_ = false;
		}
	}

	//IGW3DVectorLayerStream overrides
	virtual bool OnStream(Geoweb3d::IGW3DVectorLayerStreamResult* result)
	{
		const Geoweb3d::IGW3DAttributeCollection* attribute_collection = result->get_AttributeCollection();
		const Geoweb3d::IGW3DPropertyCollection* representation_properties = result->get_VectorRepresentationProperties(current_rep_);
		Geoweb3d::IGW3DPropertyCollectionPtr feature_prop;

		const Geoweb3d::IGW3DVariant color_r = attribute_collection->get_Property(color_r_prop_index_);
		const Geoweb3d::IGW3DVariant color_g = attribute_collection->get_Property(color_g_prop_index_);
		const Geoweb3d::IGW3DVariant color_b = attribute_collection->get_Property(color_b_prop_index_);
		const Geoweb3d::IGW3DVariant color_a = attribute_collection->get_Property(color_a_prop_index_);

		const Geoweb3d::IGW3DVariant img_property_collection_id = attribute_collection->get_Property(image_texpalette_prop_index_);

		const Geoweb3d::IGW3DVariant label = attribute_collection->get_Property(label_index_);
		const Geoweb3d::IGW3DVariant label_anchor_pos = attribute_collection->get_Property(label_anchor_index_);

		//if( representation_properties )
		//{
		//	//const Geoweb3d::IGW3DDefinitionCollection* rep_def = representation_properties->get_DefinitionCollection();
		//	result->put_VectorRepresentationProperty( current_rep_, rep_img_collection_prop_index_, img_property_collection_id );
		//}
		//else
		//{
		//	result->put_VectorRepresentationProperty( current_rep_, rep_img_collection_prop_index_, img_property_collection_id );
		//}

		result->put_VectorRepresentationProperty(current_rep_, rep_img_collection_prop_index_, img_property_collection_id);

		result->put_VectorRepresentationProperty(current_rep_, rep_color_r_prop_index_, color_r);
		result->put_VectorRepresentationProperty(current_rep_, rep_color_g_prop_index_, color_g);
		result->put_VectorRepresentationProperty(current_rep_, rep_color_b_prop_index_, color_b);
		result->put_VectorRepresentationProperty(current_rep_, rep_color_a_prop_index_, color_a);

		result->put_VectorRepresentationProperty(current_rep_, rep_label_index_, label);
		result->put_VectorRepresentationProperty(current_rep_, rep_label_anchor_index_, label_anchor_pos);

		return true;
	}

	virtual bool OnError()
	{
		return false;
	}

	virtual unsigned long count() const
	{
		return 0;
	}

	virtual bool next(int64_t *ppVal)
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

	virtual int64_t get_AtIndex(unsigned long index)
	{
		return 0;
	}

	void UpdateCamera(double lon, double lat, double elev)
	{
		camera_lon_ = lon;
		camera_lat_ = lat;
		camera_elev_ = elev;

		double cam_lon = lon;
		double cam_lat = lat;
		const double cam_elev = elev;

		float pitch = 0.0f;

		if (camera_.expired())
		{
			camera_ = window_.lock()->get_CameraCollection()->create("Main Camera");
			navHelper_->add_Camera(camera_);
		}

		camera_.lock()->get_CameraController()->put_Location(cam_lon, cam_lat);
		camera_.lock()->get_CameraController()->put_Elevation(cam_elev, Geoweb3d::IGW3DPositionOrientation::Absolute);
		camera_.lock()->get_CameraController()->put_Rotation(0.0f, pitch, 0.0f);
	}

	Geoweb3d::IGW3DVectorLayerWPtr CreateLayer(const char* layer_name, double layer_lon, double layer_lat, double layer_z_comp, double r, double g, double b, double a)
	{
		Geoweb3d::IGW3DVectorLayerWPtr layer = data_source_.lock()->get_VectorLayerCollection()->create(layer_name, Geoweb3d::gtPOINT_25D, field_definition_);

		if (!layer.expired())
		{
			const std::size_t NUM_ROWS = number_images_length_;
			const std::size_t NUM_COLS = number_images_length_;
			const std::size_t NUM_FEATURES = NUM_ROWS * NUM_COLS;

			const double inter_feature_gap = 0.02;

			double lon = layer_lon - (NUM_ROWS * inter_feature_gap * 0.5);
			double lat = layer_lat - (NUM_COLS * inter_feature_gap * 0.5);
			double z_comp = layer_z_comp;

			const double start_lon = lon;
			const double start_lat = lat;
			const double start_z_comp = layer_z_comp;

			Geoweb3d::GW3DResult result;

			Geoweb3d::IGW3DPropertyCollectionPtr temp_collection = field_definition_->create_PropertyCollection();

			temp_collection->put_Property(color_r_prop_index_, r);
			temp_collection->put_Property(color_g_prop_index_, g);
			temp_collection->put_Property(color_b_prop_index_, b);
			temp_collection->put_Property(color_a_prop_index_, a);

			temp_collection->put_Property( icon_width_index_, default_state_.size_x_ );
			temp_collection->put_Property( icon_height_index_, default_state_.size_y_ );

			temp_collection->put_Property( label_anchor_index_, Geoweb3d::IGW3DImage::ANCHOR_POSITION::TOP_RIGHT );

			unsigned int image_index = 0;
			std::string label;

			for (unsigned int i = 0; i < NUM_FEATURES; ++i)
			{
				if ((i > 0))
				{
					if ((i % NUM_ROWS == 0))
					{
						lon = start_lon;
						z_comp = start_z_comp;
					}

					if ((i % NUM_COLS == 0))
					{
						lat += inter_feature_gap;
					}
				}

				image_index = i % number_images_length_;
				temp_collection->put_Property(image_texpalette_prop_index_, images_[image_index].unsafe_get()->get_PropertyCollectionID());

				label = "Feature: " + std::to_string( i );
				temp_collection->put_Property( label_index_, label.c_str() );

				Geoweb3d::GW3DPoint p(lon, lat, z_comp);
				layer.unsafe_get()->create_Feature(temp_collection, &p, result);
				 
				z_comp += 10.0;
				lon += inter_feature_gap;
			}
		}

		return layer;
	}

	void CreateElevationLayer()
	{
		if (raster_data_source_.expired())
		{
			Geoweb3d::GW3DResult res;

			//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
			//having to guess which data source driver is needed to open up a particular dataset.
			raster_data_source_ = sdk_context_->get_RasterDriverCollection()->auto_Open("../examples/media/NED 10-meter DC.tif", res);

			Geoweb3d::IGW3DRasterLayerCollection* layer_collection = raster_data_source_.lock()->get_RasterLayerCollection();
			raster_layer_ = layer_collection->get_AtIndex(0);
		}

		if (elevation_rep_driver_.expired())
		{
			elevation_rep_driver_ = sdk_context_->get_RasterLayerRepresentationDriverCollection()->get_Driver("Elevation");
		}

		if (!elevation_rep_driver_.expired())
		{
			Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
			params.page_level = 0;
			params.priority = 0;
			params.representation_layer_activity = true;
			Geoweb3d::IGW3DRasterRepresentationWPtr raster_rep = elevation_rep_driver_.lock()->get_RepresentationLayerCollection()->create(raster_layer_, params);
		}

	}

	void PopulateCurrentStateProps(const long current_fid)
	{
		const Geoweb3d::IGW3DPropertyCollection* current_props = current_rep_.lock()->get_PropertyCollection(current_fid_);
		/*struct prop_state
	{
		float texture_blend_;
		float b_red_;
		float b_green_;
		float b_blue_;
		float b_alpha_;
		float z_offset_;
		int z_offset_mmode_;
		bool auto_height_;
		float l_red_;
		float l_green_;
		float l_blue_;
		float l_alpha_;
		float size_x_;
		float size_y_;
		float leader_top_y_increase_percent_;
		int palette_;
	};*/
		if (current_props)
		{
			specific_state_.texture_blend_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND);
			specific_state_.b_red_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::RED);
			specific_state_.b_green_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::GREEN);
			specific_state_.b_blue_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::BLUE);
			specific_state_.b_alpha_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::ALPHA);
			specific_state_.z_offset_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::TRANSLATION_Z_OFFSET);
			specific_state_.z_offset_mmode_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::TRANSLATION_Z_OFFSET_MODE);
			specific_state_.auto_height_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::AUTOHEIGHT);
			specific_state_.l_red_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_RED);
			specific_state_.l_green_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_GREEN);
			specific_state_.l_blue_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_BLUE);
			specific_state_.l_alpha_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_ALPHA);
			specific_state_.size_x_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::SIZE_X);
			specific_state_.size_y_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::SIZE_Y);
			specific_state_.leader_top_y_increase_percent_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_TOP_Y_EXTEND_PERCENT);
			specific_state_.palette_ = current_props->get_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::TEXTURE_PALETTE_INDEX);
		}
		else
		{
			specific_state_.texture_blend_ = default_state_.texture_blend_;
			specific_state_.b_red_ = default_state_.b_red_;
			specific_state_.b_green_ = default_state_.b_green_;
			specific_state_.b_blue_ = default_state_.b_blue_;
			specific_state_.b_alpha_ = default_state_.b_alpha_;
			specific_state_.z_offset_ = default_state_.z_offset_;
			specific_state_.z_offset_mmode_ = default_state_.z_offset_mmode_;
			specific_state_.auto_height_ = default_state_.auto_height_;
			specific_state_.l_red_ = default_state_.l_red_;
			specific_state_.l_green_ = default_state_.l_green_;
			specific_state_.l_blue_ = default_state_.l_blue_;
			specific_state_.l_alpha_ = default_state_.l_alpha_;
			specific_state_.size_x_ = default_state_.size_x_;
			specific_state_.size_y_ = default_state_.size_y_;
			specific_state_.leader_top_y_increase_percent_ = default_state_.leader_top_y_increase_percent_;
			specific_state_.palette_ = default_state_.palette_;
		}
	}

	void SetCurrentProps(Geoweb3d::IGW3DPropertyCollectionPtr current, bool selected)
	{
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND, specific_state_.texture_blend_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::RED, specific_state_.b_red_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::GREEN, specific_state_.b_green_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::BLUE, specific_state_.b_blue_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::ALPHA, specific_state_.b_alpha_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::TRANSLATION_Z_OFFSET, specific_state_.z_offset_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::TRANSLATION_Z_OFFSET_MODE, specific_state_.z_offset_mmode_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_RED, specific_state_.l_red_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_GREEN, specific_state_.l_green_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_BLUE, specific_state_.l_blue_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_ALPHA, specific_state_.l_alpha_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::SIZE_X, specific_state_.size_x_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::SIZE_Y, specific_state_.size_y_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_TOP_Y_EXTEND_PERCENT, specific_state_.leader_top_y_increase_percent_);
		current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::TEXTURE_PALETTE_INDEX, specific_state_.palette_);
		if (selected)
		{
			current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::SECONDARY_TEXTURE_PALETTE_IDX, 6);
		}
		else
		{
			current->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::SECONDARY_TEXTURE_PALETTE_IDX, -1);
		}

		if (!current_rep_.expired())
			current_rep_.lock()->put_PropertyCollection(current_fid_, properties_);
	};

	void SetDefaultProps(Geoweb3d::IGW3DPropertyCollectionPtr defaults)
	{
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND, default_state_.texture_blend_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::RED, default_state_.b_red_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::GREEN, default_state_.b_green_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::BLUE, default_state_.b_blue_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::ALPHA, default_state_.b_alpha_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::TRANSLATION_Z_OFFSET, default_state_.z_offset_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::TRANSLATION_Z_OFFSET_MODE, default_state_.z_offset_mmode_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_RED, default_state_.l_red_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_GREEN, default_state_.l_green_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_BLUE, default_state_.l_blue_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_ALPHA, default_state_.l_alpha_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::SIZE_X, default_state_.size_x_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::SIZE_Y, default_state_.size_y_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::LEADER_TOP_Y_EXTEND_PERCENT, default_state_.leader_top_y_increase_percent_ );
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::TEXTURE_PALETTE_INDEX, default_state_.palette_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::IndividualProperties::SECONDARY_TEXTURE_PALETTE_IDX, -1);

		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::AUTOHEIGHT, default_state_.auto_height_);
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::LEADER_LINES_ENABLED, default_layer_props_.leaders_enabled );
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::DISABLE_DEPTH, default_layer_props_.disable_depth_test );
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_PER_PIXEL_EFFECT, default_layer_props_.per_pixel_mode );
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_PER_OBJECT_EFFECT, default_layer_props_.per_object_mode );
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_BILLBOARD_RENDER_MODE, default_layer_props_.occlusion_render_mode );
		defaults->put_Property(Geoweb3d::Vector::BillboardProperties::LayerDefaultsOnlyProperties::OCCLUDED_BILLBOARD_SHADING_BLEND_AMOUNT, default_layer_props_.shaded_blend_amount );
	};

	bool CreateBillboardRepresentation(Geoweb3d::IGW3DVectorLayerWPtr layer)
	{
		Geoweb3d::IGW3DVectorRepresentationDriverWPtr rep_billboard_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("BillBoard");

		if (rep_billboard_driver.expired())
		{
			std::cout << "Error creating vector representation driver." << std::endl;
			return false;
		}

		Geoweb3d::GW3DResult can_represent;
		can_represent = rep_billboard_driver.unsafe_get()->get_CapabilityToRepresent(layer);

		if (!Geoweb3d::Succeeded(can_represent))
		{
			std::cout << "Error: The selected representation driver cannot represent this layer." << std::endl;
			return false;
		}

		//create some default properties for the layer
		properties_ = rep_billboard_driver.lock()->get_PropertyCollection()->create_Clone();

		SetDefaultProps(properties_);

		Geoweb3d::Vector::RepresentationLayerCreationParameter rep_layer_creation_params;

		rep_layer_creation_params.page_level = 8;

		rep_layer_creation_params.finalization_token = image_collection_->create_FinalizeToken();

		rep_layer_creation_params.representation_default_parameters = properties_;

		current_rep_ = rep_billboard_driver.lock()->get_RepresentationLayerCollection()->create(layer, rep_layer_creation_params);

		current_rep_.lock()->get_ClampRadiusFilter()->put_Enabled(true);
		current_rep_.lock()->get_ClampRadiusFilter()->put_MaxRadius(30000.0);
		current_rep_.lock()->put_Font( billboard_font_ );

		const Geoweb3d::IGW3DPropertyCollection* label_props = current_rep_.lock()->get_LabelProperties();
		if ( label_props )
		{
			Geoweb3d::IGW3DPropertyCollectionPtr updated_label_props = label_props->create_Clone();
			updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::FOREGROUND_COLOR_RED, 1.0 );
			updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::FOREGROUND_COLOR_GREEN, 1.0 );
			updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::FOREGROUND_COLOR_BLUE, 0.0 );
			updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::FOREGROUND_COLOR_ALPHA, 1.0 );
			updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::BACKGROUND_COLOR_RED, 0.0 );
			updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::BACKGROUND_COLOR_GREEN, 0.0 );
			updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::BACKGROUND_COLOR_BLUE, 0.0 );
			updated_label_props->put_Property( Geoweb3d::Vector::LabelProperties::BACKGROUND_COLOR_ALPHA, 1.0 );
			current_rep_.lock()->put_LabelProperties( updated_label_props );
		}

		//representation property indices.
		rep_img_collection_prop_index_ = current_rep_.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("TEXTURE_PALETTE_INDEX");
		rep_color_r_prop_index_ = current_rep_.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("RED");
		rep_color_g_prop_index_ = current_rep_.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("GREEN");
		rep_color_b_prop_index_ = current_rep_.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("BLUE");
		rep_color_a_prop_index_ = current_rep_.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("ALPHA");
		rep_label_index_ = current_rep_.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("LABEL");
		rep_label_anchor_index_ = current_rep_.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("LABEL_ANCHOR_POS");

		return true;
	}

	void CreateFieldDefinition()
	{
		field_definition_ = Geoweb3d::IGW3DDefinitionCollection::create();

		color_r_prop_index_ = field_definition_->add_Property("COLOR_R", Geoweb3d::PROPERTY_DOUBLE, "");
		color_g_prop_index_ = field_definition_->add_Property("COLOR_G", Geoweb3d::PROPERTY_DOUBLE, "");
		color_b_prop_index_ = field_definition_->add_Property("COLOR_B", Geoweb3d::PROPERTY_DOUBLE, "");
		color_a_prop_index_ = field_definition_->add_Property("COLOR_A", Geoweb3d::PROPERTY_DOUBLE, "");

		field_definition_->add_Property("BLEND_FACTOR", Geoweb3d::PROPERTY_DOUBLE, "");

		icon_width_index_ = field_definition_->add_Property("ICON_WIDTH", Geoweb3d::PROPERTY_DOUBLE, "");
		icon_height_index_ = field_definition_->add_Property("ICON_HEIGHT", Geoweb3d::PROPERTY_DOUBLE, "");

		image_texpalette_prop_index_ = field_definition_->add_Property("TEXTURE_ARRAY_INDEX", Geoweb3d::PROPERTY_INT, "");
		label_index_ = field_definition_->add_Property("LABEL", Geoweb3d::PROPERTY_STR, "");
		label_anchor_index_ = field_definition_->add_Property("LABEL_ANCHOR_POS", Geoweb3d::PROPERTY_INT, "");
	}

	void CreateImages()
	{
		if (!image_collection_)
		{
			image_collection_ = sdk_context_->get_SceneGraphContext()->create_ImageCollection();
		}

		images_.push_back(image_collection_->create("../examples/media/test_images/one.png"));
		images_.push_back(image_collection_->create("../examples/media/test_images/two.png"));
		images_.push_back(image_collection_->create("../examples/media/test_images/three.png"));
		images_.push_back(image_collection_->create("../examples/media/test_images/four.png"));
		images_.push_back(image_collection_->create("../examples/media/test_images/five.png"));
		images_.push_back(image_collection_->create("../examples/media/test_images/six.png"));
		number_images_length_ = images_.size();
		images_.push_back(image_collection_->create("bin/data/sdk/icons/grabBackgroundIcon.png"));
		images_.push_back(image_collection_->create("bin/data/sdk/icons/selectedBackgroundIcon.png"));


		//int anchor = 0;
		//for ( Geoweb3d::IGW3DImageWPtr& image : images_ )
		//{
		//	image.lock()->put_AnchorPosition( static_cast<Geoweb3d::IGW3DImage::ANCHOR_POSITION>( anchor++ ) );
		//}
	}

public:

	Geoweb3d::IGW3DGeoweb3dSDKPtr			sdk_context_;
	Geoweb3d::IGW3DVectorDataSourceWPtr		data_source_;
	Geoweb3d::IGW3DRasterDataSourceWPtr		raster_data_source_;
	Geoweb3d::IGW3DRasterLayerWPtr			raster_layer_;
	Geoweb3d::IGW3DWindowWPtr				window_;
	Geoweb3d::IGW3DVectorLayerWPtr			layer_;
	Geoweb3d::IGW3DVectorLayerWPtr			elevation_layer_;

	Geoweb3d::IGW3DDefinitionCollectionPtr	field_definition_;
	Geoweb3d::IGW3DVectorRepresentationWPtr	current_rep_;
	Geoweb3d::IGW3DImageCollectionPtr		image_collection_;
	Geoweb3d::IGW3DCameraWPtr				camera_;

	Geoweb3d::IGW3DRasterRepresentationDriverWPtr elevation_rep_driver_;
	Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr line_segment_;

	int number_images_length_ = 0;
	std::vector< Geoweb3d::IGW3DImageWPtr >	images_;
	Geoweb3d::IGW3DImage::ANCHOR_POSITION anchor_;
	Geoweb3d::IGW3DFontPtr font_;
	Geoweb3d::IGW3DFontPtr billboard_font_;

	//		std::deque< Geoweb3d::GW3DSharedPtr< IconDrawEventTask > > frame_task_;

	double camera_lon_;
	double camera_lat_;
	double camera_elev_;

	int color_r_prop_index_;
	int color_g_prop_index_;
	int color_b_prop_index_;
	int color_a_prop_index_;

	int icon_width_index_;
	int icon_height_index_;

	int label_index_;
	int label_anchor_index_;

	int image_texpalette_prop_index_;
	int raster_layer_name_index_;

	int rep_img_collection_prop_index_;
	int rep_color_r_prop_index_;
	int rep_color_g_prop_index_;
	int rep_color_b_prop_index_;
	int rep_color_a_prop_index_;

	int rep_label_index_;
	int rep_label_anchor_index_;

	bool draw_task_active_;
	bool wire_frame_;

	NavigationHelper* navHelper_;

	struct prop_state
	{
		float texture_blend_;
		float b_red_;
		float b_green_;
		float b_blue_;
		float b_alpha_;
		float z_offset_;
		int z_offset_mmode_;
		bool auto_height_;
		float l_red_;
		float l_green_;
		float l_blue_;
		float l_alpha_;
		float size_x_;
		float size_y_;
		float leader_top_y_increase_percent_;
		int palette_;
	};

	struct layer_prop_state
	{
		int leaders_enabled = Geoweb3d::Vector::BillboardProperties::LEADER_LINE_ON_EXTENDED;
		bool disable_depth_test = false;
		int per_pixel_mode = 0;
		int per_object_mode = 0;
		int occlusion_render_mode = 1;
		double shaded_blend_amount = 1.0;
	};

	bool props_dirty_;
	layer_prop_state default_layer_props_;
	prop_state default_state_;
	prop_state specific_state_;
	long current_fid_;
	Geoweb3d::IGW3DPropertyCollectionPtr properties_;
};


//Application driver function.
void RunApplication(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context)
{
	App app(sdk_context);

	if (app.Initialize())
	{
		app.Run();
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
void my_fatal_function(const char *msg)
{
	std::cout << "Fatal Info: " << msg;
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function(const char* msg)
{
	std::cout << "General Info: " << msg;
}

/*! Information handling is not required */
void SetInformationHandling()
{
	/*! Tell the engine about our error function handling */
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Information, my_info_function);
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Warning, my_info_function);
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Fatal, my_fatal_function);
}

