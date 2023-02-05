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

#ifndef __ALP_FIND_H__
#define __ALP_FIND_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Algoritmos find_pair
 *
 *   - COMENTARIOS: 
 *	 + Los algoritmos find_pair buscan pares de elementos que satisfacen
 *	o no una cierta relación o predicado. Hay 2 versiones:
 *	    find_pair	   = usa iteradores.
 *	    find_pair_i    = usa índices.
 *
 *	+ Los rangos son de la forma [j0, je) para mantener el convenio
 *	  de la STL. De esta forma incluimos el rango nulo de forma natural.
 *
 *	+ Al buscar un par podemos:
 *	    1.- encontrarlo, en cuyo caso el algoritmo devuelve el par 
 *		[i, i+1] buscado.
 *	    2.- no encontrarlo, en cuyo caso devuelve el par [i, i]. Como el par
 *		[i, i] nunca va a ser un par válido, lo usaremos para indicar
 *		"no encontrado".
 *
 *	+ Se suministran versiones diferentes de find:
 *	    find_pair	    = busca el primer par que satisface la relación rel.
 *	    find_pair_not   = busca el primer par que no satisface la relación.
 *	    find_pair_if    = busca el primer par que satisface un predicado.
 *	    find_pair_if_not= busca el primer par que no satisface el
 *			      predicado.
 *
 *	   Además, se suministran versiones rfind, para buscar en sentido
 *	   inverso.
 *
 *   - HISTORIA:
 *    Manuel Perez
 *	13/10/2017 Escrito. Basado en xp1.
 *	28/02/2018 Reestructurado y ampliado.
 *	16/02/2019 Traigo los finds de visar. 
 *		 TODO: revisar, están duplicados con diferentes argumentos!!!
 *
 ****************************************************************************/
#include <alp_concepts.h>
#include <utility>
#include <tuple>    // tie

namespace alp{
/// Devuelve la primera pareja de elementos [j0, j1] que satisface la relación
/// Solo busca dentro del intervalo [j0, je).
/// En caso de no encontrarlo devuelve una pareja no válida (j0 = j1 = je).
template <Contenedor_unidimensional C, typename R>
inline std::pair<int,int> find_pair_if_i(const C& c, int j0, int je, R rel)
{
    int j1 = j0 + 1;

    while (j1 < je and !rel(c[j0], c[j1])){
	++j0; ++j1;
    }

    if (j1 == je)
	return {je, je};

    return {j0, j1};
}


/// Devuelve la primera pareja de elementos [j0, j1] que no satisface la relación
/// Solo busca dentro del intervalo [j0, je).
/// En caso de no encontrarlo devuelve una pareja no válida (j0 = j1 = je).
template <Contenedor_unidimensional C, typename R>
inline std::pair<int,int> find_pair_if_not_i(const C& c, int j0, int je, R rel)
{
    int j1 = j0 + 1;

    while (j1 < je and rel(c[j0], c[j1])){
	++j0; ++j1;
    }

    if (j1 == je)
	return {je, je};

    return {j0, j1};
}



/// Devuelve la primera pareja de elementos [j0, j1] que satisface el predicado
/// f.
/// Solo busca dentro del intervalo [j0, je).
/// En caso de no encontrarlo devuelve una pareja no válida (j0 = j1 = je).
/// No pasamos el contenedor ya que estará dentro de func.
template <typename Func>
// requires: Functor(Func)
inline std::pair<int,int> find_pair_if_i(int j0, int je, Func func)
{
    auto j1 = j0 + 1;

    while (j1 < je and !func(j0, j1)){
	++j0; ++j1;
    }

    if (j1 == je)
	return {je, je};

    return {j0, j1};
}


/// Devuelve la primera pareja de elementos [j0, j1] que no satisface 
/// el predicado f.
/// Solo busca dentro del intervalo [j0, je).
/// En caso de no encontrarlo devuelve una pareja no válida (j0 = j1 = je).
/// No pasamos el contenedor ya que estará dentro de func.
template <typename Func>
// requires: Functor(Func)
inline std::pair<int,int> find_pair_if_not_i(int j0, int je, Func func)
{
    auto j1 = j0 + 1;

    while (j1 < je and func(j0, j1)){
	++j0; ++j1;
    }

    if (j1 == je)
	return {je, je};

    return {j0, j1};
}


//// Devuelve la primera pareja de elementos [j0, j1] que satisface la relación.
//// La búsqueda es en sentido inverso. 
//// En caso de no encontrarlo devuelve un intervalo de 1 elemento (no es un
//// pair ==> no se encontró nada). Esto es, devuelve j0 == j1 en caso de no
//// encontrar nada.
//// NOTA: en este caso se podría devolver -1 si no se encuentra nada, pero este
//// algoritmo no funcionaría con índices unsigned.
//template <typename R>
//inline std::pair<int,int> 
//    rfind_pair_if_ij(const Imagen& imgv, int i, int j1, R rel)
//{
//    int j0 = j1 - 1;
//
//    while (j0 >= 0 and !rel(imgv(i, j0), imgv(i,j1))){
//	--j0; --j1;
//    }
//
//    if (j0 < 0)
//	return {imgv.cols(), imgv.cols()};
//
//    return {j0, j1};
//}
//
//
//// Devuelve la primera pareja de elementos [j0, j1] que no satisface la relación.
//// La búsqueda es en sentido inverso.
//// En caso de no encontrarlo devuelve un intervalo de 1 elemento (no es un
//// pair ==> no se encontró nada). Esto es, devuelve j0 == j1 en caso de no
//// encontrar nada.
//template <typename R>
//inline std::pair<int,int> 
//    rfind_pair_if_not_ij(const Imagen& imgv, int i, int j1, R rel)
//{
//    int j0 = j1 - 1;
//
//    while (j0 >= 0 and rel(imgv(i, j0), imgv(i,j1))){
//	--j0; --j1;
//    }
//
//    if (j0 < 0)
//	return {imgv.cols(), imgv.cols()};
//
//    return {j0, j1};
//}


/*!
 *  \brief  Busca el primer par de elementos que cumplen la relación R.
 *
 *  Devuelve un par de iteradores apuntando a los elementos que cumplen la
 *  relación:
 *	    A x B x C < D	(x = no cumple R)
 *		    ^   ^	
 *  
 *  En caso de no encontrar dicho par, devuelve un par con dos iteradores
 *  iguales a pe (last).
 *
 *  Precondición: [p0, pe) tiene como mínimo 2 elementos.
 *
 */
template <typename It, typename R>
// requires: forward_iterator(It)
//	     predicate(R): R: *It x *It --> bool
inline std::pair<It, It> find_pair_if(It p0, It pe, R rel)
{
    // DUDA: de momento lo valido, pero puede que sea muy ineficiente
    // así que garantizar que tenga 2 elementos.
//    if (p0 == pe) 
//	return std::make_pair(pe, pe);
//
    auto p1 = siguiente(p0);
//
//    if (p1 == pe) 
//	return std::make_pair(pe, pe);

    while (p1 != pe and !rel(*p0, *p1)){
	p0 = p1;
	++p1;
    }

    if (p1 == pe)
	return std::make_pair(pe, pe);

    return std::make_pair(p0, p1);
}

template <typename It, typename R>
// requires: forward_iterator(It)
//	     predicate(R): R: *It x *It --> bool
inline std::pair<It, It> find_pair_if_not(It p0, It pe, R rel)
{
    // DUDA: de momento lo valido, pero puede que sea muy ineficiente
    // así que garantizar que tenga 2 elementos.
//    if (p0 == pe) 
//	return std::make_pair(pe, pe);
//
    auto p1 = siguiente(p0);
//
//    if (p1 == pe) 
//	return std::make_pair(pe, pe);

    while (p1 != pe and rel(*p0, *p1)){
	p0 = p1;
	++p1;
    }

    if (p1 == pe)
	return std::make_pair(pe, pe);


    return std::make_pair(p0, p1);
}


/*!
 *  \brief  Busca el primer par de elementos que cumplen la relación R.
 *
 *  La búsqueda se hace en sentido inverso: se va desde p1 hasta pb = begin, 
 *  donde begin es el inicio del rango (*begin es un valor válido). Lo busca
 *  en el intervalo [pb, p1] yendo hacia atrás (de p1 a pb). Observad que no
 *  es un intervalo semiabierto.
 *
 *  Devuelve un par de iteradores apuntando a los elementos que cumplen la
 *  relación:
 *	    A < B x C x D	(x = no cumple R)
 *	    ^   ^ 
 *  
 *  En caso de no encontrar dicho par, devuelve un par con dos iteradores
 *  iguales [p, p) = conjunto vacío!!! Notar que aquí no tendría ningún
 *  sentido devolver last, ya que no conocemos last!!!
 *
 *  Precondición: [p0, pb) tiene como mínimo 2 elementos.
 *
 */
// Ejemplo: rfind_if_pair(p, v.begin(), std::less<int>{});
template <typename It, typename R>
// requires: bidirectional_iterator(It)
//	     predicate(R): R: *It x *It --> bool
inline std::pair<It, It> rfind_pair_if(It p1, It pb, R rel)
{
    auto p0 = anterior(p1);

    while (p0 != pb and !rel(*p0, *p1)){
	p1 = p0;
	--p0;
    }

    if (p0 == pb and !rel(*p0, *p1))
	return std::make_pair(pb, pb);

    return std::make_pair(p0, p1);
}

// Ejemplo: rfind_pair_if_not(p, v.begin(), std::less<int>{});
template <typename It, typename R>
// requires: bidirectional_iterator(It)
//	     predicate(R): R: *It x *It --> bool
inline std::pair<It, It> rfind_pair_if_not(It p1, It pb, R rel)
{
    // precondición: [pb, p1] tiene mínimo 2 elementos (está definido
    // anterior(p1).
    auto p0 = anterior(p1);


    while (p0 != pb and rel(*p0, *p1)){
	p1 = p0;
	--p0;
    }

    // No hemos mirado rel(*p0, *p1) cuando p0 = pb. Lo miramos.
    if (p0 == pb and rel(*p0, *p1))
	return std::make_pair(pb, pb);

    return std::make_pair(p0, p1);
}

/*!
 *  \brief  Busca el primer rango [first, last) que esté "ordenado" dentro
 *  \brief de [p0, p1].
 *
 *  \params
 *	[f0, fe) = rango donde queremos buscar nuestro range (sería la fila
 *		   de una imagen o un std::vector)
 *	[p0, pe) = el rango de salida tiene que solaparse con este rango.
 *
 *  \return
 *	[q0, qe) = rango "ordenado" via R, tal que (si los iteradores de los
 *		    que hablo están dentro de [f0, fe)):
 *		   1.- El par q0-1, q0 no está ordenado via R.
 *		   2.- El par qe-1, qe no está ordenado via R.
 *		   3.- [q0, qe) está ordenado via R (esto es, todo par de
 *		   elementos adyacentes de [q0, qe) satisface R).
 *		   4.- [q0, qe) intersección [p0, pe) es no nula (por
 *		   supuesto, siempre que [q0, qe) no sea el conjunto vacío).
 *
 *  Devuelve un rango vacío (q0 = qe = fe) si no se encuentra rango.
 */
template <typename It, typename R>
// requires: bidirectional_iterator(It)
//	     predicate(R): R: *It x *It --> bool
std::pair<It, It> find_first_range_overlapped_if(It f0, It fe, It p0, It pe,
                                                 R rel)
{
    // precondición: [p0, pe) incluido en [f0, fe)

    // 1.- Garantizamos que haya mínimo 2 elementos en [p0, pe)
    if (p0 == pe) 
	return std::pair<It, It>{fe, fe};

    auto p1 = siguiente(p0);

    if (p1 == pe) 
	return std::pair<It, It>{fe, fe};

    // 2.- Buscamos el rango
    // 2.1.- Calculamos q0.
    auto q0 = p0;
    if (rel(*p0, *p1)){
	if (p0 != f0)	// en este caso, está definido --p0. Puedo llamar rfind.
	    // std::tie(std::ignore, q0) = rfind_pair_if_not(p0, f0, rel);
	    std::tie(std::ignore, q0) = rfind_pair_if_not(p0, f0, rel);
    }
    else{ // p0, p1 están definidos. Puedo llamar find_pair
//	It y0, y1;
//	std::tie(y0, y1) = find_pair_if(p0, pe, rel);
	auto [y0, y1] = find_pair_if(p0, pe, rel);
	if (y0 == y1)	// no encontró nada
	    return std::pair<It,It> {fe, fe};

	q0 = y0;
    }


    // 2.2.- Calculamos q1.
    // Aquí q0 apunta al primer elemento de un par que satisface R, luego
    // q0 < q0+1 ==> find ha comprobado que está definido q0+1. No necesito
    // comprobarlo.
//    It q1;
//    std::tie(std::ignore, q1) = find_pair_if_not(q0, fe, rel);
    auto [ignoralo, q1] = find_pair_if_not(q0, fe, rel);

    if (q1 == fe)
	return std::pair<It, It>{q0, fe};

    return std::pair<It, It>{q0, q1};

}



}// namespace

#endif


