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
// file:	SmokeController.cpp
//
// summary:	Implements the smoke controller class
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <set>
#include <cmath>
#include "SmokeController.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "GeoWeb3dCore/LayerParameters.h"


using namespace Geoweb3d;

float frand()
{
	return float(rand()) / float(RAND_MAX);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="rep">	The rep. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

SmokeController::SmokeController(Geoweb3d::IGW3DVectorRepresentationWPtr rep)
    : rep_(rep)
    , elevation_offset_(0)
    ,last_elevation_seen_(0)
	, rad(0.01f)
	, delX(0)
	, delY(0)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Destructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

SmokeController::~SmokeController() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets smoke elevation offset. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="elevation">	The elevation. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SmokeController::SetSmokeElevationOffset( float elevation )
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
bool up = true;
bool sw = true;
float change = 0.0f;
float plume_rad_change = 1.0f;
float red_delta = 0.01f;

bool SmokeController::OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result )
{

	const Geoweb3d::IGW3DAttributeCollection *attributesconst = result->get_AttributeCollection() ;
    //Geoweb3d::IGW3DAttributeCollection *attributes = result->get_ModifyableAttributeProperties() ;

	GW3DGeometry * geometry = result->get_EditableGeometry();
	long object_id = result->get_ObjectID();
   // if(object_id == 0)
		rad += 0.01f;

    if(!geometry) //must be we are not in edit mode????
        return false;
		
    if(geometry->get_GeometryType() == gtPOINT_25D)
    {        		
		GW3DPoint* pt = dynamic_cast<GW3DPoint*>( geometry );
        //pt->put_Z ( pt->get_Z() + elevation_offset_);
        //pt->put_X(pt->get_X() + 0.00005 );
		//pt->put_Y(pt->get_Y() - 0.000005);
        //last_elevation_seen_ =  pt->get_Z();


	//	pt->put_X(pt->get_X() - delX);
	//	pt->put_Y(pt->get_Y() - delY);

		//circle
		/*if (sin(rad) > 0)
			delX = 0.0005;
		else
			delX = -0.0005;
		if (cos(rad) > 0)
			delY = 0.0005;
		else
			delY = 0.0005;*/
		
		delX = sin(rad) * 0.000001f;
		delY = cos(rad) * 0.000001f;

		/*if (up){
			if (sw){
			change += 0.00002;
			delX = 0.00002;
			}
			else{
				change += 0.00001;
				delX = 0.00001;
			}
			if (change > 0.00099)up = false;
		}
		else{			
			if (sw){
				change -= 0.00002;
				delX = -0.00002;
			}
			else{
				change -= 0.00001;
				delX = -0.00001;
			}
			if (change < -0.00099)up = true;
		}*/
		//sw = !sw;

		pt->put_X(pt->get_X() + delX);
		pt->put_Y(pt->get_Y() + delY);

    }
    else
    {
        printf("Need to add code for this other geometry type\n");
    }

    const Geoweb3d::IGW3DPropertyCollection* representation_properties = result->get_VectorRepresentationProperties(rep_);
	if (!representation_properties)
	{
		//unsafe_get saves some overhead vs a lock, but should only be used when you are sure the object can't get deleted while you are using it
		representation_properties = rep_.unsafe_get()->get_PropertyCollection();
	}

    if(representation_properties)
    {
		//two ways of defining the target property:
		//static int property_index = Vector::ModelProperties::HEADING; //by enum...

		//static int heading_property_index = representation_properties->get_DefinitionCollection()->get_IndexByName("HEADING"); //...or, by string
		////this is the optimized way to set the representation property!
		//double heading = representation_properties->get_Property(Vector::SmokeProperties::HEADING );
		//heading +=  10.0;
		//result->put_VectorRepresentationProperty(rep_, heading_property_index, heading);

		//static int max_rad_property_index = representation_properties->get_DefinitionCollection()->get_IndexByName("PLUME_MAX_RAD"); //...or, by string
		////this is the optimized way to set the representation property!
		//double plume_max_rad = representation_properties->get_Property(Vector::SmokeProperties::PLUME_MAX_RAD);
		//if (plume_max_rad > 15)
		//	plume_rad_change = -1.0;
		//if (plume_max_rad < 5)
		//	plume_rad_change = +1.0;
		//	
		//plume_max_rad += plume_rad_change;
		//	result->put_VectorRepresentationProperty(rep_, max_rad_property_index, plume_max_rad);

		if(1){					
			float red = representation_properties->get_Property(Vector::SmokeProperties::RED);
			red += red_delta;
			if (red > 1.0)
			{
				red = 1.0f;
				red_delta = -0.01f;
			}
			if (red < 0.0)
			{
				red = 0.0f;
				red_delta = 0.01f;
			}				

			static int red_property_index = representation_properties->get_DefinitionCollection()->get_IndexByName("RED"); 	
			result->put_VectorRepresentationProperty(rep_, red_property_index, red);
		}
	}

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the error action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SmokeController::OnError(/*todo*/ )
{
    //something was done by the user in the streamer that messed things up so bad we have to tell you about it.
	return false;
}