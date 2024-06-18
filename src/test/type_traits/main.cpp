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

#include "../../alp_type_traits.h"
#include "../../alp_test.h"

#include <atd_cast.h>
#include <iostream>

using namespace std;
using namespace alp;
using namespace test;

struct A{
    using iterator = int;
    using const_iterator = double;
};

void test_type_traits()
{
    test::interfaz("type_traits");

    {
        CHECK_TRUE((std::is_same_v<Iterator_type<A>, A::iterator>),
                   "Iterator_type(no const)");

        CHECK_TRUE((std::is_same_v<Iterator_type<const A>, A::const_iterator>),
                   "Iterator_type(const)");
    }
    {
	static_assert(!alp::is_iterator_v<int>);
	static_assert(alp::is_iterator_v<int*>);
	static_assert(alp::is_iterator_v<std::vector<int>::iterator>);
	static_assert(alp::is_iterator_v<std::vector<int>::const_iterator>);
    }


    {
        CHECK_TRUE((std::is_same_v<alp::remove_cvref_t<int>, int>),
                   "remove_cvref_t()");

        CHECK_TRUE((std::is_same_v<alp::remove_cvref_t<int&>, int>),
                   "remove_cvref_t()");

        CHECK_TRUE((std::is_same_v<alp::remove_cvref_t<const int>, int>),
                   "remove_cvref_t()");

        CHECK_TRUE((std::is_same_v<alp::remove_cvref_t<const int&>, int>),
                   "remove_cvref_t()");

        CHECK_TRUE((std::is_same_v<alp::remove_cvref_t<volatile int>, int>),
                   "remove_cvref_t()");

        CHECK_TRUE((std::is_same_v<alp::remove_cvref_t<volatile int&>, int>),
                   "remove_cvref_t()");
    }

    {
        CHECK_TRUE((std::is_same_v<alp::remove_cvpointer_t<int>, int>),
                   "remove_cvpointer_t()");

        CHECK_TRUE((std::is_same_v<alp::remove_cvpointer_t<int*>, int>),
                   "remove_cvpointer_t()");

        CHECK_TRUE((std::is_same_v<alp::remove_cvpointer_t<const int*>, int>),
                   "remove_cvpointer_t()");

        CHECK_TRUE((std::is_same_v<alp::remove_cvpointer_t<const int*>, int>),
                   "remove_cvpointer_t()");

        CHECK_TRUE((std::is_same_v<alp::remove_cvpointer_t<volatile int>, int>),
                   "remove_cvpointer_t()");

        CHECK_TRUE((std::is_same_v<alp::remove_cvpointer_t<volatile int*>, int>),
                   "remove_cvpointer_t()");
    }



    {
        CHECK_TRUE((std::is_same_v<alp::Same_const_as<int, std::string>, int>),
                   "Same_const_as(no const)");

        CHECK_TRUE((std::is_same_v<alp::Same_const_as<int, const std::string>, const int>),
                   "Same_const_as(const)");
    }

    {
	struct A{
            using value_type        = long;
            using reference         = long;
            using pointer           = long;
            using difference_type   = long;
            using iterator_category = long;
            using iterator          = double;
            using const_iterator    = int;
        };

    CHECK_TRUE((std::is_same_v<typename alp::iterator_traits<A>::iterator, double>),
		"tipo con iterator");

    CHECK_TRUE((std::is_same_v<typename alp::iterator_traits<A>::const_iterator, int>),
		"tipo con const_iterator");

    CHECK_TRUE(
        (std::is_same_v<typename alp::iterator_traits<int*>::iterator, int*>),
        "puntero");

    CHECK_TRUE((std::is_same_v<
		typename alp::iterator_traits<int*>::const_iterator,
			    const int*>), 
		"const puntero");

    CHECK_TRUE(
        (std::is_same_v<typename alp::iterator_traits<const int*>::iterator, int*>),
        "const puntero");

    CHECK_TRUE((std::is_same_v<
		typename alp::iterator_traits<const int*>::const_iterator,
			    const int*>), 
		"const puntero");
    {
    using it = std::vector<int>::iterator;
    CHECK_TRUE((std::is_same_v<alp::iterator_traits<it>::const_iterator, 
	std::vector<int>::const_iterator>), "iterator_traits");


    using const_it = std::vector<int>::const_iterator;
    CHECK_TRUE((std::is_same_v<alp::iterator_traits<const_it>::iterator, 
	std::vector<int>::iterator>), "vector");

    CHECK_TRUE((std::is_same_v<alp::iterator_traits<const_it>::const_iterator, 
	std::vector<int>::const_iterator>), "vector");
    }
    {
    // iterator_traits::string
    CHECK_TRUE((!std::is_same_v<std::string::iterator,
                                    std::string::const_iterator>),
                   "string::iterator");

    using it = std::string::iterator;
    CHECK_TRUE((std::is_same_v<alp::iterator_traits<it>::iterator, 
	std::string::iterator>), "iterator_traits::string");

    CHECK_TRUE((std::is_same_v<alp::iterator_traits<it>::const_iterator, 
	std::string::const_iterator>), "iterator_traits::string");


    using const_it = std::string::const_iterator;
    CHECK_TRUE((std::is_same_v<alp::iterator_traits<const_it>::iterator, 
	std::string::iterator>), "iterator_traits::string");

    CHECK_TRUE((std::is_same_v<alp::iterator_traits<const_it>::const_iterator, 
	std::string::const_iterator>), "iterator_traits::string");
    }
    }
}


struct Punto{int x, y;};

struct F_no_const{
    int& operator()(Punto& p) {return p.x;}
};

struct F_const_return_value{
    int operator()(const Punto& p) {return p.x;}
};

struct F_const_return_reference{
    const int& operator()(const Punto& p) {return p.x;}
};

void test_view_traits()
{
    std::vector<Punto> v = {Punto{1, 10}, Punto{2, 20}, Punto{3, 30}};
    using It = std::vector<Punto>::iterator;

    {// View_traits
        using view = View_traits<F_no_const,
                                 typename std::iterator_traits<It>::reference>;

        CHECK_TRUE((std::is_same_v<view::value_type, int>), "View_traits");
	CHECK_TRUE((std::is_same_v<view::reference, int&>), "View_traits");
	CHECK_TRUE((std::is_same_v<view::pointer, int*>), "View_traits");
    }
    {// View_traits
        using view = View_traits<F_const_return_value,
                                 typename std::iterator_traits<It>::reference>;

        CHECK_TRUE((std::is_same_v<view::value_type, int>), "View_traits");
	CHECK_TRUE((std::is_same_v<view::reference, const int>), "View_traits");
	CHECK_TRUE((std::is_same_v<view::pointer, void>), "View_traits");

    }
    {// View_traits
        using view = View_traits<F_const_return_reference,
                                 typename std::iterator_traits<It>::reference>;

        CHECK_TRUE((std::is_same_v<view::value_type, int>), "View_traits");
	CHECK_TRUE((std::is_same_v<view::reference, const int&>), "View_traits");
	CHECK_TRUE((std::is_same_v<view::pointer, const int*>), "View_traits");

    }
}


//void test_less_than()
//{
//    test::interfaz("Less_than");
//
//    CHECK_TRUE((Wrapper<bool, Less_than<int, 3, 4>::value>::value == true),
//               "3 < 4?");
//    CHECK_TRUE((Wrapper<bool, Less_than<int, 4, 4>::value>::value == false),
//               "4 < 4?");
//    CHECK_TRUE((Wrapper<bool, Less_than<int, 4, 3>::value>::value == false),
//               "4 < 3?");
//}
//
//
//void test_pertenece_al_intervalo_cerrado()
//{
//    test::interfaz("test_pertenece_al_intervalo_cerrado");
//
//    CHECK_TRUE(
//        (Wrapper<bool,
//                 Pertenece_al_intervalo_cerrado<int, 5, 2, 8>::value>::value 
//		== true),
//        "5 pertenece a [2,8]?");
//
//    CHECK_TRUE(
//        (Wrapper<bool,
//                 Pertenece_al_intervalo_cerrado<int, 1, 2, 8>::value>::value 
//		== false),
//        "1 pertenece a [2,8]?");
//
//    CHECK_TRUE(
//        (Wrapper<bool,
//                 Pertenece_al_intervalo_cerrado<int, 10, 2, 8>::value>::value 
//		== false),
//        "10 pertenece a [2,8]?");
//}



int main()
{
try{
    test::header("alp_type_traits.h");

    test_type_traits();
    test_view_traits();
//    test_less_than();
//    test_pertenece_al_intervalo_cerrado();

}catch(std::exception& e)
{
    std::cerr << e.what() << endl;
    return 1;
}
}
