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


int main()
{
try{
    test::header("alp_matrix_view.h");

    test_matrix_view();

}catch(std::exception& e)
{
    std::cerr << e.what() << endl;
    return 1;
}
}
