// CompanyExtension_MFC.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CompanyExtension_MFC.h"
#include "version.h"

#include "core/GW3DGuids.h"
#include "gui/GW3DGUICommon.h"
#include "gui/GW3DGUICustomization.h"
#include "gui/GW3DGUITableOfContents.h"
#include "gui/GW3DGUIVector.h"

#include "gui/IGW3DGUIApplicationUpdateContext.h"

#include "engine/IGW3DRegistration.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


// CCompanyExtension_MFCApp

BEGIN_MESSAGE_MAP(CCompanyExtension_MFCApp, CWinApp)
END_MESSAGE_MAP()


// CCompanyExtension_MFCApp construction

CCompanyExtension_MFCApp::CCompanyExtension_MFCApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

void CCompanyExtension_MFCApp::OnPreDraw( Geoweb3d::GUI::IGW3DGUIPreDrawContext *ctx )
{
	
} 

void CCompanyExtension_MFCApp::OnFinalConstruct( Geoweb3d::GUI::IGW3DGUIFinalConstructContext *ctx )
{
	if (!Geoweb3d::Succeeded(ctx->put_ApplicationUpdateTimer(this, 500)))
	{
		//error
	}
 
	ctx->get_Geoweb3dEventContext();

	if (!Geoweb3d::Succeeded(ctx->get_Geoweb3dEventContext()->put_VectorLayerEventListener(this)))
	{
		//error
	}
	if (!Geoweb3d::Succeeded(ctx->get_Geoweb3dEventContext()->put_VectorRepresentationEventListener(this)))
	{
		//error
	}
	if (!Geoweb3d::Succeeded(ctx->get_Geoweb3dEventContext()->put_TocItemEventListener(this)))
	{
		//error
	}

	if (!Geoweb3d::Succeeded(ctx->get_Geoweb3dEventContext()->put_TocItemEventFilter(this)))
	{
		//error
	}

	if (!Geoweb3d::Succeeded(ctx->get_Geoweb3dEventContext()->put_ContextMenuEventFilter(this)))
	{
		//error
	}
}
		
void CCompanyExtension_MFCApp::OnStart( Geoweb3d::GUI::IGW3DGUIStartContext *ctx )
{
	Geoweb3d::GUI::IGW3DGUIDockWindowWPtr doc( ctx->get_DockWindowModifiableCollection()->create("Example Dockwindow", "MyCompanyDockWindow1", Geoweb3d::GUI::IGW3DGUIDockWindowModifiableCollection::RightDockArea) );

	pParentWnd.Attach((HWND)doc.lock()->get_AttachHandle());
	
	int top = 10;
	int bottom = 30;
    DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
    MyWindow.Create(_T("Buton"), dwStyle,CRect(10,top,100,bottom ),&pParentWnd, 100);
	doc.lock()->put_Visible(true);
}
void CCompanyExtension_MFCApp::OnStop( Geoweb3d::GUI::IGW3DGUIStopContext *ctx )
{
	MyWindow.DestroyWindow();
	pParentWnd.DestroyWindow();

}

void CCompanyExtension_MFCApp::OnPostDraw( Geoweb3d::GUI::IGW3DGUIPostDrawContext *ctx )
{

}

void CCompanyExtension_MFCApp::OnXMLRead ( Geoweb3d::GUI::IGW3DGUIXMLReadContext *ctx )
{
	/*
	 *  Do not store the char* of the XML buffer (ctx->get_XML ()).  It will be null 
	 *  after the return of this function.  Process inline or copy the buffer.
	 *	If no custom data has been stored, this will be null!
	 */
}

void CCompanyExtension_MFCApp::OnXMLWrite ( Geoweb3d::GUI::IGW3DGUIXMLWriteContext *ctx )
{

}

/*
 * Windows messages for mouse and keyboard events.  Return true to propogate event to Geoweb3d.
 */
bool CCompanyExtension_MFCApp::OnWinEvent( MSG* message, long* result )
{
	UINT imsg = message->message;
	WPARAM wp = message->wParam;
	LPARAM lp = message->lParam;

	switch (imsg) {
		case WM_MOUSEMOVE:
			break;
		case WM_KEYUP:
			printf("key up = %c", wp);
			break;
		case WM_KEYDOWN:
			printf("key down = %c", wp);
			break;
		case WM_LBUTTONDOWN:
			printf("left button down");
			break;
		case WM_LBUTTONUP:
			printf("left button up"); 
			break;
		case WM_RBUTTONDOWN:
			printf("right button down");
			break;
		case WM_RBUTTONUP:
			printf("right button up");
			break;
		default:
			break;
	};

	return true; //propegate
}

void CCompanyExtension_MFCApp::OnApplicationUpdate ( Geoweb3d::GUI::IGW3DGUIApplicationUpdateContext *ctx )
{
	Geoweb3d::GUI::IGW3DGUIView3dCollection* views3d = ctx->get_View3dCollection();

	Geoweb3d::GUI::IGW3DGUIViewWebCollection* viewsWeb = ctx->get_ViewWebCollection();
}

void CCompanyExtension_MFCApp::OnCreated (Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer)
{
	//IGW3DGUIGeoweb3dVectorLayerEventListener
	printf("Layer '%s' was created.\n", layer.lock()->get_Name());
}

void CCompanyExtension_MFCApp::OnDestroyed (Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer)
{
	//IGW3DGUIGeoweb3dVectorLayerEventListener
	printf("Layer '%s' was destroyed.\n", layer.lock()->get_Name());
}

void CCompanyExtension_MFCApp::OnEnabled (Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer)
{
	//IGW3DGUIGeoweb3dVectorLayerEventListener
	printf("Layer '%s' was enabled.\n", layer.lock()->get_Name());
}

void CCompanyExtension_MFCApp::OnDisabled (Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer)
{
	//IGW3DGUIGeoweb3dVectorLayerEventListener
	printf("Layer '%s' was disabled.\n", layer.lock()->get_Name());
}

void CCompanyExtension_MFCApp::OnCreated (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr representation)
{
	//IGW3DGUIGeoweb3dVectorRepresentationEventListener
	printf("Representation of layer '%s' was created.\n", representation.lock()->get_VectorLayer().lock()->get_Name());

}

void CCompanyExtension_MFCApp::OnDestroyed (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr representation)
{
	//IGW3DGUIGeoweb3dVectorRepresentationEventListener
	printf("Representation of layer '%s' was destroyed.\n", representation.lock()->get_VectorLayer().lock()->get_Name());
}

void CCompanyExtension_MFCApp::OnEnabled (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr representation)
{
	//IGW3DGUIGeoweb3dVectorRepresentationEventListener
	printf("Representation of layer '%s' was enabled.\n", representation.lock()->get_VectorLayer().lock()->get_Name());
}

void CCompanyExtension_MFCApp::OnDisabled (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr representation)
{
	//IGW3DGUIGeoweb3dVectorRepresentationEventListener
	printf("Representation of layer '%s' was disabled.\n", representation.lock()->get_VectorLayer().lock()->get_Name());
}

void CCompanyExtension_MFCApp::OnCurrent (Geoweb3d::GUI::IGW3DGUITocItemWPtr item, Geoweb3d::GUI::IGW3DGUITocItemWPtr previous)
{
	//IGW3DGUIGeoweb3dTocItemEventListener
	if (!item.expired())
	{
		printf("TOC Item '%s' is now current!", item.lock()->get_Name());
	}
	else
	{
		printf("No TOC Item is now current!");
	}
}

void CCompanyExtension_MFCApp::OnChecked (Geoweb3d::GUI::IGW3DGUITocItemWPtr item)
{
	//IGW3DGUIGeoweb3dTocItemEventListener
	printf("TOC Item '%s' is now checked!", item.lock()->get_Name());
}

void CCompanyExtension_MFCApp::OnUnchecked (Geoweb3d::GUI::IGW3DGUITocItemWPtr item)
{
	//IGW3DGUIGeoweb3dTocItemEventListener
	printf("TOC Item '%s' is now unchecked!", item.lock()->get_Name());
}

bool CCompanyExtension_MFCApp::AllowChecked (const Geoweb3d::GUI::IGW3DGUITocItemWPtr item)
{
	//IGW3DGUIGeoweb3dTocItemEventFilter
	printf("TOC Item '%s' allowed to be checked!", item.lock()->get_Name());
	return true;
}	

bool CCompanyExtension_MFCApp::AllowUnchecked (const Geoweb3d::GUI::IGW3DGUITocItemWPtr item)
{
	//IGW3DGUIGeoweb3dTocItemEventFilter
	printf("TOC Item '%s' allowed to be unchecked!", item.lock()->get_Name());
	return true;
}

bool CCompanyExtension_MFCApp::AllowMoved (const Geoweb3d::GUI::IGW3DGUITocItemWPtr item, const Geoweb3d::GUI::IGW3DGUITocItemWPtr target)
{
	const char* target_name = target.expired() ? "NULL" : target.lock()->get_Name();
	//IGW3DGUIGeoweb3dTocItemEventFilter
	printf("TOC Item '%s' allowed to be moved to target '%s'!", item.lock()->get_Name(), target_name);
	return true;
}

bool CCompanyExtension_MFCApp::OnMenu (const Geoweb3d::GUI::IGW3DGUIMenuWPtr menu)
{
	// 'Smoke' is a POC representation (for a point layer) in the 4.0 beta that we will remove here
	// The representation selection menu can either be triggered from the 'Representation' frame on
	// the vector shelf, or in the 'represent As' submenu when right-clicking a vector layer

	printf("Showing menu %s", menu.lock()->get_Id());

	//Check to see if this is the representation menu from the Vector shelf
	if (!strcmp(menu.lock()->get_Id(), "GW3DGUIMenuNewRepresentation"))
	{
		//get the item for the smoke representation
		Geoweb3d::GUI::IGW3DGUIMenuItemWPtr smokeRepItem = menu.lock()->get_MenuItemCollection()->get_ById("GW3DGUIMenuItemNewRepresentationSmoke");
		if (!smokeRepItem.expired())
		{
			//make the item invisible
			if (Geoweb3d::Succeeded(menu.lock()->get_MenuItemCollection()->put_Visible(smokeRepItem, false)))
			{
				printf ("Removed the smoke representation from the menu!");
			}
		}
	}
	//Check to see if this is the Table of Contents context menu
	else if (!strcmp(menu.lock()->get_Id(), "GW3DGUIMenuToc"))
	{
		//Get the 'Represent as' item
		Geoweb3d::GUI::IGW3DGUIMenuItemWPtr repAsItem = menu.lock()->get_MenuItemCollection()->get_ById("GW3DGUIMenuItemTocLayerRepresentAs");
		if (!repAsItem.expired())
		{
			//The 'Represent as' item pulls up the representation menu as a submenu
			Geoweb3d::GUI::IGW3DGUIMenuWPtr repAsSubmenu = repAsItem.lock()->get_Menu();
			if (!repAsSubmenu.expired())
			{
				//get the item for the smoke representation
				Geoweb3d::GUI::IGW3DGUIMenuItemWPtr smokeRepItem = repAsSubmenu.lock()->get_MenuItemCollection()->get_ById("GW3DGUIMenuItemNewRepresentationSmoke");
				if (!smokeRepItem.expired())
				{
					//make the item invisible
					if (Geoweb3d::Succeeded(repAsSubmenu.lock()->get_MenuItemCollection()->put_Visible(smokeRepItem, false)))
					{
						printf ("Removed the smoke representation from the menu!");
					}
				}
			}
		}
	}
	return true;
}
	
bool CCompanyExtension_MFCApp::OnMenu (const Geoweb3d::GUI::IGW3DGUIMenuWPtr menu, Geoweb3d::GUI::IGW3DGUITocItemWPtr target)
{
	return OnMenu(menu);
}


// The one and only CCompanyExtension_MFCApp object

CCompanyExtension_MFCApp theApp;


// CCompanyExtension_MFCApp initialization

BOOL CCompanyExtension_MFCApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

//Geoweb3d::IGW3DRegistration *registration  is only valid for the lifetime of this call.
extern "C" BASICDESKTOPEXTENSION_API void insertSupportedClassInterfaces( Geoweb3d::IGW3DRegistration *registration )
{
	registration->add_SupportedClassInterface( GUID_GEOWEB3DDESKTOP_SDK );
}

extern "C" BASICDESKTOPEXTENSION_API bool isClassInterfaceSupported( const GW3DGUID &class_type)
{
	if(IsEqualGW3DGUID( class_type, GUID_GEOWEB3DDESKTOP_SDK ))
	{
		return true;
	}

	return false;
}

extern "C" BASICDESKTOPEXTENSION_API Geoweb3d::IGW3DIBaseObject *createClassInstance( const GW3DGUID &class_type )
{

	try
	{
		if( IsEqualGW3DGUID( class_type, GUID_GEOWEB3DDESKTOP_SDK ))
		{
			return &theApp;
		} 
	}
	catch(std::exception &e) 
    { 
       printf("%s\n", e.what());
    } 

	printf("CreateClassInstance BUG!!!!!!  what class was I to create? \n");
	//this will only happen if you added something in InsertSupportedClassInterfaces
	//that you do not support.  This means you have a bug integrating with Geoweb3d
	return NULL;
}


extern "C" BASICDESKTOPEXTENSION_API void destroyClassInstance( const GUID &class_type, Geoweb3d::IGW3DIBaseObject *p )
{
	delete p;
}

extern "C" BASICDESKTOPEXTENSION_API void getPluginDescription( Geoweb3d::GW3DMetaDataPtr info) 
{	
		//This is your information:
		info->name		= PLUGIN_NAME;			// Name of the plugin 
		info->author	= PLUGIN_AUTHOR;		// Name of the plugin author
		info->web		= PLUGIN_WEB;			// Authors website if any 
		info->guid		= PLUGIN_GUID;			// Plugins unique GUID
		info->description = PLUGIN_DESC;		// *Short* description of plugin functionality (may be multiple lines)
		info->plugin_version = PLUGIN_VERSION;	//version of your plugin, not the API version
		
		//this is not your information, but the SDK API version you compile against.
		info->gw3dapi_version = GW3D_API_VERISON;
}


//API will only get called once, when a user manually adds this plugin
extern "C" BASICDESKTOPEXTENSION_API int Install( ) //optional interface, not required to be a plug-in
{
	//are we allowed to run on this system etc?  user rights etc?
	printf("Install called\n");

	return 0; //non-0 value will be a fail.  error codes not yet defined.
}

//API will only get called once, when a user manually removes this plugin
extern "C" BASICDESKTOPEXTENSION_API void UnInstall( ) //optional interface, not required to be a plug-in
{
	printf("UnInstall called\n");
}
