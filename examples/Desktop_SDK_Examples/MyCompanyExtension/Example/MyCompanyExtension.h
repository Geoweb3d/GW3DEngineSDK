#pragma once

#include "EntityController.h"
#include "BuildingClassifier.h"

#include "Qt\XMLFiles.h"
#include "Qt\QtDockWindow.h"

#include "core/GW3DGUIInterFace.h"

#include "gui/IGW3DGUISDK.h"
#include "gui/IGW3DGUIRebrandingToken.h"
#include "gui/IGW3DGUIGeoweb3dEventListener.h"
#include "gui/IGW3DGUIGeoweb3dEventFilter.h"
#include "gui/IGW3DGUIApplicationUpdateTimer.h"

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BASICDESKTOPEXTENSION_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BASICDESKTOPEXTENSION_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef BASICDESKTOPEXTENSION_EXPORTS
#define BASICDESKTOPEXTENSION_API __declspec(dllexport)
#else
#define BASICDESKTOPEXTENSION_API __declspec(dllimport)
#endif

class EntityDockWindow;
class Mouse3DQuery;
class SettingsManager;

class MyRebrandingToken : public Geoweb3d::GUI::IGW3DGUIRebrandingToken
{

	//IGW3DGUIRebrandingToken implimentation
	virtual void get_ValidateRebrandingLicense( /*todo*/ );
	virtual bool get_DisableNativeSplashscreen( );

};

// This class is exported from the BasicDesktopExtension.dll
class MyCompanyExtension  
	: public EntityController
	, public Geoweb3d::GUI::IGW3DGUISDK
	, public Geoweb3d::GUI::IGW3DGUIApplicationUpdateTimer
	, public Geoweb3d::GUI::IGW3DGUIGeoweb3dVectorLayerEventListener
	, public Geoweb3d::GUI::IGW3DGUIGeoweb3dVectorRepresentationEventListener
	, public Geoweb3d::GUI::IGW3DGUIGeoweb3dTocItemEventListener
	, public Geoweb3d::GUI::IGW3DGUIGeoweb3dTocItemEventFilter
	, public Geoweb3d::GUI::IGW3DGUIGeoweb3dContextMenuEventFilter
	//, public Geoweb3d::GUI::IGW3DGUIGeoweb3dDialogEventFilter
{
public:

	static MyCompanyExtension& Instance(); //singleton

	virtual ~MyCompanyExtension(void);

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

	MyRebrandingToken* GetRebrandingToken();

	virtual Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr GetEntityRepresentation() {return entity_models_;}

	void LogMessage(const char * format, ...);

	Mouse3DQuery* GetMouseQuery() {return mouse_query_;}

	SettingsManager* GetSettingsManager() {return settings_manager_;}

	bool GetMediaPath(std::string& sub_path);

private:

	MyCompanyExtension(void);

	virtual void RotateEntity ( Geoweb3d::GUI::IGW3DGUIVectorLayerStreamResult*, double hdg) override;
	virtual void ChangeIconColor ( Geoweb3d::GUI::IGW3DGUIVectorLayerStreamResult* result) override;

	void LoadEntities(Geoweb3d::GUI::IGW3DGUIStartContext *ctx );
	void LoadBuildings(Geoweb3d::GUI::IGW3DGUIPostDrawContext *ctx );
	void CreateDockWindows( Geoweb3d::GUI::IGW3DGUIDockWindowModifiableCollection * );
	void CreateShelves( Geoweb3d::GUI::IGW3DGUIStartContext *ctx );
	void CustomizeShelves( Geoweb3d::GUI::IGW3DGUIStartContext *ctx );

	Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr entity_models_;
	Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr entity_icons_;
	Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr entity_layer_;

	Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr building_polys_;
	BuildingClassifier* classifier_;

	Geoweb3d::GUI::IGW3DGUIView3dWPtr current3DView_;

	Mouse3DQuery* mouse_query_;

	EntityDockWindow* dw_;

	MyRebrandingToken* rebranding_token_;

	XMLProcessor* xml_processor_;

	SettingsManager* settings_manager_;
	
	DWORD ThreadFunction(void *parm);

	Geoweb3d::IGW3DVariant scratchpad_variant_;

	std::string base_path_;

	bool quit_;
	HANDLE threadhandle_;

	bool first_pass_;

	int framecount_;
};

template <typename C>
struct ThreadInfo
{		
	C* obj;	// and that object has a function that takes void* and returns DWORD
	DWORD (C::* function)(void*);	// and we have any amount of extra data that we might need.	
	
	void* data;	
	
	// default copy c-tor, d-tor and operator= are fine	
	ThreadInfo(C* o, DWORD (C::*func)(void*), void* d) : obj(o), function(func), data(d)
	{	
	}
};

template <typename C>
DWORD WINAPI runThreadFunction(void* data)
{
	std::auto_ptr<ThreadInfo<C> > ti((ThreadInfo<C>*)data);	
	return ((ti->obj)->*(ti->function))(ti->data);
};

template <typename C>
void* makeThreadInfo(C* o, DWORD(C::* f)(void*), void* d)
{	
	return (void*)new ThreadInfo<C>(o, f, d);
};