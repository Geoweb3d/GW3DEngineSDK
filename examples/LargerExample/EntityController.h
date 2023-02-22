#pragma once
#include "CritSectEx.h"
#include <map>
#include <set>
#include "engine/IGW3DVectorLayerStream.h"
#include "common/GW3DCommon.h"
#include "Geoweb3dCore/GeometryExports.h"
#include "engine/IGW3DVectorLayerStreamResult.h"



class EntityController : public Geoweb3d::IGW3DVectorLayerStream
{
public:

	struct EntityInfo
	{
		Geoweb3d::GW3DPoint wgs84_location;
		float heading_;
		float pitch_;
		float roll_;
	};

    EntityController();
    virtual ~EntityController(void);
   
	virtual void reset()
    {
		index_itr_= dirty_fids_.begin();
    }

private:

	virtual void RotateEntity ( Geoweb3d::IGW3DVectorLayerStreamResult*, double hdg) = 0; 
    
	//These get called by the SDK when Stream is called on the layer.
    bool OnStream(Geoweb3d::IGW3DVectorLayerStreamResult *result) override;
	bool OnError(/*todo*/) override
	{
		//something was done by the user in the streamer that messed things up so bad we have to tell you about it
		return false;
	}
    // These are required to be implemented in a IGW3DVectorLayerStream!
    // *NOTE* if [count] returns 0, then no other of APIs to index
    // into selection_set_fids_ will get called.  What this
    // means is if you are streaming a whole layer, its safe to
    // return 0 for everything.

    virtual unsigned long count() const
    {
		return static_cast<unsigned long>( dirty_fids_.size() );
    }

    virtual bool next( int64_t *ppVal )
    {
		if(index_itr_ == dirty_fids_.end())
		{
			*ppVal = 0;
			reset();
			return false;
		}

		*ppVal = (*index_itr_);

		++index_itr_;
		return true;
    }

    virtual int64_t operator[](unsigned long index)
    {
        return get_AtIndex( index );
    }
    virtual int64_t get_AtIndex( unsigned long index )
    {
		std::set<long>::iterator itr = dirty_fids_.begin();
		std::advance( itr, index );
		return (*itr);
    }

	virtual Geoweb3d::IGW3DVectorRepresentationWPtr GetEntityRepresentation() = 0;

protected:
	typedef std::map<unsigned, EntityInfo> EntityDatabaseType;
	EntityDatabaseType entity_database_;
	std::set<long> dirty_fids_;
	std::set<long>::iterator index_itr_;

private:
    //this is an expensive object to create and destroy, so keep a scratchpad/reusable object
    Geoweb3d::IGW3DVariant scratchpad_;
    Geoweb3d::IGW3DPropertyCollectionPtr props_scratchpad_;
    Geoweb3d::IGW3DVectorRepresentationWPtr rep_;
};

