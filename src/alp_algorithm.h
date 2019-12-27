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

#ifndef __ALP_ALGORITHM_H__
#define __ALP_ALGORITHM_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Ampliación de <algorithm>
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *	     A.Manuel Lopez - 17/02/2019 Escrito
 *
 ****************************************************************************/

#include <alp_find.h>	// familia de algoritmos find


namespace alp{

/// Copia [p0, pe) en v, borrando cualquier dato que tuviera v.
// TODO: cambiar orden: copy(p0, pe, q0) y no al revés.
template <typename Value, typename It>
inline void copy_new(It p0, It pe, std::vector<Value>& v)
{
    v.clear();

    v.resize(pe - p0);

    std::copy(p0, pe, v.begin());
}

}// namespace

#endif


