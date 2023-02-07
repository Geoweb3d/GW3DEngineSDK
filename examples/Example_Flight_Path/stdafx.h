// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// CRT
#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>
#include <string>

// Standard Geoweb3d headers and library references
#include "Geoweb3d\core\GW3DInterface.h"
#include "Geoweb3d\engine\IGW3DGeoweb3dSDK.h"

#include "Geoweb3d\core\GW3DGPUConfig.h"

#include "Geoweb3d\common\GW3DCommon.h"
#include "Geoweb3d\engine\GW3DCamera.h"
#include "Geoweb3d\engine\GW3DWindow.h"
#include "Geoweb3d\engine\GW3DVector.h"
#include "Geoweb3d\engine\GW3DRaster.h"

#include "Geoweb3dCore\SystemExports.h"
#include "Geoweb3dCore\GeometryExports.h"
#include "Geoweb3dCore\LayerParameters.h"
#include "Geoweb3dCore\CoordinateExports.h"
#include "Geoweb3d\engine\IGW3DSceneGraphContext.h"
#include "Geoweb3d\engine\IGW3DImageCollection.h"
#include "Geoweb3d\engine\IGW3DVectorLayerStream.h"
#include "Geoweb3d\engine\IGW3DVectorLayerStreamResult.h"

#include "Geoweb3d\engine\IGW3DFont.h"
#include "Geoweb3d\engine\IGW3D2DDrawContext.h"

#include "Geoweb3d\engine\GW3DFrameAnalysis.h"

#include "engine/IGW3DDisplayDevices.h"
#include "engine/IGW3DDisplayDetails.h"
#include "engine/IGW3DDisplayMode.h"

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

// OpenGL stuff for native access
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

// Commonly used app data
#include "FlightDataController.h"
#include "ModelController.h"
#include "ModelLabelController.h"