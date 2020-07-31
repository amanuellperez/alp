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


#include "../../alp_vector3D.h"
#include "../../alp_math.h"

#include "../../alp_exception.h"
#include "../../alp_test.h"

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

using namespace alp;
using namespace test;


void test_vector3D()
{
    Vector3D<int> a{1,2,3};
    Vector3D<int> b;
    b = a;

    check_true(a == b, "operator==()");

    b.x = 5;
    check_true(a != b, "operator!=()");

    auto c = a + b;
    check_true(c == Vector3D<int>{6, 4, 6}, "operator+=");

    b.z = -4;
    c = a - b;
    check_true(c == Vector3D<int>{-4, 0, 7}, "operator-=");

    c = 2*a;
    check_true(c == Vector3D<int>{2, 4, 6}, "operator*");

    a = {1, 2, 3};
    b = {4, 5, 6};
    auto x = dot_product(a, b);

    check_true(x == 4 + 10 + 18, "dot_product");

    c = cross_product(a, b);
    check_true(c == Vector3D<int>{-3, 6, -3}, "cross_product");

}




int main()
{
try{
    test_vector3D();

}catch(const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}

    return 0;
}
