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

#include "../../alp_rframe_xy.h"
#include "../../alp_test.h"

#include <iostream>

using namespace test;

void test_vector_xy()
{
    test::interfaz("Vector_xy");

    using V = alp::Vector_xy<int>;

    {// basic
    V u{2, 3};
    CHECK_TRUE(u.x == 2 and u.y == 3, "constructor");

    V v = u;
    CHECK_TRUE(v.x == u.x and v.y == u.y, "constructor");

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

void test_rectangle_xy()
{
    test::interfaz("Rectangle_xy");

    using Rect = alp::Rectangle_xy<int>;
    using V = alp::Vector_xy<int>;

    {
	Rect r;
	CHECK_TRUE((r.bottom_left_corner() == V{0,0}), "null constructor");
	CHECK_TRUE(r.width() == 0, "null constructor");
	CHECK_TRUE(r.height() == 0, "null constructor");
	CHECK_TRUE(r.empty(), "empty");
    }

    {// básica
	Rect r{V{7,2}, 6, 3};
	
	CHECK_TRUE((r.bottom_left_corner() == V{7,2}), "bottom_left_corner");
	CHECK_TRUE((r.bottom_right_corner() == V{12,2}), "bottom_right_corner");
	CHECK_TRUE((r.upper_left_corner() == V{7,4}), "upper_left_corner");
	CHECK_TRUE((r.upper_right_corner() == V{12,4}), "upper_right_corner");

	CHECK_TRUE(r.width() == 6, "width");
	CHECK_TRUE(r.height() == 3, "height");

	CHECK_TRUE(!r.empty(), "empty");

	CHECK_TRUE(r == r, "operator==");
	CHECK_TRUE(!(r != r), "operator!=");
    }
}

void test_rotate()
{
    test::interfaz("rotate");
    
    using V = alp::Vector_xy<int>;

    {
    V v{1,2};
    CHECK_TRUE((alp::rotate(v, alp::Degree{0}) == v), "rotate(0)");
    CHECK_TRUE((alp::rotate(v, alp::Degree{360}) == v), "rotate(360)");
    CHECK_TRUE((alp::rotate(v, alp::Degree{-360}) == v), "rotate(-360)");
    }
    {
    V v{3,0};
    CHECK_TRUE((alp::rotate(v, alp::Degree{90}) == V{0,3}), "rotate(90)");
    CHECK_TRUE((alp::rotate(v, alp::Degree{180}) == V{-3,0}), "rotate(180)");
    CHECK_TRUE((alp::rotate(v, alp::Degree{270}) == V{0,-3}), "rotate(270)");

    CHECK_TRUE((alp::rotate(v, alp::Degree{-90}) == V{0,-3}), "rotate(-90)");
    CHECK_TRUE((alp::rotate(v, alp::Degree{-180}) == V{-3,0}), "rotate(-180)");
    CHECK_TRUE((alp::rotate(v, alp::Degree{-270}) == V{0,3}), "rotate(-270)");
    }
    {
    V v{1,1};
    // observar el error cometido por culpa de manejar vectores de int
    CHECK_TRUE((alp::rotate(v, alp::Degree{45}) == V{0, 1}), "rotate(45)");
    CHECK_TRUE((alp::rotate(v, alp::Degree{90}) == V{-1, 1}), "rotate(90)");
    CHECK_TRUE((alp::rotate(v, alp::Degree{135}) == V{-1, 0}), "rotate(135)");
    }

    {
    using V = alp::Vector_xy<double>;
    V v{1.0,1.0};
    alp::Aproximado aprox{0.0001};
    {
    V u = alp::rotate(v, alp::Degree{45});
    V res{0.0, sqrt(2)};
    CHECK_TRUE((aprox(u.x, res.x) and aprox(u.y, res.y)), "rotate(45)");
    }
    {
    V u = alp::rotate(v, alp::Degree{90});
    V res{-1.0, 1.0};
    CHECK_TRUE((aprox(u.x, res.x) and aprox(u.y, res.y)), "rotate(90)");
    }
    {
    V u = alp::rotate(v, alp::Degree{-180});
    V res{-1.0, -1.0};
    CHECK_TRUE((aprox(u.x, res.x) and aprox(u.y, res.y)), "rotate(-180)");
    }
    {
    V u = alp::rotate(v, alp::Degree{-135});
    V res{0.0, -sqrt(2)};
    CHECK_TRUE((aprox(u.x, res.x) and aprox(u.y, res.y)), "rotate(-135)");
    }

    }
}

int main()
{
try{
    test::header("alp_rframe_xy.h");

    test_vector_xy();
    test_rectangle_xy();

    test_rotate();

}catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}
