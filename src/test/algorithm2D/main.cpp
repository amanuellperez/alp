// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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


#include "../../alp_algorithm2D.h"
#include "../../alp_test.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace alp;
using namespace std;
using namespace test;



void test_IO()
{
    auto m = read_matrix<int>("prueba.txt");

    for (auto f = m.row_begin(); f != m.row_end(); ++f){
	for (auto x: *f)
	    cout << x << ' ';
	cout << '\n';
    }

    cout << '\n';
}




void test_diferencias()
{
    test::interfaz("adjacent_difference2D");

    Matrix<int> m{4, 4};

    vector m0 = {4, 8, 10, 10,
		 3, 9, 7, 6,
		 2, 7, 6, 6,
		 1, 5, 4, 3};

    
    std::copy(m0.begin(), m0.end(), m.begin());

    Matrix<int> m1{m.size2D()};

    adjacent_difference2D(m.row_begin(), m.row_end(), m1.row_begin());

    vector res = {4, 4, 2, 0,
		  3, 6, -2, -1,
		  2, 5, -1, 0,
		  1, 4, -1, -1};

    CHECK_EQUAL_CONTAINERS(m1.begin(), m1.end(), res.begin(), res.end()
			    , "adjacent_difference2D");
}

struct Punto{int x, y;};



void test_diferencias2()
{
    test::interfaz("adjacent_difference2D (2)");

    Matrix<Punto> img1{2, 2};

    vector<Punto> m0 = {{1, 10}, {5, 20},
		        {2, 30}, {-4, 40}};
    
    std::copy(m0.begin(), m0.end(), img1.begin());

    Matrix<int> incr{img1.size2D()};

    auto p_x = [](Punto& p) {return p.x;};

    auto rb = img1.row_begin();


AQUIII: probar por columnas
    auto f0 = row_iterator_view(rb, p_x);
//    auto f1 = img1.row_end();
//    auto g0 = incr.row_begin();
//

//    adjacent_difference2D(iterator_view(m.row_begin(), m.row_end(), m1.row_begin());
//
//    vector res = {4, 4, 2, 0,
//		  3, 6, -2, -1,
//		  2, 5, -1, 0,
//		  1, 4, -1, -1};
//
//    CHECK_EQUAL_CONTAINERS(m1.begin(), m1.end(), res.begin(), res.end()
//			    , "adjacent_difference2D");
}



void test_algorithm()
{
    test::interfaz("alp_matrix_alg.h");

    {
	Matrix<char> m{7, 5};

	for (auto& c: m)
	    c = '-';

	Matrix<char> m2{2, 3};
	std::fill(m2.begin(), m2.end(), 'x');
//	for (auto& c: m2)
//	    c = 'x';


	print2D(cout, m);
	copia_dentro(m2, m, {2, 1});
	cout << "===============\n";
	print2D(cout, m);
    }
}


int main()
{
try{
    test::header("alp_matrix_alg.h");

    test_IO();
    test_diferencias();
    test_diferencias2();
    test_algorithm();

}catch(Excepcion& e){
    std::cerr << e.what() << std::endl;
    return 1;
}
    return 0;
}
