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
#include "core/GW3DGUIInterFace.h"
#include <Geoweb3dCore/GW3DResults.h>

/* Primary namespace */
namespace Geoweb3d
{
namespace GUI
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A context for rebranding the Geoweb3d application (special licensing required). </summary>
///
/// <remarks>	IGW3DGUIApplicationRebrandingContext provides access to objects and functions that control
///				the aspects of the Geoweb3d Desktop application that can only be changed by a plugin with
///				rebranding rights.
///				
///				To enable rebranding, a rebranding license must be obtained from Geoweb3d.  
///				Contact sales@geoweb3d.com for details.
///				
///				This context is only available from the OnStart function </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DGUIApplicationRebrandingContext
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Remove the native DIS feature from Geoweb3d Desktop. </summary>
	///
	/// <remarks>	When set to false, the DIS feature in Geoweb3d Desktop will be disabled for the duration
	///				of the application session, and the 'DIS' tab in the Tools/Options dialog will be removed. </remarks>
	///
	/// <param name="enable">	true to enable (the default), false to disable. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void put_Geoweb3dNativeDISEnabled(bool enable) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Check to see if the native DIS feature has been disabled. </summary>
	///
	/// <remarks>	Geoweb3d Desktop features a native DIS implementation for viewing DIS traffic
	/// 			as a virtual GIS layer.  Some vendors may wish to remove this feature in favor 
	/// 			of their own DIS implementation. </remarks>
	///
	/// <returns>	Returns true if the native DIS feature will be enabled. If this returns false, 
	/// 			the DIS feature in Geoweb3d Desktop will be disabled, and the DIS tab in the 
	/// 			Tools/Options dialog will be removed. </returns>
	/// 
	/// <see cref="put_Geoweb3dNativeDISEnabled"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool get_Geoweb3dNativeDISEnabled() = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Format for coordinate display. </summary>
	///
	/// <remarks>	The display format of latitude and longitude within Geoweb3d Desktop. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum LatLonDisplayFormat 
	{
		/// <summary>	An enum constant representing the degrees/minutes/seconds option. </summary>
		DegreeMinutesSeconds = 0x1,
		/// <summary>	An enum constant representing the decimal degrees option. </summary>
		DecimalDegrees = 0x2,
		/// <summary>	Do Not Use. </summary>
		LatLonFormat_MAX = 0xff
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the format display of geographic coordinates. </summary>
	///
	/// <remarks>	Sets the format that will be used for displaying geographic coordinates for
	/// 			the Eye and Pointer positions in the application's status bar. </remarks>
	///
	/// <param name="format"> The format to display </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void put_Geoweb3dLatLongDisplayFormat(LatLonDisplayFormat format) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets current latitude longitude display format. </summary>
	///
	/// <remarks>	Gets the format that will be used for displaying geographic coordinates for
	/// 			the Eye and Pointer positions in the application's status bar. </remarks>
	///
	/// <returns>	LatLonDisplayFormat enumeration of degrees, minutes, and seconds or decimal degrees </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual LatLonDisplayFormat get_Geoweb3dLatLongDisplayFormat() = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Values that represent elevation display format. </summary>
	///
	/// <remarks>	Measurement units that will be used to display elevation values. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum ElevationDisplayFormat 
		{
			/// <summary>	An enum constant representing the meters option. </summary>
			Meters = 0x1,	
			/// <summary>	An enum constant representing the feet option. </summary>
			Feet = 0x2,
			/// <summary>	Do Not Use. </summary>
			ElevationFormat_MAX
		};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the format display and units of elevation  </summary>
	///
	/// <remarks>	Sets the format that will be used for displaying elevations for
	/// 			the Eye and Pointer positions in the application's status bar. </remarks>
	///
	/// <param name="format"> Meters or feet </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void put_Geoweb3dElevationDisplayFormat(ElevationDisplayFormat format) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the current display units for elevation. </summary>
	///
	/// <remarks>	Gets the format that will be used for displaying geographic coordinates for
	/// 			the Eye and Pointer positions in the application's status bar. </remarks>
	///
	/// <returns>	ElevationDisplayFormat enumeration of Meters or Feet </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual ElevationDisplayFormat get_Geoweb3dElevationDisplayFormat() = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>  Get custom file extension </summary>
	///
	/// <remarks> This will return the current custom file extension for the given type of file. </remarks>
	///
	/// <param name="file_type">	Enumeration of project, layer, or representation file.</param>
	/// <param name="extension">	Existing custom extension or null if none</param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void get_CustomFileExtension ( Geoweb3dFileType file_type, char* extension ) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>  Set custom file extension </summary>
	///
	/// <remarks> Setting custom project, layer, and representation file extensions.</remarks>
	///
	/// <param name="file_type">	Enumeration of project, layer, or representation file.</param>
	/// <param name="extension">	Requested extension.</param>
	///
	/// <returns>	GW3D_sOk if successful. GW3D_eFileOpenError in error </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual GW3DResult put_CustomFileExtension ( Geoweb3dFileType file_type, const char* extension ) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the application name. </summary>
	///
	/// <remarks>	This sets the rebranded application name, which appears on the title bar. </remarks>
	///
	/// <param name="text">	The name. </param>
	///
	/// <returns>	GW3D_sOk if successful. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual GW3DResult put_ApplicationName (const char* name) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the application icon. </summary>
	///
	/// <remarks>	Sets the application icon, which is displayed at the top left of the application 
	/// 			window. </remarks>
	///
	/// <param name="path">	Full pathname of the file. </param>
	///
	/// <returns>	GW3D_sOk if successful. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual GW3DResult put_ApplicationIcon (const char* path) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets whether to display the current project path in title bar. </summary>
	///
	/// <param name="show">	true to show, false to hide. </param>
	///
	/// <returns>	GW3D_sOk if successful. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual GW3DResult put_ShowProjectPathInTitleBar (bool show) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets whether to display location clipboard button on the status bar. </summary>
	///
	/// <param name="show">	true to show, false to hide. </param>
	///
	/// <returns>	GW3D_sOk if successful. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual GW3DResult put_ShowStatusBarButtonLocationClipboard (bool show) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets whether to display window layout button on the status bar. </summary>
	///
	/// <param name="show">	true to show, false to hide. </param>
	///
	/// <returns>	GW3D_sOk if successful. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual GW3DResult put_ShowStatusBarButtonWindowLayout (bool show) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets whether to display fullscreen button on the status bar. </summary>
	///
	/// <param name="show">	true to show, false to hide. </param>
	///
	/// <returns>	GW3D_sOk if successful. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual GW3DResult put_ShowStatusBarButtonFullScreen (bool show) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets whether to open the Geoweb3d dashboard on startup. </summary>
	///
	/// <param name="open">	true to open. </param>
	///
	/// <returns>	GW3D_sOk if successful. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual GW3DResult put_OpenDashboardOnStartup (bool open) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets whether to enable latitude/longitude lines by default. </summary>
	///
	/// <param name="open">	true to enable. </param>
	///
	/// <returns>	GW3D_sOk if successful. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual GW3DResult put_EnableLatitudeLongitudeLines (bool enable) = 0;

};

}
}
