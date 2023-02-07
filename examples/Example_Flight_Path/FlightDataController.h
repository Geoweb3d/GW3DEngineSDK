#pragma once

using namespace Geoweb3d;

typedef struct __VertexInformation {
	double Latitude;
	double Longitude;
	double Elevation;
	double BearingToNext;
	double DistanceToNext;
	double PitchToNext;
	double EffectiveHeading;
	double EffectivePitch;
} VertexInformation;

class FlightDataController : public IGW3DVectorLayerStream
{
public:
	FlightDataController();
	explicit FlightDataController(const IGW3DVectorDataSourceWPtr& dataSource);
	~FlightDataController();

	void SetPostSpacing(const double spacing);
	void SetPathWidth(const double width);
	void SetSectionLength(const double length);

	const IGW3DVectorDataSourceWPtr& GetDataSource() const;
	const bool SetDataSource(const IGW3DVectorDataSourceWPtr& dataSource);

	const double GetMaxElevation() const;
	const GW3DPoint GetInitialLocation() const;
	const double GetInitialHeading() const;
	const int GetPostCount() const;
	const int GetPathLength() const;
	const VertexInformation GetVertex(const size_t index);

private:
	IGW3DVectorDataSourceWPtr _PathDataSource;

	double _SectionLength;
	double _PathWidth;
	double _PostSpacing;
	double _MaxElevation;

	std::vector<VertexInformation> _Path;
	
	GW3DPoint _StartingPoint;
	double _InitialHeading;
	
	int _Posts;
	int _PathLength;

	IGW3DVectorLayerWPtr _PathLayer;
	IGW3DDefinitionCollectionPtr _PathDefinitions;

	IGW3DVectorLayerWPtr _PostLayer;
	IGW3DDefinitionCollectionPtr _PostDefinitions;

	IGW3DVectorLayerWPtr _RibbonLayer;
	IGW3DDefinitionCollectionPtr _RibbonDefinitions;

	IGW3DVectorLayerWPtr _ModelLayer;
	IGW3DDefinitionCollectionPtr _ModelDefinitions;

	IGW3DVectorLayerWPtr _VertexLayer;
	IGW3DDefinitionCollectionPtr _VertexDefinitions;
	unsigned int _HeadingPropertyIndex;

	IGW3DVectorLayerWPtr _RibbonLines1;
	IGW3DDefinitionCollectionPtr _RibbonLine1Defs;
	IGW3DVectorLayerWPtr _RibbonLines2;
	IGW3DDefinitionCollectionPtr _RibbonLine2Defs;

private:
	// Inherited via IGW3DVectorLayerStream
	virtual unsigned long count() const override;
	virtual bool next(int64_t * ppVal) override;
	virtual void reset() override;
	virtual int64_t operator[](unsigned long index) override;
	virtual int64_t get_AtIndex(unsigned long index) override;
	virtual bool OnStream(IGW3DVectorLayerStreamResult * result) override;
	virtual bool OnError() override;

	void CalcPointFromDistanceBearing(const GW3DPoint& point, const double distance, double bearing, GW3DPoint& output);
};
