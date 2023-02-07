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
#include "GW3DCore.h"
#include "GW3DInterface.h"

/* Primary namespace */
namespace Geoweb3d
{
namespace GUI
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	The driver base class. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DGUIDriver
{

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	The layer base class. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DGUILayer
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets the underlying type of the layer. </summary>
    ///
    /// <returns>	the type. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual const char * get_Type() = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets the underlying name of the layer. </summary>
    ///
    /// <returns>	the name. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual const char * get_Name() = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets the geographic envelope of the layer. </summary>
    ///
    /// <returns>	The envelope. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual GW3DEnvelope get_Envelope() = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the name of the datasource the layer is apart of if one exists. </summary>
	///
	/// <returns>	the datasource name. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual const char* get_DataSourceName() = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Values that represent the Geoweb3d file types. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

enum Geoweb3dFileType
{
    /// <summary>	An enum constant representing the project file type. </summary>
    Geoweb3dFileTypeProject = 0,
    /// <summary>	An enum constant representing the layer file type. </summary>
    Geoweb3dFileTypeLayer,
    /// <summary>	An enum constant representing the representation file type. </summary>
    Geoweb3dFileTypeRepresentation,
    /// <summary>	Do Not Use.. </summary>
    Geoweb3dFileType_MAX = 0xff
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A collection of feature / object IDs. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DGUIFidCollection : IGW3DCollection< int64_t > {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A modifiable collection of feature / object IDs. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DGUIFidModifiableCollection : IGW3DGUIFidCollection
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Adds a feature/object id to the collection. </summary>
    ///
    /// <param name="parameter1">	The id to add. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void add (int64_t fid) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Removes a feature/object id to the collection. </summary>
    ///
    /// <param name="parameter1">	The id to remove. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void remove (int64_t fid) = 0;
};

struct IGW3DGUISDK;
_GW3D_SMARTPTR_TYPEDEF( IGW3DGUISDK );

struct IGW3DGUIDockWindow;
struct IGW3DGUIDockWindowCollection;
struct IGW3DGUIDockWindowModifiableCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIDockWindow );

struct IGW3DGUIVectorDriver;
struct IGW3DGUIVectorDriverCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIVectorDriver );

struct IGW3DGUIVectorDataSource;
struct IGW3DGUIVectorDataSourceCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIVectorDataSource );

struct IGW3DGUIVectorLayerStream;
struct IGW3DGUIVectorLayerStreamResult;

struct IGW3DGUIVectorLayer;
struct IGW3DGUIVectorLayerPreview;
struct IGW3DGUIVectorLayerCollection;
struct IGW3DGUIVectorLayerPreviewCollection;
_GW3D_SMARTPTR_TYPEDEF( IGW3DGUIVectorLayerPreview );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIVectorLayer );

struct IGW3DGUIClampRadius;

struct IGW3DGUIVectorRepresentation;
struct IGW3DGUIVectorRepresentationBasic;
struct IGW3DGUIVectorRepresentationCollection;
struct IGW3DGUIVectorRepresentationHelperCollection;
struct IGW3DGUIVectorRepresentationBasicHelperCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIVectorRepresentation );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIVectorRepresentationBasic );

struct IGW3DGUIVectorRepresentationDriver;
struct IGW3DGUIVectorRepresentationDriverCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIVectorRepresentationDriver );

struct IGW3DGUIRasterRepresentationDriver;
struct IGW3DGUIRasterRepresentationDriverCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIRasterRepresentationDriver );

struct IGW3DGUIAttributeMap;
struct IGW3DGUIAttributeMapCollection;
struct IGW3DGUIAttributeMapRootCollection;
struct IGW3DGUIAllFeaturesAttributeMap;
struct IGW3DGUICategoryGroupAttributeMap;
struct IGW3DGUICategoryAttributeMap;
struct IGW3DGUIOneToOneAttributeMap;
struct IGW3DGUISelectionSetAttributeMap;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIAttributeMap );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIAllFeaturesAttributeMap );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUICategoryGroupAttributeMap );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUICategoryAttributeMap );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIOneToOneAttributeMap );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUISelectionSetAttributeMap );

struct IGW3DGUIFrame;
struct IGW3DGUIFrameCollection;
struct IGW3DGUIShelf;
struct IGW3DGUIShelfCollection;
struct IGW3DGUIShelfModifiableCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIFrame );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIShelf );

struct IGW3DGUITocContext;
struct IGW3DGUITocItemCollection;
struct IGW3DGUITocItem;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUITocItem );

struct IGW3DGUIView3d;
struct IGW3DGUIViewWeb;
struct IGW3DGUIViewArc;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIView3d );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIViewWeb );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIViewArc );

struct IGW3DGUIView3dCollection;
struct IGW3DGUIViewWebCollection;

struct IGW3DGUIStatusOverlay;

struct IGW3DGUILineSegmentIntersectionTest;
struct IGW3DGUILineSegmentIntersectionTestCollection;
struct IGW3DGUILineSegmentIntersectionReportCollection;
struct IGW3DGUILineSegmentIntersectionReport;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUILineSegmentIntersectionTest );

struct IGW3DGUI2DIntersectionTest;
struct IGW3DGUI2DIntersectionTestCollection;
struct IGW3DGUI2DIntersectionCollection;
struct IGW3DGUI2DIntersection;
struct IGW3DGUI2DIntersectionDetailCollection;
struct IGW3DGUI2DIntersectionDetail;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUI2DIntersectionTest );

struct IGW3DGUIPreDrawContext;
struct IGW3DGUIPostDrawContext;
struct IGW3DGUIXMLReadContext;
struct IGW3DGUIXMLWriteContext;
struct IGW3DGUIStartContext;
struct IGW3DGUIStopContext;
struct IGW3DGUIFinalConstructContext;
struct IGW3DGUIApplicationUpdateContext;

struct IGW3DGUIApplicationUpdateTimer;

struct IGW3DGUIRebrandingToken;

struct IGW3DGUIApplicationRebrandingContext;
struct IGW3DGUIGeoweb3dEventContext;

struct IGW3DGUIGeoweb3dVectorLayerEventListener;
struct IGW3DGUIGeoweb3dVectorRepresentationEventListener;
struct IGW3DGUIGeoweb3dAttributeMapEventListener;
struct IGW3DGUIGeoweb3dTocItemEventListener;

struct IGW3DGUIGeoweb3dTocItemEventFilter;
struct IGW3DGUIGeoweb3dContextMenuEventFilter;
struct IGW3DGUIGeoweb3dDialogEventFilter;

struct IGW3DGUIMenu;
struct IGW3DGUIMenuItem;
struct IGW3DGUIMenuItemCollection;
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIMenu );
_GW3D_WEAK_SMARTPTR_TYPEDEF( IGW3DGUIMenuItem );


extern "C" GW3D_DLL void close_GUIVectorRepresentation( IGW3DGUIVectorRepresentationWPtr rep );
extern "C" GW3D_DLL void close_GUIVectorRepresentationBasic( IGW3DGUIVectorRepresentationBasicWPtr rep );
extern "C" GW3D_DLL void close_GUIVectorLayer( IGW3DGUIVectorLayerWPtr rep );

}
}