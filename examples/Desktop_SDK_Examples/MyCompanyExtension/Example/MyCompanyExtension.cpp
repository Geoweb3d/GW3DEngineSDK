// Entry point for the DLL application.
//

#include "stdafx.h"

#include "version.h"
#include "TocTools.h"
#include "CritSectEx.h"
#include "Mouse3DQuery.h"
#include "MyCompanyExtension.h"
#include "BuildingClassifier.h"
#include "SettingsManager.h"
#include "RangeRing.h"

#include "Qt\QtDockWindow.h"
#include "Qt\XMLFiles.h"

#include "core/GW3DGuids.h"
#include "gui/GW3DGUICommon.h"
#include "gui/GW3DGUIVector.h"
#include "gui/GW3DGUIViews.h"
#include "gui/GW3DGUICustomization.h"
#include "gui/GW3DGUITableOfContents.h"

#include "gui/IGW3DGUIApplicationUpdateContext.h"

#include <sstream>

#include "Geoweb3d/engine/IGW3DRegistration.h"
#include "Geoweb3d/engine/IGW3DImageCollection.h"
#include "Geoweb3d/engine/IGW3DImage.h"

#pragma comment(lib, "../../../lib/GW3dCommon.lib")

namespace
{
	HINSTANCE hinstDLL;

	//utility function
	double closed_interval_rand( double x0, double x1 )
	{
		return x0 + (x1 - x0) * rand() / ((double) RAND_MAX);
	}
	
	COLORREF gColor = RGB(126, 128, 255);
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


void MyRebrandingToken::get_ValidateRebrandingLicense( /*todo*/ )
{

}
	
bool MyRebrandingToken::get_DisableNativeSplashscreen( )
{
	return true;
	//todo: open custom splashscreen (or do it in createClassInstance)
}


MyCompanyExtension& MyCompanyExtension::Instance()
{
	static MyCompanyExtension *instance_ =0;
	if(!instance_)
	{
		instance_= new MyCompanyExtension;
	}
	
	return *instance_;
}

MyCompanyExtension::MyCompanyExtension() 
	: quit_(false), 
	threadhandle_(0), 
	classifier_(0), 
	framecount_(0), 
	mouse_query_(0),
	settings_manager_(new SettingsManager()),
	first_pass_(true),
	rebranding_token_(0)
{
	//qApp->setAttribute(Qt::AA_NativeWindows);
}

MyCompanyExtension::~MyCompanyExtension(){}

MyRebrandingToken* MyCompanyExtension::GetRebrandingToken()
{
	if (!rebranding_token_)
	{
		rebranding_token_ =  new MyRebrandingToken();
	}
	return rebranding_token_;
}

bool MyCompanyExtension::GetMediaPath(std::string& sub_path)
{

	static std::string root_path;

	if (root_path.empty())
	{
		char ptr[120];
		unsigned int len;

		getenv_s( &len, ptr, 120, "GEOWEB3D_DESKTOP_SDK_MEDIA");
		if (len == 0)
		{
			LogMessage("ERROR: Can't access '%s' because GEOWEB3D_DESKTOP_SDK_MEDIA environmental variable not set!", sub_path);
			return false;
		}
		char* mediavar = ptr;
		root_path = mediavar;
	}

	sub_path = root_path + sub_path;
	return true;
}

#define LOAD_BUILDINGS_AS_BASIC_REPRESENTATION

 void MyCompanyExtension::LoadBuildings( Geoweb3d::GUI::IGW3DGUIPostDrawContext *ctx )
 {

	//NOTE: You may also use IGW3DGUIVectorDriverCollection::auto_Open if you do not know/care what the driver for your datasource is.
	Geoweb3d::GUI::IGW3DGUIVectorDriverCollection* v_drivers = ctx->get_VectorDriverCollection();
	Geoweb3d::GUI::IGW3DGUIVectorDriverWPtr shp_driver = v_drivers->get_Driver("ESRI Shapefile");

	Geoweb3d::GUI::IGW3DGUIVectorDataSourceCollection* datasources = shp_driver.lock()->get_VectorDataSourceCollection();
	std::string building_footprint_data = "/bldgs_shp/shp/pyo_commercial.shp";
	if (!GetMediaPath(building_footprint_data))
	{
		return;
	}
	Geoweb3d::GUI::IGW3DGUIVectorDataSourceWPtr my_datasource = datasources->open(building_footprint_data.c_str()); 

	if (my_datasource.expired())
	{
		LogMessage("Buildings could not be loaded!  Invalid path?");
		return;
	}

	LogMessage("Loading: %s\n", my_datasource.lock()->get_Name());

	Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr polygon_layer = my_datasource.lock()->get_VectorLayerCollection()->open(my_datasource.lock()->get_VectorLayerPreviewCollection()->get_AtIndex(0)); //shp only has 1 layer
	
	if (polygon_layer.expired())
	{
		LogMessage("Failed to load layer from datasource: %s", my_datasource.lock()->get_Name());
		return;
	}

	if (!Geoweb3d::Succeeded(ctx->get_VectorRepresentationDriverCollection()->get_Driver(gw3dExtrudedPolygonRepresentation).lock()->get_CapabilityToRepresent(polygon_layer)))
	{
		LogMessage("Unable to represent as polygons (wrong geometry type?): %s", polygon_layer.lock()->get_Name());
		return;
	}
	
	Geoweb3d::GUI::IGW3DGUIVectorRepresentationDriverWPtr polygon_driver = ctx->get_VectorRepresentationDriverCollection()->get_Driver(gw3dExtrudedPolygonRepresentation);

#ifndef LOAD_BUILDINGS_AS_BASIC_REPRESENTATION

	Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr building_polys= polygon_driver.lock()->get_RepresentationLayerCollection()->create(polygon_layer);

	//make a group for the new layer
	Geoweb3d::GUI::IGW3DGUITocItemWPtr groupItem = ctx->get_RootTOCItemCollection()->create_Group("My Group");

	//add to the table of contents (initially unchecked, so we can show how to check it below)
	groupItem.lock()->get_ChildCollection()->add_Layer(polygon_layer, false);

	polygon_layer.lock()->get_TocItem().lock()->put_Name("My Layer");

	//check the new polygon layer
	polygon_layer.lock()->get_TocItem().lock()->put_Checked(true);

	TocTools toc_tools;
	LogMessage ( toc_tools.SummarizeTocVectorLayers(ctx->get_RootTOCItemCollection()).c_str() );

#else

	Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr building_polys= polygon_driver.lock()->get_RepresentationLayerCollection()->create_basic(polygon_layer);

	Geoweb3d::IGW3DPropertyCollectionPtr default_properties = polygon_driver.lock()->get_PropertyCollection()->create_Clone();

	default_properties->put_Property(Geoweb3d::GUI::ExtrudedPolygonProperties::TEXTURE_MODE_ENABLE, true);

	std::string building_roof_texture = "/bldgs_shp/textures/Demo_apt_bld_roof_01.rgb";
	if (!GetMediaPath(building_roof_texture))
	{
		return;
	}

	std::string building_wall_texture = "/bldgs_shp/textures/Demo_wh_bld_01_a.rgb";
	if (!GetMediaPath(building_wall_texture))
	{
		return;
	}

	Geoweb3d::IGW3DImageCollectionPtr texture_palette = ctx->get_SceneGraphContext()->create_ImageCollection();
	texture_palette->put_CollectionWidthHeightMode(Geoweb3d::IGW3DImageCollection::AUTO);
	Geoweb3d::IGW3DImageWPtr wall1_img = texture_palette->create(building_roof_texture.c_str());
	Geoweb3d::IGW3DImageWPtr roof1_img = texture_palette->create(building_wall_texture.c_str());
	building_polys.lock()->put_GW3DFinalizationToken(texture_palette->create_FinalizeToken());
	int wall1_index = wall1_img.lock()->get_PropertyCollectionID();
	int roof1_index = roof1_img.lock()->get_PropertyCollectionID();
	default_properties->put_Property(Geoweb3d::GUI::ExtrudedPolygonProperties::TEXTURE_SIDE_PALETTE_INDEX, wall1_index);
	default_properties->put_Property(Geoweb3d::GUI::ExtrudedPolygonProperties::TEXTURE_TOP_PALETTE_INDEX, roof1_index);

	building_polys.lock()->put_PropertyCollection(default_properties);

	//Note: there is a known issue where textures are not repeated correctly around the full perimeter of buildings
	BuildingTexturizer::Stream(building_polys);


#endif
 }

 void MyCompanyExtension::LoadEntities( Geoweb3d::GUI::IGW3DGUIStartContext *ctx )
 {
	if(threadhandle_)
	{
		LogMessage("Bug as Load data was called twice (means start was called twice with no stop)");
		return;
	}

	/* Set up our attribute fields */
	Geoweb3d::IGW3DDefinitionCollectionPtr attribute_fields = Geoweb3d::IGW3DDefinitionCollection::create();
	attribute_fields->add_Property("Entity Number", Geoweb3d::PROPERTY_INT, "My attribute for the ID of each entity");
	attribute_fields->add_Property("Label", Geoweb3d::PROPERTY_STR, "My label attribute for entities of interest", 255);
	attribute_fields->add_Property("Balloon", Geoweb3d::PROPERTY_STR, "My balloon attribute for entities", 255);

	/* Get the Geoweb3d In-Memory Datasource Driver */
	Geoweb3d::GUI::IGW3DGUIVectorDriverCollection* v_drivers = ctx->get_VectorDriverCollection();
	Geoweb3d::GUI::IGW3DGUIVectorDriverWPtr memory_driver = v_drivers->get_Driver("Geoweb3d_Datasource");
	
	if (memory_driver.expired())
	{
		LogMessage("The requested vector driver doesn't exist!");
		return;
	}

	if (!memory_driver.lock()->get_isCapable(isCreateDataSourceSupported))
	{
		//Not really needed since we know we can create datasources with the Geoweb3d_Datasource
		LogMessage("Failed test to see if driver supports creating a datasource!");
		return;
	}

	/* Create a new in-memory datasource (which can have many layers) */
	Geoweb3d::GUI::IGW3DGUIVectorDataSourceCollection* datasources = memory_driver.lock()->get_VectorDataSourceCollection();
	Geoweb3d::GUI::IGW3DGUIVectorDataSourceWPtr my_datasource = datasources->create("My Entity Datasource");
	
	if (my_datasource.expired())
	{
		LogMessage("Failed to create my datasource!");
		return;
	}

	/* Create a new point layer for my entities */
	Geoweb3d::GUI::IGW3DGUIVectorLayerCollection* my_opened_layers = my_datasource.lock()->get_VectorLayerCollection();
	entity_layer_ = my_opened_layers->create("My Entity Layer", Geoweb3d::gtPOINT_25D, attribute_fields);

	if (entity_layer_.expired())
	{
		LogMessage("Failed to create my layer!");
		return;
	}

	//Make the layer editable, so we can create features after representing, and move them
	entity_layer_.lock()->put_GeometryEditableMode(true, false);

	//Geoweb3d::GUI::IGW3DGUIVectorRepresentationDriverWPtr icon_driver = ctx->get_VectorRepresentationDriverCollection()->get_Driver("Icon"); /* OR */
	Geoweb3d::GUI::IGW3DGUIVectorRepresentationDriverWPtr icon_driver = ctx->get_VectorRepresentationDriverCollection()->get_Driver(gw3dIconRepresentation);
	
	if (icon_driver.expired())
	{
		LogMessage("Bug: Couldn't get the Icon driver!");
		return;
	}
	
	entity_icons_ = icon_driver.lock()->get_RepresentationLayerCollection()->create_basic(entity_layer_);
	
	if (entity_icons_.expired())
	{
		LogMessage("Failed to represent entities as icons!");
		return;
	}

	//Geoweb3d::GUI::IGW3DGUIVectorRepresentationDriverWPtr model_driver = ctx->get_VectorRepresentationDriverCollection()->get_Driver("Model"); /* OR */
	Geoweb3d::GUI::IGW3DGUIVectorRepresentationDriverWPtr model_driver = ctx->get_VectorRepresentationDriverCollection()->get_Driver(gw3dModelRepresentation); 
	
	if (model_driver.expired())
	{
		LogMessage("Bug: Couldn't get the Model driver!");
		return;
	}
	
	entity_models_ = model_driver.lock()->get_RepresentationLayerCollection()->create_basic(entity_layer_);
	
	if (entity_models_.expired())
	{
		LogMessage("Failed to represent entitiesW as models!");
		return;
	}

	Geoweb3d::IGW3DPropertyCollectionPtr model_properties = entity_models_.lock()->get_PropertyCollection()->create_Clone();
	model_properties->put_Property(Geoweb3d::GUI::ModelProperties::URL, ".\\data\\sdk\\models\\Vehicles\\Kia_Rio\\blue_kia.flt");
	model_properties->put_Property(Geoweb3d::GUI::ModelProperties::TRANSLATION_Z_OFFSET, 0.0); //elevation
	model_properties->put_Property(Geoweb3d::GUI::ModelProperties::TRANSLATION_Z_OFFSET_MODE, Geoweb3d::GUI::RELATIVE_MODE | Geoweb3d::GUI::IGNORE_VERTEX_Z);  //Relative to ground + ignore Z coordinate (i.e. ground clamped)
	
	model_properties->put_Property(Geoweb3d::GUI::ModelProperties::SCALE_X, 1.0); //scale in X
	model_properties->put_Property(Geoweb3d::GUI::ModelProperties::SCALE_Y, 1.0); //scale in Y
	model_properties->put_Property(Geoweb3d::GUI::ModelProperties::SCALE_Z, 1.0); //scale in Z			
	
	entity_models_.lock()->put_PropertyCollection(model_properties);

	LogMessage("\nMODEL DEFAULT PROPERTIES:");
	for (unsigned long i=0; i < model_properties->count(); i++)
	{
		LogMessage("PROPERTY %d: %s   == %s", i, model_properties->get_DefinitionCollection()->get_AtIndex(i)->property_name, model_properties->get_Property(i).to_string()->c_str());
	}

	std::string icon_url_path = "/icons/tealCircleWhiteBorder.png";
	if (!GetMediaPath(icon_url_path))
	{
		return;
	}

	Geoweb3d::IGW3DPropertyCollectionPtr icon_properties = entity_icons_.lock()->get_PropertyCollection()->create_Clone();
    icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_MODE, (int)Geoweb3d::GUI::IconProperties::LABEL_HOVER);
    icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_ATTRIBUTE, "Entity Number");
    icon_properties->put_Property(Geoweb3d::GUI::IconProperties::BALLOON_CONTENTS, (int)Geoweb3d::GUI::IconProperties::BALLOON_ATTR_TABLE);
    icon_properties->put_Property(Geoweb3d::GUI::IconProperties::ICON_URL, icon_url_path.c_str());
	// example of no icon - icon_properties->put_Property(Geoweb3d::GUI::IconProperties::ICON_URL, "");


	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_RED, 1.0); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_GREEN, 1.0); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_BLUE, 1.0); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_OUTLINE_RED, 0.0); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_OUTLINE_GREEN, 0.0); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_OUTLINE_BLUE, 0.0); 
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_POINT_SIZE, 15);   

	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::TRANSLATION_Z_OFFSET, 0.0); //elevation
	icon_properties->put_Property(Geoweb3d::GUI::IconProperties::TRANSLATION_Z_OFFSET_MODE, Geoweb3d::GUI::RELATIVE_MODE | Geoweb3d::GUI::IGNORE_VERTEX_Z);  //Relative to ground + ignore Z coordinate (i.e. ground clamped)
    entity_icons_.lock()->put_PropertyCollection(icon_properties);

	LogMessage("\nICON DEFAULT PROPERTIES:");
	for (unsigned long i=0; i < icon_properties->count(); i++)
	{
		LogMessage("PROPERTY %d: %s   == %s", i, icon_properties->get_DefinitionCollection()->get_AtIndex(i)->property_name, icon_properties->get_Property(i).to_string()->c_str());
	}
	LogMessage("\n");


	/* Set up LODs: show only models when very close, only icons when far away */
	entity_icons_.lock()->get_ClampRadiusFilter()->put_Enabled(true);
	entity_icons_.lock()->get_ClampRadiusFilter()->put_MinRadius(50.0);
	entity_icons_.lock()->get_ClampRadiusFilter()->put_MaxRadius(1000000000.0);
	entity_models_.lock()->get_ClampRadiusFilter()->put_Enabled(true);
	entity_models_.lock()->get_ClampRadiusFilter()->put_MinRadius(0.0);
	entity_models_.lock()->get_ClampRadiusFilter()->put_MaxRadius(500.0);
	
	Geoweb3d::IGW3DPropertyCollectionPtr field_values(attribute_fields->create_PropertyCollection());
	int label_field = field_values->get_DefinitionCollection()->get_IndexByName("Label");
	int balloon_field = field_values->get_DefinitionCollection()->get_IndexByName("Balloon");

	const unsigned number_of_entities  = 2000;

	const double center_lat  = 39.0;
	const double center_lon  = 125.75;
	const double center_elev  = 2.0;

    srand ( 22 );

    double R = .009;
    double Rsquared = R*R;
    double x,y,m;

	EntityInfo working_entityinfo;

    for(unsigned i = 0; i < number_of_entities; i++)
    {
        do
        {
            //make somewhat a uniform distribution
            x = R * (2 * closed_interval_rand(0,1) -1 );
            y = R * (2 * closed_interval_rand(0,1) -1 );
            m = x*x+y*y;
        }
        while (m > Rsquared);

		working_entityinfo.wgs84_location.put_X( center_lon - x );
		working_entityinfo.wgs84_location.put_Y( center_lat + y );
		working_entityinfo.wgs84_location.put_Z(  center_elev );

		working_entityinfo.heading_ = 0;
		working_entityinfo.pitch_ = 0;
		working_entityinfo.roll_ = 0;

		/* Set the entity ID attribute value */
		field_values->put_Property(0, (int)i);

		/* For the first entity, persistently display the balloon */
		if (i == 0)
		{
			field_values->put_Property(label_field, "I have a balloon!"); // Set the 'Label' attribute
			icon_properties->put_Property(Geoweb3d::GUI::IconProperties::BALLOON_MODE, (int)Geoweb3d::GUI::IconProperties::BALLOON_ON_ALWAYS); //always show the balloon
			entity_icons_.lock()->put_PropertyCollection(i, icon_properties); // set the icon to display its balloon always
		}

		/* For the next four entities, always display a multi-line label */
		else if (i >= 1 && i < 4)
		{
			field_values->put_Property(label_field, "This is an example \\nof a label that uses \\nmulti-line text");	// Set the 'Label' attribute
			icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_ATTRIBUTE, "Label");	//display the 'Label' attribute as the label
			icon_properties->put_Property(Geoweb3d::GUI::IconProperties::BALLOON_MODE, (int)Geoweb3d::GUI::IconProperties::BALLOON_ON_CLICK); //reset back to on click
			icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_MODE, (int)Geoweb3d::GUI::IconProperties::LABEL_ON); //always show the label
			entity_icons_.lock()->put_PropertyCollection(i, icon_properties); // set the icon to display its label always

		} else if (i > 4 && i < 10) {
			field_values->put_Property(label_field, "click me - example of single line text");	// Set the 'Label' attribute
			field_values->put_Property(balloon_field, "<table border=\"1\"><tr><td>Row 1 Cell 1</td><td>Row 1 Cell 2</td></tr><tr><td>Row 2 Cell 1</td><td>Row 2 Cell 2</td></tr></table>");	// Set the 'Label' attribute
			icon_properties->put_Property(Geoweb3d::GUI::IconProperties::BALLOON_ATTRIBUTE, "Balloon");	//
			icon_properties->put_Property(Geoweb3d::GUI::IconProperties::BALLOON_CONTENTS, (int)Geoweb3d::GUI::IconProperties::BALLOON_ATTR_TABLE);	//
			icon_properties->put_Property(Geoweb3d::GUI::IconProperties::BALLOON_ATTRIBUTE, "Label");	//display the 'Label' attribute as the label
			icon_properties->put_Property(Geoweb3d::GUI::IconProperties::BALLOON_MODE, (int)Geoweb3d::GUI::IconProperties::BALLOON_ON_CLICK); //only show the balloon when you click
			icon_properties->put_Property(Geoweb3d::GUI::IconProperties::LABEL_MODE, (int)Geoweb3d::GUI::IconProperties::LABEL_ON); //always show the label
			entity_icons_.lock()->put_PropertyCollection(i, icon_properties); 
		}

		/* From here on out, set keep things the same */
		else if (i == 10)
		{
			field_values->put_Property(1, "Just an entity");
		}

		//Geoweb3d::FieldValues and Geoweb3d::PropertyGroup will soon be removed completely from the API, so for now you can call get_PropertyCollection
		long feature_id = entity_layer_.lock()->create_Feature(field_values, &working_entityinfo.wgs84_location);
			
		entity_database_[feature_id] = working_entityinfo;
		/* we want to make sure we update all the fids at startup */
		dirty_fids_.insert( feature_id );
	
    } 

	  threadhandle_ = CreateThread(NULL, 0, runThreadFunction<MyCompanyExtension>, 
		 makeThreadInfo(this, &MyCompanyExtension::ThreadFunction, NULL), 0, NULL);

 }

void MyCompanyExtension::RotateEntity ( Geoweb3d::GUI::IGW3DGUIVectorLayerStreamResult* result, double hdg)
{
	if (entity_models_.expired())
	{
		LogMessage("Serious bug!  Our model representation no longer exists!");
		return;
	}

	//set the heading using a scratchpad (cheaper than creating a new variant)
	scratchpad_variant_ = hdg;
	result->put_VectorRepresentationProperty(entity_models_, Geoweb3d::GUI::ModelProperties::HEADING, scratchpad_variant_);

}

void MyCompanyExtension::ChangeIconColor ( Geoweb3d::GUI::IGW3DGUIVectorLayerStreamResult* result)
{
	if (entity_icons_.expired())
	{
		LogMessage("Serious bug!  Our icon representation no longer exists!");
		return;
	}

	static std::string greenIcon;
	static std::string purpleIcon;

	if (greenIcon.empty())
	{
		greenIcon = "/icons/grCircleWhiteBorder.png";
		if (!GetMediaPath(greenIcon))
		{
			return;
		}
	}

	if (purpleIcon.empty())
	{
		purpleIcon = "/icons/purpleCircleWhiteBorder.png";
		if (!GetMediaPath(purpleIcon))
		{
			return;
		}
	}

	//check the current properties of the feature/object
	const Geoweb3d::IGW3DPropertyCollection* current_properties = result->get_VectorRepresentationProperties(entity_icons_);
	if (!current_properties)
	{
		//if no unique properties exist for this feature, check the default properties for the representation
		current_properties = entity_icons_.lock()->get_PropertyCollection();
	}
	const char* current_icon = current_properties->get_AtIndex(Geoweb3d::GUI::IconProperties::ICON_URL)->raw_string(); //raw_string() is only safe if you know the property contains a string

	//We'll toggle the balloon mode, and change the icon color just to make it easier to spot
	if (strcmp(current_icon, greenIcon.c_str()) == 0)
	{
		result->put_VectorRepresentationProperty(entity_icons_, Geoweb3d::GUI::IconProperties::ICON_URL, purpleIcon.c_str());
	}
	else
	{
		result->put_VectorRepresentationProperty(entity_icons_, Geoweb3d::GUI::IconProperties::ICON_URL, greenIcon.c_str());
	}
}

 DWORD MyCompanyExtension::ThreadFunction(void *parm)
 {
	 while(!quit_)
	 {
		 Sleep(2);

		 if(entity_database_.empty())
			 continue;

		 //randomly pick an entity to update
		  int minc = 0;
		  int maxc = entity_database_.size();
          std::size_t iteratoroffset  = ( rand()%(maxc-minc) + minc  );

		  CritSectEx::Scope scope( database_thread_protection_ );

		  EntityDatabaseType::iterator itr = entity_database_.begin();
		  std::advance(itr, iteratoroffset );

		  EntityInfo &entityinfo = itr->second;
		  entityinfo.wgs84_location.put_X( entityinfo.wgs84_location.get_X() + .00001 );
		  entityinfo.wgs84_location.put_Y( entityinfo.wgs84_location.get_Y() + .00002 );
		  entityinfo.wgs84_location.put_Z( entityinfo.wgs84_location.get_Z() + .005 );

		  entityinfo.heading_ += 5.0;

		  /* Now we make sure the sdk updates this entity, as it has changed */
		  dirty_fids_.insert( itr->first ); 
	 }

	return 0;
 }

void MyCompanyExtension::LogMessage(const char * message, ...)
{

	#define MAX_LOG_MESSAGE_LENGTH 2048

	char msg[MAX_LOG_MESSAGE_LENGTH];
    va_list args;
    va_start( args, message );
    vsprintf(msg, message, args );
    va_end(args);
	if (dw_)
	{
		dw_->LogMessage(msg); 
	}
}

void MyCompanyExtension::OnPostDraw(  Geoweb3d::GUI::IGW3DGUIPostDrawContext *ctx )
{
	mouse_query_->PostDraw(ctx);

	current3DView_ = ctx->get_View();

	//Spin the eye:
	//float h, p, r;
	//current3DView_.lock()->get_Rotation(h, p, r);
	//current3DView_.lock()->put_Rotation(h+1.0, p, r);

	if( dirty_fids_.size() )
	{
		CritSectEx::Scope scope( database_thread_protection_ );
		
		this->reset();
			
		/* Now we stream, which prompts the Geoweb3d::IGW3DVectorLayerStream::OnStream callback to get called for each entity */
		entity_layer_.lock()->Stream( this );

		//LogMessage("streamed [%d] entities.\n",dirty_fids_.size());
		dirty_fids_.clear();
	}

	if (++framecount_ == 100)
	{
		LoadBuildings( ctx );

		//Code for setting the eyepoint
		// 
		if (first_pass_)
		{
			Geoweb3d::GW3DPoint eye(*ctx->get_View().lock()->get_Location());
			const double center_lat  = 39.0;
			const double center_lon  = 125.75;
			eye.put_X(125.75);
			eye.put_Y(39.0);
			eye.put_Z(8000.0);
			ctx->get_View().lock()->put_Location(eye);
			first_pass_ = false;
		}
	}

	//LogMessage("frame [%d]", framecount_); 


}


void MyCompanyExtension::OnPreDraw( Geoweb3d::GUI::IGW3DGUIPreDrawContext *ctx )
{
	
	mouse_query_->PreDraw (ctx);
	dw_->setCurrent3dView (ctx);
} 

void MyCompanyExtension::OnCreated (Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer)
{
	//IGW3DGUIGeoweb3dVectorLayerEventListener
	LogMessage("Layer '%s' was created.\n", layer.lock()->get_Name());
}

void MyCompanyExtension::OnDestroyed (Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer)
{
	//IGW3DGUIGeoweb3dVectorLayerEventListener
	LogMessage("Layer '%s' was destroyed.\n", layer.lock()->get_Name());
}

void MyCompanyExtension::OnEnabled (Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer)
{
	//IGW3DGUIGeoweb3dVectorLayerEventListener
	LogMessage("Layer '%s' was enabled.\n", layer.lock()->get_Name());
}

void MyCompanyExtension::OnDisabled (Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr layer)
{
	//IGW3DGUIGeoweb3dVectorLayerEventListener
	LogMessage("Layer '%s' was disabled.\n", layer.lock()->get_Name());
}

void MyCompanyExtension::OnCreated (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr representation)
{
	//IGW3DGUIGeoweb3dVectorRepresentationEventListener
	LogMessage("Representation of layer '%s' was created.\n", representation.lock()->get_VectorLayer().lock()->get_Name());

}

void MyCompanyExtension::OnDestroyed (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr representation)
{
	//IGW3DGUIGeoweb3dVectorRepresentationEventListener
	LogMessage("Representation of layer '%s' was destroyed.\n", representation.lock()->get_VectorLayer().lock()->get_Name());
}

void MyCompanyExtension::OnEnabled (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr representation)
{
	//IGW3DGUIGeoweb3dVectorRepresentationEventListener
	LogMessage("Representation of layer '%s' was enabled.\n", representation.lock()->get_VectorLayer().lock()->get_Name());
}

void MyCompanyExtension::OnDisabled (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr representation)
{
	//IGW3DGUIGeoweb3dVectorRepresentationEventListener
	LogMessage("Representation of layer '%s' was disabled.\n", representation.lock()->get_VectorLayer().lock()->get_Name());
}

void MyCompanyExtension::OnCurrent (Geoweb3d::GUI::IGW3DGUITocItemWPtr item, Geoweb3d::GUI::IGW3DGUITocItemWPtr previous)
{
	//IGW3DGUIGeoweb3dTocItemEventListener
	if (!item.expired())
	{
		LogMessage("TOC Item '%s' is now current!", item.lock()->get_Name());
	}
	else
	{
		LogMessage("No TOC Item is now current!");
	}
}

void MyCompanyExtension::OnChecked (Geoweb3d::GUI::IGW3DGUITocItemWPtr item)
{
	//IGW3DGUIGeoweb3dTocItemEventListener
	LogMessage("TOC Item '%s' is now checked!", item.lock()->get_Name());
}

void MyCompanyExtension::OnUnchecked (Geoweb3d::GUI::IGW3DGUITocItemWPtr item)
{
	//IGW3DGUIGeoweb3dTocItemEventListener
	LogMessage("TOC Item '%s' is now unchecked!", item.lock()->get_Name());
}

bool MyCompanyExtension::AllowChecked (const Geoweb3d::GUI::IGW3DGUITocItemWPtr item)
{
	//IGW3DGUIGeoweb3dTocItemEventFilter
	LogMessage("TOC Item '%s' allowed to be checked!", item.lock()->get_Name());
	return true;
}	

bool MyCompanyExtension::AllowUnchecked (const Geoweb3d::GUI::IGW3DGUITocItemWPtr item)
{
	//IGW3DGUIGeoweb3dTocItemEventFilter
	LogMessage("TOC Item '%s' allowed to be unchecked!", item.lock()->get_Name());
	return true;
}

bool MyCompanyExtension::AllowMoved (const Geoweb3d::GUI::IGW3DGUITocItemWPtr item, const Geoweb3d::GUI::IGW3DGUITocItemWPtr target)
{
	const char* target_name = target.expired() ? "NULL" : target.lock()->get_Name();
	//IGW3DGUIGeoweb3dTocItemEventFilter
	LogMessage("TOC Item '%s' allowed to be moved to target '%s'!", item.lock()->get_Name(), target_name);
	return true;
}

bool MyCompanyExtension::OnMenu (const Geoweb3d::GUI::IGW3DGUIMenuWPtr menu)
{
	// 'Smoke' is a POC representation (for a point layer) in the 4.0 beta that we will remove here
	// The representation selection menu can either be triggered from the 'Representation' frame on
	// the vector shelf, or in the 'represent As' submenu when right-clicking a vector layer

	LogMessage("Showing menu %s", menu.lock()->get_Id());

	//Check to see if this is the representation menu from the Vector shelf
	if (!strcmp(menu.lock()->get_Id(), "GW3DGUIMenuNewRepresentation"))
	{
		//get the item for the smoke representation
		Geoweb3d::GUI::IGW3DGUIMenuItemWPtr smokeRepItem = menu.lock()->get_MenuItemCollection()->get_ById("GW3DGUIMenuItemNewRepresentationSmoke");
		if (!smokeRepItem.expired())
		{
			//make the item invisible
			if (Geoweb3d::Succeeded(menu.lock()->get_MenuItemCollection()->put_Visible(smokeRepItem, false)))
			{
				LogMessage ("Removed the smoke representation from the menu!");
			}
		}
	}
	//Check to see if this is the Table of Contents context menu
	else if (!strcmp(menu.lock()->get_Id(), "GW3DGUIMenuToc"))
	{
		//Get the 'Represent as' item
		Geoweb3d::GUI::IGW3DGUIMenuItemWPtr repAsItem = menu.lock()->get_MenuItemCollection()->get_ById("GW3DGUIMenuItemTocLayerRepresentAs");
		if (!repAsItem.expired())
		{
			//The 'Represent as' item pulls up the representation menu as a submenu
			Geoweb3d::GUI::IGW3DGUIMenuWPtr repAsSubmenu = repAsItem.lock()->get_Menu();
			if (!repAsSubmenu.expired())
			{
				//get the item for the smoke representation
				Geoweb3d::GUI::IGW3DGUIMenuItemWPtr smokeRepItem = repAsSubmenu.lock()->get_MenuItemCollection()->get_ById("GW3DGUIMenuItemNewRepresentationSmoke");
				if (!smokeRepItem.expired())
				{
					//make the item invisible
					if (Geoweb3d::Succeeded(repAsSubmenu.lock()->get_MenuItemCollection()->put_Visible(smokeRepItem, false)))
					{
						LogMessage ("Removed the smoke representation from the menu!");
					}
				}
			}
		}
	}
	return true;
}
	
bool MyCompanyExtension::OnMenu (const Geoweb3d::GUI::IGW3DGUIMenuWPtr menu, Geoweb3d::GUI::IGW3DGUITocItemWPtr target)
{
	return OnMenu(menu);
}



/*
 * Windows messages for mouse and keyboard events.  Return true to propogate event to Geoweb3d.
 */
bool MyCompanyExtension::OnWinEvent( MSG* message, long* result )
{
	UINT imsg = message->message;
	WPARAM wp = message->wParam;
	LPARAM lp = message->lParam;

	switch (imsg) {
		case WM_MOUSEMOVE:
			break;
		case WM_KEYUP:
			LogMessage("key up = %c", wp);
			break;
		case WM_KEYDOWN:
			LogMessage("key down = %c", wp);
			break;
		case WM_LBUTTONDOWN:
			LogMessage("left button down");
			mouse_query_->Click();
			break;
		case WM_LBUTTONUP:
			LogMessage("left button up"); 
			break;
		case WM_RBUTTONDOWN:
			LogMessage("right button down");
			break;
		case WM_RBUTTONUP:
			LogMessage("right button up");
			break;
		default:
			break;
	};

	return true; //propegate
}


void MyCompanyExtension::OnXMLRead ( Geoweb3d::GUI::IGW3DGUIXMLReadContext *ctx )
{
	/*
	 *  Do not store the char* of the XML buffer (ctx->get_XML ()).  It will be null 
	 *  after the return of this function.  Process inline or copy the buffer.
	 *	If no custom data has been stored, this will be null!
	 */

	if (ctx->get_XML ())
	{
		xml_processor_->XMLRead (ctx->get_FileType(), ctx->get_XML ());
	}
}




void MyCompanyExtension::OnXMLWrite ( Geoweb3d::GUI::IGW3DGUIXMLWriteContext *ctx )
{
	xml_processor_->XMLWrite (ctx); 
}


void MyCompanyExtension::OnFinalConstruct( Geoweb3d::GUI::IGW3DGUIFinalConstructContext *ctx )
{
	if (!Geoweb3d::Succeeded(ctx->put_ApplicationUpdateTimer(this, 500)))
	{
		//error
	}

	ctx->get_Geoweb3dEventContext();

	if (!Geoweb3d::Succeeded(ctx->get_Geoweb3dEventContext()->put_VectorLayerEventListener(this)))
	{
		//error
	}
	if (!Geoweb3d::Succeeded(ctx->get_Geoweb3dEventContext()->put_VectorRepresentationEventListener(this)))
	{
		//error
	}
	if (!Geoweb3d::Succeeded(ctx->get_Geoweb3dEventContext()->put_TocItemEventListener(this)))
	{
		//error
	}

	if (!Geoweb3d::Succeeded(ctx->get_Geoweb3dEventContext()->put_TocItemEventFilter(this)))
	{
		//error
	}

	if (!Geoweb3d::Succeeded(ctx->get_Geoweb3dEventContext()->put_ContextMenuEventFilter(this)))
	{
		//error
	}
}

		
void MyCompanyExtension::OnStart( Geoweb3d::GUI::IGW3DGUIStartContext *ctx )
{
	CreateDockWindows(ctx->get_DockWindowModifiableCollection());

	//query the mouse position every frame (including what's under the mouse)
	xml_processor_ = new XMLProcessor(this);
	mouse_query_ = new Mouse3DQuery(this, ctx);
	mouse_query_->SetRepresentationQueriesEnabled(true);
	mouse_query_->SetClickOnlyMode(true);
	LoadEntities( ctx );
	//query entities with the mouse:
	mouse_query_->AddRepresentation(entity_models_);
	mouse_query_->AddRepresentation(entity_icons_);
	ctx->get_RebrandingContext()->put_Geoweb3dNativeDISEnabled(false);

	// Place a range ring
	const double center_lat  = 39.0;
	const double center_lon  = 125.75;
	RangeRings* rings = RangeRings::create("My Range Ring", ctx);
	rings->SetNumberOfAdditionalRingsPerRangeRingFeature(2);
	rings->SetRingInterval(0, 10); //first ring is 10 meters from the origin
	rings->SetRingInterval(1, 40); //second ring is 50 meters from the origin
	rings->SetRingInterval(2, 50); //third ring is 100 meters from the origin
	rings->SetRingColor(0, 1.0, 0.0, 0.0, 0.5);
	rings->SetRingColor(1, 0.0, 1.0, 0.0, 0.5);	
	rings->SetRingColor(2, 0.0, 0.0, 1.0, 0.5);
	rings->CommitStyleChanges();
	rings->AddRangeRingFeature(center_lon, center_lat);
	rings->RefreshRepresentation();

	// set units to decimal degrees and feet
	if (ctx->get_RebrandingContext()->get_Geoweb3dLatLongDisplayFormat() != Geoweb3d::GUI::IGW3DGUIApplicationRebrandingContext::DecimalDegrees)
	{
		ctx->get_RebrandingContext()->put_Geoweb3dLatLongDisplayFormat(Geoweb3d::GUI::IGW3DGUIApplicationRebrandingContext::DecimalDegrees);
	}

	if (ctx->get_RebrandingContext()->get_Geoweb3dElevationDisplayFormat() != Geoweb3d::GUI::IGW3DGUIApplicationRebrandingContext::Feet)
	{
		ctx->get_RebrandingContext()->put_Geoweb3dElevationDisplayFormat(Geoweb3d::GUI::IGW3DGUIApplicationRebrandingContext::Feet);
	}

	ctx->get_RebrandingContext()->put_ApplicationName("My Custom Application");
	if (!Geoweb3d::Succeeded(ctx->get_RebrandingContext()->put_ApplicationIcon("./data/icons/defaultIcon.png")))
	{
		LogMessage("Could not set application icon!");
	}

	ctx->get_RebrandingContext()->put_ShowProjectPathInTitleBar(false);

	ctx->get_RebrandingContext()->put_OpenDashboardOnStartup(false);

	//ctx->get_RebrandingContext()->put_ShowStatusBarButtonFullScreen(false);

	ctx->get_RebrandingContext()->put_ShowStatusBarButtonWindowLayout(false);

	//ctx->get_RebrandingContext()->put_ShowStatusBarButtonLocationClipboard(false);

	// Set new file extensions for project, layer, and representation files.

	Geoweb3d::GW3DResult result; 
	if (ctx->get_RebrandingContext()->put_CustomFileExtension (Geoweb3d::GUI::Geoweb3dFileTypeProject, "xproj") != Geoweb3d::GW3D_sOk) {
		LogMessage ("Error setting new extension for project file"); 
	}
	result = ctx->get_RebrandingContext()->put_CustomFileExtension (Geoweb3d::GUI::Geoweb3dFileTypeLayer, "xlyr");
	result = ctx->get_RebrandingContext()->put_CustomFileExtension (Geoweb3d::GUI::Geoweb3dFileTypeRepresentation, "xrep");

	CreateShelves( ctx );
	CustomizeShelves( ctx );

}	

void MyCompanyExtension::OnApplicationUpdate ( Geoweb3d::GUI::IGW3DGUIApplicationUpdateContext *ctx )
{
	Geoweb3d::GUI::IGW3DGUIView3dCollection* views3d = ctx->get_View3dCollection();

	Geoweb3d::GUI::IGW3DGUIViewWebCollection* viewsWeb = ctx->get_ViewWebCollection();
}

void MyCompanyExtension::OnStop( Geoweb3d::GUI::IGW3DGUIStopContext *ctx )
{
	quit_ = true;

	if( threadhandle_ ) //close and make sure the thread cleaned up uk
	{
		DWORD dwEvent =  WaitForSingleObject(threadhandle_, 2000 );
		switch (dwEvent) 
		{ 
		case WAIT_OBJECT_0: 
			LogMessage("Thread Exited Ok...");
		break; 

		case WAIT_TIMEOUT:
			LogMessage("Possible Bug with your code, please make it so it will exit...");
		break;
		default: 
		LogMessage("Wait for thread error: %d", GetLastError());
		}

		CloseHandle( threadhandle_ );
		threadhandle_ = 0;
	}

	dw_->close();
}

void MyCompanyExtension::CreateDockWindows( Geoweb3d::GUI::IGW3DGUIDockWindowModifiableCollection *dockWindows )
{
	// Create a Qt dock window
	Geoweb3d::GUI::IGW3DGUIDockWindowWPtr dockWindow1 = dockWindows->create( "Example Qt Window", "MyCompanyDockWindow1", Geoweb3d::GUI::IGW3DGUIDockWindowModifiableCollection::RightDockArea );

	if (dockWindow1.expired()) {
		LogMessage("Serious Bug detected creating a Dock Window.. will not continue"); 
		return;
	}

	dockWindow1.lock()->put_ShowInMenus (true);
	dockWindow1.lock()->put_Visible (true); 

	QWidget* dockWindow1Widget = QtWidgetHelper::GetWidget(dockWindow1.lock()->get_AttachHandle());

	dw_ = new EntityDockWindow (dockWindow1Widget, this);

	// Create a non Qt dock window
	Geoweb3d::GUI::IGW3DGUIDockWindowWPtr dockWindow = dockWindows->create( "Non Qt Window", "MyCompanyDockWindow", Geoweb3d::GUI::IGW3DGUIDockWindowModifiableCollection::RightDockArea );
	
	if(dockWindow.expired()) 
	{
		LogMessage("Serious Bug detected creating a Dock Window.. will not continue"); 
		return;
	}

	dockWindow.lock()->put_ShowInMenus(true);
	dockWindow.lock()->put_Visible(true);

//	 Creating textbox for input
	HWND hwnd_txtbox = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "Line one",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
							  CW_USEDEFAULT, CW_USEDEFAULT, 200, 20,	// x, y, w, h
							  (HWND)dockWindow.lock()->get_AttachHandle(), (HMENU)(123),
							  (HINSTANCE) GetWindowLong ((HWND)dockWindow.lock()->get_AttachHandle(), GWL_HINSTANCE), NULL);

  WNDCLASS wc = {0};
  wc.lpszClassName = TEXT( "Color dialog box" );
  wc.hInstance     = hinstDLL ; 
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.lpfnWndProc   = WndProc ;
  
  RegisterClass(&wc);
  CreateWindow( wc.lpszClassName, TEXT("Color dialog box"), 
	  WS_CHILD | WS_VISIBLE , 0, 100, 100, 100, (HWND)dockWindow.lock()->get_AttachHandle(), 0, hinstDLL , 0);  
}


void MyCompanyExtension::CustomizeShelves( Geoweb3d::GUI::IGW3DGUIStartContext *ctx )
{
	Geoweb3d::GUI::IGW3DGUIShelfWPtr enviroShelf = ctx->get_ShelfModifiableCollection()->get_ById("GW3DShelfEnvironment");
	ctx->get_ShelfModifiableCollection()->put_Name(enviroShelf, "Weather");
	//ctx->get_ShelfModifiableCollection()->put_Excluded(enviroShelf, true);
	Geoweb3d::GUI::IGW3DGUIFrameWPtr frame;
	enviroShelf.lock()->get_FrameCollection()->reset();
	LogMessage("Shelf '%s' has %d frames:", enviroShelf.lock()->get_Id(), enviroShelf.lock()->get_FrameCollection()->count());
	bool flip = true;
	while (enviroShelf.lock()->get_FrameCollection()->next(&frame))
	{
		ctx->get_ShelfModifiableCollection()->put_FrameVisible(enviroShelf, frame, flip);
		
		LogMessage("  %s%s", frame.lock()->get_Id(), flip ? "" : ": hidden");
		flip = !flip;
	}
}

void MyCompanyExtension::CreateShelves( Geoweb3d::GUI::IGW3DGUIStartContext *ctx )
{
	std::string example_icon_path = "/icons/geoweb3d.png";
	if (!GetMediaPath(example_icon_path))
	{
		return;
	}

	Geoweb3d::GUI::IGW3DGUIShelfWPtr my_shelf = ctx->get_ShelfModifiableCollection()->create("Plugin Shelf", "MyCompanyShelf1", example_icon_path.c_str());
	my_shelf.lock()->put_Visible(true);

	for (int i=0; i<settings_manager_->GetNumberOfFramesForCustomShelfExample(); ++i)
	{
		std::ostringstream oss;
		oss << "My Frame " << i;
		std::string frame_label = oss.str();

		oss.clear();
		oss << "myCompanyFrame" << i;
		std::string frame_id = oss.str();

		Geoweb3d::GUI::IGW3DGUIFrameWPtr my_frame = my_shelf.lock()->get_FrameCollection()->create(frame_label.c_str(), frame_id.c_str(), example_icon_path.c_str(), 180);

		ctx->get_ShelfModifiableCollection()->put_FrameVisible(my_shelf, my_frame, true);

		//	 Creating textbox for input
		HWND hwnd_txtbox = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "Line one",
								  WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
								  5, 5, 170, 25,	// x, y, w, h
								  (HWND)my_frame.lock()->get_AttachHandle(), (HMENU)(123),
								  (HINSTANCE) GetWindowLong ((HWND)my_frame.lock()->get_AttachHandle(), GWL_HINSTANCE), NULL);
	}

}



LRESULT CALLBACK PanelProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  HDC hdc;
  PAINTSTRUCT ps; 
  RECT rect;

  switch(msg)  
  {
    case WM_PAINT:
    {
        GetClientRect(hwnd, &rect);
        hdc = BeginPaint(hwnd, &ps);
        SetBkColor(hdc, gColor);
        ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, TEXT(""), 0, NULL);
        EndPaint(hwnd, &ps);
        break;
    }
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

void RegisterPanel(void) {

  WNDCLASS rwc = {0};
  rwc.lpszClassName = TEXT( "Panel" );
  rwc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  rwc.lpfnWndProc   = PanelProc;
  RegisterClass(&rwc);
}


COLORREF ShowColorDialog(HWND hwnd) {

  CHOOSECOLOR cc;                 
  static COLORREF crCustClr[16];     

  ZeroMemory(&cc, sizeof(cc));
  cc.lStructSize = sizeof(cc);
  cc.hwndOwner = hwnd;
  cc.lpCustColors = (LPDWORD) crCustClr;
  cc.rgbResult = RGB(0, 255, 0);
  cc.Flags = CC_FULLOPEN | CC_RGBINIT;
  ChooseColor(&cc);

  return cc.rgbResult; 
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  static HWND hwndPanel;
   
  switch(msg)  
  {
    case WM_CREATE:
  	{    
		RECT r;
		GetWindowRect(hwnd, &r);

          CreateWindow(TEXT("button"), TEXT("Color"),    
	       WS_VISIBLE | WS_CHILD ,
	       15,  5, 60, 20,        
	       hwnd, (HMENU) 1, NULL, NULL);     

          RegisterPanel();
          hwndPanel = CreateWindow(TEXT("Panel"), NULL, 
		  WS_CHILD | WS_VISIBLE,
		  15,  30, 60, 20,   
		  hwnd, (HMENU) 2, NULL, NULL);   
          break;
	}

	case WM_COMMAND:
	{
           gColor = ShowColorDialog(hwnd);
           InvalidateRect(hwndPanel, NULL, TRUE);    
           break;
	}

      case WM_DESTROY:
      {
          PostQuitMessage(0);
          break;
      }
  }

  return DefWindowProc(hwnd, msg, wParam, lParam);
}

//Geoweb3d::IGW3DRegistration *registration  is only valid for the lifetime of this call.
extern "C" BASICDESKTOPEXTENSION_API void insertSupportedClassInterfaces( Geoweb3d::IGW3DRegistration *registration )
{
	registration->add_SupportedClassInterface( GUID_GEOWEB3DDESKTOP_SDK );
}

extern "C" BASICDESKTOPEXTENSION_API bool isClassInterfaceSupported( const GW3DGUID &class_type)
{
	if(IsEqualGW3DGUID( class_type, GUID_GEOWEB3DDESKTOP_SDK ))
	{
		return true;
	}

	return false;
}

extern "C" BASICDESKTOPEXTENSION_API Geoweb3d::IGW3DIBaseObject *createClassInstance( const GW3DGUID &class_type )
{

	try
	{
		if( IsEqualGW3DGUID( class_type, GUID_GEOWEB3DDESKTOP_SDK ))
		{
			return &(MyCompanyExtension::Instance());
		} 
		else if( IsEqualGW3DGUID( class_type, GUID_GEOWEB3DDESKTOP_REBRANDING_TOKEN ))
		{
			return MyCompanyExtension::Instance().GetRebrandingToken();
		} 
	}
	catch(std::exception &e) 
    { 
       printf("%s\n", e.what());
    } 

	printf("CreateClassInstance BUG!!!!!!  what class was I to create? \n");
	//this will only happen if you added something in InsertSupportedClassInterfaces
	//that you do not support.  This means you have a bug integrating with Geoweb3d
	return NULL;
}


extern "C" BASICDESKTOPEXTENSION_API void destroyClassInstance( const GUID &class_type, Geoweb3d::IGW3DIBaseObject *p )
{
	delete p;
}

extern "C" BASICDESKTOPEXTENSION_API void getPluginDescription( Geoweb3d::GW3DMetaDataPtr info) 
{	
		//This is your information:
		info->name		= PLUGIN_NAME;			// Name of the plugin 
		info->author	= PLUGIN_AUTHOR;		// Name of the plugin author
		info->web		= PLUGIN_WEB;			// Authors website if any 
		info->guid		= PLUGIN_GUID;			// Plugins unique GUID
		info->description = PLUGIN_DESC;		// *Short* description of plugin functionality (may be multiple lines)
		info->plugin_version = PLUGIN_VERSION;	//version of your plugin, not the API version
		
		//this is not your information, but the SDK API version you compile against.
		info->gw3dapi_version = GW3D_API_VERISON;
}


//API will only get called once, when a user manually adds this plugin
extern "C" BASICDESKTOPEXTENSION_API int install( ) //optional interface, not required to be a plug-in
{
	//are we allowed to run on this system etc?  user rights etc?
	printf("Install called\n");

	return 0; //non-0 value will be a fail.  error codes not yet defined.
}

//API will only get called once, when a user manually removes this plugin
extern "C" BASICDESKTOPEXTENSION_API void unInstall( ) //optional interface, not required to be a plug-in
{
	printf("UnInstall called\n");
}


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hinstDLL = hModule;

		{
		char filename[256];
		if (GetModuleFileNameA((HINSTANCE)hModule, filename, 255))
			LoadLibraryA(filename);
		}

		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif