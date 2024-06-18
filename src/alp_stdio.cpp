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

#include "alp_stdio.h"

#include <iostream>
#include <fstream>

namespace alp{

// Lee de /dev/tty un caracter
char tty()
{
    static std::ifstream tty{"/dev/tty"};
    if (!tty)
	throw std::runtime_error{"Error: can't open /dev/tty"};

    char c;
    if(tty.get(c))
	return c;

    else 
	return std::cin.eof();
}


}// namespace



