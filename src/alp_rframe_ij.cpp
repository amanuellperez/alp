// Copyright (C) 2020 Manuel Perez <manuel2perez@proton.me>
//
// This file is part of the MCU++ Library.
//
// MCU++ Library is a free library: you can redistribute it and/or modify
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

#include "alp_rframe_ij.h"


namespace alp{

void Posiciones_bordes_rango_ij::next()
{
    if (i == 0){
	if (j < cols_ - 1)
	    ++j;
	else
	    ++i;
    }

    else if (j == cols_ - 1){
	if (i < rows_ - 1)
	    ++i;
	else
	    --j;
    }

    else if (i == rows_ - 1){
	if (j > 0)
	    --j;
	else
	    --i;
    }

    else if (j == 0) {
	if (i > 1)
	    --i;
	else
	    i = rows_; // end()
    }
}





}

