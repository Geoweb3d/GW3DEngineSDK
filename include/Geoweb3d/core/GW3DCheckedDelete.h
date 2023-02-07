//////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// Geoweb3d SDK license under which the SDK provided.
//
//////////////////////////////////////////////////////////////////////////////

# pragma once

namespace Geoweb3d
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
///
/// <typeparam name="T"> TODO </typeparam>
/// <param name="x"> TODO </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T> inline void GW3DCheckedDelete(T * x)
{
    typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
    (void) sizeof(type_must_be_complete);
    delete x;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
///
/// <typeparam name="T"> TODO </typeparam>
/// <param name="x"> TODO </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T> inline void GW3DCheckedArrayDelete(T * x)
{
    typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
    (void) sizeof(type_must_be_complete);
    delete [] x;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
///
/// <typeparam name="T"> TODO </typeparam>
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T> struct GW3DCheckedDeleter
{
    typedef void result_type;
    typedef T * argument_type;

    void operator()(T * x) const
    {
        Geoweb3d::GW3DCheckedDelete(x);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
///
/// <typeparam name="T"> TODO </typeparam>
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T> struct GW3DCheckedArrayDeleter
{
    typedef void result_type;
    typedef T * argument_type;

    void operator()(T * x) const
    {
        Geoweb3d::GW3DCheckedArrayDelete(x);
    }
};

} // namespace Geoweb3d