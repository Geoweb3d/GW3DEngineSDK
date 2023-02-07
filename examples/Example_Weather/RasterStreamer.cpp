#include "StdAfx.h"
#include "RasterStreamer.h"

#include <Geoweb3d\common\GW3DCommon.h>
#include <Geoweb3d\engine\GW3DRaster.h>
#include <Geoweb3dCore\LayerParameters.h>
#include "engine/IGW3DRasterLayerEnvelopeStream.h"
#include "engine/GW3DImage.h"

RasterStreamer::RasterStreamer()
{}


RasterStreamer::~RasterStreamer()
{}



bool RasterStreamer::OnStream(Geoweb3d::IGW3DRasterLayerEnvelopeStreamResult* result)
{
	//This 
	unsigned long time_id = result->get_ID();

	printf("Returning an image with time id %d", time_id);
	return true;
}


bool RasterStreamer::OnError(/*todo*/)
{
	//something was done by the user in the streamer that messed things up so bad we have to tell you about it.
	return false;
}