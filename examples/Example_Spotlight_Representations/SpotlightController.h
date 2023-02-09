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
// file:	SpotlightController.h
//
// summary:	Declares the spotlight controller class
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "engine/IGW3DVectorLayerStream.h"
#include "common/IGW3DVariant.h"
#include "time.h"

// The possible states of the traffic signal poles' state machine
enum eTrafficLightState {eREDLIGHT = 0,
                         eYELLOWLIGHT = 1,
                         eGREENLIGHT = 2};




////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Spotlight controller. </summary>
///
/// <remarks>	Geoweb3d, 06/10/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class SpotlightController : public Geoweb3d::IGW3DVectorLayerStream
{

public:
    SpotlightController(Geoweb3d::IGW3DVectorRepresentationWPtr rep);
    virtual ~SpotlightController(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Updates the traffic signal pole's color state machine. </summary>
	///
	/// <remarks>	Geoweb3d, 06/13/2016. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void UpdateTrafficLightState();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Changes the Lights_Identified_ bool to true when called. </summary>
	///
	/// <remarks>	Geoweb3d, 06/13/2016. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void FinalizeUtilityPoleSetup()
	{
		Lights_Identified_ = true;
	}

	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Executes the stream action. </summary>
	///
	/// <remarks>	Geoweb3d, 06/13/2016. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Executes the error action. </summary>
	///
	/// <remarks>	Geoweb3d, 06/13/2016. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool OnError(/*todo*/ );

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
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
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

	/// <summary>	The rep. </summary>
    Geoweb3d::IGW3DVectorRepresentationWPtr rep_;
   
	/// <summary> The signal that signifies that the traffic lights have been initially identified. </summary>
	bool Lights_Identified_;

	/// <summary>	The variable used to scramble the pitch values of the traffic lights. </summary>
	float Pitch_Scrambler_;

	/// <summary>	The next light state to load into a traffic light. </summary>
	eTrafficLightState Next_Traffic_Light_State_;

};
