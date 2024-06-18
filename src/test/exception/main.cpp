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

#include "../../alp_exception.h"
#include "../../alp_test.h"

#include <iostream>

using namespace std;
using namespace test;
using namespace alp;

template <typename E>
void lanza(const string& name)
{
    try{ throw E{name};}
    catch(E& e)
    {
	CHECK_TRUE(e.what() == name, name);
    }
}




void tst()
{
    test::interfaz("Probando excepciones");

    lanza<Excepcion> ("Excepcion");
}

int main()
{
try{
    test::header("alp_exception");
    tst();

}catch(const std::exception& e){
    std::cout << e.what() << '\n';
}
}
