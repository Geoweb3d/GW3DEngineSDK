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
#include "GeometryExports.h"

/* Primary namespace */
namespace Geoweb3d
{
	struct  GW3D_DLL IGW3DPropertyCollection;

		/* Helper functions for queries and commands related to the operating system */
		namespace OS_Helpers
		{

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets to a proxy. </summary>
			///
			/// <remarks>	 SSL host / certificate verification is on by default</remarks>
			///
			/// <param name="yes_allow">	true will allow untrusted, false will not allow</param>
			///
			/// <returns>	A GW3DResult. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL GW3DResult Allow_UntrustedSSL(bool yes_allow);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the current setting for SSL host / certificate verification  </summary>
			///
			/// <returns> bool </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL bool IsAllowed_UntrustedSSL();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the user agent for HTTP requests. </summary>
			///
			/// <param name="user_agent">	The new user agent. </param>
			///
			/// <returns>	A GW3DResult. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL GW3DResult SetHTTPUserAgent( const char* user_agent );

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets HTTP proxy. </summary>
			///
			/// <remarks>	Set HTTP proxy to use. The parameter should be a char * to a zero terminated string holding the
			///				host name or dotted IP address. To specify port number in this string, append :[port] to the end
			///				of the host name. The proxy string may be prefixed with [protocol]: // since any such prefix will
			///				be ignored. The proxy's port number may optionally be specified with the separate option. If not specified,
			///				default port 1080 is used. </remarks>
			///
			/// <param name="proxy">	The proxy. </param>
			///
			/// <returns>	A GW3DResult. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL GW3DResult SetHTTPProxy( const char* proxy);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets HTTP proxy user password. </summary>
			///
			/// <remarks>	Pass a char * as parameter, which should be [user name]:[password] to use for the 
			/// 			connection to the HTTP proxy. </remarks>
			///
			/// <param name="userpassword">	The userpassword. </param>
			///
			/// <returns>	A GW3DResult. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL GW3DResult SetHTTPProxyUserPassword( const char* userpassword );

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Get data at an HTTP/HTTPS or ftp url. </summary>
			///
			/// <returns>	A GW3DResult. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL IGW3DDataBufferPtr GetHTTPFetch(const char* urlfile, GW3DResult& retval);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets number of available CPUs. </summary>
			///
			/// <remarks>	Reports how many processors/cores are available to you </remarks>
			///
			/// <returns>	The number of CPUs. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL unsigned GetNumCPUs();

			struct GW3DSystemMemoryData {
				uint64_t virtual_total = 0;
				uint64_t virtual_used = 0;
				uint64_t virtual_usedByProcess = 0;
				uint64_t virtual_usedByProcess_max = 0;

				uint64_t physical_total = 0;
				uint64_t physical_used = 0;
				uint64_t physical_usedByProcess = 0;
				uint64_t physical_usedByProcess_max = 0;
			};

			//////////////////////////////////////////////////////////////////////////////////////////////////////
			// <summary> Gets the memory details of the system and this process GW3DSystemMemoryData. </summary>
			//
			// <returns>	The GW3DSystemMemoryData memory details in bytes. </returns>
			//////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL GW3DSystemMemoryData get_SystemMemoryData();
		}

		/* Functions for creating and controlling timers */
		namespace SDK_Timers
		{

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Creates stopwatch timer. </summary>
			///
			/// <remarks>	Provides a handle to the created timer, for use in the following timer functions. </remarks>
			///
			/// <param name="handle">	[out] The handle. </param>
			///
			/// <see cref="GetTime"/>
			///
			/// <returns>	GW3D_Ok if successful. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL GW3DResult CreateStopWatchTimer(/*out*/ StopWatchHandle& handle );

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Destroys the stopwatch timer described by handle. </summary>
			///
			/// <param name="handle">	[out] The handle. </param>
			///
			/// <see cref="GetTime"/>
			/// <see cref="CreateStopWatchTimer"/>
			///
			/// <returns>	A GW3DResult. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL GW3DResult DestroyStopWatchTimer(/*in*/ StopWatchHandle& handle );

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Resets the stopwatch timer described by handle. </summary>
			///
			/// <remarks>	Resets to an elapsed time of zero. </remarks>
			///
			/// <param name="handle">	The handle. </param>
			///
			/// <see cref="GetTime"/>
			/// <see cref="CreateStopWatchTimer"/>
			///
			/// <returns>	A GW3DResult. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL GW3DResult ResetStopWatchTimer( /*in*/ StopWatchHandle handle );

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Pause stopwatch timer. </summary>
			///
			/// <remarks>	Elapsed time no longer increases. </remarks>
			///
			/// <param name="handle">	The handle. </param>
			///
			/// <see cref="GetTime"/>
			/// <see cref="CreateStopWatchTimer"/>
			///
			/// <returns>	A GW3DResult. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL GW3DResult PauseStopWatchTimer( /*in*/ StopWatchHandle handle );

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Starts stopwatch timer. </summary>
			///
			/// <param name="handle">	The handle. </param>
			///
			/// <see cref="GetTime"/>
			/// <see cref="CreateStopWatchTimer"/>
			///
			/// <returns>	A GW3DResult. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL GW3DResult StartStopWatchTimer( /*in*/ StopWatchHandle handle );

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the elapsed time. </summary>
			///
			/// <param name="handle">	The handle. </param>
			///
			/// <see cref="GetTime"/>
			/// <see cref="CreateStopWatchTimer"/>
			///
			/// <returns>	The time. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL float GetTime( StopWatchHandle handle );

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets average time. </summary>
			///
			/// <remarks>	Mean time to date based on the number of times the timer has been
			///				paused (ie finished sessions) and the current total time.  Currently
			///				its a rolling average over the last 10 values. </remarks>
			///
			/// <param name="handle">	The handle. </param>
			/// 
			/// <see cref="CreateStopWatchTimer"/>
			///
			/// <returns>	The average time. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL float GetAverageTime( StopWatchHandle handle  );

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets standard deviation time. </summary>
			///
			/// <param name="handle">	The handle. </param>
			/// 
			/// <see cref="CreateStopWatchTimer"/>
			///
			/// <returns>	The standard deviation time. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL float GetStandardDeviationTime( StopWatchHandle handle  );

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets internal SDK time. </summary>
			///
			/// <remarks>	Total elapsed SDK time in milliseconds </remarks>
			///
			/// <returns>	The internal SDK time. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL float GetInternalSDKTime( );
		}

		/* Functions for querying Geoweb3d license information */
		namespace SDK_License
		{

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Geoweb3d license details. </summary>
			///
			/// <remarks>	Additional license information may exists and can be queried
			///				from the object itself. LicenseDetails below is a helper for the
			///				basic information. </remarks>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			struct LicenseDetails
			{

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Values that represent license properties. </summary>
				////////////////////////////////////////////////////////////////////////////////////////////////////

				enum LicenseProperties
				{
					CUSTOMER				= 0,	// Customer Name
					CONTRACT				= 1,	// Contract (if applicable)
					EXPIRATION				= 2,	// Expiration date
					LICENSE_EXPIRATION_DAYS	= 3,	// Days until license expires
					GOODONCE				= 4,	// True if license can only be activated once
					ISSUED					= 5,	// Date the license was issued
					OPTIONAL_INFO			= 6,	// Optional info
					LICENSE_TYPE			= 7,	// 0 is purchased, non-0 is demo/eval
					LI_PARAM_MAX			= 0xff
				};
			};

			//////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary> TODO </summary>
			///
			/// <returns> TODO </returns>
			//////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL const IGW3DPropertyCollection *GetLicenseProperty();
		}

		/* Information about SDK state.
		*  Use at your own risk right now, these APIs may change once we know what stats are important
		*  to end users.
		*/
		namespace SDK_Statistics
		{

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets general tasks pending. </summary>
			///
			/// <remarks>	Tasks pending execution. </remarks>
			///
			/// <returns>	The general tasks pending. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL unsigned GetGeneralTasksPending();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets number of lidar tasks pending. </summary>
			///
			/// <remarks>	Lidar related tasks pending execution. </remarks>
			///
			/// <returns>	The number of lidar tasks pending. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL unsigned GetNumOfLidarTasksPending();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets 3D model tasks pending. </summary>
			///
			/// <remarks>	Model tasks pending execution. </remarks>
			///
			/// <returns>	The number 3D model tasks pending. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL unsigned Get3DModelTasksPending();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets master frame count. </summary>
			///
			/// <returns>	The master frame count. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL unsigned GetMasterFrameCount(); 

		}

		/* Notification functions for SDK feedback */
		namespace SDK_Notification
		{

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Geoweb3d set I/O function. </summary>
			///
			/// <remarks>	Sets an I/O functions for the user to get verbose information back. </remarks>
			///
			/// <param name="t">   	The GeoWeb3d_InfoType to process. </param>
			/// <param name="func">	The function. </param>
			///
			/// <returns>	A GeoWeb3d_InfoFunction. </returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL GeoWeb3d_InfoFunction 	GeoWeb3dSetIOFunction(GeoWeb3d_InfoType t, GeoWeb3d_InfoFunction func);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Set default I/O functions. </summary>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL void GeoWeb3dSetDefaultIOFunctions();
		}

		namespace Vector
		{

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets number of lidar pages per frame. </summary>
			///
			/// <remarks>	POC FOR LIDAR </remarks>
			///
			/// <param name="num_pages">	Number of pages to allow per frame. </param>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3D_DLL void SetNumberOfLidarPagesPerFrame( int num_pages );

		}

	//	enum GW3DSplineType 
	//	{
	//		CATMULLROM = 0,
	//		BEZIER = 1,
	//		KOCHANEKBARTELS = 2,
	//		CARDINAL = 3,
	//		CTEND = 0xFFFF
	//	};

	//	struct GW3DControlPoint
	//	{	
	//		struct{
	//			double x, y, z;
	//		}position;
	//	
	//		struct{
	//			double x, y, z;
	//		}tangent;
	//	};

	//	////////////////////////////////////////////////////////////////////////////////////////////////////
	//	/// <summary>	A position helper. </summary>
	//	////////////////////////////////////////////////////////////////////////////////////////////////////

	//	struct  GW3D_DLL IGW3DSpline
	//	{
	//		virtual bool     is_BeginReached() const = 0;
	//		virtual bool     is_EndReached()   const= 0;
	//		virtual void	 get_Position(double *x, double *y, double *z,  unsigned int *segment_used)    const= 0;
	//		virtual void     increment_world_position(double p)= 0;
	//		virtual void     put_world_position(double p)= 0;
	//		virtual void     increment_normalized_position(float p)= 0;
	//		virtual void     put_normalized_position(float p)= 0;
	//		virtual double   get_Length()      const= 0;
	//		
	//		//TBD if we use a formal collection of GW3DControlPoint*

	//		static GW3D_DLL IGW3DSplinePtr create(GW3DControlPoint* controlPoints, 
	//			unsigned  num_controlPoints, 
	//			GW3DSplineType splinetype, 
	//		// **************************
	//		//float tension, float continuity, float bias are used for KOCHANEKBARTELS
	//		// tangentStrength is for CARDINAL  (note, if .5, CARDINAL is a CATMULLROM)
	//		// resolution is used for all
	//		float tangentStrength = 0.0001f, float tension= 0.8f, float continuity = .01f, float bias =  0.01f, float resolution = 1000.0f, unsigned interface_version = GEOWEB3D_INTERFACE_VERSION );

	//	};


	//	/// <summary>
	//	/// affected by this spline
	//	/// </summary>
	//	enum GW3DSplineAnimiation
	//	{
	//		LocationSplineAnimiation = 0,
	//		RotationSplineAnimiation = 1,
	//		ScaleSplineAnimiation = 2,
	//		GW3DSplineAnimiationEND = 0xFFFF
	//	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	///// <summary>	An animation helper. </summary>
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	//struct  GW3D_DLL IGW3DAnimation
	//{	
	//	virtual void put_Spline(IGW3DSplinePtr spline, GW3DSplineAnimiation animiationtype ) = 0;
	//	
	//	//NOTE THIS IS A BIND TO ADDRESS!!!!! so this just fills in "your" the variables when they change.
	//	//currently debating if I should use radians.. tbd with the opengl stuff
	//	//if not given a spline animation target, scales will always be 1. rotations will always be 0, and locations will always be 0.
	//	virtual void BindComponent(double* locX, double* locY, double* locZ, double* rotX, double* rotY, double* rotZ, double* scaleX, double* scaleY, double* scaleZ) = 0;
	//	//Absolute time
	//	virtual void update(float t) = 0;
	//	///Starts from the beginning
	//	virtual void start() = 0;
	//	virtual void stop() = 0;
	//	virtual void resume() = 0;

	//	virtual void put_IsLooped(bool isLooped) = 0;
	//	virtual void put_Speed( float speed = 1.0f) = 0;
	//	
	//	static GW3D_DLL IGW3DAnimationPtr create( unsigned interface_version = GEOWEB3D_INTERFACE_VERSION );
	//};

}


