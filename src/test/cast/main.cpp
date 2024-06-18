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

#include "../../alp_cast.h"
#include "../../alp_test.h"
#include "../../alp_string.h"
#include <iostream>

using namespace std;
using namespace alp;
using namespace test;


void test_narrow_cast()
{
    test::interfaz("narrow_cast");

    unsigned int ui = 10u;
    int si = 10;
    CHECK_TRUE(narrow_cast<int>(ui) == si, "narrow_cast<int>");
    CHECK_TRUE(narrow_cast<int, int>(si) == si, "narrow_cast<int=int>");
}


void test_char2int(char c, int res)
{
    int i = char2int(c);
    CHECK_TRUE(i == res, as_str() << "char2int(" << c << ")");
}

void test_char2int(char c)
{
    int i = char2int(c);
    CHECK_TRUE(static_cast<char>(i) == c, as_str() << "char2int(" << c << ")");
}

void test_char2int()
{
    test::interfaz("char2int");

    test_char2int('A');
    test_char2int('a');
    test_char2int('A', 65);
    test_char2int('a', 97);

}


int main()
{
try{
    test::header("alp_cast");

    test_narrow_cast();
    test_char2int();

}catch(std::exception& e){
    std::cerr << e.what() << endl;
    return 1;
}
}
