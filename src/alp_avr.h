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

#ifndef __ALP_AVR_H__
#define __ALP_AVR_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones compartidas por visar y avr.
 *
 *   - COMENTARIOS: 
 *
 *   - TODO: cambiarle el nombre!!!  <--- moverlo a atd!!!
 *
 *   - HISTORIA:
 *           alp  - 31/03/2017 Escrito
 *
 ****************************************************************************/
namespace alp{

// TODO: revisar Nombre (lo uso en escala)
/*!
 *  \brief  Clase genérica a partir de la que generar todos los tipos
 *  \brief integrales.
 *
 *  Nombre = dummy type. Lo uso para que el compilador genere diferentes tipos
 *  dependiendo del nombre.
 *  Int = es el tipo que almacena
 *
 *  Ejemplo:
 *  \code
 *  
 *  struct Num_grados_nombre{};
 *  struct Num_steps_nombre{};
 *
 *  using Num_grados = Integral_type<Num_grados_nombre, int>;
 *  using Num_steps = Integral_type<Num_steps_nombre, int>;
 *
 *  \endcode
 *
 *  Num_grados y Num_steps son dos tipos distintos y los dos integrales.
 *
 */
template<typename Nombre, typename Int>
struct Integral_type{
public:
    /// Construimos el objeto
    Integral_type(){}	// es un regular type

    Integral_type(const Int& v):value{v}{}

    /// Operaciones +=
    Integral_type& operator+=(const Integral_type& b)
    {
	value += b.value;
	return *this;
    }

    Integral_type& operator-=(const Integral_type& b)
    {
	value -= b.value;
	return *this;
    }


    Integral_type& operator*=(const Integral_type& b)
    {
	value *= b.value;
	return *this;
    }


    Integral_type& operator/=(const Integral_type& b)
    {
	value /= b.value;
	return *this;
    }

    // operaciones ++
    Integral_type& operator++()
    {	
	++value;
	return *this;
    }

    Integral_type operator++(int)
    {	
	auto tmp = *this;
	++value;
	return tmp;
    }


    Integral_type& operator--()
    {	
	--value;
	return *this;
    }


    Integral_type operator--(int)
    {	
	auto tmp = *this;
	--value;
	return tmp;
    }

    // Comparaciones
    bool operator<(const Integral_type& b)
    {return value < b.value;}

    bool operator<=(const Integral_type& b)
    {return value <= b.value;}

    bool operator>(const Integral_type& b)
    {return value > b.value;}

    bool operator>=(const Integral_type& b)
    {return value >= b.value;}


    // Conversión implícita
    operator Int() {return value;}

    // Datos
    // Aunque en principio la idea no es acceder a este value, no hay nada
    // que prohiba acceder a él, por eso lo dejo accesible
    Int value;
};

template <typename N, typename I>
inline Integral_type<N, I> operator+(Integral_type<N, I> a,
                                     const Integral_type<N, I>& b)
{
    a += b;
    return a;
}

template <typename N, typename I>
inline Integral_type<N, I> operator-(Integral_type<N, I> a,
                                     const Integral_type<N, I>& b)
{
    a -= b;
    return a;
}

template <typename N, typename I>
inline Integral_type<N, I> operator*(Integral_type<N, I> a,
                                     const Integral_type<N, I>& b)
{
    a *= b;
    return a;
}


template <typename N, typename I>
inline Integral_type<N, I> operator/(Integral_type<N, I> a,
                                     const Integral_type<N, I>& b)
{
    a /= b;
    return a;
}



// Esto no compila en AVR (recordar que no existe ostream en los micros!!!)
//template <typename N, typename I>
//inline std::ostream& operator<<(std::ostream& out, const Integral_type<N,I>& x)
//{return out << x.value;}


/*!
 *  \brief  Clase genérica a partir de la que generar tipos de tipo bool
 *
 *  Nombre = dummy type. Lo uso para que el compilador genere diferentes tipos
 *  dependiendo del nombre.
 *
 *  Ejemplo:
 *  \code
 *  
 *  struct Conectar_AVR_a_VCC_n{};
 *  using Conectar_AVR_a_VCC = Bool_type<Conectar_AVR_a_VCC_n>;
 *
 *  \endcode
 *
 *  Conectar_AVR_a_VCC es un tipo bool.
 *
 */
template<typename Nombre>
struct Bool_type{
    Bool_type(bool v):value{v}{}

    // Conversión implícita
    operator bool() const {return value;}

    bool value;
};


}// namespace


#endif 


