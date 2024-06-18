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

#ifndef __ALP_CAST_H__
#define __ALP_CAST_H__

#include <string>

#include "alp_exception.h"
#include "alp_string.h"

namespace alp{

/****************************************************************************
 *
 *   - FUNCIÓN: narrow_cast
 *
 *   - DESCRIPCIÓN: Permite hacer un narrow_cast seguro
 *
 *   - EJEMPLO: 
 *		unsigned int i = 10;
 *		int j = narrow_cast<int>(10);
 *
 ****************************************************************************/
// De Stroustrup: garantizamos que funcionen los narrows cast
// Si se quiere convertir a float, puede que sea interesante
// sustituir != por esta_entre


class Fail_cast:public Error_de_programacion{
public:
    Fail_cast(const std::string& funcion, const std::string& detalles);
};


template<typename Target, typename Source>
Target narrow_cast_int(Source v)
{
    auto r = static_cast<Target>(v);

    if (static_cast<Source>(r) != v){
	throw std::logic_error(
	    as_str() << "narrow_cast_int: Can't cast [" << v << "] to [" << r << "]");
    }

    return v;
}


template<typename Target, typename Source>
Target narrow_cast_float(Source v)
{
    auto r = static_cast<Target>(v);

    if (!(v-1 <= static_cast<Source>(r) and static_cast<Source>(r) <=v+1)){
	throw Fail_cast("narrow_cast_float", 
		    as_str() << "[" << v << "] -> [" << r << "]");
    }

    return v;
}


template<typename Target, typename Source>
inline Target narrow_cast(Source v)
{
    if constexpr (std::is_same_v<Target, Source>)
	return v;

    else if constexpr (std::is_integral<Source>::value) 
	return narrow_cast_int<Target>(v);

    else
	return narrow_cast_float<Target>(v);
}


/// Uno de los problemas que tiene C++ heredado de C son los castings
/// automáticos de signed y unsigned. Por eso necesito esta función.
/// La función inversa a esta es: static_cast<char>(i); // i = int
inline int char2int(char c)
{
    int y = (static_cast<int>(c) & 0xFF);

    if (static_cast<char>(y) != c)
        throw alp::Fail_cast{"char2int",
                             alp::as_str() << "Error al convertir [" << c
                                           << "] -> [" << y << "]\n"};
    return y;
}


}// namespace


#endif


