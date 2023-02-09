#include "stdafx.h"

#include "Engine.h"


Engine::Engine()
{
	this->_Pan = false;
	this->_Rotate = false;
	this->_CameraMode = CameraMode::Follow;
	this->_FollowIsDetatched = false;
	this->_CockpitView = false;
	this->_ChaseDistance = 40.0;
}

Engine::Engine(const IGW3DGeoweb3dSDKPtr & context)
{
	this->_SdkContext = context;
	this->_Pan = false;
	this->_Rotate = false;
	this->_CameraMode = CameraMode::Follow;
	this->_FollowIsDetatched = false;
	this->_CockpitView = false;
	this->_ChaseDistance = 40.0;
}

Engine & Engine::operator=(const IGW3DGeoweb3dSDKPtr & context)
{
	SetSdkContext(context);

	this->_Pan = false;
	this->_Rotate = false;
	this->_CameraMode = CameraMode::Follow;
	this->_FollowIsDetatched = false;
	this->_CockpitView = false;
	this->_ChaseDistance = 40.0;

	return *this;
}

Engine::~Engine()
{
}

// Set the application Geoweb3d SDK context used for most operations
bool Engine::SetSdkContext(const IGW3DGeoweb3dSDKPtr & context)
{
	if (context) {
		this->_SdkContext = context;
		return true;
	}

	return false;
}

// Primary application frame render path
bool Engine::Render()
{
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	double offset = 0.0;

	// Start our animation update
	// TODO : Replace this with an actual time-based timer
	if (_Timer >= 1.0)
		_Timer = 0.0;
	else
		_Timer += 0.0002;

	// Verify that our data has been loaded and is valid
	if (!_Data.GetDataSource().expired()) {
		// Find the geometry that represents the model
		auto modelLayer = _Data.GetDataSource().lock()->get_VectorLayerCollection()->get_ByName("Model");

		// Make sure that layer is valid
		if (modelLayer.expired()) {
			OutputDebugString(L"Model or path expired.");
			return false;
		}
		else {
			// Calculate where in the path we're supposed to be at a given time
			auto index = static_cast<unsigned int>(_Timer / 1.0 * static_cast<double>(_Data.GetPathLength()));
			VertexInformation vertex = _Data.GetVertex(index);

			GW3DPoint position;

			position.put_X(vertex.Longitude);
			position.put_Y(vertex.Latitude);
			position.put_Z(vertex.Elevation);

			double bearing = vertex.EffectiveHeading;
			double pitch = vertex.EffectivePitch;

			// Update the model location
			_ModelController.UpdateModel(position, bearing, modelLayer);

			// Update the orientation of the model which is done via its representation rather than the actual geometry
			auto modelRepresentationCount = modelLayer.lock()->get_RepresentationLayerCollection()->count();
			
			IGW3DVectorRepresentationWPtr modelRepresentation;

			// Since we can have multiple representations of our model layer, we need to
			// pick out an appropriate one to bind to.  Since the actual model is our primary
			// representation, let's use it.  
			// (Note: Technically we can have multiple models representing a given layer... this just looks
			// for the first representation of type "Model")
			for (unsigned long i = 0; i < modelRepresentationCount; i++) {
				modelRepresentation = modelLayer.lock()->get_RepresentationLayerCollection()->get_AtIndex(i);
				if (std::string(modelRepresentation.lock()->get_Driver().lock()->get_Name()) == "Model")
					break;
			}

			// Make sure it's valid
			if (modelRepresentation.expired()) {
				OutputDebugString(L"Model Representation not valid.");
				return false;
			}

			// Update the cameras
			if (_FollowCamera.lock()->get_Enabled()) {
				auto camera = _FollowCamera.lock()->get_CameraController();

				// Is the camera following the model or not?
				if (_FollowIsDetatched) {
					// Make sure the model is being shown since we're not "inside" it
					modelRepresentation.lock()->put_Enabled(true);
				}
				else {
					// Place the camera at the same point as the model, elevated to height of the cockpit
					position.put_Z(position.get_Z() + 3.0);
					SetCameraPosition(_FollowCamera, position);

					// Determine where we want the camera to be
					if (_CockpitView) {
						// Cockpit view, inside the model
						offset = -0.01;
						// Turn off the model representation since we're inside its geometry
						modelRepresentation.lock()->put_Enabled(false);

						// Make sure we're pointing the camera in the right direction
						SetCameraOrientation(_FollowCamera, 0.0f, 0.0f /*static_cast<float>(pitch)*/, static_cast<float>(bearing));
					}
					else {
						// Trail camera, outside the model so we can see it
						modelRepresentation.lock()->put_Enabled(true);

						// Point the camera in the direction of our next heading
						// Also pitch "up" a bit so we have an over-the-shoulder view
						SetCameraOrientation(_FollowCamera, 0.0f, 20.0f, static_cast<float>(bearing));

						// Chase view, behind the model
						offset = -_ChaseDistance;
						camera->move(offset, 0.0, true);

						// Reorient the camera to look at the target
						camera->look_At(&position);
					}
				}
			}

			// Auto-tracking map camera
			//if (_MapCamera.lock()->get_Enabled()) {
			//	SetCameraPosition(_MapCamera, position);
			//	SetCameraOrientation(_MapCamera, 0.0f, 90.0f, 0.0f);
			//}
		}
	}

	// Have the SDK draw the current frame
	if (_SdkContext) {
		return _SdkContext->draw(_Window) == GW3D_sOk;
	}

	return false;
}

// Resize the application render window
bool Engine::ResizeWindow(int x, int y, unsigned int cx, unsigned int cy)
{
	if (!_Window.expired()) {
		_Window.lock()->put_WindowSize(x, y, cx, cy);
		return true;
	}

	return false;
}

// Position the given camera
bool Engine::SetCameraPosition(const IGW3DCameraWPtr& camera, double longitude, double latitude, double elevation)
{
	// Check to see if the current camera is valid and then relocate it somewhere desirable
	if (!camera.expired()) {
		camera.lock()->get_CameraController()->put_Location(longitude, latitude);
		camera.lock()->get_CameraController()->put_Elevation(elevation);
		return true;
	}

	return false;
}

// Overload for SetCameraPosition
bool Engine::SetCameraPosition(const IGW3DCameraWPtr & camera, const GW3DPoint & point)
{
	return this->SetCameraPosition(camera, point.get_X(), point.get_Y(), point.get_Z());
}

// Orient the camera
bool Engine::SetCameraOrientation(const IGW3DCameraWPtr& camera, float roll, float pitch, float yaw)
{
	// Check is the requested camera is valid and then set it's orientation
	if (!camera.expired()) {
		camera.lock()->get_CameraController()->put_Rotation(yaw, pitch, roll);
		return true;
	}

	return false;
}

// Create required application cameras
bool Engine::CreateSdkCameras()
{
	// Verify that we have a valid window available to create
	// cameras into.
	if (_Window.expired()) {
		return false;
	}

	// Create a pair of cameras, a main camera and a smaller inset camera.
	// The naming convention helps determine the Z-Order of the cameras.
	// They are sorted in alphabetical order. (Technically, ascii order)
	_FollowCamera = CreateCamera("1");
	_MapCamera = CreateCamera("2");

	// We're creating pointers to the same cameras to facilitate easy
	// delineation of which camera occupies the screen and which is inset.
	_PrimaryCamera = _FollowCamera;
	_SecondaryCamera = _MapCamera;

	SetCameraExtents();

	return true;
}

// Create the application window
bool Engine::CreateSdkWindow(const std::string & name)
{
	// Ensure the SDK context is valid, then create a Geoweb3d window.
	// The name parameter is the Title Bar text
	if (this->_SdkContext) {
		_Window = _SdkContext->get_WindowCollection()->create_3DWindow(name.c_str(), GW3D_OVERLAPPED, 10, 10, 1280, 768, nullptr, IGW3DStereoDriverWPtr(), this);
		return true;
	}

	return false;
}

// Load a data set, create geometry and representations
bool Engine::LoadFlightData(const std::string & filename)
{
	if (_SdkContext) {
		Geoweb3d::GW3DResult res;
		//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
		//having to guess which data source driver is needed to open up a particular dataset.
		auto vectorSource = _SdkContext->get_VectorDriverCollection()->auto_Open(filename.c_str(), res);

		if (!vectorSource.expired()) {
			// Pull some details off the layer and set up its representation
			auto layers = vectorSource.lock()->get_VectorLayerCollection(true);

			// Iterate through the loaded layers for each flight path 
			// (Technically there can be multiple layers we're generating from)
			IGW3DVectorLayerWPtr layer;
			while (layers->next(&layer)) {
				std::cout << "Layer Name : " << layer.lock()->get_Name() << ", Type : " << layer.lock()->get_Type() << std::endl;

				// Make sure we're the right kind of data
				if (layer.lock()->get_GeometryType() != gtLINE_STRING_25D) {
					continue;
				}

				// Prepare a custom in-memory data set for us to work with.
				auto dataSourceDriver = _SdkContext->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");
				_Data.SetDataSource(dataSourceDriver.lock()->get_VectorDataSourceCollection()->create("FlightFeatures"));
				_Data.SetPostSpacing(100.0);
				_Data.SetPathWidth(10.0);
				_Data.SetSectionLength(5.0);

				// Stream the original path so we can get the data that we need out of it
				layer.lock()->Stream(&_Data);
				std::cout << "Posts added : " << _Data.GetPostCount() << std::endl;
				std::cout << "Ribbon polys added : " << _Data.GetPathLength() << std::endl;

				auto createdLayers = _Data.GetDataSource().lock()->get_VectorLayerCollection();

				// --- (Temporary) Represent the actual flight path

				auto pathLayer = createdLayers->get_ByName("Path");
				auto pathRepresentationDriver = _SdkContext->get_VectorRepresentationDriverCollection()->get_Driver("ColoredLine");

				if (pathRepresentationDriver.expired() || !Geoweb3d::Succeeded(pathRepresentationDriver.lock()->get_CapabilityToRepresent(pathLayer))) {
					std::cout << "Layer can not be represented" << std::endl;
					return false;
				}

				Geoweb3d::Vector::RepresentationLayerCreationParameter pathParams;
				auto dataRepresentation = pathRepresentationDriver.lock()->get_RepresentationLayerCollection()->create(pathLayer, pathParams);
				auto dataProps = dataRepresentation.lock()->get_PropertyCollection()->create_Clone();
				dataProps->put_Property(Vector::ColoredLinesProperties::ALPHA, 0.35);
				dataProps->put_Property(Vector::ColoredLinesProperties::BLUE, 0.3);
				dataProps->put_Property(Vector::ColoredLinesProperties::GREEN, 0.3);
				dataProps->put_Property(Vector::ColoredLinesProperties::RED, 0.3);
				dataProps->put_Property(Vector::ColoredLinesProperties::LINE_WIDTH, 0.25);
				dataProps->put_Property(Vector::ColoredLinesProperties::TRANSLATION_Z_OFFSET, 0.0);
				dataProps->put_Property(Vector::ColoredLinesProperties::TRANSLATION_Z_OFFSET_MODE, Vector::ABSOLUTE_MODE);
				dataRepresentation.lock()->put_PropertyCollection(dataProps);

				_Envelope.merge(layer.lock()->get_Envelope());

				// --- Represent the flight path ribbon

				auto ribbonLayer = createdLayers->get_ByName("Ribbon");
				auto solidRibbonDriver = _SdkContext->get_VectorRepresentationDriverCollection()->get_Driver("ExtrudedPolygon");
				ribbonLayer.lock()->put_GeometryEditableMode(false);

				if (solidRibbonDriver.expired() || !Geoweb3d::Succeeded(solidRibbonDriver.lock()->get_CapabilityToRepresent(ribbonLayer))) {
					std::cout << "Layer can not be represented" << std::endl;
					return false;
				}

				Geoweb3d::Vector::RepresentationLayerCreationParameter solidRibbonParams;
				auto solidPathRepresentation = solidRibbonDriver.lock()->get_RepresentationLayerCollection()->create(ribbonLayer, solidRibbonParams);
				auto solidPathProps = solidPathRepresentation.lock()->get_PropertyCollection()->create_Clone();
				solidPathProps->put_Property(Vector::ExtrudedPolygonProperties::TEXTURE_MODE_ENABLE, false);
				solidPathProps->put_Property(Vector::ExtrudedPolygonProperties::RED, 0.0);
				solidPathProps->put_Property(Vector::ExtrudedPolygonProperties::GREEN, 0.45);
				solidPathProps->put_Property(Vector::ExtrudedPolygonProperties::BLUE, 1.0);
				solidPathProps->put_Property(Vector::ExtrudedPolygonProperties::ALPHA, 0.35);
				solidPathProps->put_Property(Vector::ExtrudedPolygonProperties::EXTRUSION_HEIGHT, 0.0);
				solidPathProps->put_Property(Vector::ExtrudedPolygonProperties::TRANSLATION_Z_OFFSET, -7.0);
				solidPathProps->put_Property(Vector::ExtrudedPolygonProperties::TRANSLATION_Z_OFFSET_MODE, Vector::ABSOLUTE_MODE);		
				solidPathProps->put_Property(Vector::ExtrudedPolygonProperties::FILL_MODE, Vector::ExtrudedPolygonProperties::FillMode::GW3DSOLID);
				solidPathRepresentation.lock()->put_PropertyCollection(solidPathProps);

				auto meshRibbonDriver = _SdkContext->get_VectorRepresentationDriverCollection()->get_Driver("ColoredLine");
				ribbonLayer.lock()->put_GeometryEditableMode(false);

				if (meshRibbonDriver.expired() || !Geoweb3d::Succeeded(meshRibbonDriver.lock()->get_CapabilityToRepresent(ribbonLayer))) {
					std::cout << "Layer can not be represented" << std::endl;
					return false;
				}

				Geoweb3d::Vector::RepresentationLayerCreationParameter meshRibbonParams;
				auto meshPathRepresentation = meshRibbonDriver.lock()->get_RepresentationLayerCollection()->create(ribbonLayer, meshRibbonParams);
				auto meshPathProps = meshPathRepresentation.lock()->get_PropertyCollection()->create_Clone();
				meshPathProps->put_Property(Vector::ColoredLinesProperties::RED, 0.0);
				meshPathProps->put_Property(Vector::ColoredLinesProperties::GREEN, 0.85);
				meshPathProps->put_Property(Vector::ColoredLinesProperties::BLUE, 0.0);
				meshPathProps->put_Property(Vector::ColoredLinesProperties::ALPHA, 0.85);
				meshPathProps->put_Property(Vector::ColoredLinesProperties::TRANSLATION_Z_OFFSET, -7.0);
				meshPathProps->put_Property(Vector::ColoredLinesProperties::TRANSLATION_Z_OFFSET_MODE, Vector::ABSOLUTE_MODE);
				meshPathRepresentation.lock()->put_PropertyCollection(meshPathProps);

				meshPathRepresentation.lock()->put_Enabled(false);

				_Envelope.merge(ribbonLayer.lock()->get_Envelope());

				// --- Represent the line spires on the view

				Geoweb3d::Vector::RepresentationLayerCreationParameter postParams;

				auto spireLayer = createdLayers->get_ByName("Posts");
				auto spireDriver = _SdkContext->get_VectorRepresentationDriverCollection()->get_Driver("Cylinder");

				if (spireDriver.expired() || !Geoweb3d::Succeeded(spireDriver.lock()->get_CapabilityToRepresent(spireLayer))) {
					std::cout << "Layer can not be represented" << std::endl;
					return false;
				}

				auto spireRepresentation = spireDriver.lock()->get_RepresentationLayerCollection()->create(spireLayer, postParams);
				auto spireProps = spireRepresentation.lock()->get_PropertyCollection()->create_Clone();
				spireProps->put_Property(Vector::CylinderProperties::HEIGHT, -_Data.GetMaxElevation());	// Negative because we're going from +Z to 0
				spireProps->put_Property(Vector::CylinderProperties::RADIUS, 0.1);
				spireProps->put_Property(Vector::CylinderProperties::RED, 0.0);
				spireProps->put_Property(Vector::CylinderProperties::GREEN, 0.0);
				spireProps->put_Property(Vector::CylinderProperties::BLUE, 0.0);
				spireProps->put_Property(Vector::CylinderProperties::ALPHA, 0.85);
				spireProps->put_Property(Vector::CylinderProperties::TRANSLATION_Z_OFFSET, -7.0);
				spireProps->put_Property(Vector::CylinderProperties::TRANSLATION_Z_OFFSET_MODE, Vector::ABSOLUTE_MODE);
				spireRepresentation.lock()->put_PropertyCollection(spireProps);

				_Envelope.merge(spireLayer.lock()->get_Envelope());

				// --- Represent the vehicle model

				auto modelLayer = createdLayers->get_ByName("Model");
				auto modelDriver = _SdkContext->get_VectorRepresentationDriverCollection()->get_Driver("Model");

				if (modelDriver.expired() || !Geoweb3d::Succeeded(modelDriver.lock()->get_CapabilityToRepresent(modelLayer))) {
					std::cout << "Layer can not be represented" << std::endl;
					return false;
				}

				Geoweb3d::Vector::RepresentationLayerCreationParameter modelParams;
				auto modelRepresentation = modelDriver.lock()->get_RepresentationLayerCollection()->create(modelLayer, modelParams);
				auto modelProps = modelRepresentation.lock()->get_PropertyCollection()->create_Clone();
				modelProps->put_Property(Vector::ModelProperties::URL, _ModelPath.c_str());
				modelProps->put_Property(Vector::ModelProperties::TRANSLATION_Z_OFFSET, 0.0);
				modelProps->put_Property(Vector::ModelProperties::TRANSLATION_Z_OFFSET_MODE, Vector::ABSOLUTE_MODE);
				modelProps->put_Property(Vector::ModelProperties::HEADING, 0.0);
				modelRepresentation.lock()->put_PropertyCollection(modelProps);

				// The following lines enable the Frustum Analysis feature of the SDK
				// Which specifically we need in order to obtain the 2D window coordinates of the model
				// to do certain things with such as render 2D text attached to said model.
				modelRepresentation.lock()->get_FrameAnalysisContentsConfiguration()->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->put_Enabled(true);
				modelRepresentation.lock()->get_FrameAnalysisContentsConfiguration()->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->get_FeatureID2DPixelLocationConfiguration()->put_Enabled(true);

				//Also create a billboard representation for the overview camera

				auto billboardDriver = _SdkContext->get_VectorRepresentationDriverCollection()->get_Driver("BillBoard");
				Geoweb3d::IGW3DPropertyCollectionPtr billboard_default_properties = billboardDriver.lock()->get_PropertyCollection()->create_Clone();
				billboard_default_properties->put_Property(Vector::BillboardProperties::TEXTURE_PALETTE_INDEX, 0);
				billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("TRANSLATION_Z_OFFSET"), 0.0);
				billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("RED"), 1.0);
				billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("GREEN"), 1.0);
				billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("BLUE"), 1.0);
				billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("ALPHA"), 0.25);
				billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("TEXTURE_TO_COLOR_BLEND"), 1.0);
				billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("TEXTURE_PALETTE_INDEX"), 0);

				billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("SIZE_X"), 40.0);
				billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("SIZE_Y"), 40.0);

				Geoweb3d::Vector::RepresentationLayerCreationParameter billboardParams;
				billboardParams.page_level = 8;
				billboardParams.representation_default_parameters = billboard_default_properties;
				
				auto image_collection = _SdkContext->get_SceneGraphContext()->create_ImageCollection();
				image_collection->create("../examples/media/test_images/star.png");
				
				billboardParams.finalization_token = image_collection->create_FinalizeToken();

				auto billboardRepresentation = billboardDriver.lock()->get_RepresentationLayerCollection()->create(modelLayer, billboardParams);
				//Geoweb3d::IGW3DClampRadius* billboard_clamp_radius = billboardRepresentation.lock()->get_ClampRadiusFilter();
				//billboard_clamp_radius->put_Enabled(true);
				//billboard_clamp_radius->put_MaxRadius(100000000.0);
				//billboard_clamp_radius->put_MinRadius(1000.0);

				_Envelope.merge(modelLayer.lock()->get_Envelope());
			}
		}

		auto startLocation = _Data.GetInitialLocation();
		SetCameraPosition(_MapCamera, startLocation.get_X(), startLocation.get_Y(), 5000.0f);
		SetCameraOrientation(_MapCamera, 0.0f, 90.0f, 0.0f);
		SetCameraPosition(_FollowCamera, startLocation.get_X(), startLocation.get_Y(), startLocation.get_Z());
		SetCameraOrientation(_FollowCamera, 0.0f, 0.0f, (float)_Data.GetInitialHeading());
		return true;
	}

	return false;
}

// Load raster elevation data
bool Engine::LoadElevationData(const std::string & filename)
{
	// Check the validity of the context
	if (_SdkContext) {
		// (Auto)Open the given file
		Geoweb3d::GW3DResult res;
		//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
		//having to guess which data source driver is needed to open up a particular dataset.
		auto rasterSource = _SdkContext->get_RasterDriverCollection()->auto_Open(filename.c_str(), res);

		// If the file was proper elevation data and loaded properly create the appropriate raster representation
		if (!rasterSource.expired()) 
		{
			Geoweb3d::IGW3DRasterLayerCollection* layer_collection = rasterSource.lock()->get_RasterLayerCollection();
			Geoweb3d::IGW3DRasterLayerWPtr layer = layer_collection->get_AtIndex(0);

			if (!layer.expired())
			{
				Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
				params.page_level = 0;
				params.priority = 0;
				params.representation_layer_activity = true;
				auto representation = _SdkContext->get_RasterLayerRepresentationDriverCollection()->get_Driver("Elevation").lock()->get_RepresentationLayerCollection()->create(layer, params);
				_Elevation.push_back(representation);
				_Envelope.merge(layer.lock()->get_Envelope());
				return true;
			}
		}
	}

	return false;
}

// Load raster imagery data
bool Engine::LoadImageryData(const std::string & filename)
{
	// Check the validity of the context
	if (_SdkContext) {
		// (Auto)Open the given file
		Geoweb3d::GW3DResult res;
		//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
		//having to guess which data source driver is needed to open up a particular dataset.
		auto rasterSource = _SdkContext->get_RasterDriverCollection()->auto_Open(filename.c_str(), res);

		// If the file was proper elevation data and loaded properly create the appropriate raster representation
		if (!rasterSource.expired()) 
		{
			Geoweb3d::IGW3DRasterLayerCollection* layer_collection = rasterSource.lock()->get_RasterLayerCollection();
			Geoweb3d::IGW3DRasterLayerWPtr layer = layer_collection->get_AtIndex(0);

			if (!layer.expired())
			{
				Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
				params.page_level = 0;
				params.priority = 0;
				params.representation_layer_activity = true;
				auto representation = _SdkContext->get_RasterLayerRepresentationDriverCollection()->get_Driver("Imagery").lock()->get_RepresentationLayerCollection()->create(layer, params);
				_Imagery.push_back(representation);
				_Envelope.merge(layer.lock()->get_Envelope());
				return true;
			}
		}
	}

	return false;
}

// Set the model data file path
bool Engine::LoadModelData(const std::string & filename)
{
	_ModelPath = filename;
	return true;
}

// Export the flight path data
bool Engine::ExportData(const std::string & filename, const std::string & type)
{
	// Export is not implemented

	return false;
}

// Create a named camera
IGW3DCameraWPtr Engine::CreateCamera(const std::string & name)
{
	// Make sure the application window is created, then create a new camera with a given name
	if (!_Window.expired()) {
		return _Window.lock()->get_CameraCollection()->create(name.c_str());		
	}

	return IGW3DCameraWPtr();
}

// Resize the primary and secondary cameras
bool Engine::SetCameraExtents()
{
	// Make sure the cameras are valid
	if (_PrimaryCamera.expired() || _SecondaryCamera.expired())
		return false;

	// Resize the primary camera to occupy the entire application window
	_PrimaryCamera.lock()->put_NormalizeWindowCoordinates(0.0f, 1.0f, 0.0f, 1.0f);
	// Resize the secondary camera to the upper right corner of the application window with a small padding
	_SecondaryCamera.lock()->put_NormalizeWindowCoordinates(0.75f, 0.975f, 0.025f, 0.25f);

	// Ensure both cameras are enabled
	_MapCamera.lock()->put_Enabled(true);
	_FollowCamera.lock()->put_Enabled(true);

	return true;
}

void Engine::OnCreate()
{
}

void Engine::OnDrawBegin()
{
}

void Engine::OnDrawEnd()
{
}

// Handle 2D drawing on a given camera
void Engine::OnDraw2D(const IGW3DCameraWPtr & camera)
{
}

// Supplemental OnDraw2D overload to be used with the 2D draw context
void Engine::OnDraw2D(const IGW3DCameraWPtr & camera, IGW3D2DDrawContext * drawcontext)
{
	int x = 0;
	int y = 0;
	int z = 0;
	int wx = 0;
	int wy = 0;
	unsigned int cx = 0;
	unsigned int cy = 0;

	// Frustum Analysis object used to determine the pixel position of
	// the model
	ModelLabelController labels;

	// Create a font to use with 2D drawing.
	// TODO : This should probably be created once in order to save overhead.
	auto font = IGW3DFont::create("Arial", 16, IGW3DFont::Style::BOLD, false);
	auto smallerFont = IGW3DFont::create("Arial", 12, IGW3DFont::Style::NORMAL, false);
	auto smallestFont = IGW3DFont::create("Arial", 10, IGW3DFont::Style::NORMAL, false);

	// Get the current camera's window coordinates so we can do a bit of math
	// for things like centering.
	camera.lock()->get_Window().lock()->get_WindowSize(wx, wy, cx, cy);

	// Stream the vector representations to the Frustum Analysis object
	_SdkContext->get_VectorRepresentationDriverCollection()->Stream(&labels);

	// Pull the determined pixel coordinates off the analysis object
	// and draw our label
	labels.GetWindowPosition(x, y);
	drawcontext->draw_Text(font, x - (cx / 20), y - (cy / 7), 1.0f, 1.0f, 1.0f, 1.0f, "Flight Label");

	// Ensure we're writing the following data only to the primary camera, not the secondary (inset) camera
	if (camera == _PrimaryCamera) {
		// Output some debugging information
		GW3DPoint currentPOI;
		double currentBearing;
		_ModelController.GetCurrentLocation(currentPOI, currentBearing);

		// Set up our output string.
		std::stringstream text;
		text << std::fixed << std::setprecision(7) << "Lat: " << currentPOI.get_Y() << "  Lon: " << currentPOI.get_X() << std::endl;
		text << std::fixed << std::setprecision(2) << "Altitude: " << currentPOI.get_Z() << std::endl;
		text << std::fixed << std::setprecision(2) << "Heading: " << ((currentBearing < 0.0) ? currentBearing + 360.0 : currentBearing) << std::endl;

		drawcontext->draw_Text(smallerFont, 10, 10, 1.0, 1.0, 1.0, 1.0, text.str().c_str());

		text.clear();
		text.str("");

		text << "F1: Toggle Cameras" << std::endl;
		text << "F2: Toggle Cockpit View" << std::endl;
		text << "F3: Toggle Detached View" << std::endl << std::endl;
		text << "F5: Toggle Flight Path On/Off" << std::endl;
		text << "F6: Toggle Flight Path Posts On/Off" << std::endl;
		text << "F7: Toggle GIS Imagery On/Off" << std::endl;
		text << "F8: Toggle GIS Elevation Data On/Off" << std::endl;
		text << "F9: Toggle Path Representation Type" << std::endl;

		drawcontext->draw_Text(smallestFont, 10, cy - (cy / 4), 1.0, 1.0, 1.0, 1.0, text.str().c_str());

		CreatePIPCameraBackground(camera);
	}
}

// Handle application events (Windows style event handling)
void Engine::ProcessEvent(const WindowEvent & event)
{
	// Handle input events
	switch (event.Type) {
	case WindowEvent::MouseButtonDBLClick:
	{
		break;
	}
	case WindowEvent::MouseButtonPressed:
	{
		switch (event.MouseButton.button)
		{
		case Geoweb3d::Mouse::Left:
		{
			// LMB equates to panning the camera
			_Pan = true;
			_Rotate = false;

			auto currentCamera = _Window.lock()->get_TopCamera(event.MouseButton.x, event.MouseButton.y);

			if (currentCamera.expired())
				return;

			currentCamera.lock()->get_CameraController()->pan_Begin(event.MouseButton.x, event.MouseButton.y);

			break;
		}
		case Geoweb3d::Mouse::Right:
		{
			// RMB equates to rotating the camera
			_Pan = false;
			_Rotate = true;
			break;
		}
		case Geoweb3d::Mouse::Middle:
		default:
			break;
		}

		_StartX = event.MouseButton.x;
		_StartY = event.MouseButton.y;

		break;
	}
	case WindowEvent::MouseButtonReleased:
	{
		switch (event.MouseButton.button)
		{
		case Geoweb3d::Mouse::Left:
		{
			_Pan = false;
			break;
		}
		case Geoweb3d::Mouse::Right:
		{
			_Rotate = false;
			break;
		}
		case Geoweb3d::Mouse::Middle:
		default:
			break;
		}

		_StartX = event.MouseButton.x;
		_StartY = event.MouseButton.y;

		break;
	}
	case WindowEvent::MouseMoved:
	{
		double elevation = 0.0;
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		IGW3DPositionOrientation::AltitudeMode mode;

		auto currentCamera = _Window.lock()->get_TopCamera(event.MouseMove.x, event.MouseMove.y);

		// Make sure we're in a mode on a valid camera that can be manipulated
		if (currentCamera.expired() || currentCamera == _FollowCamera && !_FollowIsDetatched)
			return;

		// Get the current camera location/orientation
		auto cameraController = currentCamera.lock()->get_CameraController();
		auto cameraLocation = cameraController->get_Location();
		cameraController->get_Rotation(yaw, pitch, roll);
		cameraController->get_Elevation(elevation, mode);

		cameraController->put_EnableOverride(true);

		// If we're panning, move the location of the camera
		if (_Pan) {
			cameraController->pan_ToWindowCoordinate(event.MouseMove.x, event.MouseMove.y);
		}

		// If we're rotating, adjust the orientation of the camera
		if (_Rotate) {
			currentCamera.lock()->get_CameraController()->rotate_ByWindowCoordinateDelta(_StartX, _StartY, event.MouseMove.x, event.MouseMove.y, 0.5, 0.5);
		}

		cameraController->put_Elevation(elevation, mode);

		_StartX = event.MouseMove.x;
		_StartY = event.MouseMove.y;

		break;
	}
	case WindowEvent::MouseWheelMoved:
	{
		// Scroll wheel zoom
		double elevation = 0.0;
		IGW3DPositionOrientation::AltitudeMode mode;

		// Get the active camera under the mouse cursor
		auto currentCamera = _Window.lock()->get_TopCamera(event.MouseWheel.x, event.MouseWheel.y);

		// Make sure the camera is valid and can be manipulated
		if (currentCamera.expired() || currentCamera == _FollowCamera && !_FollowIsDetatched) {
			// If we're in follow mode allow the user to slide the camera toward and away from the model
			_ChaseDistance += event.MouseWheel.delta;
			if (_ChaseDistance <= 2.0)
				_ChaseDistance = 2.0;
		}
		else {
			// Otherwise the mouse wheel adjust the elevation of the current camera
			// in logarithmic increments
			auto cameraController = currentCamera.lock()->get_CameraController();
			cameraController->get_Elevation(elevation, mode);
			elevation += event.MouseWheel.delta * elevation * -0.1;
			cameraController->put_Elevation(elevation, mode);
		}

		break;
	}
	case WindowEvent::KeyPressed:
	{
		// Keyboard handlers, most are self explanatory
		auto currentCamera = _Window.lock()->get_TopCamera(_StartY, _StartX);

		switch (event.Key.code)
		{
		case Geoweb3d::Key::Up:
			if (currentCamera.expired() || currentCamera == _FollowCamera && !_FollowIsDetatched)
				return;
			currentCamera.lock()->get_CameraController()->move(10.0, 0.0, false);
			break;
		case Geoweb3d::Key::Down:
			if (currentCamera.expired() || currentCamera == _FollowCamera && !_FollowIsDetatched)
				return;
			currentCamera.lock()->get_CameraController()->move(10.0, 180.0, false);
			break;
		case Geoweb3d::Key::Left:
			if (currentCamera.expired() || currentCamera == _FollowCamera && !_FollowIsDetatched)
				return;
			currentCamera.lock()->get_CameraController()->move(10.0, 270.0, false);
			break;
		case Geoweb3d::Key::Right:
			if (currentCamera.expired() || currentCamera == _FollowCamera && !_FollowIsDetatched)
				return;
			currentCamera.lock()->get_CameraController()->move(10.0, 90.0, false);
			break;
		case Geoweb3d::Key::W:
			if (currentCamera.expired() || currentCamera == _FollowCamera && !_FollowIsDetatched)
				return;
			currentCamera.lock()->get_CameraController()->put_Pitch(_MapCamera.lock()->get_CameraController()->get_Pitch() + 1.0f);
			break;
		case Geoweb3d::Key::S:
			if (currentCamera.expired() || currentCamera == _FollowCamera && !_FollowIsDetatched)
				return;
			currentCamera.lock()->get_CameraController()->put_Pitch(_MapCamera.lock()->get_CameraController()->get_Pitch() - 1.0f);
			break;
		case Geoweb3d::Key::D:
			if (currentCamera.expired() || currentCamera == _FollowCamera && !_FollowIsDetatched)
				return;
			currentCamera.lock()->get_CameraController()->put_Heading(_MapCamera.lock()->get_CameraController()->get_Heading() + 1.0f);
			break;
		case Geoweb3d::Key::A:
			if (currentCamera.expired() || currentCamera == _FollowCamera && !_FollowIsDetatched)
				return;
			currentCamera.lock()->get_CameraController()->put_Heading(_MapCamera.lock()->get_CameraController()->get_Heading() - 1.0f);
			break;
		case Geoweb3d::Key::Q:
			if (currentCamera.expired() || currentCamera == _FollowCamera && !_FollowIsDetatched)
				return;
			currentCamera.lock()->get_CameraController()->put_Roll(_MapCamera.lock()->get_CameraController()->get_Roll() + 1.0f);
			break;
		case Geoweb3d::Key::E:
			if (currentCamera.expired() || currentCamera == _FollowCamera && !_FollowIsDetatched)
				return;
			currentCamera.lock()->get_CameraController()->put_Roll(_MapCamera.lock()->get_CameraController()->get_Roll() - 1.0f);
			break;
		case Geoweb3d::Key::F1:
		{
			double elevation = 0.0;
			float heading = 0.0;
			float pitch = 0.0;
			float roll = 0.0;
			const GW3DPoint * pLocation = nullptr;
			double lat, lon;
			IGW3DPositionOrientation::AltitudeMode mode = IGW3DPositionOrientation::AltitudeMode::Absolute;

			// Store the map camera's existing position
			_MapCamera.lock()->get_CameraController()->get_Elevation(elevation, mode);
			pLocation = _MapCamera.lock()->get_CameraController()->get_Location();
			_MapCamera.lock()->get_CameraController()->get_Rotation(heading, pitch, roll);

			lat = pLocation->get_Y();
			lon = pLocation->get_X();

			// Close the existing cameras
			_Window.lock()->get_CameraCollection()->close(_FollowCamera);
			_Window.lock()->get_CameraCollection()->close(_MapCamera);

			// Toggle between main camera modes
			_CameraMode = (_CameraMode != CameraMode::Map) ? _CameraMode = CameraMode::Map : _CameraMode = CameraMode::Follow;

			// Create new cameras in the proper order
			switch (_CameraMode) {
			case CameraMode::Map:
				_PrimaryCamera = _MapCamera = CreateCamera("1");
				_SecondaryCamera = _FollowCamera = CreateCamera("2");
				break;
			case CameraMode::Follow:
			default:
				_PrimaryCamera = _FollowCamera = CreateCamera("1");
				_SecondaryCamera = _MapCamera = CreateCamera("2");
				break;
			}

			SetCameraExtents();

			// Restore the map camera's position
			_MapCamera.lock()->get_CameraController()->put_Location(lon, lat);
			_MapCamera.lock()->get_CameraController()->put_Elevation(elevation, mode);
			_MapCamera.lock()->get_CameraController()->put_Rotation(heading, pitch, roll);
			break;
		}
		case Geoweb3d::Key::F2:
			// Toggle cockpit view
			_CockpitView = !_CockpitView;
			break;
		case Geoweb3d::Key::F3:
			// Toggle follow mode
			_FollowIsDetatched = !_FollowIsDetatched;
			break;
		case Geoweb3d::Key::F5:
		{
			// Enable/Disable the visual representation of the path ribbon
			auto layer = _Data.GetDataSource().lock()->get_VectorLayerCollection()->get_ByName("Ribbon");
			auto representation = layer.lock()->get_RepresentationLayerCollection()->get_AtIndex(0);
			representation.lock()->get_Enabled() ? representation.lock()->put_Enabled(false) : representation.lock()->put_Enabled(true);

			break;
		}
		case Geoweb3d::Key::F6:
		{
			// Enable/Disable the visual representation of the path posts
			auto layer = _Data.GetDataSource().lock()->get_VectorLayerCollection()->get_ByName("Posts");
			auto representation = layer.lock()->get_RepresentationLayerCollection()->get_AtIndex(0);
			representation.lock()->get_Enabled() ? representation.lock()->put_Enabled(false) : representation.lock()->put_Enabled(true);

			break;
		}
		case Geoweb3d::Key::F7:
		{
			// Enable/Disable the visual representation of all raster imagery layers

			// Nominally this should work, however there is currently a bug with raster collections where the iterator and index or both return null
			//auto imageryLayers = _SdkContext->get_RasterLayerRepresentationDriverCollection()->get_Driver("Imagery").lock()->get_RepresentationLayerCollection();
			//auto layerCount = imageryLayers->count();

			//for (unsigned long i = 0; i < layerCount; i++) {
			//	imageryLayers->get_AtIndex(i).lock()->get_Enabled() ? imageryLayers->get_AtIndex(i).lock()->put_Enabled(false) : imageryLayers->get_AtIndex(i).lock()->put_Enabled(true);
			//}

			for (auto layer : _Imagery) {
				if (!layer.expired()) {
					layer.lock()->get_Enabled() ? layer.lock()->put_Enabled(false) : layer.lock()->put_Enabled(true);
				}
			}

			break;
		}
		case Geoweb3d::Key::F8:
		{
			// Enable/Disable the visual representation of all raster elevation layers

			// Same applies here
			//auto elevationLayers = _SdkContext->get_RasterLayerRepresentationDriverCollection()->get_Driver("Elevation").lock()->get_RepresentationLayerCollection();
			//auto layerCount = elevationLayers->count();

			//for (unsigned long i = 0; i < layerCount; i++) {
			//	elevationLayers->get_AtIndex(i).lock()->get_Enabled() ? elevationLayers->get_AtIndex(i).lock()->put_Enabled(false) : elevationLayers->get_AtIndex(i).lock()->put_Enabled(true);
			//}

			for (auto layer : _Elevation) {
				if (!layer.expired()) {
					layer.lock()->get_Enabled() ? layer.lock()->put_Enabled(false) : layer.lock()->put_Enabled(true);
				}
			}

			break;
		}
		case Geoweb3d::Key::F9:
		{		
			// Toggle the representation of the path ribbon between solid and colored line (wireframe)
			auto layer = _Data.GetDataSource().lock()->get_VectorLayerCollection()->get_ByName("Ribbon");	
			
			if (layer.expired())
				return;
			
			auto representation = layer.lock()->get_RepresentationLayerCollection();
			auto count = representation->count();

			for (unsigned long i = 0; i < count; i++) {
				representation->get_AtIndex(i).lock()->get_Enabled() ? representation->get_AtIndex(i).lock()->put_Enabled(false) : representation->get_AtIndex(i).lock()->put_Enabled(true);
			}
		
			break;
		}
		default:
			break;
		}

		break;
	}
	default:
		std::cout << "Unhandled event [Type: " << event.Type << "]" << std::endl;
		break;
	}
}

// Create a background quad on a given camera which corresponds to the location of the inset camera
bool Engine::CreatePIPCameraBackground(const IGW3DCameraWPtr & camera)
{
	int x = 0;
	int y = 0;
	unsigned int cx = 0;
	unsigned int cy = 0;

	// Make sure the camera is valid
	if (camera.expired()) {
		return false;
	}

	// Get the window coordinates of the requested camera so we can...
	camera.lock()->get_Window().lock()->get_CameraWindowCoordinates(camera, x, y, cx, cy);

	// ... Set up an OpenGL orthographic projection
	glViewport(x, y, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);

	// Disable things we don't need for orthographic projections
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	// PIP Window coordinates (0.75f, 0.975f, 0.025f, 0.25f)
	// Using straight vertices because all we need is a single black quad

	glColor3f(0.0f, 0.0f, 0.0f);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(0.75f, 0.025f);
	glVertex2f(0.75f, 0.25f);
	glVertex2f(0.975f, 0.025f);
	glVertex2f(0.975f, 0.25f);
	glEnd();

	// Re enable things we disabled
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	return true;
}
