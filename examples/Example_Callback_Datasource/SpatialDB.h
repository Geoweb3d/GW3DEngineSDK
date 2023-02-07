#pragma once

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/GW3DVector.h"
#include "Common/IGW3DSpatialDatabase.h"
#include <map>
#include <memory>

typedef std::shared_ptr< Geoweb3d::GW3DGeometry> GW3DGeometryPtr;
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A databse datum. </summary>
///
////////////////////////////////////////////////////////////////////////////////////////////////////
struct BaseData
{
	GW3DGeometryPtr geom_data_;
	Geoweb3d::IGW3DPropertyCollectionPtr attributes_;
	long fid_;
};
typedef std::shared_ptr< BaseData> BaseDataPtr;

struct Color
{
	double red;
	double green;
	double blue;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Geoweb3d,  4/7/2020. </remarks>
	///
	/// <param name="r">	The double to process. </param>
	/// <param name="g">	The double to process. </param>
	/// <param name="b">	The double to process. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Color(double r, double g, double b)
		:red(r), green(g), blue(b)
	{}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	SpatialDB. </summary>
///
/// <remarks>	Geoweb3d, 4/7/2020. 
///				SpatialDB is essentially just an abstraction class for working with the client data.
///				It contains a spatial search utility via the IGW3DSpatialDatabase interface.
///
///				Data Input:
///					SpatialDB implements the IGW3DVectorLayerStream inteface: this is simply so we 
///					can use this same object to process stream results from some other layer and fill our data. 
///
///				Data Output:
///					SpatialDB implements IGW3DVectorPipelineCallbacks which is contains the callbacks
///					that will request data.
///
///				</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class SpatialDB : public Geoweb3d::IGW3DVectorLayerStream, public Geoweb3d::IGW3DVectorPipelineCallbacks, public Geoweb3d::IGW3DConstGeometryVisitor
{

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <param name="sdk_context">	Context for the sdk. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SpatialDB();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Destructor. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual ~SpatialDB();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	SetBounds. </summary>
	///
	/// <remarks>	IGW3DVectorPipelineCallbacks will send back requests if they intersect these
	///				bounds. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void SetBounds(Geoweb3d::GW3DEnvelope& env);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	OnStream. </summary>
	///
	/// <remarks>	IGW3DVectorLayerStream </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool OnStream(Geoweb3d::IGW3DVectorLayerStreamResult* result) override;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	OnError. </summary>
	///
	/// <remarks>	IGW3DVectorLayerStream </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool OnError() override;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection Methods. </summary>
	///
	/// <remarks>	IGW3DVectorLayerStreamFilter </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	// These are required to be implimented!  *NOTE* if
	// [count] returns 0, then no other of APIs to index
	// into selection_set_fids_ will get called.  What this
	// means is if you are streaming a whole layer, its safe to
	// return 0 for everything.

	virtual unsigned long count() const
	{
		return 0;
	}

	virtual bool next(int64_t* ppVal)
	{
		return true;
	}

	virtual void reset(){ }

	virtual int64_t operator[](unsigned long index)
	{
		return 0;
	}
	virtual int64_t get_AtIndex(unsigned long index)
	{
		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Callback Methods. </summary>
	///
	/// <remarks>	IGW3DVectorPipelineCallbacks </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual Geoweb3d::GW3DResult GW3DTHREADED_VectorPipeline_get_FeatureObjectsInBounds(const Geoweb3d::IGW3DVectorLayerWPtr layer, const Geoweb3d::GW3DEnvelope& bounds, Geoweb3d::IGW3DFeatureObjectLoader* loader) override;

	virtual Geoweb3d::GW3DResult GW3DTHREADED_VectorPipeline_remove_FeatureObjectsInBounds(const Geoweb3d::IGW3DVectorLayerWPtr layer, const Geoweb3d::GW3DEnvelope& bounds) override
	{
		return Geoweb3d::GW3D_sFalse;
	}

	virtual Geoweb3d::GW3DResult GW3DTHREADED_VectorPipeline_get_FeatureIDs(const Geoweb3d::IGW3DVectorLayerWPtr layer, Geoweb3d::IGW3DVectorLayerStreamFilter* pagedInFeaturesIds, Geoweb3d::IGW3DFeatureObjectLoader* loader) override
	{
		return Geoweb3d::GW3D_sFalse;
	}

	virtual Geoweb3d::GW3DEnvelope GW3DTHREADED_VectorPipeline_get_GetLayerBounds() override;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Database accessors </summary>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	BaseDataPtr get_DataItemRefereence(long fid);

	void get_Function(long fid, Geoweb3d::IGW3DStringPtr& str);

	void get_Name(long fid, Geoweb3d::IGW3DStringPtr& str);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	IGW3DGeometryVisitor Interface. </summary>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void visit(const Geoweb3d::GW3DPoint* pt) override;
	virtual void visit(const Geoweb3d::GW3DLineString*) override;
	virtual void visit(const Geoweb3d::GW3DLinearRing*) override;
	virtual void visit(const Geoweb3d::GW3DPolygon*) override;
	virtual void visit(const Geoweb3d::GW3DMultiPoint*)override;
	virtual void visit(const Geoweb3d::GW3DMultiLineString*)override;
	virtual void visit(const Geoweb3d::GW3DMultiPolygon*)override;

private:

	Geoweb3d::IGW3DSpatialDatabasePtr	 spatial_database_;
	std::map< uint64_t, BaseDataPtr>		data_;
	Geoweb3d::GW3DEnvelope					data_bounds_;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Used to store per feature attributes in our database </summary>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Geoweb3d::IGW3DDefinitionCollectionPtr	field_definition_;
	int name_idx_;
	int function_idx_;
};

//----Attribute Mapping---
class ExtrudedFeatureConstructionCb : public Geoweb3d::Vector::IGW3DConstructionCallback
{
public:

	ExtrudedFeatureConstructionCb() {}
	ConstructionCallbackReturnType onInsert(long feature_id, const Geoweb3d::IGW3DPropertyCollection* feature_attributes, Geoweb3d::Vector::IGW3DConstructionCallbackRepresentationSettings* rep) override;

	
};