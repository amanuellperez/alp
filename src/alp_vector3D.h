// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#ifndef __ALP_VECTOR3D_H__
#define __ALP_VECTOR3D_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Vector3D = vector de un espacio vectorial
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 21/01/2017 Escrito
 *
 ****************************************************************************/
#include <iostream>

namespace alp{

// Futuro: 
// 1.- se puede generalizar y crear:
//	Vector<int, int> = Vector2D
//	Vector<int, int, int> = Vector3D
//
// 2.- Los tipos del vector y del escalar no tienen porque ser los mismos.
// Habría que parametrizar esta template también indicando el cuerpo sobre el
// que está definido el espacio vectorial.
template <typename T>
// requires: integral_type(T)
struct Vector3D{
    using value_type = T;

    value_type x, y, z;

    /// Construimos un vector sin incializar.
    Vector3D(){}

    /// Definimos el vector
    Vector3D(value_type x0, value_type y0, value_type z0)
	:x{x0}, y{y0}, z{z0}{}

    // Suma de vectores
    Vector3D<T>& operator+=(const Vector3D<T>& v);
    Vector3D<T>& operator-=(const Vector3D<T>& v);
};

template <typename T>
inline Vector3D<T>& Vector3D<T>::operator+=(const Vector3D<T>& v)
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}


template <typename T>
inline Vector3D<T>& Vector3D<T>::operator-=(const Vector3D<T>& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}



// Regular type
template <typename T>
inline bool operator==(const Vector3D<T>& a, const Vector3D<T>& b)
{ return a.x == b.x and a.y == b.y and a.z == b.z;}

template <typename T>
inline bool operator!=(const Vector3D<T>& a, const Vector3D<T>& b)
{ return !(a == b);}

// Suma de vectores
template <typename T>
inline Vector3D<T> operator+(const Vector3D<T>& a, const Vector3D<T>& b)
{
    auto c = a;
    return c += b;
}

template <typename T>
inline Vector3D<T> operator-(const Vector3D<T>& a, const Vector3D<T>& b)
{
    auto c = a;
    return c -= b;
}


// Producto por escalares
// Solo lo defino por la izquierda, como es habitual en matemáticas.
// E = tipo del escalar
template <typename T, typename E>
inline Vector3D<T> operator*(E lambda, const Vector3D<T>& v)
{
    return {v.x*lambda, v.y*lambda, v.z*lambda};
}

/// Producto escalar
template <typename T>
inline T dot_product(const Vector3D<T>& u, const Vector3D<T>& v)
{ return (u.x * v.x + u.y * v.y + u.z * v.z); }


/// Producto vectorial
template <typename T>
inline Vector3D<T> cross_product(const Vector3D<T>& u, const Vector3D<T>& v)
{ 
    return Vector3D<T>{u.y * v.z - u.z * v.y
		     , u.z * v.x - u.x * v.z
		     , u.x * v.y - u.y * v.x};
}
	

///// Módulo de un vector
// DUDA: ¿Qué tiene que devolver? T o double?
//template <typename T>
//inline T modulo(const Vector3D<T>& v)
//{ return sqrt(dot_product(v,v));}


// Impresión
template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Vector3D<T>& v)
{ return out << '(' << v.x << ", " << v.y << ", " << v.z << ')'; }



}// namespace

#endif
