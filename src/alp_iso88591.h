// Copyright (C) 2021 Manuel Perez 
//           mail: <manuel2perez@proton.me>
//           https://github.com/amanuellperez/mcu
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


#pragma once

#ifndef __ALP_ISO88951_H__
#define __ALP_ISO88951_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Algunas funciones para manejar ISO-8895-1
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    31/10/2021 v0.0 Todo experimental!!!
 *
 ****************************************************************************/


namespace alp{

namespace iso88951{
static constexpr int num_char_codes = 256;

inline bool is_uppercase(unsigned char c)
{
    return (65 <= c and c <= 90)    // A..Z
	    or
	   (192 <= c and c <= 221  // Á..Ý
		and
	    c != 215); // 'x' signo de multiplicar
}



inline bool is_alpha(unsigned char c)
{
    return  (65 <= c and c <= 90)  // A..Z
	or
	    (97 <= c and c <= 122) // a..z
	or
	    (192 <= c and c <= 221 and c != 215) // Á..Ý
	or 
	    (224 <= c and c <= 253 and c != 247) // á..ý
	;

}


inline bool is_alnum(unsigned char c)
{
    return  is_alpha(c) 
	    or 
	    (48 <= c and c <= 57)  // 0..9
	;

}

unsigned char to_lower(unsigned char);
unsigned char to_lower_without_accents(unsigned char);

}// namespace

}// namespace

#endif


