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


#ifndef __ALP_ITERATOR_H__
#define __ALP_ITERATOR_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Clases genéricas a todos los iteradores
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           Manuel Perez- 07/03/2019 Escrito
 *
 ****************************************************************************/

#include "alp_type_traits.h"

#include <iterator>


namespace alp{

/*!
 *  \brief  Wrapper ampliador de un iterador mínimo.
 *
 *  Basado en el curso de Alexander Stepanov (A9).
 *
 *  La idea es que muchas de las operaciones que suministra un iterador se
 *  basan en un conjunto mínimo de operaciones. En It_base definimos esas
 *  operaciones y este wrapper las amplia definiendo todas las operaciones que
 *  suministra un iterador.
 *
 */
template <typename I>
// I = Iterator mínimo.
struct Iterator {
    //requires is_iterator<It>
    static_assert(alp::is_iterator_v<I>, "El tipo pasado It no es un iterador"); 

    using value_type      = typename std::iterator_traits<I>::value_type;
    using reference       = typename std::iterator_traits<I>::reference;
    using pointer         = typename std::iterator_traits<I>::pointer;
    using difference_type = typename std::iterator_traits<I>::difference_type;
    using iterator_category =
        typename std::iterator_traits<I>::iterator_category;

    // Iteradores const y no const que suministra I
    using It       = typename alp::iterator_traits<I>::iterator;
    using const_It = typename alp::iterator_traits<I>::const_iterator;

    static_assert(!std::is_same_v<It, const_It>,
                  "It = const_It!!! Esto no puede ser!");

    // Versiones const y no const de Iterator. Estas versiones son
    // fundamentales para la construcción correcta del iterador.
    using iterator       = Iterator<It>;
    using const_iterator = Iterator<const_It>;

    static constexpr bool is_const_iterator = std::is_same_v<I, const_It>;

    I it;

    // Es regular
    Iterator() {}

    // Construcción
    // ------------
    // Cuando haya concepts se puede elegir qué funciones generar o no
    // dependiendo de si este iterador es const o no const. Mientras tanto uso
    // static_assert para dar errores legibles.
    Iterator(const It& it0) : it{it0} { }

    Iterator(const const_It& it0) 
    {
        static_assert(is_const_iterator,
            "Se está intentando construir un iterator con un const_iterator");

	it = it0;
    }

    Iterator(const Iterator<It>& p)
	: it{p.it} { }

    Iterator(const Iterator<const_It>& p)
    {
        static_assert(
	    is_const_iterator,
            "Se está intentando construir un iterator con un const_iterator");

	// Lo creo así para que de el error de la aserción y poder depurar
	// más fácilmente los errores
	it = p.it;
    }

    // operator=
    // ---------
    Iterator& operator=(const It& it0)
    {
        it = it0;
        return *this;
    }

    Iterator& operator=(const const_It& it0)  
    {
        static_assert(is_const_iterator,
            "Se está intentando construir un iterator con un const_iterator");

	it = it0;
        return *this;
    
    }

    Iterator& operator=(const Iterator<It>& p)
    {
        it = p.it;
        return *this;
    } 

    Iterator& operator=(const Iterator<const_It>& p)
    {
        static_assert(
	    is_const_iterator,
            "Se está intentando construir un iterator con un const_iterator");

	// Lo creo así para que de el error de la aserción y poder depurar
	// más fácilmente los errores
	it = const_It{p.it};

	return *this;
    }

    // convertimos Iterator to const_Iterator
    explicit operator const_iterator() const
    { return const_iterator{it}; }


    // En general, operator* es const. Sin embargo, en algunos casos
    // como en Row_iterator tiene que ser no const. De aquí que suministremos
    // las dos versiones. 
    reference operator*() { return *it; }
    reference operator*() const { return *it; }

    pointer operator->() { return &(**this); }

    Iterator& operator++()
    {
        ++it;
        return *this;
    }

    Iterator operator++(int)
    {
        Iterator tmp = *this;
        ++*this;
        return tmp;
    }

    friend
    bool operator==(const Iterator& a, const Iterator& b)
    {return a.it == b.it;}

    friend
    bool operator!=(const Iterator& a, const Iterator& b)
    {return !(a == b);}


    // for bidirectional Iterator
    // --------------------------
    Iterator& operator--()
    {
        --it;
        return *this;
    }

    Iterator operator--(int)
    {
        Iterator tmp = *this;
        --*this;
        return tmp;
    }

    // for random access Iterator
    // --------------------------
    Iterator& operator+=(difference_type i)
    {
        it += i;
        return *this;
    }

    Iterator& operator-=(difference_type i)
    {
        *this += -i;
        return *this;
    }

    reference operator[](difference_type i) const 
    { return *(*this + i); }

    friend 
    difference_type operator-(const Iterator& x, const Iterator& y)
    { return x.it - y.it; }


    Iterator operator-(difference_type i) { return *this + (-i); }

    friend
    Iterator operator+(Iterator x, difference_type i) 
    { return x += i; }

    friend
    Iterator operator+(difference_type i, Iterator x) 
    { return x += i; }

    friend
    bool operator<(const Iterator& x, const Iterator& y)
    { return x - y < 0; }

    friend
    bool operator>(const Iterator& x, const Iterator& y)
    { return y < x; }

    friend
    bool operator<=(const Iterator& x, const Iterator& y)
    { return !(y < x); }

    friend
    bool operator>=(const Iterator& x, const Iterator& y)
    { return !(x < y); }
};



/***************************************************************************
 *				VIEW_ITERATOR
 ***************************************************************************/
/*!
 *
 *  PROBLEMA CON LAS VIEWS:
 *	Como las lambdas no se pueden copiar, este tipo de iteradores no
 *  se puede copiar si se usan lambdas como views!!!
 *
 *  \brief Convierte un iterador It que da acceso a un objeto en un iterador
 *  \brief que da acceso a alguna propiedad del objeto.
 *
 *  It = iterador del contenedor sobre el que colocamos la view.
 *
 *  Hay dos tipos de estos iteradores: 
 *	1.- aquellos que dan acceso a una propiedad que almacena el contenedor
 *	(por ejemplo, el red en un color RGB). Estos iteradores pueden
 *	devolver una referencia  o un puntero a la propiedad ya que 
 *	esta propiedad está almacenada en memoria.
 *
 *	2.- aquellos que dan acceso a una propiedad calculada a partir de los
 *	elementos del contenedor.  Estos iteradores no pueden devolver una
 *	referencia ni un puntero a la propiedad, ya que esta se crea
 *	temporalmente por Propiedad(*it).
 *
 *  El iterador que hay que usar es View_iterator o const_View_iterator.
 */
template <typename I, typename View>
// requires is_iterator<It>
// View(*it) = valor al que queremos acceder.
// Ejemplo: si *It = ColorRGB, View(*it) podría ser el color R, ó el G ó el B
// o podría ser la intensidad I.
struct View_iterator_base{
    //requires is_iterator<It>
    static_assert(alp::is_iterator_v<I>, "El tipo pasado I no es un iterador"); 

    // datos
    I it;
    View view;	// View = the view of *it

    // traits del iterador
    using view_traits =
        View_traits<View, typename std::iterator_traits<I>::reference>;

    using value_type = typename view_traits::value_type;
    using reference  = typename view_traits::reference;
    using pointer    = typename view_traits::pointer;

    using difference_type = typename std::iterator_traits<I>::difference_type;
    using iterator_category =
			typename std::iterator_traits<I>::iterator_category;

    // Iteradores const y no const que suministra I
    using It       = typename alp::iterator_traits<I>::iterator;
    using const_It = typename alp::iterator_traits<I>::const_iterator;

    // Versiones const y no const de Iterator. Estas versiones son
    // fundamentales para la construcción correcta del iterador.
    using iterator       = View_iterator_base<It, View>;
    using const_iterator = View_iterator_base<const_It, View>;


    // Construcción
    // ------------
    // Es regular
    View_iterator_base() {}

    // Cuando haya concepts se puede elegir qué funciones generar o no
    // dependiendo de si este iterador es const o no const. Mientras tanto uso
    // static_assert para dar errores legibles.
    View_iterator_base(const I& it0, View view0)
        : it{it0}, view{view0}
    { } 

    
    explicit View_iterator_base(const View_iterator_base<I, View>& p)
	: it{p.it}, view{p.view} { }

    // convertimos Iterator to const_Iterator
    // explicit operator const_iterator() const
    operator const_iterator() const
    { return const_iterator{it, view}; }


    friend
    bool operator==(const View_iterator_base& a, 
		    const View_iterator_base& b)
    { return a.it == b.it; }

    friend
    bool operator!=(const View_iterator_base& a, 
		    const View_iterator_base& b)
    {return !(a == b);}


    // for input iterator
    reference operator*() const {return view(*it);}

    void operator++() {++it;}

    // for bidirectional iterator
    void operator--() {--it;}

    // for random access iterator
    void operator+=(difference_type i) {it += i;}

    difference_type operator-(const View_iterator_base& b) const
    {return it - b.it;}

};

// Iteradores mínimos View_iterator. Versión const y no const.
template <typename I, typename View>
using View_iterator_min =
    View_iterator_base<typename alp::iterator_traits<I>::iterator, View>;

template <typename I, typename View>
using const_View_iterator_min =
    View_iterator_base<typename alp::iterator_traits<I>::const_iterator, View>;


// Iteradores View_iterator. Versión const y no const
template <typename It, typename View>
using View_iterator = Iterator<View_iterator_min<It, View>>;

template <typename It, typename View>
using const_View_iterator = Iterator<const_View_iterator_min<It, View>>;


/// uso: 
///	    It it0;
///	    auto it = view_iterator(it0, View{});
template <typename It, typename View>
inline View_iterator<It, View> view_iterator(const It& it, View view)
{
    return View_iterator<It, View>{View_iterator_min<It, View>{it, view}};
}

template <typename It, typename View>
inline const_View_iterator<It, View> const_view_iterator(It it, View view)
{
    return const_View_iterator<It, View>{
				const_View_iterator_min<It, View>{it, view}};
}


}// namespace

#endif

