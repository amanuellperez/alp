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


#include "../../alp_rframe_xyz.h"
#include "../../alp_math.h"

#include "../../alp_test.h"

#include <iostream>
#include <vector>


using namespace test;

void test_vector_xyz()
{
    test::interfaz("Vector_xyz");
    
    using V = alp::Vector_xyz<int>;

    V a{1,2,3};
    V b;
    b = a;

    CHECK_TRUE(a == b, "operator==()");

    b.x = 5;
    CHECK_TRUE(a != b, "operator!=()");

    auto c = a + b;
    CHECK_TRUE((c == V{6, 4, 6}), "operator+=");

    b.z = -4;
    c = a - b;
    CHECK_TRUE((c == V{-4, 0, 7}), "operator-=");

    c = 2*a;
    CHECK_TRUE((c == V{2, 4, 6}), "operator*");

    a = {1, 2, 3};
    b = {4, 5, 6};
    auto x = dot_product(a, b);

    CHECK_TRUE(x == 4 + 10 + 18, "dot_product");

    c = cross_product(a, b);
    CHECK_TRUE((c == V{-3, 6, -3}), "cross_product");

}




int main()
{
try{
    test_vector_xyz();

}catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}

}
