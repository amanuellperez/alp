// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __ALP_ALGORITHM_H__
#define __ALP_ALGORITHM_H__
/****************************************************************************
 *
 * - DESCRIPCION: Ampliación de <algorithm>
 *
 * - HISTORIA:
 *     A.Manuel L.Perez 
 *       17/02/2019 Escrito
 *       16/05/2020 Reestructuro. Fusiono alp_alg con este fichero. 
 *                  Con todo hay que revisar este fichero. 
 *
 ****************************************************************************/

#include <alp_find.h>	// familia de algoritmos find

#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <utility>

namespace alp{

/// Copia [p0, pe) en v, borrando cualquier dato que tuviera v.
// TODO: cambiar orden: copy(p0, pe, q0) y no al revés.
template <typename Value, typename It>
inline void copy_new(It p0, It pe, std::vector<Value>& v)
{
    v.clear();

    v.resize(pe - p0);

    std::copy(p0, pe, v.begin());
}




/***************************************************************************
 *		     FUNCIONES AUXILIARES PARA std::vector
 ***************************************************************************/
/// Hace un pop_back del vector. Es syntactic sugar.
/// De todas formas recordar que si hay dos funciones back and pop_back en
/// vector es para poder garantizar la strong garanty en excepciones.
template <typename T>
inline T pop_back(std::vector<T>& v)
{
    auto res = v.back();
    v.pop_back();
    return res;
}

/// Busca el índice que ocupa el elemento 'x' en el vector 'v'.
///
/// \return Si lo encuentra, el índice i que ocupa el elemento x.
///	    Si no lo encuentra, devuelve v.size().
template<typename T>
auto find_indice(const std::vector<T>& v, const T& x)
{
    for(typename std::vector<T>::size_type i = 0; i < v.size(); ++i)
	if(v[i] == x)
	    return i;

    return v.size();
}


/// Miramos si el elemento 's' pertenece al vector 'v'. 
/// Ejemplo: if(pertenece(s, v)) ...
template<typename T>
inline bool pertenece(const T& s, std::vector<T> v)
{ return (std::find(v.begin(), v.end(), s) != v.end()); }

/// Ordenamos un vector. Ejemplo: sort(v);
template<typename T>
inline void sort(std::vector<T>& v)
{sort(v.begin(), v.end());}


/// Convierte un vector<T> en un map<int, T>, donde el int indica la posición
/// del elemento en el vector.
template<typename T>
std::map<int, T> vector2map(const std::vector<T>& v)
{
    std::map<int, T> m;
    for(typename std::vector<T>::size_type i = 0; i < v.size(); ++i)
	m[i] = v[i];

    return m;
}



}// namespace

#endif


