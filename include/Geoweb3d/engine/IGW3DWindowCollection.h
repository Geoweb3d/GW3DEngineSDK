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
#include "../../Geoweb3dCore/Geoweb3dTypes.h"
#include "IGW3DWindow.h"
#include "../core/IGW3DCollection.h"
#include "GW3DExports.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of 3D windows. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DWindowCollection : public IGW3DCollection< IGW3DWindowWPtr >
    {
        virtual ~IGW3DWindowCollection() {}

#ifndef SWIGGING

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Array indexer operator. </summary>
        ///
        /// <param name="index">	Zero-based index of the window. </param>
        ///
        /// <returns>	The indexed value. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DWindowWPtr operator[](unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets at index. </summary>
        ///
        /// <param name="index">	Zero-based index of the window. </param>
        ///
        /// <returns>	at index. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DWindowWPtr get_AtIndex(unsigned long index) const = 0;
#endif //SWIGGING

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a 3D Window. </summary>
        ///
        /// <remarks>	Note: dwStyle is not yet implemented.  If parent, currently the sdk sets the size
        /// 			to that of the wndParent and ignores x,y,w,h, which may change once dwStyle is
        /// 			properly supported. </remarks>
        ///
        /// <param name="title">	  	The title. </param>
        /// <param name="windowStyle">	The window style. </param>
        /// <param name="x">		  	The x coordinate position of the window on the display. </param>
        /// <param name="y">		  	The y coordinate position of the window on the display. </param>
        /// <param name="nWidth">	  	The width. </param>
        /// <param name="nHeight">	  	The height. </param>
        /// <param name="wndParent">  	(Optional) The parent to this window. </param>
        /// <param name="stereodrv">  	(Optional) The stereo driver (if null, anaglyph is still available). </param>
        /// <param name="pCallback">  	[in] (Optional) If non-null, the window callback. </param>
        ///
        /// <returns>	The new window, or an expired pointer if failed. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DWindowWPtr create_3DWindow(
            const char* title
            , unsigned long  windowStyle
            , int x, int y, int nWidth, int nHeight
            , OSWinHandle wndParent
            ,  /*optional*/ IGW3DStereoDriverWPtr stereodrv = IGW3DStereoDriverWPtr()
            ,  /*optional*/ IGW3DWindowCallback* pCallback = 0) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Closes a window. </summary>
        ///
        /// <remarks>	Closes a window and removes it from the collection.
        ///
        /// 			To close a window: IGW3DWindowCollection::close( window );
        ///
        ///				IGW3DWindowWPtr will become expired as well as all resources dependent
        ///				on this window. </remarks>
        ///
        /// <param name="window">	[in] The window. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static void close(IGW3DWindowWPtr& window)
        {
            close_Window(window);
        }
    };
}