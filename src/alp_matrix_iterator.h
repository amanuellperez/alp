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

#ifndef __ALP_MATRIX_ITERATOR_H__
#define __ALP_MATRIX_ITERATOR_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Iteradores usados para acceder a contenedores
 *  bidimensionales.
 *
 *
 *  - HISTORIA:
 *    Manuel Perez
 *       05/07/2016 Escrito
 *	 30/12/2017 Generalizado (de img a alp)
 *	 10/03/2019 Reescrito.
 *
 ****************************************************************************/

#include <iterator>
#include <type_traits>

#include <atd_math.h>

#include "alp_type_traits.h"
#include "alp_iterator.h"
#include "alp_subcontainer.h"
#include "alp_cast.h"
#include "alp_math.h"	

namespace alp{

/*!
 *  \brief  Iterador que itera sobre un contenedor unidimensional.
 *
 *  Este iterador lo que hace es descompone el container en subcontainers
 *  todos de igual tamaño. Devuelve un subcontainer.
 *
 *  Es totalmente genérico, válido para cualquier contenedor unidimensional.
 *
 *  It = iterador del contenedor unidimensional que convertimos en
 *  bidimensional via Row_iterator_base
 *
 *  Precondición: se da por supuesto que el contenedor [p0, pe) lo podemos 
 *  descomponer en filas (esto es, que pe - p0 es divisible por cols)
 */
 // Leer comentarios al state_type
template <typename I>
// requires: is_iterator<I>
struct Row_iterator_base{
    using Subcontainer = alp::Subcontainer<I>;

    using value_type      = Subcontainer;
    using reference       = Subcontainer&;
    using pointer         = Subcontainer*;
    using iterator_category = std::random_access_iterator_tag;

    // Iteradores const y no const que suministra I
    using It       = typename alp::iterator_traits<I>::iterator;
    using const_It = typename alp::iterator_traits<I>::const_iterator;

    // Versiones const y no const de Iterator. Estas versiones son
    // fundamentales para la construcción correcta del iterador.
    using iterator       = Row_iterator_base<It>;
    using const_iterator = Row_iterator_base<const_It>;

    using size_type       = typename Subcontainer::size_type;
    using difference_type = typename Subcontainer::difference_type;

    
    // Construcción
    // ------------
    Row_iterator_base() {}

    Row_iterator_base(const I& p00, size_type cols0)
	: p0{p00}, cols{cols0}, sb{p00, cols0}
    {}


    explicit Row_iterator_base(const Row_iterator_base<I>& p)
	: p0{p.p0}, cols{p.cols}, sb{p.p0, p.cols} { }

    // convertimos Iterator to const_Iterator
    operator const_iterator() const
    { return const_iterator{p0, cols}; }


    friend
    bool operator==(const Row_iterator_base& a, 
		    const Row_iterator_base& b)
    { return a.p0 == b.p0 and a.cols == b.cols; }

    friend
    bool operator!=(const Row_iterator_base& a, 
		    const Row_iterator_base& b)
    {return !(a == b);}



    // For input iterator
    reference operator*() // const
    {
	sb = Subcontainer{p0, cols};
	return sb;
    }

    void operator++() { p0 += cols; }

    // For bidirectional iterator
    void operator--() { p0 -= cols; }

    // For random access iterator
    void operator+=(difference_type i) { p0 += i*cols; }

    difference_type operator-(const Row_iterator_base& b) const
    { return (p0 - b.p0)/narrow_cast<difference_type>(cols); }


    // datos
    I p0;
    size_type cols;	// columnas del contenedor 2D

    // Esto no pertenece al iterador. Sin embargo, *p tiene que devolver
    // una referencia al elemento que apunta así que tiene que existir en 
    // alguna parte. Idem si quiero hacer p->... Por eso necesitamos 
    // guardar aquí el subcontainer.
    Subcontainer sb;
};


// Iteradores mínimos Row_iterator_min. Versión const y no const.
template <typename I>
using Row_iterator_min =
    Row_iterator_base<typename alp::iterator_traits<I>::iterator>;

template <typename I>
using const_Row_iterator_min = 
    Row_iterator_base<typename alp::iterator_traits<I>::const_iterator>;


// Iteradores Row_iterator. Versión const y no const.
// Observar que si le pasamos a Row_iterator un const_iterator, el resultado
// es un const_Row_iterator.
template <typename It>
using Row_iterator = Iterator<Row_iterator_min<It>>;

template <typename It>
using const_Row_iterator = Iterator<const_Row_iterator_min<It>>;


/// Creamos un Row_iterator.
template <typename It>
inline Row_iterator<It>
	    row_iterator(It p0, typename Row_iterator_min<It>::size_type cols)
{
    return Row_iterator<It>{Row_iterator_min<It>{p0, cols}};
}


/// Creamos un const_Row_iterator.
template <typename It>
inline const_Row_iterator<It>
const_row_iterator(It p0, typename const_Row_iterator_min<It>::size_type cols)
{
    return const_Row_iterator<It>{const_Row_iterator_min<It>{p0, cols}};
}






// Iteradores mínimos Row_iterator_min. Versión const y no const.
template <typename It, typename View>
using Row_iterator_view_min =
    Row_iterator_base<View_iterator<It, View>>;

template <typename It, typename View>
using const_Row_iterator_view_min =
    Row_iterator_base<const_View_iterator<It, View>>;


// Iteradores Row_iterator. Versión const y no const.
template <typename It, typename View>
using Row_iterator_view = Iterator<Row_iterator_view_min<It, View>>;

template <typename It, typename View>
using const_Row_iterator_view = Iterator<const_Row_iterator_view_min<It, View>>;


/// Transforma el Row_iterator en un Row_iterator_view.
//
/// Por ejemplo:
///	Si queremos iterar por una imagen viendo solo sus colores red:
///
///	    Imagen img;
///	    for (auto f = row_iterator_view(img.row_begin(), Color_red{});
///		      f != row_iterator_view(img.row_end(), Color_red{}); ++f)
///	    ...
///
/// ¿Por qué no usar aquí imagen_red()? La ventaja de row_iterator_view es que
/// puedo usar algoritmos 2D directamente:
///	    alp::adjacent_difference(img.row_begin(), img.row_end());
//
// NOTA: si se le pasa como parámetro Row_iterator<It> falla la deducción
// automática, ya que 
// Row_iterator =
//	Iterator<Row_iterator_base<alp::iterator_traits<It>::iterator>;
//
// Parece ser que el que aparezca el iterator_traits<It>::iterator_traits 
// impide la deducción automática.  
//
template <typename It, typename View>
inline auto row_iterator_view(const Row_iterator<It>& p, View view)
{
    Row_iterator_min<It> rbase{p.it};
    View_iterator<It, View> view_it = view_iterator(rbase.p0, view);

    Row_iterator_view_min<It, View> row_view_min{view_it, rbase.cols};
    Row_iterator_view<It, View> row_view{row_view_min};

    return row_view;
}


template <typename It, typename View>
inline auto const_row_iterator_view(const const_Row_iterator<It>& p, View view)
{
    const_Row_iterator_min<It> rbase{p.it};
    const_View_iterator<It, View> view_it = const_view_iterator(rbase.p0, view);

    const_Row_iterator_view_min<It, View> row_view_min{view_it, rbase.cols};
    const_Row_iterator_view<It, View> row_view{row_view_min};

    return row_view;
}



/*!
 *  \brief  Iterador para concebir una submatriz como un contenedor
 *  \brief  unidimensional.
 *
 *  Observar que realmente es un iterador que itera sobre los elementos de un
 *  contenedor unidimensional, pero va iterando a saltos (de ahí el nombre)
 *
 *  x x x x x x x x x x x x x x x x x x x x x x x x
 *  - - - -       - - - -     - - - -     - - - - 
 *
 *  Itera sobre los elementos subrayados (que son los que forman la submatriz)
 *
 *  Observar que puesto que una columna es una submatriz de 1 columna este
 *  iterador sirve también para iterar por los elementos de una columna.
 *
 */
/***************************************************************************
 *		     
 *  Con este iterador tenemos el mismo problema que con el Row_iterator:
 *
 *		    ¿Cómo definir el Gap_iterator_base.end()?
 *		    -----------------------------------------
 *
 *  Supongamos que tenemos la matriz
 *
 *		    0	1   2	3
 *		    4	5   6	7   
 *		    8	9   a	b
 *
 *  y queremos definir la submatriz
 *		    5	6
 *		    9	a
 *  
 *  El iterator end() correspondería al iterador debajo del 9, iterador que no
 *  está definido!!! (el iterador end() de la matriz es el que está debajo del
 *  8). ¿Cómo definir el end?
 *
 *  En principio había elegido para implementar este iterador los siguientes
 *  datos:
 *	    It p0: iterador al primer elemento de la fila de la submatriz
 *	    size_type i: posición dentro de la fila de la submatriz 
 *							(estamos en p0 + i)
 *	    size_type size, cols;
 *
 *  El problema es que usando esta implementación el Gap_iterator_base.end() de la
 *  submatriz ejemplo tendría:
 *		p0 = iterador debajo del 9, ¡que no está definido!!!
 *
 *  Luego de esta forma no puedo definir Gap_iterator_base.end().
 *
 *  Pruebo a hacerlo de la siguiente forma:
 *	    It p0: iterador al primer elemento de la fila de la MATRIZ
 *	    size_type i: indice dentro de la fila de la MATRIZ
 *  Al usar coordenadas globales de la matriz, puedo definir
 *  Gap_iterator_base.end() como:
 *	    p0 = matrix.end()
 *	    i = 1 (en el ejemplo, este indice corresponde a la columna del 9)
 *
 ***************************************************************************/
template <typename I>
// requires: is_random_access_iterator(I)
struct Gap_iterator_base{
    //requires is_iterator<I>
    static_assert(alp::is_iterator_v<I>, "El tipo pasado I no es un iterador"); 

    using value_type      = typename std::iterator_traits<I>::value_type;
    using reference       = typename std::iterator_traits<I>::reference;
    using pointer         = typename std::iterator_traits<I>::pointer;
    using difference_type = typename std::iterator_traits<I>::difference_type;
    using iterator_category = std::random_access_iterator_tag;

    using Ind = difference_type;

    // Iteradores const y no const que suministra I
    using It       = typename alp::iterator_traits<I>::iterator;
    using const_It = typename alp::iterator_traits<I>::const_iterator;

    // Versiones const y no const de Iterator. Estas versiones son
    // fundamentales para la construcción correcta del iterador.
    using iterator       = Gap_iterator_base<It>;
    using const_iterator = Gap_iterator_base<const_It>;


    // Construcción
    // ------------
    Gap_iterator_base() {}


    Gap_iterator_base(const I& p0_, Ind j0_, Ind je_, Ind j_, Ind cols_)
        : p0{p0_}, j0{j0_}, je{je_}, j{j_}, cols{cols_}
    { }


    explicit Gap_iterator_base(const Gap_iterator_base<I>& it)
	: p0{it.p0}, j0{it.j0}, je{it.je}, j{it.j}, cols{it.cols} { }

    // convertimos Iterator to const_Iterator
    // explicit operator const_iterator() const
    operator const_iterator() const
    { return const_iterator{p0, j0, je, j, cols}; }


    friend
    bool operator==(const Gap_iterator_base& a, 
		    const Gap_iterator_base& b)
    {return a.p0 == b.p0 and a.j == b.j;}

    friend
    bool operator!=(const Gap_iterator_base& a, 
		    const Gap_iterator_base& b)
    {return !(a == b);}

    // For input iterator
    reference operator*() const {return *(p0 + j);}

    void operator++() 
    {
	++j;

	if (j == je){
	    p0 += cols;
	    j = j0;
	}
    }

    // For bidirectional iterator
    void operator--() 
    { 
	if (j > j0)  // i pudiera ser unsigned. Cuidado con underflow
	    --j;
	else{
	    p0 -= cols;
	    j = je - 1;
	}
    }

    // For random access iterator
    void operator+=(difference_type incr) 
    {
	auto size = je - j0;

	auto [q, r] = atd::div(incr, size);
	p0 += q*cols + r;
    }

    difference_type operator-(const Gap_iterator_base& b) const
    {
	difference_type k = (p0 - b.p0)/cols; // parte entera
	return k*(je - j0) + j - b.j;
    }


    // datos
    I p0;	// iterador al primer elemento de la fila en la MATRIZ
    Ind j0;	// indice del primer elemento de la submatriz
			// en coordenadas de matriz
    Ind je;	// [j0, je) índices de las columnas de la submatriz
			// en coordenadas de matriz.
    Ind j;	// indice del elemento actual al que apunta este iterador
		// Invariante: j pertenece [j0, je)
    Ind cols;	// columnas del contenedor 2D
};

// Iteradores mínimos
template <typename I>
using Gap_iterator_min = Gap_iterator_base<typename alp::iterator_traits<I>::iterator>;

template <typename I>
using const_Gap_iterator_min =
    Gap_iterator_base<typename alp::iterator_traits<I>::const_iterator>;

// Iteradores Submatrix_iterator (o gap_iterator)
template <typename It>
using Submatrix_iterator = Iterator<Gap_iterator_min<It>>;

template <typename It>
using const_Submatrix_iterator = Iterator<const_Gap_iterator_min<It>>;


/// uso: 
///	    It it0;
///	    auto p = submatrix_iterator(it0, i,sz, cols);
//
/// Todas las coordenadas son coordenadas globales de matriz.
/// p0 = iterador al primer elemento de la fila dentro de la MATRIZ 
/// [j0, je) = índices de las columnas de la submatriz (coord. de matriz)
/// j = indice de la columna actual a la que apunta el iterador (coord. de
///									matriz)
/// cols = columnas de la matriz
//
/// Ejemplo:
///		0 1 2 3 
//		4 5 6 7 
//		8 9 a b
//  El begin de la submatriz 5 6 9 a es:
//		submatrix_iterator(m.begin() + rows*1, 1, 3, 0, cols);
//  Siendo: rows, cols = las dimensiones de la matriz (4 x 4)
//	    j0 = 1, je = 3 (definen las columnas de la submatriz)
//	    j = 0 (en este caso el primer elemento de la submatriz)
template <typename It>
Submatrix_iterator<It> submatrix_iterator
    (const It& p0, typename Gap_iterator_base<It>::Ind j0
		 , typename Gap_iterator_base<It>::Ind je
		 , typename Gap_iterator_base<It>::Ind j
		 , typename Gap_iterator_base<It>::Ind cols)
{
    return Submatrix_iterator<It>{Gap_iterator_min<It>{p0, j0, je, j, cols}};
}


template <typename It>
const_Submatrix_iterator<It> const_submatrix_iterator
    (const It& p0, typename Gap_iterator_base<It>::Ind j0
		 , typename Gap_iterator_base<It>::Ind je
		 , typename Gap_iterator_base<It>::Ind j
		 , typename Gap_iterator_base<It>::Ind cols)
{
    return const_Submatrix_iterator<It>{
			    const_Gap_iterator_min<It>{p0, j0, je, j, cols}};
}

/// Todas las coordenadas son coordenadas globales de matriz.
/// p0 = iterador al primer elemento de la fila dentro de la MATRIZ 
/// [j0, je) = índices de las columnas de la submatriz
/// j = indice de la columna actual a la que apunta el iterador
/// cols = columnas de la matriz
/// Ejemplo:
///		0 1 2 3 
//		4 5 6 7 
//		8 9 a b
//  El begin de la submatriz 5 6 9 a es:
//		submatrix_iterator(m.begin() + rows*1, 1, 3, cols);
//  Siendo: rows, cols = las dimensiones de la matriz (4 x 4)
//	    j0 = 1, je = 3 (definen las columnas de la submatriz)
template <typename It>
Submatrix_iterator<It> submatrix_iterator
    (const It& p0, typename Gap_iterator_base<It>::Ind j0
		 , typename Gap_iterator_base<It>::Ind je
		 , typename Gap_iterator_base<It>::Ind cols)
{
    return submatrix_iterator(p0, j0, je, j0, cols);
}


template <typename It>
const_Submatrix_iterator<It> const_submatrix_iterator
    (const It& p0, typename Gap_iterator_base<It>::Ind j0
		 , typename Gap_iterator_base<It>::Ind je
		 , typename Gap_iterator_base<It>::Ind cols)
{
    return const_submatrix_iterator(p0, j0, je, j0, cols);
}




/*!
 *  \brief  Concebimos un contenedor unidimensional como una matriz
 *  transpuesta.
 */
template <typename I>
// requires is_iterator<I>
struct Transpose_iterator_base{
    //requires is_iterator<It>
    static_assert(alp::is_iterator_v<I>, "El tipo pasado I no es un iterador"); 

    // traits del iterador
    using value_type      = typename std::iterator_traits<I>::value_type;
    using reference       = typename std::iterator_traits<I>::reference;
    using pointer         = typename std::iterator_traits<I>::pointer;
    using difference_type = typename std::iterator_traits<I>::difference_type;
    using iterator_category = std::random_access_iterator_tag;

    using Ind = difference_type;

    // Iteradores const y no const que suministra I
    using It       = typename alp::iterator_traits<I>::iterator;
    using const_It = typename alp::iterator_traits<I>::const_iterator;

    // Versiones const y no const de Iterator. Estas versiones son
    // fundamentales para la construcción correcta del iterador.
    using iterator       = Transpose_iterator_base<It>;
    using const_iterator = Transpose_iterator_base<const_It>;


    // datos 
    I p0;
    Ind i, j;
    Ind rows, cols;



    // Construcción
    // ------------
    Transpose_iterator_base() {}

    Transpose_iterator_base(const It& p00, Ind i0, Ind j0, Ind rows0, Ind cols0)
        : p0{p00}, i{i0}, j{j0}, rows{rows0}, cols{cols0}
    { }

    explicit Transpose_iterator_base(const Transpose_iterator_base<I>& it)
	: p0{it.p0}, i{it.i}, j{it.j}, rows{it.rows}, cols{it.cols} { }

    // convertimos Iterator to const_Iterator
    // explicit operator const_iterator() const
    operator const_iterator() const
    { return const_iterator{p0, i, j, rows, cols}; }


    friend
    bool operator==(const Transpose_iterator_base& a, 
		    const Transpose_iterator_base& b)
    { return a.p0 == b.p0 and a.i == b.i and a.j == b.j;}

    friend
    bool operator!=(const Transpose_iterator_base& a, 
		    const Transpose_iterator_base& b)
    {return !(a == b);}


    // for input iterator
    reference operator*() const {return *(p0 + i*cols + j);}

    void operator++() 
    {
	++i;
	if (i >= rows){
	    ++j;
	    i = 0;
	}
    }

    // for bidirectional iterator
    void operator--() 
    {
	if (i > 0)
	    --i;
	else {
	    i = cols - 1;
	    --j;
	}
    }

    bool is_end() const { return i == 0 and j == cols;}


    // for random access iterator
    // Como el índice unidimensional es: n = j*rows + i
    // Si queremos añadirle incr:
    //	    n + incr = j*rows + i + incr
    //	Descomponiendo incr = incr_j*rows + incr_i, queda
    //	    n + incr = (j + incr_j)* rows + (i + incr_i)
    void operator+=(difference_type incr) 
    {
	auto [incr_j, incr_i] = atd::div(incr, rows);
	i += incr_i;
	j += incr_j;
    }

    difference_type operator-(const Transpose_iterator_base& b) const
    { return (j - b.j)*rows + (i - b.i); }

};

// Iteradores minimos.
template <typename I>
using Transpose_iterator_min =
    Transpose_iterator_base<typename alp::iterator_traits<I>::iterator>;

template <typename I>
using const_Transpose_iterator_min =
    Transpose_iterator_base<typename alp::iterator_traits<I>::const_iterator>;


// Iteradores Transpose_iterator
template <typename It>
using Transpose_iterator = Iterator<Transpose_iterator_min<It>>;

template <typename It>
using const_Transpose_iterator = Iterator<const_Transpose_iterator_min<It>>;



// Devuelve un Transpose_iterator a la matriz M de dimensiones rows x cols
// (M es la matriz que queremos transponer).
//
// p = iterador al elemento inicial
// i = fila donde se encuentra ese iterador (en la matriz M)
// rows x cols = dimensiones de la matriz M
template <typename It>
Transpose_iterator<It> transpose_iterator(It p, 
		    typename Transpose_iterator_base<It>::Ind i, 
		    typename Transpose_iterator_base<It>::Ind j, 
		    typename Transpose_iterator_base<It>::Ind rows, 
		    typename Transpose_iterator_base<It>::Ind cols)
{
    return Transpose_iterator<It>{
			Transpose_iterator_min<It>{p, i, j, rows, cols}};
}


// p0 = iterador al elemento inicial
// rows x cols = dimensiones de la matriz M
template <typename It>
Transpose_iterator<It> transpose_iterator_begin(It p0, 
		    typename Transpose_iterator_base<It>::Ind rows, 
		    typename Transpose_iterator_base<It>::Ind cols)
{
    using Ind = typename Transpose_iterator_base<It>::Ind;

    return Transpose_iterator<It>{
        Transpose_iterator_min<It>{p0, Ind{0}, Ind{0}, rows, cols}};
}


// p0 = iterador al elemento inicial
// rows x cols = dimensiones de la matriz M
template <typename It>
Transpose_iterator<It> transpose_iterator_end(It p0, 
		    typename Transpose_iterator_base<It>::Ind rows, 
		    typename Transpose_iterator_base<It>::Ind cols)
{
    using Ind = typename Transpose_iterator_base<It>::Ind;

    return Transpose_iterator<It>{
		    Transpose_iterator_min<It>{p0, Ind{0}, cols, rows, cols}};
}

} // namespace alp



#endif


