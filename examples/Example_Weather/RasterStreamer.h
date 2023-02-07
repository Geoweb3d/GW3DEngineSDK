#pragma once

#include <Geoweb3d/engine/IGW3DRasterLayerEnvelopeStream.h>
#include <Geoweb3dCore/GeometryExports.h>
#include <deque>

class RasterStreamer : public Geoweb3d::IGW3DRasterLayerEnvelopeStream
{
public:
	RasterStreamer();
	virtual ~RasterStreamer(void);
	virtual bool OnStream(Geoweb3d::IGW3DRasterLayerEnvelopeStreamResult* result);
	virtual bool OnError(/*todo*/);

	virtual unsigned long count() const
	{
		return 0;
	}

	virtual bool next(unsigned long* ppVal)
	{
		return true;
	}

	virtual void reset()
	{}

	virtual unsigned long operator[](unsigned long index)
	{
		return 0;
	}
	virtual unsigned long get_AtIndex(unsigned long index)
	{
		return 0;
	}

protected:
};
