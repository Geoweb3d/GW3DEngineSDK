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
#include <cstddef>          // std::size_t
//#include "GW3DInterFace.h"

namespace Geoweb3d
{
namespace detail
{

template<class X> class gw3d_sp_counted_impl_p: public GW3DSpCountedBase
{
private:

    X * px_;

    gw3d_sp_counted_impl_p( gw3d_sp_counted_impl_p const & );
    gw3d_sp_counted_impl_p & operator= ( gw3d_sp_counted_impl_p const & );

    typedef gw3d_sp_counted_impl_p<X> this_type;

public:

    explicit gw3d_sp_counted_impl_p( X * px ): px_( px )
    {

    }

    virtual void dispose() // no throw
    {
        Geoweb3d::GW3DCheckedDelete( px_ );
    }

    virtual void * get_deleter( detail::GW3DSpTypeinfo const & )
    {
        return nullptr;
    }

    virtual void * get_untyped_deleter()
    {
        return nullptr;
    }

    void * operator new( std::size_t sz)
    {
        return GW3DNew( sz ) ;// std::allocator<this_type>().allocate( 1, static_cast<this_type *>(0) );
    }

    void operator delete( void * p )
    {
        GW3DDelete(  p );// std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
    }
};

template<class P, class D> class gw3d_sp_counted_impl_pd: public GW3DSpCountedBase
{
private:

    P ptr; // copy constructor must not throw
    D del; // copy constructor must not throw

    gw3d_sp_counted_impl_pd( gw3d_sp_counted_impl_pd const & );
    gw3d_sp_counted_impl_pd & operator= ( gw3d_sp_counted_impl_pd const & );

    typedef gw3d_sp_counted_impl_pd<P, D> this_type;

public:

    // pre: d(p) must not throw

    gw3d_sp_counted_impl_pd( P p, D d ): ptr(p), del(d)
    {
    }

    gw3d_sp_counted_impl_pd( P p ): ptr( p ), del()
    {
    }

    virtual void dispose() // nothrow
    {
        del( ptr );
    }

    virtual void * get_deleter( detail::GW3DSpTypeinfo const & ti )
    {
        return ti == GW3D_SP_TYPEID(D)? &reinterpret_cast<char&>( del ): nullptr;
    }

    virtual void * get_untyped_deleter()
    {
        return &reinterpret_cast<char&>( del );
    }    
	
	void * operator new( std::size_t sz)
    {
        return GW3DNew( sz ) ;// std::allocator<this_type>().allocate( 1, static_cast<this_type *>(0) );
    }


    void operator delete( void * p )
    {
        GW3DDelete( p );// std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
    }
};

template<class P, class D, class A> class gw3d_sp_counted_impl_pda: public GW3DSpCountedBase
{
private:

    P p_; // copy constructor must not throw
    D d_; // copy constructor must not throw
    A a_; // copy constructor must not throw

    gw3d_sp_counted_impl_pda( gw3d_sp_counted_impl_pda const & );
    gw3d_sp_counted_impl_pda & operator= ( gw3d_sp_counted_impl_pda const & );

    typedef gw3d_sp_counted_impl_pda<P, D, A> this_type;

public:

    // pre: d( p ) must not throw

    gw3d_sp_counted_impl_pda( P p, D d, A a ): p_( p ), d_( d ), a_( a )
    {
    }

    gw3d_sp_counted_impl_pda( P p, A a ): p_( p ), d_(), a_( a )
    {
    }

    virtual void dispose() // nothrow
    {
        d_( p_ );
    }

    virtual void destroy() // nothrow
    {
        typedef typename A::template rebind< this_type >::other A2;

        A2 a2( a_ );

        this->~this_type();
        a2.deallocate( this, 1 );
    }

    virtual void * get_deleter( detail::GW3DSpTypeinfo const & ti )
    {
        return ti == GW3D_SP_TYPEID( D )? &reinterpret_cast<char&>( d_ ): nullptr;
    }
};

} // namespace detail

} // namespace Geoweb3d
