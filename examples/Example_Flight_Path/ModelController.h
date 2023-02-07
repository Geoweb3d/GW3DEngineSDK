#pragma once

using namespace Geoweb3d;

class ModelController : public IGW3DVectorLayerStream
{
public:
	ModelController();
	~ModelController();

	void UpdateModel(const GW3DPoint& location, const double bearing, const IGW3DVectorLayerWPtr& model);
	void GetCurrentLocation(GW3DPoint& location, double& bearing) const;

private:
	GW3DPoint _ModelLocation;
	double _ModelBearing;
	IGW3DVectorRepresentationWPtr _ModelRepresentation;

public:
	// Inherited via IGW3DVectorLayerStream
	virtual unsigned long count() const override;
	virtual bool next(int64_t * ppVal) override;
	virtual void reset() override;
	virtual int64_t operator[](unsigned long index) override;
	virtual int64_t get_AtIndex(unsigned long index) override;
	virtual bool OnStream(IGW3DVectorLayerStreamResult * result) override;
	virtual bool OnError() override;
};

