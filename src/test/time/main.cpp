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

#include "../../alp_time.h"
#include "../../alp_exception.h"
#include "../../alp_math.h"
#include "../../alp_test.h"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace alp;
using namespace test;

void f() 
{
    cout << '.';
}

void test_cronometro()
{
    Cronometro c;
    Cronometro_high_resolution c2;
    c.start();
    c2.start();

    for (size_t i = 0; i < 1000000; ++i) 
	cout << '.';

    cout << '\n';

    c.stop();
    c2.stop();
    cout << "Tiempo: " << c.milisegundos() << " ms\n";
    cout << "Tiempo: " << std::setw(6) << std::fixed
			<< std::setprecision(0) << 
			c2.nanosegundos() << " ns\n";
}



void test_es_bisiesto()
{
//    cout << "Años bisiestos\n"
//	 << "--------------\n";

    int n = 0; // número de años bisiestos
    for (int i = 0; i < 400; ++i){
	if (es_bisiesto(i)){
//	    cout << i << "\n";
	    ++n;
	}
    }
    
//    cout << "\n\nn = " << n << "\n";
    CHECK_TRUE(n == 97, "es_bisiesto()");
}



int main()
{
try{
    // test_cronometro();
    test_es_bisiesto();
}catch(const std::exception& e){
    std::cout << e.what() << '\n';
    return 1;
}
}
