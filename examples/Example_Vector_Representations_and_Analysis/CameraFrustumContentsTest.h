#include "engine/IGW3DGeoweb3dSDK.h"

#include "engine/GW3DFrameAnalysis.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Camera frustum contents test. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct CameraFrustumContentsTest : public Geoweb3d::IGW3DFrustumAnalysisStream
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Default constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CameraFrustumContentsTest();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~CameraFrustumContentsTest();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Report what was in the view frustum last frame. </summary>
    ///
    /// <remarks>	This function will be called per camera, once for every vector representation that 
	///				had at least one feature within the camera's view frustum.  If the representation 
	///				supports reporting of individual features and the representation has this enabled for 
	///				this camera by setting 
	///				IGW3DCameraFrustumContentsConfiguration::put_EnableObjectIDWindowCoordinate 
	///				to true, then details of each feature that intersects the frustum may be queried.
	///				</remarks>
    ///
    /// <param name="camera">	The camera. </param>
    /// <param name="rep">   	The rep. </param>
    /// <param name="result">	[out] if non-null, the result. </param>
    ///
    /// <returns>	true to continue, false to cancel the stream. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool OnStream( Geoweb3d::IGW3DContentsResult *contents, Geoweb3d::IGW3DFeatureFeedbackResultCollection *featurefeedback );

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the error action. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool OnError(/*todo*/ );
};