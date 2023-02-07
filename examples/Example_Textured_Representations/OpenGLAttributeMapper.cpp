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

#include "OpenGLAttributeMapper.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/IGW3DImage.h"
#include "engine/IGW3DOpenGLTextureCollection.h"
#include "engine/IGW3DOpenGLTexture.h"
#include "GeoWeb3dCore/LayerParameters.h"


extern unsigned int red_texture_id;
extern unsigned int green_texture_id;


using namespace Geoweb3d;


/////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <param name="rep">	The rep. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

OpenGLAttributeMapper::OpenGLAttributeMapper( Geoweb3d::IGW3DOpenGLTextureCollectionPtr palettein, int datasource_layers_color_property_index, Geoweb3d::IGW3DVectorRepresentationWPtr rep)
    : rep_(rep)
    , palette_( palettein )
	, datasource_layers_color_property_index_( datasource_layers_color_property_index)
{

	rep_texture_property_index_ = rep.lock()->get_PropertyCollection()->get_DefinitionCollection()->get_IndexByName("TEXTURE_PALETTE_INDEX"); //by string

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

OpenGLAttributeMapper::~OpenGLAttributeMapper() {}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the stream action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="result">	[out] if non-null, the result. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool OpenGLAttributeMapper::OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result )
{	
	static IGW3DOpenGLTextureWPtr rogl = palette_->get_ByOpenGLTextureID( red_texture_id );
	static IGW3DOpenGLTextureWPtr gogl = palette_->get_ByOpenGLTextureID( green_texture_id );
	
	const Geoweb3d::IGW3DAttributeCollection *attributesconst = result->get_AttributeCollection();
	
	//this will have the color id in the datasource's layer
	const Geoweb3d::IGW3DVariant &vurl = attributesconst->get_Property( datasource_layers_color_property_index_);

	int colorID = vurl.to_int();

	IGW3DOpenGLTextureWPtr color_to_use;
	switch(colorID)
	{
	case 1:
		//printf(".. red detected in the datasource layer\n");
		color_to_use = rogl;
		break;
	case 2:
		//printf(".. green detected in the datasource layer\n");
		color_to_use = gogl;
		break;
	default:
		color_to_use = gogl;
		break;
	
	}

	const Geoweb3d::IGW3DPropertyCollection* representation_properties = result->get_VectorRepresentationProperties(rep_);

	if(!representation_properties)
	{
		//this rep does not have unique properties yet and is using representation layer defaults.  So here
		//we clone from the layer defaults and set our unique properties.  We can target the specific property,
		//which will create a clone within the SDK from the current representation layer defaults.  Other option
		//we could do is clone the layer defaults and set all of them.
		result->put_VectorRepresentationProperty(rep_, rep_texture_property_index_, color_to_use.lock()->get_PropertyCollectionID() );
		
	
	}
	else
	{
		result->put_VectorRepresentationProperty(rep_, rep_texture_property_index_, color_to_use.lock()->get_PropertyCollectionID() );
	}



		

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the error action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool OpenGLAttributeMapper::OnError(/*todo*/ )
{
    //something was done by the user in the streamer that messed things up so bad we have to tell you about it.
	return false;
}