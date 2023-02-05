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

#include "../../../alp_matrix_view.h"
#include "../../../alp_matrix.h"
#include "../../../alp_iterator.h"
#include "../../../alp_test.h"

#include <iostream>
#include <numeric>

using namespace std;
using namespace alp;
using namespace test;

template <typename C1, typename C2>
// m = matriz; res = resultado
void prueba_matrix(C1& m, const C2& res)
{
    CHECK_TRUE((int)m.size() == (int)res.size(), "size()");

    auto p = res.begin();
    for (auto x: m){
	CHECK_TRUE(*p == x, "begin/end");
	++p;
    }

    p = res.begin();
    for (auto f = m.row_begin(); f != m.row_end(); ++f){
	for (auto x: *f){
	    CHECK_TRUE(*p == x, "row_begin/end");
		++p;
	    }
    }
}


template <typename M>
// M = contenedor unidimensional
void test_matrix_view_impl(M& v, int rows, int cols, const std::string& str)
{
    test::interfaz(str);

    for (auto x: v)
	cout << x << ' ';
    cout << '\n';

    {
	Matrix_view m{v.begin(), rows, cols};
	prueba_matrix(m, v);
    }
    {
	int rows = 4, cols = 5;
//	auto p0 = transpose_iterator_begin(v.begin(), rows, cols);
//	Matrix_view m{p0, cols, rows};

	auto m = transpose_matrix(v.begin(), rows, cols);

        std::vector res{0, 4, 8,  12, 16, 1, 5, 9,  13, 17,
                        2, 6, 10, 14, 18, 3, 7, 11, 15, 19};
        prueba_matrix(m, res);
    }


    {
	int rows = 4, cols = 4;
	int j0 = 1;
	auto p0 = submatrix_iterator(v.begin() + 2*rows, j0, 3, cols);
	Matrix_view m{p0, 2, 2};

//	int submatrix_rows = 2, submatrix_cols = 2;
//        auto m = submatrix(v.begin() + 2 * rows,
//			    j0, 
//			    submatrix_rows, submatrix_cols, 
//			    cols);

        std::vector res{9, 10, 13, 14};
        prueba_matrix(m, res);

    }


}

// CUIDADO: recordar siempre definir una versión const de la función.
struct Mi_view{
    int& operator()(int& x) const{return x;}
    int operator()(const int& x) const{return x;}
};


struct const_Mi_view{
//    int& operator()(int& x) const{return x;}
    int operator()(const int& x) const{return x;}
};

void test_matrix_view()
{
    int rows = 5, cols = 4;

    std::vector<int> v;
    v.resize(rows*cols);
    std::iota(v.begin(), v.end(), 0);

    test_matrix_view_impl(v, rows, cols, "vector");

    {
	Matrix<int, int> m{rows, cols};
	
	std::copy(v.begin(), v.end(), m.begin());

	test_matrix_view_impl(m, rows, cols, "Matrix");

	// CUIDADO: no funciona con las lambdas ya que las lambdas no se
	// pueden copiar!!!
//	auto fview = [](int x) -> int{ return x*10; };
//	auto p0   = view_iterator(v.begin(), fview);
//
//        Matrix_view view{p0, m.rows(), m.cols()};

	{
	auto view = matrix_view(v.begin(), m.rows(), m.cols(), Mi_view{});
	test_matrix_view_impl(view, m.rows(), m.cols(), "matrix_view");

	auto tmp = view(0,0);
	view(0,0) = 10;
        CHECK_TRUE((v[0] == 10), "matrix_view(i,j) =");
	view(0,0) = tmp;
	}

	{
	auto const_view = matrix_view(v.begin(), m.rows(), m.cols(), const_Mi_view{});
        test_matrix_view_impl(
            const_view, m.rows(), m.cols(), "const_matrix_view");

        // const_view(0,0) = 10; // este no tiene que compilar
	}

	{// const_iterator
            auto const_view = const_matrix_view(
                v.cbegin(), m.rows(), m.cols(), const_Mi_view{});

	    auto f = const_view.row_begin();
	    auto fe = const_view.row_end();

	    for (; f != fe; ++f)
		for (auto p = f->begin();p != f->end(); ++p)
		    std::cout << *p << ' ';

//	    test_matrix_view_impl(
//                const_view, m.rows(), m.cols(), "const_matrix_view");

            // const_view(0,0) = 10; // este no tiene que compilar
	}
    }

}

void test_matrix_xy()
{
    using Matrix = alp::Matrix<int, int>;
    Matrix m{4,5};

    for (int i = 0; i < m.rows(); ++i)
	for (int j = 0; j < m.cols(); ++j)
	    m(i,j) = i*m.cols() + j;

    std::cout << "\nImprimimos m\n";
    std::cout << "-----------------\n";
    test::print_range2D(m);

    {
	alp::Matrix_xy<int,int> mxy{m};
	std::cout << "\nImprimimos Matrix_xy\n";
	std::cout << "-----------------\n";
	for (int y = 0; y <= mxy.y_max(); ++y){
	    for (int x = 0; x <= mxy.x_max(); ++x)
		std::cout << mxy(x,y) << ' ';

	    std::cout << '\n';
	}
    }
    {// caso const
	const Matrix& m1 = m;

	alp::const_Matrix_xy<int,int> mxy{m1};
	std::cout << "Imprimimos const_Matrix_xy\n";
	std::cout << "-----------------------\n";
	for (int y = 0; y <= mxy.y_max(); ++y){
	    for (int x = 0; x <= mxy.x_max(); ++x)
		std::cout << mxy(x,y) << ' ';

	    std::cout << '\n';
	}
    }

    {// caso genérico (+,+)
//	using Point = alp::Matrix_xy<int,int>::Point;

	alp::Matrix_xy<int,int> mxy{m, 1, 3};
	CHECK_TRUE(mxy.x(0) == -3, "x(0)");
	CHECK_TRUE(mxy.x(4) == 1, "x(4)");
	CHECK_TRUE(mxy.y(0) == 1, "y(0)");
	CHECK_TRUE(mxy.y(3) == -2, "y(3)");

	CHECK_TRUE(mxy.rows()  == 4, "rows");
	CHECK_TRUE(mxy.cols()  == 5, "cols");
	CHECK_TRUE(mxy.x_min() == -3, "x_min");
	CHECK_TRUE(mxy.x_max() == 1, "x_max");
	CHECK_TRUE(mxy.y_min() == -2, "y_min");
	CHECK_TRUE(mxy.y_max() == 1, "y_max");

	CHECK_TRUE(mxy(0,0) == 8, "(0,0)");
	CHECK_TRUE(mxy(-3,-2) == 15, "(-3,-2)");
	CHECK_TRUE(mxy(-3,1) == 0, "(-3,1)");
	CHECK_TRUE(mxy(1,1) == 4, "(1,1)");
	CHECK_TRUE(mxy(1,-2) == 19, "(1,-2)");

//	CHECK_TRUE((upper_left_corner(mxy) == Point{-3,1}), "upper_left_corner");
//	CHECK_TRUE((upper_right_corner(mxy) == Point{1,1}), "upper_right_corner");
//	CHECK_TRUE((bottom_left_corner(mxy) == Point{-3,-2}), "bottom_left_corner");
//	CHECK_TRUE((bottom_right_corner(mxy) == Point{1,-2}), "bottom_right_corner");

	std::cout << "\nCambiamos el origen\n";
	mxy.origen_de_coordenadas(1,2);
	CHECK_TRUE(mxy.x_min() == -2, "x_min");
	CHECK_TRUE(mxy.x_max() == 2, "x_max");
	CHECK_TRUE(mxy.y_min() == -2, "y_min");
	CHECK_TRUE(mxy.y_max() == 1, "y_max");

	CHECK_TRUE(mxy(0,0) == 7, "(0,0)");
	CHECK_TRUE(mxy(2,1) == 4, "(2,1)");
	CHECK_TRUE(mxy(2,-2) == 19, "(2,-2)");
	CHECK_TRUE(mxy(-2,-2) == 15, "(-2,-2)");
	CHECK_TRUE(mxy(-2,1) == 0, "(-2,1)");
    }


    {// caso genérico (-,-)
	alp::Matrix_xy<int,int, -1, -1> mxy{m, 1, 3};
	CHECK_TRUE(mxy.x(0) == 3, "x(0)");
	CHECK_TRUE(mxy.x(4) == -1, "x(4)");
	CHECK_TRUE(mxy.y(0) == -1, "y(0)");
	CHECK_TRUE(mxy.y(3) == 2, "y(3)");

	CHECK_TRUE(mxy.rows()  == 4, "rows");
	CHECK_TRUE(mxy.cols()  == 5, "cols");
	CHECK_TRUE(mxy.x_min() == -1, "x_min");
	CHECK_TRUE(mxy.x_max() == 3, "x_max");
	CHECK_TRUE(mxy.y_min() == -1, "y_min");
	CHECK_TRUE(mxy.y_max() == 2, "y_max");

	CHECK_TRUE(mxy(0,0) == 8, "(0,0)");
	CHECK_TRUE(mxy(3,2) == 15, "(3,2)");
	CHECK_TRUE(mxy(3,-1) == 0, "(3,-1)");
	CHECK_TRUE(mxy(-1,-1) == 4, "(-1,-1)");
	CHECK_TRUE(mxy(-1,2) == 19, "(-1,2)");

	std::cout << "\nCambiamos el origen\n";
	mxy.origen_de_coordenadas(1,2);
	CHECK_TRUE(mxy.x_min() == -2, "x_min");
	CHECK_TRUE(mxy.x_max() == 2, "x_max");
	CHECK_TRUE(mxy.y_min() == -1, "y_min");
	CHECK_TRUE(mxy.y_max() == 2, "y_max");

	CHECK_TRUE(mxy(0,0) == 7, "(0,0)");
	CHECK_TRUE(mxy(-2,-1) == 4, "(-2,-1)");
	CHECK_TRUE(mxy(-2,2) == 19, "(-2,2)");
	CHECK_TRUE(mxy(2,2) == 15, "(2,2)");
	CHECK_TRUE(mxy(2,-1) == 0, "(2,-1)");
    }

}

int main()
{
try{
    test::header("alp_matrix_view.h");

    test_matrix_view();
    test_matrix_xy();

}catch(std::exception& e)
{
    std::cerr << e.what() << endl;
    return 1;
}
}
