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

#ifndef __ALP_TYPE_TRAITS_H__
#define __ALP_TYPE_TRAITS_H__

/****************************************************************************
 *
 *   - DESCRIPCION: Funciones para hacer metaprogramming
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           Manuel Perez - 13/10/2017 Reestructurado.
 *
 ****************************************************************************/
#include <type_traits>
#include <cstddef>   // std::size_t, ptrdiff_t
#include <vector>
#include <string>
#include <atd_type_traits.h>

namespace alp{
// MOVIDO A atd
//// Tipo que permite hacer:
//// static_assert(always_false<T>::value, "aqui no debe de llegar");
//template <typename T>
//struct always_false_type : std::false_type { };
//
//template <typename T>
//inline constexpr bool always_false_v = always_false_type<T>::value;


/***************************************************************************
 *			    has_xxx_member
 ***************************************************************************/
// ¿es un iterador?
// ----------------
template <typename, typename = std::void_t<>>
struct is_iterator : std::false_type { };

template <typename T>
struct is_iterator<T*, std::void_t<>> : std::true_type { };


template <typename T>
struct is_iterator <T, std::void_t<
		   typename T::iterator_category,
		   typename T::value_type,
		   typename T::reference,
		   typename T::pointer, 
		   typename T::difference_type> >
		   : std::true_type { };

template <typename T>
inline constexpr bool is_iterator_v = is_iterator<T>::value;


// ¿la clase tiene iterator?
// -------------------------------
template <typename, typename = std::void_t<>>
struct has_iterator: std::false_type { };

template <typename T>
struct has_iterator<T, std::void_t<typename T::iterator>>
    : std::true_type { };

template <typename T>
inline constexpr bool has_iterator_v = has_iterator<T>::value;

// ¿la clase tiene const_iterator?
// -------------------------------
template <typename, typename = std::void_t<>>
struct has_const_iterator: std::false_type { };

template <typename T>
struct has_const_iterator<T, std::void_t<typename T::const_iterator>>
    : std::true_type { };

template <typename T>
inline constexpr bool has_const_iterator_v = has_const_iterator<T>::value;


// ¿la clase tiene value_type?
// ---------------------------
template <typename, typename = std::void_t<>>
struct has_value_type : std::false_type { };

template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>>
	    : std::true_type { };

template <typename T>
inline constexpr bool has_value_type_v = has_value_type<T>::value;


/***************************************************************************
 *			Value_type, ...
 ***************************************************************************/
// cuando el tipo T es const, el value tiene que ser const!!!
template<typename T>
using Value_type = std::conditional_t<std::is_const_v<T>, 
			    const typename T::value_type,
			    typename T::value_type>;


// cuando el tipo T es const, la reference tiene que ser const!!!
template<typename T>
using Reference_type = std::conditional_t<std::is_const_v<T>, 
			    typename T::const_reference,
			    typename T::reference>;

template<typename T>
using Pointer_type = std::conditional_t<std::is_const_v<T>, 
			    typename T::const_pointer,
			    typename T::pointer>;


// cuando el tipo T es const, el value tiene que ser const!!!
template<typename T>
using Return_type = std::conditional_t
			    <std::is_const_v<T>, 
			    const typename T::return_type,
			    typename T::return_type>;

// cuando el tipo T es const, el iterador tiene que ser const_iterator!!!
template<typename T>
using Iterator_type = 
    std::conditional_t <std::is_const_v<T>, 
				    typename T::const_iterator, 
				    typename T::iterator>;

template<typename C>
using const_Iterator_type = typename C::const_iterator;


//template<typename T>
//using Result_of = std::result_of_t<T>;


// Añade o quita a T el const, haciendo que tenga el mismo tipo de const que C
// (same const T as C)
template <typename T, typename C>
using Same_const_as = typename std::conditional
			<std::is_const_v<C>,
			const T,
			std::remove_const_t<T>>::type;



// Es la función identidad: f(x) = x;
template <typename T>
struct Identidad{
    T& operator()(T& x) const {return x;}
    const T& operator()(const T& x) const{return x;}
};


// C++20 implementa remove_cvref_t. Como todavía gcc no lo tiene, lo
// implemento yo. Eliminar esto cuando ya exista remove_cvref_t
template <typename T>
struct remove_cvref{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};


template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;


// Extraemos el puntero subyacente de T
template <typename T>
struct remove_cvpointer{
    using type = std::remove_cv_t<std::remove_pointer_t<T>>;
};


template <typename T>
using remove_cvpointer_t= typename remove_cvpointer<T>::type;



/***************************************************************************
 *			    VIEW_TRAITS
 ***************************************************************************/
/*!
 *  \brief  Traits de una view
 *
 *  Una View es una forma de ver a un objeto T.
 *  Por ejemplo:
 *	1.- Una view de un ColorRGB puede ser el color red.
 *	2.- Otra podría ser ver la intensidad del color.
 *
 *  Matemáticamente una view no es más que una función
 *
 *			T ----> R	ó	  R = view(T);
 *
 *  Dependiendo de lo que vemos value_type, reference and pointer tendrán uno
 *  u otro valor:
 *		|	  T	    |	    T&		|    const T&
 *  ------------+-------------------+-------------------+--------------------
 *  value_type	|	  R	    |	    R		|	R
 *  reference	|    const R	    |	    R&		|   const R&
 *  const_refer |    const R	    |	const R&	|   const R&
 *  pointer	|	void	    |	    R*		|   const R*
 *  const_point |	void	    |	const R*	|   const R*
 *  
 *  El void indica "no definido".
 */
template <typename Return_type>
struct View_traits_helper // Return_type = T 
{
    // Tipos que vemos
    using value_type	  = Return_type;
    using reference       = const Return_type;
    using pointer         = void;
};

template <typename R>
struct View_traits_helper<R&> 
{
    // Tipos que vemos
    using value_type	  = R;
    using reference       = R&;
    using pointer         = R*;
};


template <typename R>
struct View_traits_helper<const R&> 
{
    // Tipos que vemos
    using value_type	  = R;
    using reference       = const R&;
    using pointer         = const R*;
};

// F = Función objeto
// Arg = argumento que le pasamos.
//
// Esta View_traits está pensada para ser usada con iteradores
//
template <typename F, typename Arg>
struct View_traits: public View_traits_helper<std::invoke_result_t<F, Arg>>
{ };


/***************************************************************************
 *			    ITERATOR_TRAITS
 ***************************************************************************/
// Convierte un tipo iterator It en el iterator correspondiente
// Básicamente:
//	si It tiene It::iterator devuelve este
//	si It=T* es puntero devuelve T*
//	si It es el iterador de un vector devuelve vector::iterator
//	...
template <typename It>
constexpr decltype(auto) iterator_to_iterator()
{
    static_assert(is_iterator_v<It>, "It tiene que ser un iterador");

    if constexpr (has_iterator_v<It>)
	return typename It::iterator{};

    else if constexpr (std::is_pointer_v<It>){
	using value_type = remove_cvpointer_t<It>;
	using pointer = value_type*;
	return pointer{0};
    }
    else if constexpr (has_value_type_v<It>){
	using value_type = typename It::value_type;
        if constexpr (std::is_same_v<typename std::vector<value_type>::iterator,
                                     It>)
            return typename std::vector<value_type>::iterator();

        else if constexpr (std::is_same_v<
                               typename std::vector<value_type>::const_iterator,
                               It>)
            return typename std::vector<value_type>::iterator();

        else if constexpr (std::is_same_v<typename std::string::iterator, It>)
            return typename std::string::iterator();

        else if constexpr (std::is_same_v<typename std::string::const_iterator,
                                          It>)
            return typename std::string::iterator();

	else static_assert(atd::always_false_v<It>,
                               "El tipo tiene value_type. Si es un "
                               "iterador es de un contenedor que todavía no he "
                               "implementado la conversión. Implementarla.");
    }
    else 
	static_assert(atd::always_false_v<It>,
            "Falta implementar este caso o no es un iterador lo pasado");
}



// Convierte un iterator It en el const_iterator correspondiente
// Básicamente:
//	si It tiene It::const_iterator devuelve este
//	si It=T* es puntero devuelve const T*
//	si It es el iterador de un vector devuelve vector::const_iterator
//	...
template <typename It>
constexpr decltype(auto) iterator_to_const_iterator()
{
    static_assert(is_iterator_v<It>, "It tiene que ser un iterador");

    if constexpr (has_const_iterator_v<It>)
	return typename It::const_iterator{};

    else if constexpr (std::is_pointer_v<It>){
	using value_type = remove_cvpointer_t<It>;
	using const_pointer = const value_type*;
	return const_pointer{0};
    }
    else if constexpr (has_value_type_v<It>){
	using value_type = typename It::value_type;
        if constexpr (std::is_same_v<typename std::vector<value_type>::iterator,
                                     It>)
            return typename std::vector<value_type>::const_iterator();

        else if constexpr (std::is_same_v<
                               typename std::vector<value_type>::const_iterator,
                               It>)
            return typename std::vector<value_type>::const_iterator();

        else if constexpr (std::is_same_v<typename std::string::iterator, It>)
            return typename std::string::const_iterator();

        else if constexpr (std::is_same_v<typename std::string::const_iterator, It>)
            return typename std::string::const_iterator();

	else static_assert(atd::always_false_v<It>,
                               "Error: el tipo tiene value_type. Si es un "
                               "iterador es de un contenedor que todavía no he "
                               "implementado la conversión. Implementarla.");

    }
    else 
	static_assert(atd::always_false_v<It>,
            "Error: falta implementar este caso o no es un iterador lo pasado");
}

// std::iterator_traits no dice cómo convertir un iterator to const_iterator,
// lo cual es un claro olvido (los iteradores tienen que funcionar como los
// punteros y si tienes un puntero int* el tipo const es 'const int*', ¿por
// qué no podemos hacer lo mismo con iteradores?)
template <typename It>
struct iterator_traits{
    using iterator       = decltype(iterator_to_iterator<It>());
    using const_iterator = decltype(iterator_to_const_iterator<It>());
};

}// namespace

#endif
