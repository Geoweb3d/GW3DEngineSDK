#pragma once

using namespace Geoweb3d;

class FlightPath : public IGW3DVectorLayerStream
{
public:
	FlightPath();
	explicit FlightPath(const IGW3DVectorDataSourceWPtr& dataSource);
	~FlightPath();

	void SetSpacing(const double spacing);

	double GetMaxElevation();
	const IGW3DVectorDataSourceWPtr& GetDataSource();
	bool SetDataSource(const IGW3DVectorDataSourceWPtr& dataSource);

	int pointsAdded;
	double _Speed;
	double _Time;

private:
	IGW3DVectorDataSourceWPtr _PathDataSource;

	double _Spacing;
	double _MaxElevation;

private:
	// Inherited via IGW3DVectorLayerStream
	virtual unsigned long count() const override;
	virtual bool next(long * ppVal) override;
	virtual void reset() override;
	virtual long operator[](unsigned long index) override;
	virtual long get_AtIndex(unsigned long index) override;
	virtual bool OnStream(IGW3DVectorLayerStreamResult * result) override;
	virtual bool OnError() override;

	GW3DPoint CalcPointFromDistanceBearing(const GW3DPoint& point, const double distance, const double bearing);
};
