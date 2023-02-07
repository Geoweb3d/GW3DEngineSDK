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

#if !defined(_GW3D_WEAKSMARTPTR)
#include "GW3DWeakPtr.h"
#define _GW3D_WEAKSMARTPTR  Geoweb3d::GW3DWeakPtr
#endif

#if !defined(_GW3D_SMARTPTR)
#include "GW3DSharedPtr.h"
#define _GW3D_SMARTPTR        Geoweb3d::GW3DSharedPtr
#endif

/* Primary namespace */
namespace Geoweb3d
{

#if defined(_GW3D_SMARTPTR)
#if !defined(_GW3D_SMARTPTR_TYPEDEF)
#define _GW3D_SMARTPTR_TYPEDEF(Interface) \
			typedef _GW3D_SMARTPTR<Interface> \
			Interface ## Ptr
#endif
#endif

#if defined(_GW3D_WEAKSMARTPTR)
#if !defined(_GW3D_WEAK_SMARTPTR_TYPEDEF)
#define _GW3D_WEAK_SMARTPTR_TYPEDEF(Interface) \
			typedef _GW3D_WEAKSMARTPTR<Interface> \
			Interface ## WPtr
#endif
#endif
#define GEOWEB3D_INTERFACE_VERSION  1

template <typename T>
bool operator == (const _GW3D_WEAKSMARTPTR<T>& a, const _GW3D_WEAKSMARTPTR<T>& b)
{
    return a.unsafe_get() == b.unsafe_get();
}

template <typename T>
bool operator != (const _GW3D_WEAKSMARTPTR<T>& a, const _GW3D_WEAKSMARTPTR<T>& b)
{
    return a.unsafe_get() != b.unsafe_get();
}

template <typename T>
bool operator < (const _GW3D_WEAKSMARTPTR<T>& a, const _GW3D_WEAKSMARTPTR<T>& b)
{
    return a.unsafe_get() < b.unsafe_get();
}
} //end namespace