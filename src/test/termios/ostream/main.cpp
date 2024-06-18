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

// Programa que envía datos al avr via usb, para probar
// Termios_iostream.operator<<.
// En el avr poner un lcd y que vaya mostrando lo que le vamos enviando.
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

/* Use this variable to remember original terminal attributes. */
static alp::Termios_cfg old_cin_cfg;

static void reset_input_mode ()
{
    old_cin_cfg.apply_cfg_now(STDIN_FILENO);
}


void cfg_cin()
{
    /* Make sure stdin is a terminal. */
    if (!::isatty (STDIN_FILENO))
	throw alp::Perror{"cfg_cin"};

    /* Save the terminal attributes so we can restore them later. */
    old_cin_cfg.copy_cfg_from(STDIN_FILENO);
    ::atexit (reset_input_mode);

    /* Set the funny terminal modes. */
    alp::Termios_cfg cfg;
    cfg.copy_cfg_from(STDIN_FILENO);

    cfg.noncanonical_polling_read();
    if (cfg.apply_cfg_now(STDIN_FILENO) == -1)
	throw alp::Perror("cfg_cin::apply_cfg_now");
}


void test_interactivo()
{
    alp::Termios_cfg usb_cfg;
    // alp::cfg_avr_uart_read_with_timeout(usb_cfg, 2); // timeout = 200 ms
    alp::cfg_avr_uart_polling_read(usb_cfg); 

    alp::Termios_iostream usb{usb_port, usb_cfg};
    if (!usb.good()){
	std::cerr << "Error al abrir la conexión usb\n";
	return;
    }

    cfg_cin();
    std::cout << "Escribe lo que quieras, que lo enviamos al avr:\n";
    char c = 0;
    while (1){
	if (::read(STDIN_FILENO, &c, 1))
	    usb << c;
    }
}


void test_automatico()
{
    // conecta_usb()
    alp::Termios_cfg usb_cfg;
    alp::cfg_avr_uart_polling_read(usb_cfg); 

    alp::Termios_iostream usb{usb_port, usb_cfg};
    if (!usb.good()){
	std::cerr << "Error al abrir la conexión usb\n";
	return;
    }

    cfg_cin();

    // main
    constexpr const char msg[] = "1234567890abcdefghijklmnopqrstuvwxyz";
    while (1){
//	std::cout << msg << '\n';

//	int n = ::write(usb.fd(), msg, ::strlen(msg));
//	std::cout << "Escritos: [" << n << "] de [" << ::strlen(msg) << "]";
	for (size_t i = 0; i < strlen(msg); ++i){
//	    char c;
//	    while(::read(STDIN_FILENO, &c, 1) == 0) 
//	    { ; }
	    // Fundamental: no tengo implementado un buffer en UART, así que
	    // si envio demasiado rápido datos los pierdo. 
	    alp::sleep_for(std::chrono::milliseconds(100));
	    usb << msg[i];
	    if (!usb){
		std::cerr << "Error al intentar escribir\n";
		return;
	    }
		
	}
	
	alp::sleep_for(std::chrono::seconds(1));

	usb << '\n';
    }

}


int main(int argc, char* argv[])
{
try{
//    test_interactivo();
    test_automatico();
}catch(const std::exception& e)
{
    std::cerr  << argv[0] << ": " << e.what() << '\n';
    return 1;
}

}



