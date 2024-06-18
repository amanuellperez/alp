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

#ifndef __ALP_CONCEPTS_H__
#define __ALP_CONCEPTS_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Definición de conceptos propios.
 *
 *   - COMENTARIOS: 
 *	 Usar nombres largos para los defines, para evitar colisiones
 *	 de nombres.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	13/10/2017 Escrito
 *
 ****************************************************************************/

// Container
// ---------
#define Contenedor_unidimensional typename
// C::value_type = tipo que se almacena en el contenedor
// C::Ind	 = tipo del índice
// C::size_type  = tipo de la longitud
// c[i]		 = elemento almacenado en la posición i
// c.size()      = número de elementos almacenados


// Realmente el contenedor bidimensional que defino es un contenedor con
// acceso aleatorio. ¿Qué sería un contenedor bidimensional básico? ¿Aquel en
// el que puedo hacer un barrido horizontal usando iteradores por filas? (???)
//
// Container2D
// -----------
#define Contenedor_bidimensional typename
// OJO: No puede depender del sistema de coordenadas (i, j) ó (x, y) que
// usemos!!!
//
// Tiene que quedar garantizado que es continuo (???) para poder ser eficiente
// is_regular(C)
//
// Queda garantizado que se puede concebir como un array unidimensional
// continuo.
//
// Datos que contiene
// ------------------
// C::value_type = tipo que se almacena dentro del contenedor.
// C::reference  = referencia a un elemento del contenedor = value_type&
// C::pointer	 = puntero a un elemento del contenedor = value_type*
//
//
// Como contenedor unidimensional
// ------------------------------
// C::Ind = tipo de los índices que usamos (ya sea (i,j) ó de (x,y))
//
// c.size() = número de elementos total del contenedor
//
// c.begin()
// c.end()
//
//
// Como contenedor bidimensional
// ------------------------------
// C::Posicion = tipo del índice bidimensional
//
// C::Size = tipo que nos da el Size del contenedor (las dimensiones)
//
// c.size2D() = size del contenedor. 
//	    Será (rows, cols) si usamos coordenadas (i, j) 
//	    ó será (ancho, alto) si usamos coordenadas (x, y).
//
// c.extension() = extensión que ocupa dentro del sistema de referencia. 
//	    Es un rectángulo definido por [p0, p1].
//	    Notar que un contenedor bidimensional puede ser una view, en cuyo
//	    caso p0 != (0, 0)!!! La extensión nos dice en qué punto del
//	    sistema de referencia empezamos.
//
//
//  row_begin()/row_end(): iteradores para las filas
//



#endif


