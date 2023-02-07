#include "stdafx.h"
#include "EntityController.h"

#include <Geoweb3d\common\GW3DCommon.h>
#include <Geoweb3d\engine\GW3DVector.h>
#include <Geoweb3dCore\LayerParameters.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="rep">	The rep. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

EntityController::EntityController()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Destructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

EntityController::~EntityController()
{}

void EntityController::SetRepresentation( Geoweb3d::IGW3DVectorRepresentationWPtr rep )
{
	rep_ = rep;
}

void EntityController::SeEntityPosition( int index, const TourPathPoint& point )
{
	positions_[index] = point;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the stream action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="result">	[out] if non-null, the result. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool EntityController::OnStream(Geoweb3d::IGW3DVectorLayerStreamResult *result)
{
	const Geoweb3d::IGW3DAttributeCollection *attributesconst = result->get_AttributeCollection();
	//Geoweb3d::IGW3DAttributeCollection *attributes = result->get_ModifyableAttributeProperties() ;

	Geoweb3d::GW3DGeometry* geometry = result->get_EditableGeometry();

	long object_id = result->get_ObjectID();

	if (!geometry) //must be we are not in edit mode????
		return false;

	auto iter = positions_.find( static_cast<int>( object_id ) );
	if ( iter == positions_.end() )
		return false;

	if (geometry->get_GeometryType() == Geoweb3d::gtPOINT_25D)
	{
		Geoweb3d::GW3DPoint* pt = dynamic_cast<Geoweb3d::GW3DPoint*>(geometry);
		pt->put_X( iter->second.longitude );
		pt->put_Y( iter->second.latitude );
		pt->put_Z( iter->second.elevation );
	}
	else
	{
		printf("Need to add code for this other geometry type\n");
	}

	const Geoweb3d::IGW3DPropertyCollection* representation_properties = result->get_VectorRepresentationProperties(rep_);
	if (!representation_properties)
	{
		//unsafe_get saves some overhead vs a lock, but should only be used when you are sure the object can't get deleted while you are using it
		representation_properties = rep_.unsafe_get()->get_PropertyCollection();
	}

	if (representation_properties)
	{
		std::string url = R"(data\sdk\models\DIS\t72m1_tank.flt)";
		static int url_index = representation_properties->get_DefinitionCollection()->get_IndexByName("URL"); //...or, by string
		result->put_VectorRepresentationProperty(rep_, url_index, url.c_str());

		static int scale_x = representation_properties->get_DefinitionCollection()->get_IndexByName("SCALE_X");
		static int scale_y = representation_properties->get_DefinitionCollection()->get_IndexByName("SCALE_Y");
		static int scale_z = representation_properties->get_DefinitionCollection()->get_IndexByName("SCALE_Z");
		result->put_VectorRepresentationProperty(rep_, scale_x, 1.0);
		result->put_VectorRepresentationProperty(rep_, scale_y, 1.0);
		result->put_VectorRepresentationProperty(rep_, scale_z, 1.0);

		static int heading_property_index = representation_properties->get_DefinitionCollection()->get_IndexByName("HEADING"); //...or, by string
		static int pitch_offset_property_index = representation_properties->get_DefinitionCollection()->get_IndexByName("PITCH"); //...or, by string
		static int roll_offset_property_index = representation_properties->get_DefinitionCollection()->get_IndexByName("ROLL"); //...or, by string

		static int z_offset_property_index = representation_properties->get_DefinitionCollection()->get_IndexByName("TRANSLATION_Z_OFFSET"); //...or, by string
		static int z_offset_mode_property_index = representation_properties->get_DefinitionCollection()->get_IndexByName("TRANSLATION_Z_OFFSET_MODE"); //...or, by string
																																					   ////this is the optimized way to set the representation property!
		double heading = representation_properties->get_Property(Geoweb3d::Vector::ModelProperties::HEADING);
		double pitch = representation_properties->get_Property(Geoweb3d::Vector::ModelProperties::PITCH);
		double roll = representation_properties->get_Property(Geoweb3d::Vector::ModelProperties::ROLL);

		double z_offset = representation_properties->get_Property(Geoweb3d::Vector::ModelProperties::TRANSLATION_Z_OFFSET);

		result->put_VectorRepresentationProperty( rep_, heading_property_index, iter->second.yaw );
		result->put_VectorRepresentationProperty( rep_, pitch_offset_property_index, iter->second.pitch );
		result->put_VectorRepresentationProperty( rep_, roll_offset_property_index, iter->second.roll );
		//result->put_VectorRepresentationProperty( rep_, z_offset_property_index, iter->second.elevation );
		//result->put_VectorRepresentationProperty( rep_, z_offset_mode_property_index, 0 );
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the error action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool EntityController::OnError(/*todo*/)
{
	//something was done by the user in the streamer that messed things up so bad we have to tell you about it.
	return false;
}
