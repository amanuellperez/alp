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


#include "../../../alp_matrix_iterator.h"

#include "../../../alp_test.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace alp;
using namespace std;
using namespace test;




void test_row_iterator()
{
    test::interfaz("Row_iterator");
    
    size_t cols = 3, rows = 4;
    std::vector<size_t> v{0, 1,2,3,4,5,6,7,8, 9, 10, 11};
    
    {// comprobando tipos
	using Row_iterator = Row_iterator_base<std::vector<size_t>::iterator>;
        static_assert(std::is_signed_v<Row_iterator::difference_type>);
    }


    {
	auto f = row_iterator(v.begin(), cols);
	auto f_end = row_iterator(v.end(), cols);
	size_t i = 0;
	for (; f != f_end; ++f){
	    for (auto x: *f){
		CHECK_TRUE(x == v[i], "iterando");
		++i;
	    }
	    
	    if (i > v.size()) 
		CHECK_TRUE(false, "ERROR: se ha quedado colgado");
		
	}
	CHECK_TRUE(i == v.size(), "size!!!");
    }
    {
	Row_iterator<std::vector<size_t>::iterator> f0 = row_iterator(v.begin(), cols);
	// auto f0 = row_iterator(v.begin(), cols);
	auto f_end = row_iterator(v.end(), cols);

	Row_iterator<std::vector<size_t>::iterator> f1 = std::next(f0);

	//auto f1 = std::next(f0);
	CHECK_TRUE(f1 - f0 == 1, "difference");
	f1 += 2;
	CHECK_TRUE(f1 - f0 == 3, "difference");

	++f1;	// f1 == f_end
        CHECK_TRUE(f1 == f_end, "f1 == fend");


	CHECK_TRUE(f1 - f0 == 4, "difference (end)");

	CHECK_TRUE(f_end - f0 == 4, "difference");
	CHECK_TRUE(f0 - f_end == -4, "difference");

	f0 += rows;
	CHECK_TRUE(f0 == f_end, "offset()");
    }


}

struct Punto {int x, y;};

struct Ref_x {
    int& operator()(Punto& p) const {return p.x;}
    const int& operator()(const Punto& p) const{return p.x;}

};

void test_row_iterator_view()
{
    test::interfaz("Row_iterator_view");
    
    std::vector<Punto> v{Punto{1, 100}, Punto {2, 200}, Punto{3, 300}, 
			 Punto{4, 400}, Punto{5, 500}, Punto {6, 600}};

 
    {
	auto f0 = row_iterator(v.begin(), 3);
	auto f_end = row_iterator(v.end(), 3);
	auto r0 = row_iterator_view<std::vector<Punto>::iterator>(f0, Ref_x{});
	auto r_end = row_iterator_view<std::vector<Punto>::iterator>(f_end, Ref_x{});

	for (auto r = r0; r != r_end; ++r, ++f0){
	    CHECK_TRUE(r->size() == f0->size(), "size()");

	    auto p = r->begin();
	    auto q = f0->begin();
	    for (; p != r->end(); ++p, ++q)
		CHECK_TRUE(*p == q->x, "row_iterator_view");
	}

	auto p = r0->begin();
	*p = -30;
	CHECK_TRUE(v[0].x == -30, "operator*() =");
    }

    {
	auto f0 = row_iterator(v.begin(), 3);
	auto f_end = row_iterator(v.end(), 3);
	auto r0 = const_row_iterator_view<std::vector<Punto>::const_iterator>(f0, Ref_x{});
	auto r_end = const_row_iterator_view<std::vector<Punto>::const_iterator>(f_end, Ref_x{});

	for (auto r = r0; r != r_end; ++r, ++f0){
	    CHECK_TRUE(r->size() == f0->size(), "size()");

	    auto p = r->begin();
	    auto q = f0->begin();
	    for (; p != r->end(); ++p, ++q){
		CHECK_TRUE(*p == q->x, "row_iterator_view");
	    }
	}

//	auto p = r0->begin();
//	*p = 30;    // este no tiene que compilar
//	CHECK_TRUE(v[0].x == 30, "operator*() =");
    }

    {
	auto cf0 = const_row_iterator(v.cbegin(), 3);
	auto cf_end = const_row_iterator(v.cend(), 3);

	auto r0 = const_row_iterator_view<std::vector<Punto>::const_iterator>(cf0, Ref_x{});
	auto r_end = const_row_iterator_view<std::vector<Punto>::const_iterator>(cf_end, Ref_x{});

	for (auto r = r0; r != r_end; ++r, ++cf0){
	    CHECK_TRUE(r->size() == cf0->size(), "size()");

	    auto p = r->begin();
	    auto q = cf0->begin();
	    for (; p != r->end(); ++p, ++q)
		CHECK_TRUE(*p == q->x, "row_iterator_view");
	}

//	auto p = r0->begin();
//	*p = 30;    // este no tiene que compilar
//	CHECK_TRUE(v[0].x == 30, "operator*() =");
    }
}


void test_gap_iterator()
{
    test::interfaz("Gap_iterator");

    size_t cols = 4;//, rows = 4;
    std::vector<size_t> v{0, 1,2,3,4,5,6,7,8, 9, 10, 11, 12, 13, 14, 15};



    int j0 = 1;
    int je = 3;

    {
	auto p0 = submatrix_iterator(v.begin() + 4, j0, je, cols);

	auto p = p0;
	for (int i = 0; i < 3; ++i)
	    for (int j = 0; j < 2; ++j){
		CHECK_TRUE(*p == v[4 + i*cols + j0 + j], "operator*");
		++p;
	    }
    }


    {
	auto p0 = submatrix_iterator(v.begin() + 4, j0, je, cols);
	auto p_end = submatrix_iterator(v.end(), j0, je, cols);
	auto p = p0;

	CHECK_TRUE((p - p0) == 0, "p - p0");
	CHECK_TRUE((p_end - p) == 6, "p_end - p");

	p+=4;
	CHECK_TRUE((p - p0) == 4, "p - p0");
	CHECK_TRUE((p0 - p) == -4, "p - p0");

	--p;
	CHECK_TRUE((p - p0) == 3, "p - p0");
    }

    {// sirve para iterar por una columna!!!
	int j0 = 2;
        auto p     = submatrix_iterator(v.begin(), j0, j0 + 1, cols);
        auto p_end = submatrix_iterator(v.end(), j0, j0 + 1, cols);

        int i = 0;
	for(; p!= p_end; ++p, ++i){
	    CHECK_TRUE(*p == v[j0 + i*cols], "iterando por una columna");
	    if (i > 10)
		throw test::Error{"Se quedo colgado"};
	}
    }
}

void test_transpose_iterator()
{
    test::interfaz("Transpose_iterator");

    int rows = 5, cols = 3;
    std::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};

    auto p0 = transpose_iterator_begin(v.begin(), rows, cols);
    auto p_end = transpose_iterator_end(v.begin(), rows, cols);

    std::vector<int> res = {0, 3, 6, 9, 12, 1, 4, 7, 10, 13, 2, 5, 8, 11, 14};
    CHECK_EQUAL_CONTAINERS(res.begin(), res.end(), p0, p_end, "Transpose_iterator");

    {
	auto p = p0;

	CHECK_TRUE((p - p0) == 0, "p - p0");
	CHECK_TRUE((p_end - p) == static_cast<int>(v.size()), "p_end - p");

	p+=4;
	CHECK_TRUE((p - p0) == 4, "p - p0");
	CHECK_TRUE((p0 - p) == -4, "p - p0");

	--p;
	CHECK_TRUE((p - p0) == 3, "p - p0");
    }
}

int main()
{
try{
    test::header("alp_matrix_iterator.h");

    test_row_iterator();
    test_row_iterator_view();
    test_gap_iterator();
    test_transpose_iterator();


}catch(const std::exception& e){
    std::cerr << e.what() << std::endl;
    return 1;
}
    return 0;
}
