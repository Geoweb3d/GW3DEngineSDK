#include "stdafx.h"

#include "RangeRing.h"

#include "Mouse3DQuery.h"

#include "gui/GW3DGUICommon.h"
#include "gui/GW3DGUIVector.h"

#include "Geoweb3dCore/GeometryExports.h"

#include "Geoweb3d/engine/IGW3DGeoweb3dSDK.h"
#include "Geoweb3d/gui/IGW3DGUISDK.h"

#include "common/helpers/GW3DRangeRingHelper.h"

RangeRings* RangeRings::create (const char* name, Geoweb3d::GUI::IGW3DGUIStartContext *ctx) /* static */
{
	Geoweb3d::GUI::IGW3DGUIVectorDataSourceWPtr ds (ctx->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource").lock()->get_VectorDataSourceCollection()->create(name));
	if (ds.expired())
	{
		return 0;
	}

	Geoweb3d::IGW3DDefinitionCollectionPtr field_def (Geoweb3d::IGW3DDefinitionCollection::create());

	/* optional - add fields for storing feature information */

	Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer (ds.lock()->get_VectorLayerCollection()->create(name, Geoweb3d::gtPOINT, field_def));

	if (layer.expired())
	{
		Geoweb3d::GUI::IGW3DGUIVectorDataSourceCollection::close(ds);
		return 0;
	}

	Geoweb3d::GUI::IGW3DGUIVectorRepresentationDriverWPtr range_ring_rep_driver (ctx->get_VectorRepresentationDriverCollection()->get_Driver("RangeRing"));

	if (range_ring_rep_driver.expired())
	{
		Geoweb3d::GUI::IGW3DGUIVectorDataSourceCollection::close(ds);
		return 0;
	}

	Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr range_ring_rep (range_ring_rep_driver.lock()->get_RepresentationLayerCollection()->create_basic(layer));

	if (range_ring_rep.expired())
	{
		Geoweb3d::GUI::IGW3DGUIVectorDataSourceCollection::close(ds);
		return 0;
	}

	const Geoweb3d::IGW3DPropertyCollection* layer_defaults = range_ring_rep.lock()->get_PropertyCollection();
	Geoweb3d::IGW3DPropertyCollectionPtr new_properties = layer_defaults->create_Clone();
	new_properties->put_Property(Geoweb3d::GUI::RingProperties::NUMBER_OF_BEARING_LINES, 8);
	new_properties->put_Property(Geoweb3d::GUI::RingProperties::RING_0_INNER_RADIUS, 0.0);
	new_properties->put_Property(Geoweb3d::GUI::RingProperties::DRAW_RING_TYPE, 1 ); //1 = FILL, 0 = OUTLINE
	range_ring_rep.lock()->put_PropertyCollection(new_properties);
	range_ring_rep.lock()->get_ClampRadiusFilter()->put_MaxRadius(10000.0);
	return new RangeRings(name, range_ring_rep, ds);
}

RangeRings::RangeRings(const char* name, Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr rep, Geoweb3d::GUI::IGW3DGUIVectorDataSourceWPtr ds)
	:	rep_ (rep)
	,	ds_ (ds)
	,	scratch_point_ (new Geoweb3d::GW3DPoint())
	,	original_default_definition_ ( rep.lock()->get_PropertyCollection()->get_DefinitionCollection() )
{
	
}

RangeRings::~RangeRings()
{
	//tear down
	Geoweb3d::GUI::IGW3DGUIVectorDataSourceCollection::close(ds_);
}

long RangeRings::AddRangeRingFeature (const double& longitude, const double& latitude)
{
	if (rep_.expired())
	{
		//fatal error
		return -1;
	}

	Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer = rep_.lock()->get_VectorLayer();
	if (layer.expired())
	{
		//fatal error
		return -1;
	}

	scratch_point_->put_X(longitude);
	scratch_point_->put_Y(latitude);
	return layer.lock()->create_Feature(layer.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection(), scratch_point_);
}

int RangeRings::GetNumberOfAdditionalRingsPerRangeRingFeature() const
{
	const Geoweb3d::IGW3DPropertyCollection* layer_defaults = rep_.lock()->get_PropertyCollection();
	return layer_defaults->get_AtIndex(Geoweb3d::GUI::RingProperties::NUMBER_OF_ADDITIONAL_RINGS)->to_int();
}
	
void RangeRings::SetNumberOfAdditionalRingsPerRangeRingFeature (int count)
{
	const Geoweb3d::IGW3DPropertyCollection* layer_defaults = rep_.lock()->get_PropertyCollection();
	if (dirty_style_changes_)
	{
		dirty_style_changes_ = Geoweb3d::Helpers::resize_ToAdditionalRings(count, dirty_style_changes_, original_default_definition_);
	}
	else
	{
		dirty_style_changes_ = Geoweb3d::Helpers::resize_ToAdditionalRings(count, layer_defaults, original_default_definition_);
	}
}

Geoweb3d::GW3DResult RangeRings::GetRingInterval ( int ring, double& distance ) const
{
	const Geoweb3d::IGW3DPropertyCollection* layer_defaults = rep_.lock()->get_PropertyCollection();
	return Geoweb3d::Helpers::get_RingInterval( ring, layer_defaults, distance);
}

Geoweb3d::GW3DResult RangeRings::SetRingInterval ( int ring, const double& distance )
{
	const Geoweb3d::IGW3DPropertyCollection* layer_defaults = rep_.lock()->get_PropertyCollection();
	if (!dirty_style_changes_)
	{
		dirty_style_changes_ = layer_defaults->create_Clone(true);
	}
	Geoweb3d::GW3DResult success = Geoweb3d::Helpers::put_RingInterval( ring, dirty_style_changes_, distance);

	return success;
}

Geoweb3d::GW3DResult RangeRings::GetRingColor ( int ring, double& r, double& g, double& b, double& a ) const
{
	const Geoweb3d::IGW3DPropertyCollection* layer_defaults = rep_.lock()->get_PropertyCollection();
	return Geoweb3d::Helpers::get_RingColor( ring, layer_defaults, r, g, b, a );
}

Geoweb3d::GW3DResult RangeRings::SetRingColor ( int ring, const double& r, const double& g, const double& b, const double& a)
{
	const Geoweb3d::IGW3DPropertyCollection* layer_defaults = rep_.lock()->get_PropertyCollection();
	if (!dirty_style_changes_)
	{
		dirty_style_changes_ = layer_defaults->create_Clone(true);
	}
	Geoweb3d::GW3DResult success = Geoweb3d::Helpers::put_RingColor( ring, dirty_style_changes_, r, g, b, a);

	return success;
}

void RangeRings::CommitStyleChanges()
{
	if (dirty_style_changes_)
	{
		rep_.lock()->put_PropertyCollection(dirty_style_changes_);
	}
	dirty_style_changes_.reset();
}

void RangeRings::RefreshRepresentation()
{
	rep_.lock()->get_VectorLayer().lock()->force_RepresentationRefresh();
}
