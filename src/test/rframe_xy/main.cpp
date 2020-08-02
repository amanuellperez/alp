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

#include "../../alp_rframe_xy.h"
#include "../../alp_test.h"

#include <iostream>


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


int main()
{
try{
    test::header("alp_rframe_xy.h");

    test_vector_xy();
    test_rectangle_xy();


}catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}
