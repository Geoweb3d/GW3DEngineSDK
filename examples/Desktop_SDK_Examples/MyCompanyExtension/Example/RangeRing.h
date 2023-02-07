#pragma once

#include "core/GW3DGUIInterFace.h"

class RangeRings
{
public:

	static RangeRings* create (const char* name, Geoweb3d::GUI::IGW3DGUIStartContext *ctx);
    ~RangeRings();

	long AddRangeRingFeature (const double& longitude, const double& latitude);

	// For the following functions, batch your changes and then call 'CommitStyleChanges'
	// (the reason is, it's more efficient to only call put_Properties once)

	int GetNumberOfAdditionalRingsPerRangeRingFeature() const;
	void SetNumberOfAdditionalRingsPerRangeRingFeature (int);

	Geoweb3d::GW3DResult GetRingInterval ( int ring, double& distance ) const;
	Geoweb3d::GW3DResult SetRingInterval ( int ring, const double& distance );

	Geoweb3d::GW3DResult GetRingColor ( int ring, double& r, double& g, double& b, double& a ) const;
	Geoweb3d::GW3DResult SetRingColor ( int ring, const double& r, const double& g, const double& b, const double& a);

	// after representation property changes have been batched, submit them to the SDK
	void CommitStyleChanges();

	// if features have been added since the representation was created, the representation must be refreshed
	// this should be done after all new features have been added in batch, for efficiency
	void RefreshRepresentation();
		
private:

    RangeRings(const char* name, Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr rep, Geoweb3d::GUI::IGW3DGUIVectorDataSourceWPtr ds);

	Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr rep_;
	Geoweb3d::GUI::IGW3DGUIVectorDataSourceWPtr ds_;

	Geoweb3d::GW3DPoint* scratch_point_;

	Geoweb3d::IGW3DPropertyCollectionPtr dirty_style_changes_;

	const Geoweb3d::IGW3DDefinitionCollection* original_default_definition_;

};
