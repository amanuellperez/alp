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

#include "../../alp_subcontainer.h"
#include "../../alp_iterator.h"
#include "../../alp_test.h"

#include <iostream>
#include <numeric>

using namespace std;
using namespace alp;
using namespace test;


struct Punto{int x, y;};

struct Por_dos{
    int operator()(const int& x) const {return 2*x;}
};

struct Ref_x{
    int& operator()(Punto& p) const {return p.x;}
    const int& operator()(const Punto& p) const {return p.x;}
};


struct Value_x{
    int operator()(const Punto& p) const {return p.x;}
};

struct const_Ref_x{
    const int& operator()(const Punto& p) const {return p.x;}
};

void test_container_view()
{
    test::interfaz("container_view");

    // Probamos
    std::vector<int> v = {1, 2, 3, 4, 5, 6};

    {// Por_dos (function object)
    auto s = container_view(v.begin(), v.end(), Por_dos{});
    CHECK_TRUE(s.size() == v.size(), "size");

    for (size_t i = 0; i < v.size(); ++i)
	CHECK_TRUE(s[i] == 2*v[i], "operator[]");

    size_t i = 0;
    for (auto p = s.begin(); p != s.end(); ++p, ++i)
	CHECK_TRUE(s[i] == 2*v[i], "begin/end");


    i = 0;
    for (auto p = s.cbegin(); p != s.cend(); ++p, ++i)
	CHECK_TRUE(s[i] == 2*v[i], "cbegin/cend");

    }

    {
	std::vector<Punto> v {Punto{1,10}, Punto{2,20}, Punto{3, 30}};

	auto s = container_view(v.begin(), v.end(), Ref_x{});
	CHECK_TRUE(s.size() == v.size(), "size");

	for (size_t i = 0; i < v.size(); ++i)
	    CHECK_TRUE(s[i] == v[i].x, "operator[]");

	size_t i = 0;
	for (auto p = s.begin(); p != s.end(); ++p, ++i)
	    CHECK_TRUE(s[i] == v[i].x, "begin/end");


	i = 0;
	for (auto p = s.cbegin(); p != s.cend(); ++p, ++i)
	    CHECK_TRUE(s[i] == v[i].x, "cbegin/cend");

    }


}


template <typename Container>
void test_subcontainer1(Container& a)
{
    Subcontainer sb{&a[3], &a[8]};

    CHECK_TRUE(sb.size() == 5, "size()");
    CHECK_TRUE(!sb.empty(), "!empty()");

    for (size_t i = 0; i < sb.size(); ++i)
	CHECK_TRUE(sb[i] == a[3 + i], "operator[]");

    size_t i = 0;
    for (auto p = sb.begin(); p != sb.end(); ++p, ++i)
	CHECK_TRUE(*p == a[3 + i], "begin/end");

    i = 0;
    for (auto p = sb.cbegin(); p != sb.cend(); ++p, ++i)
	CHECK_TRUE(*p == a[3 + i], "cbegin/cend");
}



void test_subcontainer()
{
    test::interfaz("Subcontainer");
    
    {// vector
	std::vector<int> a(10);
	std::iota(begin(a), end(a), 0);

	// version no const
	test_subcontainer1<std::vector<int>>(a);

	// version const
	test_subcontainer1<const std::vector<int>>(a);

	// escritura
	Subcontainer sb{&a[3], &a[8]};
	sb[2] = 100;
	CHECK_TRUE(sb[2] == 100, "operator[] =");
	CHECK_TRUE(a[5] == 100, "operator[] =");

//	Subcontainer<const std::vector<int>> const_sb{a, 3, 8};
//	const_sb[2] = 100; // esto no tiene que compilar
    }
    {// array
	int a[10] = {1,2,3,4,5,6,7,8,9,10};
	
	test_subcontainer1<int [10]>(a);
	test_subcontainer1<const int [10]>(a);
	    
	Subcontainer sb2{a + 3, 4};

	size_t i = 0;
	for (auto p = sb2.begin(); p != sb2.end(); ++p, ++i)
	    CHECK_TRUE(*p == a[3 + i], "Constructor(p0, n)");

	// escritura
	Subcontainer sb{&a[3], &a[8]};
	sb[2] = 100;
	CHECK_TRUE(sb[2] == 100, "operator[] =");
	CHECK_TRUE(a[5] == 100, "operator[] =");

//	Subcontainer<const std::vector<int>> const_sb{a, 3, 8};
//	const_sb[2] = 100; // esto no tiene que compilar
    }


    {// subcontainer_view
	int i0 = 1;
	size_t  size = 3;
	std::vector<Punto> v 
	{Punto{1,10}, Punto{2,20}, Punto{3, 30}, Punto{4, 40}, Punto{5,50}};

	Subcontainer sb{view_iterator(v.begin() + i0, Ref_x{}),
			view_iterator(v.begin() + i0 + size, Ref_x{})};

    
	CHECK_TRUE(sb.size() == size, "size()");

	for (size_t i = 0; i < sb.size(); ++i)
	    CHECK_TRUE(sb[i] == v[i0 + i].x, "operator[]");

	int i = 0;
	for (auto p = sb.begin(); p != sb.end(); ++p, ++i)
	    CHECK_TRUE(*p == v[i0 + i].x, "begin/end");

	i = 0;
	for (auto p = sb.cbegin(); p != sb.cend(); ++p, ++i)
	    CHECK_TRUE(*p == v[i0 + i].x, "begin/end");

	sb[0] = 23;
	CHECK_TRUE(v[i0].x == 23, "operator[] =");

    }

    {// const_subcontainer_view
	int i0 = 1;
	size_t  size = 3;
	std::vector<Punto> v 
	{Punto{1,10}, Punto{2,20}, Punto{3, 30}, Punto{4, 40}, Punto{5,50}};

	Subcontainer sb{const_view_iterator(v.begin() + i0, Ref_x{}),
			const_view_iterator(v.begin() + i0 + size, Ref_x{})};

    
	CHECK_TRUE(sb.size() == size, "size()");

	for (size_t i = 0; i < sb.size(); ++i)
	    CHECK_TRUE(sb[i] == v[i0 + i].x, "operator[]");

	int i = 0;
	for (auto p = sb.begin(); p != sb.end(); ++p, ++i)
	    CHECK_TRUE(*p == v[i0 + i].x, "begin/end");

	i = 0;
	for (auto p = sb.cbegin(); p != sb.cend(); ++p, ++i)
	    CHECK_TRUE(*p == v[i0 + i].x, "begin/end");

//	sb[0] = 23; // este no tiene que compilar
//	CHECK_TRUE(v[i0].x == 23, "operator[] =");

    }
}


int main()
{
try{
    test::header("alp_container.h");

    test_container_view();
    test_subcontainer();


}catch(std::exception& e)
{
    std::cerr << e.what() << endl;
    return 1;
}
}
