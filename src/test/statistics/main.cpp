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

#include "../../alp_statistics.h"

#include "../../alp_test.h"
#include "../../alp_string.h"

#include <iostream>

using namespace test;

void test_median(int a, int b, int c, int res)
{
    CHECK_TRUE(alp::median(a, b, c) == res,
               alp::as_str()
                   << "median(" << a << ", " << b << ", " << c << ")");
}

void test_median()
{
    test::interface("median");

    test_median(2, 2, 2, 2);

    test_median(2, 2, 3, 2);

    test_median(2, 6, 4, 4);
    test_median(4, 2, 6, 4);
    test_median(4, 6, 2, 4);
    test_median(6, 2, 4, 4);
    test_median(6, 4, 2, 4);
    test_median(6, 4, 2, 4);
}

void test_frequency_table()
{
    test::interface("frequency_table");

    std::vector<std::string> data{"a", "b", "a", "b", "c", "a"};
    alp::Frequency_table<std::string> t;
    for (auto x: data)
	t.add(x);

    CHECK_TRUE(t.size() == 3, "size");
    CHECK_TRUE(t["a"] == 3, "a");
    CHECK_TRUE(t["b"] == 2, "b");
    CHECK_TRUE(t["c"] == 1, "c");

    std::cout << "No automáticas:\n";
    auto gsort = t.as_sorted_vector();
    for (const auto& x: gsort)
	std::cout << x.first << " = " << x.second << '\n';

    std::cout << "------\n";
    auto lsort = t.as_sorted_vector(std::less<size_t>{});
    for (const auto& x: lsort)
	std::cout << x.first << " = " << x.second << '\n';

}


int main()
{
try{
    test::header("alp::statistics");

    test_median();
    test_frequency_table();

}catch(std::exception& e)
{
    std::cerr << "EXCEPTION: " << e.what() << std::endl;
}

}
