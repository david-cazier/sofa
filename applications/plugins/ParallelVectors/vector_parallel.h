/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2017 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_HELPER_VECTOR_PARALLEL_H
#define SOFA_HELPER_VECTOR_PARALLEL_H

#include <sofa/helper/vector.h>
#include "ParallelMemoryManager.h"

namespace sofa
{

namespace helper
{



/// Parallel vector
template <class T>
class vector<T, ParallelMemoryManager<T> > : public std::vector<T, std::allocator<T> >
{
public:
    typedef ParallelMemoryManager<T> memory_manager;
    typedef std::allocator<T> Alloc;
    /// size_type
    typedef typename std::vector<T,Alloc>::size_type size_type;
    /// reference to a value (read-write)
    typedef typename std::vector<T,Alloc>::reference reference;
    /// const reference to a value (read only)
    typedef typename std::vector<T,Alloc>::const_reference const_reference;

    template<class T2> struct rebind
    {
        typedef vector< T2,ParallelMemoryManager<T2> > other;
    };

    /// Basic constructor
    vector() : std::vector<T,Alloc>() {}
    /// Constructor
    vector(size_type n, const T& value): std::vector<T,Alloc>(n,value) {}
    /// Constructor
    vector(int n, const T& value): std::vector<T,Alloc>(n,value) {}
    /// Constructor
    vector(long n, const T& value): std::vector<T,Alloc>(n,value) {}
    /// Constructor
    explicit vector(size_type n): std::vector<T,Alloc>(n) {}
    /// Constructor
    vector(const std::vector<T, Alloc>& x): std::vector<T,Alloc>(x) {}
    /// Move constructor
    vector(std::vector<T,Alloc>&& v): std::vector<T,Alloc>(std::move(v)) {}

    /// Copy operator
    vector<T, Alloc>& operator=(const std::vector<T, Alloc>& x)
    {
        std::vector<T,Alloc>::operator=(x);
        return *this;
    }
    /// Move assignment operator
    vector<T, Alloc>& operator=(std::vector<T,Alloc>&& v)
    {
        std::vector<T,Alloc>::operator=(std::move(v));
        return *this;
    }


#ifdef __STL_MEMBER_TEMPLATES
    /// Constructor
    template <class InputIterator>
    vector(InputIterator first, InputIterator last): std::vector<T,Alloc>(first,last) {}
#else /* __STL_MEMBER_TEMPLATES */
    /// Constructor
    vector(typename vector<T>::const_iterator first, typename vector<T>::const_iterator last): std::vector<T>(first,last) {}
#endif /* __STL_MEMBER_TEMPLATES */


#ifdef SOFA_VECTOR_ACCESS_FAILURE

    /// Read/write random access
    reference operator[](size_type n)
    {
        if (n>=this->size())
            vector_access_failure(this, this->size(), n, typeid(T));
        //assert( n<this->size() );
        return *(this->begin() + n);
    }

    /// Read-only random access
    const_reference operator[](size_type n) const
    {
        if (n>=this->size())
            vector_access_failure(this, this->size(), n, typeid(T));
        //assert( n<this->size() );
        return *(this->begin() + n);
    }

#endif // SOFA_VECTOR_ACCESS_FAILURE


    std::ostream& write(std::ostream& os) const
    {
        if( this->size()>0 )
        {
            for( size_type i=0; i<this->size()-1; ++i )
                os<<(*this)[i]<<" ";
            os<<(*this)[this->size()-1];
        }
        return os;
    }

    std::istream& read(std::istream& in)
    {
        T t=T();
        this->clear();
        while(in>>t)
        {
            this->push_back(t);
        }
        if( in.rdstate() & std::ios_base::eofbit ) { in.clear(); }
        return in;
    }

/// Output stream
    inline friend std::ostream& operator<< ( std::ostream& os, const vector<T>& vec )
    {
        return vec.write(os);
    }

/// Input stream
    inline friend std::istream& operator>> ( std::istream& in, vector<T>& vec )
    {
        return vec.read(in);
    }

    /// Sets every element to 'value'
    void fill( const T& value )
    {
        std::fill( this->begin(), this->end(), value );
    }

    /// this function is usefull for vector_device because it resize the vector without device operation (if device is not valid).
    /// Therefore the function is used in asynchronous code to safly resize a vector which is either cuda of helper::vector
    void fastResize(size_type n) {
        this->resize(n);
    }
};



} // namespace helper

} // namespace sofa
#endif //SOFA_HELPER_VECTOR_PARALLEL_H
