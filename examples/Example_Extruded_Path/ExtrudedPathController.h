#pragma once

#include <Geoweb3d/engine/IGW3DVectorLayerStream.h>
#include <Geoweb3dCore/GeometryExports.h>
#include <deque>
class ExtrudedPathController : public Geoweb3d::IGW3DVectorLayerStream
{
public:
	ExtrudedPathController();
	virtual ~ExtrudedPathController(void);
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

	void SetRepresentation(Geoweb3d::IGW3DVectorRepresentationWPtr rep);
	void SetLayer(Geoweb3d::IGW3DVectorLayerWPtr dynamic_layer);
	void SetLocation(double lon, double lat);
	void SetDragMode(bool drag_mode);
	void SetWorkingFID(long fid);
	long GetWorkingFID() { return working_fid_; };
	void CaptureAScribPoint();
	
	void CreateScribedLine(Geoweb3d::IGW3DPropertyCollectionPtr defaults);
	void StopScribingLine();
	void DeleteFeature();

protected:
	double lon_ = 0.0, lat_ = 0.0;
	bool drag_mode_ = false;
	bool new_feature_mode_ = false;
	long working_fid_ = -1;
	Geoweb3d::IGW3DVectorRepresentationWPtr rep_;
	Geoweb3d::IGW3DVectorLayerWPtr dynamic_layer_;
	std::deque<Geoweb3d::GW3DPoint> scribing_pts_;
};
