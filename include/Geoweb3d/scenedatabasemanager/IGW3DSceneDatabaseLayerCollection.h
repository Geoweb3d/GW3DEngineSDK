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
#include "GW3DInterFace.h"
#include <Geoweb3d/core/IGW3DCollection.h>

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Collection of Scene Database Layer Objects. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct IGW3DSceneDatabaseLayerCollection : public IGW3DCollection< IGW3DSceneDatabaseLayerPtr >
    {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Array index operator.                                         </summary>
        ///
        /// <param name="index"> Zero-based index of the database layer.            </param>
        /// 
        /// <remarks> This will return a shared pointer, and as such the client does 
        ///           not need to manage the memory returned by this.               </remarks>
        /// 
        /// <returns> The indexed database layer object.                            </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DSceneDatabaseLayerPtr operator[]( unsigned long index ) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Gets the database layer at index.                             </summary>
        ///
        /// <param name="index"> Zero-based index of the database layer.            </param>
        ///
        /// <remarks> This will return a shared pointer, and as such the client does 
        ///           not need to manage the memory returned by this.               </remarks>
        /// 
        /// <returns> The database layer object at index.                           </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DSceneDatabaseLayerPtr get_AtIndex( unsigned long index ) const = 0;

    };

}
