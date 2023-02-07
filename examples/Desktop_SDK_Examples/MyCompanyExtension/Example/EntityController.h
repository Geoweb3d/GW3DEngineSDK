#pragma once

#include "gui/IGW3DGUIVectorLayerStream.h"

#include "core/GW3DGUIInterFace.h"

#include "gui/GW3DGUICommon.h"

class EntityController : public Geoweb3d::GUI::IGW3DGUIVectorLayerStream
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

	virtual void RotateEntity ( Geoweb3d::GUI::IGW3DGUIVectorLayerStreamResult*, double hdg) = 0; 
	virtual void ChangeIconColor ( Geoweb3d::GUI::IGW3DGUIVectorLayerStreamResult* result) = 0;
    
	//These get called by the SDK when Stream is called on the layer.
    virtual bool OnStream( Geoweb3d::GUI::IGW3DGUIVectorLayerStreamResult *result ); 
    virtual void OnError(/*todo*/ ) ;


    // These are required to be implimented in a IGW3DGUIVectorLayerStream!
    // *NOTE* if [count] returns 0, then no other of APIs to index
    // into selection_set_fids_ will get called.  What this
    // means is if you are streaming a whole layer, its safe to
    // return 0 for everything.

    virtual unsigned long count() const
    {
		return dirty_fids_.size();
    }

    virtual bool next( long *ppVal )
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

    virtual long operator[](unsigned long index)
    {
        return get_AtIndex( index );
    }
    virtual long get_AtIndex( unsigned long index )
    {
		std::set<long>::iterator itr = dirty_fids_.begin();
		std::advance( itr, index );
		return (*itr);
    }

	virtual Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr GetEntityRepresentation() = 0;

protected:
	typedef std::map<unsigned, EntityInfo> EntityDatabaseType;
	EntityDatabaseType entity_database_;
	std::set<long> dirty_fids_;
	std::set<long>::iterator index_itr_;
	CritSectEx  database_thread_protection_;

private:
    //this is an expensive object to create and destory, so keep a scratchpad/reusable object
    Geoweb3d::IGW3DVariant scratchpad_;
    Geoweb3d::IGW3DPropertyCollectionPtr props_scratchpad_;
    Geoweb3d::IGW3DVectorRepresentationWPtr rep_;
};
