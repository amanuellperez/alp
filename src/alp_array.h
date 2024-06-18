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

#ifndef __ALP_ARRAY_H__
#define __ALP_ARRAY_H__

#include <array>

namespace alp{

/*!
 *  \brief  Es un array finito que concebimos como infinito.
 *
 *  Cuando se llega al final del array, se vuelve a empezar.
 *
 *  Esto es, si el array es {1,2,3,4}, el array_circular es como si tuviésemos
 *
 *		    1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4...
 *
 *  Util, por ejemplo, para generar infinitos colores a partir de solo 10 y
 *  marcar zonas de intensidad.
 */
// TODO: usar el de atd tiene ya Circular_array
// Hay que modificar vis_exp.cpp es donde lo estoy usando.
template<typename T, std::size_t N>
struct Array_circular:public std::array<T, N>{
    using reference = typename std::array<T,N>::reference;
    using const_reference = typename std::array<T,N>::const_reference;

    using size_type = typename std::array<T,N>::size_type;

    reference operator[](size_type i)
    {return std::array<T,N>::operator[](i%N);}

    const_reference operator[](size_type i) const
    {return std::array<T,N>::operator[](i%N);}

};


}// namespace

#endif
