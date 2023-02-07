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

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A contents filter. </summary>
	///
	/// <remarks>	Filters what data will be reported back.  If disabled, no filtering will take place 
	///				and all data that is paged in will be reported. </remarks>
	///
	///	<see cref="IGW3DFeatureFeedbackConfiguration::get_ContentsFilter"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DContentsFilter
	{
		virtual ~IGW3DContentsFilter() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the enabled state of the contents filter. </summary>
		///
		/// <remarks>	Query whether this contents filter is enabled.  If not enabled, all the settings
		///				in this class are ignored and no filtering takes place. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Enabled() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets whether the contents filter is enabled. </summary>
		///
		/// <remarks>	Enable or disable this contents filter.  If not enabled, all the settings
		///				in this class are ignored and no filtering takes place. </remarks>
		///
		/// <param name="enable"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_Enabled(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Values representing how the minimum radius for this contents filter is determined. 
		///				</summary>
		///
		/// <see cref="IGW3DContentsFilter::put_MinRadiusMode"/>
		/// <see cref="IGW3DContentsFilter::put_MinRadiusValue"/>
		/// <see cref="IGW3DContentsFilter::get_MinRadiusMode"/>
		/// <see cref="IGW3DContentsFilter::get_MinRadiusValue"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum MIN_RADIUS_MODE
		{
			/// <summary>	Use the minimum clamp radius of the representation. </summary>
			MIN_DEFAULT = 0,
			/// <summary>	Use the minimum radius value as set in put_MinRadiusValue, or the minimum 
			///				clamp radius of the representation, whichever is less (the minimum). </summary>
			MIN_RADIUS_MIN = 1,
			/// <summary>	Use the minimum radius value as set in put_MinRadiusValue, or the minimum
			///				clamp radius of the representation, whichever is greater (the maximum). </summary>
			MIN_RADIUS_MAX = 2,
			///	<summary>	Max enum value, DO NOT USE	</summary>
			MIN_RADIUS_MODE_MAX = 0xFFFF
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Values representing how the maximum radius for this contents filter is determined. 
		///				</summary>
		///
		/// <see cref="IGW3DContentsFilter::put_MaxRadiusMode"/>
		/// <see cref="IGW3DContentsFilter::put_MaxRadiusValue"/>
		/// <see cref="IGW3DContentsFilter::get_MaxRadiusMode"/>
		/// <see cref="IGW3DContentsFilter::get_MaxRadiusValue"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum MAX_RADIUS_MODE
		{
			/// <summary>	Use the maximum clamp radius of the representation. </summary>
			MAX_DEFAULT = 0,
			/// <summary>	Use the maximum radius value as set in put_MaxRadiusValue, or the maximum 
			///				clamp radius of the representation, whichever is less (the minimum). </summary>
			MAX_RADIUS_MIN = 1,
			/// <summary>	Use the maximum radius value as set in put_MaxRadiusValue, or the maximum 
			///				clamp radius of the representation, whichever is greater (the maximum). </summary>
			MAX_RADIUS_MAX = 2,
			/// <summary>	Allow all data as far out as it is loaded
			TO_PAGING_DISTANCE = 3,
			///	<summary>	Max enum value, DO NOT USE	</summary>
			MAX_RADIUS_MODE_MAX = 0xFFFF
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the mode by which the minimum radius of the filter will be determined.
		///				</summary>
		///
		///	<remarks>	Control over the minimum radius of the filter is useful when you wish to get 
		///				information about objects that are in the field of view and paged in, but not 
		///				rendered.	</remarks>
		///
		/// <param name="MIN_RADIUS_MODE"> TODO </param>
		///
		/// <see cref="IGW3DContentsFilter::get_MinRadiusMode"/>
		/// <see cref="IGW3DContentsFilter::put_MinRadiusValue"/>
		/// <see cref="IGW3DContentsFilter::put_MaxRadiusMode"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_MinRadiusMode(MIN_RADIUS_MODE mode) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the minimum radius, in meters, below which data will be filtered out and not
		///				reported. </summary>
		///
		///	<remarks>	Control over the minimum radius of the filter is useful when you wish to get 
		///				information about objects that are in the field of view and paged in, but not 
		///				rendered.	</remarks>
		///
		/// <param name="value"> TODO </param>
		///
		/// <see cref="IGW3DContentsFilter::get_MinRadiusValue"/>
		/// <see cref="IGW3DContentsFilter::put_MinRadiusMode"/>
		/// <see cref="IGW3DContentsFilter::put_MaxRadiusValue"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_MinRadiusValue(float value) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the mode by which the minimum radius of the filter will be determined. </summary>
		///
		/// <see cref="IGW3DContentsFilter::put_MinRadiusMode"/>
		/// <see cref="IGW3DContentsFilter::get_MinRadiusValue"/>
		/// <see cref="IGW3DContentsFilter::get_MaxRadiusMode"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual MIN_RADIUS_MODE get_MinRadiusMode() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the minimum radius, in meters, below which data will be filtered out and not
		///				reported. </summary>
		///
		/// <see cref="IGW3DContentsFilter::put_MinRadiusValue"/>
		/// <see cref="IGW3DContentsFilter::get_MinRadiusMode"/>
		/// <see cref="IGW3DContentsFilter::get_MaxRadiusValue"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual float get_MinRadiusValue() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the mode by which the maximum radius of the filter will be determined. </summary>
		///
		///	<remarks>	Control over the maximum radius of the filter is useful when you wish to get 
		///				information about objects that are in the field of view and paged in, but not 
		///				rendered.	</remarks>
		///
		/// <param name="MAX_RADIUS_MODE"> TODO </param>
		///
		/// <see cref="IGW3DContentsFilter::get_MaxRadiusMode"/>
		/// <see cref="IGW3DContentsFilter::put_MaxRadiusValue"/>
		/// <see cref="IGW3DContentsFilter::put_MinRadiusMode"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_MaxRadiusMode(MAX_RADIUS_MODE mode) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the maximum radius, in meters, above which data will be filtered out and not
		///				reported. </summary>
		///
		///	<remarks>	Control over the maximum radius of the filter is useful when you wish to get 
		///				information about objects that are in the field of view and paged in, but not 
		///				rendered.	</remarks>
		///
		/// <param name="value"> TODO </param>
		///
		/// <see cref="IGW3DContentsFilter::get_MaxRadiusValue"/>
		/// <see cref="IGW3DContentsFilter::put_MaxRadiusMode"/>
		/// <see cref="IGW3DContentsFilter::put_MinRadiusValue"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_MaxRadiusValue(float value) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the mode by which the maximum radius of the filter will be determined. </summary>
		///
		/// <see cref="IGW3DContentsFilter::put_MaxRadiusMode"/>
		/// <see cref="IGW3DContentsFilter::get_MaxRadiusValue"/>
		/// <see cref="IGW3DContentsFilter::get_MinRadiusMode"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual MAX_RADIUS_MODE get_MaxRadiusMode() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the maximum radius, in meters, above which data will be filtered out and not
		///				reported. </summary>
		///
		/// <see cref="IGW3DContentsFilter::put_MaxRadiusValue"/>
		/// <see cref="IGW3DContentsFilter::get_MaxRadiusMode"/>
		/// <see cref="IGW3DContentsFilter::get_MinRadiusValue"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual float get_MaxRadiusValue() const = 0;

	};
}