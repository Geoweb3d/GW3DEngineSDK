/**
 * Property of Geoweb3d: GIS SDK
 * --------------------------------
 * Copyright 2008-2016 
 * Author: Ethan R. Paoletti, Geoweb3d
 * Geoweb3d SDK is not free software: you cannot redistribute it and/or modify
 * it under any terms unless we have a written agreement between us.
 * Geoweb3d SDK and example applications are distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  
**/

////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	SpotlightController.cpp
//
// summary:	Implements the spotlight controller class
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <set>
#include <cmath>
#include "SpotlightController.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "GeoWeb3dCore/LayerParameters.h"


using namespace Geoweb3d;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <remarks>	Geoweb3d, 06/10/2016. </remarks>
///
/// <param name="rep">	The rep. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

SpotlightController::SpotlightController(Geoweb3d::IGW3DVectorRepresentationWPtr rep)
		: rep_(rep),
		  Lights_Identified_(false),
		  Next_Traffic_Light_State_(eREDLIGHT),
		  Pitch_Scrambler_(180.0)
{}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Destructor. </summary>
///
/// <remarks>	Geoweb3d, 06/10/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

SpotlightController::~SpotlightController() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Determines the next state for the traffic lights. </summary>
///
/// <remarks>	Geoweb3d, 06/10/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
void SpotlightController::UpdateTrafficLightState()
{
	switch(Next_Traffic_Light_State_)
	{
		case eREDLIGHT:
			Next_Traffic_Light_State_ = eGREENLIGHT;
			break;
		case eYELLOWLIGHT:
			Next_Traffic_Light_State_ = eREDLIGHT;
			break;
		case eGREENLIGHT:
			Next_Traffic_Light_State_ = eYELLOWLIGHT;
			break;
		default:
			printf("Invalid traffic signal state, aborted!\n");
			return;				
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the stream action. </summary>
///
/// <remarks>	Geoweb3d, 06/13/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpotlightController::OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result )
{
	static  const  Geoweb3d::IGW3DDefinitionCollection* repdef = rep_.lock()->get_PropertyCollection( )->get_DefinitionCollection();

    const  Geoweb3d::IGW3DAttributeCollection *attibutes = result->get_AttributeCollection();

	//this is just showing how to drive down into the defintions if interested...
    const  Geoweb3d::IGW3DDefinitionCollection* def = attibutes->get_DefinitionCollection();

	// Get the DESC_ index of the current vector layer result
	int index = def->get_IndexByName("DESC_");


	if(index < 0)
    {
        printf("File does not contain a DESC_ definition, aborted!\n");
        return false;
    }

	// Get the property value out of the DESC_ definition index
    const Geoweb3d::IGW3DVariant &val = attibutes->get_Property( index );

	const char *strVal = val.raw_string();


	// If the property value is "Traffic Signal Pole" this result requires special handling
	if(!strcmp(strVal,"Traffic Signal Pole"))
	{
		// Add initial setup settings to the traffic signal pole
		if(!Lights_Identified_)
		{
			result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::PITCH, Pitch_Scrambler_);
			result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::INTENSITY, 50.0);
			result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::INNER_CUTOFF_ANGLE, 15.0);

			// Toggle the pitch by 180 for the next set of traffic signal poles
			if(Pitch_Scrambler_ == 180)
			{
				Pitch_Scrambler_ = 360;
			}
			else
			{
				Pitch_Scrambler_ = 180;
			}			
		}
		
		
		// Change the light color of this traffic signal pole to the next color of the traffic light state machine signal
		switch(Next_Traffic_Light_State_)
		{
			case eREDLIGHT:
				result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::RED, 100.0);
				result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::GREEN, 0.0);
				result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::BLUE, 0.0);
				break;
			case eYELLOWLIGHT:
				result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::RED, 100.0);
				result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::GREEN, 100.0);
				result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::BLUE, 0.0);
				break;
			case eGREENLIGHT:
				result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::RED, 0.0);
				result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::GREEN, 100.0);
				result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::BLUE, 0.0);
				break;
			default:
				printf("Invalid traffic signal state, aborted!\n");
				return false;				
		}

		// Update the traffic light next state signal
		UpdateTrafficLightState();
	}	
	else
	{
		// Add initial setup settings of all of the utility poles that are not traffic signal poles.
		if(!Lights_Identified_)
		{
			result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::INTENSITY, 500.0);
			result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::INNER_CUTOFF_ANGLE, 80.0);
		}
	}

	// Set all the spotlight's headings to 90.0
	if(!Lights_Identified_)
	{
		result->put_VectorRepresentationProperty(rep_, Geoweb3d::Vector::SpotLightProperties::HEADING, 90.0);
	}


    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the error action. </summary>
///
/// <remarks>	Geoweb3d, 06/13/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SpotlightController::OnError(/*todo*/ )
{
    //something was done by the user in the streamer that messed things up so bad we have to tell you about it.
	return false;
}