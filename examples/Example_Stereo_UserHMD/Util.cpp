#include "stdafx.h"
#include "Util.h"

#include <GeoWeb3dCore/SystemExports.h>
#include <stdarg.h>

/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function(const char *msg )
{
	printf("Fatal Info: %s", msg);
}

/*! General info function, this will be used to print any non-fatal warnings */
void my_warning_function(const char *msg )
{
	printf("General Warning: %s", msg);
}


/*! General info function, this will be used to print any general info */
void my_info_function(const char *msg )
{
	printf("General Info: %s", msg);
}

/*! Information handling is not required */
void SetInformationHandling()
{
	/*! Tell the engine about our error function handling */
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Information, my_info_function);
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Warning, my_warning_function);
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Fatal, my_fatal_function);
}
