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

#include "../../alp_bit.h"
#include "../../alp_test.h"
#include "../../alp_string.h"

#include <iostream>

using namespace std;
using namespace alp;
using namespace test;

template <int pos>
void test_bit(int x, int res)
{
    CHECK_TRUE(bit<pos>(x) == res, as_str() << "bit(" << x << ")");
}

void test_bit()
{
    test::interfaz("bit");

    test_bit<0>(0x01, 1);
    test_bit<0>(0x02, 0);
    test_bit<1>(0x02, 1);
    test_bit<1>(0x04, 0);
    test_bit<2>(0x04, 1);
    test_bit<2>(0x08, 0);
    test_bit<3>(0x08, 1);
    test_bit<3>(0x10, 0);
    test_bit<4>(0x10, 1);
    test_bit<4>(0x20, 0);
}

int main()
{
try{
    test::header("bit");

    test_bit();

}catch(std::exception& e)
{
    std::cerr << e.what() << endl;
    return 1;
}
}
