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

#include "../../alp_vector.h"
#include "../../alp_test.h"
#include "../../alp_string.h"	// as_str() 

#include <iostream>
#include <sstream>

using namespace std;
using namespace alp;
using namespace test;

void test_print(const std::vector<int>& v,
                const std::string& sep,
                const std::string& res)
{
    std::stringstream s;
    print(s, v, sep);
    CHECK_TRUE(s.str() == res, "print");
}

void test_print()
{
    test::interfaz("print");

    test_print(std::vector<int>{}, " ", "");

    std::vector<int> v = {1,2,3,4,5};

    test_print(v, " ", "1 2 3 4 5");
    test_print(v, ", ", "1, 2, 3, 4, 5");
    test_print(v, "\n", "1\n2\n3\n4\n5");
}

int main()
{
try{
    test::header("alp_vector");

    test_print();

}catch(std::exception& e)
{
    std::cerr << e.what() << endl;
    return 1;
}
}
