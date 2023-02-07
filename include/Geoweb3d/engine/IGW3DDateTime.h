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
    /// <summary>	The camera date and time interface. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DDateTime
    {
        virtual ~IGW3DDateTime() {}

        ///////// LocationReference.  This is enabled by default,

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets whether to follow camera location for reference. </summary>
        ///
        /// <remarks>	Enabled by default. Since time is local based on latitude, this setting uses the
        /// 			camera's latitude to determine where on the globe the specified time should be set.
        /// 			If enabled, the value given via put_LocationReference will be ignored. </remarks>
        ///
        /// <param name="enable">	true to enable following, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_isFollowCameraLocationForReference(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Checks if following camera location for reference. </summary>
        ///
        /// <returns>	true if following enabled, false if disabled. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_IsFollowCameraLocationForReference() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the location reference. </summary>
        ///
        /// <remarks>	The SDK will use the given longitude as the reference for where to set the time.
        /// 			If put_isFollowCameraLocationForReference is set to true, this value will be ignored
        /// 			and the camera longitude will be used instead. </remarks>
        ///
        /// <param name="longitude">	The longitude. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void   put_LocationReference(const double& longitude) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the location reference. </summary>
        ///
        /// <returns>	The location reference. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual double get_LocationReference() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets whether to use computer date. </summary>
        ///
        /// <remarks>	If set to true, the computer date will be used for the date and the date set via
        /// 			the other APIs will be ignored. </remarks>
        ///
        /// <param name="enable">	true to use computer date, false to use set date. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_isUseComputerDate(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Checks if using computer date. </summary>
        ///
        /// <returns>	true if using computer date, false if not. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_IsUseComputerDate() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets date using a delimited string. </summary>
        ///
        /// <remarks>	From delimited date string where with order year-month-day
        /// 			eg: 2002-1-25, 2002/1/25 etc. </remarks>
        ///
        /// <param name="val">	The date string. </param>
        ///
        /// <returns>	true if it succeeds, false if it fails. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool put_DateFromDelimitedString(const char* val) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the date as a delimited string. </summary>
        ///
        /// <remarks>	To YYYY-MM-DD where all components are integers. "2002-01-31". </remarks>
        ///
        /// <returns>	The date as a delimited string. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual  IGW3DStringPtr get_DateFromDelimitedString() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Puts date from an undelimited string. </summary>
        ///
        /// <remarks>	Date from iso type date string where with order year month day eg: 20020125. </remarks>
        ///
        /// <param name="val">	The date string. </param>
        ///
        /// <returns>	true if it succeeds, false if it fails. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool put_DateFromUnDelimitedString(const char* val) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets date from undelimited string. </summary>
        ///
        /// <remarks>	To YYYYMMDD where all components are integers. "20020131". </remarks>
        ///
        /// <returns>	The date from un delimited string. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStringPtr get_DateFromUnDelimitedString() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets whether to use computer time. </summary>
        ///
        /// <remarks>	If enabled, the camera will display the time based on the computer's local time.
        /// 			The value set by other APIs will be ignored. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_isUseComputerTime(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets whether using computer time. </summary>
        ///
        /// <returns>	true if using computer time, false if not. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_IsUseComputerTime() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the time. </summary>
        ///
        /// <param name="hour">  	The hour. </param>
        /// <param name="minute">	The minute. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_Time(int hour, int minute) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the time. </summary>
        ///
        /// <param name="hour">  	[out] The hour. </param>
        /// <param name="minute">	[out] The minute. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_Time(int& hour, int& minute) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets time and date </summary>
        ///
        /// <remarks>	If you have any of the "autos" enabled, those parts of this may get ignored. </remarks>
        ///
        /// <param name="reference_longitude">	The reference longitude. </param>
        /// <param name="year">				  	The year. </param>
        /// <param name="month">			  	The month. </param>
        /// <param name="day">				  	The day. </param>
        /// <param name="hour">				  	The hour. </param>
        /// <param name="minute">			  	The minute. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_TimeOfDay(double reference_longitude, int year, int month, int day, int hour, int minute) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets time and date. </summary>
        ///
        /// <param name="reference_longitude">	[out] The reference longitude. </param>
        /// <param name="year">				  	[out] The year. </param>
        /// <param name="month">			  	[out] The month. </param>
        /// <param name="day">				  	[out] The day. </param>
        /// <param name="hour">				  	[out] The hour. </param>
        /// <param name="minute">			  	[out] The minute. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_TimeOfDay(double& reference_longitude, int& year, int& month, int& day, int& hour, int& minute) const = 0;

    };
}