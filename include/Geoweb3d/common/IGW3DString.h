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
        /// <summary>	A string. </summary>
        ///
        /// <remarks>	The Geoweb3d string object is used when the long-term validity of the string is
        /// 			not guaranteed within the SDK.  It is usually provided as a strong pointer, where
        /// 			the SDK client owns the memory. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        struct GW3D_DLL IGW3DString
        {
            virtual ~IGW3DString()
            {}

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Equality operator. </summary>
            ///
            /// <param name="s">	The string to process. </param>
            ///
            /// <returns>	true if the parameters are considered equivalent. </returns>
            ////////////////////////////////////////////////////////////////////////////////////////////////////

            virtual bool operator == (const char* s) const = 0;

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Equality operator. </summary>
            ///
            /// <param name="s">	The string to process. </param>
            ///
            /// <returns>	true if the parameters are considered equivalent. </returns>
            ////////////////////////////////////////////////////////////////////////////////////////////////////

            virtual bool operator == (const IGW3DString& s) const = 0;

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Equality operator. </summary>
            ///
            /// <param name="s">	The string to process. </param>
            ///
            /// <returns>	true if the parameters are considered equivalent. </returns>
            ////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SWIGGING
            virtual bool operator == (const IGW3DStringPtr& s) const = 0;
#endif

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Equality operator. </summary>
            ///
            /// <param name="s">	The string to process. </param>
            ///
            /// <returns>	true if the parameters are considered equivalent. </returns>
            ////////////////////////////////////////////////////////////////////////////////////////////////////

            virtual bool operator == (const IGW3DString* s) const = 0;

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Equality operator. </summary>
            ///
            /// <param name="s">	The string to process. </param>
            ///
            /// <returns>	true if the parameters are considered equivalent. </returns>
            ////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SWIGGING
            virtual bool operator == (const IGW3DStringPtr s) const = 0;
#endif

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Sets the stored string. </summary>
            ///
            /// <param name="str">	The string value. </param>
            ////////////////////////////////////////////////////////////////////////////////////////////////////

            virtual void put_string(const char* str) = 0;

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Gets the string value. </summary>
            ///
            /// <returns>	null if it fails, else the value. </returns>
            ////////////////////////////////////////////////////////////////////////////////////////////////////

            virtual const char* c_str() const = 0;

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Empties this string. </summary>
            ///
            /// <returns>	true if it succeeds, false if it fails. </returns>
            ////////////////////////////////////////////////////////////////////////////////////////////////////

            virtual bool empty() const = 0;

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Gets the size of the string. </summary>
            ///
            /// <returns>	The size. </returns>
            ////////////////////////////////////////////////////////////////////////////////////////////////////

            virtual unsigned size() const = 0;

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Gets the string value as a char*. </summary>
            ///
            /// <returns>	the string value. </returns>
            ////////////////////////////////////////////////////////////////////////////////////////////////////

            virtual operator const char* () const = 0;

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Create a new string. </summary>
            ///
            /// <param name="str">	The string value </param>
            /// <param name="interface_version">	(optional) the interface version. </param>
            ///
            /// <returns>	The new string. </returns>
            ////////////////////////////////////////////////////////////////////////////////////////////////////

            static IGW3DStringPtr create(const char* str, unsigned interface_version = GEOWEB3D_INTERFACE_VERSION);
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Equality operator. </summary>
        ///
        /// <param name="s">  	The first instance to compare. </param>
        /// <param name="str">	The second instance to compare. </param>
        ///
        /// <returns>	true if the parameters are considered equivalent. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        inline bool operator==(const IGW3DStringPtr s, const char* str)
        {
            return  *s.get() == str;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Equality operator. </summary>
        ///
        /// <param name="str">	The first instance to compare. </param>
        /// <param name="s">  	The second instance to compare. </param>
        ///
        /// <returns>	true if the parameters are considered equivalent. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        inline bool operator== (const char* str, const IGW3DStringPtr& s)
        {
            return  *s.get() == str;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Equality operator. </summary>
        ///
        /// <param name="str">	The first instance to compare. </param>
        /// <param name="s">  	The second instance to compare. </param>
        ///
        /// <returns>	true if the parameters are considered equivalent. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        inline bool operator== (const IGW3DStringPtr& str, const IGW3DStringPtr& s)
        {
            return  *s.get() == *str.get();
        }
}