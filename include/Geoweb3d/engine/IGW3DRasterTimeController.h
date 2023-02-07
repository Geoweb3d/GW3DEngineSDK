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
#include "../core/GW3DInterFace.h"
#include "../core/IGW3DCollection.h"

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DRasterLayerDimension
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned long get_ID() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DRasterLayerWPtr get_Layer() = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DRasterTimeSlice : public IGW3DRasterLayerDimension
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DStringPtr get_TimeDelimitedString() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned long get_LastID() const = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DRasterTimeSliceCollection : public IGW3DCollection< IGW3DRasterTimeSlicePtr >
	{

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum TimeSliceUpdateType
	{
		//Expect all time slices in the IGW3DRasterTimeSliceCollection to now be different
		ALL_SLICES_DIFFERENT = 1,
		//Expect the IGW3DRasterTimeSliceCollection values to have be shifted by one position with a new IGW3DRasterTimeSlicePtr
		//to be in the last index of the collection. 
		SLICES_SHIFTED = 2,
		NO_SHIFT = 3
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> A IGW3DRasterTimeControllerCallback is useful to catch changes to the underlying 
	/// Time Controller data structures, specifically the time slice collection. If the TimeSliceUpdateType
	/// indicates a change, then a new IGW3DRasterTimeSliceCollectionPtr must be obtained, and if using 
	/// a IGW3DPlanetRasterTimePlayer then RegisterRasterTimeControllerWithPlanet must be invoked again. </summary>
	///
	/// <remarks> 
	/// The callback is meant mostly to tell a client that the time range of the Time Controller data has changed.
	/// Typically this is spawned from AutoUpdateTimeExtentState::AUTO_UPDATE_TIME_EXTENT_ON. 
	/// </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  IGW3DRasterTimeControllerCallback
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Contents of your IGW3DRasterTimeSliceCollection that was retrieved by get_IGW3DRasterTimeSliceCollection has changed
		/// Any local tracking of those values by a client needs to updated. </summary>
		///
		/// <param name="update_type"> TODO </param>
		/// <param name="shifted_by"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnTimeSlicesHaveChanged(TimeSliceUpdateType update_type, int shifted_by) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	The Time Controller that controls the time aspects of an individual raster Layer or
	///	a collection of raster layers. Changes to the time controller will be propagated to any representations
	/// that are linked to the data sources.
	/// For example if the time controller changes the time extents then the imagery representations of the raster layer
	/// will change what planet time it is seen at. </summary>
	///
	/// <remarks> 
	/// This was first designed where most use cases of a controller were made from a raster layer of a WMS-T type.  
	/// WMS-T should come with valid time extents and a valid interval resolution of where unique time stamped imagery can be queried. 
	/// Important WMS-T time dimension attributes of interest are "current" and "nearestValue".
	///
	/// Some of the APIs of the interface were put in place to deal with some WMS-T datasets that didn't have valid extents or intervals.
	/// We supply the ability to change the number of time slices or extents (which should only really be valid for a
	/// "nearestValue=1" type WMS-T time attribute type)
	///
	/// Note: A single raster layer does not need to be of type WMS-T. Number of time slices would not pertain but all other APIs would.
	///
	/// Another mode of the interface is that of controlling time aspects of a collection of raster layers.  For this case
	/// most the APIs are intuitive except for changing the slice number - as this is already set in stone by the number of raster layers. 
	/// Note: if any raster layers in the collection are of a type WMS-T : that layers time meta data is not considered. 
	///
	/// refer to: https://www.ogc.org/standards/wms WMS spec 1.3.0 Annex C for more information. 
	/// </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DRasterTimeController
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Gets the start time as an ISO Delimited String </summary>
		///
		/// <returns>	The ISO start time </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DStringPtr get_StartTime() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Gets the start time as an ISO Delimited String </summary>
		///
		/// <returns>	The ISO end time </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DStringPtr get_EndTime() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Configures the start and end time of the controller. </summary>
		///
		/// <remarks> 
		/// We are only currently accepting universal times of this format: yyyy-mm-ddThh:mm:ssZ (ie: 1981-10-30T00:04:00Z)
		/// </remarks>
		///
		/// <param name="start_time"> TODO </param>
		/// <param name="end_time"> TODO </param>
		///
		/// <returns>	A GW3DResult indication of the state of the execution.   
		/// GW3D_eNotimpl will be returned if get_Modifiable is false
		/// GW3D_sOk will be return is the time state has changed an been propagated thru the system </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_TimeExtents(const IGW3DStringPtr& start_time, const IGW3DStringPtr& end_time) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Configures the time extents relative to the system time. </summary>
		///
		/// <remarks> 
		/// A positive duration will assume you want the start time to be at the current local universal time.
		/// A negative duration will assume you want the end time to be at the current local universal time. 
		///
		/// Pertaining to a Time Controller created from a WMS-T Raster Layer:
		/// Some WMS-T datasets may have invalid time extents in their capabilities files.  This is one way to seed them to some workable values.
		/// Internally we move the extents using a modulus. We mod the start time with the slice duration (computed with the number of slices).
		/// And shift the extents so that they fall on specific time values.  What this allows for is the reuse of time based caches at the expense 
		/// of the slight shift from the current time. 
		///
		/// Note: care must be taken when dealing with WMS-T datasets that have a time dimension attribute of nearestValue=0
		/// refer to: https://www.ogc.org/standards/wms WMS spec 1.3.0 Annex C for more information. 
		/// Any adjustments using this API will surely not honor the exact slice values that some servers may expect. 
		///
		/// Pertaining to a Time Controller created of Raster Layer Collection:
		/// No such adjustment mentioned above is preformed for this controller.
		/// </remarks>
		///
		/// <param name="minutes_duration"> TODO </param>
		///
		/// <returns>	A GW3DResult indication of the state of the execution.   
		/// GW3D_eNotimpl will be returned if get_Modifiable is false
		/// GW3D_sOk will be return is the time state has changed an been propagated thru the system </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_TimeExtent(int minutes_duration) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Gets the current number of Time slices </summary>
		///
		/// <param name="num_slices"> TODO </param>
		///
		/// <returns> A GW3DResult indication of the state of the execution. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult get_NumTimeSlices(unsigned int& num_slices) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change the number of time slices. </summary>
		///
		/// <remarks>
		/// Only pertains to a Time Controller created from a WMS-T Raster Layer. Does not make sense to change the 
		/// slices of a non-WMS-T layer or a Time Controller constructed from a raster layer collection. All following discussion is for
		/// single WMS-T raster layer Time Controllers that has get_Modifiable = true;
		///
		/// Some WMS-T datasets may have invalid time resolution (intervals) in their Capabilities.  This API is one way to seed it to some workable values.
		/// Internally we move the extents using a modulus. We mod the start time with the slice duration (computed with the number of slices).
		/// And shift the extents so that they fall on specific time values.  What this allows for is the reuse of time based caches at the expense 
		/// of the slight shift from the current time. 
		///
		/// Note: care must be taken when dealing with WMS-T datasets that have a time dimension attribute of nearestValue=0
		/// refer to: https://www.ogc.org/standards/wms WMS spec 1.3.0 Annex C for more information. 
		/// Any adjustments using this API will surely not honor the exact slice values that some servers may expect. 
		/// </remarks>
		///
		/// <param name="num_slices"> TODO </param>
		///
		/// <returns>	A GW3DResult of indication of the state of the execution.   
		/// GW3D_eNotimpl will be returned if get_Modifiable is false
		/// GW3D_sOk will be return is the time state has changed an been propagated thru the system </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_NumTimeSlices(unsigned int num_slices) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum AutoUpdateTimeExtentState
		{
			AUTO_UPDATE_TIME_EXTENT_OFF = 1,
			AUTO_UPDATE_TIME_EXTENT_ON = 2
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Put the controller in an auto update mode. </summary>
		///
		/// <remarks>
		/// The system will refresh the layer after timer expires; which counts down from the time slice duration.
		/// A Note for WMS-T type Layer: 
		/// Typically the time dimension listed in the capabilities files will have current="1" if it is expected
		/// to support a mode such as this.  Care must be taken to know if your service supports this mode.
		/// The reason we allow this to be set for WMS-T layer that has current set to "0" is that we've
		/// seen some datasources that support current even though they didn't follow the spec. 
		///
		/// This mode is not implemented for a time controller of a group of raster layers
		/// </remarks>
		///
		/// <param name="state"> TODO </param>
		///
		/// <returns>	A GW3DResult indication of the state of the execution.  
		/// A GW3D_eNotimpl will be returned for a Time controller of a list of raster layers.  </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_AutoUpdateTimeExtentState(AutoUpdateTimeExtentState state) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Gets whether the auto update mode currently active.</summary>
		///
		/// <param name="state"> TODO </param>
		///
		/// <returns>	A GW3DResult indication of the state of the execution.   </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult get_AutoUpdateTimeExtentState(AutoUpdateTimeExtentState& state) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Refreshes the controller to get in sync with the system time. </summary>
		///
		/// <remarks>
		/// Only auto updates if AutoUpdateTimeExtentState is active via put_AutoUpdateTimeExtentState
		/// If sufficient time has elapsed then a return result of GW3D_s0k will be returned,
		/// and the time values will shift in time. 
		/// Any previous IGW3DRasterTimeSliceCollectionPtr will be out of date. 
		/// If a IGW3DRasterTimeControllerCallback has been registered it will get triggered as well.
		/// </remarks>
		///
		/// <param name="update_type"> TODO </param>
		/// <param name="shifted_by"> TODO </param>
		///
		/// <returns>	A GW3DResult indication of the state of the execution.   </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult refresh(TimeSliceUpdateType& update_type, int& shifted_by) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Gets a snapshot of the current time slice collection. </summary>
		///
		/// <remarks>
		/// Here the client controls the memory with a smart pointer.  This object will not change. It is a snapshot of the 
		/// current time slice collection when this api was invoked. 
		/// If IGW3DRasterTimeControllerCallback triggers an update or the refresh of this controller indicates a change has occurred
		/// then the client must get a new IGW3DRasterTimeSliceCollectionPtr via this call. 
		/// </remarks>
		///
		/// <returns>A IGW3DRasterTimeSliceCollection smart pointer object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DRasterTimeSliceCollectionPtr get_TimeSliceCollection() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Get if the controller is able to modify is time extents and slices. </summary>
		///
		/// <remarks>
		/// This specifically pertains to if a WMS-T layer has nearestValue set to "0" in it's capabilities file. 
		/// Which would mean this controller is not modifiable because the time values are rigid. 
		/// If not modifiable the we have disabled put_TimeExtent, put_TimeExtents and put_NumTimeSlices APIs.
		/// </remarks>
		///
		/// <returns> True if modifiable </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Modifiable() = 0;
	};
}