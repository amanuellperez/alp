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

#include "../../alp_getopts.h"
#include "../../alp_test.h"
#include "../../alp_string.h"	// as_str() 

#include <iostream>

using namespace std;
using namespace alp;
using namespace test;


int main(int argc, char* argv[])
{
try{
    const std::string program_name = argv[0];
    
    std::string output, input;
    int num = 22;
    unsigned int unum = 50;
    float time = 23.2;
    bool ok = false;
    bool b1 = false;
    bool b2 = false;
    bool help = false;

    bool str = false;
    int number = 0;

    Getopts args{1,
                 "Forma de uso: xx [-o output] [-i input] [-t time] [-n num] "
                 "[-u unum] [-k] [-1] [-2] [--str] [--number n] file [file2...]"};

    args.add_option('o', output);
    args.add_option('i', input);
    args.add_option("time", time);
    args.add_option('n', num);
    args.add_option('u', unum);
    args.add_option('k', ok);
    args.add_option('1', b1);
    args.add_option('2', b2);
    args.add_option("str", str);
    args.add_option("number", number);


    // TODO: añadir -help (se le puede añadir en la función parse. Se mira si
    // tiene esta opción, si no la tiene, se toma por defecto -help).
    auto resto = args.parse(argc, argv);

    if (help){
	std::cerr << "esta es la ayuda\n";
	return 1;
    }

    std::cout << "n " << num << '\n';
    std::cout << "u " << unum << '\n';
    std::cout << "time " << time << '\n';
    std::cout << "i " << input << '\n';
    std::cout << "o " << output << '\n';
    std::cout << "k " << ok << '\n';
    std::cout << "1 " << b1 << '\n';
    std::cout << "2 " << b2 << '\n';
    std::cout << "str " << str << '\n';
    std::cout << "number " << number << '\n';

    for (auto x: resto)
	std::cout << "resto " << x << "\n";
}
catch(std::exception& e)
{
    std::cerr << e.what() << endl;
    return 1;
}
}
