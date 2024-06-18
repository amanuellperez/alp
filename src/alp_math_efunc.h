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

#ifndef __ALP_MATH_EFUNC_H__
#define __ALP_MATH_EFUNC_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Estudio del crecimiento y decrecimiento de una función.
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           Manuel Perez- 17/02/2019 Escrito
 *
 ****************************************************************************/
#include "alp_find.h"
#include "alp_math.h"
#include <ostream>

namespace alp{
/*!
 *  \brief  Estudiamos el crecimiento y decrecimiento de una función.
 *
 */
class Estudia_crecimiento_decrecimiento{
public:
    // Region de crecimiento: es un intervalo (i, j) donde decimos si es
    // creciente o decreciente la función.
    struct Region{
	int i, j;
	int incr;   // incremento de y de la región (incr = p[j] - p[i])
	char name;  // nombre imprimible de la región: <, =, >...
    };


    /*!
     *  \brief  Función objeto que devuelve true si m <= y1 - y0 < M
     *  
     *  Si m != M:
     *	    Nos dice si el incremento que hay al pasar de y0 a y1
     *  pertenece al intervalo [m, M).
     *
     *  Si m == M:
     *	    Nos dice si y0 == y1.
     *
     *	Uso m == M para poder incluir la relación de igualdad de forma
     *	sencilla. Si no fuera por este caso particular, esto sería una
     *	relación genérica: pertenece_al_intervalo(m, M, 'C'); <--
     *	el nombre 'C' también es algo particular del algoritmo. No es
     *	genérico. Por eso no lo saco de aquí.
     *
     */
    struct Relacion{
	int m, M;	// mínimo y máximo

	char name;


	Relacion(int m0, int M0, char name0)
	    :m{m0}, M{M0}, name{name0} {}

	bool operator()(int y0, int y1) const 
	{
	    if (m == M) // operator ==
		return y0 == y1;

	    // operator <
	    int incr = y1 - y0;

	    if (incr >= M or incr < m)
		return false;

	    if (m != 0){
		if (incr >= m) return true;
	    }else // m == 0
		if (incr > m) return true;

	    return false;
	}
    };

    using Estudio = std::vector<Region>;

    Estudia_crecimiento_decrecimiento
	(std::initializer_list<Relacion> rels)
	:rels_{rels}
	{}

    /*!
     *  \brief  Estudia el crecimiento decrecimiento de una función.
     *
     *	\params
     *	    p = iterador al primer elemento de la sucesión.
     *	    ie = índice del último elemento = número de elementos de la sucesión.
     *
     *	\return 
     *	    Devuelve un vector con las distintas regiones de la función.
     */
    template <typename It, typename Size>
    Estudio operator()(It p, Size ie);


    /*!
     *  \brief  Expande el estudio del crecimiento/decrecimiento.
     *
     *  Lo que hago es que cada región [a,b]C+3, la expando en 
     *  CCCCC...C
     *
     *  Esto va a ser útil para marcar.
     *  La función objeto f, decide cómo expandimos. 
     *  Por ejemplo:
     *	    1.- Para hacer el test, expando [3,7]C en CCC
     *	    2.- En las imágenes, expando [3,7]C convirtiendo C en un color.
     *  
     *  Las regiones [4,7]c[7,10], tienen en común el punto 7.
     *  ¿Cómo colorear esto? Con color_bordes. ¿Cómo colorear 
     *  el interior? color_bordes
     *  
     *  \params
     *	[p0, pe) = rango donde almacenamos el estudio expandido.
     *  color = color con el que pintamos el interior de cada región.
     *  color_bordes = color con el que pintamos los bordes de cada región.
     *
     *  \return Devuelve el último elemento + 1 que se coloreo.
     *
     */
    template <typename It, typename Color, typename ColorBordes>
    // requires: is_iterator(It)
    //	    is_functor(Color)
    //
    // Precondición: sizeof([p0, pe)) >= estudio expandido
    It expande(const Estudio& est
		, It p0, It pe, Color color, ColorBordes color_bordes);
    

private:
    std::vector<Relacion> rels_;

    // Determina qué relación existe entre x e y. Devuelve esa relación
    // para poder continuar haciendo el estudio.
    template <typename Value>
    const Relacion& relacion_existente_entre(Value x, Value y)
    {
	for (const auto& rel: rels_)
	    if (rel(x, y)) 
		return rel;

        throw Imposible_llegar_aqui{
            __FILE__,
            __LINE__,
            "Estudia_crecimiento_decrecimiento::relacion_existente_entre()"};
    }

};



template <typename It, typename Size>
Estudia_crecimiento_decrecimiento::Estudio 
	Estudia_crecimiento_decrecimiento::operator()(It p, Size ie)
{
    std::vector<Region> region;

    Size i0 = 0;
    Size i1 = 1;

    Region reg;
    while (i1 < ie){
	Relacion rel = relacion_existente_entre(p[i0], p[i1]);
	
	reg.i = i0;

	std::tie(i0, i1) = find_pair_if_not_i(p, i0, ie, rel);

	i1 = i0 + 1;

	if (i0 == ie) --i0;

	reg.j = i0;	// intervalo [i, j]
	reg.incr = p[reg.j] - p[reg.i];
	reg.name = rel.name;

	region.push_back(reg);
    }

    return region;
}


template <typename It, typename Color, typename ColorBordes>
// requires: is_iterator(It)
//	    is_functor(Color)
//
// Precondición: sizeof([p0, pe)) >= estudio expandido
It Estudia_crecimiento_decrecimiento::expande(const Estudio& est
	    , It p0, It pe, Color color, ColorBordes color_bordes)
{
    for (auto rg = est.begin(); rg != est.end(); ++rg){
	*p0 = color_bordes();

	p0 = std::next(p0);

	p0 = std::fill_n(p0, rg->j - rg->i -  1, color(rg->name));
    }

    *p0 = color_bordes();

    return p0;
}


inline std::ostream&
operator<<(std::ostream& out,
           const Estudia_crecimiento_decrecimiento::Region& reg)
{
    return out << '[' << reg.i << ", " << reg.j << ']' << reg.name << reg.incr;
}

}// namespace

#endif

