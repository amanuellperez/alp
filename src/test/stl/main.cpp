// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#include "../../alp_stl.h"

#include "../../alp_test.h"

#include <iostream>


using std::cout;
using std::endl;
using std::vector;

using namespace alp;
using namespace test;

void test_copy_new()
{
    std::vector<int> u = {1,2,3,4};
    std::vector<int> v = {10,11};

    copy_new(v, begin(u), end(u));
    check_equal_containers(begin(v), end(v), begin(u), end(u), "copy_new");
}

struct A{int x, y;};

struct A_x{
    using value_type = int;
    int operator()(const A& a) const {return a.x;}
};

struct A_y{
    using value_type = int;
    int operator()(const A& a) const {return a.y;}
};

std::ostream& operator<<(std::ostream& out, const A& a)
{ return out << "(" << a.x << ", " << a.y << ")";}

void test_mascara_propiedad()
{
    std::vector<A> v = {{1,2}, {3,4}, {5,6}};
    for (auto a: v)
	cout << a << ' ';

    cout << '\n';

    auto m_x = mascara_propiedad_contenedor1_aleatorio(v, A_x{});
    auto m_y = mascara_propiedad_contenedor1_aleatorio(v, A_y{});
//    for (auto x: m)
//	cout << x << ' ';
//    cout << '\n';
    for (int i = 0; i < m_x.size(); ++i)
	cout << m_x[i] << ' ';
    cout << '\n';

    for (int i = 0; i < m_y.size(); ++i)
	cout << m_y[i] << ' ';
    cout << '\n';

    for (auto x: m_x)
	cout << x << ' ';
    cout << '\n';
}

void test_incr()
{
    {
    check_true(Incr_positivo_menor_que<int>{3}(2,3) == true
				    , "Incr_positivo_menor_que(true)");

    check_true(Incr_positivo_menor_que<int>{3}(2,10) == false
				    , "Incr_positivo_menor_que(false)");

    check_true(Incr_positivo_menor_que<int>{3}(2,5) == false
				    , "Incr_positivo_menor_que(false)");

    check_true(Incr_positivo_menor_que<int>{3}(5,2) == false
				    , "Incr_positivo_menor_que(false)");
    }

    {
    check_true(Incr_positivo_menor_o_igual_que<int>{3}(2,3) == true
				    , "Incr_positivo_menor_o_igual_que(true)");

    check_true(Incr_positivo_menor_o_igual_que<int>{3}(2,10) == false
				    , "Incr_positivo_menor_o_igual_que(false)");

    check_true(Incr_positivo_menor_o_igual_que<int>{3}(2,5) == true
				    , "Incr_positivo_menor_o_igual_que(true)");

    check_true(Incr_positivo_menor_o_igual_que<int>{3}(5,2) == false
				    , "Incr_positivo_menor_o_igual_que(false)");
    }

    {
    check_true(Incr_positivo_mayor_que<int>{3}(2,3) == false
				    , "Incr_positivo_mayor_que(false)");

    check_true(Incr_positivo_mayor_que<int>{3}(2,10) == true
				    , "Incr_positivo_mayor_que(true)");

    check_true(Incr_positivo_mayor_que<int>{3}(2,5) == false
				    , "Incr_positivo_mayor_que(false)");

    check_true(Incr_positivo_mayor_que<int>{3}(5,2) == false
				    , "Incr_positivo_mayor_que(false)");
    }

    {
    check_true(Incr_positivo_mayor_o_igual_que<int>{3}(2,3) == false
				    , "Incr_positivo_mayor_o_igual_que(false)");

    check_true(Incr_positivo_mayor_o_igual_que<int>{3}(2,10) == true
				    , "Incr_positivo_mayor_o_igual_que(true)");

    check_true(Incr_positivo_mayor_o_igual_que<int>{3}(2,5) == true
				    , "Incr_positivo_mayor_o_igual_que(true)");

    check_true(Incr_positivo_mayor_o_igual_que<int>{3}(5,2) == false
				    , "Incr_positivo_mayor_o_igual_que(false)");
    }


    // --------------

    {
    check_true(Incr_negativo_menor_que<int>{3}(3, 2) == true
				    , "Incr_negativo_menor_que(true)");

    check_true(Incr_negativo_menor_que<int>{3}(10, 2) == false
				    , "Incr_negativo_menor_que(false)");

    check_true(Incr_negativo_menor_que<int>{3}(5, 2) == false
				    , "Incr_negativo_menor_que(false)");

    check_true(Incr_negativo_menor_que<int>{3}(2,5) == false
				    , "Incr_negativo_menor_que(false)");
    }

    {
    check_true(Incr_negativo_menor_o_igual_que<int>{3}(3, 2) == true
				    , "Incr_negativo_menor_o_igual_que(true)");

    check_true(Incr_negativo_menor_o_igual_que<int>{3}(10, 2) == false
				    , "Incr_negativo_menor_o_igual_que(false)");

    check_true(Incr_negativo_menor_o_igual_que<int>{3}(5, 2) == true
				    , "Incr_negativo_menor_o_igual_que(true)");

    check_true(Incr_negativo_menor_o_igual_que<int>{3}(2,5) == false
				    , "Incr_negativo_menor_o_igual_que(false)");
    }


    {
    check_true(Incr_negativo_mayor_que<int>{3}(3, 2) == false
				    , "Incr_negativo_mayor_que(false)");

    check_true(Incr_negativo_mayor_que<int>{3}(10, 2) == true
				    , "Incr_negativo_mayor_que(true)");

    check_true(Incr_negativo_mayor_que<int>{3}(5, 2) == false
				    , "Incr_negativo_mayor_que(false)");

    check_true(Incr_negativo_mayor_que<int>{3}(2,5) == false
				    , "Incr_negativo_mayor_que(false)");
    }


    {
    check_true(Incr_negativo_mayor_o_igual_que<int>{3}(3, 2) == false
				    , "Incr_negativo_mayor_o_igual_que(false)");

    check_true(Incr_negativo_mayor_o_igual_que<int>{3}(10, 2) == true
				    , "Incr_negativo_mayor_o_igual_que(true)");

    check_true(Incr_negativo_mayor_o_igual_que<int>{3}(5, 2) == true
				    , "Incr_negativo_mayor_o_igual_que(false)");

    check_true(Incr_negativo_mayor_o_igual_que<int>{3}(2,5) == false
				    , "Incr_negativo_mayor_o_igual_que(false)");
    }

}




int main()
{
try{
    test_copy_new();
    test_mascara_propiedad();

    test_incr();

}catch(alp::Excepcion e)
{
    std::cerr << e.what() << std::endl;
}

    return 0;
}
