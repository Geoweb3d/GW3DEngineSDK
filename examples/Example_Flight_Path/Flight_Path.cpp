// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Engine.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include <string>
#include <vector>
#include <map>
#include <iomanip>

using namespace Geoweb3d;

typedef std::multimap<std::string, std::vector<std::string>> CommandLine;
typedef std::pair<std::string, std::vector<std::string>> CommandLineArg;

HRESULT ParseCommandLine(int argc, char* argv[], CommandLine& commands) {
	HRESULT hr = S_OK;
	std::string currentSwitch = "/flightdata";
	wchar_t delim = '/';

	try {
		// Get the local app path (which can be done in other ways)
		std::string filespec = argv[0];
		commands.emplace("apppath", std::vector<std::string>())->second.push_back(filespec.substr(0, filespec.find_last_of('\\')));

		// Parse the rest of the args out
		for (int i = 1; i < argc; i++) {
			// Is this one an command switch?
			if (argv[i][0] == delim) {
				currentSwitch = argv[i];

				// Add a new switch if and only if we haven't seen this switch before
				if (commands.find(currentSwitch) == commands.end()) {
					commands.emplace(argv[i], std::vector<std::string>());
				}
			}
			else {
				// Everything after the switch is a parameter of that switch unless it's another switch

				// Check to see if the switch doesn't have any parameters
				auto command = commands.find(currentSwitch);
				if (command == commands.end()) {
					// Technically it shouldn't be possible to find a new switch down here since
					// the delimiter check is in the other side of the if() block, but here we are
					// just in case.
					commands.emplace(currentSwitch, std::vector<std::string>())->second.push_back(argv[i]);
				}
				else {
					// Push the argument onto the switch parameter stack
					command->second.push_back(argv[i]);
				}
			}
		}

		// Check to see if the invoker gave us anything to actually register
		if (commands.find("/help") != commands.end()) {
			std::wcout << L"Usage: Demo.exe [/flightdata] <file1> ... <fileN>" << std::endl;
			std::wcout << L"Options:" << std::endl;
			std::wcout << L"  [/imagery <file1> ... <fileN>]" << std::endl;
			std::wcout << L"  [/elevation <file1> ... <fileN>]" << std::endl;
			std::wcout << L"  [/model <file1>]" << std::endl;
			std::wcout << L"  [/export <file> <driver>]" << std::endl << std::endl;

			std::wcout << std::left << std::setw(0);
			hr = E_INVALIDARG;
		}
	}
	catch (std::exception& ex) {
		std::wcout << L"Error parsing command line: " << ex.what() << std::endl;
		hr = E_FAIL;
	}

	return hr;
}

void LogInfo(const char* data ) 
{
	printf("[Info] %s", data);
}

void LogError(const char* data )
{
	printf("[Critical] %s", data);
}

bool TestSystemSupport(IGW3DGeoweb3dSDKPtr sdkContext) 
{
	bool supportsG3D = false;
	auto devices = sdkContext->get_DisplayDevices();
		
	std::cout << devices->get_Count() << " display devices found." << std::endl;
	
	for (int i = 0; i < devices->get_Count(); i++) {
		auto details = devices->get_Details(i);
		auto mode = details->get_CurrentDeviceMode();
		auto abilities = mode->TestModeForGeoweb3dAbilities(false);
		if (abilities->get_IsGeoweb3dCapable()) {
			std::cout << details->get_DeviceName() << " supports Geoweb3d SDK" << std::endl;
			supportsG3D = true;
		}
		else {
			std::cout << details->get_DeviceName() << " does not supports Geoweb3d SDK" << std::endl;
		}
	}

	return supportsG3D;
}

int main(int argc, char* argv[])
{
	// Set up message handling
	SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Information, LogInfo);
	SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Warning, LogInfo);
	SDK_Notification::GeoWeb3dSetIOFunction(Geoweb3d::Fatal, LogError);

	try {
		std::string outputFile;
		CommandLine commands;

		if (SUCCEEDED(ParseCommandLine(argc, argv, commands))) {
			// Create a new Geoweb3d SDK context object
			auto sdkContext = IGW3DGeoweb3dSDK::CreateInterface();

			if (sdkContext) {
				// Create an initialization context
				auto init = sdkContext->create_InitializationConfiguration();

				// We're not using ESRI so disable the entitlement request
				init->put_ESRILicenseCheckout(false);

				// Initialize the SDK
				if (Geoweb3d::Succeeded(sdkContext->InitializeLibrary("geoweb3dsdkdemo", init, 5, 0))) {
					// Test if the system can properly support Geoweb3d
					if (!TestSystemSupport(sdkContext)) {
						return -1;
					}

					// Create the rendering engine
					// Note: The implementation of the following is completely developer specific.
					// Nothing says you can't simply start using the Geoweb3d SDK right here, creating
					// windows and loading data.  This example wraps the SDK up in a class to support
					// a few additional features in meaningful ways.
					Engine engine(sdkContext);

					// Create a Geoweb3d output window and camera
					engine.CreateSdkWindow("Flight Path Demo");
					engine.CreateSdkCameras();

					auto model = commands.find("/model");
					if (model != commands.end()) {
						for (auto modelFile : model->second) {
							if (!engine.LoadModelData(modelFile)) {
								std::cout << "Failed to load model from file '" << modelFile << "'." << std::endl;
							}
						}
					}

					// Load specified imagery data
					auto imagery = commands.find("/imagery");
					if (imagery != commands.end()) {
						for (auto imageryFile : imagery->second) {
							if (!engine.LoadImageryData(imageryFile)) {
								std::cout << "Failed to load imagery data from file '" << imageryFile << "'." << std::endl;
							}
						}
					}

					// Load specified elevation data
					auto elevation = commands.find("/elevation");
					if (elevation != commands.end()) {
						for (auto elevationFile : elevation->second) {
							if (!engine.LoadElevationData(elevationFile)) {
								std::cout << "Failed to load elevation data from file '" << elevationFile << "'." << std::endl;
							}
						}
					}

					// Load specified flight path data
					auto flightData = commands.find("/flightdata");
					if (flightData != commands.end()) {
						for (auto dataSet : flightData->second) {
							if (!engine.LoadFlightData(dataSet)) {
								std::cout << "Failed to load flight path data set from file '" << dataSet << "'." << std::endl;
							}
						}
					}

					// Export loaded flight data
					auto exportInfo = commands.find("/export");
					if (exportInfo != commands.end() && exportInfo->second.size() == 2) {
						if (!engine.ExportData(exportInfo->second[0], exportInfo->second[1])) {
							std::cout << "Data export failed." << std::endl;
						}
					}

					// Let the SDK do its thing
					while (engine.Render()) {
						// Do... nothing.
					}
				}
			}
		}
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
		return -1;
	}

    return 0;
}