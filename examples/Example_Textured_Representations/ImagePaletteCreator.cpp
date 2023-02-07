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
// file:	ImagePaletteCreator.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <set>

#include "ImagePaletteCreator.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "engine/IGW3DSceneGraphContext.h"
#include "engine/IGW3DImageCollection.h"
#include "GeoWeb3dCore/Geoweb3dWebImageExports.h"
#include "GeoWeb3dCore/Geoweb3dVideoCapture.h"

using namespace Geoweb3d;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <param name="rep">	The rep. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

ImagePaletteCreator::ImagePaletteCreator( Geoweb3d::IGW3DGeoweb3dSDKPtr &sdk_context, int filename_property_index, int use_ip_camera_property_index )
	: filename_property_index_( filename_property_index )
	, use_ip_camera_property_index_( use_ip_camera_property_index )
{
	imagepalette = sdk_context->get_SceneGraphContext()->create_ImageCollection();
	imagepalette->put_CollectionWidthHeightMode( IGW3DImageCollection::FORCED );
	imagepalette->put_Width( 1024 );
	imagepalette->put_Height( 1024 );

	//this will all be changing with the c++ api
	Geoweb3d::VideoCapture::SystemCaptureDeviceInfo system_info;
	Geoweb3d::VideoCapture::ScanVideoDevices(system_info);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Destructor. </summary>
///
////////////////////////////////////////////////////////////////////////////////////////////////////

ImagePaletteCreator::~ImagePaletteCreator() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the stream action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="result">	[out] if non-null, the result. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool ImagePaletteCreator::OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result )
{
	const Geoweb3d::IGW3DAttributeCollection *attributesconst = result->get_AttributeCollection() ;
   

	//this will have the url that was set when made the datasource layer
	const Geoweb3d::IGW3DVariant& vurl = attributesconst->get_Property( filename_property_index_ );
	const Geoweb3d::IGW3DVariant& vuse_ip = attributesconst->get_Property( use_ip_camera_property_index_ );

	if(!vurl.is_string())
	{
		printf("Example bug detection, property of the datasource is not even a string! Should be a path/url\n");
		return false;
	}

	if ( !vuse_ip.is_bool() )
	{
		printf("Example bug detection, property of the usage flag is not even a bool! \n");
		return false;
	}

	if ( vuse_ip.to_bool() == true )
	{
		Geoweb3d::GW3DResult result_img;

		Geoweb3d::IPCameraSourceHandle handle;
		Geoweb3d::create_Camera( vurl.raw_string(), handle );
		Geoweb3d::put_URI( handle, vurl.raw_string() );
		Geoweb3d::put_Connect( handle );
		Geoweb3d::get_Image_IP( handle, imagepalette, result_img );
	}
	else
	{
		imagepalette->create( vurl.raw_string() );
	}

	return true;


	//unsigned imagehandle;
	//Geoweb3d::WebImageStreamer::OpenUrl (vurl.raw_string() , vurl.raw_string(), 3, imagehandle);
	//
	//Geoweb3d::WebImageStreamer::get_Image(imagehandle,imagepalette,result_img);
    //return true;


	//if(!Geoweb3d::VideoCapture::isDeviceSetup(0))
	//{
	//	Geoweb3d::VideoCapture::SetupDevice(0/*,640,480*/);
	//}
	//Geoweb3d::VideoCapture::get_Image( 0, imagepalette, result_img);
	//return true;
}

Geoweb3d::IGW3DFinalizationTokenPtr ImagePaletteCreator::GetPalletToken()
{
	return imagepalette->create_FinalizeToken();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the error action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool ImagePaletteCreator::OnError(/*todo*/ )
{
    //something was done by the user in the streamer that messed things up so bad we have to tell you about it.
	return false;
}