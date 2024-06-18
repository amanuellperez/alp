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

#ifndef __ALP_MATRIX_VIEW_H__
/****************************************************************************
 *
 *   - DESCRIPCION: View de una matriz.
 *
 *   - HISTORIA:
 *    Manuel Perez
 *       23/03/2019 Escrito
 *       28/11/2020 Migro Matrix_xy de Imagen_xy y añado sentido de ejes.
 *
 ****************************************************************************/

#include <iterator> 
#include "alp_iterator.h"
#include "alp_rframe_ij.h"
#include "alp_subcontainer.h"
#include "alp_matrix_iterator.h"
#include "alp_type_traits.h"
#include "alp_math.h"
#include "alp_rframe_xy.h"
#include "alp_matrix.h"

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
    using Position  = alp::Vector_ij<Ind>;  // Indice 2D
    using Size2D    = alp::Size_ij<Ind>;      
    using Range2D   = alp::Range_ij<Ind>;

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
    Range2D extension() const {return Range2D{0, rows(), 0, cols()};}


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

    reference operator()(const Position& p) {return (*this)(p.i, p.j);}
    const_reference operator()(const Position& p) const
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


/***************************************************************************
 *				Matrix_xy
 ***************************************************************************/
/*!
 *  \brief Iteramos por una matriz usando sistema de referencia {x,y}
 *
 *  En muchas ocasiones es mucho más natural usar como coordenadas un sistema
 *  de coordenadas cartesiano {x,y}. Esta view suministra dicho interfaz.
 *
 *  El sistema de referencia xy que se usa tendrá como origen de coordenadas
 *  el punto (i0, j0) (en coordenadas de matriz) y estará orientado de forma
 *  normal (el eje x hacia la derecha y el eje y hacia arriba).
 *
 *  Voy a permitir cambiar de coordenadas (x,y) a (i,j), y dar acceso a la
 *  matriz desde esta clase para poder reutilizar algoritmos de dibujo.
 *  Imaginar que creo una función draw_circle(Matrix); ¿cómo definir la
 *  equivalente para Matrix_xy sin hacerla friend? Basta con saber cómo
 *  convertir del sistema de coordenadas xy a ij y tener acceso a la imagen
 *  para poder reescribir cualquier función definida usando Matrix para
 *  Matrix_xy.
 *
 */
template <typename Matrix_type, int x_sign, int y_sign>
class Matrix_xy_base{
public:
    // Objetos que contiene
    using value_type      = alp::Value_type<Matrix_type>;
    using reference       = alp::Reference_type<Matrix_type>;
    using const_reference = typename Matrix_type::const_reference;
    using pointer         = alp::Pointer_type<Matrix_type>;
    using const_pointer   = typename Matrix_type::const_pointer;

    // Indices unidimensionales
    using size_type       = typename Matrix_type::size_type;
    using difference_type = typename Matrix_type::difference_type;

    // Tipos que definen los contenedores bidimensionales
    using Ind           = typename Matrix_type::Ind;
    using Position	= typename Matrix_type::Position;
    using Size2D	= typename Matrix_type::Size2D;

    // Propios
    using Point	    = alp::Vector_xy<Ind>;
    using Vector    = alp::Vector_xy<Ind>;


    /// Colocamos una máscara sobre la imagen img0. Por defecto el origen de
    /// coordenadas esta en la esquina inferior izquierda de la imagen.
    Matrix_xy_base(Matrix_type& img0, Ind i0 = - 1, Ind j0 = -1)
	    :img_{&img0}
    {
	if (i0 != -1)
	    i0_ = i0;
	else
	    i0_ = img0.rows() - 1;

	if (j0 != -1)
	    j0_ = j0;
	else 
	    j0_ = 0;
    }

    /// Fijamos el origen de coordenadas. Después de la llamada de esta
    /// función cualquier acceso usará este nuevo origen.
    void origen_de_coordenadas(Ind i0, Ind j0);

    void origen_de_coordenadas(const Position& p0)
    {origen_de_coordenadas(p0.i, p0.j);}

    Position origen_de_coordenadas() const
    { return Position{i0_, j0_};}

    /// Colocamos el origen de coordenadas en el centro de la imagen
    void origen_de_coordenadas_en_el_centro()
    { origen_de_coordenadas(posicion_del_centro(img_->extension())); }
    
    /// Valor mínimo de x
    Ind x_min() const 
    {
	if constexpr (x_sign > 0)
	    return x(0);
	else 
	    return x(cols() - 1);
    }

    /// Valor máximo que puede tener x (img(x_max()) es válido).
    Ind x_max() const 
    {
	if constexpr (x_sign > 0)
	    return x(cols() - 1);
	else 
	    return x(0);
    }

    /// Valor mínimo de y
    Ind y_min() const 
    {
	if constexpr (y_sign > 0)
	    return y(rows() - 1);
	else
	    return y(0);
    }

    /// Valor máximo de y
    Ind y_max() const 
    {
	if constexpr (y_sign > 0)
	    return y(0);
	else
	    return y(rows() - 1);
    }


    /// Número de filas
    Ind rows() const {return img_->rows();}

    /// Número de columnas
    Ind cols() const {return img_->cols();}

    /// Devuelve el color del pixel (x,y)
    /// OJO: se da por supuesto que el usuario sabe dónde está accediendo.
    /// Es responsabilidad del usuario validar que los puntos (x, y) sean
    /// puntos válidos de la matriz.
    reference operator()(Ind x, Ind y) {return (*img_)(posicion(x,y));}

    /// Devuelve el color del pixel (x,y)
    /// OJO: se da por supuesto que el usuario sabe dónde está accediendo.
    /// Es responsabilidad del usuario validar que los puntos (x, y) sean
    /// puntos válidos de la matriz.
    const_reference operator()(Ind x, Ind y) const
    {return (*img_)(posicion(x,y));}


// Cambio de sistema de referencia
// -------------------------------
    /// Devuelve la posición (i,j) a la que apunta el punto de coordenadas (x,y)
    Position posicion(Ind x, Ind y) const
    { return {i(y), j(x)};}

    /// Devuelve la posición (i,j) a la que apunta el punto p
    Position posicion(const Point& p) const
    {return posicion(p.x, p.y);}


    /// Cambia la coordenada x en la coordenada j correspondiente
    Ind j(Ind x) const 
    { 
	if constexpr (x_sign > 0)
	    return (j0_ + x); 
	else 
	    return (j0_ - x); 
    }

    /// Cambia la coordenada y en la coordenada i correspondiente
    Ind i(Ind y) const 
    { 
	if constexpr (y_sign > 0)
	    return (i0_ - y);
	else 
	    return (i0_ + y);
    }

    /// Pasamos de j a x (j --> x)
    Ind x(Ind j) const 
    { 
	if constexpr (x_sign > 0)
	    return (j - j0_);
	else 
	    return (j0_ - j);
    }

    /// Pasamos de i a y (i --> y)
    Ind y(Ind i) const 
    { 
	if constexpr (y_sign > 0)
	    return (i0_ - i);
	else
	    return (i - i0_);
    }


    /// Acceso a la matriz 
    Matrix_type& matrix() {return *img_;}

    /// Acceso a la matriz
    const Matrix_type& matrix() const {return *img_;}

private:
    Matrix_type* img_;

    // Coordenadas de matriz (i, j) del origen del sistema de referencia.
    Ind i0_, j0_;

};


// Fijamos el origen de coordenadas. Después de la llamada de esta
// función cualquier acceso usará este nuevo origen.
template <typename It, int xs, int ys>
inline void Matrix_xy_base<It,xs,ys>::origen_de_coordenadas(Ind i0, Ind j0)
{
    i0_ = i0;
    j0_ = j0;
}


// -----
// Alias
// -----
// El signo + indica que x es hacia la derecha, y que el eje y es hacia
// arriba. (convención habitual)
template <typename T, typename Ind_t, int x_sign = +1, int y_sign = +1>
using Matrix_xy = Matrix_xy_base<Matrix<T, Ind_t>, x_sign, y_sign>;

template <typename T, typename Ind_t, int x_sign = +1, int y_sign = +1>
using const_Matrix_xy = Matrix_xy_base<const Matrix<T, Ind_t>, x_sign, y_sign>;


}// namespace

#endif
