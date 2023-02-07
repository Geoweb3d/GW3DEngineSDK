#pragma once

using namespace Geoweb3d;

class FlightController : public IGW3DVectorLayerStream
{
public:
	FlightController();
	~FlightController();

	GW3DPoint GetPointAtTime(const double time, const IGW3DVectorLayerWPtr& path);
	GW3DPoint MoveModelToTimePoint(const double time, const IGW3DVectorLayerWPtr& path, const IGW3DVectorLayerWPtr& model);

	double _Bearing;
	double _Pitch;
	double _Distance;

private:
	double _Time;
	GW3DPoint _PointOfInterest;

public:
	// Inherited via IGW3DVectorLayerStream
	virtual unsigned long count() const override;
	virtual bool next(long * ppVal) override;
	virtual void reset() override;
	virtual long operator[](unsigned long index) override;
	virtual long get_AtIndex(unsigned long index) override;
	virtual bool OnStream(IGW3DVectorLayerStreamResult * result) override;
	virtual bool OnError() override;
};

