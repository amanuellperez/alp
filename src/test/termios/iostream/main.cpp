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

// Programa básico equivalente a screen. Probarlo con el test de uart.
#include "../../../alp_termios_iostream.h"
#include "../alp_termios_avr.h"
#include "../../../alp_exception.h"
#include "../../../alp_string.h"
#include "../../../alp_time.h"

#include <atd_ascii.h>
#include <atd_ostream.h>
#include <type_traits>
#include <iostream>
#include <string.h>

constexpr uint32_t baud_rate = 9'600;
//constexpr uint32_t baud_rate = 19'200;
//constexpr uint32_t baud_rate = 57'600;
//constexpr uint32_t baud_rate = 500'000;	    // Funciona 
//constexpr uint32_t baud_rate = 1'000'000; // no puedo probarlo con el atmega


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
	throw std::runtime_error{"cfg_cin"};

    /* Save the terminal attributes so we can restore them later. */
    old_cin_cfg.copy_cfg_from(STDIN_FILENO);
    ::atexit (reset_input_mode);

    /* Set the funny terminal modes. */
    alp::Termios_cfg cfg;
    cfg.copy_cfg_from(STDIN_FILENO);

    cfg.noncanonical_polling_read();
    if (cfg.apply_cfg_now(STDIN_FILENO) == -1)
	throw std::runtime_error("cfg_cin::apply_cfg_now");

    cfg.no_echo(); // TODO: El flag se cambia, pero no se modifica el comportamiento ???
    cfg.print_local_modes(std::cout);
//    cfg.print_as_hex(std::cout);
    std::cout << "---------------------------\n\n";
}


void test_termios_iostream()
{
    std::string usb_port = "/dev/ttyUSB0";
    alp::Termios_cfg usb_cfg;
    alp::cfg_avr_uart_polling_read<baud_rate>(usb_cfg); 

    alp::Termios_iostream usb{usb_port, usb_cfg};
    cfg_cin();

    usb_cfg.print(std::cout);


    // Observar la forma de hacer el polling. No puedo usar los operadores >>
    // ya que bloquean. Necesitamos llamar a read directamente.
    while (1){
	char c;
	if (alp::read(usb, c))
	    std::cout << c << std::flush;

	if (alp::cin_read(c))
	    usb << c;

	if (!std::cin){
	    perror("stdin error");
	    return;
	}
	if (!usb){
	    perror("usb error");
	    return;
	}
    }

}



int main(int argc, char* argv[])
{
try{
    test_termios_iostream();

}catch(const std::exception& e)
{
    std::cerr  << argv[0] << ": " << e.what() << '\n';
    return 1;
}

}



