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

#ifndef __ALP_RFRAME_IJ_H__
#define __ALP_RFRAME_IJ_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Sistema de referencia (i,j).
 *
 *   - COMENTARIOS: Incluimos todo lo que se puede representar en este sistema
 *   de referencia:
 *	+ Size_ij   : dimensiones de un objeto (rows, cols).
 *	+ Range_i   : intervalo [i0, ie). 
 *	+ Range_ij  : región del plano [i0, ie) x [j0, je). Es un rectángulo.
 *	+ Range_acotado_ij: es un Range_ij dentro de otro rango.
 *	
 *  Tenemos además los siguientes objetos geométricos:
 *	+ Vector_ij	: un vector vulgar y corriente. 
 *	+ Segment_ij	: segmento de 2 puntos {A, B} dados en coordenadas (i,j).
 *	+ Rectangle_ij	: es un sinónimo de Range_ij. Es la misma clase.
 *
 *
 *   - HISTORIA:
 *    Manuel Perez
 *	27/02/2019 v0.0
 *	01/08/2020 Reestructurado. TODO: borrar todo lo comentado en unos
 *				   meses.
 *	14/11/2020 alrededor (generalizado de img)
 *	06/12/2020 Posiciones_rango_ij
 *
 ****************************************************************************/

#include <iostream>
#include <vector>

namespace alp{

/***************************************************************************
 *			    SEGMENTO DE ÍNDICES
 ***************************************************************************/
/*!
 *  \brief  Intervalo cerrado de números naturales o enteros [a, b]
 *
 *  TODO: llamarlo Segmento_i
 *
 *  DUDA: ¿Qué otro nombre se podría elegir? 
 *		1. CRange_i???		[i0, i1]
 *		2. Closed_range_i ???	[i0, i1]
 *		3. RangeC_i
 *
 *	  ¿Merece la pena crear esta clase o dejarlo como funciones sueltas?
 */
//template <typename Int>
//// requires: is_integral(Ind)
//struct Segmento1D{
//    Int a, b;
// 
//    constexpr Segmento1D(Int a0, Int b0)
//	:a{a0}, b{b0} 
//    {}
//
//    /// Devuelve el número de pasos que hay que dar para ir de 'a' a 'b'.
//    /// postcondicion: b = a + num_pasos
//    constexpr Int num_pasos()
//    {
//	if constexpr (std::is_signed_v<Int>)
//	    return std::abs(a - b);
//	else 
//	    return a > b? a - b: b - a;
//
//    }
//
//    /// Devuelve la longitud del segmento [a, b]. Esto es, el número de puntos
//    /// que tiene el intervalo [a, b]
//    constexpr Int longitud()
//    {
//	return num_pasos() + Int{1};
//    }
//};
//




/***************************************************************************
 *			COORDENADAS DE MATRIZ LOCALES	
 ***************************************************************************/
/*!
 *  \brief  Coordenadas matriciales de un punto en el plano.
 *
 *
 *  En principio tenemos 2 conceptos diferentes:
 *  1.- Point_ij: son puntos en un sistema de referencia de matriz.
 *  2.- Vector_ij: son vectores.
 *
 *  La diferencia básica es que con los vectores podemos operar y los puntos
 *  no. Ahora bien lo habitual es que a los puntos se les asocie un vector
 *  posición, que es lo que se hace en mecánica, y sea ese vector posición el
 *  que realmente se use. Esto sugiere que en lugar de tener 2 tipos
 *  diferentes, puntos y vectores, basta con tener 1 único tipo Vector_ij. Los
 *  puntos los definiremos por los vectores.
 *  
 *  ¿Qué son coordenadas matriciales?
 *
 *  Al empezar a trabajar con imágenes se genera un montón de lio con las
 *  coordenadas. En matrices se usan índices (i,j), pero si quieres dibujar en
 *  una imagen es más natural usar coordenadas cartesianas (x,y). Al final
 *  acabé bautizando a (i,j) como coordenadas matriciales (es el nombre de
 *  esperar).
 *
 *
 *  ¿Por qué el subíndice _ij? 
 *  
 *  Porque tenemos diferentes tipos de sistemas de
 *  coordenadas. El otro habitual es el Vector_xy, que sería un sistema
 *  cartesiano, pero también podemos tener el Vector_rt, que estaría dado en
 *  polares.
 *
 *  ¿Por qué digo que son locales?
 *
 *  Las matrices las voy a usar para contener imágenes. Cuando quiero dibujar
 *  en imágenes me interesa sobre una imagen colocar diferentes sistemas de
 *  referencia para poder dibujar. Vector_ij son las coordenadas del punto en
 *  el sistema de referencia local asociado a la matriz, mientras que
 *  Vector_ij_in<2> son las coordenadas del mismo punto dadas en el sistema de
 *  referencia 2.
 *
 *  Por supuesto que si solo se usa un sistema de referencia Vector_ij da las
 *  coordenadas en ese sistema y serían coordenadas absolutas.
 *  
 */
template <typename Int>
// requires: is_integer(Int) and is_signed(Int)
//	     Int = tipo del índice usado en el contenedor bidimensional
struct Vector_ij{
// Types
    using Ind  = Int;

// Data
    Int i;
    Int j;

// Constructors
    constexpr Vector_ij():i{0}, j{0} {}
    constexpr Vector_ij(Int i0, Int j0):i{i0}, j{j0} {}

// Operations: estructura de espacio vectorial
    constexpr Vector_ij& operator+=(const Vector_ij& b);
    constexpr Vector_ij& operator-=(const Vector_ij& b);
    constexpr Vector_ij& operator*=(const Ind& a);
    constexpr Vector_ij& operator/=(const Ind& a);

};


// Algebraic structure
template <typename I>
inline constexpr 
Vector_ij<I>& Vector_ij<I>::operator+=(const Vector_ij<I>& b)
{
    i += b.i;
    j += b.j;

    return *this;
}

template <typename I>
inline constexpr 
Vector_ij<I>& Vector_ij<I>::operator-=(const Vector_ij<I>& b)
{
    i -= b.i;
    j -= b.j;

    return *this;
}

template <typename I>
inline constexpr 
Vector_ij<I>& Vector_ij<I>::operator*=(const Ind& a)
{
    i *= a;
    j *= a;

    return *this;
}

template <typename I>
inline constexpr 
Vector_ij<I>& Vector_ij<I>::operator/=(const Ind& a)
{
    i /= a;
    j /= a;

    return *this;
}

template <typename I>
inline constexpr Vector_ij<I> operator+(Vector_ij<I> a,
                                            const Vector_ij<I>& b)
{ return a+=b; }

template <typename I>
inline constexpr Vector_ij<I> operator-(Vector_ij<I> a,
                                            const Vector_ij<I>& b)
{ return a-=b; }

template <typename I>
inline constexpr 
Vector_ij<I> operator*(const typename Vector_ij<I>::Ind a, 
                                            Vector_ij<I> b)
{ return b *= a; }

template <typename I>
inline constexpr Vector_ij<I> operator*(Vector_ij<I> b,
			       const typename Vector_ij<I>::Ind a)
{ return a*b;}


template <typename I>
inline constexpr Vector_ij<I> operator/(Vector_ij<I> b,
			       const typename Vector_ij<I>::Ind a)
{ return b /= a; }


// Equality
template<typename Int>
inline bool operator==(const Vector_ij<Int>& p1, const Vector_ij<Int>& p2)
{ return p1.i == p2.i and p1.j == p2.j; }

template<typename Int>
inline bool operator!=(const Vector_ij<Int>& p1, const Vector_ij<Int>& p2)
{return !(p1 == p2);}


// Iostreams
template<typename Int>
inline std::ostream& operator<<(std::ostream& out, const Vector_ij<Int>& p)
{
    out << '(' << p.i << ", " << p.j << ')';
    return out;
}


template<typename Int>
std::istream& operator>>(std::istream& input, Vector_ij<Int>& p)
{
    char c;
    input >> c >> p.i >> c >> c >> p.j >> c;
    return input;
}



//// TODO: eliminar estas.
///// Partiendo de p nos movemos num_pasos a la derecha
//template<typename Int>
//inline Vector_ij<Int> a_la_dcha_de(const Vector_ij<Int>& p, Int num_pasos)
//{ return {p.i, p.j + num_pasos}; }
//
///// Devuelve el punto que se encuentra num_pasos a la derecha de p,
///// sin sobrepasar tope
//template <typename Int>
//inline Vector_ij<Int> a_la_dcha_de(const Vector_ij<Int>& p,
//                                     Int num_pasos,
//                                     const Vector_ij<Int>& tope)
//{ 
//    if (p.j + num_pasos > tope.j) return {p.i, tope.j};
//
//    return {p.i, p.j + num_pasos};
//}
//
//
//
///// Partiendo de p nos movemos num_pasos a la izquierda
//template<typename Int>
//inline Vector_ij<Int> a_la_izda_de(const Vector_ij<Int>& p, Int num_pasos)
//{ return {p.i, p.j - num_pasos}; }
//
//
///// Devuelve el punto que se encuentra num_pasos a la izquierda de p,
///// sin sobrepasar tope.
//// Observar que no resto en la comprobación. Int puede ser unsigned
//// y la resta podría dar underflow
//template <typename Int>
//inline Vector_ij<Int> a_la_izda_de(const Vector_ij<Int>& p,
//                                     Int num_pasos,
//                                     const Vector_ij<Int>& tope)
//{ 
//    if (p.j < tope.j + num_pasos) return {p.i, tope.j};
//
//    return {p.i, p.j - num_pasos};
//}
//


///// Partiendo de p nos movemos num_pasos hacia arriba
//template<typename Int>
//inline Vector_ij<Int> arriba_de(const Vector_ij<Int>& p, Int num_pasos)
//{ return Vector_ij<Int>{p.i - num_pasos, p.j}; }
//
//
//
///// Devuelve el punto que se encuentra num_pasos arriba de p,
///// sin sobrepasar tope
//template <typename Int>
//inline Vector_ij<Int> arriba_de(const Vector_ij<Int>& p,
//                                     Int num_pasos,
//                                     const Vector_ij<Int>& tope)
//{ 
//    if (p.i < num_pasos + tope.j) return {tope.i, p.j};
//
//    return {p.i - num_pasos, p.j};
//}
//
//
///// Partiendo de p nos movemos num_pasos hacia abajo
//template<typename Int>
//inline Vector_ij<Int> abajo_de(const Vector_ij<Int>& p, Int num_pasos) 
//{ return Vector_ij<Int>{p.i + num_pasos, p.j}; }
//
//
///// Devuelve el punto que se encuentra num_pasos abajo de p,
///// sin sobrepasar tope
//template <typename Int>
//inline Vector_ij<Int> abajo_de(const Vector_ij<Int>& p,
//                                     Int num_pasos,
//                                     const Vector_ij<Int>& tope)
//{ 
//    if (p.i + num_pasos > tope.j) return {tope.i, p.j};
//
//    return {p.i + num_pasos, p.j};
//}
//
//
///// Intercambiamos las coordenadas horizontales
//template <typename Int>
//constexpr void swap_h(Vector_ij<Int>& a, Vector_ij<Int>& b)
//{std::swap(a.j, b.j);}
//
///// Intercambiamos las coordenadas horizontales
//template <typename Int>
//constexpr void swap_v(Vector_ij<Int>& a, Vector_ij<Int>& b)
//{std::swap(a.i, b.i);}
//
//
//
///// Devuelve la longitud del segmento [a_h, b_h]
//template <typename Int>
//constexpr Int longitud_segmento_h(const Vector_ij<Int>& a,
//                               const Vector_ij<Int>& b)
//{ return Segmento1D<Int>{a.j, b.j}.longitud(); }
//
//
///// Devuelve la longitud del segmento [a_v, b_v]
//template <typename Int>
//constexpr Int longitud_segmento_v(const Vector_ij<Int>& a, const Vector_ij<Int>& b)
//{ return Segmento1D<Int>{a.i, b.i}.longitud(); }
//
///// Devuelve el número de puntos que hay para ir de 'a' a 'b' horizontalmente
///// Para ir de a_x a b_x
///// Se cumple: b_x = a_x + num_pasos
//template <typename Int>
//constexpr Int num_pasos_h(const Vector_ij<Int>& a, const Vector_ij<Int>& b)
//{return Segmento1D<Int>{a.j, b.j}.num_pasos();}
//
///// Devuelve el número de pasos que hay dar ir de 'a' a 'b' verticalmente
//template <typename Int>
//constexpr Int num_pasos_v(const Vector_ij<Int>& a, const Vector_ij<Int>& b)
//{return Segmento1D<Int>{a.i, b.i}.num_pasos();}
//
//

/***************************************************************************
 *				SIZE
 ***************************************************************************/

/// Size que define un rectángulo cuando usamos coordenadas (i,j)
template <typename Int>
struct Size_ij 
{
    using Ind = Int;
    Int rows, cols;
};

// Equality
template <typename Int>
bool operator==(const Size_ij<Int>& s1, const Size_ij<Int>& s2)
{ return s1.rows == s2.rows and s1.cols == s2.cols; }

template <typename Int>
bool operator!=(const Size_ij<Int>& s1, const Size_ij<Int>& s2)
{ return !(s1 == s2); }


// Iostreams
template <typename Int>
inline std::ostream& operator<<(std::ostream& out, const Size_ij<Int>& s)
{ return out << s.rows << " x " << s.cols; }

/***************************************************************************
 *			    RANGOS DE INDICES
 ***************************************************************************/
/*!
 *  \brief  Range unidimensional de índices.
 *
 *  Un rango es un intervalo semiabierto [i0, ie).
 *
 *  El rango no garantiza que i0 <= ie. Para garantizarlo suministra
 *  la función ordena_indices (llamarla antes de crear el rango)
 *
 */
template <typename Int>
struct Range_i{
    using Ind = Int;

    Ind i0, ie;

    /// Número de índices que hay en [i0, ie)
    constexpr Ind size() const {return ie - i0;}

    /// Ordena los índices
    constexpr void ordena_indices()
    {
	if (ie < i0)
	    std::swap(i0, ie);
    }
};


/*!
 *  \brief  Range bidimensional de índices: [i0, ie) x [j0, je)
 *
 *  El rango no garantiza que i0 <= ie. Para garantizarlo suministra
 *  la función ordena_indices (llamarla antes de crear el rango)
 *
 *  Un Range_ij es lo mismo que un Rectangle_ij.
 */
template <typename Int>
class Range_ij{
public:
// Types
    using Ind      = Int;
    using Position = Vector_ij<Int>;
    using Size2D   = Size_ij<Int>;

// Data
    Ind i0, ie;
    Ind j0, je;

// Constructors
    /// Creamos un rango vacío
    constexpr Range_ij():Range_ij{0,0,0,0}{}

    /// Creamos el rango [i0, ie) x [j0, je)
    constexpr Range_ij(Int i00, Int ie0, Int j00, Int je0)
	:i0{i00}, ie{ie0}, j0{j00}, je{je0} 
    {
	ordena_indices();
    }

    /// Creamos el rango ri x rj = [i0, ie)  x [j0, je)
    constexpr Range_ij(const Range_i<Int>& ri, const Range_i<Int>& rj)
	:Range_ij{ri.i0, ri.ie, rj.i0, rj.ie} {}

    /// Range [p0, p1] = [upper_left_corner, bottom_right_corner]
    constexpr Range_ij(const Position& p0, const Position& p1)
	:Range_ij{p0.i, p1.i, p0.j, p1.j} { ++ie; ++je;}

    /// Range (p0, sz) = (upper_left_corner, size)
    constexpr Range_ij(const Position& p0, const Size2D& sz)
	:Range_ij{p0.i, p0.i + sz.rows, p0.j, p0.j + sz.cols} {}



// Dimensions
// ----------
    /// Número de filas del rango
    constexpr Int rows() const {return ie - i0;}

    /// Número de columnas del rango
    constexpr Int cols() const {return je - j0;}

    /// Size 2D del rango
    constexpr Size2D size() const {return {rows(), cols()};}

    /// ¿Es un rango vacío?
    constexpr bool empty() const {return (i0 == ie) or (j0 == je);}


// Esquinas
// --------
    // Solo tienen sentido si el rectángulo no es nulo (empty() != true)
    // Recordar que son coordenadas de matriz: i va hacia abajo!!!
    constexpr Position upper_left_corner() const;
    constexpr Position bottom_right_corner() const;
    constexpr Position upper_right_corner() const;
    constexpr Position bottom_left_corner() const;

    /// Definimos la esquina superior izquierda
    constexpr void upper_left_corner(const Position& p);

    /// Definimos la esquina inferior derecha
    constexpr void bottom_right_corner(const Position& p);


    // Varios
    // ------
    // Funciones auxiliares
    constexpr void ordena_indices()
    {
	if (ie < i0) std::swap(i0, ie);
	if (je < j0) std::swap(j0, je);
    }

};


// Esquinas
template <typename Int>
inline constexpr 
Range_ij<Int>::Position Range_ij<Int>::upper_left_corner() const
{
    return Position{i0, j0};
}


template <typename Int>
inline constexpr 
Range_ij<Int>::Position Range_ij<Int>::bottom_right_corner() const
{ return Position{ie - Ind{1}, je - Ind{1}}; }

template <typename Int>
inline constexpr 
Range_ij<Int>::Position Range_ij<Int>::upper_right_corner() const
{ return Position{i0, je - Ind{1}}; }

template <typename Int>
inline constexpr 
Range_ij<Int>::Position Range_ij<Int>::bottom_left_corner() const
{ return Position{ie - Ind{1}, j0}; }

template <typename Int>
inline constexpr 
void Range_ij<Int>::upper_left_corner(const Position& p)
{ *this = Range_ij{p, bottom_right_corner()}; }

template <typename Int>
inline constexpr 
void Range_ij<Int>::bottom_right_corner(const Position& p) 
{ *this = Range_ij{upper_left_corner(), p}; }



// Iostreams
template <typename Int>
inline std::ostream& operator<<(std::ostream& out, const Range_ij<Int>& rg)
{
    return out	<< '[' << rg.i0 << ", " << rg.ie << ") x [" 
		       << rg.j0 << ", " << rg.je << ')';
}




// Relación: rg1 es subconjunto de rg2
// Idioma: if (es_subconjunto(rg1).de(rg2)) ...
// Observar que esta es una relación definida entre rangos. Tiene total
// sentido!!!
template <typename Int>
struct Range_ij_ser_subconjunto_de{
    using Range_ij = alp::Range_ij<Int>;
    constexpr Range_ij_ser_subconjunto_de(const Range_ij& rg10):rg1{rg10}{}

    // ¿rg1 es subconjunto de rg2?
    constexpr bool de(const Range_ij& rg2){
	return  (rg2.i0 <= rg1.i0 and rg1.ie <= rg2.ie)
	    and (rg2.j0 <= rg1.j0 and rg1.je <= rg2.je);
    }

    const Range_ij& rg1;
};

/// Idioma: if (es_subconjunto(rg1).de(rg2)) ...
template <typename Int>
constexpr Range_ij_ser_subconjunto_de<Int> es_subconjunto(const Range_ij<Int>& rg)
{return Range_ij_ser_subconjunto_de<Int>{rg};}

template <typename Int>
constexpr Range_ij_ser_subconjunto_de<Int> esta_dentro(const Range_ij<Int>& rg)
{return es_subconjunto(rg);}





/*!
 *  \brief  Range bidimensional acotado por otro rango
 *
 *  Al estar acotado garantizamos que no nos podemos salir del rango.
 *
 *  Invariante:
 *	El rango siempre estará dentro de cota.
 *	Los índices de los rangos están ordenados (i0 <= ie, j0 <= je)
 */
// No me funciona bien la herencia con templates, asi que no heredo de 
// Range_ij (¿por qué? Los iostream funcionan perfectamente!!!)
template <typename Int>
struct Range_acotado_ij{
    using Ind      = Int;
    using Range2D  = alp::Range_ij<Int>;
    using Position = alp::Vector_ij<Int>;
    using Size2D   = Size_ij<Int>;

    Ind i0, ie;	
    Ind j0, je;	

    Range2D cota;	// cota del rango. Este rango no se saldrá de cota.

    // Conversión por defecto
    operator Range2D() const {return Range2D{i0, ie, j0, je};}


    // Construcción
    // ------------
    /// Creamos un rango vacío
    constexpr Range_acotado_ij():i0{0}, ie{i0}, j0{0}, je{j0} {}

    /// Creamos un rango rg acotado por cota0.
    /// Esta función garantiza que el rango queda bien creado, con los
    /// índices ordenados. 
    /// En imágenes cuando se selecciona una región los índices no tienen
    /// por qué quedar bien ordenados. Por eso los ordeno aquí.
    constexpr Range_acotado_ij(const Range2D& rg, const Range2D cota0)
	:cota{cota0} 
    {
	cota.ordena_indices();
	extension(rg);
    }


    // Características
    // ---------------
    /// Número de filas del rango
    constexpr Int rows() const { return ie - i0; }

    /// Número de columnas del rango
    constexpr Int cols() const { return je - j0; }

    /// Size 2D del rango
    constexpr Size2D size() const { return {rows(), cols()}; }

    /// Esquina superior izquierda
    constexpr Position p0() const {return Position{i0, j0};}

    /// Esquina inferior derecha 
    /// Cuidado: para que p1 sea válido, el rango no puede estar vacío
    constexpr Position p1() const {return Position{ie - Ind{1}, je - Ind{1}};}

    constexpr void p0(const Position& p)
    { extension(Range2D{p, p1()}); }

    constexpr void p1(const Position& p)
    { extension(Range2D{p0(), p}); }


    /// ¿Es un rango vacío?
    constexpr bool empty() const {return (i0 == ie) or (j0 == je);}


    /// Devuelve la extensión del rango. La extensión es el rango 
    /// propiamente dicho
    Range2D extension() const { return Range2D{i0, ie, j0, je}; }

    /// Definimos la extensión que tiene este rango
    void extension(Range2D rg)
    {
	rg.ordena_indices();
	
	rg = rango_dentro_de_la_cota(rg);

	i0 = rg.i0; ie = rg.ie;
	j0 = rg.j0; je = rg.je;
    }

    /// Definimos la extensión que tiene este rango
    void extension(const Position& p0, const Position& p1)
    { extension(Range2D{p0, p1}); }

    // Operaciones
    // -----------
    /// Mueve el rango x indices a la derecha.
    /// Si es signed esta función se puede usar con números negativos
    void muevete_a_la_dcha(Ind x) { mueve_mas_j(x); }

    /// Mueve el rango x pixeles a la izquierda.
    void muevete_a_la_izda(Ind x){ mueve_menos_j(x); }

    /// Mueve el rango x indices hacia abajo.
    /// Si es signed esta función se puede usar con números negativos
    void muevete_abajo(Ind x) { mueve_mas_i(x);}

    /// Mueve el rango x indices hacia arriba.
    void muevete_arriba(Ind x) { mueve_menos_i(x);}

    void mueve_lado_dcho_a_la_dcha(Ind x)
    {je = mueve_sentido_mas(je, x, cota.je);}

    void mueve_lado_dcho_a_la_izda(Ind x)
    {je = mueve_sentido_menos(je, x, j0);}

    void mueve_lado_izdo_a_la_dcha(Ind x)
    {j0 = mueve_sentido_mas(j0, x, je);}

    void mueve_lado_izdo_a_la_izda(Ind x)
    {j0 = mueve_sentido_menos(j0, x, cota.j0);}

    void mueve_lado_superior_arriba(Ind x)
    {i0 = mueve_sentido_mas(i0, x, cota.i0);}

    void mueve_lado_superior_abajo(Ind x)
    {ie = mueve_sentido_menos(ie, x, ie);}

    void mueve_lado_inferior_arriba(Ind x)
    {ie = mueve_sentido_mas(ie, x, i0);}

    void mueve_lado_inferior_abajo(Ind x)
    {ie = mueve_sentido_menos(ie, x, cota.ie);}


    // Funciones auxiliares
    constexpr void ordena_indices()
    {
	if (ie < i0) std::swap(i0, ie);
	if (je < j0) std::swap(j0, je);
    }


private:
    // Es necesario definir operaciones mas_i/menos_i ya que Int puede ser
    // unsigned: ¡No se le puede pasar un incr_i negativo!!! ya que no existen
    // números negativos unsigned!!!
    void mueve_mas_i(Ind incr_i)
    {
	if constexpr (std::is_signed_v<Ind>){
	    if (incr_i < 0){
		mueve_menos_i(-incr_i);
		return;
	    }
	}

	Ind di = std::min(incr_i, cota.ie - ie);
	i0 += di;
	ie += di;
    }

    // precondition: incr_i >= 0
    void mueve_menos_i(Ind incr_i)
    {
	if constexpr (std::is_signed_v<Ind>){
	    if (incr_i < 0){
		mueve_mas_i(-incr_i);
		return;
	    }
	}

	Ind di = std::min(incr_i, i0 - cota.i0);
	i0 -= di;
	ie -= di;
    }

    void mueve_mas_j(Ind incr_j)
    {
	if constexpr (std::is_signed_v<Ind>){
	    if (incr_j < 0){
		mueve_menos_j(-incr_j);
		return;
	    }
	}

	Ind dj = std::min(incr_j, cota.je - je);
	j0 += dj;
	je += dj;
    }

    // precondition: incr_j >= 0
    void mueve_menos_j(Ind incr_j)
    {
	if constexpr (std::is_signed_v<Ind>){
	    if (incr_j < 0){
		mueve_mas_j(-incr_j);
		return;
	    }
	}

	Ind dj = std::min(incr_j, j0 - cota.j0);
	j0 -= dj;
	je -= dj;
    }

    // mueve el índice i en el sentido + sin sobrepasar i_max
    Ind mueve_sentido_mas(Ind i, Ind incr_i, Ind i_max)
    {
	if (i + incr_i > i_max) return i_max;
	
	return i + incr_i;
    }


    // mueve el índice i en el sentido - sin sobrepasar i_min
    Ind mueve_sentido_menos(Ind i, Ind incr_i, Ind i_min)
    {
	if (i < i_min + incr_i) return i_min;
	
	return i - incr_i;
    }


    // Devuelve el rango rg dentro de la cota del rango acotado
    // Garantiza que el rango devuelto esté dentro de la cota (del terreno)
    Range2D rango_dentro_de_la_cota(Range2D rg)
    {
	if (	rg.i0 > cota.ie or rg.ie < cota.i0
	    or  rg.j0 > cota.je or rg.je < cota.j0){
	    std::cerr << "\n\nERROR: Range_acotado_ij::rango_dentro_de_la_cota: "
			 "el rango está fuera de la cota. Defino el "
			 "rango a toda la cota\n";
	    return cota;
	}

	if (rg.i0 < cota.i0)	rg.i0 = cota.i0;
	if (rg.ie > cota.ie)	rg.ie = cota.ie;

	if (rg.j0 < cota.j0)	rg.j0 = cota.j0;
	if (rg.je > cota.je)	rg.je = cota.je;

	return rg;
    }
};

template <typename Int>
inline std::ostream& operator<<(std::ostream& out,
                                const Range_acotado_ij<Int>& rg)
{
    return out	<< '[' << rg.i0 << ", " << rg.ie << ") x [" 
		       << rg.j0 << ", " << rg.je << ')';
}


/***************************************************************************
 *				RELACIONES
 ***************************************************************************/
// Relación: Position belongs to ...
// Idiom: if (belongs(p).to(rg)) ...
template <typename Int>
struct Vector_ij_belongs_to{
    constexpr Vector_ij_belongs_to(const Vector_ij<Int>& p0):p{p0}{}

    // p belongs to range rg?
    constexpr bool to(const Range_ij<Int>& rg){
	return  (rg.i0 <= p.i  and p.i < rg.ie) 
	    and (rg.j0 <= p.j  and p.j < rg.je);
    }

    const Vector_ij<Int>& p;
};

// Idiom: if (belongs(p).to(rg)) ...
template <typename Int>
constexpr Vector_ij_belongs_to<Int> belongs(const Vector_ij<Int>& p)
{return Vector_ij_belongs_to<Int>{p};}




/***************************************************************************
 *				    GEOMETRY
 ***************************************************************************/
/*!
 *  \brief  Es un segmento.
 *
 *  Un segmento viene definido por dos puntos [A, B].
 *
 *  Con esta descripción es imposible hablar de segmentos 0 (o vacíos)
 *  ya que el segmento [A, A] está formado por el punto A no siendo vacío.
 *  Por ello, para poder incluir segmentos vacíos se puede implementar un
 *  segmento como [p0, pe). Tenemos dos formas básicas de describir un
 *  segmento:
 *	1.- [p0, p1] = [A, B]
 *	2.- [p0, pe)
 *
 *
 *  Es muy sencillo implementar una clase Segment<Point> donde el parámetro de
 *  plantilla sea el tipo de punto. Sin embargo, como tendría un interfaz
 *  distinto que Rectangle_ij<Int> (<--- viene parametrizado por el índice y no
 *  por el tipo de punto) eso genera confusión (al hacerlo yo mismo me
 *  confundí). Por eso la implemento de forma separada. (La opción de
 *  implementar una clase Rectangle<Point> complica todo ya que hay que
 *  complicar todas las clases Point para poder implementarlo. Lo hicé y
 *  complicó el código).
 */
template<typename Int>
class Segment_ij
{
public:
// Types
    using Ind   = Int;
    using Point = Vector_ij<Int>;

// Data
    Point A, B; // extremos del segmento

// Constructors
    Segment_ij(const Point& A0, const Point& B0) : A{A0}, B{B0} {}
};



// Equality
template<typename I>
inline bool operator==(const Segment_ij<I>& a, const Segment_ij<I>& b)
{ return (a.A == b.A) and (a.B == b.B); }

template<typename I>
inline bool operator!=(const Segment_ij<I>& a, const Segment_ij<I>& b)
{ return !(a == b); }


// Iostreams
template<typename I>
inline std::ostream& operator<<(std::ostream& out, const Segment_ij<I>& s)
{
    out << "[" << s.A << ", " << s.B << "]";

    return out;
}


// Predicates
template<typename I>
inline bool es_un_punto(const Segment_ij<I>& s)
{ return (s.A == s.B); }



/***************************************************************************
 *				ALIAS
 ***************************************************************************/
template <typename Int>
using Rectangle_ij = Range_ij<Int>;


/***************************************************************************
 *				ALGORITMOS
 ***************************************************************************/
/*!
 *  \brief  Devuelve los puntos del rango que se encuentran alrededor de p.
 *
 *  Ejemplo:
 *  \code
 *		for(auto p: alrededor(rango, {3,4})) ...
 *  \endcode
 *
 */
template <typename I>
std::vector<typename Range_ij<I>::Position>
alrededor(const Range_ij<I>& rg, const typename Range_ij<I>::Position& p)
{
    using Position = typename Range_ij<I>::Position;

    std::vector<Position> v;
    v.reserve(8);
   
    // (i1, j1) valores máximos de los índices
    auto i1 = rg.rows()-1; auto  j1 = rg.cols() - 1;
    auto i = p.i; auto j = p.j;

    if(i-1 >= 0){
	if(j-1 >= 0)	v.push_back(Position{i-1, j-1});
	v.push_back(Position{i-1, j});
	if(j+1 <= j1)	v.push_back(Position{i-1, j+1});
    }

    // i
    if(j-1 >= 0)    v.push_back(Position{i, j-1});
//		    v.push_back(Position{i, j}); <--- este no está alrededor!
    if(j+1 <= j1)   v.push_back(Position{i, j+1});


    if(i+1 <= i1){
	if(j-1 >= 0)	v.push_back(Position{i+1, j-1});
	v.push_back(Position{i+1, j});
	if(j+1 <= j1)	v.push_back(Position{i+1, j+1});
    }

    return v;
}

/*!
 *  \brief  Devuelve la posición que ocupa el centro de la matrix
 *
 */
template <typename Int>
inline Range_ij<Int>::Position posicion_del_centro(const Range_ij<Int>& rg)
{ return {rg.i0 + rg.rows() / 2, rg.j0 + rg.cols() / 2}; }


/*!
 *  \brief  Posiciones_bordes
 *
 *  Si queremos iterar por los bordes de un rango podemos hacerlo:
 *  1. Usando posiciones:
 *	    Posiciones_bordes bordes(m);
 *	    for (auto p = bordes.begin(); p != bordes.end(); ++p)
 *		... p.i, p.j = posición del siguiente elemento del borde.
 *	
 *  2. Iterando directamente por los valores:
 *	    Bordes bordes(m);
 *	    for (auto p = bordes.begin(); p != bordes.end(); ++p)
 *		    *p = valor de p.
 *
 */
class Posiciones_bordes_rango_ij{
public:
    int i, j; // posición actual


    template <typename I>
    static Posiciones_bordes_rango_ij begin(const Range_ij<I>& rg)
    {
	Posiciones_bordes_rango_ij p;

	p.i = 0;
	p.j = 0;
	p.rows_ = rg.rows();
	p.cols_ = rg.cols();

	return p;
    }

    template <typename I>
    static Posiciones_bordes_rango_ij end(const Range_ij<I>& rg){
        Posiciones_bordes_rango_ij p;
        p.i = rg.rows();
	p.j = 0;

	return p;
    }

    Posiciones_bordes_rango_ij& operator++();

    friend bool operator==(const Posiciones_bordes_rango_ij&, const Posiciones_bordes_rango_ij&);

private:
    int rows_, cols_;

    Posiciones_bordes_rango_ij(){}
    void next();
};



inline Posiciones_bordes_rango_ij& Posiciones_bordes_rango_ij::operator++()
{
    next();
    return *this;
}

inline bool operator==(const Posiciones_bordes_rango_ij& a,
                const Posiciones_bordes_rango_ij& b)
{
    return (a.i == b.i and a.j == b.j);
}

inline bool operator!=(const Posiciones_bordes_rango_ij& a,
                const Posiciones_bordes_rango_ij& b)
{ return !(a == b);}


/*!
 *  \brief Posiciones que tiene un Range_ij
 *
 *  Para iterar con índices en un contenedor unidimensional hacemos:
 *	    for (int i = 0; i < v.size(); ++i)
 *		...
 *
 *  ¿Cómo hacer el equivalente en contenedores bidimensionales?
 *	    Posiciones_rango_ij posiciones(m);
 *	    for (auto p = posiciones.begin(); p != posiciones.end(); ++p)
 *		... p.i, p.j = posición del siguiente elemento.
 *
 *  Esto es equivalente a:
 *	    for (int i = 0; i < m.rows(); ++i)
 *		for (int j = 0; j < m.cols(); ++j)
 *		    ...
 */
class Posiciones_rango_ij{
public:
    int i, j; // posición actual


    template <typename I>
    static Posiciones_rango_ij begin(const Range_ij<I>& rg)
    {
	Posiciones_rango_ij p;

	p.i = 0;
	p.j = 0;
	p.cols_ = rg.cols();

	return p;
    }

    template <typename I>
    static Posiciones_rango_ij end(const Range_ij<I>& rg){
        Posiciones_rango_ij p;
        p.i = rg.rows();
	p.j = 0;

	return p;
    }

    Posiciones_rango_ij& operator++();

    friend bool operator==(const Posiciones_rango_ij&,
                           const Posiciones_rango_ij&);

  private:
    int cols_;

    Posiciones_rango_ij(){}
    void next();
};



inline Posiciones_rango_ij& Posiciones_rango_ij::operator++()
{
    ++j;

    if (j == cols_){
	j = 0;
	++i;
    }

    return *this;
}


inline bool operator==(const Posiciones_rango_ij& a,
                const Posiciones_rango_ij& b)
{
    return (a.i == b.i and a.j == b.j);
}

inline bool operator!=(const Posiciones_rango_ij& a,
                const Posiciones_rango_ij& b)
{ return !(a == b);}


}// namespace


#endif

