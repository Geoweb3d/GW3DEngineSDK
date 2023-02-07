#pragma once

/*! 
	This functionality is outlined and used in Example 0,
	it's been placed here for more concise use in other examples
	without making the emphasis of further examples less clear */

namespace Geoweb3d
{
	namespace Example
	{

bool InitializeSDK( const char* system_name );

bool ShutdownSDK();

void SetupErrorHandling();

void DumpStatistics();

void TestMemory();

	}
}