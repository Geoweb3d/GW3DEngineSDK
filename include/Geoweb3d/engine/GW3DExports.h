//////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// Geoweb3d SDK license under which the SDK provided.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Geoweb3d/core/GW3DInterFace.h>

extern "C" GW3D_DLL void close_RasterDataSource(Geoweb3d::IGW3DRasterDataSourceWPtr& ds);
extern "C" GW3D_DLL void close_RasterRepresentation(Geoweb3d::IGW3DRasterRepresentationWPtr& rep);
extern "C" GW3D_DLL void close_VectorDataSource(Geoweb3d::IGW3DVectorDataSourceWPtr& ds);
extern "C" GW3D_DLL void close_VectorRepresentation(Geoweb3d::IGW3DVectorRepresentationWPtr& rep);
extern "C" GW3D_DLL void close_Window(Geoweb3d::IGW3DWindowWPtr& window);
extern "C" GW3D_DLL void close_Camera(Geoweb3d::IGW3DCameraWPtr& camera);
extern "C" GW3D_DLL void close_GeoFence(Geoweb3d::IGW3DGeoFenceWPtr& geo_fence);
extern "C" GW3D_DLL void close_GeoFenceIntruder(Geoweb3d::IGW3DGeoFenceIntruderWPtr& geo_fence_intruder);
extern "C" GW3D_DLL void close_Query(Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr& query);
extern "C" GW3D_DLL void close_LineSegmentIntersectionTest(Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr& ltest);
extern "C" GW3D_DLL void close_PerspectiveViewshedTest(Geoweb3d::IGW3DPerspectiveViewshedTestWPtr& p);
extern "C" GW3D_DLL void close_PanoramicViewshedTest(Geoweb3d::IGW3DPanoramicViewshedTestWPtr& p);
extern "C" GW3D_DLL void close_SlopeAnalysis(Geoweb3d::IGW3DSlopeAnalysisWPtr& p);
extern "C" GW3D_DLL void close_TerrainAvoidanceTest(Geoweb3d::IGW3DTerrainAvoidanceTestWPtr& p);
extern "C" GW3D_DLL void close_VisibilityDomeTest(Geoweb3d::IGW3DVisibilityDomeTestWPtr& p);
