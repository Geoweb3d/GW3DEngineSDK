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

namespace Geoweb3d
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Values that represent GW3DResult. </summary>
///
/// <remarks>	Result and error codes. Values with the 'GW3D_e' prefix (i.e. negative values)
///				represent error definitions, while values with the 'GW3D_s' prefix represent 
///				success definitions. </remarks>
/// 
/// <see cref="Geoweb3d::Succeeded"/>
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning ( push )
#pragma warning ( disable: 4369 )
#endif
	enum GW3DResult : int
	{
		GW3D_COMPILER_HELPER = (-0x7fffffff - 1),
		//SKIP-CODE-BEGIN

				//SUCCESS DEFINITIONS

		//SKIP-CODE-END

		GW3D_sOk = (0x00000000),
		GW3D_sFalse = (0x00000001),
		GW3D_sPending = (0x00000002),
		GW3D_sAlreadyOpened = (0x00000003),
		GW3D_sAlreadyInserted = (0x00000004),
		GW3D_sProjectionGuessed = (0x00000005),
		GW3D_sAmbiguousArgument = (0x00000006),
		GW3D_sTimedOut = (0x00000007),
		GW3D_sTypeConverted = (0x00000008),
		//if a datasource layer is already represented, and you add more features, you will need to call refresh 
		//on the datasource layer to ensure the 3d scene properly represents the datasource.
		GW3D_sLayerRefreshNeeded = (0x00000009),
		// Returned when InitializeLibrary is successfully licensed, but there is no GPU detected or the version of
		// openGL does not meet the minimum requirements. 3D rendering will not function, but other parts such as 
		// the geofencing system can still be used.
		GW3D_sValidLicenseUnsupportedOpenGlVersion = (0x0000000A),
		GW3D_sArgumentClamped = (0x0000000B),
		GW3D_sCanceled = (0x0000000C),
		//SKIP-CODE-BEGIN

				//ERROR DEFINITIONS

		//SKIP-CODE-END

	//Using INT_MIN, otherwise produces compiler warning C4146
	GW3D_eUnknown = (-2147483647),// (0x80000000),
	GW3D_eNotimpl = (-2147483646),//(0x80000001),
	GW3D_eInvalidArgument = (-2147483645),//(0x80000002),
	GW3D_eApiDisabled = (-2147483644),//(0x80000003),
	GW3D_eInvalidHandle = (-2147483643),//(0x80000004),
	GW3D_eNotOpened = (-2147483642),//(0x80000005),
	GW3D_eIOError = (-2147483641),// (0x80000006),
	GW3D_eFileOpenError = (-2147483640),//(0x80000007),
	GW3D_eArgumentOutOfRange = (-2147483639),//(0x80000008),
	GW3D_eInvalidData = (-2147483638),// (0x80000009),
	GW3D_eFilterHandleMismatch = (-2147483637),//(0x8000000A),
	GW3D_eLibraryAlreadyInitialized = (-2147483636),// (0x8000000B),
	GW3D_eLibraryNotInitialized = (-2147483635),//(0x8000000C),
	GW3D_eUnknownType = (-2147483634),// (0x8000000D),
	GW3D_eNotEnabled = (-2147483633),//(0x8000000E),
	GW3D_eLayerNotFound = (-2147483632),//(0x8000000F),
	GW3D_eFeatureIdNotFound = (-2147483631),//(0x80000010),
	GW3D_eLayerLoadError = (-2147483630),//(0x80000011),
	GW3D_eLayerProjectionError = (-2147483629),//(0x80000012),
	GW3D_eUnsuccessfulCast = (-2147483628),//(0x80000013),
	GW3D_eParameterNotSet = (-2147483627),//(0x80000014),
	GW3D_eUnsupportedOpenGlVersion = (-2147483626),//(0x80000015),
	GW3D_eFileSaveError = (-2147483625),//(0x80000016),
	GW3D_eUnsupportedType = (-2147483624),//(0x80000017),

	//SKIP-CODE-BEGIN

			//note gaps between number groups are reserved

	//SKIP-CODE-END

	GW3D_eLicense_NOPRODUCT = -2147482879,//(0x80000301),	/* No authorization for product */
	GW3D_eLicense_NOTME = -2147482878,//(0x80000302),	/* Authorization is for another ISV */
	GW3D_eLicense_EXPIRED = -2147482877,//(0x80000303),	/* Authorization has expired */
	GW3D_eLicense_NOTTHISHOST = -2147482876,//(0x80000304),	/* Wrong host for authorization */
	GW3D_eLicense_BADKEY = -2147482875,//(0x80000305),	/* Bad key in authorization */
	GW3D_eLicense_BADVER = -2147482874,//(0x80000306),	/* Requested version not supported */
	GW3D_eLicense_BADDATE = -2147482873,//(0x80000307),	/* bad date format - not permanent or dd-mm-yy */
	GW3D_eLicense_TOOMANY = -2147482872,// (0x80000308),	/* checkout request for too many licenses */
	GW3D_eLicense_NOAUTH = -2147482871,//(0x80000309),	/* No license auth supplied to call */
	GW3D_eLicense_ON_EXC_ALL = -2147482870,// (0x80000310),	/* On excludeall list */
	GW3D_eLicense_ON_EXC = -2147482869,// (0x80000311),	/* On feature exclude list */
	GW3D_eLicense_NOT_INC_ALL = -2147482868,//(0x80000312),	/* Not on the includeall list */
	GW3D_eLicense_NOT_INC = -2147482867,//(0x80000313),	/* Not on the feature include list */
	GW3D_eLicense_OVER_MAX = -2147482866,//(0x80000314),	/* Request would go over license MAX */
	GW3D_eLicense_REMOVED = -2147482865,// (0x80000315),	/* License (rlm)removed by server */
	GW3D_eLicense_SERVER_BADRESP = -2147482864,//(0x80000316),	/* Unexpected response from server */
	GW3D_eLicense_COMM_ERROR = -2147482863,//(0x80000317),	/* Error communicating with server */
	GW3D_eLicense_NO_SERV_SUPP = -2147482862,//(0x80000318),	/* License server doesn't support this */
	GW3D_eLicense_NOHANDLE = -2147482861,//(0x80000319),	/* No license handle */
	GW3D_eLicense_SERVER_DOWN = -2147482860,// (0x80000320),	/* Server closed connection */
	GW3D_eLicense_NO_HEARTBEAT = -2147482859,//(0x80000321),	/* No heartbeat response received */
	GW3D_eLicense_ALLINUSE = -2147482858,//(0x80000322),	/* All licenses in use */
	GW3D_eLicense_NOHOSTID = -2147482857,//(0x80000323),	/* No hostid on uncounted license */
	GW3D_eLicense_TIMEDOUT = -2147482856,//(0x80000324),	/* License timed out by server */
	GW3D_eLicense_INQUEUE = -2147482855,//(0x80000325),	/* In queue for license */
	GW3D_eLicense_SYNTAX = -2147482854,//(0x80000326),	/* License syntax error */
	GW3D_eLicense_ROAM_TOOLONG = -2147482853,//(0x80000327),	/* Roam time exceeds maximum */
	GW3D_eLicense_NO_SERV_HANDLE = -2147482852,//(0x80000328),	/* Server does not know this handle */
	GW3D_eLicense_ON_EXC_ROAM = -2147482851,//(0x80000329),	/* On roam exclude list */
	GW3D_eLicense_NOT_INC_ROAM = -2147482850,//(0x80000330),	/* Not on roam include list */
	GW3D_eLicense_TOOMANY_ROAMING = -2147482849,//(0x80000331),	/* Too many licenses roaming */
	GW3D_eLicense_WILL_EXPIRE = -2147482848,//(0x80000332),	/* License expires before roam ends */
	GW3D_eLicense_ROAMFILEERR = -2147482847,//(0x80000333),	/* Problem with roam file */
	GW3D_eLicense_RLM_ROAM_ERR = -2147482846,//(0x80000334),	/* Cannot checkout rlm_roam license */
	GW3D_eLicense_WRONG_PLATFORM = -2147482845,//(0x80000335),	/* Wrong platform for client */
	GW3D_eLicense_WRONG_TZ = -2147482844,//(0x80000336),	/* Wrong timezone for client */
	GW3D_eLicense_NOT_STARTED = -2147482843,//(0x80000337),	/* License start date not reached */
	GW3D_eLicense_CANT_GET_DATE = -2147482842,//(0x80000338),	/* time() failure */
	GW3D_eLicense_OVERSOFT = -2147482841,// (0x80000339),	/* Over license soft limit */
	GW3D_eLicense_WINDBACK = -2147482840,//(0x80000340),	/* Clock setback detected */
	GW3D_eLicense_BADPARAM = -2147482839,// (0x80000341),	/* Bad parameter to checkout() call */
	GW3D_eLicense_NOROAM_FAILOVER = -2147482838,// (0x80000342),	/* Roam operations not permitted on	   failover servers */
	GW3D_eLicense_BADHOST = -2147482837,//(0x80000343),	/* Bad hostname in LF/port@host */
	GW3D_eLicense_APP_INACTIVE = -2147482836,//(0x80000344),	/* Application is inactive */
	GW3D_eLicense_NOT_NAMED_USER = -2147482835,//(0x80000345),	/* Not a named user */
	GW3D_eLicense_TS_DISABLED = -2147482834,//(0x80000346),	/* Terminal Server disabled */
	GW3D_eLicense_VM_DISABLED = -2147482833,//(0x80000347),	/* Virtual Machines disabled */
	GW3D_eLicense_PORTABLE_REMOVED = -2147482832,// (0x80000348),	/* Portable hostid removed */
	GW3D_eLicense_DEMOEXP = -2147482831,//(0x80000349),	/* Demo license has expired */
	GW3D_eLicense_FAILED_BACK_UP = -2147482830,//(0x80000350),	/* Failed host back up */
	GW3D_eLicense_SERVER_LOST_XFER = -2147482829,//(0x80000351) 	/* Server lost it's transferred license */
};
#ifdef _MSC_VER
#pragma warning ( pop )
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Returns true if GW3DResult is a success result. </summary>
///
/// <param name="check_result">	The result. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

static bool Succeeded(const GW3DResult check_result) 
{
	return !(check_result < GW3DResult::GW3D_sOk);
}

}
