////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	BuildingTextureMapping.h
//
// summary:	Declares the building classifier class
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include "engine/IGW3DVectorLayerStream.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Building classifier. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class BuildingTextureMapping : public Geoweb3d::IGW3DVectorLayerStream
{
    struct Color
    {
        double red;
        double green;
        double blue;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Constructor. </summary>
        ///
        /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
        ///
        /// <param name="r">	The double to process. </param>
        /// <param name="g">	The double to process. </param>
        /// <param name="b">	The double to process. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        Color(double r, double g, double b)
            :red(r), green(g), blue(b)
        {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the. </summary>
        ///
        /// <value>	. </value>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

    };

public:
    BuildingTextureMapping(Geoweb3d::IGW3DVectorRepresentationWPtr rep);
    virtual ~BuildingTextureMapping(void);


    virtual bool OnStream(  Geoweb3d::IGW3DVectorLayerStreamResult *result );
    virtual bool OnError(/*todo*/ ) ;

    // These are required to be implimented!  *NOTE* if
    // [count] returns 0, then no other of APIs to index
    // into selection_set_fids_ will get called.  What this
    // means is if you are streaming a whole layer, its safe to
    // return 0 for everything.

    virtual unsigned long count() const;
    virtual bool next( int64_t *ppVal );
    virtual void reset();
    virtual int64_t operator[](unsigned long index);
    virtual int64_t get_AtIndex( unsigned long index );

    int roof_propertycollection_id_for_oddnumbered_features_;
protected:

    /// <summary>	The rep. </summary>
    Geoweb3d::IGW3DVectorRepresentationWPtr rep_;
    std::vector<long> selection_set_fids_;
    /// <summary>	The index. </summary>
    unsigned nIndex_;
};
