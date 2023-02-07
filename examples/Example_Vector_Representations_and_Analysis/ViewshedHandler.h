
#include "engine/IGW3DGeoweb3dSDK.h"

class ViewshedHandler
{
public:

	ViewshedHandler(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context);

	void ToggleViewshed(Geoweb3d::IGW3DCameraWPtr&);

	void RunViewshedReport();

private:

	friend class MiniEngine;

	/// <summary>	The viewshed analysis. </summary>
	Geoweb3d::IGW3DPanoramicViewshedTestWPtr viewshed_test_;

	/// <summary>	Context for the sdk. </summary>
    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;

};