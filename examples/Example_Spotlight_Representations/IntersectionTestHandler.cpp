
#include "IntersectionTestHandler.h"

#include "engine/GW3DVector.h"
#include "common/GW3DCommon.h"

#include "engine/IGW3DLineSegmentIntersectionTest.h"
#include "engine/IGW3DLineSegmentIntersectionTestCollection.h"

	IntersectionTestHandler::IntersectionTestHandler(Geoweb3d::IGW3DGeoweb3dSDKPtr& sdk_context)
		: sdk_context_ ( sdk_context )
	{

	}

	void IntersectionTestHandler::PrintLineSegmentCollisions( Geoweb3d::IGW3DLineSegmentIntersectionTestCollection *collection)
	{
		pick_sorted_database_.clear();
		CollisionPrinter prints;
		prints.Print( pick_sorted_database_, collection->get_IntersectionResultsCollection( true ) );
	};

	void IntersectionTestHandler::DoPostDrawWork()
	{
		for(PickSortedDatabase::iterator itr = pick_sorted_database_.begin(); itr != pick_sorted_database_.end(); ++itr)
        {
            Geoweb3d::IGW3DVectorRepresentationWPtr rep_ = itr->first;

            Geoweb3d::IGW3DPropertyCollectionPtr defaultsettings = rep_.lock()->get_PropertyCollection( )->create_Clone();
            const  Geoweb3d::IGW3DDefinitionCollection* repdef = defaultsettings->get_DefinitionCollection();

            int color_red_index = repdef->get_IndexByName("Red");
            int color_green_index = repdef->get_IndexByName("Green");
            int color_blue_index = repdef->get_IndexByName("Blue");

            defaultsettings->put_Property(color_red_index,   1.0 );
            defaultsettings->put_Property(color_green_index, 0.0);
            defaultsettings->put_Property(color_blue_index,  0.0 );

            //only highlight the first/closest fid. (as the data was put in sorted!
            int i = 0;
            //for(unsigned i =0; i < itr->second.size(); ++i)
            {
                rep_.lock()->put_PropertyCollection(itr->second[i].fid,  defaultsettings);
            }
        }

		PrintLineSegmentCollisions( sdk_context_->get_LineSegmentIntersectionTestCollection( true ) );

        if(sdk_context_->get_LineSegmentIntersectionTestCollection ()->count())
            sdk_context_->get_LineSegmentIntersectionTestCollection ()->get_AtIndex(0).lock()->put_Enabled ( false ); //only do a one shot
	}



    CollisionPrinter::CollisionPrinter() 
	{}

	CollisionPrinter::~CollisionPrinter() 
	{}

    void CollisionPrinter::Print( PickSortedDatabase& database, Geoweb3d::IGW3DLineSegmentIntersectionTestResultsCollection *linesegmenttests )
    {
        if(linesegmenttests->count())
        {
            //create GW3DPoint objects once, as they are expensive to have in a loop!
            Geoweb3d::GW3DPoint vert_a;
            Geoweb3d::GW3DPoint vert_b;
            Geoweb3d::GW3DPoint vert_c;

            printf("\n*** Intersection Detected! Linesegment Count:[ %d ]***\n", linesegmenttests->count() );

            //NOTE, OnStream gives the results of all the new collisions, but nothing is stopping you to
            //query the results directly. *However*, calling STREAM is what runs the analysis within the SDK!
            //What this means is, if you are not interested the way to get spatially all the linesegment results,
            //you can just call stream leaving this function empty, and then call get_IntersectionCollection
            //on your linesegment of interest.
            Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr linesegmenttest;

            while(linesegmenttests->next( &linesegmenttest))
            {
                Geoweb3d::IGW3DLineSegmentIntersectionReportCollection *intersectioninfo =	linesegmenttest.lock()->get_IntersectionReportCollection( true );


                Geoweb3d::IGW3DLineSegmentIntersectionReport *intersectionreport;


                printf("\n*** Representation Intersection Report Count:[ %d ]***\n", intersectioninfo->count() );

                while(intersectioninfo->next( &intersectionreport ))
                {
                    printf("Representation Driver[%s] vector layer[%s]\n",
                           intersectionreport->get_VectorRepresentation().lock()->get_Driver().lock()->get_Name(),
                           intersectionreport->get_VectorRepresentation().lock()->get_VectorLayer().lock()->get_Name() );

                    Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection *details_collection =
                        intersectionreport->get_IntersectionDetailCollection( true );

                    Geoweb3d::IGW3DLineSegmentIntersectionDetail *intersection_detail;


                    printf("\n***Count of intersections with the Representation:[ %d ]***\n", details_collection->count() );

                    details_collection->do_Sort( Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection::Ascending );
                    while(details_collection->next( &intersection_detail ))
                    {

                        unsigned object_id =  intersection_detail->get_ObjectID();

                        const Geoweb3d::GW3DPoint * collision_point = intersection_detail->get_IntersectionPoint();
                        double intersectiondistance = intersection_detail->get_IntersectionDistance();

                        //the triangle at this point.  This is the real triangle of the geometry, and is not clipped or modified in any way.
                        intersection_detail->get_TriangleIntersected( vert_a, vert_b, vert_c );

                        //the normal at this point
                        const Geoweb3d::GW3DPoint *normal = intersection_detail->get_NormalOfIntersection( );

                        printf("ObjectID[%d]\n", object_id);
                        printf("Intersection Distance[%f]\n", intersectiondistance);
                        printf("Intersection longitude[%f] latitude[%f] elevation[%f]\n",
                               collision_point->get_X(),collision_point->get_Y(),collision_point->get_Z());

                        printf("Triangle:\nlongitude[%f] latitude[%f] elevation[%f]\nlongitude[%f] latitude[%f] elevation[%f]\nlongitude[%f] latitude[%f] elevation[%f]\n",
                               vert_a.get_X(),vert_a.get_Y(),vert_a.get_Z(),
                               vert_b.get_X(),vert_b.get_Y(),vert_b.get_Z(),
                               vert_c.get_X(),vert_c.get_Y(),vert_c.get_Z());

                        printf("Normal[%f] [%f] [%f]\n",
                               normal->get_X(),normal->get_Y(),normal->get_Z());


                        IntersectionRecord record;
                        record.fid = object_id;
                        record.distance = intersectiondistance;
                        database[intersectionreport->get_VectorRepresentation()].push_back( record );
                    }
                }
            }

            printf("*** Intersection Detected INFO END! ***\n\n");
        }//end if anything intersected
    }