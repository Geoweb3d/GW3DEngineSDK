#include "engine/IGW3DGeoweb3dSDK.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/GW3DRaster.h"
#include "engine/GW3DFrameAnalysis.h"

#include "engine/IGW3DEventStream.h"
#include "engine/IGW3DWindowCallback.h"
#include "engine/IGW3DVectorRepresentationCollection.h"

//interfaces that will be going away.
#include "Geoweb3dCore/GeometryExports.h"
#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/LayerParameters.h"
#include "Geoweb3dcore/ImageryExports.h"

//2D overlay handler
struct Overlay2DHandler_;
class Overlay2DHandler 
	: public Geoweb3d::IGW3DWindowCallback 
	, public Geoweb3d::IGW3DFrustumAnalysisStream
{
	public:

		Overlay2DHandler();
		
		Overlay2DHandler( Geoweb3d::IGW3DGeoweb3dSDKPtr& sdkcontext );

		virtual ~Overlay2DHandler();

		void addRepresentation(  Geoweb3d::IGW3DVectorRepresentationWPtr rep );

		//IGW3DWindowCallback interface
		virtual void OnCreate( ) override;

		virtual void OnDrawBegin( ) override;

		virtual void OnDrawEnd( ) override;

		virtual void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera ) override;

		//IGW3DFrustumAnalysisStream interface
		virtual bool OnStream( Geoweb3d::IGW3DContentsResult *contents, Geoweb3d::IGW3DFeatureFeedbackResultCollection *featurefeedback ) override;

	    virtual bool OnError( ) override;

	private:
		Overlay2DHandler_* m_imp;
		Geoweb3d::IGW3DGeoweb3dSDKPtr m_sdkcontext_;

};
