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

#ifndef __ALP_MATRIX_H__
#define __ALP_MATRIX_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Matrix = contenedor bidimensional.
 *
 *   - COMENTARIOS: Aunque he tardado en decidir el nombre
 *   (Contenedor_bidimensional, Contbi, Matrix) creo que el más sencillo (por
 *   breve y conocido) es matrix.
 *
 *   Realmente no es una matrix, sino un contenedor bidimensional, pero
 *   std::vector tampoco es un vector y no pasa nada.
 *
 *  
 *	Concepto: contenedor bidimensional
 *	----------------------------------
 *	Un contenedor bidimensional tiene unas dimensiones (rows, cols)
 *	y una forma de acceder a cada elemento. El contenedor bidimensional
 *	no sabe NADA de los elementos que contiene ==> implementar Imagen
 *	como contenedor bidimensional: no sabe nada de ColorRGB!!!
 *
 *	Formas de acceder a los elementos:
 *	    + acceso aleatorio: m(i,j);
 *	    + como contenedor unidimensional: for(auto x: m) ...
 *	    + como contenedor bidimensional:
 *		a) Por filas: 
 *			for(auto f= m.row_begin(); f!= m.row_end(); ++f)
 *				...
 *
 *   - HISTORIA:
 *    Manuel Perez
 *      25/08/2017 Escrito
 *      02/08/2020 Corners: upper_left_corner,...
 *
 ****************************************************************************/

#include <iterator> // bidirectional_iterator_tag
#include <cstring>  // memcpy
#include <algorithm> // copy

#include "alp_concepts.h"
#include "alp_exception.h"
#include "alp_cast.h"
#include "alp_rframe_ij.h"
#include "alp_subcontainer.h"
#include "alp_matrix_iterator.h"

namespace alp{

/*!
 *  \brief  Matrix. Es un contenedor bidimensional.
 *
 *  El nombre más correcto sería Matrix_ij, para diferenciarlo de Matrix_xy.
 *  La primera usa coordenadas (i, j) mientras que la segunda coordenadas 
 *  (x, y). Sin embargo, como en matemáticas las coordenadas usadas son (i, j)
 *  por defecto llamamos Matrix a Matrix_ij.
 *
 *  Aunque quedaría mejor llamarlo Contenedor_bidimensional, el nombre es muy
 *  largo, así que opto por llamarlo Matrix. No es del todo correcto ya que
 *  esto es un contenedor donde, en principio, no hay definida el álgebra de
 *  matrices. Pero en la STL se llama vector a un contenedor unidimensional
 *  que no tiene definidas las operaciones de espacio vectorial.
 *
 *  En principio no tendría por qué ser necesario pasarle el Size_t. Dos
 *  motivos por el que lo paso:
 *	1.- A día de hoy Imagen está usando 'int' como Size_t, y si quiero
 *	    usar esta matrix en los mapas, necesito que el índice sea tipo
 *	    int, no size_t.
 *
 *	2.- Stepanov define así una estructura indicando que si buscas
 *	eficiencia, y sabes que tu matrix va a ser pequeña, ¿para qué usar 64
 *	bits para índices cuando puedes usar 32? Pensar que una imagen como
 *	mucho será 1000 x 1000 = 1 M que entra de sobra en uint32_t. Los mapas
 *	serán Matrix<T, int32_t> ('int' porque uso int en las imágenes)
 */
template <typename T, typename Ind_t = size_t>
class Matrix{
public:
    // Objetos que contiene
    using value_type	  = T;
    using reference       = T&;
    using const_reference = const T&;
    using pointer         = T*;
    using const_pointer   = const T*;

    // Indices unidimensionales
    using size_type       = Ind_t;
    using difference_type = std::make_signed_t<Ind_t>;

    // Tipos asociados a las coordenadas (i, j)
    using Ind	      = Ind_t;	    // indice 

    // Tipos asociados a la matrix
    // using Position  = alp::Position_ij<Ind>;  // Indice 2D
    using Position  = Vector_ij<Ind>; 
    using Size2D    = alp::Size_ij<Ind>;      
    using Range2D   = alp::Range_ij<Ind>;

    // Iteradores unidimensionales
    using iterator       = T*;
    using const_iterator = const T*;

    static_assert(!std::is_same_v<iterator, const_iterator>,
                  "Matrix::iterator == const_iterator! Error grave");

    //    using transpose_iterator       = Transpose_iterator<iterator>;
    //    using const_transpose_iterator = Transpose_iterator<const_iterator>;

    // --------------------------
    // Iteradores bidimensionales
    // --------------------------
    // Iteramos por filas
    // ------------------
    using Row       = Subcontainer<iterator>;
    using const_Row = Subcontainer<const_iterator>;

    using row_iterator       = Row_iterator<iterator>;
    using const_row_iterator = const_Row_iterator<const_iterator>;

    // Construcción
    // ------------
    /// Matrix de rows x cols sin inicializar
    /// Este constructor permite intercambiar dimensiones entre distintas
    /// matrices: por ejemplo, si queremos crear un Matrix<int> de las mismas
    /// dimensiones que una Imagen, lo siguiente no compila:
    ///		Matrix<int> m{img0.size2D()};
    /// ya que los tipos Size2D de matrix e imagen no tienen por qué ser
    /// los mismos. (no compila cuando Ind_t es diferente. Si al final acabo
    /// definiendo siempre size_t = difference_type entonces compilaría)
    Matrix(Ind rows, Ind cols);

    /// Matrix de rows x cols sin inicializar
    Matrix(Size2D sz):Matrix{sz.rows, sz.cols}{}
 
    Matrix(const Matrix& m);
    Matrix& operator=(const Matrix& m);	

    Matrix(Matrix&& m);
    Matrix& operator=(Matrix&& m);

    /// Construye la matriz a partir de las filas de otra matriz [f0, fe)
    Matrix(const_row_iterator f0, const_row_iterator fe);

    ~Matrix() {delete[] p0_;}

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

    /// Extensión que ocupa dentro del sistema de referencia local (i, j)
    Range2D extension() const {return Range2D{0, rows(), 0, cols()};}


    // -------------------------------------
    // Acceso como contenedor unidimensional
    // -------------------------------------
    iterator begin() {return p0();}
    iterator end() {return pe();}
    iterator it(Ind i, Ind j) {return p0() + indice(i, j);}
    iterator it(const Position& p) {return it(p.i, p.j);}

    const_iterator begin() const {return p0();}
    const_iterator end() const {return pe();}
    const_iterator it(Ind i, Ind j) const {return p0() + indice(i, j);}
    const_iterator it(const Position& p) const {return it(p.i, p.j);}

    const_iterator cbegin() const {return p0();}
    const_iterator cend() const {return pe();}
    const_iterator cit(Ind i, Ind j) const {return p0() + indice(i, j);}
    const_iterator cit(const Position& p) const {return cit(p.i, p.j);}


    // ----------------
    // Acceso aleatorio
    // ----------------
    value_type& operator()(Ind i, Ind j) {return *(p0() + indice(i,j));}
    const value_type& operator()(Ind i, Ind j) const  {return *(p0() + indice(i,j));}

    value_type& operator()(const Position& p) {return (*this)(p.i, p.j);}
    const value_type& operator()(const Position& p) const  {return (*this)(p.i, p.j);}


    // -------------------
    // Iteración por filas
    // -------------------
    /// Devuelve la fila i
    Row row(Ind i)
    { return Row{p0() + indice(i,0), p0() + indice(i+1,0)}; }

    /// Devuelve la fila i
    const_Row row(Ind i) const
    { return const_Row{p0() + indice(i,0), p0() + indice(i+1,0)}; }

    row_iterator row_begin() 
    {return alp::row_iterator(begin(), cols());}

    row_iterator row_end() 
    { return alp::row_iterator(end(), cols()); }

    const_row_iterator row_begin() const
    {return alp::const_row_iterator(begin(), cols());}

    const_row_iterator row_end() const
    { return alp::const_row_iterator(end(), cols()); }

    const_row_iterator row_cbegin() const {return row_begin();}
    const_row_iterator row_cend() const {return row_end();}


//    // ----------------------
//    // Iteración por columnas
//    // ----------------------
//    /// Devuelve la columna j
//    Col col(Ind j)
//    {
//        return Col{
//            alp::transpose_iterator(transpose_begin(), Ind{0}, j, rows(), cols()),
//            alp::transpose_iterator(transpose_begin(), Ind{0}, j + 1, rows(), cols())
//		  };
//    }
//
//    /// Devuelve la columna j
//    const_Col col(Ind j) const
//    {
//        return const_Col{
//            alp::transpose_iterator(transpose_begin(), Ind{0}, j, rows(), cols()),
//            alp::transpose_iterator(transpose_begin(), Ind{0}, j + 1, rows(), cols())
//		  };
//    }
//
//
//    col_iterator col_begin()
//    { return alp::row_iterator(transpose_begin(), transpose_end(), rows());}
//
//    col_iterator col_end() 
//    { return alp::row_iterator_end(transpose_end(), rows());}
//
//    const_col_iterator col_begin() const
//    { return alp::row_iterator(transpose_begin(), transpose_end(), rows());}
//
//    const_col_iterator col_end() const
//    { return alp::row_iterator_end(transpose_end(), rows());}
//
//    const_col_iterator col_cbegin() const { return col_begin(); }
//    const_col_iterator col_cend() const { return col_end(); }

private:
    // Es propietario del contenido de este puntero
    value_type* p0_ = nullptr;

    Ind rows_ = 0; // número de filas
    Ind cols_ = 0; // número de columnas

    // Funciones de ayuda
    // Devuelve el índice unidimensional k correspondiente (i,j)
    Ind indice(Ind i, Ind j) const { return cols() * i + j; }

    // La única diferencia entre Matrix y Matrix_view está en estas dos
    // funciones.
    iterator p0() {return p0_;}
    iterator pe() {return p0_ + size();}

    const_iterator p0() const {return p0_;}
    const_iterator pe() const {return p0_ + size();}
};



// Matrix de rows x cols sin inicializar
template <typename T, typename Size_t>
inline Matrix<T, Size_t>::Matrix(Size_t rows, Size_t cols)
    :p0_{new T[rows*cols]}, rows_{rows}, cols_{cols}
{}

template <typename T, typename S>
inline Matrix<T, S>::Matrix(const Matrix& a)
    :p0_{new T[a.size()]}, rows_{a.rows_}, cols_{a.cols_}
{// Stroustrup usa uninitialized_copy (???)
    std::copy(a.p0_, a.p0_ + a.size(), p0_);
}


template <typename T, typename S>
inline Matrix<T, S>& Matrix<T, S>::operator=(const Matrix& a)
{
    precondicion((a.rows_ == rows_) and (a.cols_ == cols_)
		 , __FILE__, __LINE__,
                 "Matrix::operator=(const Matrix&)",
                 "Intentando copiar matrices de diferente tamaño");

//    std::copy(a.p0_, a.p0_ + a.size(), p0_);
    std::memcpy(p0_, a.p0_, a.size()*sizeof(T));

    return *this;
}


template <typename T, typename S>
inline Matrix<T, S>::Matrix(Matrix&& a)
    :p0_{a.p0_}, rows_{a.rows_}, cols_{a.cols_}
{
    a.p0_    = nullptr;
    a.rows_ = 0;
    a.cols_ = 0;
}


template <typename T, typename S>
inline Matrix<T, S>& Matrix<T, S>::operator=(Matrix&& a)
{   // el destructor de a, se encargará de liberar la memoria
    std::swap(p0_, a.p0_);
    std::swap(rows_, a.rows_);
    std::swap(cols_, a.cols_);

    return *this;
}


// Construye la matriz a partir de las filas de otra matriz [f0, fe)
// [ expects: f0->size() == fe->size() and fe - f0 definido ]
template <typename T, typename S>
Matrix<T, S>::Matrix(const_row_iterator f0, const_row_iterator fe)
    :Matrix{narrow_cast<Ind>(fe - f0), narrow_cast<Ind>(f0->size())}
{
    auto g = row_begin();

    for (; f0 != fe; ++f0, ++g)
	std::copy(f0->begin(), f0->end(), g->begin());
}


// Corners (en coordenadas de matriz)
// ----------------------------------
template <typename T, typename I>
Matrix<T,I>::Position upper_left_corner(const Matrix<T,I>& m)
{return {0,0};}


template <typename T, typename I>
Matrix<T,I>::Position upper_right_corner(const Matrix<T,I>& m)
{return {0, m.cols() - 1};}

template <typename T, typename I>
Matrix<T,I>::Position bottom_left_corner(const Matrix<T,I>& m)
{return {m.rows() - 1, 0};}

template <typename T, typename I>
Matrix<T,I>::Position bottom_right_corner(const Matrix<T,I>& m)
{return {m.rows() - 1, m.cols() - 1};}


}// namespace

#endif
