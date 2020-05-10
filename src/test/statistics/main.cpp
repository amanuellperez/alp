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

#include "../../alp_statistics.h"

#include "../../alp_test.h"
#include "../../alp_string.h"

#include <iostream>

void test_median(int a, int b, int c, int res)
{
    CHECK_TRUE(alp::median(a, b, c) == res,
               alp::as_str()
                   << "median(" << a << ", " << b << ", " << c << ")");
}

void test_median()
{
    test::interfaz("median");

    test_median(2, 2, 2, 2);

    test_median(2, 2, 3, 2);

    test_median(2, 6, 4, 4);
    test_median(4, 2, 6, 4);
    test_median(4, 6, 2, 4);
    test_median(6, 2, 4, 4);
    test_median(6, 4, 2, 4);
    test_median(6, 4, 2, 4);
}


int main()
{
try{
    test::header("alp::statistics");

    test_median();

}catch(std::exception& e)
{
    std::cerr << "EXCEPTION: " << e.what() << std::endl;
}

}
