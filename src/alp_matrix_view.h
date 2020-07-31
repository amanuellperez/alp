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

#ifndef __ALP_MATRIX_VIEW_H__
#define __ALP_MATRIX_VIEW_H__
/****************************************************************************
 *
 *   - DESCRIPCION: View de una matriz.
 *
 *   - HISTORIA:
 *           alp  - 23/03/2019 Escrito
 *
 ****************************************************************************/

#include <iterator> 
#include "alp_iterator.h"
#include "alp_indices.h"
#include "alp_subcontainer.h"
#include "alp_matrix_iterator.h"
#include "alp_type_traits.h"

namespace alp{

/*!
 *  \brief  View de una matriz.
 *
 */
template <typename It>
// requires It = random access iterator
class Matrix_view{
public:
    // Objetos que contiene
    using value_type	  = typename std::iterator_traits<It>::value_type;
    using reference       = typename std::iterator_traits<It>::reference;
    using const_reference = const reference;
    using pointer         = typename std::iterator_traits<It>::pointer;
    using const_pointer   = const pointer;

    // Indices unidimensionales
    using size_type       = typename std::iterator_traits<It>::difference_type;
    using difference_type = size_type;

    // Tipos asociados a las coordenadas (i, j)
    using Ind	      = difference_type;

    // Tipos asociados a la matrix
    // using Posicion  = alp::Posicion_ij<Ind>;  // Indice 2D
    using Posicion  = alp::Vector_ij<Ind>;  // Indice 2D
    using Size2D    = alp::Size_ij<Ind>;      
    using Rango2D   = alp::Rango_ij<Ind>;

    // Iteradores unidimensionales
    using iterator       = It;
    using const_iterator = typename alp::iterator_traits<It>::const_iterator;

    static constexpr bool is_const = std::is_same_v<iterator, const_iterator>;

    // --------------------------
    // Iteradores bidimensionales
    // --------------------------
    // Iteramos por filas
    // ------------------
    using Row       = Subcontainer<iterator>;
    using const_Row = Subcontainer<const_iterator>;

    using row_iterator       = Row_iterator<iterator>;
    using const_row_iterator = const_Row_iterator<const_iterator>;


    // ------------
    // Construcción
    // ------------
    /// La matriz es una view del contenedor unidimensional [p0, size)
    /// con size = rows * cols. p0 = primer elemento de la matriz_view.
    // precondición: [p0, size) con size = rows * cols
    Matrix_view(It p0, Ind rows, Ind cols)
	:p0_{p0}, rows_{rows}, cols_{cols} {}

    Matrix_view(It p0, const Size2D& sz)
	:Matrix_view(p0, sz.rows, sz.cols) {}

    // No defino operator==. Es un operador costoso y podría pasar
    // desapercibido su coste.

    // -----------
    // Dimensiones
    // -----------
    /// Número de elementos que tiene la matrix almacenados
    size_type size() const {return rows()*cols();}

    /// Número de filas
    Ind rows() const {return rows_;}

    /// Número de columnas
    Ind cols() const {return cols_;}

    /// Dimensiones de la matrix
    Size2D size2D() const {return Size2D{rows(), cols()};}

    /// Extensión que ocupa dentro del sistema de referencia (i, j)
    Rango2D extension() const {return Rango2D{0, rows(), 0, cols()};}


    // -------------------------------------
    // Acceso como contenedor unidimensional
    // -------------------------------------
    iterator begin() {return p0();}
    iterator end() {return pe();}
    iterator it(Ind i, Ind j) {return p0() + indice(i, j);}

    const_iterator begin() const {return p0();}
    const_iterator end() const {return pe();}
    const_iterator it(Ind i, Ind j) const {return p0() + indice(i, j);}

    const_iterator cbegin() const {return p0();}
    const_iterator cend() const {return pe();}
    const_iterator cit(Ind i, Ind j) const {return p0() + indice(i, j);}

    // ----------------
    // Acceso aleatorio
    // ----------------
    reference operator()(Ind i, Ind j) {return *(p0() + indice(i,j));}
    const_reference operator()(Ind i, Ind j) const
    { return *(p0() + indice(i, j)); }

    reference operator()(const Posicion& p) {return (*this)(p.i, p.j);}
    const_reference operator()(const Posicion& p) const
    { return (*this)(p.i, p.j); }

    // -------------------
    // Iteración por filas
    // -------------------
    /// Devuelve la fila i
    Row row(Ind i) 
    { return Row{p0() + indice(i, 0), p0() + indice(i + 1, 0)}; }

    /// Devuelve la fila i
    const_Row row(Ind i) const
    { return const_Row{p0() + indice(i, 0), p0() + indice(i + 1, 0)}; }

    auto row_begin()
    {
	if constexpr (is_const)
	    return alp::const_row_iterator(begin(), cols()); 

	else
	    return alp::row_iterator(begin(), cols()); 
    }

    auto row_end() 
    { 
	if constexpr (is_const)
	    return alp::const_row_iterator(end(), cols()); 

	else
	    return alp::row_iterator(end(), cols()); 
    }

    const_row_iterator row_begin() const
    {return alp::const_row_iterator(begin(), cols());}

    const_row_iterator row_end() const
    { return alp::const_row_iterator(end(), cols()); }

    const_row_iterator row_cbegin() const {return row_begin();}
    const_row_iterator row_cend() const {return row_end();}


private:
    iterator p0_;

    Ind rows_; // número de filas
    Ind cols_; // número de columnas

    // Funciones de ayuda
    // Devuelve el índice unidimensional n correspondiente (i,j)
    Ind indice(Ind i, Ind j) const { return cols() * i + j; }

    // La única diferencia entre Matrix y Matrix_view está en estas dos
    // funciones.
    iterator p0() {return p0_;}
    iterator pe() {return p0_ + size();}

    const_iterator p0() const {return p0_;}
    const_iterator pe() const {return p0_ + size();}
};


// syntactic sugar
// TODO: cuando elimine Submatrix, recuperar esta función
/// Crea una submatriz
//template <typename It, typename Int>
//inline Matrix_view<Submatrix_iterator<It>> submatrix(It begin, // iterador al primer elemento de la fila de la submatriz
//				Int j0,	    // indice de la primera columna de la submatriz dentro de la matriz
//				Int submatrix_rows, 
//				Int submatrix_cols,
//				Int matrix_cols)
//{
//    auto p0 = submatrix_iterator(begin, j0, j0 + submatrix_cols, matrix_cols);
//    return {p0, submatrix_rows, submatrix_cols};
//}


/// La matriz es [begin, rows, cols)
template <typename It, typename Int>
inline Matrix_view<Transpose_iterator<It>> transpose_matrix(It begin,
						     Int rows, Int cols)
{ 
    auto p0 = transpose_iterator_begin(begin, cols, rows); 
    return {p0, rows, cols};
}

/// La matriz es [begin, rows, cols)
template <typename It, typename View, typename Int>
inline Matrix_view<View_iterator<It, View>> matrix_view(It begin,
							Int rows, Int cols, 
							View view)
{
    auto p0 = view_iterator(begin, view);
    return Matrix_view<View_iterator<It, View>> {p0, rows, cols};
}
	

/// La matriz es [begin, rows, cols)
template <typename It, typename View, typename Int>
inline Matrix_view<const_View_iterator<It, View>> const_matrix_view(It begin,
							Int rows, Int cols, 
							View view)
{
    auto p0 = const_view_iterator(begin, view);
    return Matrix_view<const_View_iterator<It, View>> {p0, rows, cols};
}


}// namespace

#endif
