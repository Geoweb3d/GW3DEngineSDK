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
#include <stdint.h>
#include "GW3DResults.h"
#include "RasterTypes.h"

 /* Define a low-level window handle type, specific to each platform */

#ifdef __ANDROID__
struct ANativeWindow;
typedef ANativeWindow *OSWinHandle;
#elif defined(__linux__) 
typedef unsigned long OSWinHandle;
#elif defined(_WIN32)

 /*HWND so we don't need to include windows.h here
   as we don't want to assume LEAN_AND_MEAN etc.*/

typedef void* OSWinHandle;
#else
typedef void* OSWinHandle;
#endif

namespace Geoweb3d
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef struct _GW3D_GUID
	{
		unsigned long   Data1;
		unsigned short  Data2;
		unsigned short  Data3;
		unsigned char   Data4[8];
	} GW3DGUID, *REFGW3DGUID, *LPGW3DGUID;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Values that represent PropertyType. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum PropertyType : int
	{
		UNKNOWN = 0,
		PROPERTY_INT = 1,// 32bit int 
		PROPERTY_DOUBLE = 2, //Double Precision floating point
		PROPERTY_STR = 3, //String of ASCII chars 
	    PROPERTY_INT64 = 4,// Single 64bit integer
		PROPERTY_BOOL = 5,// bool (c++)
		PROPERTY_DATETIME = 6, //stored as a RFC822 string. 
		PROPERTY_DATE= 7, //stored as a RFC822 string.
		PROPERTY_TIME= 8, //stored as a RFC822 string.	</summary>
		PRO_END = 0xFF
	} ;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Enum of Stereo Types. </summary>
	///
	/// <remarks>	Note: The current  stereo apis are more than likely temporary and will be moved 
	///				over to mimic the standards used for representations in the future.
	///				NOTE!!! Once, if you own the window (HWND), you cannot go back and forth between
	///				Stereo and non stereo with the QUADRO Types.  You can switch between stereo
	///				modes however, and you can switch between NO_STEREO and ANAGLYPH.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum StereoType : int
	{
		NO_STEREO = 0,
		GEOWEB3D_QUADRO = 1,
		GEOWEB3D_GEFORCE = 2,
		INFINITEZ_QUADRO = 3,
		INFINITEZ_GEFORCE = 4,
		ANAGLYPH = 5, //should work on all hardware, as it is not active stereo
		MAX_STEREO_TYPE = 0xffff
	};

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   /// <summary>	Values that represent information level. </summary>
   ////////////////////////////////////////////////////////////////////////////////////////////////////

   enum GeoWeb3d_InfoType : int { Information, Warning, Fatal, Debug };

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   /// <summary>	For functions to be used to report info, warnings, or errors. </summary>
   ////////////////////////////////////////////////////////////////////////////////////////////////////

   typedef void (*GeoWeb3d_InfoFunction)(const char *);

	namespace SDK_Timers
	{
		/* Opaque stopwatch handle */
		struct StopWatchHandle_;

		typedef StopWatchHandle_* StopWatchHandle;
	}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Values that represent GeometryType. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum GeometryType : int
		{
			/* Unknown type */
			gtUNKNOWN,
			/* Point type (2D, not z-aware)*/
			gtPOINT,
			/* Line String type (2D, not z-aware)*/
			gtLINE_STRING,
			/* Polygon type (2D, not z-aware)*/
			gtPOLYGON,
			/* Polyline type (2D, not z-aware) */
			gtPOLYLINE,
			/* Multi Point type (2D, not z-aware) - deprecated(?), use line string */
			gtMULTI_POINT,
			/* Multi Line String type (2D, not z-aware) */
			gtMULTI_LINE_STRING,
			/* Multi-Part Polygon type (2D, not z-aware) */
			gtMULTI_POLYGON,
			/* Point 2.5d type */
			gtPOINT_25D,
			/* Line String 2.5d type */
			gtLINE_STRING_25D,
			/* Polygon 2.5d type */
			gtPOLYGON_25D,
			/* Multi Point 2.5d type */
			gtMULTI_POINT_25D,
			/* Multi Line String 2.5d type */
			gtMULTI_LINE_STRING_25D,
			/* Multi-Part Polygon 2.5d type */
			gtMULTI_POLYGON_25D,
			/* Multipatch type */
			gtMULTI_PATCH,
			/* Spatial Envelope type */
			gtENVELOPE,
			/*  Line type */
			gtLINE,
			/*  Circular Arc type */
			gtCIRCULAR_ARC,
			/*  Elliptical Arc type */
			gtELLIPTIC_ARC,
			/*  Bezier Curve type */
			gtBEZIER3_CURVE,
			/*  Path type */
			gtPATH,
			/*  Ring type */
			gtRING,
			/*  Triangle Strip type */
			gtTRIANGLE_STRIP,
			/*  Triangle Fan type */
			gtTRIANGLE_FAN,
			/*  Ray type */
			grRAY,
			/*  Sphere type */
			gtSPHERE,
			/*  Triangle type */
			gtTRIANGLES,
			/*  Surface Model type */
			gtSURFACE_MODEL,
			gtLINEAR_RING,
			GEOM_TYPE_MAX = 0xFF
		};

	//  _GW3DDISPLAY_DEVICE: 
	//	StateFlags
	//	http://msdn.microsoft.com/en-us/library/dd183569(v=vs.85).aspx
	//	Thus, if looking for an active and primary display, once can 
	//  check StateFlags against dwCheck below
	//	DWORD dwCheck = DISPLAY_DEVICE_ACTIVE | DISPLAY_DEVICE_PRIMARY_DEVICE;	
	//	For Linux and Apple,  those can one day be abstracted to be like
	//  Microsoft.

	typedef struct _GW3DDISPLAY_DEVICE {
		unsigned long  cb;
		char   DeviceName[32];
		char   DeviceString[128];
		unsigned long   StateFlags;
		char   DeviceID[128];
		char   DeviceKey[128];
	} GW3DDISPLAY_DEVICE;
}