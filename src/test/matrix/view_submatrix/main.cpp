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
#include "../../../alp_submatrix.h"
#include "../../../alp_matrix.h"

#include "../../../alp_test.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace alp;
using namespace std;
using namespace test;




struct Punto{ int x, y;};

inline bool operator==(const Punto& p, const Punto& q)
{return (p.x == q.x) and (p.y == q.y);}

std::ostream& operator<< (std::ostream& out, const Punto& p)
{
    return out << '(' << p.x << ", " << p.y << ')';
}

struct Punto_x{
    int& operator()(Punto& p) const {return p.x;}
    const int& operator()(const Punto& p) const {return p.x;}
};

struct const_Punto_x{
    const int& operator()(const Punto& p) const {return p.x;}
};


void test_submatrix_and_matrix_view()
{
    test::interfaz("Matrix_view and Submatrix");

    using Matriz = Matrix<Punto, int>;
    using Position = Matriz::Position;
    using Size2D = Matriz::Size2D;

    Matrix<Punto, int> m{3, 4};
    
    int i = 1;
    for (auto p = m.begin(); p != m.end(); ++p, ++i)
	*p = Punto{i, 10*i};

    for (auto f = m.row_begin(); f != m.row_end(); ++f){
	for (auto p: *f)
	    cout << p << ' ';

	cout << '\n';
    }

    {// no const
	int rows = 2;
	int cols = 2;
	int i0 = 0, j0 = 1;
        Submatrix<Matrix<Punto, int>> sb{m, Position{i0,j0}, Size2D{rows,cols}};

	using SB = Submatrix<Matrix<Punto, int>>;
	static_assert(std::is_same_v<Matrix_view<SB::iterator>::iterator, SB::iterator>,
		"iteradores distintos!!!!!!!!!!!!!!!!!!!");

        auto x = matrix_view(sb.begin(), sb.rows(), sb.cols(), Punto_x{});

	cout << "===============================\n";
	for (auto c: x)
	    cout << c << ' ';
	cout << '\n';


	CHECK_TRUE(x.size() == rows*cols, "size()");
	CHECK_TRUE(x.rows() == rows, "rows()");
	CHECK_TRUE(x.cols() == cols, "cols()");

	// Iteradores unidimensionale
	{
	    auto p = x.begin();
	    for (int i = i0; i < i0 + rows; ++i)
		for (int j = j0; j < j0 + cols; ++j, ++p){
		    CHECK_TRUE(*p == m(i,j).x, "begin/end");
	    }
	}
    }
}

int main()
{
try{
    test::header("alp_matrix_view.h");

    test_submatrix_and_matrix_view();

}catch(const std::exception& e){
    std::cerr << e.what() << std::endl;
    return 1;
}
    return 0;
}
