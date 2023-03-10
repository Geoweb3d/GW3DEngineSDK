#include "engine/IGW3DGeoweb3dSDK.h"

#include <map>
#include <deque>

class MiniEngine;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Intersection record. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IntersectionRecord
{
    /// <summary>	The fid. </summary>
    long fid;
    /// <summary>	The distance. </summary>
    double distance;
};


typedef std::map<Geoweb3d::IGW3DVectorRepresentationWPtr, std::deque<IntersectionRecord> > PickSortedDatabase;


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// /
/// 					CollisionPrinter
/// /.
/// </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class CollisionPrinter //: public Geoweb3d::IGW3DLineSegmentIntersectionStream
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Default constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CollisionPrinter();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~CollisionPrinter();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Prints the given linesegments. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="linesegments">	[in,out] If non-null, the linesegments. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void  Print( PickSortedDatabase&, Geoweb3d::IGW3DLineSegmentIntersectionTestResultsCollection *linesegmenttests );

};

class IntersectionTestHandler
{
public:

	IntersectionTestHandler(Geoweb3d::IGW3DGeoweb3dSDKPtr&);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// /
	/// 					PrintLineSegmentCollisions
	/// /.
	/// </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="collection">	[in,out] If non-null, the collection. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void PrintLineSegmentCollisions( Geoweb3d::IGW3DLineSegmentIntersectionTestCollection *collection);

	void DoPostDrawWork();

private:

	Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;

	//note that we sort the data before going into the deque.
	PickSortedDatabase pick_sorted_database_;

};


