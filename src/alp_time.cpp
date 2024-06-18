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

#include "alp_time.h"
#include "alp_math.h"

namespace alp{

/// Indica si el año year es bisiesto o no
bool es_bisiesto(int year)
{
    // Un año bisiesto si es múltiplo de 4
    if (es_multiplo(year).de(4)){
	if (!es_multiplo(year).de(100))
	    return true;

	// de los que acaban en 00 (múltiplos de 100) son bisiestos los 
	// múltiplos de 400
	if(es_multiplo(year).de(400))
	    return true;
    }

    return false;
}

}// namespace
