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

#ifndef __ALP_STATISTICS_H__
#define __ALP_STATISTICS_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones para hacer estadística
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *    Manuel Perez
 *	19/09/2017 Escrito
 *
 ****************************************************************************/
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>

namespace alp{

/// Devuelve la mediana de los 3 números a, b y c.
template <typename Int>
inline Int median(const Int& a, const Int& b, const Int& c)
{
    if (a <= b){ // ab
	if (c <= a) // cab
	    return a;

	if (b <= c) // abc
	    return b;

	return c;   // acb
    }
    else{ // ba
	if (a <= c) // bac
	    return a;

	if (b <= c) // bca
	    return c;

	return b;   // cba
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
 *	Frequency_t<std::string> t;
 *	t.add("a");
 *	t.add("b");
 *	t.add("a");
 *	t.add("a");
 *	t.add("b");
 *	t.add("a");
 *	t.add("a");
 *	t.add("b");
 *	t.add("b");
 *	t.add("c");
 *
 *	t.print(std::cout) << '\n';
 *  \endcode
 */
template <typename T>
class Frequency_table{
public:
    using value_type = T;
    using size_t = ::std::size_t;
    
// input-output
    /// Sumamos 1 a la frecuencia absoluta del elemento x.
    void add(const value_type& x) { ++est_[x]; }

    std::ostream& print(std::ostream& out);

// Data
    /// Tamaño de la tabla
    size_t size() const {return est_.size();}

    /// Frecuencia del elemento x
    size_t frequency(const value_type& x) const {return est_.at(x);}
    size_t operator[](const value_type& x) const {return frequency(x);}


// Helpers
    /// Devuelve la tabla de frecuencias como un vector ordenado de pairs.
    /// Ordena la frecuencia absoluta usando Comparator
    template <typename Comparator>
    std::vector<std::pair<T,size_t>> as_sorted_vector(Comparator comp) const;

    /// Devuelve la tabla de frecuencias como un vector ordenado de pairs.
    /// Lo devuelve ordenado por frecuencia absoluta de mayor a menor.
    std::vector<std::pair<T,size_t>> as_sorted_vector() const
    { return as_sorted_vector(std::greater<size_t>{}); }


private:
    std::map<value_type, size_t> est_;
};


template <typename T> template <typename Comparator>
std::vector<std::pair<T, size_t>>
	    Frequency_table<T>::as_sorted_vector(Comparator comp) const
{
    std::vector<std::pair<T,size_t>> res{est_.begin(), est_.end()};

    std::sort(begin(res), end(res), [&comp](auto x1, auto x2){
	    return comp(x1.second, x2.second);
	    });

    return res;
}



template <typename T> 
std::ostream& Frequency_table<T>::print(std::ostream& out)
{
    for (const auto& x: est_)
	out << x.first << " = " << x.second << '\n';

    return out;
}



/*!
 *  \brief  Hace `uniq --count [p0, pe)`
 *
 *   Devuelve un vector de pairs donde first es el elemento que está contando
 *   y second es el número de veces que aparece 'first' en x.
 *
 *   Precondición: los elementos de x tienen que estar ordenados
 *
 */
template<typename Iterator>
std::vector<
    std::pair<typename std::iterator_traits<Iterator>::value_type, size_t>> 
unique_count(Iterator p0, Iterator pe)
{
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    std::vector<std::pair<value_type, size_t>> res;
    
    if(p0 == pe)
	return res;

    // valor actual que contamos
    auto x_a = *p0;
    size_t n_a = 1;	// número de elementos que hay de x_a
    ++p0;

    for(; p0 != pe; ++p0){
	if(x_a != *p0){
	    res.push_back(std::make_pair(x_a, n_a));
	    x_a = *p0; // x[i];
	    n_a = 1;
	} else
	    ++n_a;
    }

    // metemos el último elemento
    res.push_back(std::make_pair(x_a, n_a));

    return res;
}


}// namespace

#endif


