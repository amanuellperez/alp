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

#include "../../../alp_matrix.h"

#include "../../../alp_test.h"

#include <iostream>
#include <numeric>

using namespace test;

// Crea una matriz de dimensiones rows x cols, donde los números secuenciales
// a partir de n0
alp::Matrix<int> matrix(size_t rows, size_t cols, int n0)
{
    alp::Matrix<int> res{rows, cols};
    std::iota(res.begin(), res.end(), n0);
    return res;
}

void print(const alp::Matrix<int>& m)
{
    for (auto f = m.row_begin(); f != m.row_end(); ++f){
	for (auto x: *f)
	    std::cout << x << ' ';
	std::cout << '\n';
    }
}

void test_matrix()
{
    // TODO: falta probar las move operations: constructor y assigment.
    // TODO: revisar también que no haya leaks de memoria.

    test::interfaz("Matrix");

    constexpr int num_rows = 3, num_cols = 4;
    alp::Matrix<int> m1{num_rows, num_cols};

    CHECK_TRUE(m1.rows() == num_rows, "rows()");
    CHECK_TRUE(m1.cols() == num_cols, "cols()");
    CHECK_TRUE(m1.size() == num_rows*num_cols, "size()");

    for (size_t i = 0; i < m1.rows(); ++i)
	for (size_t j = 0; j < m1.cols(); ++j)
	    m1(i,j) = i*m1.cols() + j;

    std::cout << "Testing 'const Matrix' as parameter of a function\n";
    print(m1);

    for (size_t i = 0; i < m1.rows(); ++i){
	for (size_t j = 0; j < m1.cols(); ++j){
	    CHECK_TRUE(m1(i,j) == static_cast<int>(i*m1.cols() + j) , "operator(i,j)");
	    std::cout << m1(i,j) << ' ';
	}

	std::cout << '\n';
    }

    std::cout << "-------------------------\n";
    std::cout << "Matriz m1:\n";
    for (size_t i = 0; i < m1.rows(); ++i){
	for (size_t j = 0; j < m1.cols(); ++j)
	    std::cout << m1(i,j) << ' ';

	std::cout << '\n';
    }
    std::cout << "-------------------------\n";


    const alp::Matrix<int> m2 = m1;
    CHECK_TRUE(m2.rows() == m1.rows(), "constructor de copia");
    CHECK_TRUE(m2.cols() == m1.cols(), "constructor de copia");
    CHECK_TRUE(m2.size() == m1.size(), "constructor de copia");

    for (size_t i = 0; i < m1.rows(); ++i)
	for (size_t j = 0; j < m1.cols(); ++j)
	    CHECK_TRUE(m2(i,j) == m1(i,j), "");

    std::cout << "Probando range for:\n";
    for(auto x: m1)
	std::cout << x << ' ';
    std::cout << "\n\n";



    for(size_t i = 0; i < m1.rows(); ++i){
	auto f = m1.row(i); // versión no const
	// std::cout << "Fila " << i << ": ";
	// acceso aleatorio
	for (size_t j = 0; j < f.size(); ++j)
	    CHECK_TRUE(f[j] == m1(i,j), "row[]");

	int j = 0;
	for (auto x: f){
	    CHECK_TRUE(x == m1(i,j), "for range");
	    ++j;
	}
    }


    for(size_t i = 0; i < m2.rows(); ++i){
	auto f = m2.row(i); // versión const
	int j = 0;
	for(auto p = f.begin(); p != f.end(); ++p, ++j)
	    CHECK_TRUE(m2(i,j) == *p, "row");
    }


    {// row_begin/row_end
	auto f0 = m1.row_begin();
	int j = 0;
	for(auto p = f0->begin(); p != f0->end(); ++p, ++j)
	    CHECK_TRUE(m1(0,j) == *p, "row_begin()");

	auto fe = m1.row_end();

	f0 += m1.rows();
	CHECK_TRUE(f0 == fe, "f0 == fe");
    }

    {
	auto f0 = m1.row_begin();
	int j = 0;
	for(auto p = f0->begin(); p != f0->end(); ++p, ++j)
	    CHECK_TRUE(m1(0,j) == *p, "row_begin()");

	++f0;
	j = 0;
	for(auto p = f0->begin(); p != f0->end(); ++p, ++j)
	    CHECK_TRUE(m1(1,j) == *p, "row_begin()");
	
	++f0;
	j = 0;
	for(auto p = f0->begin(); p != f0->end(); ++p, ++j)
	    CHECK_TRUE(m1(2,j) == *p, "row_begin()");

	++f0;
	CHECK_TRUE(f0 == m1.row_end(), "f0 == row_end()");
    }

    size_t i = 0;
    for(auto f = m1.row_begin(); f != m1.row_end(); ++f, ++i){
	int j = 0;
	for(auto p = f->begin(); p != f->end(); ++p, ++j){
	    CHECK_TRUE(*p == m1(i,j), "");
	}
	std::cout << '\n';
    }

    std::cout << "--------------------------------\n";
    for(auto f = m2.row_begin(); f != m2.row_end(); ++f){
	for(auto p = f->begin(); p != f->end(); ++p){
	    std::cout << *p << ' ';
	}
	std::cout << '\n';
    }

    std::cout << "--------------------------------\n";
    for(auto f = m2.row_begin(); f != m2.row_end(); ++f){
	for(auto x: *f)
	    std::cout << x << ' ';
	std::cout << '\n';
    }


    {// metodo fill
	std::cout << "\n\n";
	alp::Matrix<int> m2{3,3};
	std::fill(m2.begin(), m2.end(), 1);
	
	test::check_for_each_condicion(m2.begin(), m2.end(), 
		[](auto x){
		    return x == 1;
		}, "Probando fill()");

    }

    {
	alp::Matrix<int> m3{m1.rows(), m1.cols()};
	m3 = m1;

        CHECK_EQUAL_CONTAINERS(
            m1.begin(), m1.end(), m3.begin(), m3.end(), "operator=");
    }
    {
	alp::Matrix<test::Objeto> m{2, 3};
	for (int i = 0; i < (int) m.rows(); ++i)
	    for (int j = 0; j < (int) m.cols(); ++j)
		m(i,j) = test::Objeto{i, j};

	alp::Matrix<test::Objeto> m3{m.rows(), m.cols()};
	m3 = m;

        CHECK_EQUAL_CONTAINERS(
            m.begin(), m.end(), m3.begin(), m3.end(), "operator=");
    }

}

void test_constructor_por_filas(const alp::Matrix<int>::const_row_iterator& f0,
			        const alp::Matrix<int>::const_row_iterator& fe,
				const alp::Matrix<int>& res)
{
    alp::Matrix<int> y{f0, fe};

    CHECK_EQUAL_CONTAINERS(
	y.begin(), y.end(), res.begin(), res.end(), "constructor por filas");
}

void test_constructor_por_filas()
{
    auto m = matrix(5, 4, 0);

    {
    auto f0 = m.row_begin();
    ++f0;
    auto fe = f0;
    ++fe; ++fe;

    auto res = matrix(2, 4, 4);
    test_constructor_por_filas(f0, fe, res);
    }

    {
	auto f0 = m.row_begin();
	auto fe = f0;
	alp::Matrix<int> res{0, 4};
	test_constructor_por_filas(f0, fe, res);
    }
    
}


void test_tipos()
{
    using M = alp::Matrix<int>;
    CHECK_TRUE((std::is_same_v<M::iterator, int*>), "Matrix::iterator");
    CHECK_TRUE((std::is_same_v<M::const_iterator, const int*>), "Matrix::const_iterator");

    CHECK_TRUE((std::is_same_v<M::row_iterator, alp::Row_iterator<int*>>),
                  "row_iterator");

    CHECK_TRUE((std::is_same_v<M::row_iterator, alp::Row_iterator<int*>>),
                  "row_iterator");

    CHECK_TRUE((std::is_same_v<M::row_iterator::It, alp::Row_iterator_base<int*>>),
                  "row_iterator::It");

    CHECK_TRUE((std::is_same_v<alp::iterator_traits<const int*>::const_iterator, const int*>),
                  "iterator_traits<const int*>");

    using R = alp::const_Row_iterator_min<const int*>;
    CHECK_TRUE(
        (std::is_same_v<alp::iterator_traits<const int*>::iterator, int*>),
        "iterator_traits::iterator");
    CHECK_TRUE((std::is_same_v<R::It, int*>), "R::It");

    CHECK_TRUE((!std::is_same_v<alp::iterator_traits<R>::iterator,
                                alp::iterator_traits<R>::const_iterator>),
               "iterator != const_iterator ");

    //    const_Row_iterator_min<const int*>::const_It p;
    //    const_Row_iterator<const int*>::const_It q;
    //    CHECK_TRUE((std::is_same_v<const_Row_iterator<const int*>::const_It,
    //                               const_Row_iterator_min<const int*>>),
    //               "const_row_iterator::It");

    //    CHECK_TRUE((std::is_same_v<M::const_row_iterator::It,
    //                               const_Row_iterator_base<const int*>>),
    //               "const_row_iterator::It");
    

}


void test_corners()
{
    test::interfaz("corners");

    using M = alp::Matrix<int>;
    using Pos = alp::Matrix<int>::Position;
    M m{3,5};

    CHECK_TRUE((upper_left_corner(m) == Pos{0,0}), "upper_left_corner");
    CHECK_TRUE((upper_right_corner(m) == Pos{0,4}), "upper_right_corner");
    CHECK_TRUE((bottom_left_corner(m) == Pos{2,0}), "bottom_left_corner");
    CHECK_TRUE((bottom_right_corner(m) == Pos{2,4}), "bottom_right_corner");
}

int main()
{
try{
    test::header("alp_matrix_base.h");
    test_tipos();

    test_matrix();
    test_constructor_por_filas();
    test_corners();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}
}
