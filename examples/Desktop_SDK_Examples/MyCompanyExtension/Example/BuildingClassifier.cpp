#include "stdafx.h"
#include "BuildingClassifier.h"
#include "gui/IGW3DGUIVectorLayerStreamResult.h"

#include "gui/GW3DGUIVector.h"
#include "gui/GW3DGUICommon.h"

#include <set>

using namespace Geoweb3d;
using namespace GUI;

BuildingTexturizer::BuildingTexturizer(Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr buildings)
	: buildings_(buildings)
{}

BuildingTexturizer::~BuildingTexturizer() 
{}

void BuildingTexturizer::Stream(Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr buildings)
{
	BuildingTexturizer texturizer(buildings);
	buildings.lock()->get_VectorLayer().lock()->Stream(&texturizer);
}

bool BuildingTexturizer::OnStream( IGW3DGUIVectorLayerStreamResult *result )
{

	double perimeter_in_meters = 0;

	const Geoweb3d::GW3DPolygon *poly = dynamic_cast<const Geoweb3d::GW3DPolygon*>( result->get_Geometry() );
		 
	GW3DLinearRingPtr ring = poly->get_ExteriorRing();
		
	ring->get_Point(0, &pt1_);
	ring->get_Point(0, &pt2_);

	for(int i = 0; i < ring->get_NumPoints();++i)
	{
		ring->get_Point(i, &pt1_);
			
		double distance; 
		pt1_.get_DistanceInMeters(&pt2_, false, distance);
			
		perimeter_in_meters +=distance;

		pt2_ = pt1_;
	}

	// If we can determine the width of the texture in meters, 
	// we can derive the number of times the texture should be repeated.
	// Here we assume the texture width is 30 meters.

	double repeat_n_horiz = perimeter_in_meters / 30.0;

	result->put_VectorRepresentationProperty(buildings_, Geoweb3d::GUI::ExtrudedPolygonProperties::TEXTURE_SIDE_REPEAT_N_TIMES_X, repeat_n_horiz);

    return true;
}

void BuildingTexturizer::OnError(/*todo*/ )
{
    //something was done by the user in the streamer that messed things up so bad we have to tell you about it.
}



BuildingClassifier::BuildingClassifier(Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr buildings)
	: buildings_(buildings)
	, bldg_layer_(buildings.lock()->get_VectorLayer())
	, neighborhood_field_index_(-1)
	, height_field_index_(-1)
	, do_randomize_textures_(false)
	, do_assign_heights_(false)
	, fp_ (true)
{}

BuildingClassifier::~BuildingClassifier() 
{}

void BuildingClassifier::SetHeightAttribute (const char* attrib_heading)
{
	const Geoweb3d::IGW3DDefinitionCollection* attribute_def = bldg_layer_.lock()->get_AttributeDefinitionCollection();
	int height_field_index_ = attribute_def->get_IndexByName(attrib_heading);
}

void BuildingClassifier::SetNeighborhoodAttribute (const char* attrib_heading)
{
	const Geoweb3d::IGW3DDefinitionCollection* attribute_def = bldg_layer_.lock()->get_AttributeDefinitionCollection();
	int neighborhood_field_index_ = attribute_def->get_IndexByName(attrib_heading);
}

void BuildingClassifier::Run()
{
	bldg_layer_.lock()->Stream(this);
	dirty_neighborhoods_.clear();
	fp_ = false;
}

void BuildingClassifier::CreateNeighborhoodIcons(Geoweb3d::GUI::IGW3DGUIVectorDriverCollection* vdrivers, Geoweb3d::GUI::IGW3DGUIVectorRepresentationDriverCollection* rdrivers)
{
	Geoweb3d::GUI::IGW3DGUIVectorDriverWPtr shp_driver = vdrivers->get_Driver("Geoweb3d_Datasource");

	Geoweb3d::GUI::IGW3DGUIVectorDataSourceCollection* datasources = shp_driver.lock()->get_VectorDataSourceCollection();
	Geoweb3d::GUI::IGW3DGUIVectorDataSourceWPtr my_datasource = datasources->create("Neighborhoods");

		/* Set up our attribute fields */
	Geoweb3d::IGW3DDefinitionCollectionPtr attribute_fields = Geoweb3d::IGW3DDefinitionCollection::create();;
	attribute_fields->add_Property("NeighborhoodID", Geoweb3d::PROPERTY_INT, "");
	attribute_fields->add_Property("ENERGY MANAGEMENT", Geoweb3d::PROPERTY_STR, "", 255);
	attribute_fields->add_Property("Label", Geoweb3d::PROPERTY_STR, "Hoola", 255);

	/* Create a new point layer for my neighborhoods */
	Geoweb3d::GUI::IGW3DGUIVectorLayerCollection* layers_in_my_datasource = my_datasource.lock()->get_VectorLayerCollection();
	neighborhood_layer_ = layers_in_my_datasource->create("Neighborhoods", Geoweb3d::gtPOINT, attribute_fields);

	Geoweb3d::GUI::IGW3DGUIVectorRepresentationDriverWPtr icon_driver = rdrivers->get_Driver(gw3dIconRepresentation);

	neighborhood_icons_ = icon_driver.lock()->get_RepresentationLayerCollection()->create_basic(neighborhood_layer_);

	Geoweb3d::IGW3DPropertyCollectionPtr icon_properties = neighborhood_icons_.lock()->get_PropertyCollection()->create_Clone();
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_MODE, (int)Geoweb3d::GUI::IconProperties::LABEL_OFF);
    icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_ATTRIBUTE, "Label");
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::BALLOON_ATTRIBUTE, "ENERGY MANAGEMENT");	//
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::BALLOON_CONTENTS, (int)Geoweb3d::GUI::IconProperties::BALLOON_SINGLE_ATTR);	//
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::BALLOON_MODE, (int)Geoweb3d::GUI::IconProperties::BALLOON_ON_CLICK); //only show the balloon when you click
    icon_properties->put_Property(Geoweb3d::GUI::IconProperties::ICON_URL, "");
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_RED, 1.0); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_GREEN, 1.0); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_BLUE, 1.0); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_OUTLINE_RED, 0.0); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_OUTLINE_GREEN, 0.0); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_OUTLINE_BLUE, 0.0); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_POINT_SIZE, 15);   
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::TRANSLATION_Z_OFFSET, 5.0); //elevation
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::TRANSLATION_Z_OFFSET_MODE, Geoweb3d::GUI::RELATIVE_MODE | Geoweb3d::GUI::IGNORE_VERTEX_Z);  //Relative to ground + ignore Z coordinate (i.e. ground clamped)
    neighborhood_icons_.lock()->put_PropertyCollection(icon_properties);

	//Geoweb3d::GW3DPoint scratch_pt;
	//NeighborhoodExtents::iterator i = extents_database_.begin();
	//while (i != extents_database_.end())
	//{
	//	Geoweb3d::FieldValues fieldvals(&attribute_fields);
	//	fieldvals.SetProperty(0, i->first);
	//	if (i->first == 2)
	//	{
	//		fieldvals.SetProperty(1, "http://app.geoweb3d.com/demo/CostMessage.PNG");
	//	}
	//	else if (i->first == 6) 
	//	{
	//		fieldvals.SetProperty(1, "http://app.geoweb3d.com/demo/CostMessage.PNG");
	//	}
	//	fieldvals.SetProperty(2, "Energy Management Disabled");

	//	scratch_pt.setX((i->second.MaxX + i->second.MinX)/2);
	//	scratch_pt.setY((i->second.MaxY + i->second.MinY)/2);
	//	neighborhood_layer_.lock()->create_Feature(&fieldvals, &scratch_pt);
	//	i++;
	//}
	
}

void BuildingClassifier::AlertNeighborhood (int neighborhood_id)
{
	Geoweb3d::IGW3DPropertyCollectionPtr icon_properties = neighborhood_icons_.lock()->get_PropertyCollection()->create_Clone();
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::ICON_URL, "./html/images/warn.png");
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::ICON_SCALE, 1.5);
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_MODE, (int)Geoweb3d::GUI::IconProperties::LABEL_ON); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_ATTRIBUTE, "Label"); 

	//icon_properties->put_Property(Geoweb3d::GUI::IconProperties::BALLOON_MODE, (int)Geoweb3d::GUI::IconProperties::ON_ALWAYS); //always show the balloon
	neighborhood_icons_.lock()->put_PropertyCollection(neighborhood_id,icon_properties);
}

void BuildingClassifier::AssignNeighborhoodColor (int neighborhood_id, BuildingColor color)
{
	//color_database_[neighborhood_id] = color;
	//dirty_neighborhoods_.insert(neighborhood_id);
}

bool BuildingClassifier::OnStream( IGW3DGUIVectorLayerStreamResult *result )
{

	const Geoweb3d::IGW3DAttributeCollection* attributes = result->get_AttributeCollection();
	
	const Geoweb3d::IGW3DPropertyCollection* props = buildings_.lock()->get_PropertyCollection(result->get_ObjectID());
	if (!props)
	{
		props = buildings_.lock()->get_PropertyCollection();
	}
	Geoweb3d::IGW3DPropertyCollectionPtr props_copy = props->create_Clone();
	bool set_it = false;

	if (do_assign_heights_)
	{
		if (height_field_index_ >= 0)
		{
			const Geoweb3d::IGW3DVariant& height = attributes->get_Property(height_field_index_);
			props_copy->put_Property(Geoweb3d::GUI::ExtrudedPolygonProperties::EXTRUSION_HEIGHT, height);
			set_it = true;
		}
	}

	//if (neighborhood_field_index_ >= 0)
	//{
	//	Geoweb3d::IGW3DVariant neighborhood_id = attributes->get_Property(neighborhood_field_index_);

	//	if (fp_)
	//	{
	//		//determine extents
	//		NeighborhoodExtents::iterator ei = extents_database_.find(neighborhood_id);
	//		if (ei != extents_database_.end())
	//		{
	//			Geoweb3d::GW3DEnvelope extents;
	//			result->get_Geometry()->getEnvelope(&extents);
	//			extents_database_[neighborhood_id].Merge(extents); 
	//		}
	//		else
	//		{
	//			Geoweb3d::GW3DEnvelope extents;
	//			result->get_Geometry()->getEnvelope(&extents);
	//			extents_database_[neighborhood_id] = extents; 
	//		}
	//	}

	//	std::set<int>::iterator itr_hood_dirty = std::find(dirty_neighborhoods_.begin(), dirty_neighborhoods_.end(), neighborhood_id);
	//	if (itr_hood_dirty != dirty_neighborhoods_.end())
	//	{
	//		NeighborhoodColors::iterator i = color_database_.find(neighborhood_id);
	//		if (i != color_database_.end())
	//		{
	//			props_copy->put_Property(Geoweb3d::GUI::ExtrudedPolygonProperties::RED, (float)i->second.red_/255.f);
	//			props_copy->put_Property(Geoweb3d::GUI::ExtrudedPolygonProperties::GREEN, (float)i->second.green_/255.f);
	//			props_copy->put_Property(Geoweb3d::GUI::ExtrudedPolygonProperties::BLUE, (float)i->second.blue_/255.f);
	//			set_it = true;
	//		}
	//	}
	//}

	if (props_copy.get())
	{
		buildings_.lock()->put_PropertyCollection(result->get_ObjectID(), props_copy);
	}
 
    return true;
}

void BuildingClassifier::OnError(/*todo*/ )
{
    //something was done by the user in the streamer that messed things up so bad we have to tell you about it.
}
