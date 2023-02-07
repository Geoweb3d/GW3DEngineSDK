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
// file:	SpotlightTasks.h
//
// summary:	Declares the spotlight controller class
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "MiniEngine.h"

#include "engine/IGW3DVectorLayerStream.h"
#include "common/IGW3DVariant.h"

#include "SpotlightController.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Pre post draw tasks. </summary>
///
/// <remarks>	Geoweb3d, 06/13/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct PrePostDrawTasks
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPreDraw() = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the post draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPostDraw() =0;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Spotlight controller task. </summary>
///
/// <remarks>	Geoweb3d, 06/13/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class SpotlightControllerTask : public PrePostDrawTasks
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
    ///
    /// <param name="rep">	  	The rep. </param>
    /// <param name="pengine">	[in,out] If non-null, the pengine. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    SpotlightControllerTask(Geoweb3d::IGW3DVectorRepresentationWPtr rep, MiniEngine *pengine): stream_ (rep),pengine_(pengine)
    {
        layer_ = rep.lock()->get_VectorLayer();
        Spotlight_Pitch_ = 0.0;

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ~SpotlightControllerTask() {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPreDraw( )
    {
		// Initialize spotlight parameters
		controller_->SetGlobalSpotLightHeading(90.0, &stream_);

		controller_->SetGlobalSpotLightIntensity(255.0, &stream_);

		controller_->SetStreetLightController(&layer_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the post draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPostDraw()
    {
        layer_.lock()->Stream( &stream_ );
        
        // Move through all 360 degrees of pitch
	    controller_->SetGlobalSpotLightPitch(Spotlight_Pitch_, &stream_);

	    Spotlight_Pitch_ += 5;
       
	    // Ensure that we do not move past 360 degrees
	    if(Spotlight_Pitch_ > 360)
	    {
			 Spotlight_Pitch_ = 0;
	    }		   

    }


private:

	/// <summary>	The controller. </summary>
    SpotlightController controller_;
    /// <summary>	The stream. </summary>
    Geoweb3d::IGW3DVectorRepresentationWPtr stream_;
    /// <summary>	The layer. </summary>
    Geoweb3d::IGW3DVectorLayerWPtr layer_;
    /// <summary>	The pengine. </summary>
    MiniEngine *pengine_;

	/// <summary>	The global spotlight pitch. </summary>
	double Spotlight_Pitch_;
};


