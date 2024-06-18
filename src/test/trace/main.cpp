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

#include "../../alp_trace.h"
#include "../../alp_exception.h"

#include <iostream>


void test_log()
{
    {
	alp::Log log{"prueba.txt"};

    for (int i = 0; i < 100; ++i)
	log << i << ' ';
    }

    std::ifstream log{"prueba.txt"};
    if (!log)
	throw alp::Excepcion{"No se generó el fichero!!!"};
    
    int j;
    for (int i = 0; i < 100 and log >> j; ++i){
	if (i != j){
	    std::cerr << "Error: grabado " << j << " en lugar de " << i << '\n';
	}
    }

}

void test_ctrace()
{
    alp::trace(2) << "esto es una prueba\n";
    alp::trace(3) << "otra\n";
}

int main()
{
try{
    test_log();
    test_ctrace();

}catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}
