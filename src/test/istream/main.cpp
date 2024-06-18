// Copyright (C) 2022 Manuel Perez 
//           mail: <manuel2perez@proton.me>
//           https://github.com/amanuellperez/mcu
//
// This file is part of the ALP Library.
//
// MCU++ Library is a free library: you can redistribute it and/or modify
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
#include "../../alp_istream.h"
#include "../../alp_test.h"

#include <iostream>
#include <string>
#include <fstream>


using namespace test;

void test_read()
{
    const std::string fname = "kk.tmp";
    int x[4] = {10, 20, 30, 40};
    constexpr size_t N = sizeof(x);

{
    std::ofstream out{fname};

    out.write((char*) x, N);
    out.close();
}

{
    std::byte b[N];
    std::ifstream in{fname};
    alp::read(in, b, N);

    for (size_t i = 0; i < 4; ++i){
	int* res = reinterpret_cast<int*>(&b[sizeof(int)*i]);
	CHECK_TRUE(*res == x[i], "read");

    }
}
    
}


int main(int argc, char* argv[])
{
try{
    test::header("alp_istream");

    test_read();
}
catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}
