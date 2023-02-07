#include "pch.h"
#include "EntityController.h"


using namespace Geoweb3d;


EntityController::EntityController()
{}

EntityController::~EntityController() 
{}

bool EntityController::OnStream(IGW3DVectorLayerStreamResult *result )
{

	//no need to validate it exists, because this class said it does already (as we tell it the fids_)
	EntityInfo &entityinfo_  = entity_database_[result->get_ObjectID()];
   
    //we make sure we only go into the sdk once for a variable value, as we
    //are trying to be useable with over 100,000 objects on older hardware
    GW3DGeometry * geometry = result->get_EditableGeometry();

    if(!geometry) 
        return false; //must be we are not in edit mode????

    if(geometry->get_GeometryType() == gtPOINT_25D)
    {
        GW3DPoint* pt = dynamic_cast<GW3DPoint*>( geometry );
		pt->put_X( entityinfo_.wgs84_location.get_X());
		pt->put_Y( entityinfo_.wgs84_location.get_Y());
		pt->put_Z( entityinfo_.wgs84_location.get_Z());
    }
    else
    {
        printf("Need to add code for this other geometry type\n");
    }

	RotateEntity(result, entityinfo_.heading_);

 
    return true;
}
