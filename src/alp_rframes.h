// Copyright (C) 2020 Manuel Perez <manuel2perez@proton.me>
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

#ifndef __ALP_RFRAMES_H__
#define __ALP_RFRAMES_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Gestión de diferentes sistemas de coordenadas.
 *  
 *  - COMENTARIOS:
 *	Vector_xy_in y Vector_ij_in es la misma clase. Lo único que cambia es la
 *	notación: (x,y) ó (i, j). Son un copy/paste una de la otra.
 *	DUDA: ¿No se podría fusionar la implementación de las 2 en una?
 *	(usando funciones sería sencillo: x(), y(), i(), j(), pero quiero que
 *	sea una clase muy simple ya que se espera de ella que sea básica).
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    29/07/2020 v0.0
 *
 ****************************************************************************/

#include <iostream>
#include <utility>  // std::swap

#include "alp_rframe_xy.h"
#include "alp_rframe_ij.h"

namespace alp{

/***************************************************************************
 *			CARTISIAN REFERENCE SYSTEMS
 ***************************************************************************/
template <int N, typename Int>
struct Vector_ij_in;


/*!
 *  \brief  Vector en el sistema de referencia `num_reference_system`
 *
 *  Si trabajamos con varios sistemas de referencias las coordenadas tienen
 *  "unidades". No basta con decir: sea el vector A = (2,3). ¿(2, 3) en qué
 *  sistema de referencia?
 *
 *  Lo que hago es numerar los sistemas de referencia: el 0 es el absoluto, 1,
 *  2, ... De esta forma al hablar de coordenadas hablaremos de "(2,3) en 1" ó
 *  de "(2, 3) en 2". 
 *
 */
template <int num_reference_system, typename Int = int>
struct Vector_xy_in : public Vector_xy<Int>{
// Types
    using Ind = Int;	// tipo de índice usado

// Constructors
    constexpr Vector_xy_in() = default;

    constexpr Vector_xy_in(Int x0, Int y0) : Vector_xy<Int>{x0, y0} {}

    constexpr Vector_xy_in(const Vector_ij_in<num_reference_system, Int>& p);
};



// Equality
template <int N1, int N2, typename I>
inline constexpr bool operator==(const Vector_xy_in<N1, I>& a, const Vector_xy_in<N2, I>& b)
{ 
    static_assert(N1 == N2,
                  "No he implementado igualdad de vectores en diferentes "
                  "sistemas de referencia");
    
    return (a.x == b.x) and (a.y == b.y); }


template <int N1, int N2, typename I>
inline constexpr bool operator!=(const Vector_xy_in<N1, I>& a, const Vector_xy_in<N2, I>& b)
{ return !(a == b);}



// Iostreams
template <int N, typename I>
std::ostream& operator<<(std::ostream& out, const Vector_xy_in<N, I>& p)
{
    return out << '(' << p.x << ", " << p.y << ") in " << N;
}


// Operations
// Al usar una clase Reference_frames_xy donde almaceno los orígenes no puedo
// sumar dos vectores dados en sistemas de referencias diferentes. Para
// sumarlos es necesario que el usuario los convierta.
template <int N1, int N2, typename I>
Vector_xy_in<N1, I> operator+(const Vector_xy_in<N1, I>& u, const Vector_xy_in<N2, I>& v)
{
    static_assert(N1 == N2, "Esta implementación no admite la suma de vectores en "
                  "diferentes sistemas de referencia. Para sumarlos, "
                  "conviertelos al mismo sistema primero");

    const Vector_xy<I>& ur = u;
    const Vector_xy<I>& vr = v;
    auto res = ur + vr;
    return Vector_xy_in<N1, I>{res.x, res.y};
}


template <int N1, int N2, typename I>
Vector_xy_in<N1, I> operator-(const Vector_xy_in<N1, I>& u, const Vector_xy_in<N2, I>& v)
{
    static_assert(N1 == N2, "Esta implementación no admite la resta de vectores en "
                  "diferentes sistemas de referencia. Para restarlos, "
                  "conviertelos al mismo sistema primero");

    const Vector_xy<I>& ur = u;
    const Vector_xy<I>& vr = v;
    auto res = ur - vr;
    return Vector_xy_in<N1, I>{res.x, res.y};
}


/***************************************************************************
 *			MATRIX REFERENCE SYSTEMS
 ***************************************************************************/
/*!
 *  \brief  Posición de un punto en coordenadas de matriz.
 *
 *  Las matrices lo habitual es referirlas como (i,j). Esto genera un lío
 *  cuando se trabaja con imágenes, sobre todo si se concibe una imagen como
 *  una simple matriz (= contenedor bidimensional de colores RGB).
 *
 *  Estos Vector_ij_in están pensados para que se puedan integrar de forma
 *  sencilla con un sistema de referencia cartesiano, que son más naturales
 *  cuando quieres dibujar sobre una imagen.
 *
 *  \see Vector_xy_in
 *
 */
template <int num_reference_system, typename Int = int>
struct Vector_ij_in : public Vector_ij<Int>{
// Types
    using Ind = Int;	// tipo de índice usado

// Constructors
    constexpr Vector_ij_in() = default;

    constexpr Vector_ij_in(Int i0, Int j0) : Vector_ij<Int>{i0, j0} { }

    constexpr Vector_ij_in(const Vector_xy_in<num_reference_system>& p)
	: Vector_ij_in{-p.y, p.x} { }

};

// Equality
template <int N1, int N2, typename I>
inline constexpr bool operator==(const Vector_ij_in<N1, I>& a, const Vector_ij_in<N2, I>& b)
{
    static_assert(N1 == N2,
                  "No he implementado igualdad de vectores en diferentes "
                  "sistemas de referencia");
    return (a.i == b.i) and (a.j == b.j); }

template <int N, typename I>
inline constexpr bool operator!=(const Vector_ij_in<N, I>& a, const Vector_ij_in<N, I>& b)
{ return !(a == b);}



// Iostreams
template <int N, typename I>
std::ostream& operator<<(std::ostream& out, const Vector_ij_in<N, I>& p)
{
    return out << '(' << p.i << ", " << p.j << ") in " << N;
}


// Conversions: Vector_xy_in <--> Vector_ij_in
template <int N, typename I>
constexpr Vector_xy_in<N, I>::Vector_xy_in(const Vector_ij_in<N, I>& p) 
    : Vector_xy_in{p.j, -p.i} { }



/***************************************************************************
 *			    REFERENCE FRAMES
 ***************************************************************************/
template <typename Int = int>
struct _Reference_frame_xy_origin{
    Int x, y;
};

/*!
 *  \brief Sistemas de referencias que vamos a usar.
 *
 *  En tiempo de compilación decidimos cuántos sistemas de referencias se van
 *  a usar (si se necesita hacerlo dinámicamente usar std::vector en lugar de
 *  array para origin).
 *
 *  Contiene un listado con todos los sistemas de referencia que usamos
 *  (básicamente un listado con los orígenes dónde colocarlos) y las funciones
 *  de conversión de un sistema a otro.
 *
 */
template <int num_reference_frames, typename Int = int>
class Reference_frames_xy
{
public:
    using Origin = _Reference_frame_xy_origin <Int>;

    // Positiones del origen respecto del sistema de referencia absoluto.
    inline static Origin origin[num_reference_frames];

    template <int N2, int N1>
    static Vector_xy_in<N2, Int> convert(const Vector_xy_in<N1, Int>& p)
    {
	static_assert (0 <= N1 and N1 < num_reference_frames);
	static_assert (0 <= N2 and N2 < num_reference_frames);

	Vector_xy_in<N2> q;

	q.x = origin[N1].x - origin[N2].x + p.x;
	q.y = origin[N1].y - origin[N2].y + p.y;

	return q;
    }

    template <int N2, int N1>
    static Vector_xy_in<N2, Int> convert(const Vector_ij_in<N1, Int>& p)
    { return convert<N2, N1> (Vector_xy_in{p}); }


};




/***************************************************************************
 *				ALIAS	
 ***************************************************************************/
// Los puntos los vamos a referir por sus vectores posiciones para poder
// operar con ellos. Esto es, en lugar de concebir un punto diferente de un
// vector concebimos que los puntos son vectores posición (lo habitual en
// mecánica).
// (???) El standard parece que no deja usar alias-template inicializarlos por
// defecto, así que no funciona el I = int!!!
//template <int N, typename I = int>
//using Point_ij_in = Vector_ij_in<N, I>;
//
//template <int N, typename I = int>
//using Point_xy_in = Vector_xy_in<N, I>;





}// namespace

#endif


