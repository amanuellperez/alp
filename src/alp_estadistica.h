// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#ifndef __ALP_ESTADISTICA_H__
#define __ALP_ESTADISTICA_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones para hacer estadística
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 19/09/2017 Escrito
 *
 ****************************************************************************/
#include <iostream>
#include <map>
#include <algorithm>

// namespace Estadistica{ ¿¿¿Mejor Estadistica???
namespace alp{

/// Devuelve la mediana de los 3 números a, b y c.
template <typename Int>
Int mediana(Int a, Int b, Int c)
{
    if (a <= b){
	if (b <= c) return b;
	else if (c <= a) return a;
	else return c;
    }
    else{// b < a
	if (a <= c) return a;
	else if (b <= c) return c;
	else return b;
    }
}

/*!
 *  \brief  Tabla de frecuencias
 *
 *  Tabla para sacar estadísticas de un tipo. Construimos la tabla 
 *  como si construyésemos el diagrama de tallos: vamos añadiendo uno a uno
 *  cada elemento.
 *
 *
 *  Ejemplo:
 *  \code
 *	Tabla_de_frecuencias<std::string> tabla;
 *	tabla.add("a");
 *	tabla.add("b");
 *	tabla.add("a");
 *	tabla.add("a");
 *	tabla.add("b");
 *	tabla.add("a");
 *	tabla.add("a");
 *	tabla.add("b");
 *	tabla.add("b");
 *	tabla.add("c");
 *
 *	cout << tabla << '\n';
 *  \endcode
 */
template <typename T>
class Tabla_de_frecuencias{
public:
    using value_type = T;
    using size_t = ::std::size_t;
    
    /// Sumamos 1 a la frecuencia absoluta del elemento x.
    /// Si es la primera vez que anotamos este número, su frecuencia inicial
    /// es 0 (+1 = 1).
    void add(const value_type& x)
    {
	if (est_.find(x) == est_.end())
	    est_[x] = size_t{1};
	else
	    ++est_[x];
    }



    /// Devuelve la tabla de frecuencias como un vector de pairs.
    /// Ordena la frecuencia absoluta usando Comparator
    template <typename Comparator>
    std::vector<std::pair<T,size_t>> as_sorted_vector(Comparator comp) const
    {
	std::vector<std::pair<T,size_t>> res;

	for (const auto& x: est_)
	    res.push_back(x);

	std::sort(begin(res), end(res), [&comp](auto x1, auto x2){
		return comp(x1.second, x2.second);
		});

	return res;
    }

    /// Devuelve la tabla de frecuencias como un vector de pairs.
    /// Lo devuelve ordenado por frecuencia absoluta de mayor a menor.
    std::vector<std::pair<T,size_t>> as_sorted_vector() const
    {
	return as_sorted_vector(std::greater<size_t>{});
    }


    friend std::ostream& operator<<(std::ostream& out,
                                    const Tabla_de_frecuencias& t)
    {
	for (const auto& x: t.est_)
	    out << x.first << " = " << x.second << '\n';

	return out;
    }


private:
    std::map<value_type, size_t> est_;
};

/*!
 *  \brief  Crea la tabla de frecuencias de [p0, pe)
 *
 *   Devuelve un vector de pairs donde first es el elemento que está contando
 *   y second es el número de veces que aparece 'first' en x.
 *
 *   Precondición: los elementos de x tienen que estar ordenados
 *
 */
template<typename Iterator>
// T is regular type
std::vector<
    std::pair<typename std::iterator_traits<Iterator>::value_type, size_t>> 
tabla_de_frecuencias(Iterator p0, Iterator pe)
{
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    std::vector<std::pair<value_type, size_t>> num_elementos;
    
    if(p0 == pe)
	return num_elementos;

    // valor actual que contamos
    auto x_a = *p0;
    size_t n_a = 1;	// número de elementos que hay de x_a
    ++p0;

    for(; p0 != pe; ++p0){
	if(x_a != *p0){
	    num_elementos.push_back(std::make_pair(x_a, n_a));
	    x_a = *p0; // x[i];
	    n_a = 1;
	} else
	    ++n_a;
    }

    // metemos el último elemento
    num_elementos.push_back(std::make_pair(x_a, n_a));

    return num_elementos;
}

}// namespace

#endif


