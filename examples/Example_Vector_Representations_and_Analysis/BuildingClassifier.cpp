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
// file:	BuildingClassifier.cpp
//
// summary:	Implements the building classifier class
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "BuildingClassifier.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="rep">	The rep. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

BuildingClassifier::BuildingClassifier(Geoweb3d::IGW3DVectorRepresentationWPtr rep) : rep_(rep),nIndex_(0)
{
    // selection_set_fids_.push_back(100);
    // selection_set_fids_.push_back(1000);
    // selection_set_fids_.push_back(10000);
    // selection_set_fids_.push_back(1000000);
    // selection_set_fids_.push_back(1);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Destructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

BuildingClassifier::~BuildingClassifier() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets the count. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long BuildingClassifier::count() const
{
    return static_cast<unsigned long>( selection_set_fids_.size() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Nexts the given pp value. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="ppVal">	[in,out] If non-null, the value. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool BuildingClassifier::next( int64_t *ppVal )
{
    if(nIndex_ >= selection_set_fids_.size())
    {
        *ppVal = 0;
        reset();
        return false;
    }

    *ppVal =  get_AtIndex( nIndex_ );
    ++nIndex_;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Resets this object. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void BuildingClassifier::reset()
{
    nIndex_ = 0;
}
int64_t BuildingClassifier::operator[](unsigned long index)
{
    return get_AtIndex( index );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets at index. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="index">	Zero-based index of the. </param>
///
/// <returns>	at index. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

int64_t BuildingClassifier::get_AtIndex( unsigned long index )
{
    if(index < selection_set_fids_.size() )
        return selection_set_fids_[ index ];

    return 0;

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

bool BuildingClassifier::OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result )
{

    Color color = Color(1.0, 1.0, 1.0);
    bool classify_color_ = false;

    const  Geoweb3d::IGW3DAttributeCollection *attibutes = result->get_AttributeCollection();

    //this is just showing how to drive down into the definitions if interested...
    const  Geoweb3d::IGW3DDefinitionCollection* def = attibutes->get_DefinitionCollection();

    //incase you don't know, "Function" is a field name in the shapefile
    int index = def->get_IndexByName("Function");

    const Geoweb3d::IGW3DVariant &val = attibutes->get_Property( index );

    const char *category = val.raw_string();
    if (!strcmp(category, "Police Station"))
    {
        classify_color_ = true;
		color = Color(0.0, 0.0, 1.0);
    }
    else if (!strcmp(category, "Hotel"))
    {
        classify_color_ = true;
		color = Color(1.0, 0.5, 0.0);
    }
    else if (!strcmp(category, "Hospital"))
    {
        classify_color_ = true;
		color = Color(0.0, 1.0, 0.0);
    }
    else if (!strcmp(category, "Fire Station"))
    {
        classify_color_ = true;
		color = Color(1.0, 0.0, 0.0);
    }
    else
    {
        classify_color_ = false;
    }


    if(classify_color_)
    {

        //NOTE! Geoweb3d::IGW3DPropertyCollectionPtr defaultsettings could be a scratchpad instead of cloning every
        //fid.  Just an FYI if you are looking to gain performance/keep memory fragmentation low
        Geoweb3d::IGW3DPropertyCollectionPtr defaultsettings = rep_.lock()->get_PropertyCollection( )->create_Clone();
        const  Geoweb3d::IGW3DDefinitionCollection* repdef = defaultsettings->get_DefinitionCollection();

        int color_red_index = repdef->get_IndexByName("RED");
        int color_green_index = repdef->get_IndexByName("GREEN");
        int color_blue_index = repdef->get_IndexByName("BLUE");

        defaultsettings->put_Property(color_red_index, color.red  );
        defaultsettings->put_Property(color_green_index, color.green);
        defaultsettings->put_Property(color_blue_index, color.blue );

        rep_.lock()->put_PropertyCollection( result->get_ObjectID(),  defaultsettings);
    }


    //		//NOTE! what I want to do here is detect if this fid already has unique settings, where if
    //		//it is, this return the defaults where rep_settings_->isSameAsLayerDefaults will be true.
    //		//then if they go to change a value different than the global settings, an internal
    //		//copy takes place and this fid is then put on the unique list.
    //		someobject*rep_settings_ =  rep_->get_ObjectIDPropertyCollection( result->get_ObjectID() );

    //		//Note, now sure if I will have this done in time for the cpp interface release.. but some ideas.
    //		NOW! when all this classification is completed we can add something like this:
    //		//this will go through and find the most common settings and return them
    //		someobject*mostcommonrep_settings *rep_->FindOptimalDefaultLayerValues();

    //		//where now:
    //		rep_->ApplyNewDefaultSettings(mostcommonrep_settings)
    //		//will remove any fids on a unique list if they are the same

    //		//rep_settings_->isSameAsLayerDefaults() ?
    //		//rep_settings_->isSameAsApplicationGlobalDefaults() ?

    //		//Set the color in the representation parameter group
    //		//rep_settings_.SetProperty(color_red_index, color_.red);
    //		//rep_settings_.SetProperty(color_green_index, color_.green);
    //		//rep_settings_.SetProperty(color_blue_index, color_.blue);
    //	}

    // keep processing.. if you want to break early, return false.
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the error action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool BuildingClassifier::OnError(/*todo*/ )
{
    //something was done by the user in the streamer that messed things up so bad we have to tell you about it
	return false;
}

