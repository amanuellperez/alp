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

#include "../../alp_functional.h"
#include "../../alp_test.h"

#include <iostream>

using namespace std;
using namespace alp;
using namespace test;

void test_less_than()
{
    test::interfaz("less_than");

    constexpr int i0 = 5;


    {
    Less_than r{i0};
    for (int i = 0; i < 10; ++i)
	CHECK_TRUE(r(i) == (i < i0), "Less_than");
    }


    {
    Less_equal_than r{i0};
    for (int i = 0; i < 10; ++i)
	CHECK_TRUE(r(i) == (i <= i0), "Less_equal_than");
    }

    {
    Greater_than r{i0};
    for (int i = 0; i < 10; ++i)
	CHECK_TRUE(r(i) == (i > i0), "Greater_than");
    }


    {
    Greater_equal_than r{i0};
    for (int i = 0; i < 10; ++i)
	CHECK_TRUE(r(i) == (i >= i0), "Greater_equal_than");
    }
}

int main()
{
try{
    test::header("alp_functional.h");

    test_less_than();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
}
}
