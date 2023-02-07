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
// file:	OpenGLPaletteCreator.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <set>
#include <windows.h>
#include <gl/gl.h>

#include "OpenGLPaletteCreator.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "engine/IGW3DSceneGraphContext.h"
#include "engine/IGW3DOpenGLTextureCollection.h"
#include "engine/IGW3DOpenGLTexture.h"

#pragma comment(lib, "OpenGL32.lib") 
GLuint red_texture_id = -1;
GLuint green_texture_id = -1;

void  CreateCheckboard()
{
    glGenTextures( 1, &red_texture_id);
	glGenTextures( 1, &green_texture_id);

   // std::cout << "no_texture_id: " << no_texture_id << std::endl;

    unsigned char redcheckImage[32][32][4];
	unsigned char greencheckImage[32][32][4];

    int i,j, c;

	int width_height_out = 32;
    for (i=0; i<32; i++)
    {
        for (j=0; j<32; j++)
        {
            c = (((i&0x8)==0)^((j&0x8)==0))*255;
            redcheckImage[i][j][0] = (GLubyte)255;
            redcheckImage[i][j][1] = (GLubyte)c;
            redcheckImage[i][j][2] = (GLubyte)c;
            redcheckImage[i][j][3] = (GLubyte)255;

            c = (((i&0x8)==0)^((j&0x8)==0))*255;
            greencheckImage[i][j][0] = (GLubyte)c;
            greencheckImage[i][j][1] = (GLubyte)255;
            greencheckImage[i][j][2] = (GLubyte)c;
            greencheckImage[i][j][3] = (GLubyte)255;
        }
    }

	glBindTexture(GL_TEXTURE_2D, red_texture_id );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, redcheckImage );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );


	glBindTexture(GL_TEXTURE_2D, green_texture_id );
	glTexImage2D(  GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, greencheckImage );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glBindTexture(GL_TEXTURE_2D, 0 );

}

using namespace Geoweb3d;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <param name="rep">	The rep. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

OpenGLPaletteCreator::OpenGLPaletteCreator(Geoweb3d::IGW3DGeoweb3dSDKPtr &sdk_context, int color_property_index) : color_property_index_(color_property_index)
{
	palette = sdk_context->get_SceneGraphContext()->create_OpenGLTextureCollection();
	CreateCheckboard();

	red_ = palette->create( red_texture_id, 32, 32);
	green_ = palette->create(green_texture_id ); //test the gpu readback
	
	green_.lock()->put_ReadBackGPUParametersNow();
	if(green_.lock()->get_Width() !=32)
		printf("BUG DETECTED? DID NOT GET EXPECTED WITH OF 32 PIXELS\n");
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Destructor. </summary>
///
////////////////////////////////////////////////////////////////////////////////////////////////////

OpenGLPaletteCreator::~OpenGLPaletteCreator() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the stream action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="result">	[out] if non-null, the result. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool OpenGLPaletteCreator::OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result )
{

	const Geoweb3d::IGW3DAttributeCollection *attributesconst = result->get_AttributeCollection() ;
  
	const Geoweb3d::IGW3DVariant &vurl = attributesconst->get_Property(color_property_index_);

	int colorID = vurl.to_int();

	switch(colorID)
	{
	case 1:
		printf(".. red detected in the datasource layer\n");
		break;
	case 2:
		printf(".. green detected in the datasource layer\n");
		break;	
	
	}
	return true;

}

Geoweb3d::IGW3DOpenGLTextureFinalizationTokenPtr OpenGLPaletteCreator::GetPalletToken()
{
	return palette->create_FinalizeToken();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the error action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool OpenGLPaletteCreator::OnError(/*todo*/ )
{
    //something was done by the user in the streamer that messed things up so bad we have to tell you about it.
	return false;
}