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

#ifndef __ALP_SMOOTH_H__
#define __ALP_SMOOTH_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones para suavizar una función matemática
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           A.Manuel L.Perez- 01/06/2019 Escrito
 *
 ****************************************************************************/
#include <vector>
#include <type_traits>

#include "alp_estadistica.h"

namespace alp{


// 1ª versión de smooth: versión copiadora. 
// Pasas un listado con la función a suavizar, y devuelve una copia con 
// la función suavizada. Creo que es mejor usar un algoritmo transformador.
///// Devuelve la función suavizada. Uso el algoritmo de Tukey 
///// (ver Exploratory Data Analysis). La idea es calcular la mediana cada 3
///// elementos.
/////
///// Como mínimo hay que pasarle 3 elementos para que el algoritmo funcione. Si
///// se pasan menos elementos o p0 == pe devuelve los datos sin procesarlos.
//template <typename It>
//std::vector<typename std::iterator_traits<It>::value_type> smooth3(It p0, It pe)
//{
//    using Int = typename std::iterator_traits<It>::value_type;
//    std::vector<Int> res;
//
//    // validamos
//    if (p0 == pe)
//	return res;
//
//    auto p1 = std::next(p0);
//    if (p1 == pe)
//	return {p0, pe};
//
//    auto p2 = std::next(p1);
//    if (p2 == pe)
//	return {p0, pe};
//
//    // algoritmo
//    res.reserve(pe - p0);
//    res.push_back(*p0);
//
//    for (; p2 != pe; ++p0, ++p1, ++p2)
//	res.push_back(mediana(*p0, *p1, *p2));
//
//    res.push_back(*p1);
//
//    return res;
//}

/// Transforma la función [p0, pe) suavizándola. Uso el algoritmo de Tukey 
/// (ver Exploratory Data Analysis). La idea es calcular la mediana cada 3
/// elementos.
///
/// Como mínimo hay que pasarle 3 elementos para que el algoritmo funcione. Si
/// se pasan menos elementos o p0 == pe no se hace nada.
///
/// Devuelve el número de elementos que se han cambiado.
template <typename It>
int smooth3(It p0, It pe)
{
    // validamos
    if (p0 == pe)
	return 0;

    auto p1 = std::next(p0);
    if (p1 == pe)
	return 0;

    auto p2 = std::next(p1);
    if (p2 == pe)
	return 0;

    auto v0 = *p0;
    auto v1 = *p0;
    auto v2 = *p1;

    // algoritmo
    int num_cambiados = 0;

    for (; p2 != pe; ++p0, ++p1, ++p2){
	v0 = v1;    
	v1 = v2;    // *p0 y *p1 están invalidados ya que escribimos la mediana
	v2 = *p2;   // en ellos. No contienen los valores originales.

	// [ expects: v0,v1,v2 valores originales de p0,p1,p2 ]
	auto med = mediana(v0, v1, v2);

	if (*p1 != med){
	    *p1 = med;
	    ++num_cambiados;
	}
    }

    return num_cambiados;
}



}// namespace


#endif

