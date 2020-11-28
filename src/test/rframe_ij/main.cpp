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

#include "../../alp_rframe_ij.h"
#include "../../alp_test.h"

#include <iostream>


void test_vector_ij()
{
    test::interfaz("Vector_ij");

    using V = alp::Vector_ij<int>;

    {// basic
    V u{2, 3};
    CHECK_TRUE(u.i == 2 and u.j == 3, "constructor");

    V v = u;
    CHECK_TRUE(v.i == u.i and v.j == u.j, "constructor");

    CHECK_TRUE(v == u, "operator==");
    v = V{5,6};
    CHECK_TRUE(v != u, "operator!=");
    }

    {// operator+=
    V u{2, 3};
    V v{4, 6};
    u += v;
    CHECK_TRUE((u == V{6,9}), "operator+=");
    u -= v;
    CHECK_TRUE((u == V{2,3}), "operator-=");
    u *= 3;
    CHECK_TRUE((u == V{6,9}), "operator*=");
    u /= 3;
    CHECK_TRUE((u == V{2,3}), "operator/=");
    }

    {// operations
    V u {2, 3};
    V v {4, 6};

    CHECK_TRUE(((u + v) == V{6,9}), "operator+");
    CHECK_TRUE(((u - v) == V{-2,-3}), "operator-");
    CHECK_TRUE(((2*u) == V{4, 6}), "operator*");
    CHECK_TRUE(((u*2) == V{4, 6}), "operator*");
    CHECK_TRUE(((v/2) == V{2, 3}), "operator*");
    }

}

void test_rango_ij()
{
    test::interfaz("Rango_ij");

    using R = alp::Rango_ij<int>;
    using Pos = alp::Rango_ij<int>::Posicion;

    {// vacio
    R r;
    CHECK_TRUE(r.i0 == 0 and r.ie == 0 and r.j0 == 0 and r.je == 0,
               "Rango_ij::vacio");
    CHECK_TRUE(r.empty(), "empty()");
    }
    {// basico
    R r{1, 5, 3, 6};
    CHECK_TRUE(r.i0 == 1 and r.ie == 5 and r.j0 == 3 and r.je == 6,
	    "Rango_ij::Rango_ij");
    CHECK_TRUE(!r.empty(), "empty()");
    CHECK_TRUE(r.rows() == 4, "rows()");
    CHECK_TRUE(r.cols() == 3, "cols()");
    CHECK_TRUE((r.upper_left_corner() == Pos{1,3}), "upper_left_corner");
    CHECK_TRUE((r.upper_right_corner() == Pos{1,5}), "upper_right_corner");
    CHECK_TRUE((r.bottom_left_corner() == Pos{4,3}), "bottom_left_corner");
    CHECK_TRUE((r.bottom_right_corner() == Pos{4,5}), "bottom_right_corner");
    }

    {// ok
    R r{Pos{2,3}, Pos{5,8}};
    CHECK_TRUE(r.i0 == 2 and r.ie == 6 and r.j0 == 3 and r.je == 9,
	    "Rango_ij::Rango_ij(pos)");
    CHECK_TRUE((r.upper_left_corner() == Pos{2,3}), "upper_left_corner");
    CHECK_TRUE((r.bottom_right_corner() == Pos{5,8}), "bottom_right_corner");
    }
    {// desordenado
    R r{Pos{5,8}, Pos{2,3}};
    CHECK_TRUE(r.i0 == 2 and r.ie == 6 and r.j0 == 3 and r.je == 9,
	    "Rango_ij::Rango_ij(pos-des)");
    CHECK_TRUE((r.upper_left_corner() == Pos{2,3}), "upper_left_corner");
    CHECK_TRUE((r.bottom_right_corner() == Pos{5,8}), "bottom_right_corner");
    }

    {
    R r{1, 5, 3, 6};
    CHECK_TRUE(r.i0 == 1 and r.ie == 5 and r.j0 == 3 and r.je == 6,
	    "Rango_ij::Rango_ij");
    r.upper_left_corner(Pos{10,8});
    CHECK_TRUE(r.i0 == 4 and r.ie == 11 and r.j0 == 5 and r.je == 9,
		"upper_left_corner(10,8)");

    r.bottom_right_corner(Pos{20,12});
    CHECK_TRUE(r.i0 == 4 and r.ie == 21 and r.j0 == 5 and r.je == 13,
		"bottom_right_corner(20,12)");
    }
    {// ordena indices
    R r{2, 1, 4, 3};
    CHECK_TRUE(r.i0 == 1 and r.ie == 2 and r.j0 == 3 and r.je == 4,
	    "Rango_ij::Rango_ij - indices desordenados");
    }
}

void test_alrededor()
{
    test::interfaz("alrededor");

    using R = alp::Rango_ij<int>;
    using Pos = alp::Rango_ij<int>::Posicion;

    R r{Pos{0,0}, Pos{2,2}};

    {
    std::vector<Pos> res{Pos{0,0}, Pos{0,1}, Pos{0,2},
			 Pos{1,0},           Pos{1,2},
			 Pos{2,0}, Pos{2,1}, Pos{2,2}};

    auto y = alp::alrededor(r, Pos{1,1});
//    test::print_range(y.begin(), y.end());
    CHECK_EQUAL_CONTAINERS_C(res, y, "alrededor(1,1)");
    }
    {
    std::vector<Pos> res{          Pos{0,1},
			 Pos{1,0}, Pos{1,1}};

    auto y = alp::alrededor(r, Pos{0,0});
    test::print_range(y.begin(), y.end());
    CHECK_EQUAL_CONTAINERS_C(res, y, "alrededor(0,0)");
    }

    {
    std::vector<Pos> res{Pos{0,0},           Pos{0,2},
			 Pos{1,0}, Pos{1,1}, Pos{1,2}};

    auto y = alp::alrededor(r, Pos{0,1});
    CHECK_EQUAL_CONTAINERS_C(res, y, "alrededor(0,1)");
    }

    {
    std::vector<Pos> res{          Pos{0,1}, 
			           Pos{1,1}, Pos{1,2}};

    auto y = alp::alrededor(r, Pos{0,2});
    CHECK_EQUAL_CONTAINERS_C(res, y, "alrededor(0,2)");
    }

    {
    std::vector<Pos> res{Pos{0,0}, Pos{0,1},
			           Pos{1,1},
			 Pos{2,0}, Pos{2,1}};

    auto y = alp::alrededor(r, Pos{1,0});
    CHECK_EQUAL_CONTAINERS_C(res, y, "alrededor(1,0)");
    }

    {
    std::vector<Pos> res{          Pos{0,1}, Pos{0,2},
			           Pos{1,1},
			           Pos{2,1}, Pos{2,2}};

    auto y = alp::alrededor(r, Pos{1,2});
    CHECK_EQUAL_CONTAINERS_C(res, y, "alrededor(1,2)");
    }

    {
    std::vector<Pos> res{
			 Pos{1,0}, Pos{1,1}, 
			           Pos{2,1}};

    auto y = alp::alrededor(r, Pos{2,0});
    CHECK_EQUAL_CONTAINERS_C(res, y, "alrededor(2,0)");
    }

    {
    std::vector<Pos> res{
			 Pos{1,0}, Pos{1,1}, Pos{1,2},
			 Pos{2,0}          , Pos{2,2}};

    auto y = alp::alrededor(r, Pos{2,1});
    CHECK_EQUAL_CONTAINERS_C(res, y, "alrededor(2,1)");
    }


    {
    std::vector<Pos> res{
			           Pos{1,1}, Pos{1,2},
			           Pos{2,1}          };

    auto y = alp::alrededor(r, Pos{2,2});
    CHECK_EQUAL_CONTAINERS_C(res, y, "alrededor(2,2)");
    }

}

void test_posicion_del_centro()
{
    test::interfaz("posicion_del_centro");

    using R = alp::Rango_ij<int>;
    using Pos = alp::Rango_ij<int>::Posicion;

    {
    R r{Pos{0,0}, Pos{2,2}};
    Pos c = alp::posicion_del_centro(r);
    CHECK_TRUE(c.i == 1 and c.j == 1, "posicion_del_centro");
    }
    {
    R r{Pos{0,0}, Pos{3,3}};
    Pos c = alp::posicion_del_centro(r);
    CHECK_TRUE(c.i == 2 and c.j == 2, "posicion_del_centro");
    }

    {
    R r{Pos{8,8}, Pos{10,10}};
    Pos c = alp::posicion_del_centro(r);
    CHECK_TRUE(c.i == 9 and c.j == 9, "posicion_del_centro");
    }
    {
    R r{Pos{8,8}, Pos{11,11}};
    Pos c = alp::posicion_del_centro(r);
    CHECK_TRUE(c.i == 10 and c.j == 10, "posicion_del_centro");
    }


}

int main()
{
try{
    test::header("alp_rframe_ij.h");

    test_vector_ij();
    test_rango_ij();
    test_alrededor();
    test_posicion_del_centro();

}catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}
