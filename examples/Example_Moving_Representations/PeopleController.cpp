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
// file:	PeopleController.cpp
//
// summary:	Implements the people controller class
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <set>

#include "PeopleController.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "GeoWeb3dCore/LayerParameters.h"


using namespace Geoweb3d;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="rep">	The rep. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

PeopleController::PeopleController(Geoweb3d::IGW3DVectorRepresentationWPtr rep)
    : rep_(rep)
    , elevation_offset_(0)
    ,last_elevation_seen_(0)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Destructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

PeopleController::~PeopleController() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets model elevation offset. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="elevation">	The elevation. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void PeopleController::SetModelElevationOffset( float elevation )
{
    elevation_offset_ = elevation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the stream action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="result">	[out] if non-null, the result. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

 void PeopleController::visit(GW3DPoint* pt) 
{
	if (pt->is_3D())
	{
		pt->put_Z(pt->get_Z() + elevation_offset_);
		pt->put_X(pt->get_X() + 0.00005);
		last_elevation_seen_ = pt->get_Z();
	}
	else
	{
		printf("-not 3D-");
	}
}

 void PeopleController::visit(GW3DLineString* p)  
{
	 if (p->is_3D())
	 {
		 printf("-not implemented-");
	 }
	 else
	 {
		 printf("-not 3D-");
	 }
}

 void PeopleController::visit(GW3DLinearRing*p) 
{
	 if (p->is_3D())
	 {
		 printf("-not implemented-");
	 }
	 else
	 {
		 printf("-not 3D-");
	 }
}

 void PeopleController::visit(GW3DPolygon*p) 
{
	 if (p->is_3D())
	 {
		 printf("-not implemented-");
	 }
	 else
	 {
		 printf("-not 3D-");
	 }
}

 void PeopleController::visit(GW3DMultiPoint*p) 
{
	 if (p->is_3D())
	 {
		 printf("-not implemented-");
	 }
	 else
	 {
		 printf("-not 3D-");
	 }
}

 void PeopleController::visit(GW3DMultiLineString*p) 
{
	 if (p->is_3D())
	 {
		 printf("-not implemented-");
	 }
	 else
	 {
		 printf("-not 3D-");
	 }
}

 void PeopleController::visit(GW3DMultiPolygon*p) 
{
	 if (p->is_3D())
	 {
		 printf("-not implemented-");
	 }
	 else
	 {
		 printf("-not 3D-");
	 }
}

bool PeopleController::OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result )
{
	
	const Geoweb3d::IGW3DAttributeCollection *attributesconst = result->get_AttributeCollection() ;
    //Geoweb3d::IGW3DAttributeCollection *attributes = result->get_ModifyableAttributeProperties() ;

	GW3DGeometry * geometry = result->get_EditableGeometry();



    if(!geometry) //must be we are not in edit mode????
        return false;

	//if (result->get_ObjectID() == -1)
	{
		geometry->accept(this);

		const Geoweb3d::IGW3DPropertyCollection* representation_properties = result->get_VectorRepresentationProperties(rep_);
		if (!representation_properties)
		{
			//unsafe_get saves some overhead vs a lock, but should only be used when you are sure the object can't get deleted while you are using it
			representation_properties = rep_.unsafe_get()->get_PropertyCollection();
		}

		if (representation_properties)
		{
			//two ways of defining the target property:
			//static int property_index = Vector::ModelProperties::HEADING; //by enum...
			static int heading_property_index = representation_properties->get_DefinitionCollection()->get_IndexByName("HEADING"); //...or, by string

			//this is the optimized way to set the representation property!
			double heading = representation_properties->get_Property(Vector::ModelProperties::HEADING);
			heading += 10.0;
			result->put_VectorRepresentationProperty(rep_, heading_property_index, heading);

			//      static const bool targeted_property_set_test = true;
			//      static IGW3DVariant johnval("JOHN"); //John is only created once (as its static), so after the first hash query, a string compare is pretty cheap

			//      // for 2006 hardware:
			//      // 222ms if targeted_property_set_test = *false* for 100k models
			//      // vs. 144ms if  targeted_property_set_test = *true*.

			//      //Thus, a PropertyCollection copy adds a lot of time, but if you are replacing most of the properties, its probably
			//      //still faster than targeting all the individual property sets...
			//      if(targeted_property_set_test)
			//      {
			//          //THIS EXPECTS you already have settings for this specific object id/feature id!
			//          //this will return false/fail if it does not.
			//          result->put_VectorRepresentationProperty(rep_, heading_property_index, scratchpad_ );

			//          //when dealing with strings, its faster using the hash compare.
			//		  if(result->get_AttributeCollection()->get_Property(0).get_HashValue () == johnval.get_HashValue())
			//          {
			//              //just use the same value for pitch as the heading...
			//              result->put_VectorRepresentationProperty(rep_, Vector::ModelProperties::PITCH, scratchpad_ );
			//          }
			//      }
		}
	}

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the error action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool PeopleController::OnError(/*todo*/ )
{
    //something was done by the user in the streamer that messed things up so bad we have to tell you about it.
	return false;
}