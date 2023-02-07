#include "engine/IGW3DGeoweb3dSDK.h"

#include "PrePostDrawTasks.h"

class MiniEngine;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Camera move to animation task. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class CameraMoveToAnimationTask : public PrePostDrawTasks
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="query">  	The query. </param>
    /// <param name="pengine">	[in,out] If non-null, the pengine. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CameraMoveToAnimationTask( Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr query, MiniEngine *pengine);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~CameraMoveToAnimationTask();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Resets the query. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="x">	The x coordinate. </param>
    /// <param name="y">	The y coordinate. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void ResetQuery(int x, int y);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPreDraw();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Mouse wheel delta. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="delta">	The delta. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void MouseWheelDelta( float delta );

    virtual void RunPostDraw();

private:

	friend class MiniEngine;

    /// <summary>	The query. </summary>
    Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr query_;
    /// <summary>	The pengine. </summary>
    MiniEngine *pengine_;
    /// <summary>	The mouse wheel delta. </summary>
    float mouse_wheel_delta_;
    /// <summary>	true to delta set. </summary>
    bool delta_set_;
};