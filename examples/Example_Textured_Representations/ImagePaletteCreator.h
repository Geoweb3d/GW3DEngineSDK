/**
 * Property of Geoweb3d: GIS SDK
 * --------------------------------
 * Copyright 2008-2014 
 * Author: Vincent A. Autieri, Geoweb3d
 * Geoweb3d SDK is not free software: you cannot redistribute it and/or modify
 * it under any terms unless we have a written agreement between us.
 * Geoweb3d SDK and example applications are distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  
**/

////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	TexturePaletteCreator.h
//
// summary:	Declares the people controller class
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "engine/IGW3DVectorLayerStream.h"
#include "common/IGW3DVariant.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	People controller. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class ImagePaletteCreator : public Geoweb3d::IGW3DVectorLayerStream
{

public:
    ImagePaletteCreator( Geoweb3d::IGW3DGeoweb3dSDKPtr &sdk_context, int filename_property_index, int use_ip_camera_property_index );
    virtual ~ImagePaletteCreator(void);
    virtual bool OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result );
    virtual bool OnError(/*todo*/ ) ;


    // These are required to be implemented!  *NOTE* if
    // [count] returns 0, then no other of APIs to index
    // into selection_set_fids_ will get called.  What this
    // means is if you are streaming a whole layer, its safe to
    // return 0 for everything.

    virtual unsigned long count() const
    {
        return 0;
    }

    virtual bool next( int64_t *ppVal )
    {
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Resets this object. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void reset()
    {

    }

    virtual int64_t operator[](unsigned long index)
    {
        return 0;
    }
    virtual int64_t get_AtIndex( unsigned long index )
    {
        return 0;
    }

	Geoweb3d::IGW3DFinalizationTokenPtr GetPalletToken();


	Geoweb3d::IGW3DImageCollectionPtr imagepalette;

protected:
	int filename_property_index_;
	int use_ip_camera_property_index_;

};
