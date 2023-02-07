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
    /// <summary> Collection of Scene Database Driver Objects </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct IGW3DSceneDatabaseManagerDriverCollection : public IGW3DCollection< IGW3DSceneDatabaseManagerDriverPtr >
    {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Array index operator.                                         </summary>
        ///
        /// <param name="index"> Zero-based index of the database driver.           </param>
        /// 
        /// <remarks> This will return a shared pointer, and as such the client does 
        ///           not need to manage the memory returned by this.               </remarks>
        /// 
        /// <returns> The indexed database driver object.                           </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DSceneDatabaseManagerDriverPtr operator[]( unsigned long index ) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Gets the database driver at index.                            </summary>
        ///
        /// <param name="index"> Zero-based index of the database driver.           </param>
        ///
        /// <remarks> This will return a shared pointer, and as such the client does 
        ///           not need to manage the memory returned by this.               </remarks>
        /// 
        /// <returns> The database driver object at index.                          </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DSceneDatabaseManagerDriverPtr get_AtIndex( unsigned long index ) const = 0;

    };

}
