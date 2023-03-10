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
// file:	OpenGLAttributeMapper.h
//
// summary:	
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "engine/IGW3DVectorLayerStream.h"
#include "common/IGW3DVariant.h"

class OpenGLAttributeMapper : public Geoweb3d::IGW3DVectorLayerStream
{

public:
    OpenGLAttributeMapper( Geoweb3d::IGW3DOpenGLTextureCollectionPtr imagepalette, int datasource_layers_color_property_index, Geoweb3d::IGW3DVectorRepresentationWPtr rep);
    virtual ~OpenGLAttributeMapper(void);
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
protected:
  Geoweb3d::IGW3DOpenGLTextureCollectionPtr palette_;
  int datasource_layers_color_property_index_;
  int rep_texture_property_index_;
  Geoweb3d::IGW3DVectorRepresentationWPtr rep_;
};