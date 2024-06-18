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


#include "../../../alp_matrix_algorithm.h"
#include "../../../alp_test.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>


using namespace test;

void test_read_matrix()
{
    test::interfaz("read_matrix");

    {// flujo
    std::stringstream file;
    file << "1 2 3\n"
	    "4 5 6\n"
	    "\n"
	    "Esto ya no lo lee";

    std::vector<int> res = {1,2,3,4,5,6};
    auto m = alp::read_matrix<int, size_t>(file);

    CHECK_TRUE(m.rows() == 2 and m.cols() == 3, "read_matrix(flujo)");
    CHECK_EQUAL_CONTAINERS_C(m, res, "read_matrix");
    }

    {// fichero
    std::string fname = "/tmp/read_matrix";
    std::ofstream out{fname};

    out << "1 2 3\n"
	   "4 5 6\n";
    out.close();

    std::vector<int> res = {1,2,3,4,5,6};
    auto m = alp::read_matrix<int, size_t>(fname);

    CHECK_TRUE(m.rows() == 2 and m.cols() == 3, "read_matrix(file)");
    CHECK_EQUAL_CONTAINERS_C(m, res, "read_matrix");
    }

}


void test_print_matrix()
{
    test::interfaz("print_matrix");
    
    {
	std::vector<int> v = {1, 2,
			      3, 4,
			      5, 6};

	auto m = alp::vector2matrix<int, size_t>(v, 3);
	alp::print(std::cout, m);
	std::cout << '\n';
    }
}




void test_diferencias()
{
    test::interfaz("adjacent_difference2D");

    alp::Matrix<int> m{4, 4};

    std::vector m0 = {4, 8, 10, 10,
		 3, 9, 7, 6,
		 2, 7, 6, 6,
		 1, 5, 4, 3};

    
    std::copy(m0.begin(), m0.end(), m.begin());

    alp::Matrix<int> m1{m.size2D()};

    adjacent_difference2D(m.row_begin(), m.row_end(), m1.row_begin());

    std::vector res = {4, 4, 2, 0,
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

    alp::Matrix<Punto> img1{2, 2};

    std::vector<Punto> m0 = {{1, 10}, {5, 20},
		        {2, 30}, {-4, 40}};
    
    std::copy(m0.begin(), m0.end(), img1.begin());

//    alp::Matrix<int> incr{img1.size2D()};

//    auto p_x = [](Punto& p) {return p.x;};
//
//    auto rb = img1.row_begin();
//

//TODO: AQUIII: probar por columnas
//    auto f0 = row_iterator_view(rb, p_x);
//    auto f1 = img1.row_end();
//    auto g0 = incr.row_begin();
//

//    adjacent_difference2D(iterator_view(m.row_begin(), m.row_end(), m1.row_begin());
//
//    std::vector res = {4, 4, 2, 0,
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
	alp::Matrix<char> m{7, 5};

	for (auto& c: m)
	    c = '-';

	alp::Matrix<char> m2{2, 3};
	std::fill(m2.begin(), m2.end(), 'x');
//	for (auto& c: m2)
//	    c = 'x';


	alp::print(std::cout, m);
	alp::copia_dentro(m2, m, {2, 1});
	std::cout << "===============\n";
	alp::print(std::cout, m);
    }
}

void test_transform1D()
{
    test::interfaz("transform_1D_alrededor");

    {
	std::vector<int> vx = {1,0,0,1,1,1,0,
	                       0,1,1,2,1,0,1,
	                       2,1,1,0,1,0,0,
	                       2,1,2,0,1,2,1};

	alp::Matrix<int> x{4,7};
	std::copy(vx.begin(), vx.end(), x.begin());

	auto y = alp::transform1D_alrededor(x,
		[](int x0, int x1, int x2){
		    if (x1 != 1)
			return 0;

		    if (x0 == 0 or x2 == 0)
			return 1;
                    else
                        return 0;
		},
		[](int x0, int x1){
		    if (x0 != 1)
			return 0;

		    if (x1 == 0)
			return 1;
                    else
                        return 0;
		},
		[](int x0, int x1){
		    if (x1 != 1)
			return 0;

		    if (x0 == 0)
			return 1;
                    else
                        return 0;
		});


        CHECK_TRUE(y.rows() == x.rows() and y.cols() == x.cols(),
                   "transform1D_alrededor:rows x cols");

	std::vector<int> res = {1,0,0,1,0,1,0,
				0,1,0,0,1,0,1,
				0,0,1,0,1,0,0,
				0,0,0,0,1,0,0};
	CHECK_EQUAL_CONTAINERS_C(y, res, "transform1D_alrededor");

//        print(std::cout, y);

    }

}
void test_transform2D()
{
    test::interfaz("transform_2D_alrededor");

    {
	std::vector<int> vx = {1,2,3,4,5,
	                       2,1,2,1,2,
			       1,2,2,2,1,
			       5,4,3,2,1};
	alp::Matrix<int> x{5,4};
	std::copy(vx.begin(), vx.end(), x.begin());

	auto y = alp::transform2D_alrededor(x, 47, 
		[](int o0, int o1, int o2,
		   int p0, int p1, int p2,
		   int q0, int q1, int q2){
	std::cout << "Copiando " << o0 << " -> " << p1 << " -> " << q2 << '\n';
		    if (p1 == 1)
			return 10;
		    else
			return 33;
		});

        CHECK_TRUE(y.rows() == x.rows() and y.cols() == x.cols(),
                   "transform2D_alrededor:rows x cols");
	std::cerr << "TODO: NO ES AUTOMATICA\n";
        print(std::cout, y);

    }

}

void test_y_symmetry()
{
    test::interfaz("y_symmetry");

    {
	std::vector<int> vx = {1,2,3,4,5,
	                       6,7,8,9,0};
	std::vector<int> res= {5,4,3,2,1,
	                       0,9,8,7,6};

	alp::Matrix<int> x{2,5};
	std::copy(vx.begin(), vx.end(), x.begin());
	x = alp::y_symmetry(x);

	CHECK_EQUAL_CONTAINERS_C(x, res, "y_symmetry");
    }
}

void test_rotate()
{
    test::interfaz("rotate");

    {
	std::vector<int> vx = {1,2,3,4,5,
	                       6,7,8,9,0};
	std::vector<int> res= {5, 0,
			       4, 9,
			       3, 8,
			       2, 7, 
			       1, 6};


	auto x0 = alp::vector2matrix(vx, 2);
	auto x = alp::rotate_plus90(x0);

	CHECK_EQUAL_CONTAINERS_C(x, res, "rotate_plus90");
	x = alp::rotate_minus90(x);
	CHECK_EQUAL_CONTAINERS_C(x, x0, "rotate_minus90");

	// rotate_180 = 2 rotate_90
	auto x180 = alp::rotate_180(x);
	auto x90 = alp::rotate_plus90(x);
	x90 = alp::rotate_plus90(x90);

	CHECK_TRUE(x180.rows() == x.rows() 
		and x180.cols() == x.cols(), "size");
	CHECK_EQUAL_CONTAINERS_C(x180, x90, "rotate_180");

    }
}


void test_differences()
{
    test::interfaz("h_differences");

{// basico
    std::vector<int> vx = {100, 90, 70,
			   20, 30, 30};
    std::vector<int> res= {-10, -20,
			   +10, 0};


    alp::Matrix<int> x{2,3};
    std::copy(vx.begin(), vx.end(), x.begin());
    auto y = alp::h_differences(x);

//    print(std::cout, y);
    CHECK_TRUE(y.rows() == x.rows() 
	    and y.cols() == (x.cols() - 1), "size");
    CHECK_EQUAL_CONTAINERS_C(y, res, "h_differences");

}

{// una linea
    std::vector<int> vx = {1, 2, 4, 7, 11};
    std::vector<int> res= {1, 2, 3, 4};


    alp::Matrix<int> x{1,5};
    std::copy(vx.begin(), vx.end(), x.begin());
    auto y = alp::h_differences(x);

//    print(std::cout, y);
    CHECK_TRUE(y.rows() == x.rows() 
	    and y.cols() == (x.cols() - 1), "size");
    CHECK_EQUAL_CONTAINERS_C(y, res, "h_differences");
}
}


void test_operations()
{
    test::interfaz("operations");

{// basic +
    std::vector<int> va = {1, 2, 3, 
			   4, 5, 6};
    std::vector<int> vb = {10, 20, 30,
			   40, 50, 60};

    std::vector<int> res = {11, 22, 33,
			   44, 55, 66};

    auto a = alp::vector2matrix(va, 2);
    auto b = alp::vector2matrix(vb, 2);
    alp::Matrix<int, int> c = a + b;
    CHECK_EQUAL_CONTAINERS_C(c, res, "operator+");
}

{// basic -
    std::vector<int> va = {1, 2, 3, 
			   4, 5, 6};
    std::vector<int> vb = {10, 20, 30,
			   40, 50, 60};

    std::vector<int> res = {9, 18, 27,
			    36, 45, 54};

    auto a = alp::vector2matrix(va, 2);
    auto b = alp::vector2matrix(vb, 2);
    alp::Matrix<int, int> c = b - a;
    CHECK_EQUAL_CONTAINERS_C(c, res, "operator-");
}
}


int main()
{
try{
    test::header("alp_matrix_alg.h");

    test_read_matrix();
    test_print_matrix();
    test_diferencias();
    test_diferencias2();
    test_algorithm();
    test_transform1D();
    test_transform2D();
    test_y_symmetry();
    test_rotate();
    test_differences();
    test_operations();

}catch(std::exception& e){
    std::cerr << e.what() << std::endl;
    return 1;
}
    return 0;
}
