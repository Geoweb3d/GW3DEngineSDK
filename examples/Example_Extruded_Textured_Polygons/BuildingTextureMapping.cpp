////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	BuildingTextureMapping.cpp
//
// summary:	Implements the building classifier class
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "BuildingTextureMapping.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "Geoweb3dCore/GeometryExports.h"
#include "Geoweb3dCore/LayerParameters.h"


using namespace Geoweb3d;
using namespace Vector;
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="rep">	The rep. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

BuildingTextureMapping::BuildingTextureMapping(Geoweb3d::IGW3DVectorRepresentationWPtr rep) : rep_(rep),nIndex_(0)
{
    roof_propertycollection_id_for_oddnumbered_features_ = 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Destructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

BuildingTextureMapping::~BuildingTextureMapping() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets the count. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long BuildingTextureMapping::count() const
{
    return static_cast<unsigned long>( selection_set_fids_.size() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Nexts the given pp value. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="ppVal">	[in,out] If non-null, the value. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool BuildingTextureMapping::next( int64_t *ppVal )
{
    if(nIndex_ >= selection_set_fids_.size())
    {
        *ppVal = 0;
        reset();
        return false;
    }

    *ppVal =  get_AtIndex( nIndex_ );
    ++nIndex_;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Resets this object. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void BuildingTextureMapping::reset()
{
    nIndex_ = 0;
}
int64_t BuildingTextureMapping::operator[](unsigned long index)
{
    return get_AtIndex( index );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets at index. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="index">	Zero-based index of the. </param>
///
/// <returns>	at index. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

int64_t BuildingTextureMapping::get_AtIndex( unsigned long index )
{
    if(index < selection_set_fids_.size() )
        return selection_set_fids_[ index ];

    return 0;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the stream action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="result">	[out] if non-null, the result. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool BuildingTextureMapping::OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result )
{

    static  const  Geoweb3d::IGW3DDefinitionCollection* repdef = rep_.lock()->get_PropertyCollection( )->get_DefinitionCollection();

    const  Geoweb3d::IGW3DAttributeCollection *attibutes = result->get_AttributeCollection();

    //this is just showing how to drive down into the defintions if interested...
    const  Geoweb3d::IGW3DDefinitionCollection* def = attibutes->get_DefinitionCollection();

    int index = def->get_IndexByName("Height");

    if(index < 0)
    {
        printf("File does not have height, so I'm not sure if my mapping will work, aborted!\n");
        return false;
    }

    const Geoweb3d::IGW3DVariant &val = attibutes->get_Property( index );

    static int Extrusion_height_index = repdef->get_IndexByName("EXTRUSION_HEIGHT");

    double height = val.to_double();
    result->put_VectorRepresentationProperty(rep_, Extrusion_height_index,  height );

    if(height <10)
    {
        result->put_VectorRepresentationProperty(rep_, ExtrudedPolygonProperties::TEXTURE_SIDE_REPEAT_N_TIMES_Y,  1.0 );
    }
    else if(height < 20)
    {
        result->put_VectorRepresentationProperty(rep_,  ExtrudedPolygonProperties::TEXTURE_SIDE_REPEAT_N_TIMES_Y,  2.0 );
        static int color_red_index = repdef->get_IndexByName("RED");
        static int color_green_index = repdef->get_IndexByName("GREEN");
        static int color_blue_index = repdef->get_IndexByName("BLUE");

        result->put_VectorRepresentationProperty(rep_,color_red_index,  1.0 );
        result->put_VectorRepresentationProperty(rep_,color_green_index,0.0 );
        result->put_VectorRepresentationProperty(rep_,color_blue_index, 0.0 );
        result->put_VectorRepresentationProperty(rep_,ExtrudedPolygonProperties::TEXTURE_TO_COLOR_BLEND,  0.2 ); //only show the color
    }
    else if(height < 30)
    {
        result->put_VectorRepresentationProperty(rep_,ExtrudedPolygonProperties::TEXTURE_SIDE_REPEAT_N_TIMES_Y,  3.0 );
    }
    else if(height < 40)
    {
        result->put_VectorRepresentationProperty(rep_, ExtrudedPolygonProperties::TEXTURE_SIDE_REPEAT_N_TIMES_Y,  4.0 );
    }
    else if(height < 50)
    {
        result->put_VectorRepresentationProperty(rep_, ExtrudedPolygonProperties::TEXTURE_SIDE_REPEAT_N_TIMES_Y,  5.0 );
    }
    else if(height < 60)
    {
        result->put_VectorRepresentationProperty(rep_, ExtrudedPolygonProperties::TEXTURE_SIDE_REPEAT_N_TIMES_Y,  6.0 );
    }
    else if(height < 70)
    {
        result->put_VectorRepresentationProperty(rep_, ExtrudedPolygonProperties::TEXTURE_SIDE_REPEAT_N_TIMES_Y,  7.0 );
    }
    else
    {
        result->put_VectorRepresentationProperty(rep_, ExtrudedPolygonProperties::TEXTURE_SIDE_REPEAT_N_TIMES_Y,  8.0 );
    }


    double longestside = 0;

    {   //real way to find the longest side.
        const Geoweb3d::GW3DPolygon *poly = dynamic_cast<const Geoweb3d::GW3DPolygon*>( result->get_Geometry() );

        GW3DLinearRingPtr ring = poly->get_ExteriorRing();

        Geoweb3d::GW3DPoint pt1;
        Geoweb3d::GW3DPoint pt2;

        double perimeter = 0;

        ring->get_Point(0, &pt1);
        ring->get_Point(0, &pt2);
        //the length of the longest side is less than
        //the sum of the other sides in the polygon.
        for(int i = 0; i < ring->get_NumPoints(); ++i)
        {
            ring->get_Point(i, &pt1);

            double distance;
            pt1.get_DistanceInMeters(&pt2, false, distance);

            perimeter +=distance;

            if (distance > longestside)
                longestside = distance;

            pt2 = pt1;
        }

        //double sum = perimeter;
        //// validation.... subtract out the longest side from the sum of sides
        //sum -= longestside;
        //// validation a polygon from the sides

        //if(longestside < sum)
        //{
        //	printf("test proved to be true\n");
        //}
    }

    // use longest bounding edge and figure out our repeat
    //determine if the rectangle is portrait or landscape
    //Geoweb3d::GeometryType t = result->get_Geometry()->getGeometryType();

    //const Geoweb3d::GW3DPolygon *poly = dynamic_cast<const Geoweb3d::GW3DPolygon*>( result->get_Geometry() );

    //Geoweb3d::GW3DEnvelope envelope;
    //poly->getEnvelope(&envelope);

    //double dy = envelope.MaxY - envelope.MinY;
    //double dx = envelope.MaxX - envelope.MinX;

    //bool x_islongest = false;

    //if(max(dy, dx) == dx)
    //x_islongest = true;

    //float roof_repeat = 1.0;



    //Geoweb3d::GW3DPoint point1(envelope.MaxX, envelope.MaxY);
    //Geoweb3d::GW3DPoint point2(envelope.MinX, envelope.MinY);

    double roof_length = longestside;
    //point1.DistanceInMeters(&point2, false, roof_length);

    if(roof_length < 10)
    {
        result->put_VectorRepresentationProperty(rep_, ExtrudedPolygonProperties::TEXTURE_TOP_REPEAT_N_TIMES_Y,  1.0 );
    }
    else if(roof_length < 40)
    {

        static int color_red_index = repdef->get_IndexByName("RED");
        static int color_green_index = repdef->get_IndexByName("GREEN");
        static int color_blue_index = repdef->get_IndexByName("BLUE");

        result->put_VectorRepresentationProperty(rep_,color_red_index,  0.0 );
        result->put_VectorRepresentationProperty(rep_,color_green_index,0.0 );
        result->put_VectorRepresentationProperty(rep_,color_blue_index, 1.0 );

        result->put_VectorRepresentationProperty(rep_,  ExtrudedPolygonProperties::TEXTURE_TOP_REPEAT_N_TIMES_Y,  1.5 );
    }
    else
    {
        result->put_VectorRepresentationProperty(rep_,  ExtrudedPolygonProperties::TEXTURE_TOP_REPEAT_N_TIMES_Y,  2.0 );
    }

    if(result->get_ObjectID()%2)
        result->put_VectorRepresentationProperty(rep_,  ExtrudedPolygonProperties::TEXTURE_TOP_PALETTE_INDEX,  roof_propertycollection_id_for_oddnumbered_features_ );


    // keep processing.. if you want to break early, return false.
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the error action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool BuildingTextureMapping::OnError(/*todo*/ )
{
	return true;
    //something was done by the user in the streamer that messed things up so bad we have to tell you about it.
}

