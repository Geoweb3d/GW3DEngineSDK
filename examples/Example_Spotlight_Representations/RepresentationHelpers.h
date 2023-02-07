#include "engine/IGW3DGeoweb3dSDK.h"

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
/// 					RepresentAsSpotLights
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

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsSpotLights( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr light_layer );