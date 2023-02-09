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
		, property_mode_(0)
	{
		current_fid_ = -1;

		/*struct prop_state
		{
			Geoweb3d::Vector::ExtrudedPathProperties::RenderMode render_mode_;
			Geoweb3d::Vector::ExtrudedPathProperties::OITMode oit_mode_;
			Geoweb3d::Vector::OutlineMode outline_mode_;
			float line_width_;
			float curtain_color_bias_;
			float alpha_;
			float width_right_;
			float width_left_;
			float texture_blend_;
			float red_;
			float green_;
			float blue_;
			float draped_line_color_bias_;
			float curtain_alpha_;
			float draped_line_alpha_;
			Geoweb3d::Vector::ExtrudedPathProperties::LightingMode lighting_;
			float z_offset_;
			int z_offset_mode_
		};*/

		default_state_extruded_path_prop_ = {
			Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::EXTRUDED_PATH,
			Geoweb3d::Vector::OITMode::STIPPLE_MODE,
			Geoweb3d::Vector::OutlineMode::OUTLINE_ON,// outline_mode_
			4,// line_width_
			0.0f,// curtain_color_bias_
			1.0f,// alpha_
			5.0f,// width_right_
			5.0f,// width_left_
			1.0f,// texture_blend_
			1.0f,// red_
			0.0f,// green_
			0.0f,// blue_
			0.0f,// draped_line_color_bias_
			0.8f,// curtain_alpha_
			0.6f,// draped_line_alpha_
			Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::NONE_SHADED,
			20.0f,
			1
		};

		specific_state_extruded_path_prop_ = {
			Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::EXTRUDED_PATH,
			Geoweb3d::Vector::OITMode::STIPPLE_MODE,
			Geoweb3d::Vector::OutlineMode::OUTLINE_ON,// outline_mode_
			4,// line_width_
			0.0f,// curtain_color_bias_
			1.0f,// alpha_
			5.0f,// width_right_
			5.0f,// width_left_
			1.0f,// texture_blend_
			1.0f,// red_
			0.0f,// green_
			0.0f,// blue_
			0.0f,// draped_line_color_bias_
			0.8f,// curtain_alpha_
			0.6f,// draped_line_alpha_
			Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::NONE_SHADED,
			1.0f,
			1
		};

		default_state_extruded_path_dynamic_prop_ = {
			Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::EXTRUDED_PATH,
			Geoweb3d::Vector::OITMode::STIPPLE_MODE,
			Geoweb3d::Vector::OutlineMode::OUTLINE_ON,// outline_mode_
			4,// line_width_
			0.0f,// curtain_color_bias_
			1.0f,// alpha_
			5.0f,// width_right_
			5.0f,// width_left_
			0.0f,// texture_blend_
			0.0f,// red_
			0.0f,// green_
			1.0f,// blue_
			0.0f,// draped_line_color_bias_
			0.8f,// curtain_alpha_
			0.6f,// draped_line_alpha_
			Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::NONE_SHADED,
			20.0f,
			1
		};

		specific_state_extruded_path_dynamic_prop_ = {
			Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::EXTRUDED_PATH,
			Geoweb3d::Vector::OITMode::STIPPLE_MODE,
			Geoweb3d::Vector::OutlineMode::OUTLINE_ON,// outline_width_
			4,// line_width_
			0.0f,// curtain_color_bias_
			1.0f,// alpha_
			5.0f,// width_right_
			5.0f,// width_left_
			0.0f,// texture_blend_
			0.0f,// red_
			0.0f,// green_
			1.0f,// blue_
			0.0f,// draped_line_color_bias_
			0.8f,// curtain_alpha_
			0.6f,// draped_line_alpha_
			Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::NONE_SHADED,
			1.0f,
			1
		};
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


	void MiniEngine::PopulateCurrentExtrudedPathStateProps(Geoweb3d::IGW3DVectorRepresentationWPtr extruded_rep, const long current_fid)
	{
		const Geoweb3d::IGW3DPropertyCollection* current_props = extruded_rep.lock()->get_PropertyCollection(current_fid_);

		if (current_props)
		{
			specific_state_extruded_path_prop_.render_mode_ = (Geoweb3d::Vector::ExtrudedPathProperties::RenderMode)((int)current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::LayerDefaultsOnlyProperties::RENDER_MODE));
			specific_state_extruded_path_prop_.oit_mode_ = (Geoweb3d::Vector::OITMode)((int)current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::LayerDefaultsOnlyProperties::OIT));
			specific_state_extruded_path_prop_.outline_mode_ = ((Geoweb3d::Vector::OutlineMode)(int) current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::OUTLINE_MODE));
			specific_state_extruded_path_prop_.line_width_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LINE_WIDTH);
			specific_state_extruded_path_prop_.curtain_color_bias_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::CURTAIN_COLOR_BIAS);
			specific_state_extruded_path_prop_.alpha_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::ALPHA);
			specific_state_extruded_path_prop_.width_right_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::RIGHT_DISTANCE);
			specific_state_extruded_path_prop_.width_left_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LEFT_DISTANCE);
			specific_state_extruded_path_prop_.texture_blend_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND);
			specific_state_extruded_path_prop_.red_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::RED);
			specific_state_extruded_path_prop_.green_= current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::GREEN);
			specific_state_extruded_path_prop_.blue_= current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::BLUE);
			specific_state_extruded_path_prop_.lighting_ = (Geoweb3d::Vector::ExtrudedPathProperties::LightingMode)((int)current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::LayerDefaultsOnlyProperties::LIGHTING_MODE));
			specific_state_extruded_path_prop_.z_offset_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TRANSLATION_Z_OFFSET);
			specific_state_extruded_path_prop_.draped_line_color_bias_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::DRAPED_LINE_COLOR_BIAS);
			specific_state_extruded_path_prop_.curtain_alpha_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::CURTAIN_ALPHA);
			specific_state_extruded_path_prop_.draped_line_alpha_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::DRAPED_LINE_ALPHA);
			specific_state_extruded_path_prop_.z_offset_mode_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TRANSLATION_Z_OFFSET_MODE);
		}
		else
		{
			specific_state_extruded_path_prop_					= default_state_extruded_path_prop_;		
		}
	}

	void MiniEngine::PopulateCurrentExtrudedPathDynamicStateProps(Geoweb3d::IGW3DVectorRepresentationWPtr extruded_rep, const long current_fid)
	{
		const Geoweb3d::IGW3DPropertyCollection* current_props = extruded_rep.lock()->get_PropertyCollection(current_fid_);

		if (current_props)
		{
			specific_state_extruded_path_dynamic_prop_.render_mode_ = (Geoweb3d::Vector::ExtrudedPathProperties::RenderMode)((int)current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::LayerDefaultsOnlyProperties::RENDER_MODE));
			specific_state_extruded_path_dynamic_prop_.oit_mode_ = (Geoweb3d::Vector::OITMode)((int)current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::LayerDefaultsOnlyProperties::OIT));
			specific_state_extruded_path_dynamic_prop_.outline_mode_ = ((Geoweb3d::Vector::OutlineMode)(int)current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::OUTLINE_MODE));
			specific_state_extruded_path_dynamic_prop_.line_width_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LINE_WIDTH);
			specific_state_extruded_path_dynamic_prop_.curtain_color_bias_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::CURTAIN_COLOR_BIAS);
			specific_state_extruded_path_dynamic_prop_.alpha_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::ALPHA);
			specific_state_extruded_path_dynamic_prop_.width_right_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::RIGHT_DISTANCE);
			specific_state_extruded_path_dynamic_prop_.width_left_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LEFT_DISTANCE);
			specific_state_extruded_path_dynamic_prop_.texture_blend_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND);
			specific_state_extruded_path_dynamic_prop_.red_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::RED);
			specific_state_extruded_path_dynamic_prop_.green_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::GREEN);
			specific_state_extruded_path_dynamic_prop_.blue_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::BLUE);
			specific_state_extruded_path_dynamic_prop_.lighting_ = (Geoweb3d::Vector::ExtrudedPathProperties::LightingMode)((int)current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::LayerDefaultsOnlyProperties::LIGHTING_MODE));
			specific_state_extruded_path_dynamic_prop_.z_offset_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TRANSLATION_Z_OFFSET);
			specific_state_extruded_path_dynamic_prop_.draped_line_color_bias_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::DRAPED_LINE_COLOR_BIAS);
			specific_state_extruded_path_dynamic_prop_.curtain_alpha_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::CURTAIN_ALPHA);
			specific_state_extruded_path_dynamic_prop_.draped_line_alpha_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::DRAPED_LINE_ALPHA);
			specific_state_extruded_path_dynamic_prop_.z_offset_mode_ = current_props->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::TRANSLATION_Z_OFFSET_MODE);
		}
		else
		{
			specific_state_extruded_path_dynamic_prop_					= default_state_extruded_path_dynamic_prop_;
		}
	}
	void MiniEngine::SetCurrentProps(Geoweb3d::IGW3DPropertyCollectionPtr current)
	{
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LEFT_DISTANCE, specific_state_extruded_path_prop_.width_left_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::RIGHT_DISTANCE, specific_state_extruded_path_prop_.width_right_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::ALPHA, specific_state_extruded_path_prop_.alpha_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TEXTURE_MODE_ENABLE, true);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND, specific_state_extruded_path_prop_.texture_blend_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::RED, specific_state_extruded_path_prop_.red_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::GREEN, specific_state_extruded_path_prop_.green_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::BLUE, specific_state_extruded_path_prop_.blue_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TRANSLATION_Z_OFFSET, specific_state_extruded_path_prop_.z_offset_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::CURTAIN_COLOR_BIAS, specific_state_extruded_path_prop_.curtain_color_bias_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::DRAPED_LINE_COLOR_BIAS, specific_state_extruded_path_prop_.draped_line_color_bias_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::CURTAIN_ALPHA, specific_state_extruded_path_prop_.curtain_alpha_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::DRAPED_LINE_ALPHA, specific_state_extruded_path_prop_.draped_line_alpha_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TRANSLATION_Z_OFFSET_MODE, specific_state_extruded_path_prop_.z_offset_mode_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LINE_WIDTH, specific_state_extruded_path_prop_.line_width_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::OUTLINE_MODE, specific_state_extruded_path_prop_.outline_mode_);

	};

	void MiniEngine::SetCurrentPropsDynamic(Geoweb3d::IGW3DPropertyCollectionPtr current)
	{
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LEFT_DISTANCE, specific_state_extruded_path_dynamic_prop_.width_left_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::RIGHT_DISTANCE, specific_state_extruded_path_dynamic_prop_.width_right_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::ALPHA, specific_state_extruded_path_dynamic_prop_.alpha_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TEXTURE_MODE_ENABLE, true);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND, specific_state_extruded_path_dynamic_prop_.texture_blend_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::RED, specific_state_extruded_path_dynamic_prop_.red_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::GREEN, specific_state_extruded_path_dynamic_prop_.green_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::BLUE, specific_state_extruded_path_dynamic_prop_.blue_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TRANSLATION_Z_OFFSET, specific_state_extruded_path_dynamic_prop_.z_offset_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::CURTAIN_COLOR_BIAS, specific_state_extruded_path_dynamic_prop_.curtain_color_bias_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::DRAPED_LINE_COLOR_BIAS, specific_state_extruded_path_dynamic_prop_.draped_line_color_bias_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::CURTAIN_ALPHA, specific_state_extruded_path_dynamic_prop_.curtain_alpha_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::DRAPED_LINE_ALPHA, specific_state_extruded_path_dynamic_prop_.draped_line_alpha_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TRANSLATION_Z_OFFSET_MODE, specific_state_extruded_path_dynamic_prop_.z_offset_mode_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LINE_WIDTH, specific_state_extruded_path_dynamic_prop_.line_width_);
		current->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::OUTLINE_MODE, specific_state_extruded_path_dynamic_prop_.outline_mode_);
	};

	void MiniEngine::SetDefaultProps(Geoweb3d::IGW3DPropertyCollectionPtr defaults)
	{
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::CURTAIN_COLOR_BIAS, default_state_extruded_path_prop_.curtain_color_bias_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LINE_WIDTH, default_state_extruded_path_prop_.line_width_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::OUTLINE_MODE, default_state_extruded_path_prop_.outline_mode_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::LayerDefaultsOnlyProperties::RENDER_MODE, default_state_extruded_path_prop_.render_mode_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::LayerDefaultsOnlyProperties::OIT, default_state_extruded_path_prop_.oit_mode_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::LayerDefaultsOnlyProperties::LIGHTING_MODE, default_state_extruded_path_prop_.lighting_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LEFT_DISTANCE, default_state_extruded_path_prop_.width_left_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::RIGHT_DISTANCE, default_state_extruded_path_prop_.width_right_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::ALPHA, default_state_extruded_path_prop_.alpha_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TEXTURE_MODE_ENABLE, true);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND, default_state_extruded_path_prop_.texture_blend_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::RED, default_state_extruded_path_prop_.red_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::GREEN, default_state_extruded_path_prop_.green_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::BLUE, default_state_extruded_path_prop_.blue_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TRANSLATION_Z_OFFSET, default_state_extruded_path_prop_.z_offset_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::ConstructionProperties::SEGMENTATION_DISTANCE, 50.0);
		//Note: DESIRED_RESOLUTION uses opengl tessellation shader. In order to achieve the desired resolution the representation may need to increase the value
		//		set via SEGMENTATION_DISTANCE in order to reach the final value (as opengl only allows 64 splits).
		//		Also note - Use of SEGMENTATION_DISTANCE can allow more memory to be used - in trade for - less GPU calculations.
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::ConstructionProperties::DESIRED_RESOLUTION, 5.0);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::TEXTURE_PALETTE_INDEX, texture_propertycollection_id);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::DRAPED_LINE_COLOR_BIAS, default_state_extruded_path_prop_.draped_line_color_bias_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::CURTAIN_ALPHA, default_state_extruded_path_prop_.curtain_alpha_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::DRAPED_LINE_ALPHA, default_state_extruded_path_prop_.draped_line_alpha_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::TRANSLATION_Z_OFFSET_MODE, default_state_extruded_path_prop_.z_offset_mode_);
	};

	void MiniEngine::SetDefaultPropsDynamic(Geoweb3d::IGW3DPropertyCollectionPtr defaults)
	{
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::CURTAIN_COLOR_BIAS, default_state_extruded_path_dynamic_prop_.curtain_color_bias_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LINE_WIDTH, default_state_extruded_path_dynamic_prop_.line_width_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::OUTLINE_MODE, default_state_extruded_path_dynamic_prop_.outline_mode_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::LayerDefaultsOnlyProperties::RENDER_MODE, default_state_extruded_path_dynamic_prop_.render_mode_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::LayerDefaultsOnlyProperties::OIT, default_state_extruded_path_dynamic_prop_.oit_mode_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::LayerDefaultsOnlyProperties::LIGHTING_MODE, default_state_extruded_path_dynamic_prop_.lighting_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LEFT_DISTANCE, default_state_extruded_path_dynamic_prop_.width_left_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::RIGHT_DISTANCE, default_state_extruded_path_dynamic_prop_.width_right_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::ALPHA, default_state_extruded_path_dynamic_prop_.alpha_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TEXTURE_MODE_ENABLE, true);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TEXTURE_TO_COLOR_BLEND, default_state_extruded_path_dynamic_prop_.texture_blend_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::RED, default_state_extruded_path_dynamic_prop_.red_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::GREEN, default_state_extruded_path_dynamic_prop_.green_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::BLUE, default_state_extruded_path_dynamic_prop_.blue_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::TRANSLATION_Z_OFFSET, default_state_extruded_path_dynamic_prop_.z_offset_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::ConstructionProperties::SEGMENTATION_DISTANCE, 50.0);
		//Note: DESIRED_RESOLUTION uses opengl tessellation shader. In order to achieve the desired resolution the representation may need to increase the value
		//		set via SEGMENTATION_HAVERSINE in order to reach the final value (as opengl only allow 64 splits).
		//		Also note - Use of SEGMENTATION_DISTANCE can allow more memory to be used - in trade for - less GPU calculations.
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::ConstructionProperties::DESIRED_RESOLUTION, 5.0);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::TEXTURE_PALETTE_INDEX, texture_propertycollection_id);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::DRAPED_LINE_COLOR_BIAS, default_state_extruded_path_dynamic_prop_.draped_line_color_bias_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::CURTAIN_ALPHA, default_state_extruded_path_dynamic_prop_.curtain_alpha_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::DRAPED_LINE_ALPHA, default_state_extruded_path_dynamic_prop_.draped_line_alpha_);
		defaults->put_Property(Geoweb3d::Vector::ExtrudedPathProperties::TRANSLATION_Z_OFFSET_MODE, default_state_extruded_path_dynamic_prop_.z_offset_mode_);
	};
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
		LoadDemoImageryDataset(sdk_context_);

		Geoweb3d::GW3DResult res;
		//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
		//having to guess which data source driver is needed to open up a particular dataset.
		Geoweb3d::IGW3DVectorDataSourceWPtr data_source = sdk_context_->get_VectorDriverCollection()->auto_Open( "../examples/media/DC_LINES.shp", res );

		if (data_source.expired())
		{
			std::cout << "Could not load the following Vector Data Source\n";
		}

		Geoweb3d::IGW3DVectorLayerCollection *vlyrcollection = data_source.lock()->get_VectorLayerCollection();
		vlyrcollection->reset();

		Geoweb3d::IGW3DVectorLayerWPtr layer;
		Geoweb3d::GW3DEnvelope env;

		//Order is not preserved.
		Geoweb3d::IGW3DImageCollectionPtr imagepalette = sdk_context_->get_SceneGraphContext()->create_ImageCollection();

		{
			//If you add a duplicate, it will return the same IGW3DImage
			Geoweb3d::IGW3DImageWPtr image = imagepalette->create("../examples/media/Texture/Roads/road_texture.jpg");

			texture_propertycollection_id = image.lock()->get_PropertyCollectionID();
		}

		Geoweb3d::IGW3DImageWPtr imagetest;
		imagepalette->reset();
		while (imagepalette->next(&imagetest))
		{
			printf("file: [%s], userdata: [%i]\n", imagetest.lock()->get_ImageName(), static_cast<int>( reinterpret_cast<std::int64_t>(imagetest.lock()->get_UserData() ) ) );
		}

		Geoweb3d::IGW3DFinalizationTokenPtr reptoken = imagepalette->create_FinalizeToken();

		Geoweb3d::IGW3DVectorRepresentationDriverWPtr extruded_path_driver;

		while (vlyrcollection->next(&layer))
		{
			extruded_path_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("ExtrudedPath");
			extruded_path_properties_ = extruded_path_driver.lock()->get_PropertyCollection()->create_Clone();
			SetDefaultProps(extruded_path_properties_);

			Geoweb3d::GW3DResult validitychk;

			if (!Geoweb3d::Succeeded(validitychk = extruded_path_driver.lock()->get_CapabilityToRepresent(layer)))
			{
				printf("not able to mix this geometry type with the rep..\n");
			}
			else
			{
				Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
				params.page_level = 12;
				params.representation_default_parameters = extruded_path_properties_;
				extruded_path_rep_ = extruded_path_driver.lock()->get_RepresentationLayerCollection()->create(layer, params);

			}

			env.merge(layer.lock()->get_Envelope());
		}


		Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context_->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");

		if (!driver.expired())
		{
			Geoweb3d::IGW3DVectorDataSourceCollection* datasource_collection = driver.lock()->get_VectorDataSourceCollection();
			std::string data_source_name = "Dynamic_Path";
			dynamic_path_datasource_ = datasource_collection->create(data_source_name.c_str());
		}
		else
		{
			printf("SDK Bug Detected\n");
		}

		Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());
		Geoweb3d::IGW3DPropertyCollectionPtr field_values(field_definition->create_PropertyCollection());

		dynamic_path_layer_ = dynamic_path_datasource_.lock()->get_VectorLayerCollection()->create("Path_Dynamic", Geoweb3d::gtLINE_STRING, field_definition);
		if (!dynamic_path_layer_.expired())
		{
			Geoweb3d::GW3DLineString line_string;

			int pt_index = 0;
			line_string.put_Point(pt_index++, -77.04561, 38.88038, 0);
			line_string.put_Point(pt_index++, -77.03797, 38.88581, 0);
			line_string.put_Point(pt_index++, -77.04525, 38.89192, 0);
			line_string.put_Point(pt_index++, -77.04040, 38.89615, 0);
			Geoweb3d::GW3DResult result;

			long feature_id = dynamic_path_layer_.lock()->create_Feature(field_values, &line_string, result);

			if (!Geoweb3d::Succeeded(result))
			{
				printf("create_FeatureWithinMemoryFile Error\n");
			}

			dynamic_path_layer_.lock()->put_GeometryEditableMode(true);

			Geoweb3d::GW3DResult validitychk;

			if (!Geoweb3d::Succeeded(validitychk = extruded_path_driver.lock()->get_CapabilityToRepresent(dynamic_path_layer_)))
			{
				printf("not able to mix this geometry type with the rep..\n");
			}
			else
			{
				extruded_path_properties2_ = extruded_path_driver.lock()->get_PropertyCollection()->create_Clone();
				SetDefaultPropsDynamic(extruded_path_properties2_);
				Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
				params.page_level = 12;
					
				params.representation_default_parameters = extruded_path_properties2_;
				extruded_path_dynamic_rep_ = extruded_path_driver.lock()->get_RepresentationLayerCollection()->create(dynamic_path_layer_, params);

				dynamic_path_stream_.SetRepresentation(extruded_path_dynamic_rep_);
				dynamic_path_stream_.SetLayer(dynamic_path_layer_);
			}
		}
		
		line_segment_ = sdk_context_->get_LineSegmentIntersectionTestCollection()->create();

		if (!extruded_path_rep_.expired())
		{
			extruded_path_rep_.lock()->put_GW3DFinalizationToken(reptoken);

			if (!line_segment_.expired())
			{
				line_segment_.lock()->get_VectorRepresentationCollection()->add(extruded_path_rep_);
			}
		}

		if (!extruded_path_dynamic_rep_.expired())
		{
			extruded_path_dynamic_rep_.lock()->put_GW3DFinalizationToken(reptoken);

			if (!line_segment_.expired())
			{
				line_segment_.lock()->get_VectorRepresentationCollection()->add(extruded_path_dynamic_rep_);
			}
		}

		if (CreateElevationRep(sdk_context_, "../examples/media/NED 10-meter DC.tif", 6, 0, true, 6.0).expired())
		{
			printf("CreateElevationRep Error\n");
		}


		camera_.lock()->get_CameraController()->put_NavigationRestrictionMode(Geoweb3d::IGW3DPositionOrientation::NavigationRestrictionMode::ABOVE_GROUND_ONLY);
		double longitude, latitude;
		longitude = (env.MinX + env.MaxX) * 0.5;
		latitude = (env.MinY + env.MaxY) * 0.5;
		camera_controller_->put_Location(longitude, latitude);
		camera_controller_->put_Elevation(3000);
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
		const Geoweb3d::GW3DPoint *mouse_geo = 0;
		if (navHelper_->getMousePixelLocation(&mouse_geo))
		{
			dynamic_path_stream_.SetLocation(mouse_geo->get_X(), mouse_geo->get_Y());

			if (!dynamic_path_layer_.expired())
			{
				dynamic_path_layer_.lock()->Stream(&dynamic_path_stream_);
			}
		}
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

						if (rep == extruded_path_rep_)
						{
							current_fid_ = streamer.get_AtIndex(0);
							found_one = true;
							property_mode_ = 0;
							PopulateCurrentExtrudedPathStateProps(rep, current_fid_);
							dynamic_path_stream_.SetWorkingFID(-1);

						}
						else if (rep == extruded_path_dynamic_rep_)
						{
							current_fid_ = streamer.get_AtIndex(0);
							found_one = true;
							property_mode_ = 1;
							PopulateCurrentExtrudedPathDynamicStateProps(rep, current_fid_);
							dynamic_path_stream_.SetWorkingFID(current_fid_);
						}
					}
				}

				if (!found_one)
				{
					//unset any previous fid 
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
				if (property_mode_ == 0)
				{
					SetCurrentProps(extruded_path_properties_);
					if (!extruded_path_rep_.expired())
					{
						extruded_path_rep_.lock()->put_PropertyCollection(current_fid_, extruded_path_properties_);
					}
				}
				else
				{
					SetCurrentPropsDynamic(extruded_path_properties2_);
					if (!extruded_path_dynamic_rep_.expired())
					{
						extruded_path_dynamic_rep_.lock()->put_PropertyCollection(current_fid_, extruded_path_properties2_);
					}
				}
			}
			else
			{
				if (property_mode_ == 0)
				{
					SetDefaultProps(extruded_path_properties_);
					if (!extruded_path_rep_.expired())
					{
						extruded_path_rep_.lock()->put_PropertyCollection(extruded_path_properties_);
					}
				}
				else
				{
					SetDefaultPropsDynamic(extruded_path_properties2_);
					if (!extruded_path_dynamic_rep_.expired())
					{
						extruded_path_dynamic_rep_.lock()->put_PropertyCollection(extruded_path_properties2_);
					}
				}
			}

			props_dirty_ = false;
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
						if (drag_mode_readied_)
						{
							if (dynamic_path_stream_.GetWorkingFID() < 0)
							{
								printf("You need to click on a modifiable feature in order to drag.");
							}
							else
							{
								dynamic_path_stream_.SetDragMode(true);
							}

						}
						else if (working_new_feature)
						{
							dynamic_path_stream_.CaptureAScribPoint();
							//once delete feature is ironed out - we can show the line getting built
							//dynamic_path_stream_.CreateScribedLine(extruded_path_properties2_);
						}

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

			case win_event.MouseButtonReleased:
			{
				dynamic_path_stream_.SetDragMode(false);
			}
			break;

			case win_event.KeyPressed:
			{
				bool global_prop_state = false;

				prop_state* state;
				if (current_fid_ >= 0)
				{
					if (property_mode_ == 0)
					{
						state = &specific_state_extruded_path_prop_;
					}
					else
					{
						state = &specific_state_extruded_path_dynamic_prop_;
					}
					
				}
				else
				{
					if (property_mode_ == 0)
					{
						state = &default_state_extruded_path_prop_;
					}
					else
					{
						state = &default_state_extruded_path_dynamic_prop_;
					}

					global_prop_state = true;
				}

				switch (win_event.Key.code)
				{
				case Geoweb3d::Key::W:
					{
					if (!global_prop_state)
						{
						printf("RENDER MODE (must unselect a specific feature as this is a global property \n");
						}
					else
					{
						bool extruded_path_on = state->render_mode_ & Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::EXTRUDED_PATH;
						if (extruded_path_on)
						{
							state->render_mode_ = (Geoweb3d::Vector::ExtrudedPathProperties::RenderMode)(state->render_mode_ & ~Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::EXTRUDED_PATH);
							printf("EXTRUDED_PATH OFF\t");
						}
						else
						{
							state->render_mode_ = (Geoweb3d::Vector::ExtrudedPathProperties::RenderMode)(state->render_mode_ | Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::EXTRUDED_PATH);
							printf("EXTRUDED_PATH ON\t");
						}
						props_dirty_ = true;
					}
					filter_out = true;
						}
				break;
				case Geoweb3d::Key::E:
					{
					if (!global_prop_state)
					{
						printf("RENDER MODE (must unselect a specific feature as this is a global property \n");
					}
					else
					{
						bool curtain_on = state->render_mode_ & Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::CURTAIN;
						if (curtain_on)
						{
							state->render_mode_ = (Geoweb3d::Vector::ExtrudedPathProperties::RenderMode)(state->render_mode_ & ~Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::CURTAIN);
							printf("CURTAIN OFF\t");
						}
						else
						{
							state->render_mode_ = (Geoweb3d::Vector::ExtrudedPathProperties::RenderMode)(state->render_mode_ | Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::CURTAIN);
							printf("CURTAIN ON\t");
						}
						props_dirty_ = true;
						}
					filter_out = true;
				}
				break;
				case Geoweb3d::Key::R:
				{
					if (!global_prop_state)
						{
						printf("RENDER MODE (must unselect a specific feature as this is a global property \n");
						}
					else
					{
						bool draped_line_on = state->render_mode_ & Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::DRAPED_LINE;
						if (draped_line_on)
						{
							state->render_mode_ = (Geoweb3d::Vector::ExtrudedPathProperties::RenderMode)(state->render_mode_ & ~Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::DRAPED_LINE);
							printf("DRAPED_LINE OFF\t");
						}
						else
						{
							state->render_mode_ = (Geoweb3d::Vector::ExtrudedPathProperties::RenderMode)(state->render_mode_ | Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::DRAPED_LINE);
							printf("DRAPED_LINE ON\t");
						}

						props_dirty_ = true;
					}
					filter_out = true;
				}
				break;
				case Geoweb3d::Key::T:
				{
					if (!global_prop_state)
					{
						printf("RENDER MODE (must unselect a specific feature as this is a global property \n");
						}
						else
						{
						bool line_on = state->render_mode_ & Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::LINE;
						if (line_on)
						{
							state->render_mode_ = (Geoweb3d::Vector::ExtrudedPathProperties::RenderMode)(state->render_mode_ & ~Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::LINE);
							printf("LINE OFF\t");
						}
						else
						{
							state->render_mode_ = (Geoweb3d::Vector::ExtrudedPathProperties::RenderMode)(state->render_mode_ | Geoweb3d::Vector::ExtrudedPathProperties::RenderMode::LINE);
							printf("LINE ON\t");
						}
						props_dirty_ = true;
					}
					filter_out = true;
				}
				break;
		
				case Geoweb3d::Key::Y:
				{
					if (state->z_offset_mode_ == 1)
					{
						state->z_offset_mode_ = 0;
						printf("Absolute mode on\t");
					}
					else
					{
						state->z_offset_mode_ = 1;
						printf("Relative mode on\t");
					}

					props_dirty_ = true;
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::P:
				{
					drag_mode_readied_ = !drag_mode_readied_;
					if (!drag_mode_readied_)
					{
						dynamic_path_stream_.SetDragMode(false);
					}

					if (drag_mode_readied_)
					{
						printf("Enable Point Dragging\n");
						working_new_feature = false;
					}
					else
					{
						printf("Disable Point Dragging\n");
					}
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::O:
				{
					if (working_new_feature)
					{
						//dynamic_path_stream_.StopScribingLine();
							dynamic_path_stream_.CreateScribedLine(extruded_path_properties2_);
					}

					working_new_feature = !working_new_feature;
					/*if (!working_new_feature)
					{
						dynamic_path_stream_.SetWorkingOnANewFeatureMode(working_new_feature);
					}*/

					if (working_new_feature)
					{
						printf("Enable working on a new feature\n");
						drag_mode_readied_ = false;
					}
					else
					{
						printf("Disable working on a new feature\n");
					}
					filter_out = true;
				}
				break;
				/*case Geoweb3d::Key::I:
				{
					dynamic_path_stream_.DeleteFeature();
				}
				break;*/
				case Geoweb3d::Key::Q:
				{
					if (!global_prop_state)
					{
						printf("OITModeE (must unselect a specific feature as this is a global property \n");
					}
					else
					{
						state->oit_mode_ = (Geoweb3d::Vector::OITMode)(state->oit_mode_ + 1);
						if (state->oit_mode_ > Geoweb3d::Vector::OITMode::STIPPLE_MODE)
						{
							state->oit_mode_ = Geoweb3d::Vector::OITMode::ALPHA_MODE;
						}

						if (state->oit_mode_ == Geoweb3d::Vector::OITMode::STIPPLE_MODE)
						{
							printf("OIT: STIPPLE MODE\n");
						}
						else
						{
							printf("OIT: ALPHA MODE\n");
						}
						props_dirty_ = true;
						}
						filter_out = true;
				}
				break;

				case Geoweb3d::Key::A:
				{
					if (win_event.Key.shift)
					{
						state->line_width_ -= 1;
						if (state->line_width_ < 0)
							state->line_width_ = 0;
					}
					else
					{
						state->line_width_ += 1;
						if (state->line_width_ > 32)
							state->line_width_ = 32;
					}
					printf("LINE_WIDTH: %d\n", state->line_width_);
					props_dirty_ = true;
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::S:
				{
					if (state->outline_mode_ == Geoweb3d::Vector::OutlineMode::OUTLINE_ON)
					{
						state->outline_mode_ = Geoweb3d::Vector::OutlineMode::OUTLINE_OFF;
						printf("OUTLINE_OFF\n");
					}
					else
					{
						state->outline_mode_ = Geoweb3d::Vector::OutlineMode::OUTLINE_ON;
						printf("OUTLINE_ON\n");
					}

					props_dirty_ = true;
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::V:
				{
					if (win_event.Key.shift)
					{
						state->curtain_alpha_ -= 0.1f;
						if (state->curtain_alpha_ < 0.0f)
							state->curtain_alpha_ = 0.0f;
					}
					else
					{
						state->curtain_alpha_ += 0.1f;
						if (state->curtain_alpha_ > 1.0f)
							state->curtain_alpha_ = 1.0f;
					}
					printf("CURTAIN_ALPHA: %1.1f\n", state->curtain_alpha_);
					props_dirty_ = true;
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::B:
				{
					if (win_event.Key.shift)
					{
						state->curtain_color_bias_ -= 0.1f;
						if (state->curtain_color_bias_ < -1.0f)
							state->curtain_color_bias_ = -1.0f;
					}
					else
					{
						state->curtain_color_bias_ += 0.1f;
						if (state->curtain_color_bias_ > 1.0f)
							state->curtain_color_bias_ = 1.0f;
					}
					printf("CURTAIN_COLOR_BIAS: %1.1f\n", state->curtain_color_bias_);
					props_dirty_ = true;
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::N:
				{
					if (win_event.Key.shift)
					{
						state->draped_line_alpha_ -= 0.1f;
						if (state->draped_line_alpha_ < 0.0f)
							state->draped_line_alpha_ = 0.0f;
					}
					else
					{
						state->draped_line_alpha_ += 0.1f;
						if (state->draped_line_alpha_ > 1.0f)
							state->draped_line_alpha_ = 1.0f;
					}
					printf("DRAPED_LINE_ALPHA: %1.1f\n", state->draped_line_alpha_);
					props_dirty_ = true;
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::M:
				{
					if (win_event.Key.shift)
					{
						state->draped_line_color_bias_ -= 0.1f;
						if (state->draped_line_color_bias_ < -1.0f)
							state->draped_line_color_bias_ = -1.0f;
					}
					else
					{
						state->draped_line_color_bias_ += 0.1f;
						if (state->draped_line_color_bias_ > 1.0f)
							state->draped_line_color_bias_ = 1.0f;
					}
					printf("DRAPED_LINE_COLOR_BIAS: %1.1f\n", state->draped_line_color_bias_);
					props_dirty_ = true;
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::Z:
				{
					if (win_event.Key.shift)
					{
						state->alpha_ -= 0.1f;
						if (state->alpha_ < 0.0f)
							state->alpha_ = 0.0f;
					}
					else
					{
						state->alpha_ += 0.1f;
						if (state->alpha_ > 1.0f)
							state->alpha_ = 1.0f;
					}
					printf("ALPHA: %1.1f\n", state->alpha_);
					props_dirty_ = true;
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::D:
				{
					if (win_event.Key.shift)
					{
						state->width_right_ -= 1.0f;
						if (state->width_right_ < 0.0f)
							state->width_right_ = 0.0f;
					}
					else
					{
						state->width_right_ += 1.0f;
						if (state->width_right_ > 100.0f)
							state->width_right_ = 100.0f;
					}
					printf("WIDTH RIGHT: %3.1f\n", state->width_right_);
					props_dirty_ = true;
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::F:
				{
					if (win_event.Key.shift)
					{
						state->width_left_ -= 1.0f;
						if (state->width_left_ < 0.0f)
							state->width_left_ = 0.0f;
					}
					else
					{
						state->width_left_ += 1.0f;
						if (state->width_left_ > 100.0f)
							state->width_left_ = 100.0f;
					}
					printf("WIDTH LEFT: %3.1f\n", state->width_left_);
					props_dirty_ = true;
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::C:
				{
					if (win_event.Key.shift)
					{
						state->texture_blend_ -= 0.1f;
						if (state->texture_blend_ < 0.0f)
							state->texture_blend_ = 0.0f;
					}
					else
					{
						state->texture_blend_ += 0.1f;
						if (state->texture_blend_ > 1.0f)
							state->texture_blend_ = 1.0f;
					}
					printf("TEXTURE_TO_COLOR_BLEND: %1.1f\n", state->texture_blend_);
					props_dirty_ = true;
					filter_out = true;
				}
				break;

				

				case Geoweb3d::Key::X:
				{
					if (win_event.Key.shift)
					{
						state->green_ -= 0.1f;
						if (state->green_ < 0.0f)
							state->green_ = 0.0f;
					}
					else
					{
						state->green_ += 0.1f;
						if (state->green_ > 1.0f)
							state->green_ = 1.0f;
					}
					printf("GREEN COMPONENT: %1.1f\n", state->green_);
					props_dirty_ = true;
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::G:
				{
					if (win_event.Key.shift)
					{
						state->z_offset_ -= 1.0f;
						if (state->z_offset_ < -200.0f)
							state->z_offset_ = -200.0f;
					}
					else
					{
						state->z_offset_ += 1.0f;
						if (state->green_ > 1000.0f)
							state->green_ = 1000.0f;
					}
					printf("Z OFFSET: %1.1f\n", state->z_offset_);
					props_dirty_ = true;
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::J:
				{
					if (!global_prop_state)
					{
						printf("LIGHTING_MODE (must unselect a specific feature as this is a global property \n");
					}
					else
					{
						if (state->lighting_ & Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::EXTRUDED_PATH_SHADED)
						{
							state->lighting_ = (Geoweb3d::Vector::ExtrudedPathProperties::LightingMode)(state->lighting_ & ~Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::EXTRUDED_PATH_SHADED);
							printf("LIGHTING: EXTRUDED_PATH_SHADED OFF\n");
						}
						else
						{
							state->lighting_ = (Geoweb3d::Vector::ExtrudedPathProperties::LightingMode)(state->lighting_ | Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::EXTRUDED_PATH_SHADED);
							printf("LIGHTING: EXTRUDED_PATH_SHADED ON\n");
						}
						props_dirty_ = true;
					}
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::K:
				{
					if (!global_prop_state)
					{
						printf("LIGHTING_MODE (must unselect a specific feature as this is a global property \n");
					}
					else
					{
						if (state->lighting_ & Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::CURTAIN_SHADED)
						{
							state->lighting_ = (Geoweb3d::Vector::ExtrudedPathProperties::LightingMode)(state->lighting_ & ~Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::CURTAIN_SHADED);
							printf("LIGHTING: CURTAIN_SHADED OFF\n");
						}
						else
						{
							state->lighting_ = (Geoweb3d::Vector::ExtrudedPathProperties::LightingMode)(state->lighting_ | Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::CURTAIN_SHADED);
							printf("LIGHTING: CURTAIN_SHADED ON\n");
						}
						props_dirty_ = true;
					}
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::L:
				{
					if (!global_prop_state)
					{
						printf("LIGHTING_MODE (must unselect a specific feature as this is a global property \n");
					}
					else
					{
						if (state->lighting_ & Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::DRAPED_LINE_SHADED)
						{
							state->lighting_ = (Geoweb3d::Vector::ExtrudedPathProperties::LightingMode)(state->lighting_ & ~Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::DRAPED_LINE_SHADED);
							printf("LIGHTING: DRAPED_LINE_SHADED OFF\n");
						}
						else
						{
							state->lighting_ = (Geoweb3d::Vector::ExtrudedPathProperties::LightingMode)(state->lighting_ | Geoweb3d::Vector::ExtrudedPathProperties::LightingMode::DRAPED_LINE_SHADED);
							printf("LIGHTING: DRAPED_LINE_SHADED ON\n");
						}
						props_dirty_ = true;
					}
					filter_out = true;
				}
				break;

				case Geoweb3d::Key::H:
				{
					printf("Changeable Properties: Click off a feature to change the defaults properties. Note: once an individual feature properties have changed\n");
					printf("the representation defaults will no longer apply to that feature (just need to hook up the mode to remove the feature properties if so desired).");
					printf(
					"\t Q - Alpha Mode							\n"
					"\t W - Extruded Path						\n"
					"\t E - Curtain								\n"
					"\t R - Draped Line							\n"
					"\t T - Line								\n"
					"\t Y - Relative offset on/off				\n"
					"\t A - Line Width							\n"
					"\t S - Toggle Outline 						\n"
					"\t D - Right metric Width					\n"
					"\t F - Left Metric Width					\n"
					"\t G - Z Offset							\n"
					"\t J - Extruded Path Shaded on/off			\n"
					"\t K - Curtain Shaded	on/off				\n"
					"\t L - Draped Line Shaded on/ff			\n"
					"\t Z - Alpha Value							\n"
					"\t X - Green Component of Color			\n"
					"\t C - Texture to Color Blend				\n"
					"\t V - curtain color alpha					\n"
					"\t B - curtain color bias					\n"
					"\t N - draped line color alpha				\n"
					"\t M - draped color bias					\n"
					
					
					
					
					
					
						);
					printf("Edit Mode: ");
					printf(
					"O - New feature creation mode. (left click - note doesn't show until you disengage 'O' )	\n"
					"P - Dragger mode. Click on feature. We will then search for the closest vertex to the mouse pointer to move."
					/*"I - Delete the feature."*/
						);


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
			if (!drag_mode_readied_ && !working_new_feature)
			{
				navHelper_->ProcessEvent(win_event, window_);
			}
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

