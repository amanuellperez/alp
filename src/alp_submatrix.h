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

#ifndef __ALP_SUBMATRIX_H__
#define __ALP_SUBMATRIX_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Es una view de un contenedor 2D.
 *	    Nos permite ver solo una parte del contenedor como si fuera
 *	otro contenedor.
 *
 *   - COMENTARIOS: 
 *	    Posibles formas de poner la view.
 *		Subcontainer2D<Imagen> r;   // es de lectura/escritura
 *		Subcontainer2D<const Imagen> r;	// solo lectura
 *		const Subcontainer2D<Imagen> r;	// solo lectura
 *		const Subcontainer2D<const Imagen> r;	// solo lectura
 *
 *   - TODO: No tiene ningún sentido que las funciones de mueve... esten
 *   dentro del subcontainer. Sacarlas fuera dejando la implementación del
 *   subcontainer mínima y eliminar esta clase. La submatrix realmente es
 *	    
 *	    using Submatrix = Matrix_view<Submatrix_iterator>!!! 
 *
 *	    Más sencillo de mantener!!!
 *   
 *   - IMPLEMENTACION:
 *	En principio esta clase tendría que llamarse Subcontainer2D. De hecho
 *	así la implementé incialmente. El problema es que al crear un alias
 *	Submatrix no deducía automáticamente el tipo. Hay una propuesta en C++
 *	para que eso funcione pero todavía no es más que una propuesta. Si en
 *	un futuro se añade, crear Subcontainer2D. Sino prefiero manejar
 *	Submatrix.
 *
 *   - FORMA DE USO:
 *	Las máscaras son contenedores bidimensionales, así que su uso
 *	es el habitual. La única diferencia es cómo crearlas.
 *
 *	+ Región:
 *	  ------
 *	    (const) Imagen img;
 *	    Subcontainer2D<Imagen> r{img, I0, J0, rows, cols};	// lectura-escritura
 *	    // (I0, J0) = esquina superior izquierda de la región
 *	    const Subcontainer2D<Imagen> r{img, I0, J0, rows, cols};	  // lectura
 *	    const Subcontainer2D<const Imagen> r{img, I0, J0, rows, cols};// lectura
 *
 *	    auto r = submatrix(img, I0, J0, rows, cols);
 *
 *	    OJO: las submatrices solo se pueden aplicar a Imagenes, ya que
 *	    los iteradores devuelven 'value_type&' para poderlo modificar
 *	    el color.
 *
 *
 *   - HISTORIA:
 *     Manuel Perez - 05/07/2016 Escrito
 *			30/12/2017 Generalizado (de img a alp)
 *			28/02/2019 Reescrito y generalizado. 
 *				   Convierto submatrix en un rango_acotado.
 *
 ****************************************************************************/
#include "alp_matrix_iterator.h"
#include "alp_rframe_ij.h"

#include <type_traits>	// remove_const
#include "alp_matrix.h"	// quitarlo al mover clone de aquí


namespace alp{

/*!
 *  \brief  Es una view de una matrix. Muestra una región.
 *
 *   - CONCEPTO: Contenedor bidimensional.
 *
 *   - COMENTARIOS:
 *	Usaré 2 tipos de índices: 
 *	    + (I,J) = referentes al contenedor
 *	    + (i,j) = referentes a la región
 *
 *	Para crear una región, en lugar del constructor, mejor usar la función
 *	auxiliar 'submatrix' que deduce de forma automática los parámetros de 
 *	la template:
 *		auto r = submatrix(img, 2, 3, 4, 5);	
 *
 *	+ reference:
 *	  --------------
 *	  Necesito este tipo porque no sé qué es lo que devuelve c(i,j).
 *	  Si el contenedor es una Imagen, c(i,j) devuelve una referencia, 
 *	  pero si es un grid devuelve una submatrix (devuelve por valor, no por
 *	  referencia!!!). Este tipo resuelve este problema.
 *
 *  TODO: Revisar que los iteradores (fila->begin()/end()) devuelvan punteros.
 *  Si devuelven punteros las funciones std::copy... haran memcpy internamente
 *  siendo eficientes. Si no devuelven punteros se pierde esto. Creo que no
 *  están devolviendo punteros. Revisar. CUIDADO: depende de lo que sea
 *  Container2D. ¿Por qué no puedo hacer una submatrix de una view? La view no
 *  dará acceso a los punteros. 
 *
 *	Por ejemplo:    Submatrix<Imagen_I<Imagen>> ...
 *
 *  Imagen_I devuelve las intensidades, que calcula. ¡No puede devolver un
 *  puntero!!!
 *  Sería interesante dar diferentes versiones, dependiendo de si se puede
 *  devolver un puntero o no.
 *
 *  TODO: reescribir. Estoy mezclando 2 responsabilidades. Por una parte es
 *  una submatriz, y por otra parte la submatriz la puedo mover. Todas las
 *  funciones de movimiento sacarlas de aqui.
 *
 *  TODO: Las submatrices las tengo implementadas como Matrix_view!!!! Sobra
 *  esta clase!!!
 */
template <typename Container2D>
// requeris: is_matrix(Container2D(oncept))
class Submatrix{
public:
    // Objetos que contiene
    using value_type      = Value_type<Container2D>;
    using reference       = Reference_type<Container2D>;
    using const_reference = typename Container2D::const_reference;
    using pointer         = Pointer_type<Container2D>;
    using const_pointer   = typename Container2D::const_pointer;

    // Indices unidimensionales
    using size_type       = typename Container2D::size_type;
    using difference_type = typename Container2D::difference_type;

    // Tipos que definen los contenedores bidimensionales
    using Ind             = typename Container2D::Ind;
    using Position	= typename Container2D::Position;
    using Size2D	= typename Container2D::Size2D;
    using Range2D	= typename Container2D::Range2D;

    // interno
    using Range2D_acotado = alp::Range_acotado_ij<Ind>;

    // iterator siempre es iterador unidimensional!!!
    using iterator = std::conditional_t <std::is_const_v<Container2D>,
			    const_Submatrix_iterator<typename Container2D::const_iterator>,
			    Submatrix_iterator<typename Container2D::iterator>>;

    using const_iterator =
        const_Submatrix_iterator<typename Container2D::const_iterator>;

    // Observar que las filas son subcontainers del contenedor original
    // y no de la submatriz.
    using Row		= Subcontainer<iterator>;
    using const_Row	= Subcontainer<const_iterator>;

    using row_iterator       = Row_iterator<iterator>;
    using const_row_iterator = const_Row_iterator<const_iterator>;


    // Construcción
    // ------------
    /// Creamos una submatrix sin enlazarla a ninguna matrix.
    /// Antes de usarla es fundamental enlazarla con la matrix
    Submatrix():m_{nullptr}{}


    /// Creamos una submatrix de la matrix m0, cuya esquina superior
    /// izquierda es p0 y de tamaño Size2D
    Submatrix(Container2D& m0, const Position& p0, const Size2D& sz)
	:Submatrix{m0, to_rango2D(p0, sz)}
    {}

    /// Creamos una submatrix de la matrix m0, cuya esquina superior
    /// izquierda es p0, y la inferior derecha es p1
    Submatrix(Container2D& m0, const Position& p0, const Position& p1)
	:Submatrix{m0, to_rango2D(p0, p1)}
    {}

    /// Creamos una submatrix de la matrix m0. Vemos el rectángulo r.
    /// precondición: r pertenece a la matrix m0
    Submatrix(Container2D& m0, const Range2D& rg)
//	:m_{&m0}, rg_{r, extension(m0)}
    {
	de(m0, rg);  // así es más fácil de mantener
    }


    // Asociamos la subimagen con una region de una imagen
    // ---------------------------------------------------
    /// Colocamos la submatrix encima de la matrix m0, mostrando la región 
    /// cuya esquina superior izquierda es p0 y tamaño sz.
    void de(Container2D& m0, const Position& p0, const Size2D& sz)
    { de(m0, to_rango2D(p0, sz));}

    /// Colocamos la submatrix encima de la matrix m0, mostrando la región 
    /// definida por los puntos [p0, p1] (= rectángulo).
    void de(Container2D& m0, const Position& p0, const Position& p1)
    {de (m0, to_rango2D(p0, p1));}

    /// Colocamos la submatrix encima de la matrix m0, mostrando la región r.
    void de(Container2D& m0, const Range2D& rg)
    {
	m_ = &m0;
	rg_ = Range2D_acotado{rg, m0.extension()};
    }


    // Redimensionamos la región que mostramos de la imagen
    // ----------------------------------------------------
    // Definición: Llamo extensión al rango que ocupa la submatrix en el
    // sistema de coordenadas (i,j). 
    // OJO: observar que no es el rango que ocupa dentro de la matrix, sino 
    // dentro del sistema de coordenadas. 
    /// Definimos la nueva extensión de la subimagen
    /// precondición: m_ != nullptr
    void extension(const Position& p0, const Size2D& sz)
    {extension(to_rango2D(p0, sz));}

    /// Definimos la nueva extensión de la subimagen
    /// precondición: m_ != nullptr
    void extension(const Position& p0, const Position& p1)
    { extension(to_rango2D(p0, p1)); }

    /// Definimos la nueva extensión de la subimagen
    /// La extensión es el región que ocupa la subimagen dentro de la imagen.
    /// precondición: m_ != nullptr
    void extension(const Range2D& rg) { rg_.extension(rg); }

    /// Devuelve la extensión que ocupa la subimagen
    Range2D extension() const { return rg_.extension(); }


    // -----------
    // Dimensiones
    // -----------
    /// Número de elementos que contiene la submatrix
    Ind size() const {return rg_.rows()*rg_.cols();}

    bool empty() const {return m_ == nullptr;}

    // acceso aleatorio bidimensional
    /// Número de filas
    Ind rows() const {return rg_.rows();}

    /// Número de columnas
    Ind cols() const {return rg_.cols();}

    /// Dimensiones de la submatrix 
    Size2D size2D() const {return rg_.size();}

    // -------------------------------------
    // Acceso como contenedor unidimensional
    // -------------------------------------
    // Tenemos 2 posibles submatrices: aquellas que son submatrices de una
    // matriz (en este caso iterator != const_iterator) y aquellas que lo son 
    // de una const Matrix (en cuyo caso iterator == const_iterator)
    iterator begin() 
    {
	if constexpr (std::is_same_v<iterator, const_iterator>)
	    return const_submatrix_iterator(
			    m_->it(rg_.i0, 0), rg_.j0, rg_.je, m_->cols());
	else 
	    return submatrix_iterator(
			    m_->it(rg_.i0, 0), rg_.j0, rg_.je, m_->cols());
    }

    iterator end()
    {
	if constexpr (std::is_same_v<iterator, const_iterator>)
	    return const_submatrix_iterator(
			    m_->it(rg_.ie, 0), rg_.j0, rg_.je, m_->cols());

	else
	    return submatrix_iterator(
			    m_->it(rg_.ie, 0), rg_.j0, rg_.je, m_->cols());
    }

    const_iterator begin() const
    {// si se pasa m_->it directamente a submatrix_iterator no compila bien
	typename Container2D::const_iterator p0 = m_->it(rg_.i0, 0);

        return const_submatrix_iterator(p0, rg_.j0, rg_.je, m_->cols());
    }

    const_iterator end()   const
    {// si se pasa m_->it directamente a submatrix_iterator no compila bien
	typename Container2D::const_iterator p0 = m_->it(rg_.ie, 0);

        return const_submatrix_iterator(p0, rg_.j0, rg_.je, m_->cols());
    }

    const_iterator cbegin() const {return begin();}
    const_iterator cend() const {return end();}

    // acceso bidimensional
    Row fila(Ind i)
    { return Row{m_->it(i + rg_.i0, rg_.j0), m_->it(i + rg_.i0, rg_.je)}; }

    const_Row fila(Ind i) const 
    { return const_Row{m_->it(i + rg_.i0, rg_.j0), m_->it(i + rg_.i0, rg_.je)}; }

    row_iterator row_begin() 
    {return alp::row_iterator(begin(), cols());}

    row_iterator row_end() 
    { return alp::row_iterator(end(), cols()); }

    const_row_iterator row_begin() const 
    {return alp::const_row_iterator(begin(), cols());}

    const_row_iterator row_end() const 
    { return alp::const_row_iterator(end(), cols()); }

    const_row_iterator row_cbegin() const 
    {return alp::const_row_iterator(begin(), cols());}

    const_row_iterator row_cend() const 
    { return alp::const_row_iterator(end(), cols()); }

    // ----------------
    // Acceso aleatorio
    // ----------------
    /// (i, j) = índices locales
    reference operator()(Ind i, Ind j)
	{return (*m_)(I(i),J(j));}

    /// (i, j) = índices locales
    const reference operator()(Ind i, Ind j) const 
	{return (*m_)(I(i),J(j));}

    // El índice de una matrix es Position
    // Esta función es la forma natural de acceder
    // a los elementos de una matrix
    /// p = posicón local a la submatrix
    reference operator()(const Position& p)
	{return (*this)(p.i, p.j);}

    /// p = posicón local a la submatrix
    const reference operator()(const Position& p) const 
	{return (*this)(p.i, p.j);}


    // Funciones para mover la submatrix (la parte que mostramos de la matrix)
    // -----------------------------------------------------------------------
    // TODO: eliminar estas funciones de aqui. Sacarlas fuera!!!
    /// Mueve el rango x indices a la derecha.
    void muevete_a_la_dcha(Ind x) 
    {rg_.muevete_a_la_dcha(x);}

    /// Mueve el rango x pixeles a la izquierda.
    void muevete_a_la_izda(Ind x)
    {rg_.muevete_a_la_izda(x);}

    /// Mueve el rango x indices hacia abajo.
    void muevete_abajo(Ind x) 
    {rg_.muevete_abajo(x);}

    /// Mueve el rango x indices hacia arriba.
    void muevete_arriba(Ind x)
    {rg_.muevete_arriba(x);}

    // movemos los lados de la submatrix (garantizamos que nunca
    // salgan de la matrix que la contiene)
    void mueve_lado_dcho_a_la_dcha(Ind x)
    {rg_.mueve_lado_dcho_a_la_dcha(x);}

    void mueve_lado_dcho_a_la_izda(Ind x)
    {rg_.mueve_lado_dcho_a_la_izda(x);}

    void mueve_lado_izdo_a_la_dcha(Ind x)
    {rg_.mueve_lado_izdo_a_la_dcha(x);}

    void mueve_lado_izdo_a_la_izda(Ind x)
    {rg_.mueve_lado_izdo_a_la_izda(x);}

    void mueve_lado_superior_arriba(Ind x)
    {rg_.mueve_lado_superior_arriba(x);}

    void mueve_lado_superior_abajo(Ind x)
    {rg_.mueve_lado_superior_abajo(x);}

    void mueve_lado_inferior_arriba(Ind x)
    {rg_.mueve_lado_inferior_arriba(x);}

    void mueve_lado_inferior_abajo(Ind x)
    {rg_.mueve_lado_inferior_abajo(x);}



    // de información de la región
    /// Posición que ocupa la esquina superior izquierda en la matrix
    /// Son coordenadas referentes a la matrix (coordenadas globales)
    Position P0() const {return Position{rg_.i0, rg_.j0};}

    /// Posición que ocupa la esquina inferior derecha en la matrix
    /// Son coordenadas referentes a la matrix (coordenadas globales)
    Position P1() const {return Position{rg_.ie - Ind{1}, rg_.je - Ind{1}};}

private:
    Container2D* m_;	// contenedor donde colocamos la máscara
    Range2D_acotado rg_; // región que mostramos

    // Convertimos de coordenadas locales a globles: (i,j) --> (I,J)
    Ind I(Ind i) const {return rg_.i0 + i;}
    Ind J(Ind j) const {return rg_.j0 + j;}

    // Funciones de ayuda
    Range2D to_rango2D(const Position& p0, const Position& p1)
    {return Range2D{p0.i, p1.i + Ind{1}, p0.j, p1.j + Ind{1}};}

    Range2D to_rango2D(const Position& p0, const Size2D& sz)
    { return Range2D{p0.i, p0.i + sz.rows, p0.j, p0.j + sz.cols};}
};


/// Clonamos una matrix. Devolvemos una copia de la matrix pasada
/// Mezclando esta función con las submatrices podemos clonar cualquier
/// parte de una matrix facilmente.
template <typename Container2D>
// requires: is_matrix(Container2D)
auto clone(const Submatrix<Container2D>& m)
{
    Matrix<typename Container2D::value_type> res{m.size2D()};

    auto f0 = m.row_begin();
    auto f1 = res.row_begin();

    for (; f0 != m.row_end(); ++f0, ++f1)
	std::copy(f0->begin(), f0->end(), f1->begin());

    return res;
}


}// namespace

#endif


