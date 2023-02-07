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
#include "GW3DSharedCount.h"
#include "GW3DSharedPtr.h"

/* Primary namespace */
namespace Geoweb3d
{

template<class T> class GW3DWeakPtr
{
private:
    typedef GW3DWeakPtr<T> this_type;

public:
    typedef T element_type;

    GW3DWeakPtr(): px(nullptr), pn()
    {
    }

    template<class Y>
    GW3DWeakPtr( GW3DWeakPtr<Y> const & r, typename detail::sp_enable_if_convertible<Y,T>::type = detail::sp_empty() )
        : px(r.lock().get()), pn(r.pn) // never throws
    {
    }

    template<class Y>
    GW3DWeakPtr( GW3DSharedPtr<Y> const & r, typename detail::sp_enable_if_convertible<Y,T>::type = detail::sp_empty() )
        : px( r.px ), pn( r.pn ) // never throws
    {
    }

    template<class Y>
    GW3DWeakPtr & operator=(GW3DWeakPtr<Y> const & r) // never throws
    {
        px = r.lock().get();
        pn = r.pn;
        return *this;
    }

    template<class Y>
    GW3DWeakPtr & operator=(GW3DSharedPtr<Y> const & r) // never throws
    {
        px = r.px;
        pn = r.pn;
        return *this;
    }

    GW3DSharedPtr<T> lock() const // never throws
    {
        return GW3DSharedPtr<element_type>( *this, detail::sp_nothrow_tag() );
    }

    T * unsafe_get() const // never throws
    {
        return px;
    }

    long use_count() const // never throws
    {
        return pn.use_count();
    }

    bool expired() const // never throws
    {
        return pn.use_count() == 0;
    }

    bool _empty() const // extension, not in std::GW3DWeakPtr
    {
        return pn.empty();
    }

    void reset()
    {
        this_type().swap(*this);
    }

    void swap(GW3DWeakPtr<T> & other) // never throws
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

    void _internal_assign(T * px2, detail::gw3d_shared_count const & pn2)
    {
        px = px2;
        pn = pn2;
    }

    template<class Y> bool owner_before( GW3DWeakPtr<Y> const & rhs ) const
    {
        return pn < rhs.pn;
    }

    template<class Y> bool owner_before( GW3DSharedPtr<Y> const & rhs ) const
    {
        return pn < rhs.pn;
    }

    void _internal_assign(T * px2, detail::gw3d_weak_count const & pn2)
    {
        px = px2;
        pn = pn2;
    }

    detail::gw3d_weak_count *get_internal_weakcount()
    {
       return &pn;
    }
private:

    template<class Y> friend class GW3DWeakPtr;
    template<class Y> friend class GW3DSharedPtr;
    T * px;
    detail::gw3d_weak_count pn;
};  // GW3DWeakPtr

template<class T, class U> inline bool operator<(GW3DWeakPtr<T> const & a, GW3DWeakPtr<U> const & b)
{
    return a.owner_before( b );
}

template<class T> void swap(GW3DWeakPtr<T> & a, GW3DWeakPtr<T> & b)
{
    a.swap(b);
}
}
