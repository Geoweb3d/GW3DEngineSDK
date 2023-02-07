#pragma once

#include <Geoweb3d/engine/IGW3DVectorLayerStream.h>
#include <Geoweb3dCore/GeometryExports.h>
#include <map>

struct TourPathPoint
{
	double longitude = 0.0;
	double latitude = 0.0;
	double elevation = 0.0;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float time = 0.0f;
};

class EntityController : public Geoweb3d::IGW3DVectorLayerStream
{
public:
	EntityController();
	virtual ~EntityController(void);
	virtual bool OnStream(Geoweb3d::IGW3DVectorLayerStreamResult *result);
	virtual bool OnError(/*todo*/);

	virtual unsigned long count() const
	{
		return 0;
	}

	virtual bool next(int64_t *ppVal)
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

	void SetRepresentation( Geoweb3d::IGW3DVectorRepresentationWPtr rep );
	void SeEntityPosition( int index, const TourPathPoint& point );

protected:
	/// <summary>	The last elevation seen. </summary>
	std::map< int, TourPathPoint > positions_;
	Geoweb3d::IGW3DVectorRepresentationWPtr rep_;
};
