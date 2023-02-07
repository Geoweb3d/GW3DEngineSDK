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
// file:	EntityController.h
// file:	EntityController.h
//
// summary:	Declares the entity controller class
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Geoweb3d/engine/IGW3DVectorLayerStream.h>
#include <Geoweb3dCore/GeometryExports.h>
#include <Geoweb3d/engine/IGW3DAnimationPathPlayback.h>
#include <Geoweb3d/engine/IGW3DAnimationPathPlaybackEventCallback.h>

#include <map>

struct TourPathPoint
{
	double latitude = 0.0;
	double longitude = 0.0;
	double elevation = 0.0;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float time = 0.0f;
};

class EntityController 
	: public Geoweb3d::IGW3DVectorLayerStream
	, public Geoweb3d::IGW3DAnimationPathPlaybackEventCallback
{
public:
	EntityController();
	virtual ~EntityController(void);
	virtual bool OnStream(Geoweb3d::IGW3DVectorLayerStreamResult* result);
	virtual bool OnError(/*todo*/);

	virtual unsigned long count() const
	{
		return 0;
	}

	virtual bool next(int64_t* ppVal)
	{
		return true;
	}

	virtual void reset()
	{}

	virtual int64_t operator[](unsigned long index)
	{
		return 0;
	}
	virtual int64_t get_AtIndex(unsigned long index)
	{
		return 0;
	}

	//IGW3DAnimationPathPlaybackEventCallback overrides
	void OnPlaybackStarted( const Geoweb3d::IGW3DAnimationPathPlayback* owner) override;

	void OnPlaybackStopped( const Geoweb3d::IGW3DAnimationPathPlayback* owner) override;

	void OnPlaybackUpdated( const Geoweb3d::IGW3DAnimationPathPlayback* owner, const Geoweb3d::GW3DPathPoint& playback_point) override;

public:
	void SetRepresentation(Geoweb3d::IGW3DVectorRepresentationWPtr rep);
	void SeEntityPosition(int index, const TourPathPoint& point);
	void RegisterEntityPlaybackObject(int fid, Geoweb3d::IGW3DAnimationPathPlaybackWPtr playback_object);

protected:
	struct PlaybackData
	{
		Geoweb3d::IGW3DAnimationPathPlaybackWPtr playback_object;
		bool playback_started_ = false;
	};

	using EntityPlaybackObjectMap = std::map< int, PlaybackData>;
	EntityPlaybackObjectMap entity_playback_objects_;

	std::map< int, TourPathPoint > positions_;
	Geoweb3d::IGW3DVectorRepresentationWPtr rep_;

};
