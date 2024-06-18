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

#ifndef __ALP_RFRAME_XY_H__
#define __ALP_RFRAME_XY_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Sistema de coordenadas (x,y)
 *
 *   - COMENTARIOS: 
 *	Puntos en 2D: (realmente son los vectores posición de los puntos)
 *	    (vamos a operar con ellos)
 *	    + Position_ij: posición dentro de una matriz (i,j)
 *	    + Punto_xy: punto en cartesianas (x,y)
 *	    + Punto_rt: punto en polares (r, theta)
 *
 *	    ¡¡¡RECORDAR QUE REALMENTE SON VECTORES!!!
 *
 *	Puntos en 3D:
 *	    + Punto_xyz: en cartesianas (x,y,z)
 *	    + Punto_rtz: en cilíndricas (r, theta, z)
 *	    + Punto_rtf: en esféricas (r, theta, fi)
 *
 *	Rectángulos en 2D:
 *	    + Rectangulo_ij: en (i,j)
 *	    + Rectangulo_xy: en (x,y)
 *
 *	Todo siempre lo vamos a referir al sistema de referencia humano:
 *	horizontal y vertical, a la derecha, izquierda, arriba y abajo.
 *	Este sistema es el natural para nosotros, por eso todas las funciones
 *	lo usan. Además, permiten desvincular mucho código del sistema de
 *	referencia que estemos usando.
 *
 *   - CUIDADO: Aunque los puntos están parametrizados por Int, Int es 
 *	signed. Usar siempre signed. Si se quiere usar size_t (que en
 *	principio parecería ser lo más lógico para matrices) hay que revisar
 *	el código.
 *
 *   - HISTORIA:
 *    Manuel Perez
 *	14/10/2017 Reescrito
 *	22/02/2019 Generalizo Rectangulo y notación.
 *	31/07/2020 Reestructurado. 
 *
 ****************************************************************************/

#include <iostream>


#include "alp_math.h"	// Radian/Degree

namespace alp{
/*!
 *  \brief  Vector en coordenadas locales.
 *
 *  Sistemas de referencias que podemos tener:
 *	1.- Sistema local, asociado al objeto.
 *	2.- Sistema absoluto (sería el sistema de referencia universal de
 *	    Newton).
 *	3.- Diferentes sistemas de referencias.
 *
 *
 *  CUIDADO: el sistema de referencia xy es un sistema de ejes cartesiano,
 *  perpendiculares, con el eje x hacia la derecha y el eje y hacia arriba.
 *  Esto es fundamental para coordinarlo con el sistema matricial ij que es el
 *  sistema donde j va hacia la derecha e i hacia abajo.
 *
 */
template <typename Int>
struct Vector_xy{
// Types
    using Ind = Int;	// tipo de índice usado

// Data
    Ind x;
    Ind y;

// Constructors
    constexpr Vector_xy() : x{}, y{} {}
    constexpr Vector_xy(Ind x0, Ind y0) : x{x0}, y{y0} {}

// Operations: estructura de espacio vectorial
    constexpr Vector_xy& operator+=(const Vector_xy& b);
    constexpr Vector_xy& operator-=(const Vector_xy& b);
    constexpr Vector_xy& operator*=(const Ind& a);
    constexpr Vector_xy& operator/=(const Ind& a);
};


// Algebraic structure
template <typename I>
inline constexpr 
Vector_xy<I>& Vector_xy<I>::operator+=(const Vector_xy<I>& b)
{
    x += b.x;
    y += b.y;

    return *this;
}

template <typename I>
inline constexpr 
Vector_xy<I>& Vector_xy<I>::operator-=(const Vector_xy<I>& b)
{
    x -= b.x;
    y -= b.y;

    return *this;
}

template <typename I>
inline constexpr 
Vector_xy<I>& Vector_xy<I>::operator*=(const Ind& a)
{
    x *= a;
    y *= a;

    return *this;
}

template <typename I>
inline constexpr 
Vector_xy<I>& Vector_xy<I>::operator/=(const Ind& a)
{
    x /= a;
    y /= a;

    return *this;
}

template <typename I>
inline constexpr Vector_xy<I> operator+(Vector_xy<I> a,
                                            const Vector_xy<I>& b)
{ return a+=b; }

template <typename I>
inline constexpr Vector_xy<I> operator-(Vector_xy<I> a,
                                            const Vector_xy<I>& b)
{ return a-=b; }

template <typename I>
inline constexpr 
Vector_xy<I> operator*(const typename Vector_xy<I>::Ind a, 
                                            Vector_xy<I> b)
{ return b *= a; }

template <typename I>
inline constexpr Vector_xy<I> operator*(Vector_xy<I> b,
			       const typename Vector_xy<I>::Ind a)
{ return a*b;}


template <typename I>
inline constexpr Vector_xy<I> operator/(Vector_xy<I> b,
			       const typename Vector_xy<I>::Ind a)
{ return b /= a; }

// Equality
template <typename I>
inline constexpr bool operator==(const Vector_xy<I>& a, const Vector_xy<I>& b)
{ return (a.x == b.x) and (a.y == b.y); }

template <typename I>
inline constexpr bool operator!=(const Vector_xy<I>& a, const Vector_xy<I>& b)
{ return !(a == b);}



// Iostreams
template <typename I>
std::ostream& operator<<(std::ostream& out, const Vector_xy<I>& p)
{
    return out << '(' << p.x << ", " << p.y << ") local";
}

// Alias
template <typename Int>
using Point_xy = Vector_xy<Int>;



// Other operations
// ----------------
// TODO: what is the best name in english? modulus? norm? lenght?
template <typename Int>
Int modulo(const Vector_xy<Int>& v)
{ return sqrt(v.x * v.x + v.y * v.y); }


/// Gira el vector `v`, `angle` radianes.
template <typename Int>
Vector_xy<Int> rotate(const Vector_xy<Int>& v, const Radian& angle)
{
    Vector_xy<Int> res;

    auto x = v.x * cos(angle) - v.y * sin(angle);
    auto y = v.x * sin(angle) + v.y * cos(angle);

    if constexpr (std::is_integral_v<Int>){
	res.x = std::round(x);  // round para evitar los problemas de casting
	res.y = std::round(y);
    }
    else{// float, double
	res.x = x;
	res.y = y;
    }

    return res;
}

/// Gira el vector `v`, `angle` grados.
template <typename I>
inline Vector_xy<I> rotate(const Vector_xy<I>& v, const Degree& angle)
{ return rotate(v, Radian{angle}); }


/*!
 *  \brief  Rectángulo en un sistema de coordenadas (x,y)
 *
 */
template <typename Int>
class Rectangle_xy{
public:
// Types
    using Ind    = Int;
    using Vector = Vector_xy<Int>;

// Constructors
    constexpr Rectangle_xy() : b0_{0,0}, width_{0}, height_{0} { }

    // Definirmos un rectángulo con las esquinas dadas, ambas perteneciendo al
    // rectángulo.
    constexpr Rectangle_xy(Vector bottom_left_corner, Ind width0, Ind height0)
	    : b0_{bottom_left_corner}, width_{width0}, height_{height0} { }

// Esquinas
    constexpr Vector upper_left_corner() const;
    constexpr Vector upper_right_corner() const;
    constexpr Vector bottom_left_corner() const;
    constexpr Vector bottom_right_corner() const;


// Dimensiones
    Ind width() const {return width_;}
    Ind height() const {return height_;}

    /// ¿Rectángulo nulo?
    bool empty() const {return width_ == 0 or height_ == 0;}

// Equality
    template<typename V>
    friend bool operator==(const Rectangle_xy<V>& r1, const Rectangle_xy<V>& r2);

private:
    Vector b0_;	// esquina inferior izda
    Ind width_;
    Ind height_;
};



// Esquinas
template <typename I>
inline constexpr 
Rectangle_xy<I>::Vector Rectangle_xy<I>::bottom_left_corner() const
{ return b0_; }

template <typename I>
inline constexpr
Rectangle_xy<I>::Vector Rectangle_xy<I>::bottom_right_corner() const
{ return b0_ +  Vector{width_ - 1, 0}; }

template <typename I>
inline constexpr
Rectangle_xy<I>::Vector Rectangle_xy<I>::upper_left_corner() const
{ return b0_ + Vector{0, height_ - 1}; }

template <typename I>
inline constexpr
Rectangle_xy<I>::Vector Rectangle_xy<I>::upper_right_corner() const
{ return b0_ +  Vector{width_ - 1, height_ - 1}; }


// Equality
template <typename I>
inline bool operator==(const Rectangle_xy<I>& r1, const Rectangle_xy<I>& r2)
{
    return (r1.b0_ == r2.b0_ and 
	    r1.width_ == r2.width_ and
            r1.height_ == r2.height_);
}

template <typename I>
inline bool operator!=(const Rectangle_xy<I>& r1, const Rectangle_xy<I>& r2)
{ return !(r1 == r2); }


}// namespace

#endif 
