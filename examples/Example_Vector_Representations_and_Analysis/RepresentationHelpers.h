#include "engine/IGW3DGeoweb3dSDK.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// /
/// 					RepresentAsLights
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

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsLights( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr light_layer );

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

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsExtruded( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// /
/// 					RepresentAsCustom
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

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsCustom( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// /
/// 					UpdateDefaultPolygonExtrusion
/// /.
/// </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="polygonrepresentation">	The polygonrepresentation. </param>
/// <param name="new_height">				Height of the new. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void UpdateDefaultPolygonExtrusion(Geoweb3d::IGW3DVectorRepresentationWPtr polygonrepresentation, double new_height = 25.);