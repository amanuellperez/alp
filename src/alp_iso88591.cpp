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


#include "alp_iso88591.h"

namespace alp{
namespace iso88951{
unsigned char to_lower(unsigned char c)
{
    if ( 65 <= c and c <= 90)	// A..Z
	return 97 + (c - 65);	

    if (c == 215)   // excluimos el signo de multiplicación
	return c;

    if (192 <= c and c <= 221)
	return 224 + (c - 192);

    return c;
}

unsigned char to_lower_without_accents(unsigned char c)
{
// case c == 'a':
    if (c == 0x41 // 'A'
	  or
	(0xC0 <= c and c <= 0xC5)  // Á...
	  or
	(0xE0 <= c and c <= 0xE5) // á
	)
	    return 0x61; // a

//  case c == 'e':
    if (c == 0x45 // E
	    or
	(0xC8 <= c and c <= 0xCB) // É
	    or
	(0xE8 <= c and c <= 0xEB) // é
       )
	    return 0x65; // e

//  case c == 'i':
    if (c == 0x49   // I
	    or
	(0xCC <= c and c <= 0xCF) // Í
	    or
	(0xEC <= c and c <= 0xEF) // í
       )
	    return 0x69;    // i

//  case c == 'o':
    if (c == 0x4F   // O
	    or
	(0xD2 <= c and c <= 0xD6)   // Ó
	    or
	(0xF2 <= c and c <= 0xF6)
       )
	    return 0x6F;

//  case c == 'u':
    if (c == 0x55   // U
	    or
	(0xD9 <= c and c <= 0xDC)
	    or
	(0xF9 <= c and c <= 0xFC)
       )
	    return 0x75;    // u

// default:
    return to_lower(c);
}

}// namespace
}// namespace
