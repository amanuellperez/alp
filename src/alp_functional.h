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

#ifndef __ALP_FUNCTIONAL_H__
#define __ALP_FUNCTIONAL_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Ampliación de <functional>
 *
 *   - COMENTARIOS: 
 *
 *	Ver TODO.
 *
 *   - HISTORIA:
 *           alp  - 17/02/2019 Escrito
 *
 ****************************************************************************/

#include <functional>

namespace alp{

/// Función objeto que indica si un número es menor que otro dado inicialmente
/// El "menor" viene dado por la relación Rel.
template <typename Int, typename Rel>
//requires Integral<Int> and BinaryPredicate<Rel>
struct Less_than_rel{
    const Int v;
    const Rel r;

    constexpr Less_than_rel(Int v0, Rel r0) :v{v0}, r{r0}{}

    constexpr bool operator()(Int x) const
    { return r(x, v); }
};


///Function object que indica si un número es menor que otro
template <typename Int>
//requires Integral<Int>
struct Less_than: public Less_than_rel<Int, std::less<Int>>{
    constexpr Less_than(Int v0)
        : Less_than_rel<Int, std::less<Int>>(v0, std::less<Int>{})
    {}
};


///Function object que indica si un número es menor o igual que otro
template <typename Int>
//requires Integral<Int>
struct Less_equal_than: public Less_than_rel<Int, std::less_equal<Int>>{
    constexpr Less_equal_than(Int v0)
        : Less_than_rel<Int, std::less_equal<Int>>(v0, std::less_equal<Int>{})
    {}
};


///Function object que indica si un número es mayor que otro
template <typename Int>
//requires Integral<Int>
struct Greater_than: public Less_than_rel<Int, std::greater<Int>>{
    constexpr Greater_than(Int v0)
        : Less_than_rel<Int, std::greater<Int>>(v0, std::greater<Int>{})
    {}
};


///Function object que indica si un número es mayor o igual que otro
template <typename Int>
// requires Integral<Int>
struct Greater_equal_than
    : public Less_than_rel<Int, std::greater_equal<Int>> {

    constexpr Greater_equal_than(Int v0)
	    : Less_than_rel<Int, std::greater_equal<Int>>
	    				(v0, std::greater_equal<Int>{})
    {}
};





/***************************************************************************
 *			    COMPARACIONES
 ***************************************************************************/
// TODO: revisar todos estos tipos Incr_... 
//  1.- ¿los uso? El nomber no me gusta. (en vis_math.h los uso)
//  2.. Simplificar la implementación. ???
/// Devuelve true si 0 < b - a < incr_max
template <typename Int>
struct Incr_positivo_menor_que{
    constexpr explicit Incr_positivo_menor_que(const Int& incr_max0)
	:incr_max{incr_max0}{}

    constexpr bool operator()(const Int& a, const Int& b) const
    {
	auto incr = b - a;
	return (0 < incr) and (incr < incr_max);
    }


    Int incr_max;
};

/// Devuelve true si 0 < b - a <= incr_max
template <typename Int>
struct Incr_positivo_menor_o_igual_que{
    constexpr explicit Incr_positivo_menor_o_igual_que(const Int& incr_max0)
	:incr_max{incr_max0}{}

    constexpr bool operator()(const Int& a, const Int& b) const
    {
	auto incr = b - a;
	return (0 < incr) and (incr <= incr_max);
    }


    Int incr_max;
};


/// Devuelve true si b - a > incr_max
template <typename Int>
struct Incr_positivo_mayor_que{
    constexpr explicit Incr_positivo_mayor_que(const Int& incr_min0)
	:incr_min{incr_min0}{}

    constexpr bool operator()(const Int& a, const Int& b) const
    {
	return (b - a) > incr_min;
    }


    Int incr_min;
};


/// Devuelve true si b - a >= incr_max
template <typename Int>
struct Incr_positivo_mayor_o_igual_que{
    constexpr explicit Incr_positivo_mayor_o_igual_que(const Int& incr_min0)
	:incr_min{incr_min0}{}

    constexpr bool operator()(const Int& a, const Int& b) const
    {
	return (b - a) >= incr_min;
    }


    Int incr_min;
};


/// Devuelve true si a - b > incr_min
template <typename Int>
struct Incr_negativo_mayor_que{
    constexpr explicit Incr_negativo_mayor_que(const Int& incr_min0)
	:incr_min{incr_min0}{}

    constexpr bool operator()(const Int& a, const Int& b) const
    {
	return (a - b) > incr_min;
    }


    Int incr_min;
};

/// Devuelve true si a - b >= incr_min
template <typename Int>
struct Incr_negativo_mayor_o_igual_que{
    constexpr explicit Incr_negativo_mayor_o_igual_que(const Int& incr_min0)
	:incr_min{incr_min0}{}

    constexpr bool operator()(const Int& a, const Int& b) const
    {
	return (a - b) >= incr_min;
    }

    Int incr_min;
};


/// Devuelve true si 0 < a - b < incr_max
template <typename Int>
struct Incr_negativo_menor_que{
    constexpr explicit Incr_negativo_menor_que(const Int& incr_max0)
	:incr_max{incr_max0}{}

    constexpr bool operator()(const Int& a, const Int& b) const
    {
	auto incr = a - b;
	return (0 < incr) and (incr < incr_max);
    }

    Int incr_max;
};

/// Devuelve true si 0 < a - b <= incr_max
template <typename Int>
struct Incr_negativo_menor_o_igual_que{
    constexpr explicit Incr_negativo_menor_o_igual_que(const Int& incr_max0)
	:incr_max{incr_max0}{}

    constexpr bool operator()(const Int& a, const Int& b) const
    {
	auto incr = a - b;
	return (0 < incr) and (incr <= incr_max);
    }

    Int incr_max;
};


}// namespace

#endif
