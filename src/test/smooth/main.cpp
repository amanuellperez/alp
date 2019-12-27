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

#include "../../alp_smooth.h"
#include "../../alp_test.h"
#include "../../alp_string.h"	// as_str() 

#include <iostream>

using namespace std;
using namespace alp;
using namespace test;

void test_mediana(int a, int b, int c, int res)
{
    CHECK_TRUE((mediana(a, b, c) == res), "mediana");
}



void test_mediana()
{
    test::interfaz("mediana");
    test_mediana(2, 2, 2, 2);

    test_mediana(2, 6, 4, 4);
    test_mediana(4, 2, 6, 4);
    test_mediana(4, 6, 2, 4);
    test_mediana(6, 2, 4, 4);
    test_mediana(6, 4, 2, 4);
    test_mediana(6, 4, 2, 4);
}

void test_smooth3()
{
    std::vector in{4,7,3,9,8,7,11,12,1320,15,10};

    std::cout << "in : ";
    for (const auto& x: in)
	std::cout << x << ' ';
    std::cout << '\n';

    constexpr int num_max_repeticiones = 10;

    for (int i = 0; i < num_max_repeticiones; ++i){
	int n = smooth3(in.begin(), in.end());
	std::cout << "out: ";
	for (const auto& x: in)
	    std::cout << x << ' ';
	std::cout << '\n';
	std::cout << "núm. cambiados = " << n << '\n';
	if (n == 0)
	    break;
    }

}

int main()
{
try{
    test::header("alp_smooth");

    test_mediana();
    test_smooth3();

}catch(std::exception& e)
{
    std::cerr << e.what() << endl;
    return 1;
}
}
