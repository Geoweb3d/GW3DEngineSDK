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
// file:	PeopleController.h
//
// summary:	Declares the people controller class
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "engine/IGW3DVectorLayerStream.h"
#include "common/IGW3DVariant.h"
#include "Geoweb3dCore/GeometryExports.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	People controller. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class PeopleController : public Geoweb3d::IGW3DVectorLayerStream, public Geoweb3d::IGW3DGeometryVisitor
{

public:
    PeopleController(Geoweb3d::IGW3DVectorRepresentationWPtr rep);
    virtual ~PeopleController(void);
    virtual bool OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result );
    virtual bool OnError(/*todo*/ ) ;

	virtual void visit(Geoweb3d::GW3DPoint*);
	// Visit GW3DLineString. 
	virtual void visit(Geoweb3d::GW3DLineString*);
	// Visit GW3DLinearRing. 
	virtual void visit(Geoweb3d::GW3DLinearRing*);
	// Visit GW3DPolygon. 
	virtual void visit(Geoweb3d::GW3DPolygon*);
	// Visit GW3DMultiPoint. 
	virtual void visit(Geoweb3d::GW3DMultiPoint*);
	// Visit GW3DMultiLineString. 
	virtual void visit(Geoweb3d::GW3DMultiLineString*);
	// Visit GW3DMultiPolygon. 
	virtual void visit(Geoweb3d::GW3DMultiPolygon*);


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

    void SetModelElevationOffset( float elevation );

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets the last elevation seen. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <returns>	the last elevation seen. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    double GetTheLastElevationSeen( )
    {
        return last_elevation_seen_;
    }

protected:
    /// <summary>	The last elevation seen. </summary>
    double last_elevation_seen_;
    double elevation_offset_;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>
    /// this is an expensive object to create and destroy, so keep a scratchpad/reusable object.
    /// </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    /// <summary>	The properties scratchpad. </summary>
    Geoweb3d::IGW3DPropertyCollectionPtr props_scratchpad_;
    /// <summary>	The rep. </summary>
    Geoweb3d::IGW3DVectorRepresentationWPtr rep_;
};
