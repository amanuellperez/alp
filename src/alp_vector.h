// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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


#ifndef __ALP_VECTOR_H__
#define __ALP_VECTOR_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones que operan sobre std::vector
 *
 *
 *   - HISTORIA:
 *           A.Manuel L.Perez- 09/06/2019 Escrito
 *
 ****************************************************************************/
#include <vector>
#include <string>

namespace alp{

/// Imprime el vector, usando el separador indicado para separar elementos.
template <typename T>
std::ostream& print(std::ostream& out, const std::vector<T>& v
	    , const std::string& sep = " ")
{
    auto p = v.begin();
    auto pe = v.end();

    if (p == pe)
	return out;

    out << *p;
    ++p;

    for (; p != pe; ++p)
	out << sep << *p;

    return out;
}

}// namespace

#endif
