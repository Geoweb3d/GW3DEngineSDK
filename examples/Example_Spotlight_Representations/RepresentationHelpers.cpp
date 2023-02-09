#include "RepresentationHelpers.h"

#include "engine/GW3DVector.h"
#include "common/GW3DCommon.h"

#include "Geoweb3dCore/LayerParameters.h"

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsCustom( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer)
{

    Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "Custom" );

    Geoweb3d::GW3DResult validitychk;

    if(!Geoweb3d::Succeeded( validitychk =  driver.lock()->get_CapabilityToRepresent( layer ) ))
    {
        printf("Asked to represent a vector layer with a representation, but the layer is not compatible!\n");
    }
    else
    {
        // prints the values the layers has to help the user know what they can attribute map into the representation
        const Geoweb3d::IGW3DDefinitionCollection* def = layer.lock()->get_AttributeDefinitionCollection();

        printf("Attribute Names within the layer: [%s] you can map to the a custom property: \n", layer.lock()->get_Name() );
        for(unsigned i = 0; i < def->count(); ++i )
        {
            printf("\t[%s]\n", def->get_AtIndex(i)->property_name  );
        }


        //good to go!
        Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
        return driver.lock()->get_RepresentationLayerCollection()->create( layer, params );
    }

    return Geoweb3d::IGW3DVectorRepresentationWPtr();
}


Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsSpotLights( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer )
{
    Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "SpotLight" );

    Geoweb3d::GW3DResult validitychk;

    if(!Geoweb3d::Succeeded( validitychk =  driver.lock()->get_CapabilityToRepresent( layer ) ))
    {
        printf("Asked to represent a vector layer with a representation, but the layer is not compatible!\n");
    }
    else
    {
        // prints the values the layers has to help the user know what they can attribute map into the representation
        const Geoweb3d::IGW3DDefinitionCollection* def = layer.lock()->get_AttributeDefinitionCollection();

        printf("Attribute Names within the layer: [%s] you can map to the a SpotLight property: \n", layer.lock()->get_Name() );
        for(unsigned i = 0; i < def->count(); ++i )
        {
            printf("\t[%s]\n", def->get_AtIndex(i)->property_name  );
        }


        //good to go!
        Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
        return driver.lock()->get_RepresentationLayerCollection()->create( layer, params );
    }

    return Geoweb3d::IGW3DVectorRepresentationWPtr();
}