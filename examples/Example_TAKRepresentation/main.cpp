/**
 * Property of Geoweb3d: GIS SDK
 * --------------------------------
 * Copyright 2008-2014
 * Author: James Pieszala, Geoweb3d
 * Geoweb3d SDK is not free software: you cannot redistribute it and/or modify
 * it under any terms unless we have a written agreement between us.
 * Geoweb3d SDK and example applications are distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
**/
//#include "stdafx.h"
#include <windows.h>
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "engine/GW3DCamera.h"

#include "MiniEngine.h"
#include <string>
#include <locale>
#include <codecvt>

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

void my_fatal_function(const char* msg );
void my_info_function(const char* msg );
void SetInformationHandling();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the application operation. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">	Context for the sdk. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void RunApplication(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, CommandLineArgs& args )
{
    MiniEngine* pengine = new MiniEngine(sdk_context, args );

    if (pengine->LoadConfiguration("TAK Representation Example"))
    {
        ////******* CHANGE THIS TO YOUR SAMPLE DATA LOCATION ******/
        const char* root_path = "../examples/media/";
        if (pengine->LoadData(root_path))
        {

        }
        else
        {
            printf("Could not load data, you probably need to setup the path to your data\n");
        }

        //the engine loaded all its data ok

        while (pengine->Update())
        {
            //could do other app stuff here
        }
    }



    delete pengine;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Main entry-point for this application. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="argc">	The argc. </param>
/// <param name="argv[1]"> TAK Server hostname/ip address</param>
/// <param name="argv[2]"> TAK Server port</param>
/// <param name="argv[3]"> TAK user callsign</param>
/// <param name="argv[4]"> TAK Server user name(if required)</param>
/// <param name="argv[5]"> TAK Server password( if required)</param>
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
    SetInformationHandling();

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context(Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface());

    if (sdk_context)
    {
        Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
        if (Geoweb3d::Succeeded(sdk_context->InitializeLibrary("geoweb3dsdkdemo", sdk_init, 5, 0)))
        {
            std::cout << "\nUsage: " << "TAKRepresentation.exe <server ip> <server port> <callsign> <user_name> <password>" << std::endl;

			CommandLineArgs commandline_arg;
			using convert_type = std::codecvt_utf8<wchar_t>;
			std::wstring_convert<convert_type, wchar_t>converter;

			for(int i = 1; i < argc; ++i)
			{
				std::wstring args(argv[i]);

				if(i == 1)
				{
                    //Server ip address required for connect to a given TAK server.
					commandline_arg.server_ip = converter.to_bytes(args);
					continue;
				}

				if(i == 2)
				{
                    //Server ip address required for connect to a given TAK server.
                    commandline_arg.server_port = std::stoi( converter.to_bytes(args) );
					continue;
				}
				if(i == 3)
				{
                    //Assign the user callsign that will be used to identify this instance on the TAK server.
					commandline_arg.callsign = converter.to_bytes(args);
					continue;
				}

				if(i == 4)
				{
                    //Assign user name used to connect to the TAK server if required.
                    commandline_arg.user_name = converter.to_bytes(args);
					continue;
				}

				if(i == 5)
				{
                    //Assign password used to connect to the TAK server if required.
                    commandline_arg.password = converter.to_bytes(args);
					continue;
				}
			}

            RunApplication(sdk_context, commandline_arg);
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

    //system("pause");

    return 0;
}


/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	My fatal function. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="msg">	The message. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void my_fatal_function(const char* msg)
{
    printf("Fatal Info: %s", msg);
}

/*! General info function, this will be used to print any general info and non-fatal warnings */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	My information function. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="msg">	The message. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void my_info_function(const char* msg)
{
    printf("General Info: %s", msg);
}

/*! Information handling is not required */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets information handling. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SetInformationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Information, my_info_function);
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Warning, my_info_function);
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Fatal, my_fatal_function);
}