//////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// Geoweb3d SDK license under which the SDK provided.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../../core/GW3DInterFace.h"

namespace Geoweb3d
{

//Here and not in the interface as this is not implemented stuff.
struct	IGW3DNode;
_GW3D_SMARTPTR_TYPEDEF( IGW3DNode );

//Nodes can have state, as well as children (IGW3DSpatialCollection) that have state as well.
//Being smart about how you are changing (or not changing) states can make or break performance.
//Example would be, is if you add children, add them all under the same node with the same state (best case), or
//make all the children have the same state (second best case). If you add a node, set color, add another node,
//set to a different color, then add a 3rd node that was the same state as the 1st node, you are not optimized
//and will always be uploading state information to the GPU instead of just focusing on rendering....


//NOTE You are not involved in the actual traversal of the scene graph, as all you are doing is building up
//an in memory "model" that the SDK can load from memory (instead of off disk for example).
extern "C++"
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> TODO </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct IGW3DNode : public IGW3DSpatial
    {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DSpatialCollection* get_SpatialCollection() = 0;
    };
}
}
