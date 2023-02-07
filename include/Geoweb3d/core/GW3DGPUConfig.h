//////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// Geoweb3d SDK license under which the SDK provided.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

//Enable High Performance GPU on Optimus and AMD devices.   
//You do not call these, as the drivers turn on your gpu if needed by the GPU driver itself.

extern "C" 
{
   _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
   _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
