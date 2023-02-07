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
#include <algorithm>            // for std::swap
#include <functional>           // for std::less
#include <typeinfo>             // for std::bad_cast
#include <cstddef>              // for std::size_t
#include "GW3DCheckedDelete.h"
#include "GW3DSharedCount.h"
#include "GW3DSpConvertible.h"

namespace Geoweb3d
{
template<class T> class GW3DSharedPtr;
template<class T> class GW3DWeakPtr;
template<class T> class enable_shared_from_this;
template<class T> class enable_shared_from_this2;

namespace detail
{

struct static_cast_tag {};
struct const_cast_tag {};
struct dynamic_cast_tag {};
struct polymorphic_cast_tag {};

//struct sp_nothrow_tag {};


template<class T> struct GW3DSharedPtr_traits
{
    typedef T & reference;
};

template<> struct GW3DSharedPtr_traits<void>
{
    typedef void reference;
};

// enable_shared_from_this support

template< class X, class Y, class T > inline void gw3dsp_enable_shared_from_this( Geoweb3d::GW3DSharedPtr<X> const * ppx, Y const * py, Geoweb3d::enable_shared_from_this< T > const * pe )
{
    if( pe != nullptr )
    {
        pe->_internal_accept_owner( ppx, const_cast< Y* >( py ) );
    }
}

template< class X, class Y, class T > inline void gw3dsp_enable_shared_from_this( Geoweb3d::GW3DSharedPtr<X> * ppx, Y const * py, Geoweb3d::enable_shared_from_this2< T > const * pe )
{
    if( pe != nullptr )
    {
        pe->_internal_accept_owner( ppx, const_cast< Y* >( py ) );
    }
}

inline void gw3dsp_enable_shared_from_this( ... )
{
}

} // namespace detail


//
//  GW3DSharedPtr

template<class T> class GW3DSharedPtr
{
    typedef GW3DSharedPtr<T> this_type;
public:

    typedef T element_type;
    typedef T value_type;
    typedef T * pointer;
    typedef typename detail::GW3DSharedPtr_traits<T>::reference reference;

    GW3DSharedPtr(): px(nullptr), pn()
    {
    }

    template<class Y>
    explicit GW3DSharedPtr( Y * p ): px( p ), pn( p ) // Y must be complete
    {
        detail::gw3dsp_enable_shared_from_this( this, p, p );
    }

    //
    // Requirements: D's copy constructor must not throw
    //
    // GW3DSharedPtr will release p by calling d(p)
    //

    template<class Y, class D> GW3DSharedPtr(Y * p, D d): px(p), pn(p, d)
    {
        detail::gw3dsp_enable_shared_from_this( this, p, p );
    }

    // As above, but with allocator. A's copy constructor shall not throw.

    template<class Y, class D, class A> GW3DSharedPtr( Y * p, D d, A a ): px( p ), pn( p, d, a )
    {
        detail::gw3dsp_enable_shared_from_this( this, p, p );
    }

    template<class Y>
    explicit GW3DSharedPtr(GW3DWeakPtr<Y> const & r): pn(r.pn) // may throw
    {
        // it is now safe to copy r.px, as pn(r.pn) did not throw
        px = r.px;
    }

    template<class Y>
    GW3DSharedPtr( GW3DWeakPtr<Y> const & r, detail::sp_nothrow_tag ): px( nullptr ), pn( r.pn,detail::sp_nothrow_tag() ) // never throws
    {
        if( !pn.empty() )
        {
            px = r.px;
        }
    }

    template<class Y>
    GW3DSharedPtr( GW3DSharedPtr<Y> const & r, typename detail::sp_enable_if_convertible<Y,T>::type =detail::sp_empty() )
        : px( r.px ), pn( r.pn ) // never throws
    {
    }

    // aliasing
    template< class Y >
    GW3DSharedPtr( GW3DSharedPtr<Y> const & r, T * p ): px( p ), pn( r.pn ) // never throws
    {
    }

    template<class Y>
    GW3DSharedPtr(GW3DSharedPtr<Y> const & r,detail::static_cast_tag): px(static_cast<element_type *>(r.px)), pn(r.pn)
    {
    }

    template<class Y>
    GW3DSharedPtr(GW3DSharedPtr<Y> const & r,detail::const_cast_tag): px(const_cast<element_type *>(r.px)), pn(r.pn)
    {
    }

    template<class Y>
    GW3DSharedPtr(GW3DSharedPtr<Y> const & r,detail::dynamic_cast_tag): px(dynamic_cast<element_type *>(r.px)), pn(r.pn)
    {
        if(px == nullptr) // need to allocate new counter -- the cast failed
        {
            pn =detail::gw3d_shared_count();
        }
    }

    template<class Y>
    GW3DSharedPtr(GW3DSharedPtr<Y> const & r,detail::polymorphic_cast_tag): px(dynamic_cast<element_type *>(r.px)), pn(r.pn)
    {
        if(px == nullptr)
        {
            //printf("GW3D Bad Cast\n");
        }
    }

    // assignment

    GW3DSharedPtr & operator=( GW3DSharedPtr const & r ) // never throws
    {
        this_type(r).swap(*this);
        return *this;
    }

    template<class Y>
    GW3DSharedPtr & operator=(GW3DSharedPtr<Y> const & r) // never throws
    {
        this_type(r).swap(*this);
        return *this;
    }

    void reset()
    {
        this_type().swap(*this);
    }

    template<class Y> void reset(Y * p) // Y must be complete
    {
        this_type(p).swap(*this);
    }

    template<class Y, class D> void reset( Y * p, D d )
    {
        this_type( p, d ).swap( *this );
    }

    template<class Y, class D, class A> void reset( Y * p, D d, A a )
    {
        this_type( p, d, a ).swap( *this );
    }

    template<class Y> void reset( GW3DSharedPtr<Y> const & r, T * p )
    {
        this_type( r, p ).swap( *this );
    }

    reference operator* () const // never throws
    {
        return *px;
    }

    T * operator-> () const // never throws
    {
        return px;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Gets the raw pointer. This function never throws. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    T * get() const
    {
        return px;
    }

// implicit conversion to "bool"
    typedef T * this_type::*unspecified_bool_type;

    operator unspecified_bool_type() const // never throws
    {
        return px == nullptr? nullptr: &this_type::px;
    }
    // operator! is redundant, but some compilers need it
    bool operator! () const // never throws
    {
        return px == nullptr;
    }

    bool unique() const // never throws
    {
        return pn.unique();
    }

    long use_count() const // never throws
    {
        return pn.use_count();
    }

    void swap(GW3DSharedPtr<T> & other) // never throws
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

    template<class Y> bool owner_before( GW3DSharedPtr<Y> const & rhs ) const
    {
        return pn < rhs.pn;
    }

    template<class Y> bool owner_before( GW3DWeakPtr<Y> const & rhs ) const
    {
        return pn < rhs.pn;
    }

    void * _internal_get_deleter(detail::GW3DSpTypeinfo const & ti ) const
    {
        return pn.get_deleter( ti );
    }

    bool _internal_equiv( GW3DSharedPtr const & r ) const
    {
        return px == r.px && pn == r.pn;
    }

private:

    template<class Y> friend class GW3DSharedPtr;
    template<class Y> friend class GW3DWeakPtr;
    T * px;                     // contained pointer
    detail::gw3d_shared_count pn;    // reference counter

};  // GW3DSharedPtr

template<class T, class U> inline bool operator==(GW3DSharedPtr<T> const & a, GW3DSharedPtr<U> const & b)
{
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(GW3DSharedPtr<T> const & a, GW3DSharedPtr<U> const & b)
{
    return a.get() != b.get();
}

template<class T, class U> inline bool operator<(GW3DSharedPtr<T> const & a, GW3DSharedPtr<U> const & b)
{
    return a.owner_before( b );
}

template<class T> inline void swap(GW3DSharedPtr<T> & a, GW3DSharedPtr<T> & b)
{
    a.swap(b);
}

template<class T, class U> GW3DSharedPtr<T> static_pointer_cast(GW3DSharedPtr<U> const & r)
{
    return GW3DSharedPtr<T>(r,detail::static_cast_tag());
}

template<class T, class U> GW3DSharedPtr<T> const_pointer_cast(GW3DSharedPtr<U> const & r)
{
    return GW3DSharedPtr<T>(r,detail::const_cast_tag());
}

template<class T, class U> GW3DSharedPtr<T> dynamic_pointer_cast(GW3DSharedPtr<U> const & r)
{
    return GW3DSharedPtr<T>(r,detail::dynamic_cast_tag());
}

// get_pointer() enables boost::mem_fn to recognize GW3DSharedPtr
template<class T> inline T * get_pointer(GW3DSharedPtr<T> const & p)
{
    return p.get();
}

template<class D, class T> D * get_deleter(GW3DSharedPtr<T> const & p)
{
    return static_cast<D *>(p._internal_get_deleter(typeid(D)));
}

} // namespace Geoweb3d
