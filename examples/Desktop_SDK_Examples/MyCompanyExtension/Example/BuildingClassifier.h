#pragma once

#include "gui/IGW3DGUIVectorLayerStream.h"

#include "core/GW3DGUIInterFace.h"

#include "gui/GW3DGUICommon.h"

#include <map>
#include <set>


class BuildingTexturizer : public Geoweb3d::GUI::IGW3DGUIVectorLayerStream
{
public:

    virtual ~BuildingTexturizer(void);

	static void Stream(Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr buildings);

private:

	BuildingTexturizer(Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr buildings);

	//scratchpad points
	Geoweb3d::GW3DPoint pt1_;
	Geoweb3d::GW3DPoint pt2_;
	
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
		return 0;
    }

    virtual bool next( long *ppVal )
    {
		return true;
    }

    virtual long operator[](unsigned long index)
    {
        return 0;
    }
    virtual long get_AtIndex( unsigned long index )
    {
		return 0;
    }
	virtual void reset() {}

protected:
	Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr buildings_;

};

class BuildingClassifier : public Geoweb3d::GUI::IGW3DGUIVectorLayerStream
{
public:

	struct BuildingColor
	{
		float red_;
		float green_;
		float blue_;

		BuildingColor(float red, float green, float blue)
			: red_(red), green_(green), blue_(blue)
		{}

		BuildingColor()
			: red_(0.0), green_(0.0), blue_(0.0)
		{}
	};

    BuildingClassifier(Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr buildings);
    virtual ~BuildingClassifier(void);

	void AssignNeighborhoodColor (int neighborhood_id, BuildingColor);

	void AlertNeighborhood (int neighborhood_id);

	void SetNeighborhoodAttribute (const char* attrib_heading );
  
	void SetHeightAttribute (const char* attrib_heading );

	void EnableHeightAssignment (bool enable) { do_assign_heights_ = enable; }

	void EnableTextureRandomization (bool enable) { do_randomize_textures_ = enable; }

	void Run();

	void CreateNeighborhoodIcons(Geoweb3d::GUI::IGW3DGUIVectorDriverCollection* vdrivers, Geoweb3d::GUI::IGW3DGUIVectorRepresentationDriverCollection* rdrivers);

private:
    
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
		return 0;
    }

    virtual bool next( long *ppVal )
    {
		return true;
    }

    virtual long operator[](unsigned long index)
    {
        return 0;
    }
    virtual long get_AtIndex( unsigned long index )
    {
		return 0;
    }
	virtual void reset() {}

protected:
	Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr buildings_;
	Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr bldg_layer_;

	//class compare_variants { // simple comparison function
	//   public:
	//	  bool operator()(const Geoweb3d::IGW3DVariant x,const Geoweb3d::IGW3DVariant y) { return (x-y)>0; } // returns x>y
	//};

	//typedef std::map<Geoweb3d::IGW3DVariant, BuildingColor> NeighborhoodColors;
	//NeighborhoodColors color_database_;

	//typedef std::map<Geoweb3d::IGW3DVariant, Geoweb3d::GW3DEnvelope> NeighborhoodExtents;
	//NeighborhoodExtents extents_database_;

	Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr neighborhood_layer_;
	Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr neighborhood_icons_;

	bool do_randomize_textures_;

	bool do_assign_heights_;
	int height_field_index_;

	int neighborhood_field_index_;

	bool fp_;

	std::set<int> dirty_neighborhoods_;


private:

};
