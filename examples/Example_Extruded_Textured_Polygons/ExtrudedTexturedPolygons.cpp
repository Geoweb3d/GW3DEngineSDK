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
#include "stdafx.h"
#include <windows.h>
#include <deque>
#include <string>
#include <locale>
#include <algorithm>
#include <set>
#include <map>
#include <ctype.h>
#include <math.h>
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"

#include "engine/IGW3DSceneGraphContext.h"
#include "engine/IGW3DImageCollection.h"
#include "engine/IGW3DImage.h"
#include "engine/IGW3DEventStream.h"
//interfaces that will be going away.
#include "Geoweb3dCore/GeometryExports.h"
#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/LayerParameters.h"

#include "BuildingTextureMapping.h"
#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();


Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events );
Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle );
Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsExtruded( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer, bool createtextured, int default_top_texture_id, int default_side_texture_id );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Query if 'searchingfor' does string contain. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="searchingfor">	   	[in,out] The searchingfor. </param>
/// <param name="stringofinterest">	[in,out] The stringofinterest. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool DoesStringContain( std::string &searchingfor,  std::string &stringofinterest )
{
    std::transform(searchingfor.begin(), searchingfor.end(), searchingfor.begin(), toupper );
    std::transform(stringofinterest.begin(), stringofinterest.end(), stringofinterest.begin(), toupper );

    if(stringofinterest.find( searchingfor.c_str() )!=std::string::npos)
    {
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Mini engine. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class MiniEngine : public Geoweb3d::IGW3DEventStream, public Geoweb3d::IGW3DWindowCallback
{
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="sdk_context">	Context for the sdk. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context ) : sdk_context_(sdk_context) {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~MiniEngine() { }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Updates this object. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Update()
    {
        DoPreDrawWork();

        if( sdk_context_->draw( window_ ) == Geoweb3d::GW3D_sOk )
        {
            DoPostDrawWork();
            return true;
        }

        //return that we did not draw. (app closing?)
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Loads a configuration. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="example_name">	Name of the example. </param>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool LoadConfiguration(const char *example_name )
    {
        window_ = CreateWindowCreation( sdk_context_, example_name, this );

        if(window_.expired())return false;

        camera_ = CreateCamera(window_,"Camera 1");
        camera_controller_ = camera_.lock()->get_CameraController();

        if(camera_.expired())return false;

        sdk_context_->put_EnableEventStream( true );


        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Loads a data. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="rootpath">	The rootpath. </param>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool LoadData( const char *rootpath )
    {

        //test our attrib loader for rgb textures.

        Geoweb3d::IGW3DPropertyCollectionPtr properties =
            sdk_context_->get_SceneGraphContext()->get_FileAttributes("../examples/media/bldgs_shp/textures/Demo_apt_bld_roof_03.rgb.attr");

        if(properties)
        {
            //size_u Real world size u for floating point databases
            //size_v Real world size v for floating point databases

            unsigned u_index = properties->get_DefinitionCollection()->get_IndexByName("size_u");
            unsigned v_index = properties->get_DefinitionCollection()->get_IndexByName("size_v");
            printf("\n");
            printf("%s [%f]\n",properties->get_DefinitionCollection()->get_AtIndex(u_index)->property_description,(double)properties->get_Property(u_index));
            printf("%s [%f]\n",properties->get_DefinitionCollection()->get_AtIndex(v_index)->property_description, (double)properties->get_Property(v_index));
        }

        //now lets create a palette of textures for our representation to use.  Note that on the next version, the
        //sdk will be a bit smarter about when these are uploaded to the GPU.  Thus, if you have a desert palette
        //vs a tropical palette used in completely different locations of the earth, its probably better to
        //break them up like such, instead of one giant palette for the whole application.
        //Also, Expects all be the same IE, all images 3 channel, or all images 4 channels... again, these rules
        //both can and probably will be lifted in a future release.

        int roof_propertycollection_id = -1;
        int side_propertycollection_id = -1;

        int roof_propertycollection_id_for_oddnumbered_features = -1;

        //Order is not preserved.
        Geoweb3d::IGW3DImageCollectionPtr imagepalette = sdk_context_->get_SceneGraphContext()->create_ImageCollection();

        {
            //If you add a duplicate, it will return the same IGW3DImage
            Geoweb3d::IGW3DImageWPtr image = imagepalette->create("../examples/media/bldgs_shp/textures/Demo_apt_bld_roof_03.rgb");

            roof_propertycollection_id = image.lock()->get_PropertyCollectionID();

            //this also shows how you can store your attribute data to the iamge.  Here is using an index,
            //but you can also use a pointer as its a void * internally.
            int my_index_to_sum_user_attributedata = 0; //so it can be an index into your array for example
            image.lock()->put_UserData( reinterpret_cast<void*>( std::int64_t( my_index_to_sum_user_attributedata )));
        }

        {
            //If you add a duplicate, it will return the same IGW3DImage
            Geoweb3d::IGW3DImageWPtr image = imagepalette->create("../examples/media/bldgs_shp/textures/Demo_wh_bld_01_a.rgb");

            side_propertycollection_id = image.lock()->get_PropertyCollectionID();

            //this also shows how you can store your attribute data to the iamge.  Here is using an index,
            //but you can also use a pointer as its a void * internally.
			///so it can be an index into your array for example
            image.lock()->put_UserData( reinterpret_cast<void*>( std::int64_t( side_propertycollection_id ) ) );
        }

        {
            //If you add a duplicate, it will return the same IGW3DImage
            Geoweb3d::IGW3DImageWPtr image = imagepalette->create("../examples/media/bldgs_shp/textures/Demo_apt_bld_roof_02.rgb");

            roof_propertycollection_id_for_oddnumbered_features = image.lock()->get_PropertyCollectionID();

            //this also shows how you can store your attribute data to the iamge.  Here is using an index,
            //but you can also use a pointer as its a void * internally.
             //so it can be an index into your array for example
            image.lock()->put_UserData( reinterpret_cast<void*>( std::int64_t( roof_propertycollection_id_for_oddnumbered_features ) ) );
        }

        Geoweb3d::IGW3DImageWPtr imagetest;
        imagepalette->reset();
        while(imagepalette->next(&imagetest))
        {
            printf("file: [%s], userdata: [%i]\n",imagetest.lock()->get_ImageName(), static_cast<int>( reinterpret_cast<std::int64_t>(imagetest.lock()->get_UserData() ) ) );
        }

        //Note, you can reuse imagepalette all you want.. as its kinda of like your workspace.
        //You can also create more of them.. we don't really care.  However, we do not
        //try to reuse a same texture after you call create_FinalizeToken !! Thus, if
        //you call create_FinalizeToken on the same palette 1000 times, you just wasted
        //a lot of GPU memory!  So the general rule should be to try to be smart about
        //making your palettes.

        //Also, our propertyObjects only allow basic types, thus if you plan to attribute map,
        //You need to get your ID, which is what the callback does.

        Geoweb3d::IGW3DFinalizationTokenPtr reptoken = imagepalette->create_FinalizeToken();

        Geoweb3d::GW3DResult res;

        //NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
        //having to guess which data source driver is needed to open up a particular dataset.
        Geoweb3d::IGW3DVectorDataSourceWPtr data_source = sdk_context_->get_VectorDriverCollection()->auto_Open( "../examples/media/bldgs_shp/shp/pyo_commercial.shp", res);

        printf("Geoweb3d Data Detected the LV_AOI_com_a data(most likely) and will both represent it and attribute map it\n");


        Geoweb3d::IGW3DVectorLayerCollection *vlyrcollection = data_source.lock()->get_VectorLayerCollection();
        vlyrcollection->reset();

        Geoweb3d::IGW3DVectorLayerWPtr layer;

        while(vlyrcollection->next(&layer))
        {
            //for a shapefile, we only have 1 layer.. but.. just beware this
            //is would overwrite if it has more than 1 layer
            extruded_representation_ = RepresentAsExtruded(sdk_context_, layer, true, roof_propertycollection_id, side_propertycollection_id);
        }

        //ADD the texture palette to the representation so we can attribute map to it..

        extruded_representation_.lock()->put_GW3DFinalizationToken ( reptoken );

        BuildingTextureMapping texturemappingStream( extruded_representation_ );
        texturemappingStream.roof_propertycollection_id_for_oddnumbered_features_ = roof_propertycollection_id_for_oddnumbered_features;
        //now get the layer of this representation belongs to and stream
        //the changes
        extruded_representation_.lock()->get_VectorLayer().lock()->Stream( &texturemappingStream );


        //lets try to get a starting point for the camera by looking at all the vector data
        //and zoom to the center area of its bounds. This is more of an SDK test too, as
        //we go through all the vector drivers and gather all the layers etc. to find
        //the overall 2D bounding box.
        Geoweb3d::GW3DEnvelope env;

        Geoweb3d::IGW3DVectorDriverWPtr vdriver;
        while( sdk_context_->get_VectorDriverCollection()->next(&vdriver) )
        {
            //note, going through a lock() all the time creates alot of extra work
            //we can avoid when we are in a tight loop, as we know we are all good programers
            Geoweb3d::IGW3DVectorDataSourceCollection *vdscollection = vdriver.lock()->get_VectorDataSourceCollection();

            Geoweb3d::IGW3DVectorDataSourceWPtr vdatasource;
            while(vdscollection->next(&vdatasource))
            {
                Geoweb3d::IGW3DVectorLayerCollection *vlyrcollection = vdatasource.lock()->get_VectorLayerCollection();

                Geoweb3d::IGW3DVectorLayerWPtr layer;

                while(vlyrcollection->next(&layer))
                {
                    Geoweb3d::GW3DResult retval = layer.lock()->get_IsValidForVectorRepresentation();

                    if(retval ==  Geoweb3d::GW3D_sOk || retval == Geoweb3d::GW3D_sProjectionGuessed)
                    {
                        if(retval == Geoweb3d::GW3D_sProjectionGuessed)
                        {
                            printf("Warning: [%s] Has valid WGS84 bounds, but it must not have a spacial reference.\n", layer.lock()->get_Name() );
                        }

                        env.merge( layer.lock()->get_Envelope() );

                    }
                    else
                    {
                        printf("Data is probably not in wgs 84, look at the MFC example how to load projection for this layer\n");
                    }
                }
            }
        }


        double longitude, latitude;
        camera_controller_->get_Location(longitude, latitude );
        longitude = (env.MinX + env.MaxX ) * 0.5;
        latitude  = (env.MinY + env.MaxY ) * 0.5;
        camera_controller_->put_Location( longitude, latitude);
        camera_controller_->put_Elevation(100, Geoweb3d::IGW3DPositionOrientation::Relative );
        camera_controller_->put_Rotation(0,25,0);

        return true;
    }

private:

	 virtual void OnCreate( )
	 {
	 }
	 virtual void OnDrawBegin( )
	 {
	 }

  	 virtual void OnDrawEnd( ) 
	 {
	 }

    virtual void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera ) {};

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw work operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void DoPreDrawWork()
    {
        static float circle_counter = .0f;
        float radius = 1;

        static  double longitude, latitude = -1;

        if(latitude == -1) //just a way to init.
        {
            camera_controller_->get_Location( longitude, latitude );
        }

        //process all the vector pipeline events
        sdk_context_->run_EventStream( *this );

        longitude = longitude+ (cos(circle_counter) * .00003 );
        latitude  = latitude + (sin(circle_counter) * .00003 );

        camera_controller_->put_Location(longitude, latitude);

        circle_counter+=.005f;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the post draw work operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void DoPostDrawWork()
    {

    }
private:


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Linear interpolation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="startvalue">	The startvalue. </param>
    /// <param name="endvalue">  	The endvalue. </param>
    /// <param name="value">	 	The value. </param>
    ///
    /// <returns>	. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    double linearInterpolation( double startvalue, double endvalue, double value)
    {
        if (value < startvalue) return startvalue;
        if (value > endvalue)  return endvalue;

        return (endvalue - value) * startvalue + value * endvalue;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	OS event system, basically a helper off ProcessLowLevelEvent. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="win_event">	The window event. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void ProcessEvent( const Geoweb3d::WindowEvent& win_event )
    {
        switch(win_event.Type)
        {
        case win_event.MouseWheelMoved:
        {

            double longitude,latitude;
            camera_.lock()->get_CameraController()->get_Location(longitude,latitude);

            const double millimeter	=   0.001;
            const double centimeter	=   0.010;
            const double decimeter	=   0.100;
            const double meter		=   1.000;
            const double dekameter  =  10.000;
            const double hectometer = 100.000;
            const double kilometer	=1000.000;


            double elevation;
            Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode;
            camera_.lock()->get_CameraController()->get_Elevation( elevation,  mode  );

            double multipiler = 0;
            double abs_altitude = abs( elevation );

            if(abs_altitude > kilometer)
            {
                multipiler = linearInterpolation(kilometer, abs_altitude * 0.5,abs_altitude );
            }
            else if(abs_altitude > hectometer)
            {
                multipiler = linearInterpolation(kilometer, hectometer, abs_altitude )/hectometer;
            }
            else if(abs_altitude  > dekameter)
            {
                multipiler = linearInterpolation(hectometer,dekameter, abs_altitude )/dekameter;
            }
            else if(abs_altitude  > meter)
            {
                multipiler = linearInterpolation( dekameter,meter,abs_altitude )/meter;
            }
            else if(abs_altitude > decimeter)
            {
                multipiler = linearInterpolation( meter,decimeter,abs_altitude )/decimeter;
            }
            else if(abs_altitude > centimeter)
            {
                multipiler = linearInterpolation( decimeter,centimeter,abs_altitude )/centimeter;
            }
            else//(abs_altitude > millimeter)
            {
                multipiler = linearInterpolation(centimeter,millimeter, abs_altitude )/millimeter;
            }


            double newaltitude =  elevation + (float)(-win_event.MouseWheel.delta * multipiler);

            if(newaltitude< .01)  newaltitude = .01;

            camera_.lock()->get_CameraController()->put_Location( longitude, latitude);
            camera_.lock()->get_CameraController()->put_Elevation(newaltitude, Geoweb3d::IGW3DPositionOrientation::Relative );

        }
        break;

        case win_event.KeyPressed:

            if(!camera_.expired())
            {
                float heading, pitch, roll;
                camera_controller_->get_Rotation( heading, pitch, roll );

                switch(	win_event.Key.code )
                {
                    //Move forward
                case  Geoweb3d::Key::W:
                    //moveCamForward
                    break;

                    //Rotate right (yaw)
                case  Geoweb3d::Key::D:
                    if(win_event.Key.shift)
                    {
                        heading+=1.f;
                    }
                    else
                    {
                        heading-=1.f;
                    }
                    //rotCamHorizontal
                    break;

                    //Rotate up (pitch)
                case  Geoweb3d::Key::Q:
                    //rotCamVertical
                    if(win_event.Key.shift)
                    {
                        pitch+=1.f;
                    }
                    else
                    {
                        pitch-=1.f;
                    }
                    break;
                    //Strafe right
                case  Geoweb3d::Key::X:
                    //moveCamRight
                    break;
                }


                camera_controller_->put_Rotation(heading,pitch, roll );
            }


            break;
        default:
            break;
        }

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	OS event system, raw data directly from the windows message pump. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="hWndParent">	The window parent. </param>
    /// <param name="message">   	The message. </param>
    /// <param name="wParam">	 	The wParam field of the message. </param>
    /// <param name="lParam">	 	The lParam field of the message. </param>
    ///
    /// <returns>	. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual LRESULT CALLBACK ProcessLowLevelEvent( OSWinHandle hWndParent, unsigned int message, WPARAM wParam, LPARAM lParam )
    {
        // We don't forward the WM_CLOSE message to prevent the internal SDK from closing us (click on X)
        if (message == WM_CLOSE)
        {
            printf("ProcessLowLevelEvent: WM_Close detected and prevented!\n");
            return 0;
        }
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>
    /// Pipeline messages Because we are a globe and can load data everywhere, we are never *done*
    /// loading anything as if you move away and come back, its going to load the area again.
    /// </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="representation">	The representation. </param>
    /// <param name="etype">		 	The etype. </param>
    /// <param name="unique_id">	 	Unique identifier. </param>
    /// <param name="bounds">		 	The bounds. </param>
    /// <param name="value">		 	The value. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void OnPagingEvent( Geoweb3d::IGW3DVectorRepresentationWPtr representation, PagingEventType etype, unsigned unique_id, const Geoweb3d::GW3DEnvelope& bounds, unsigned int value )
    {
        if(etype ==  IGW3DEventStream::QuickCacheCreateFinished)
        {
            printf("Cache for this Envelope/unique_id finished : %s\n", representation.lock()->get_Driver().lock()->get_Name());
        }
        else if(etype ==  Geoweb3d::IGW3DEventStream::DataSourceFinishedLoading)
        {
            printf("Loading for this Envelope/unique_id finished : %s\n", representation.lock()->get_Driver().lock()->get_Name());
        }
        else if(etype ==  Geoweb3d::IGW3DEventStream::DataSourceWorkEstimationComplete || etype ==  Geoweb3d::IGW3DEventStream::DataSourceWorkEstimationComplete  )
        {
            printf("Percent for this area [%s] %d \n", representation.lock()->get_Driver().lock()->get_Name(), value );
        }
    }

    virtual void OnStereoEvent( Geoweb3d::IGW3DStereoDriverWPtr stereo_driver, StereoEventType etype )
    {
        if ( etype == Geoweb3d::IGW3DEventStream::HMD_CONNECTED )
        {
            printf( "A HMD has been connected. \n" );
        }
        else if ( etype == Geoweb3d::IGW3DEventStream::HMD_DISCONNECTED )
        {
            printf( "A HMD has been disconnected. \n" );
        }
    }

private:

    /// <summary>	Context for the sdk. </summary>
    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
    /// <summary>	The window. </summary>
    Geoweb3d::IGW3DWindowWPtr window_;
    /// <summary>	The camera. </summary>
    Geoweb3d::IGW3DCameraWPtr camera_;
    /// <summary>	The camera controller. </summary>
    Geoweb3d::IGW3DCameraController *camera_controller_;

    /// <summary>	The extruded representation. </summary>
    Geoweb3d::IGW3DVectorRepresentationWPtr extruded_representation_;

}; //engine end of class


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// /
/// 					RepresentAsExtruded
/// /.
/// </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">	Context for the sdk. </param>
/// <param name="layer">	  	The layer. </param>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsExtruded( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer , bool construct_textured, int default_top_texture_id, int default_side_texture_id  )
{

    Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "ExtrudedPolygonEx" );


    Geoweb3d::GW3DResult validitychk;

    if(!Geoweb3d::Succeeded( validitychk =  driver.lock()->get_CapabilityToRepresent( layer ) ))
    {
        printf("Asked to represent a vector layer with a representation, but the layer is not compatible!\n");
    }
    else
    {
        // prints the values the layers has to help the user know what they can attribute map into the representatin
        const Geoweb3d::IGW3DDefinitionCollection* def = layer.lock()->get_AttributeDefinitionCollection();

        printf("Attribute Names within the layer: [%s] you can map to the a extruded polygon propery: \n", layer.lock()->get_Name() );
        for(unsigned i = 0; i < def->count(); ++i )
        {
            printf("\t[%s]\n", def->get_AtIndex(i)->property_name  );
        }

        if(construct_textured)
        {
            Geoweb3d::IGW3DPropertyCollectionPtr defaults = driver.lock()->get_PropertyCollection()->create_Clone();

            //turn on texturing.  This is global do the representation layer.
            defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::TEXTURE_MODE_ENABLE, true);

            //note, these can be done per feature ID.. just an fyi
            defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::TEXTURE_TOP_PALETTE_INDEX, default_top_texture_id );
            defaults->put_Property(Geoweb3d::Vector::ExtrudedPolygonProperties::TEXTURE_SIDE_PALETTE_INDEX, default_side_texture_id );

            //good to go!
            Geoweb3d::Vector::RepresentationLayerCreationParameter  params;

            params.representation_default_parameters = defaults;

            return driver.lock()->get_RepresentationLayerCollection()->create( layer, params );
        }
        else
        {
            //good to go!
            Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
            return driver.lock()->get_RepresentationLayerCollection()->create( layer, params );
        }
    }

    return Geoweb3d::IGW3DVectorRepresentationWPtr();
}


/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">  	Context for the sdk. </param>
/// <param name="title">			The title. </param>
/// <param name="window_events">	(optional) [in,out] If non-null, the window events. </param>
///
/// <returns>	The new window creation. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events = 0 )
{
    Geoweb3d::IGW3DWindowCollection * wcol = sdk_context->get_WindowCollection(  );

    Geoweb3d::IGW3DWindowWPtr window = wcol->create_3DWindow(title, GW3D_OVERLAPPED,10,10,1280,720, 0, Geoweb3d::IGW3DStereoDriverPtr(), window_events);

    if(window.expired())
    {
        printf("Error Creating window: [%s]!\n", title );

        return Geoweb3d::IGW3DWindowWPtr();
    }

    return window;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates a camera. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="windowptr">  	The windowptr. </param>
/// <param name="cameratitle">	The cameratitle. </param>
///
/// <returns>	The new camera. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle )
{
    Geoweb3d::IGW3DCameraCollection *cameracollection = windowptr.lock()->get_CameraCollection();
    Geoweb3d::IGW3DCameraWPtr camera =	cameracollection->create( cameratitle );

    if(!camera.expired())
    {
        camera.lock()->get_DateTime()->put_isUseComputerDate(true);
        camera.lock()->get_DateTime()->put_isUseComputerTime(false);
        camera.lock()->get_DateTime()->put_isFollowCameraLocationForReference(true);
        camera.lock()->get_DateTime()->put_Time(12,0);
    }

    return camera;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the application operation. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">	Context for the sdk. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{

    MiniEngine *pengine = new MiniEngine( sdk_context );

    if(pengine->LoadConfiguration("Extruded Polygon Texture Example"))
    {
        ////******* CHANGE THIS TO YOUR SAMPLE DATA LOCATION ******/
        const char * root_path = "../examples/media/";
         if(pengine->LoadData( root_path ))
        {

        }
        else
        {
            printf("Could not load data, you probably need to setup the path to your data\n");
        }

        //the engine loaded all its data ok

        while( pengine->Update() )
        {
            //could do other app stuff here
        }
    }



    delete pengine;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Main entry-point for this application. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="argc">	The argc. </param>
/// <param name="argv">	[in,out] If non-null, the argv. </param>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
    SetInformationHandling( );

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if(sdk_context)
    {
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
		sdk_init->put_ESRILicenseCheckout(false); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
        {
            RunApplication( sdk_context );
        }
        else
        {
            printf("\nSDK Context Creation Error! Press any key to continue...\n");
            getchar();
        }
    }
    else
    {
        return -1;
    }

    //example to show if you want to control its when the sdk context gets destroyed.  This will invalidate all
    // the pointers the SDK owned!  (xxx.expired() ==true)
    sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();

    //system("pause");

    return 0;
}


/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	My fatal function. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="msg">	The message. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void my_fatal_function(const char *msg )
{
    printf("Fatal Info: %s", msg);
}

/*! General info function, this will be used to print any general info and non-fatal warnings */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	My information function. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="msg">	The message. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void my_info_function(const char *msg )
{
   printf("General Info: %s", msg);
}

/*! Information handling is not required */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets information handling. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SetInformationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}