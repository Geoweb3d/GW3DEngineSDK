// Icons.cpp

#include "Icons.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <tchar.h>

#define GLEW_STATIC
#include <GL/glew.h>

#pragma comment( lib, "GW3DEngineSDK.lib" )
#pragma comment( lib, "GW3DCommon.lib" )

#ifndef GL_CLAMP_TO_EDGE
    #define GL_CLAMP_TO_EDGE                  0x812F
#endif


void my_fatal_function( const char* msg );
void my_info_function( const char *msg );
void SetInformationHandling();

class IconsTestApp
{
	public:

		IconsTestApp( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
			: sdk_context_( sdk_context )
			, overlay2d_handler_( 0 )
		{
		}

		~IconsTestApp()
		{
			if( overlay2d_handler_ != 0 )
			{
				delete overlay2d_handler_;
				overlay2d_handler_= 0;
			}
		}

		void ConfigureCameras()
		{
			//create and configure our cameras

			Geoweb3d::IGW3DCameraWPtr cam0 = window_.lock()->get_CameraCollection()->create( "Camera0" );
			Geoweb3d::IGW3DCameraWPtr cam1 = window_.lock()->get_CameraCollection()->create( "Camera1" );
			Geoweb3d::IGW3DCameraWPtr cam2 = window_.lock()->get_CameraCollection()->create( "Camera2" );
			Geoweb3d::IGW3DCameraWPtr cam3 = window_.lock()->get_CameraCollection()->create( "Camera3" );

			Geoweb3d::GW3DEnvelope envelope = icon_layer_.lock()->get_Envelope();
			
			double cam_lon = ( envelope.MaxX + envelope.MinX ) * 0.5;
			double cam_lat = ( envelope.MaxY + envelope.MinY ) * 0.5;
			const double cam_elev = 100.0;

			const float h_offset = 0.5f;
			const float v_offset = 0.5f;
			const float heading_incr = 45.0f;
			const float pitch = 85.0f;
			float heading = 0.0f;
			
			cam0.lock()->get_CameraController()->put_Location( cam_lon, cam_lat );
			cam0.lock()->get_CameraController()->put_Elevation( cam_elev );
			cam0.lock()->get_CameraController()->put_Rotation( heading, pitch, 0.0f );
			cam0.lock()->put_NormalizeWindowCoordinates( 0.0f, h_offset, 0.0f, v_offset );
			heading+=heading_incr;

			cam1.lock()->get_CameraController()->put_Location( cam_lon, cam_lat );
			cam1.lock()->get_CameraController()->put_Elevation( cam_elev );
			cam1.lock()->get_CameraController()->put_Rotation( heading, 0.0f, 0.0f );
			cam1.lock()->put_NormalizeWindowCoordinates( h_offset, 1.0f, 0.0f, v_offset );
			heading+=heading_incr;

			cam2.lock()->get_CameraController()->put_Location( cam_lon, cam_lat );
			cam2.lock()->get_CameraController()->put_Elevation( cam_elev );
			cam2.lock()->get_CameraController()->put_Rotation( heading, 0.0f, 0.0f );
			cam2.lock()->put_NormalizeWindowCoordinates( 0.0f, h_offset, v_offset, 1.0f );
			heading+=heading_incr;

			cam3.lock()->get_CameraController()->put_Location( cam_lon, cam_lat );
			cam3.lock()->get_CameraController()->put_Elevation( cam_elev );
			cam3.lock()->get_CameraController()->put_Rotation( heading, 0.0f, 0.0f );
			cam3.lock()->put_NormalizeWindowCoordinates( h_offset, 1.0f, v_offset, 1.0f );
			heading+=heading_incr;

		}

		bool Initialize()
		{
			overlay2d_handler_ = new Overlay2DHandler( sdk_context_ );

			window_ = sdk_context_->get_WindowCollection()->create_3DWindow( "Icons Example", GW3D_OVERLAPPED, 10, 10, 1280,720, 0, Geoweb3d::IGW3DStereoDriverPtr(), overlay2d_handler_ ); 

			//set up layer
			Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context_->get_VectorDriverCollection()->get_Driver( "Geoweb3d_Datasource" );

			if( driver.expired() )
			{
				std::cout << "Could not find the requested driver! For a list of supported drivers, run the Print Capabilities example" << std::endl;
				return false;
			}

			if( !driver.lock()->get_IsCapable( "CreateDataSource" ) )
			{
				std::cout << "This driver is not capable of creating new datasources!" << std::endl;
				return false;
			}

			//open data source
			std::string filepath = "../examples/media/Utility Poles.shp";

			Geoweb3d::GW3DResult res;
			//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
			//having to guess which data source driver is needed to open up a particular dataset.
			Geoweb3d::IGW3DVectorDataSourceWPtr data_source = sdk_context_->get_VectorDriverCollection()->auto_Open( filepath.c_str(), res);

			if( data_source.expired() )
			{
				std::cout << "Could not load the following Vector Data Source: " << filepath.c_str();
				return false;
			}

			icon_layer_ = data_source.lock()->get_VectorLayerCollection()->get_AtIndex( 0 );

			if( icon_layer_.expired() )
			{
				std::cout << "Datasource do not contain any layers." <<std::endl;
				return false;
			}

			//try representing the features in the layer
			Geoweb3d::IGW3DVectorRepresentationDriverWPtr rep_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver( "Box" );

			if( rep_driver.expired() )
			{
				std::cout<< "Error creating vector representation driver." <<std::endl;
				return false;
			}

			Geoweb3d::GW3DResult can_represent;
			can_represent = rep_driver.unsafe_get()->get_CapabilityToRepresent( icon_layer_ );

			if( !Geoweb3d::Succeeded( can_represent ) )
			{
				std::cout << "Error: The selected representation driver cannot represent this layer." << std::endl;
				return false;
			}

			//create some default properties for the layer
			Geoweb3d::IGW3DPropertyCollectionPtr default_properties = rep_driver.lock()->get_PropertyCollection()->create_Clone();

			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "X_EXTENT" ), 5.0 );
			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "Y_EXTENT" ), 5.0 );
			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "Z_EXTENT" ), 5.0 );

			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "RED" ),   0.0 );
			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "GREEN" ), 1.0 );
			default_properties->put_Property( default_properties->get_DefinitionCollection()->get_IndexByName( "BLUE" ),  0.0 );

			Geoweb3d::Vector::RepresentationLayerCreationParameter rep_layer_creation_params;

			rep_layer_creation_params.page_level = 8;
			rep_layer_creation_params.representation_default_parameters = default_properties;

			Geoweb3d::IGW3DVectorRepresentationWPtr rep = rep_driver.lock()->get_RepresentationLayerCollection()->create( icon_layer_, rep_layer_creation_params );

			rep.lock()->get_ClampRadiusFilter()->put_Enabled( true );
			rep.lock()->get_ClampRadiusFilter()->put_MaxRadius( 30000.0 );

			//frustum analysis setup
			rep.lock()->get_FrameAnalysisContentsConfiguration()->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->put_Enabled( true );
			rep.lock()->get_FrameAnalysisContentsConfiguration()->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->get_FeatureID2DPixelLocationConfiguration()->put_Enabled( true );

			ConfigureCameras();

			return true;
		}

		void Run()
		{

			bool valid = true;

			while( valid )
			{
				valid = ( sdk_context_->draw( window_ ) == Geoweb3d::GW3D_sOk );
			}
		}

	private:
		IconsTestApp(){}

	private:

		Geoweb3d::IGW3DGeoweb3dSDKPtr	sdk_context_;
		Geoweb3d::IGW3DWindowWPtr		window_;
	
		Geoweb3d::IGW3DVectorLayerWPtr	icon_layer_;

		Overlay2DHandler*				overlay2d_handler_;

};


//Overlay2DHandler implementation

//2D overlay handler end
struct Overlay2DMetadata
{
	size_t v_index_start;
	size_t count;
};

struct Overlay2DVertex
{
	short x;
	short y;
	float u;
	float v;
};

struct Overlay2DHandler_
{
	Overlay2DHandler_()
		: texture( 0 )
	{
	}

	Overlay2DMetadata* addRep( Geoweb3d::IGW3DVectorRepresentationWPtr rep )
	{
		Overlay2DMetadata* metadata = &(rep_container[rep] );

		metadata->v_index_start = vertex_array.size();

		return metadata;
	}

	void ProcessFeatureFeedbackResult( Geoweb3d::IGW3DFeatureFeedbackResult* feature_result )
	{
		float fx, fy;

		const int icon_width = 32;
		const int icon_height = 32;

		int px, py;

		//No sorting yet.
		feature_result->get_FeatureID2DPixelLocationResult()->get_WindowCoordinate( px, py );
		feature_result->get_FeatureID2DPixelLocationResult()->get_NormalizedCoordinate( fx, fy );

		unsigned int width, height;
		int x, y;
		
		active_camera.unsafe_get()->get_Window().unsafe_get()->get_CameraWindowCoordinates( active_camera, x, y, width, height );

		//
		Overlay2DVertex vtx;

		//v0
		vtx.x = px - icon_width/2;
		vtx.y = py - icon_height/2;
		vtx.u = 0.0f;
		vtx.v = 0.0f;
		vertex_array.push_back( vtx );

		//v1
		vtx.x = px + icon_width/2;
		vtx.y = py - icon_height/2;
		vtx.u = 1.0f;
		vtx.v = 0.0f;
		vertex_array.push_back( vtx );

		//v2
		vtx.x = px + icon_width/2;
		vtx.y = py + icon_height/2;
		vtx.u = 1.0f;
		vtx.v = 1.0f;
		vertex_array.push_back( vtx );

		//v3
		vtx.x = px - icon_width/2;
		vtx.y = py + icon_height/2;
		vtx.u = 0.0f;
		vtx.v = 1.0f;
		vertex_array.push_back( vtx );
	}

	void CreateTexture()
	{
		const unsigned int tex_width  = 32;
		const unsigned int tex_height = 32;

		std::vector< unsigned char > tex_data( tex_width * tex_height * 4, 255 );

		int c, index = 0;

 		for( int i = 0; i < tex_height; ++i )
		{
			for ( int j = 0; j < tex_width; ++j )
			{
				c = ( ( ( i & 0x8 ) == 0 ) ^ ( ( ( j & 0x8 ) ) == 0 ) ) * 255;

				index = ( i * tex_width + j ) * 4;

				tex_data[index]   = (unsigned char) c;
				tex_data[index+1] = (unsigned char) c;
				tex_data[index+2] = (unsigned char) c;
			}
		}
		
		glGenTextures( 1, &texture );
		
		glBindMultiTextureEXT( GL_TEXTURE0, GL_TEXTURE_2D, texture );

		glTextureParameteriEXT( texture, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTextureParameteriEXT( texture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTextureParameteriEXT( texture, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTextureParameteriEXT( texture, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		glTextureImage2DEXT( texture, GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &tex_data[0] );

		glBindTexture( GL_TEXTURE_2D, 0 );

	}

	void Draw()
	{
		unsigned int width, height;
		int x, y;
		
		float left  = 0.0;
		float right = 1.0f;
		float top   = 0.0f;
		float bottom= 1.0f;

		if( texture == 0 )
		{
			glewInit();
			CreateTexture();
		}

		if( !active_camera.expired() && !vertex_array.empty() )
		{
			active_camera.unsafe_get()->get_Window().unsafe_get()->get_CameraWindowCoordinates( active_camera, x, y, width, height );
			active_camera.unsafe_get()->get_NormalizeWindowCoordinates( left, right, top, bottom );

			const char* camera_name = active_camera.unsafe_get()->get_Name();

			glViewport( x, y, width, height );

			glMatrixLoadIdentityEXT( GL_PROJECTION );
			
			glMatrixOrthoEXT( GL_PROJECTION, 0, width, height, 0, 0.0, 1.0 );

			//glMatrixOrthoEXT( GL_PROJECTION, left, right, bottom, top, 0.0, 1.0 );

			glMatrixLoadIdentityEXT( GL_MODELVIEW );
		
			glDisable( GL_BLEND );
			glDisable( GL_DEPTH_TEST );

			glEnable( GL_TEXTURE_2D );
			glBindMultiTextureEXT( GL_TEXTURE0, GL_TEXTURE_2D, texture );
		
			glEnableClientState( GL_VERTEX_ARRAY );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glVertexPointer( 2, GL_SHORT , sizeof( Overlay2DVertex ), &vertex_array[0].x );
			glTexCoordPointer( 2, GL_FLOAT, sizeof( Overlay2DVertex ), &vertex_array[0].u );

			glDrawArrays( GL_QUADS, 0, static_cast<GLsizei>( vertex_array.size() ) );

			glDisableClientState( GL_VERTEX_ARRAY );
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glBindTexture( GL_TEXTURE_2D, 0 );
			glDisable (GL_TEXTURE_2D);  
			glEnable( GL_DEPTH_TEST );

		}
	}

	Geoweb3d::IGW3DCameraWPtr active_camera;

	std::map< Geoweb3d::IGW3DVectorRepresentationWPtr, Overlay2DMetadata > rep_container;
	std::vector< Overlay2DVertex > vertex_array;
	
	unsigned int texture;
};

Overlay2DHandler::Overlay2DHandler()
{
	m_imp = new Overlay2DHandler_;
}


Overlay2DHandler::Overlay2DHandler( Geoweb3d::IGW3DGeoweb3dSDKPtr& sdkcontext )
	: m_sdkcontext_( sdkcontext )
{
	m_imp = new Overlay2DHandler_;
}

Overlay2DHandler::~Overlay2DHandler()
{
	if( m_imp  )
	{
		delete m_imp;
		m_imp = 0;
	}
}

void Overlay2DHandler::addRepresentation( Geoweb3d::IGW3DVectorRepresentationWPtr rep )
{
	if( m_imp )
	{
		rep.lock()->get_FrameAnalysisContentsConfiguration()->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->put_Enabled( true );
		rep.lock()->get_FrameAnalysisContentsConfiguration()->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->get_FeatureID2DPixelLocationConfiguration()->put_Enabled( true );

		m_imp->addRep( rep );
	}
}

//IGW3DWindowCallback interface
void Overlay2DHandler::OnCreate( ) 
{
}

void Overlay2DHandler::OnDrawBegin( )
{
}

void Overlay2DHandler::OnDrawEnd( ) 
{
}

void Overlay2DHandler::OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera )
{
	m_imp->active_camera = camera;

	//just clear our vertex array for now( can be optimize to reuse via a tracking index )
	m_imp->vertex_array.clear();

	//stream our reps for analysis and build our 'display list'
	m_sdkcontext_->get_VectorRepresentationDriverCollection()->Stream( this );

	m_imp->Draw();
}

//IGW3DFrustumAnalysisStream interface
bool Overlay2DHandler::OnStream( Geoweb3d::IGW3DContentsResult* contents, Geoweb3d::IGW3DFeatureFeedbackResultCollection* featurefeedback )
{
	if( contents->get_Camera() != m_imp->active_camera )
		return false;

	if( featurefeedback->get_WasEnabled() )
	{
		Geoweb3d::IGW3DVectorRepresentationWPtr rep = contents->get_VectorRepresentation();
		Overlay2DMetadata* metadata = m_imp->addRep( rep );

		featurefeedback->reset();
		int feature_count = featurefeedback->count();

		metadata->count = static_cast< size_t >( feature_count );

		if( feature_count )
		{
			Geoweb3d::IGW3DFeatureFeedbackResult* feature_result;

			while( featurefeedback->next( &feature_result ) )
			{
				//if the feature is not occluded by the globe and was rendered then
				//add to the list
				if( !feature_result->get_OccludedByGlobe() )
				{
					m_imp->ProcessFeatureFeedbackResult( feature_result );
				}
			}
		}
	}

	return false;
}

bool Overlay2DHandler::OnError( )
{
	return false;
}


void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
	IconsTestApp app( sdk_context );

	if( app.Initialize() )
	{
		app.Run();
	}
}

int _tmain( int argc, _TCHAR* argv[] )
{
    SetInformationHandling();

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if( sdk_context )
    {
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
		sdk_init->put_ESRILicenseCheckout(false); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
        {
            RunApplication( sdk_context );
        }
        else
        {
			std::cout << "\nError creating SDK context! Press any key to continue...\n" << std::endl;
            getchar();
        }
    }
    else
    {
        return -1;
    }

   //Example showing control over SDK destruction.  This will invalidate all
    //the pointers the SDK owned! ( xxx.expired() == true )
    sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();

    return 0;
}

/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function(const char *msg )
{
	std::cout<< "Fatal Info: " << msg;
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function( const char* msg )
{
	std::cout<< "General Info: " << msg;
}

/*! Information handling is not required */
void SetInformationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}


