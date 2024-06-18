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

// Conectar el avr al usb via uart y que envía texto. Este programa lo lee y
// lo va mostrando en pantalla.
#include "../../../alp_termios_avr.h"
#include "../../../alp_exception.h"
#include "../../../alp_string.h"
#include "../../../alp_cerror.h"

#include <atd_ascii.h>
#include <atd_ostream.h>

#include <iostream>


void test_termios_cfg()
{
    std::string usb_port = "/dev/ttyUSB0";
    int fd = open(usb_port.c_str(), O_RDWR);

    if (fd == -1)
	throw alp::Cant_open_conexion{usb_port};

    
    // Bloqueamos esperando indefinidamente
    if (alp::cfg_avr_uart(fd, 1, 0) == -1){
	std::cerr << "Error al intentar configurar la conexión\n";
	return;
    }
	
    char c;
    while(read(fd, &c, sizeof(char)))
	std::cout <<  c << std::flush;
}




int main(int argc, char* argv[])
{
try{
    test_termios_cfg();

}catch(const std::exception& e)
{
    std::cerr  << argv[0] << ": " << e.what() << '\n';
    return 1;
}

}



