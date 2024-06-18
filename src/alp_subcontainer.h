// Copyright (C) 2019-2020 Manuel Perez <manuel2perez@proton.me>
//
// This file is part of the ALP Library.
//
// ALP Library is a free library: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#ifndef __ALP_SUBCONTAINER_H__
#define __ALP_SUBCONTAINER_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Un subcontainer = parte de un container.
 *	    Es un view del container.
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           Manuel Perez- 05/03/2019 Escrito
 *
 ****************************************************************************/
#include <iterator> 

#include "alp_type_traits.h"
#include "alp_iterator.h"   // view_iterator

namespace alp{

/*!
 *  \brief  Mostramos una pequeña parte de un container.
 *  
 *  No es más que un bounded_range [p0, pe) de iteradores It.
 *
 *  Precondición: [p0, pe) están bien definidos (ambos son iteradores
 *  válidos; bueno *pe no está definido).
 *
 */
template <typename It>
// requires is_iterator<It>
struct Subcontainer{
    // Tipos que vemos
    using value_type      = typename std::iterator_traits<It>::value_type;
    using reference       = typename std::iterator_traits<It>::reference;
    using const_reference = std::add_const<reference>;
    using pointer         = typename std::iterator_traits<It>::pointer;
    using const_pointer   = std::add_const<pointer>;

    // Indices
    using difference_type = typename std::iterator_traits<It>::difference_type;
    using size_type       = std::make_unsigned_t<difference_type>;

    // Iteradores
    using iterator       = It;
    using const_iterator = typename alp::iterator_traits<It>::const_iterator;

    //    using reverse_iterator
    //    using const_reverse_iterator

    // Construimos el subcontainer [p0, n), con n = size del rango.
    Subcontainer() {}

    Subcontainer(iterator p00, size_type n)
	: p0{p00}, pe{p00 + n}
    {}


    // Construimos el subcontainer [p0, pe)
    Subcontainer(iterator p00, iterator pe0)
	: p0{p00}, pe{pe0}
    { }


    // Tamaño
    bool empty() const {return p0 == pe;}
    size_type size() const {return pe - p0;}

    // Iteradores
    iterator begin() {return p0;}
    iterator end() {return pe;}

    const_iterator begin() const {return p0;}
    const_iterator end() const{return pe;}

    const_iterator cbegin() const {return p0;}
    const_iterator cend() const {return pe;}

    // Acceso aleatorio
    reference operator[](size_type i) {return *(p0 + i);}
    const_reference operator[](size_type i) const {return *(p0 + i);}


    // Relaciones
    friend
    bool operator==(const Subcontainer& s1, const Subcontainer& s2)
    {return (s1.p0 == s2.p0) and (s1.pe == s2.pe);}

    friend
    bool operator!=(const Subcontainer& s1, const Subcontainer& s2)
    {return !(s1 == s2);}


    iterator p0, pe;	// [p0, pe)
};

template <typename It, typename View>
inline 
Subcontainer<View_iterator<It, View>> container_view(It p0, It pe, View view)
{
    return Subcontainer<View_iterator<It, View>>{view_iterator(p0, view), 
						 view_iterator(pe, view)};
}
}// namespace

#endif


