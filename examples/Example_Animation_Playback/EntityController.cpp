/**
 * Property of Geoweb3d: GIS SDK
 * --------------------------------
 * Copyright 2008-2014 
 * Author: Vincent A. Autieri, Geoweb3d
 * Geoweb3d SDK is not free software: you cannot redistribute it and/or modify
 * it under any terms unless we have a written agreement between us.
 * Geoweb3d SDK and example applications are distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  
**/

////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	PeopleController.cpp
//
// summary:	Implements the people controller class
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <set>

#include "EntityController.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "GeoWeb3dCore/LayerParameters.h"


using namespace Geoweb3d;

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

void EntityController::SetRepresentation(Geoweb3d::IGW3DVectorRepresentationWPtr rep)
{
	rep_ = rep;
}

void EntityController::SeEntityPosition(int index, const TourPathPoint& point)
{
	positions_[index] = point;
}

void EntityController::RegisterEntityPlaybackObject(int fid, Geoweb3d::IGW3DAnimationPathPlaybackWPtr playback_object)
{
	PlaybackData& item = entity_playback_objects_[fid];
	item.playback_object = playback_object;
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

bool EntityController::OnStream(Geoweb3d::IGW3DVectorLayerStreamResult* result)
{
	const Geoweb3d::IGW3DAttributeCollection* attributesconst = result->get_AttributeCollection();

	Geoweb3d::GW3DGeometry* geometry = result->get_EditableGeometry();

	if (!geometry) //must be we are not in edit mode????
		return false;

	int object_id = static_cast<int>( result->get_ObjectID() );

	auto playback_data_iter = entity_playback_objects_.find( object_id );

	auto iter = positions_.find( object_id );
	if (iter == positions_.end() || playback_data_iter == entity_playback_objects_.end() )
		return false;

	int image_index = object_id % 3;


	if (geometry->get_GeometryType() == Geoweb3d::gtPOINT_25D )
	{
		if( playback_data_iter->second.playback_started_ )
		{
			Geoweb3d::GW3DPoint* pt = dynamic_cast<Geoweb3d::GW3DPoint*>(geometry);
			pt->put_X(iter->second.longitude);
			pt->put_Y(iter->second.latitude);
		}
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
		auto propertyIndex = representation_properties->get_DefinitionCollection()->get_IndexByName("TEXTURE_PALETTE_INDEX");

		if (propertyIndex != -1)
		{
			result->put_VectorRepresentationProperty( rep_, propertyIndex, image_index );
		}
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

//IGW3DAnimationPathPlaybackEventCallback overrides
void EntityController::OnPlaybackStarted(const Geoweb3d::IGW3DAnimationPathPlayback* owner)
{
	auto iter = std::find_if(std::begin(entity_playback_objects_), std::end(entity_playback_objects_)
		, [owner](EntityPlaybackObjectMap::value_type& item)
	{

		return item.second.playback_object.lock().get() == owner;
	});

	if (iter != std::end(entity_playback_objects_))
	{
		iter->second.playback_started_ = true;
	}
}

void EntityController::OnPlaybackStopped(const Geoweb3d::IGW3DAnimationPathPlayback* owner)
{
	auto iter = std::find_if(std::begin(entity_playback_objects_), std::end(entity_playback_objects_)
		, [owner](EntityPlaybackObjectMap::value_type& item)
	{

		return item.second.playback_object.lock().get() == owner;
	});

	if (iter != std::end(entity_playback_objects_))
	{
		iter->second.playback_started_ = false;
	}
}

void EntityController::OnPlaybackUpdated(const Geoweb3d::IGW3DAnimationPathPlayback* owner, const Geoweb3d::GW3DPathPoint& playback_point)
{
	auto iter = std::find_if(std::begin(entity_playback_objects_), std::end(entity_playback_objects_)
		, [owner](EntityPlaybackObjectMap::value_type& item)
	{

		return item.second.playback_object.lock().get() == owner;
	});

	if (iter != std::end(entity_playback_objects_))
	{
		auto& point = positions_[iter->first];
		point = { 0 };
		point.latitude	= playback_point.latitude;
		point.longitude = playback_point.longitude;
	}
}

