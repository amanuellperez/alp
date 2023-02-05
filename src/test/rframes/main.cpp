// Copyright (C) 2020 Manuel Perez <manuel2perez@proton.me>
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


using namespace test;

template <typename V>
void test_vector()
{
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

void test_vector_xy()
{
    test::interfaz("Vector_xy_in");
    
    using V0 = alp::Vector_xy_in<0>;
    using V1 = alp::Vector_xy_in<1>;
    
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
    test::interfaz("Vector_ij_in");
    
    using V0 = alp::Vector_ij_in<0>;
    using V1 = alp::Vector_ij_in<1>;
    
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

    using Vector0 = alp::Vector_xy_in<0>;
    using Vector1 = alp::Vector_xy_in<1>;

    Vector0 p1{10,20};
    Vector1 q1{10,20};

    {
    CHECK_DONT_COMPILE(auto kk = p1 + q1, "suma")
    CHECK_DONT_COMPILE(auto kk = p1 - q1, "resta")
    }
    std::cout << "p1 = " << p1 << '\n';
    std::cout << "q1 = " << q1 << '\n';

    Vector1 q2 = RF::convert<1>(p1);
    std::cout << "q2 = " << q2 << '\n';

    Vector0 p2 = RF::convert<0>(q2);
    CHECK_TRUE(p2 == p1, "convert");


    using Pos = alp::Vector_ij_in<1>;

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




int main()
{
try{
    test::header("alp_rframes.h");
 
    test_vector_xy();
    test_vector_ij();
    test_reference_frame();



}catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}

}
