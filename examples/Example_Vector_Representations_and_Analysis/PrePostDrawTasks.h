
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Pre post draw tasks. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct PrePostDrawTasks
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPreDraw() = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the post draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPostDraw() =0;
};