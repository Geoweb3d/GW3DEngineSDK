#include "SystemFunctionality.h"
#include "stdafx.h"

#include "Geoweb3dCore/SystemExports.h"
#include "Geoweb3dCore/GeoWeb3dEngine.h"

namespace
{
	char myinfobuf[1024] = {0};
	Geoweb3d::IGW3DGeoweb3dSDKPtr g_sdk_context;

	void my_fatal_function(const char *msg,...) 
	{
		va_list ap; 
		va_start( ap, msg );
		vsprintf(myinfobuf,msg,ap);
		va_end(ap); 
		printf("Fatal Info: %s", myinfobuf);
	}

	void my_info_function(const char *msg,...) 
	{
		//va_list ap; 
		//va_start( ap, msg );
		//vsprintf(myinfobuf,msg,ap);
		//va_end(ap); 
 	//	printf("General Info: %s", myinfobuf);
	}
}

namespace Geoweb3d
{
	namespace Example
	{

bool InitializeSDK( const char* product )
{
	Geoweb3d::WorldConfiguration wconfig;

	 g_sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

	 if( !Geoweb3d::Succeeded( g_sdk_context->InitializeLibrary( product, wconfig, 5, 0 ) ) )
   {
      printf("Error %s trying to InitializeLibrary GW3DEngineSDK.dll\n", product);
      printf("Press any key to continue . . .");
      getchar();
      return false;
   }

   return true;
}

bool ShutdownSDK()
{
 
	g_sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();

	return true;
}

void SetupErrorHandling()
{
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}

void DumpStatistics()
{
	std::cout << "\n** DumpStatistics ** "<< std::endl;

	/*! Number of general tasks left to execute */
	std::cout << "GetGeneralTasksPending(): "	<< Geoweb3d::SDK_Statistics::GetGeneralTasksPending() << std::endl;

	/*! Number of tasks related to imagery to execute */
	std::cout << "GetImageryTasksPending(): "	<< Geoweb3d::SDK_Statistics::GetImageryTasksPending() << std::endl;

	/*! Number of tasks related to paging 3D models */
	std::cout << "Get3DModelTasksPending(): "	<< Geoweb3d::SDK_Statistics::Get3DModelTasksPending() << std::endl;

	/*! Number of 3d models in scene */
	std::cout << "GetTotalActive3DModels(): "	<< Geoweb3d::SDK_Statistics::GetTotalActive3DModels() << std::endl;
	
	/*! Number of frames executed */
	std::cout << "GetMasterFrameCount(): "		<< Geoweb3d::SDK_Statistics::GetMasterFrameCount() << std::endl << std::endl;
}

void TestMemory()
{
	std::cout << "\nMemory Usage: "			<< Geoweb3d::OS_Helpers::GetMemoryUsage()				<< std::endl;
	std::cout << "Total Physical Memory: "	<< Geoweb3d::OS_Helpers::GetTotalPhysicalMemoryInBytes()<< std::endl;
	std::cout << "Free Physical Memory: "	<< Geoweb3d::OS_Helpers::GetFreePhysicalMemoryInBytes()	<< std::endl	<< std::endl;
	std::cout << "Total PageFile: "			<< Geoweb3d::OS_Helpers::GetTotalPageFileInBytes()		<< std::endl;
	std::cout << "Free PageFile: "			<< Geoweb3d::OS_Helpers::GetFreePageFileInBytes()		<< std::endl	<< std::endl;
	std::cout << "Total Virtual Memory: "	<< Geoweb3d::OS_Helpers::GetTotalVirtualMemoryInBytes() << std::endl;
	std::cout << "Free Virtual Memory: "	<< Geoweb3d::OS_Helpers::GetFreeVirtualMemoryInBytes()	<< std::endl;
	std::cout << "Free Extended Memory: "	<< Geoweb3d::OS_Helpers::GetFreeExtendedMemoryInBytes()	<< std::endl	<< std::endl;
}

	}
}