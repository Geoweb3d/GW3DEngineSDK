
#include "SpatialDB.h"

#include "common/GW3DCommon.h"
#include "Geoweb3dCore/LayerParameters.h"

#include <sstream>

SpatialDB::SpatialDB()
{
	spatial_database_ = Geoweb3d::SpatialDatabaseFactory::create_SpatialDatabase();

	field_definition_ = Geoweb3d::IGW3DDefinitionCollection::create();
	name_idx_ = field_definition_->add_Property("Name", Geoweb3d::PROPERTY_STR, "");
	function_idx_ = field_definition_->add_Property("Function", Geoweb3d::PROPERTY_STR, "");
}

SpatialDB::~SpatialDB()
{
}

void SpatialDB::SetBounds(Geoweb3d::GW3DEnvelope& env)
{
	data_bounds_ = env;
}

int fid_to_look_for = 0;

bool SpatialDB::OnStream(Geoweb3d::IGW3DVectorLayerStreamResult* result)
{
	const Geoweb3d::IGW3DAttributeCollection* attributesconst = result->get_AttributeCollection();
	const Geoweb3d::GW3DGeometry* geometry = result->get_Geometry();
	long object_id = result->get_ObjectID();

	std::string name;
	std::string function;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///
	/// Here we use some of the shape file data to populate our own database
	/// note - not all features have a name or a function attribute in this reference dataset
	/// so we'll doctor our properties a little to make up for that. 
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//note - not features have a name or a function in this reference dataset
	int name_attribute_idx = attributesconst->get_DefinitionCollection()->get_IndexByName("NAME");
	int function_attribute_idx = attributesconst->get_DefinitionCollection()->get_IndexByName("Function");

	std::stringstream ss;

	//Always good practice to validate and property is present that we expect
	if (name_attribute_idx >= 0)
	{
		Geoweb3d::IGW3DStringPtr gw3d_str;
		attributesconst->get_Property(name_attribute_idx).to_string(gw3d_str);

		name = gw3d_str->c_str();

		ss << object_id;
		name = name + " ID: " + ss.str();
	}

	if (name.empty())
	{
		//Adding the Feature ID so we have something in this field
		ss << object_id;
		name = "No name found for ID: " + ss.str();

	}

	ss.clear();
	//Always good practice to validate and property is present that we expect
	if (function_attribute_idx >= 0)
	{
		Geoweb3d::IGW3DStringPtr gw3d_str;
		attributesconst->get_Property(function_attribute_idx).to_string(gw3d_str);
		function = gw3d_str->c_str();
	}

	if (function.empty())
	{
		//Adding some text to for a fuller database 
		ss << object_id;
		function = "None listed" ;

	}

	Geoweb3d::IGW3DPropertyCollectionPtr attributes = field_definition_->create_PropertyCollection();

	attributes->put_Property(name_idx_, name.c_str());
	attributes->put_Property(function_idx_, function.c_str());

	data_[object_id] = BaseDataPtr(new BaseData({ GW3DGeometryPtr(geometry->clone()), attributes, object_id }));

	Geoweb3d::GW3DEnvelope record;
	geometry->get_Envelope(&record);
	spatial_database_->put_Data(object_id, record);

	return true;
}

bool SpatialDB::OnError()
{
	return true;
}


class OnResultFound : public Geoweb3d::IGW3DSpatialDatabaseCallback
{
public:
	using ResultCallback = std::function<void(const uint64_t& index, const Geoweb3d::GW3DEnvelope& env)>;

	OnResultFound(ResultCallback lambda_callback)
		: lambda_callback_(lambda_callback)
	{}

	void found_RecordIndex(const uint64_t& index, const Geoweb3d::GW3DEnvelope& env) override
	{
		if (lambda_callback_)
		{
			lambda_callback_(index, env);
		}
	}

private:
	ResultCallback lambda_callback_;
};


Geoweb3d::GW3DResult SpatialDB::GW3DTHREADED_VectorPipeline_get_FeatureObjectsInBounds(const Geoweb3d::IGW3DVectorLayerWPtr layer, const Geoweb3d::GW3DEnvelope& bounds, Geoweb3d::IGW3DFeatureObjectLoader* loader)
{
	if (spatial_database_)
	{
		auto CallPageInReader = [&](const uint64_t& index, const Geoweb3d::GW3DEnvelope& env)
		{
			loader->add_Feature(static_cast<long>(index), data_[index]->geom_data_.get(), data_[index]->attributes_);
		};

		OnResultFound finder(CallPageInReader);

		spatial_database_->search(bounds, &finder);
	}
	return Geoweb3d::GW3D_sOk;
}

Geoweb3d::GW3DEnvelope SpatialDB::GW3DTHREADED_VectorPipeline_get_GetLayerBounds()
{
	return data_bounds_;
}

Geoweb3d::Vector::IGW3DConstructionCallback::ConstructionCallbackReturnType ExtrudedFeatureConstructionCb::onInsert(long feature_id, const Geoweb3d::IGW3DPropertyCollection* feature_attributes, Geoweb3d::Vector::IGW3DConstructionCallbackRepresentationSettings* rep)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	///
	/// Here we get the layer attribute indices
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	static const unsigned attribute_function_idx = feature_attributes->get_DefinitionCollection()->get_IndexByName("Function");

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///
	/// Here we get the representation property indices
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	static const unsigned rep_red_idx = Geoweb3d::Vector::ExtrudedPolygonProperties::RED;
	static const unsigned rep_green_idx = Geoweb3d::Vector::ExtrudedPolygonProperties::GREEN;
	static const unsigned rep_blue_idx = Geoweb3d::Vector::ExtrudedPolygonProperties::BLUE;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///
	/// And here we map the layer attribute to a corresponding represenation property
	///
	///	We show some basic mapping of the feature's "Function" to a representation color
	/// And we also show how not to save individual representation properties when not needed
	/// (i.e. use the representation defaults)
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Color color = Color(0.0, 0.0, 0.0);
	bool classify_color_ = false;

	const Geoweb3d::IGW3DVariant& val = feature_attributes->get_Property(attribute_function_idx);

	const char* category = val.raw_string();
	if (!strcmp(category, "Police Station"))
	{
		classify_color_ = true;
		color = Color(1.0, 1.0, 0.0);
	}
	else if (!strcmp(category, "Hotel"))
	{
		classify_color_ = true;
		color = Color(0.0, 1.0, 0.0);
	}
	else if (!strcmp(category, "Hospital"))
	{
		classify_color_ = true;
		color = Color(1.0, 0.0, 1.0);
	}
	else if (!strcmp(category, "Fire Station"))
	{
		classify_color_ = true;
		color = Color(1.0, 1.0, 1.0);
	}
	else
	{
		classify_color_ = false;
	}

	if (classify_color_)
	{
		rep->put_VectorRepresentationProperty(rep_red_idx, color.red);
		rep->put_VectorRepresentationProperty(rep_green_idx, color.green);
		rep->put_VectorRepresentationProperty(rep_blue_idx, color.blue);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		///
		///  Decide whether to persist this property. Persistence allows a client not to have to store the state
		///	 of a propety themselves (for example if they wish to query back later what the property was. 
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////
		return Geoweb3d::Vector::IGW3DConstructionCallback::KEEP_AND_SAVE;
	}
	else
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		///
		///  Decide whether to persist this property. Persistence allows a client not to have to store the state
		///	 of a propety themselves (for example if they wish to query back later what the property was. 
		///
		///  In this case no need to save the properties as the defaults are fine (saves on memory)
		////////////////////////////////////////////////////////////////////////////////////////////////////
		return Geoweb3d::Vector::IGW3DConstructionCallback::KEEP;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Database accessors </summary>
///
////////////////////////////////////////////////////////////////////////////////////////////////////

BaseDataPtr SpatialDB::get_DataItemRefereence(long fid)
{
	std::map<uint64_t, BaseDataPtr> data_map;
	std::map< uint64_t, BaseDataPtr>::iterator itr;

	itr = data_.find(fid);

	if (itr != data_map.end())
	{
		return itr->second;
	}
	else
	{
		return BaseDataPtr();
	}
}

void SpatialDB::get_Function(long fid, Geoweb3d::IGW3DStringPtr& str)
{
	std::map<uint64_t, BaseDataPtr> data_map;
	std::map< uint64_t, BaseDataPtr>::iterator itr;

	itr = data_.find(fid);

	if (itr != data_map.end())
	{
		itr->second->attributes_->get_Property(function_idx_).to_string(str);
	}
	else
	{
		return;
	}
}

void SpatialDB::get_Name(long fid, Geoweb3d::IGW3DStringPtr& str)
{
	std::map<uint64_t, BaseDataPtr> data_map;
	std::map< uint64_t, BaseDataPtr>::iterator itr;

	itr = data_.find(fid);

	if (itr != data_map.end())
	{
		itr->second->attributes_->get_Property(name_idx_).to_string(str);
	}
	else
	{
		return;
	}
}

//Geoweb3d::IGW3DGeometryVisitor
void SpatialDB::visit(const Geoweb3d::GW3DPoint* geom)
{

}
void SpatialDB::visit(const Geoweb3d::GW3DLineString* geom)
{

}
void SpatialDB::visit(const Geoweb3d::GW3DLinearRing* geom)
{

}
void SpatialDB::visit(const Geoweb3d::GW3DPolygon* geom)
{
	printf("polygon num points %d:\n", geom->get_ExteriorRing()->get_NumPoints());
}
void SpatialDB::visit(const Geoweb3d::GW3DMultiPoint* geom)
{

}
void SpatialDB::visit(const Geoweb3d::GW3DMultiLineString* geom)
{

}
void SpatialDB::visit(const Geoweb3d::GW3DMultiPolygon* geom)
{

}
