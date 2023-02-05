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

void test_submatrix()
{
    test::interfaz("Submatrix");

    using Position = Matrix<int>::Position;
    using Size = Matrix<int>::Size2D;

    alp::Matrix<int> m{6,6};
    {
	int i = 0;
	for (auto& x: m)
	    x = i++;
    }

    for (auto f = m.row_begin(); f != m.row_end(); ++f){
	for (auto p = f->begin(); p != f->end(); ++p)
	    cout << *p << ' ';

	cout << '\n';
    }

    {// version no const
    
    Position p0{2,1};
    Size sz{2,3};

    //auto  s = Submatrix<Matrix<int>>{m, p0, sz};
    auto  s = Submatrix{m, p0, sz};

    CHECK_TRUE((s.P0() == p0), "P0()");
    CHECK_TRUE(s(0,0) == 13, "m(i,j)");
    CHECK_TRUE(s(Position{0,1}) == 14, "m(i,j)");
    CHECK_TRUE(s(0,2) == 15, "m(i,j)");
    CHECK_TRUE(s(1,0) == 19, "m(i,j)");
    CHECK_TRUE(s(1,1) == 20, "m(i,j)");
    CHECK_TRUE(s(1,2) == 21, "m(i,j)");

    {// row_begin
	int i = 0, j = 0;
	for (auto f = s.row_begin(); f != s.row_end(); ++f, ++i, j = 0){
	    for (auto p = f->begin(); p != f->end(); ++p, ++j){
		CHECK_TRUE(*p == s(i,j), "row_begin/end");
	    }
	}
    }


    vector<int> res = {13,14,15,19,20,21};
    CHECK_EQUAL_CONTAINERS(s.begin(), s.end(), res.begin(), res.end(),
		    "submatrix (en medio de la matrix)");


    auto ext = s.extension();
    CHECK_TRUE((ext.i0 == 2 and ext.j0 == 1 and ext.ie == 4 and ext.je == 4),
               "extension()");

    using Matrix = alp::Matrix<int>;
    Submatrix<Matrix> s2;	
    
    //Submatrix<Matrix> s2;
    s2.de(m, Position{2, 1}, Size{2, 3});
    CHECK_TRUE((s2.P0() == Position{2,1} and s2.P1() == Position{3,3}),
               "de()");
    CHECK_EQUAL_CONTAINERS(s.begin(), s.end(), s2.begin(), s2.end(),
		    "submatrix.de()");

    Submatrix<Matrix> s3;
    s3.de(m, Position{2, 1}, Position{3, 3});
    CHECK_TRUE((s3.P0() == Position{2,1} and s3.P1() == Position{3,3}),
               "de()");

    CHECK_TRUE(s3.P0() == s2.P0(), "de()");
    CHECK_TRUE(s3.P1() == s2.P1(), "de()");
    CHECK_TRUE(s3.rows() == s2.rows(), "de()");
    CHECK_TRUE(s3.cols() == s2.cols(), "de()");

    CHECK_EQUAL_CONTAINERS(s3.begin(), s3.end(), s2.begin(), s2.end(),
		    "submatrix.de()");
    {
	Matrix res{s.size2D()};
	auto f0 = s.row_begin();
	auto f1 = res.row_begin();
	for (; f0 != s.row_end(); ++f0, ++f1)
	    std::copy(f0->begin(), f0->end(), f1->begin());

    }

    auto copia = clone(s);
    CHECK_EQUAL_CONTAINERS(copia.begin(), copia.end(), res.begin(), res.end(),
		    "clone(submatrix)");


    }




    {
    auto s = Submatrix{m, Position{2,4}, Size{4, 2}};
    // auto s = Submatrix{m, Position{2,4}, Size{4, 2}};

    vector<int> res = {16,17,22,23,28,29,34,35};
    CHECK_EQUAL_CONTAINERS(s.begin(), s.end(), res.begin(), res.end(),
		    "submatrix (pegando a la esquina de la matrix)");
    
    auto p0 = s.begin();
    auto pe = s.end();

    std::vector<int> v;
    v.resize(pe - p0);
    std::copy(p0, pe, v.begin());
    }


    {// version const
    
    Position p0{2,1};
    Size sz{2,3};

    auto  s = Submatrix<const Matrix<int>>{m, p0, sz};

    CHECK_TRUE((s.P0() == p0), "P0()");
    CHECK_TRUE(s(0,0) == 13, "m(i,j)");
    CHECK_TRUE(s(Position{0,1}) == 14, "m(i,j)");
    CHECK_TRUE(s(0,2) == 15, "m(i,j)");
    CHECK_TRUE(s(1,0) == 19, "m(i,j)");
    CHECK_TRUE(s(1,1) == 20, "m(i,j)");
    CHECK_TRUE(s(1,2) == 21, "m(i,j)");

    vector<int> res = {13,14,15,19,20,21};
    CHECK_EQUAL_CONTAINERS(s.begin(), s.end(), res.begin(), res.end(),
		    "const_submatrix (en medio de la matrix)");


    auto ext = s.extension();
    CHECK_TRUE((ext.i0 == 2 and ext.j0 == 1 and ext.ie == 4 and ext.je == 4),
               "const_extension()");

    auto copia = clone(s);
    CHECK_EQUAL_CONTAINERS(copia.begin(), copia.end(), res.begin(), res.end(),
		    "clone(const_submatrix)");
    }


}


int main()
{
try{
    test::header("alp_submatrix.h");

    test_submatrix();

}catch(const std::exception& e){
    std::cerr << e.what() << std::endl;
    return 1;
}
    return 0;
}
