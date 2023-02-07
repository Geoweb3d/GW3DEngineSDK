#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_syswm.h>

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"

#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dcore/ImageryExports.h"

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

namespace
{
char myinfobuf[1024] = {0};
/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function(const char *msg,...)
{
    va_list ap;
    va_start( ap, msg );
    vsprintf_s( myinfobuf, msg, ap );
    va_end( ap );

	printf("SDK Fatal: %s", myinfobuf);
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function( const char* msg, ... )
{
    va_list ap;
    va_start( ap, msg );
    vsprintf_s( myinfobuf, msg, ap );
    va_end( ap );
    
	printf("SDK Info: %s", myinfobuf);
}

/*! Information handling is not required */
void SetInfomationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}

Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_global_;

Geoweb3d::IGW3DWindowWPtr window_global1;
Geoweb3d::IGW3DWindowWPtr window_global2;

Geoweb3d::IGW3DCameraWPtr window1_camera1;
Geoweb3d::IGW3DCameraWPtr window2_camera1;

Geoweb3d::IGW3DCameraWPtr window_global_native_camera_native;
Geoweb3d::IGW3DWindowWPtr window_global_native;


bool Running = true;
}

void sdldie(const char *msg)
{
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}
 
 
void checkSDLError(int line = -1)
{
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		printf("SDL Error: %s\n", error);
		if (line != -1)
			printf(" + line: %i\n", line);
		SDL_ClearError();
	}
}
 
void OnEvent(SDL_Event* Event)
{
    if(Event->type == SDL_QUIT)
	{
        Running = false;
    }

    if(Event->type == SDL_KEYDOWN)
    {
    }

    if(Event->type == SDL_KEYDOWN)
    {
        switch(Event->key.keysym.sym)
        {
            case SDLK_ESCAPE:
				Running = false;
				break;
        }
    }

    if(Event->type == SDL_MOUSEMOTION)
    {
		int x,y;
        SDL_GetMouseState(&x,&y);
    }
}

/* Required to set the context active (currently only used when the GW3D 3D Window is created) */
 class CallbackForSDLContextInitialization : public Geoweb3d::IGW3DWindowCallback
 {
 public:

	 CallbackForSDLContextInitialization(SDL_Window* sdl_window, SDL_GLContext sdl_context) : sdl_window_(sdl_window), sdl_context_(sdl_context)
	 {
	 
	 }

	void ProcessEvent( const Geoweb3d::WindowEvent& win_event )
	{
		switch(win_event.Type)
		{
		
		case win_event.GW3DOpenGLType:
			switch(	win_event.OpenGLEvent.code)
			{
			case  Geoweb3d::GW3DOpenGL::MakeContextCurrent:
				// Required to make the context active (currently only does this at window creation)
				SDL_GL_MakeCurrent( sdl_window_, sdl_context_ );
				break;

			};	
		};
	}
 
	virtual void OnCreate( ){};
    virtual void OnDrawBegin( ) {};
	virtual void OnDrawEnd( ) {};
	virtual void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera ){}
 private:
	SDL_Window* sdl_window_;
	SDL_GLContext sdl_context_; 
 };



/* Our program's entry point */
int _tmain(int argc, _TCHAR* argv[])
{
     if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
	 {
        sdldie("Unable to initialize SDL"); /* Or die on error */
		return -1;
	 }
 
    // Request opengl 4.3 context.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
 
    /// Turn on double buffering with a 24bit Z buffer.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SetInfomationHandling( );

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );
	sdk_context_global_ = sdk_context;

    if(sdk_context)
    {
		//Its important that the (a) opengl context be created before calling InitializeLibrary, otherwise geoweb3d will create
		//its own context to validate if the system is capable to support the opengl version required.
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
		sdk_init->put_ESRILicenseCheckout(false); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
        {


			SDL_Window *mainwindow1 = SDL_CreateWindow("SDL-window1", 512, 0, 512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
			SDL_GLContext maincontext1 = SDL_GL_CreateContext(mainwindow1);
			CallbackForSDLContextInitialization window1_callback(mainwindow1,maincontext1);
			
			{
				int x,y,w,h;
				SDL_GetWindowPosition(mainwindow1, &x, &y);
				SDL_GetWindowSize(mainwindow1,&w,&h);

				SDL_SysWMinfo wmInfo1;
				SDL_VERSION(&wmInfo1.version);
				SDL_GetWindowWMInfo( mainwindow1, &wmInfo1 );
				HWND hwndSDL1 = wmInfo1.info.win.window;

				//Now create the windows 3d windows with SDL
				window_global1 = 
					sdk_context->get_WindowCollection(  )->create_3DWindow( SDL_GetWindowTitle( mainwindow1 ), GW3D_EXTERNAL_CONTEXT/*SDL is the external context*/,
					x, y, w, h, hwndSDL1/*external window to draw to*/, Geoweb3d::IGW3DStereoDriverPtr(), &window1_callback /*required for an external so the context can be set active*/);

				window1_camera1 = window_global1.lock()->get_CameraCollection()->create("window1_camera1");
			}

	//Now create window 2

	SDL_Window *mainwindow2 = SDL_CreateWindow("SDL-window2", 512, 0, 512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_GLContext maincontext2 = SDL_GL_CreateContext(mainwindow2);

	CallbackForSDLContextInitialization window2_callback(mainwindow2,maincontext2);
	
	{
		int x,y,w,h;
		SDL_GetWindowPosition(mainwindow2, &x, &y);
		SDL_GetWindowSize(mainwindow2,&w,&h);

		SDL_SysWMinfo wmInfo2;
		SDL_VERSION(&wmInfo2.version);
		SDL_GetWindowWMInfo( mainwindow2, &wmInfo2 );
		HWND hwndSDL2 = wmInfo2.info.win.window;

		//Now create the windows 3d windows with SDL
		window_global2 = 
			sdk_context->get_WindowCollection(  )->create_3DWindow( SDL_GetWindowTitle( mainwindow2 ), GW3D_EXTERNAL_CONTEXT/*let the sdk know to be friendly with an external context*/, x, y, w, h, hwndSDL2, Geoweb3d::IGW3DStereoDriverPtr(), &window2_callback );
		window2_camera1 = window_global2.lock()->get_CameraCollection()->create("window2_camera1");

	}
    checkSDLError(__LINE__);

		window_global_native = 	sdk_context->get_WindowCollection(  )->create_3DWindow( "Windows_Native", GW3D_OVERLAPPED, 20, 20, 120, 120, 0, Geoweb3d::IGW3DStereoDriverPtr(), 0 );
		window_global_native_camera_native = window_global_native.lock()->get_CameraCollection()->create("window_global_native Camera");

	Geoweb3d::Imagery::ImageryLoadProperties imagery_properties;
	Geoweb3d::Raster::RasterSourceHandle sample_raster_handle2;
   if(! Geoweb3d::Succeeded( Geoweb3d::Imagery::LoadImagery("../examples/media/DC_1ft_demo.ecw" , 0, imagery_properties, sample_raster_handle2 ) ) )
    {
        printf("LoadImagery Error\n");
    }


	Geoweb3d::Raster::RasterSourceHandle sample_raster_handle;
   if(! Geoweb3d::Succeeded( Geoweb3d::Imagery::LoadImagery("../examples/media/WSI-Earth99-2k.ecw" , 0, imagery_properties, sample_raster_handle ) ) )
    {
        printf("LoadImagery Error\n");
    }


			while(Running) 
			{
				SDL_Event Event;
			   
				while(SDL_PollEvent(&Event))
				{
					OnEvent(&Event);					
				}	

				if(sdk_context_global_ && !window_global1.expired() /*&& !window_global2.expired()*/)
				{ 
					sdk_context_global_->draw( window_global1/*,-1 Geoweb3d::DISABLE_VERTICAL_SYNC*/); //disable the sdk's swap buffer
					sdk_context_global_->draw( window_global2/*,-1 Geoweb3d::DISABLE_VERTICAL_SYNC*/); //disable the sdk's swap buffer
					sdk_context_global_->draw( window_global_native );
				}


				if( !window1_camera1.expired() ) //This is just showing a best practice. Just in case the window this camera belongs to was deleted.
				{
					Geoweb3d::IGW3DCameraController* position_controller1 = window1_camera1.lock()->get_CameraController();
					Geoweb3d::IGW3DCameraController* position_controller2 = window2_camera1.lock()->get_CameraController();
					Geoweb3d::IGW3DCameraController* position_controller3 = window_global_native_camera_native.lock()->get_CameraController();
					


					float heading, pitch, roll;
					const Geoweb3d::GW3DPoint* loc = position_controller1->get_Location();

					position_controller1->put_Location( loc->get_X() + 1.1, loc->get_Y() + .000001 );
					position_controller2->put_Location( loc->get_X() + 1.1, loc->get_Y() + .000001 );
					position_controller3->put_Location( loc->get_X() + 1.1, loc->get_Y() + .000001 );

					//again, this is pointing out the readback and setting capabilities
					position_controller1->get_Rotation( heading, pitch, roll );
					
					position_controller1->put_Rotation( heading + 2.f, 30.f, roll+30 );	
					position_controller2->put_Rotation( -heading + 2.f, 60.f, roll-30 );
					position_controller3->put_Rotation(0,90,0);
					

					position_controller3->put_Elevation(9000000);
				}

			}
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

	    SDL_Quit();

 
    return 0;
}