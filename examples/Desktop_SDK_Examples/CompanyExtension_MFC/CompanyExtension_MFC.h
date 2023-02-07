// CompanyExtension_MFC.h : main header file for the CompanyExtension_MFC DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "core/GW3DGUIInterFace.h"
#include "gui/IGW3DGUISDK.h"

#include "gui/IGW3DGUIGeoweb3dEventListener.h"
#include "gui/IGW3DGUIGeoweb3dEventFilter.h"
#include "gui/IGW3DGUIApplicationUpdateTimer.h"

#ifdef BASICDESKTOPEXTENSION_EXPORTS
#define BASICDESKTOPEXTENSION_API __declspec(dllexport)
#else
#define BASICDESKTOPEXTENSION_API __declspec(dllimport)
#endif


// CCompanyExtension_MFCApp
// See CompanyExtension_MFC.cpp for the implementation of this class
//

class CCompanyExtension_MFCApp 
	: public CWinApp
	, public Geoweb3d::GUI::IGW3DGUISDK 
	, public Geoweb3d::GUI::IGW3DGUIApplicationUpdateTimer
	, public Geoweb3d::GUI::IGW3DGUIGeoweb3dVectorLayerEventListener
	, public Geoweb3d::GUI::IGW3DGUIGeoweb3dVectorRepresentationEventListener
	, public Geoweb3d::GUI::IGW3DGUIGeoweb3dTocItemEventListener
	, public Geoweb3d::GUI::IGW3DGUIGeoweb3dTocItemEventFilter
	, public Geoweb3d::GUI::IGW3DGUIGeoweb3dContextMenuEventFilter
{
public:
	CCompanyExtension_MFCApp();

	//IGW3DGUISDK implimentation
	virtual void OnFinalConstruct( Geoweb3d::GUI::IGW3DGUIFinalConstructContext *ctx );
	virtual void OnStart( Geoweb3d::GUI::IGW3DGUIStartContext *ctx );
	virtual void OnPostDraw( Geoweb3d::GUI::IGW3DGUIPostDrawContext *ctx );
	virtual void OnPreDraw( Geoweb3d::GUI::IGW3DGUIPreDrawContext *ctx );
	virtual void OnXMLRead ( Geoweb3d::GUI::IGW3DGUIXMLReadContext *ctx );
	virtual void OnXMLWrite ( Geoweb3d::GUI::IGW3DGUIXMLWriteContext *ctx );
	virtual bool OnWinEvent( MSG* message, long* result );
	virtual void OnStop( Geoweb3d::GUI::IGW3DGUIStopContext *ctx );

	//IGW3DGUIApplicationUpdateTimer implimentation
	virtual void OnApplicationUpdate ( Geoweb3d::GUI::IGW3DGUIApplicationUpdateContext *ctx );

	//IGW3DGUIGeoweb3dVectorLayerEventListener implimentation
	virtual void OnCreated (Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer);
	virtual void OnDestroyed (Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer);
	virtual void OnEnabled (Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer);
	virtual void OnDisabled (Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer);

	//IGW3DGUIGeoweb3dVectorRepresentationEventListener implimentation
	virtual void OnCreated (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr representation);
	virtual void OnDestroyed (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr representation);
	virtual void OnEnabled (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr representation);
	virtual void OnDisabled (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr representation);

	//IGW3DGUIGeoweb3dTocItemEventListener implimentation
	virtual void OnCurrent (Geoweb3d::GUI::IGW3DGUITocItemWPtr item, Geoweb3d::GUI::IGW3DGUITocItemWPtr previous);
	virtual void OnChecked (Geoweb3d::GUI::IGW3DGUITocItemWPtr item);
	virtual void OnUnchecked (Geoweb3d::GUI::IGW3DGUITocItemWPtr item);

	//IGW3DGUIGeoweb3dTocItemEventFilter implimentation
	virtual bool AllowChecked (const Geoweb3d::GUI::IGW3DGUITocItemWPtr item);
	virtual bool AllowUnchecked (const Geoweb3d::GUI::IGW3DGUITocItemWPtr item);
	virtual bool AllowMoved (const Geoweb3d::GUI::IGW3DGUITocItemWPtr item, const Geoweb3d::GUI::IGW3DGUITocItemWPtr target);

	//IGW3DGUIGeoweb3dContextMenuEventFilter implimentation
	virtual bool OnMenu (const Geoweb3d::GUI::IGW3DGUIMenuWPtr menu);
	virtual bool OnMenu (const Geoweb3d::GUI::IGW3DGUIMenuWPtr menu, Geoweb3d::GUI::IGW3DGUITocItemWPtr target);

	CWnd pParentWnd;
	CButton MyWindow;
// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
