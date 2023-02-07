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
#include <Geoweb3dCore/Geoweb3dExports.h>
#include "GW3DInterfaceUtility.h"
#include "IGW3DIBaseObject.h"

/* Primary namespace */
namespace Geoweb3d
{

class GW3DPoint;
class GW3DEnvelope;
struct IGW3DCameraOffset;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Driver base class. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DDriver;

struct   IGW3DFilter;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Client User Data base class. </summary>
///
/// <remarks>	An interface inheriting from IGW3DClientUserData allows a developer to store
/// 			their own pointer in the object. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class IGW3DClientUserData;
_GW3D_SMARTPTR_TYPEDEF( IGW3DClientUserData );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DClientUserData );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A token. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct   IGW3DToken;
_GW3D_SMARTPTR_TYPEDEF(IGW3DToken);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Image finalization token. </summary>
///
/// <remarks>	A token containing information about a palette of images. </remarks>
///
/// <see cref="Geoweb3d::IGW3DImage"/>
/// <see cref="Geoweb3d::IGW3DImageCollection::create_FinalizeToken"/>
/// <see cref="Geoweb3d::IGW3DVectorRepresentation::put_GW3DFinalizationToken"/>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct   IGW3DFinalizationToken;
_GW3D_SMARTPTR_TYPEDEF(IGW3DFinalizationToken);

/// //////////////////////////////////////////////////////////////////////////////////////////
/// <summary> OpenGL Texture finalization token.</summary>
///
/// <remarks> A token containing information about a palette of
///           OpenGL textures.</remarks>
///
/// <see cref="Geoweb3d::IGW3DImage"/>
/// <see cref="Geoweb3d::IGW3DOpenGLTextureCollection::create_FinalizeToken"/>
/// <see cref="Geoweb3d::IGW3DVectorRepresentation::put_GW3DFinalizationToken"/>
/// //////////////////////////////////////////////////////////////////////////////////////////

struct   IGW3DOpenGLTextureFinalizationToken;
_GW3D_SMARTPTR_TYPEDEF(IGW3DOpenGLTextureFinalizationToken);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Color Lookup Table finalization token. </summary>
///
/// <remarks>	A token containing information about a color lookup table. </remarks>
///
/// <see cref="Geoweb3d::IGW3DColor"/>
/// <see cref="Geoweb3d::IGW3DColorCollection"/>
/// <see cref="Geoweb3d::IGW3DColorLookupTable::create_FinalizeToken"/>
/// <see cref="Geoweb3d::IGW3DViewshedTest::put_GW3DFinalizationToken"/>
/// <see cref="Geoweb3d::IGW3DTerrainAvoidanceTest::put_GW3DFinalizationToken"/>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DRasterLayerImageCollectionFinalizationToken;
_GW3D_SMARTPTR_TYPEDEF(IGW3DRasterLayerImageCollectionFinalizationToken);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DRasterLayerImageCollectionFinalizationToken);

struct   IGW3DColorLookupTableFinalizationToken;
_GW3D_SMARTPTR_TYPEDEF(IGW3DColorLookupTableFinalizationToken);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Layer base class. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct   IGW3DLayer;
_GW3D_SMARTPTR_TYPEDEF(IGW3DLayer);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DLayer);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Values that represent the window coordinate origin. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

enum WindowCoordinateOrigin
{
    /// <summary>	Lower Left - i.e. OpenGL. </summary>
    LOWER_LEFT = 0,
    /// <summary>	Upper Left - i.e. GDI, DirectX, X11. </summary>
    UPPER_LEFT = 1,
    /// <summary>	Do Not Use. </summary>
    WindowCoordinateOriginMAX = 0xFFFF
};

//## Vector
class GW3DGeometry;
_GW3D_SMARTPTR_TYPEDEF( GW3DGeometry );

class GW3DLinearRing;
_GW3D_SMARTPTR_TYPEDEF( GW3DLinearRing );


class GW3DLineString;
_GW3D_SMARTPTR_TYPEDEF( GW3DLineString );

struct   IGW3DString;
_GW3D_SMARTPTR_TYPEDEF( IGW3DString );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DString );

struct   IGW3DStringCollection;
_GW3D_SMARTPTR_TYPEDEF( IGW3DStringCollection );

struct   IGW3DDataBuffer;
_GW3D_SMARTPTR_TYPEDEF( IGW3DDataBuffer );


struct   IGW3DSpline;
_GW3D_SMARTPTR_TYPEDEF( IGW3DSpline );

struct   IGW3DAnimation;
_GW3D_SMARTPTR_TYPEDEF( IGW3DAnimation );

struct   IGW3DVectorDriver;
struct   IGW3DVectorDriverCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DVectorDriver );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DVectorDriverCollection );

struct   IGW3DTask;
_GW3D_SMARTPTR_TYPEDEF(IGW3DTask);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DTask);

struct IGW3DContourUtility;
struct IGW3DDataSourceCompositor;
struct   IGW3DLayerHelperCollection;
_GW3D_SMARTPTR_TYPEDEF(IGW3DLayerHelperCollection);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DLayerHelperCollection);

struct   IGW3DVectorLayer;
struct   IGW3DVectorLayerCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DVectorLayer );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DVectorLayerCollection );
struct   IGW3DVectorPipelineCallbacks;
struct   IGW3DGeofencePipelineCallbacks;
struct   IGW3DVectorDataSource;
struct   IGW3DVectorDataSourceCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DVectorDataSource );
_GW3D_SMARTPTR_TYPEDEF( IGW3DVectorDataSource );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DVectorDataSourceCollection );

struct   IGW3DVectorDataSourceSummary;
_GW3D_SMARTPTR_TYPEDEF( IGW3DVectorDataSourceSummary );

struct   IGW3DPropertyCollection;

struct   IGW3D2DDrawContext;
struct   IGW3D3DDrawContext;
struct   IGW3DRenderPassDetails;

_GW3D_SMARTPTR_TYPEDEF(IGW3DPropertyCollection);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DPropertyCollection);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Defines an alias for IGW3DPropertyCollection representing a collection of attributes. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

typedef IGW3DPropertyCollection IGW3DAttributeCollection;
_GW3D_SMARTPTR_TYPEDEF(IGW3DAttributeCollection);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DAttributeCollection);

struct   IGW3DDefinitionCollection;
_GW3D_SMARTPTR_TYPEDEF( IGW3DDefinitionCollection );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DDefinitionCollection );

struct   IGW3DIdCollection;
_GW3D_SMARTPTR_TYPEDEF( IGW3DIdCollection );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DIdCollection );

struct   IGW3DVectorLayerStream;
struct   IGW3DVectorLayerStreamResult;

struct   IGW3DLayerCollectionAutoRepresenter;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DLayerCollectionAutoRepresenter );

//## raster
struct   IGW3DRasterDriverCollection;
struct   IGW3DRasterDriver;

struct   IGW3DRasterBandCollection;
struct   IGW3DRasterBand;

struct   IGW3DRasterRepresentationDriverCollection;
struct   IGW3DRasterRepresentationDriver;

struct IGW3DAsyncStatus;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DAsyncStatus);
_GW3D_SMARTPTR_TYPEDEF(IGW3DAsyncStatus);

struct IG3DRasterServiceConfiguration;
_GW3D_SMARTPTR_TYPEDEF(IG3DRasterServiceConfiguration);

struct   IGW3DFont;
_GW3D_SMARTPTR_TYPEDEF( IGW3DFont );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DFont );
struct   IGW3DLatitudeLongitudeGrid;

struct   IGW3DRasterRepresentation;

struct   IGW3DRasterDataSourceCollection;
struct   IGW3DRasterDataSource;
struct   IGW3DRasterPipelineCallbacks;

_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DRasterRepresentation );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DRasterDriver );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DRasterDataSource );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DRasterRepresentationDriver );

_GW3D_SMARTPTR_TYPEDEF( IGW3DRasterBand );

struct   IGW3DRasterRepresentationCollection;
_GW3D_SMARTPTR_TYPEDEF( IGW3DRasterRepresentationCollection );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DRasterRepresentationCollection );

struct   IGW3DRasterLayer;
struct   IGW3DRasterLayerCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DRasterLayer);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DRasterLayerCollection);

struct IGW3DRasterTimeSlice;
_GW3D_SMARTPTR_TYPEDEF(IGW3DRasterTimeSlice);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DRasterTimeSlice);

struct IGW3DRasterTimeSliceCollection;
_GW3D_SMARTPTR_TYPEDEF(IGW3DRasterTimeSliceCollection);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DRasterTimeSliceCollection);

struct IGW3DRasterTimeControllerCallback;

struct IGW3DRasterTimeController;
_GW3D_SMARTPTR_TYPEDEF(IGW3DRasterTimeController);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DRasterTimeController);

struct IGW3DPlanetRasterTimePlayer;
_GW3D_SMARTPTR_TYPEDEF(IGW3DPlanetRasterTimePlayer);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DPlanetRasterTimePlayer);

struct IGW3DRasterLayerEnvelopeStreamResult;
struct   IGW3DRasterLayerEnvelopeStream;
_GW3D_SMARTPTR_TYPEDEF(IGW3DRasterLayerEnvelopeStream);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DRasterLayerEnvelopeStream);
//## Monitor/Display System

struct   IGW3DOverviewConfiguration;
_GW3D_SMARTPTR_TYPEDEF(IGW3DOverviewConfiguration);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DOverviewConfiguration);

struct   IGW3DDisplayMode;
struct   IGW3DDisplayDetails;
struct   IGW3DDisplayDevices;
struct   IGW3DDisplayModeTest;
struct   IGW3DDisplayModeQueryFilter;

_GW3D_SMARTPTR_TYPEDEF( IGW3DDisplayMode );
_GW3D_SMARTPTR_TYPEDEF( IGW3DDisplayDetails );
_GW3D_SMARTPTR_TYPEDEF( IGW3DDisplayModeTest );

_GW3D_SMARTPTR_TYPEDEF( IGW3DDisplayModeQueryFilter );

//struct   IGW3DSymbolProperties;
//_GW3D_SMARTPTR_TYPEDEF( IGW3DSymbolProperties );

struct   IGW3DColor;
_GW3D_SMARTPTR_TYPEDEF( IGW3DColor );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DColor );

struct   IGW3DColorCollection;
_GW3D_SMARTPTR_TYPEDEF(IGW3DColorCollection);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DColorCollection);

struct   IGW3DDisplayModeCollection;

struct   IGW3DBearingHelper;
_GW3D_SMARTPTR_TYPEDEF( IGW3DBearingHelper );

struct   IGW3DAnimationPath;
_GW3D_SMARTPTR_TYPEDEF( IGW3DAnimationPath );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DAnimationPath );

struct   IGW3DAnimationPathPlayback;
_GW3D_SMARTPTR_TYPEDEF( IGW3DAnimationPathPlayback );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DAnimationPathPlayback );

struct   IGW3DCamera;
struct   IGW3DCameraCollection;
struct   IGW3DCameraStereoSettings;
struct   IGW3DCameraOffset;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DCamera );

struct   IGW3DPositionOrientation;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DPositionOrientation);

struct   IGW3DCameraController;
struct   IGW3DCameraThrowSettings;
struct   IGW3DEnvironmentEffects;
struct   IGW3DDateTime;

struct   IGW3DWindowCoordinateToGeodeticQuery;
struct   IGW3DWindowCoordinateToGeodeticQueryCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DWindowCoordinateToGeodeticQuery );

struct   IGW3DEnvironmentLabs; //## experimental API.

struct   IGW3DLightingConfiguration;
_GW3D_SMARTPTR_TYPEDEF( IGW3DLightingConfiguration );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DLightingConfiguration );

//##LineSegment Intersection
struct   IGW3DLineSegmentIntersectionReportCollection;
struct   IGW3DLineSegmentIntersectionReport;
struct   IGW3DLineSegmentIntersectionDetailCollection;
struct   IGW3DLineSegmentIntersectionDetail;
struct   IGW3DLineSegmentIntersectionTestResultsCollection;

struct   IGW3DLineSegmentIntersectionTestCollection;
struct   IGW3DLineSegmentIntersectionTest;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DLineSegmentIntersectionTest );


struct   IGW3DViewshedTest;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DViewshedTest );

struct   IGW3DPerspectiveViewshedTestCollection;
struct   IGW3DPerspectiveViewshedTest;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DPerspectiveViewshedTest );

struct   IGW3DPanoramicViewshedTestCollection;
struct   IGW3DPanoramicViewshedTest;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DPanoramicViewshedTest );

struct   IGW3DSlopeAnalysisCollection;
struct   IGW3DSlopeAnalysis;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DSlopeAnalysis);


struct   IGW3DViewshedReportCollection;
struct   IGW3DViewshedReport;

struct   IGW3DTerrainAvoidanceTestCollection;
struct   IGW3DTerrainAvoidanceTest;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DTerrainAvoidanceTest);

struct   IGW3DVisibilityDomeTestCollection;
struct   IGW3DVisibilityDomeTest;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DVisibilityDomeTest);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Defines an alias for IGW3DPropertyCollection representing a collection of stereo configurations. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

typedef IGW3DPropertyCollection IGW3DStereoConfigurationCollection;

struct   IGW3DViewport;

struct   IGW3DFrustum; //maybe make a geometry type?
struct   IGW3DViewshedFrustum;

struct   IGW3DVectorLayerFieldFilter;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DVectorLayerFieldFilter );
_GW3D_SMARTPTR_TYPEDEF( IGW3DVectorLayerFieldFilter );

struct   IGW3DAnalysis;
struct   IGW3DFrustumAnalysis;
struct   IGW3DFrameAnalysisContentsConfiguration; 
struct   IGW3DFeatureFeedbackConfiguration;
struct   IGW3DFeatureID2DPixelLocationConfiguration;
struct   IGW3DContentsFilter;

struct   IGW3DFrustumAnalysisStream;
struct   IGW3DFeatureID2DPixelLocationResult;
struct   IGW3DFeatureFeedbackResult;
struct   IGW3DFeatureFeedbackResultCollection;
struct   IGW3DContentsResult;

struct   IGW3DClampRadius;
struct   IGW3DFeatureIdFilter;

//## Windowing
struct   IGW3DWindow;
struct   IGW3DWindowCollection;
struct   IGW3DWindowCallback;
struct   IGW3DStereoDriver;
struct   IGW3DStereoDriverCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DWindow );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DStereoDriver );
_GW3D_SMARTPTR_TYPEDEF(IGW3DStereoDriver);

struct WebViewSourceHandle_;
_GW3D_SMARTPTR_TYPEDEF(WebViewSourceHandle_);
typedef WebViewSourceHandle_Ptr WebViewSourceHandle;

struct   IGW3DStereoDevice;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DStereoDevice );
_GW3D_SMARTPTR_TYPEDEF(IGW3DStereoDevice);

//##GeoFencing
struct   IGW3DGeoFence;
struct   IGW3DGeoFenceCollection;
struct   IGW3DGeoFenceIntruder;
struct   IGW3DGeoFenceIntruderCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DGeoFence);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DGeoFenceIntruder);

//## Spatial Databse
struct   IGW3DSpatialDatabase;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DSpatialDatabase);
_GW3D_SMARTPTR_TYPEDEF(IGW3DSpatialDatabase);


//## Smart Cache Database
struct   IGW3DSmartCacheDatabase;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DSmartCacheDatabase);
_GW3D_SMARTPTR_TYPEDEF(IGW3DSmartCacheDatabase);

//## Smart Cache Scoped Record
struct   IGW3DSmartCacheScopedRecord;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DSmartCacheScopedRecord);
_GW3D_SMARTPTR_TYPEDEF(IGW3DSmartCacheScopedRecord);


struct   IGW3DSceneGraphContext;
struct   IGW3DImage;
struct   IGW3DImageCollection;
_GW3D_SMARTPTR_TYPEDEF(IGW3DImage);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DImage);
_GW3D_SMARTPTR_TYPEDEF(IGW3DImageCollection);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DImageCollection);

struct   IGW3DOpenGLTexture;
struct   IGW3DOpenGLTextureCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DOpenGLTexture);
_GW3D_SMARTPTR_TYPEDEF(IGW3DOpenGLTextureCollection);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DOpenGLTextureCollection);

struct IGW3DRasterLayerImageCollection;
_GW3D_SMARTPTR_TYPEDEF(IGW3DRasterLayerImageCollection);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DRasterLayerImageCollection);

struct   IGW3DColorLookupTable;
_GW3D_SMARTPTR_TYPEDEF(IGW3DColorLookupTable);
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DColorLookupTable);

//## Representations
struct   IGW3DVectorRepresentationDriver;
struct   IGW3DVectorRepresentationDriverCollection;

_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DVectorRepresentationDriver );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DVectorRepresentationDriverCollection );

struct   IGW3DVectorRepresentationCollection;
struct   IGW3DVectorRepresentationHelperCollection;
struct   IGW3DVectorRepresentationCollectionViewer;
namespace Vector
{
	struct   IGW3DLODPagingPolicy;
}
_GW3D_SMARTPTR_TYPEDEF(IGW3DVectorRepresentationHelperCollection);

struct   IGW3DVectorRepresentation;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DVectorRepresentation );

//## System Utilities
struct   IGW3DEventStream;

struct   IGW3DTimer;
_GW3D_SMARTPTR_TYPEDEF( IGW3DTimer );

struct   IGW3DSpatialReference;
_GW3D_SMARTPTR_TYPEDEF( IGW3DSpatialReference );

struct   IGW3DCoordinateTransformation;
_GW3D_SMARTPTR_TYPEDEF(IGW3DCoordinateTransformation);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	SDK and Bridge Contexts </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct   IGW3DGeoweb3dSDK;
_GW3D_SMARTPTR_TYPEDEF( IGW3DGeoweb3dSDK );

struct   IGW3DIntegrationBridgeArbiter;
_GW3D_SMARTPTR_TYPEDEF(IGW3DIntegrationBridgeArbiter);

struct   IGW3DIntegrationBridgeClient;
_GW3D_SMARTPTR_TYPEDEF(IGW3DIntegrationBridgeClient);

struct   IGW3DSDKContext;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DSDKContext);

struct   IGW3DSDKVectorContext;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DSDKVectorContext);

struct   IGW3DSDKRasterContext;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DSDKRasterContext);

struct   IGW3DBridgedWindowCommunicator;
struct   IGW3DBridgedWindow;
struct   IGW3DBridgedCamera;
struct   IGW3DBridgedWindowCallbacks;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct   IGW3DInitializationConfiguration;
_GW3D_SMARTPTR_TYPEDEF( IGW3DInitializationConfiguration );

struct   IGW3DBuffer;
struct   IGW3DMediaCenter;
struct   IGW3DMediaCenterTask;
_GW3D_SMARTPTR_TYPEDEF( IGW3DMediaCenterTask );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Callback base class. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct   IGW3DCallback {};

struct   IGW3DGeoweb3dFusionSDK;
struct   IGW3DDataChannel;
_GW3D_SMARTPTR_TYPEDEF( IGW3DDataChannel );
_GW3D_SMARTPTR_TYPEDEF( IGW3DGeoweb3dFusionSDK );
struct   IGW3DCameraDescriptionCollection;
struct   IGW3DCameraDescription;

class IGW3DVariant;
_GW3D_SMARTPTR_TYPEDEF( IGW3DVariant );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DVariant );


struct   IGW3DCodecDriverCollection ;
struct   IGW3DCodecDriver;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DCodecDriver );

struct IPCameraSourceHandle_;
_GW3D_SMARTPTR_TYPEDEF(IPCameraSourceHandle_);
typedef IPCameraSourceHandle_Ptr IPCameraSourceHandle;

//## The GUI
struct   IGW3DGeoweb3dDesktopSDK;
_GW3D_SMARTPTR_TYPEDEF( IGW3DGeoweb3dDesktopSDK );

typedef unsigned int GW3DCullMask;

struct   IGW3DSceneLoadingCallbacks;
struct   IGW3DSceneCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DSceneCollection);

struct   IGW3DScene;
_GW3D_WEAK_SMARTPTR_TYPEDEF(IGW3DScene);

struct   IGW3DAnimationController;

}
extern "C" GW3D_DLL void* GW3DNew(size_t size);
extern "C" GW3D_DLL void GW3DDelete(void* p);
