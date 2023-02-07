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
// file:	BirdAttributeMapper.cpp
//
// summary:	Implements the people controller class
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <set>

#include "BirdAttributeMapper.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/IGW3DImage.h"
#include "engine/IGW3DImageCollection.h"
#include "GeoWeb3dCore/LayerParameters.h"


using namespace Geoweb3d;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <param name="rep">	The rep. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

BirdAttributeMapper::BirdAttributeMapper( Geoweb3d::IGW3DImageCollectionPtr imagepalettein, int filename_property_index, int fid_property_idx, Geoweb3d::IGW3DVectorRepresentationWPtr rep)
    : rep_(rep)
    , imagepalette( imagepalettein )
	, filename_property_index_( filename_property_index)
	, fid_property_idx_( fid_property_idx )
{
	rep_texture_property_index_ = rep.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("TEXTURE_PALETTE_INDEX"); //by string

	// prevent color change for billboards
	const char* rep_name = rep.lock()->get_Driver().lock()->get_Name();
	if ( strcmp( rep_name, "BillBoard" ) == 0 )
		return;

	int red_index = rep.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("RED"); //by string
	int green_index = rep.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("GREEN"); //by string
	int blue_index = rep.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("BLUE"); //by string

	//we want to blend with white if the rep also supports color
	if(red_index != -1)
	{
		Geoweb3d::IGW3DPropertyCollectionPtr defaultprops = rep_.lock()->get_PropertyCollection( true )->create_Clone();
		defaultprops->put_Property(red_index, 1.0);
		defaultprops->put_Property(green_index, 1.0);
		defaultprops->put_Property(blue_index, 1.0);
		rep_.lock()->put_PropertyCollection( defaultprops );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Destructor. </summary>
///
////////////////////////////////////////////////////////////////////////////////////////////////////

BirdAttributeMapper::~BirdAttributeMapper() {}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the stream action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="result">	[out] if non-null, the result. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool BirdAttributeMapper::OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result )
{
	const Geoweb3d::IGW3DAttributeCollection *attributesconst = result->get_AttributeCollection() ;

	//this will have the url that was set when made the datasource layer
	const Geoweb3d::IGW3DVariant &vurl = attributesconst->get_Property(filename_property_index_);

	if(!vurl.is_string())
	{
		printf("Example bug detection, property of the datasource is not even a string!  Should be a path/url\n");
		return true;
	}

	const Geoweb3d::IGW3DPropertyCollection* representation_properties = result->get_VectorRepresentationProperties(rep_);

	IGW3DImageWPtr img = imagepalette->get_ByName( vurl.raw_string() );
	if ( !img.expired() )
	{
		if ( !representation_properties )
		{
			//this rep does not have unique properties yet and is using representation layer defaults.  So here
			//we clone from the layer defaults and set our unique properties.  We can target the specific property,
			//which will create a clone within the SDK from the current representation layer defaults.  Other option
			//we could do is clone the layer defaults and set all of them.
			result->put_VectorRepresentationProperty( rep_, rep_texture_property_index_, img.lock()->get_PropertyCollectionID() );
		}
		else
		{
			result->put_VectorRepresentationProperty( rep_, rep_texture_property_index_, img.lock()->get_PropertyCollectionID() );
		}
	}
	else
	{
		const Geoweb3d::IGW3DVariant &vfid = attributesconst->get_Property( fid_property_idx_ );
		int fid = vfid.to_int();
		result->put_VectorRepresentationProperty( rep_, rep_texture_property_index_, fid );
	}

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the error action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool BirdAttributeMapper::OnError(/*todo*/ )
{
    //something was done by the user in the streamer that messed things up so bad we have to tell you about it.
	return false;
}