/**
 * Property of Geoweb3d: GIS SDK
 * --------------------------------
 * Copyright 2008-2014
 * Author: Vincent A. Autieri, Geoweb3d
 * Geoweb3d SDK is not free software: you cannot redistribute it and/or modify
 * it under any terms unless we have a written agreement between us.
 * Geoweb3d SDK and example applications are distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
**/
#include "stdafx.h"
#include <windows.h>

#include "GeoWeb3dCore/SystemExports.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/IGW3DGeoweb3dSDK.h"

//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "common/IGW3DVariant.h"
#include "common/IGW3DString.h"


#include "OpenGLImageLoader.h"

#include <MyGUI.h>
#include <MyGUI_OpenGLDataManager.h>
#include <MyGUI_OpenGLPlatform.h>
#include <MyGUI_OpenGLRenderManager.h>


#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "../ThirdParty/devil-1.7.8/DevIL-SDK-x86-1.7.8/lib/ILU.lib")
#pragma comment (lib, "../ThirdParty/devil-1.7.8/DevIL-SDK-x86-1.7.8/lib/ILUT.lib")
#pragma comment (lib, "../ThirdParty/devil-1.7.8/DevIL-SDK-x86-1.7.8/lib/DevIL.lib")
#pragma comment (lib, "../ThirdParty/devil-1.7.8/DevIL-SDK-x86-1.7.8/lib/DevIL.lib")

#pragma comment (lib, "../ThirdParty/devil-1.7.8/DevIL-SDK-x86-1.7.8/lib/DevIL.lib")
#pragma comment (lib, "../ThirdParty/MyGUI_3.2.0/vs2005/lib/release/MyGUIEngineStatic.lib")
#pragma comment (lib, "../ThirdParty/MyGUI_3.2.0/vs2005/lib/release/Common.lib")
#pragma comment (lib, "../ThirdParty/MyGUI_3.2.0/vs2005/lib/release/MyGUI.OpenGLPlatform.lib")
#pragma comment (lib, "../ThirdParty/freetype-2.3.11/objs/win32/vc2005/freetype2311MT.lib")

char myinfobuf[1024] = {0};
void my_fatal_function(const char *msg,...);
void my_info_function(const char *msg,...);
void SetInfomationHandling();


Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events , int x_loc);
Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle );

class MiniEngine;

OpenGLImageLoader_Devil test;
int starting_width = 800;
int starting_height = 600;

class MiniEngine : public Geoweb3d::IGW3DWindowCallback
{
public:
    MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context ) : sdk_context_(sdk_context) {}
    virtual ~MiniEngine()
    {
    }

    void notifyMouseButtonPressed( MyGUI::Widget* _sender,int _left, int _top, MyGUI::MouseButton _id)
    {
        printf("MYGUI Button Pressed!\n");
    }

    bool Update()
    {
        DoPreDrawWork();

		const int disable_wait_for_vertical_retrace = 0;
        
		if( sdk_context_->draw( win_a.window_, disable_wait_for_vertical_retrace ) == Geoweb3d::GW3D_sOk )
        {

			//we want to wait for the last vertical retrace here
			//to throttle us and at leat avoid tearing on this window
			if( sdk_context_->draw( win_b.window_ ) == Geoweb3d::GW3D_sOk )
			{

			}

            DoPostDrawWork();
            return true;
        }

        //return that we did not draw. (app closing?)
        return false;
    }

    bool LoadConfiguration(const char *example_name )
    {
        win_a.window_ = CreateWindowCreation( sdk_context_, example_name, this , 0);

        if(win_a.window_.expired())return false;

        win_a.camera_ = CreateCamera(win_a.window_,"Camera 1 window a");
        win_a.camera_controller_ = win_a.camera_.lock()->get_CameraController();

        if(win_a.camera_.expired())return false;

  

        win_b.window_ = CreateWindowCreation( sdk_context_, "WINDOW B", 0 /*we do not want callbacks*/,300 );

        if(win_b.window_.expired())return false;

        win_b.camera_ = CreateCamera(win_b.window_,"Camera 1 window b");
        win_b.camera_controller_ = win_b.camera_.lock()->get_CameraController();

        if(win_b.camera_.expired())return false;

       
		sdk_context_->put_EnableEventStream( true );    


        return true;
    }

    bool LoadData( const char *rootpath )
    {
        return true;
    }

private:
    void DoPreDrawWork()
    {
    }

    void DoPostDrawWork()
    {
    }

private:

    //call back for 2d drawing/client side
    virtual void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera )
    {
        //Draw icons in 2D or just to find out whats in view.
        //If you are drawing in OpenGL, at this point we have an OpenGL 3.3 render
        //context inside the SDK.
        // CameraFrustumContentsTest frustum_intersection;
        //sdk_context_->get_VectorRepresentationDriverCollection()->Stream( &frustum_intersection );

       
    }

	void OnCreate( ) 
	{
		mPlatform = new MyGUI::OpenGLPlatform();
        mPlatform->initialise(&test, "mIL");

        mPlatform->getRenderManagerPtr()->setViewSize(starting_width,starting_height);

        //this will iterator over alot of directories.. but this broad
        //as it will work for this example app pending where
        //their working directory is
        mPlatform->getDataManagerPtr()->addResourceLocation("../", true);

        mGUI = new MyGUI::Gui();
        mGUI->initialise("MyGUI_Core.xml");


        MyGUI::ButtonPtr button = mGUI->createWidget<MyGUI::Button>("Button", 10, 10, 300, 26, MyGUI::Align::Default, "Main");
        button->setTextAlign(MyGUI::Align::Center);
        button->eventMouseButtonPressed += MyGUI::newDelegate(this, &MiniEngine::notifyMouseButtonPressed);
        button->setCaption("Hello World!");

	}

	void OnDrawBegin( ) 
	{

	}
	void OnDrawEnd( ) 
	{
		 mPlatform->getRenderManagerPtr()->drawOneFrame();
	}




    // OS event system, raw data directly from the windows message pump.
    virtual LRESULT CALLBACK ProcessLowLevelEvent(OSWinHandle hWndParent, UINT message, WPARAM wParam, LPARAM lParam )
    {


        // We don't forward the WM_CLOSE message to prevent the internal SDK from closing us (click on X)
        if (message == WM_CLOSE)
        {
            printf("ProcessLowLevelEvent: WM_Close detected and prevented!\n");
            return 0;
        }

        // ???? ?????? ?? ???????????
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#define __WM_REALMOUSELAST WM_MOUSEWHEEL
#else
#define __WM_REALMOUSELAST WM_MOUSELAST
#endif // WM_MOUSEWHEEL

#define GET_HIWORD(param) ((short)HIWORD(param))
#define GET_LOWORD(param) ((short)LOWORD(param))

        static int old_x = 0;
        static int old_y = 0;
        static int old_z = 0;
        static bool left_button = false;
        static bool right_button = false;

        switch (message)
        {
        case WM_MOUSEMOVE:
        {
            int x = GET_LOWORD(lParam);
            int y = GET_HIWORD(lParam);

            old_x = x;
            old_y = y;
            MyGUI::InputManager::getInstance().injectMouseMove(old_x, old_y, old_z);
        }

        break;

        case WM_MOUSEWHEEL:
            old_z += GET_HIWORD(wParam);
            MyGUI::InputManager::getInstance().injectMouseMove(old_x, old_y, old_z);
            break;

        case WM_LBUTTONDOWN:
            left_button = true;

            MyGUI::InputManager::getInstance().injectMousePress(old_x, old_y, MyGUI::MouseButton::Left);
            break;

        case WM_LBUTTONDBLCLK:
            left_button = true;

            MyGUI::InputManager::getInstance().injectMousePress(old_x, old_y, MyGUI::MouseButton::Left);
            break;

        case WM_RBUTTONDOWN:
            right_button = true;

            MyGUI::InputManager::getInstance().injectMousePress(old_x, old_y, MyGUI::MouseButton::Right);
            break;

        case WM_RBUTTONDBLCLK:
            right_button = true;

            MyGUI::InputManager::getInstance().injectMousePress(old_x, old_y, MyGUI::MouseButton::Right);
            break;

        case WM_MBUTTONDOWN:
            MyGUI::InputManager::getInstance().injectMousePress(old_x, old_y, MyGUI::MouseButton::Middle);
            break;

        case WM_LBUTTONUP:
            MyGUI::InputManager::getInstance().injectMouseRelease(old_x, old_y, MyGUI::MouseButton::Left);
            left_button = false;

            break;
        case WM_RBUTTONUP:
            right_button = false;

            MyGUI::InputManager::getInstance().injectMouseRelease(old_x, old_y, MyGUI::MouseButton::Right);
            break;
        case WM_MBUTTONUP:
            MyGUI::InputManager::getInstance().injectMouseRelease(old_x, old_y, MyGUI::MouseButton::Middle);
            break;

        case WM_SIZE :
        {
            // Ignore size events triggered by a minimize (size == 0 in this case)

            if(mPlatform)
                if (wParam != SIZE_MINIMIZED)
                {
                    // Update window size
                    RECT rectangle;
                    GetClientRect((HWND)hWndParent /*(HWND)window_.lock()->get_OSWinHandle()*/, &rectangle);
                    int width_  = rectangle.right - rectangle.left;
                    int height_ = rectangle.bottom - rectangle.top;
                    mPlatform->getRenderManagerPtr()->setViewSize(width_,height_);

                    break;
                }
        }
        }

        return 1;
    }

private:

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;

	struct WindowType
	{
		//its not really needed to pull these out of the 
		//sdk context.  This is just to help
		//someone understand whats going on.
		Geoweb3d::IGW3DWindowWPtr window_;
		Geoweb3d::IGW3DCameraWPtr camera_;
		Geoweb3d::IGW3DCameraController *camera_controller_;
	};

	WindowType win_a;
	WindowType win_b;

    MyGUI::Gui* mGUI;
    MyGUI::OpenGLPlatform* mPlatform;

}; //engine end of class

Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events, int x_loc )
{
    Geoweb3d::IGW3DWindowCollection * wcol = sdk_context->get_WindowCollection(  );


    Geoweb3d::IGW3DWindowWPtr window = wcol->create_3DWindow(title, GW3D_OVERLAPPED, x_loc, 10, starting_width,starting_height, 0, Geoweb3d::IGW3DStereoDriverPtr(), window_events);

    if(window.expired())
    {
        printf("Error Creating window: [%s]!\n", title );

        return Geoweb3d::IGW3DWindowWPtr();
    }

    return window;
}


Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle )
{
    Geoweb3d::IGW3DCameraCollection *cameracollection = windowptr.lock()->get_CameraCollection();
    Geoweb3d::IGW3DCameraWPtr camera =	cameracollection->create( cameratitle );

    if(!camera.expired())
    {
        printf("DelimitedString: %s\n",   camera.lock()->get_DateTime()->get_DateFromDelimitedString()->c_str());
        printf("UnDelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromUnDelimitedString()->c_str());

        camera.lock()->get_DateTime()->put_isUseComputerDate(false);
        camera.lock()->get_DateTime()->put_isUseComputerTime(false);


        if(!camera.lock()->get_DateTime()->put_DateFromDelimitedString("2002-1-25"))
        {
            printf("Error with put_DateFromDelimitedString\n");
        }

        if(!camera.lock()->get_DateTime()->put_DateFromUnDelimitedString("2002125"))
        {
            printf("Error with put_DateFromUnDelimitedString\n");
        }


        printf("User Set: DelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromDelimitedString()->c_str());
        printf("User Set: UnDelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromUnDelimitedString()->c_str());


        //camera.lock()->get_DateTime()->put_Time(12,0);
        //go back to useing the date and time of this computer
        camera.lock()->get_DateTime()->put_isUseComputerDate(true);
        camera.lock()->get_DateTime()->put_isUseComputerTime(true);
    }

    return camera;
}

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{

    MiniEngine *pengine = new MiniEngine( sdk_context );

    if(pengine->LoadConfiguration("Example  - MYGUI GUI INTEGRATION"))
    {
        ////******* CHANGE THIS TO YOUR SAMPLE DATA LOCATION ******/
        const char * root_path = "C:/User/Docs/Geoweb3d/Samples/Washington DC";
        //  download the Geoweb3d DC sample project here: http://www.geoweb3d.com/download/sample-projects/
        if(pengine->LoadData( root_path ))
        {

        }
        else
        {
            printf("Could not load data, you probably need to setup the path to your data\n");
        }

        //the engine loaded all its data ok

        while( pengine->Update() )
        {
            //could do other app stuff here
        }
    }
    delete pengine;
}

int _tmain(int argc, _TCHAR* argv[])
{
    SetInfomationHandling( );

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if(sdk_context)
    {
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
		sdk_init->put_ESRILicenseCheckout(false); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
        {
            RunApplication( sdk_context );
        }
        else
        {
            printf("\nSDK Context Creation Error! Press any key to continue...\n");
            getchar();
        }
    }
    else
    {
        return -1;
    }

    //example to show if you want to control its when the sdk context gets destroyed.  This will invalidate all
    // the pointers the SDK owned!  (xxx.expired() ==true)
    sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();


    return 0;
}


/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function(const char *msg,...)
{
    va_list ap;
    va_start( ap, msg );
    vsprintf_s(myinfobuf,msg,ap);
    va_end(ap);
    printf("Fatal Info: %s", myinfobuf);
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function(const char *msg,...)
{
    va_list ap;
    va_start( ap, msg );
    vsprintf_s(myinfobuf,msg,ap);
    va_end(ap);
    printf("General Info: %s", myinfobuf);
}

/*! Information handling is not required */
void SetInfomationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}