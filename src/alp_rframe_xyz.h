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

#ifndef __ALP_RFRAME_XYZ_H__
#define __ALP_RFRAME_XYZ_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Vector_xyz = vector de un espacio vectorial
 *
 *  - COMENTARIOS: 
 *	Los vectores en 3 dimensiones se pueden representar usando diferentes
 *	sistemas de coordenadas:
 *	    + Vector_xyz: en cartesianas (x,y,z)
 *	    + Vector_rtz: en cilíndricas (r, theta, z)
 *	    + Vector_rtf: en esféricas (r, theta, fi)
 *	    + Vector_ijk: en coordenadas de matriz (i, j, k)
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	21/01/2017 Escrito
 *	01/08/2020 Renombro por consistencia con rframe_xy/ij.
 *
 ****************************************************************************/


#include <iostream>

namespace alp{

// Futuro: 
// 1.- se puede generalizar y crear:
//	Vector<int, int> = Vector2D
//	Vector<int, int, int> = Vector_xyz
//
// 2.- Los tipos del vector y del escalar no tienen porque ser los mismos.
// Habría que parametrizar esta template también indicando el cuerpo sobre el
// que está definido el espacio vectorial.
template <typename T>
// requires: integral_type(T)
struct Vector_xyz{
    using value_type = T;

    value_type x, y, z;

    /// Construimos un vector sin incializar.
    Vector_xyz(){}

    /// Definimos el vector
    Vector_xyz(value_type x0, value_type y0, value_type z0)
	:x{x0}, y{y0}, z{z0}{}

    // Suma de vectores
    Vector_xyz<T>& operator+=(const Vector_xyz<T>& v);
    Vector_xyz<T>& operator-=(const Vector_xyz<T>& v);
};

template <typename T>
inline Vector_xyz<T>& Vector_xyz<T>::operator+=(const Vector_xyz<T>& v)
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}


template <typename T>
inline Vector_xyz<T>& Vector_xyz<T>::operator-=(const Vector_xyz<T>& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}



// Regular type
template <typename T>
inline bool operator==(const Vector_xyz<T>& a, const Vector_xyz<T>& b)
{ return a.x == b.x and a.y == b.y and a.z == b.z;}

template <typename T>
inline bool operator!=(const Vector_xyz<T>& a, const Vector_xyz<T>& b)
{ return !(a == b);}

// Suma de vectores
template <typename T>
inline Vector_xyz<T> operator+(const Vector_xyz<T>& a, const Vector_xyz<T>& b)
{
    auto c = a;
    return c += b;
}

template <typename T>
inline Vector_xyz<T> operator-(const Vector_xyz<T>& a, const Vector_xyz<T>& b)
{
    auto c = a;
    return c -= b;
}


// Producto por escalares
// Solo lo defino por la izquierda, como es habitual en matemáticas.
// E = tipo del escalar
template <typename T, typename E>
inline Vector_xyz<T> operator*(E lambda, const Vector_xyz<T>& v)
{
    return {v.x*lambda, v.y*lambda, v.z*lambda};
}

/// Producto escalar
template <typename T>
inline T dot_product(const Vector_xyz<T>& u, const Vector_xyz<T>& v)
{ return (u.x * v.x + u.y * v.y + u.z * v.z); }


/// Producto vectorial
template <typename T>
inline Vector_xyz<T> cross_product(const Vector_xyz<T>& u, const Vector_xyz<T>& v)
{ 
    return Vector_xyz<T>{u.y * v.z - u.z * v.y
		     , u.z * v.x - u.x * v.z
		     , u.x * v.y - u.y * v.x};
}
	

///// Módulo de un vector
// DUDA: ¿Qué tiene que devolver? T o double?
//template <typename T>
//inline T modulo(const Vector_xyz<T>& v)
//{ return sqrt(dot_product(v,v));}


// Impresión
template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Vector_xyz<T>& v)
{ return out << '(' << v.x << ", " << v.y << ", " << v.z << ')'; }



}// namespace

#endif
