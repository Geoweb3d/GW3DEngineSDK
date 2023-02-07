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
#include <functional>
#include <typeinfo>

namespace Geoweb3d
{
namespace detail
{
typedef std::type_info GW3DSpTypeinfo;
} // namespace detail
} // namespace Geoweb3d

#define GW3D_SP_TYPEID(T) typeid(T)
