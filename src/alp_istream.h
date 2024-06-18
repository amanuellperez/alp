// Copyright (C) 2022 Manuel Perez 
//           mail: <manuel2perez@proton.me>
//           https://github.com/amanuellperez/mcu
//
// This file is part of the ALP Library.
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

#ifndef __ALP_ISTREAM_H__
#define __ALP_ISTREAM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Ampliación de std::istream
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    12/07/2022 read(in, bytes, n)
 *
 ****************************************************************************/
#include <istream>
#include <cstddef>

namespace alp{

// TODO: mover esta función a atd 
inline 
std::istream& read(std::istream& in, std::byte* b, std::streamsize n)
{
    using char_t = std::istream::char_type;
    return in.read(reinterpret_cast<char_t*>(b), n);
}


}// namespace alp

#endif



