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

// Programa que recibe un fichero desde el avr.
// Probamos Termios_iostream.operator>>
#include "../../../alp_termios_iostream.h"
#include "../alp_termios_avr.h"
#include "../../../alp_exception.h"
#include "../../../alp_string.h"
#include "../../../alp_cerror.h"
#include "../../../alp_time.h"

#include <atd_ascii.h>
#include <atd_ostream.h>
#include <type_traits>
#include <iostream>
#include <string.h>

static const std::string usb_port = "/dev/ttyUSB0";

void test_istream()
{
    alp::Termios_cfg usb_cfg;
    alp::cfg_avr_uart_blocking_read(usb_cfg); 

    alp::Termios_iostream usb{usb_port, usb_cfg};
    if (!usb.good()){
	std::cerr << "Error al abrir la conexión usb\n";
	return;
    }

//    while (usb >> c)	<-- este ignora espacios en blanco, '\n'...
//			    se puede usar noskipws, pero el get muestra que
//			    opero con bytes.
    char c;
    while (usb.get(c))
	std::cout << c << std::flush;

    // Probando string. Funciona!
//    std::string s;
////    while (std::getline(usb, s, '\n'))
//    while (usb >> s)
//	std::cout << s << ' ' << std::flush;
}


int main(int argc, char* argv[])
{
try{
    test_istream();
}catch(const std::exception& e)
{
    std::cerr  << argv[0] << ": " << e.what() << '\n';
    return 1;
}

}



