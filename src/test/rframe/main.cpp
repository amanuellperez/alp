// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "../../alp_rframes.h"
#include "../../alp_test.h"

#include <iostream>

template <typename Vector>
void test_vector()
{
    using V = Vector;

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

void test_vector_xy_local()
{
    test::interfaz("Vector_xy_local");

    using V = alp::Vector_xy_local<int>;
    test_vector<V>();
}


void test_vector_xy()
{
    test::interfaz("Vector_xy");
    
    using V0 = alp::Vector_xy<0>;
    using V1 = alp::Vector_xy<1>;
    
    test_vector<V0>();

    {
    V0 u1{2,3};
    V1 v1{2,3};
    CHECK_DONT_COMPILE(u1 == v1, "operator==");
    }
}


template <typename Vector>
void test_vectorij()
{
    using V = Vector;

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




void test_vector_ij()
{
    test::interfaz("Vector_ij");
    
    using V0 = alp::Vector_ij<0>;
    using V1 = alp::Vector_ij<1>;
    
    test_vectorij<V0>();

    {
    V0 u1{2,3};
    V1 v1{2,3};
    CHECK_DONT_COMPILE(u1 == v1, "operator==");
    }
}



void test_reference_frame()
{
    test::interfaz("Reference_frames_xy");

    using RF = alp::Reference_frames_xy<2>;

    RF::origin[0] = RF::Origin{0,0};
    RF::origin[1] = RF::Origin{0,7};

    using Vector0 = alp::Vector_xy<0>;
    using Vector1 = alp::Vector_xy<1>;

    Vector0 p1{10,20};
    Vector1 q1{10,20};

    std::cout << "p1 = " << p1 << '\n';
    std::cout << "q1 = " << q1 << '\n';

    Vector1 q2 = RF::convert<1>(p1);
    std::cout << "q2 = " << q2 << '\n';

    Vector0 p2 = RF::convert<0>(q2);
    CHECK_TRUE(p2 == p1, "convert");


    using Pos = alp::Vector_ij<1>;

    {
    Pos pos{0, 0};
    std::cout << "pos = " << pos << '\n';

    Vector1 p1 = pos;
    std::cout << "p1 = " << p1 << '\n';

    Vector0 q = RF::convert<0> (p1);
    std::cout << "q = " << q << '\n';

    pos = Pos{3,6};
    q = RF::convert<0> (pos);
    CHECK_TRUE((q == Vector0{6,4}), "convert(ij -> xy)");

    }
}

void test_segment()
{
    test::interfaz("Segment");

    using Point = alp::Vector_ij_local<int>;
    using S = alp::Segment<Point>;
    Point A{2,3};
    Point B{4,5};
    S seg{A, B};
    CHECK_TRUE(seg == seg, "operator==");
    CHECK_TRUE(!(seg != seg), "operator!=");

    std::cout << seg << '\n';
    
}


void test_rectangle_xy()
{
    test::interfaz("Rectangle_xy");

    using Point = alp::Vector_xy_local<int>;
    using Rect = alp::Rectangle_xy<alp::Vector_xy_local<int>>;
    {
    Rect r;
    CHECK_TRUE(r.width() == 0 and r.height() == 0, "constructor null");
    CHECK_TRUE(r.empty(), "empty");
    }
    {
//    Point A{2,3};
//    Rect r{A, 5, 6};
    
    Rect r{Point{2,8}, Point{6,3}};
    CHECK_TRUE(r.width() == 5 and r.height() == 6, "constructor");
    CHECK_TRUE(!r.empty(), "!empty");

    CHECK_TRUE((r.bottom_left_corner() == Point{2,3}), "bottom_left_corner");
    CHECK_TRUE((r.bottom_right_corner() == Point{6,3}), "bottom_right_corner");
    CHECK_TRUE((r.upper_left_corner() == Point{2,8}), "upper_left_corner");
    CHECK_TRUE((r.upper_right_corner() == Point{6,8}), "upper_right_corner");

    CHECK_TRUE(r == r, "operator==");
    CHECK_TRUE(!(r != r), "operator!=");
    }

    {// orden
    Rect r0{Point{2,8}, Point{6,3}};
    Rect r1{Point{6,3}, Point{2,8}};
    std::cout << r1.upper_left_corner() << ", " << r1.bottom_right_corner() << '\n';
    CHECK_TRUE(r0 == r1, "invariante constructor");

    }


}


int main()
{
try{
    test::header("alp_rframes.h");
 
    test_vector_xy_local();
    test_vector_xy();
    test_vector_ij();
    test_reference_frame();

    test_segment();
    test_rectangle_xy();


}catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}

}
