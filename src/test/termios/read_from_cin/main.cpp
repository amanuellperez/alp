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

#include "../../../alp_termios_cfg.h"
#include "../../../alp_exception.h"
#include "../../../alp_string.h"

#include <atd_ascii.h>
#include <atd_ostream.h>

#include <iostream>
#include <limits>
#include <cctype>   // isprint


/* Use this variable to remember original terminal attributes. */
static alp::Termios_cfg old_cin_cfg;

static void reset_input_mode ()
{
    old_cin_cfg.apply_cfg_now(STDIN_FILENO);
}

enum class cin_type{
    polling_read,
    blocking_read,
    with_timeout
};

void cfg_cin(cin_type type)
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
    cfg.no_echo();

    switch(type){
	break; case cin_type::polling_read : cfg.noncanonical_polling_read();
	break; case cin_type::blocking_read: cfg.noncanonical_blocking_read(1);
	break; case cin_type::with_timeout : 
				    cfg.noncanonical_with_timeout(2);
    }
    if (cfg.apply_cfg_now(STDIN_FILENO) == -1)
	throw alp::Perror("cfg_cin::apply_cfg_now");
}


// std::isprint no considera printable: \n, \r
inline bool isprint(char c)
{
    return std::isprint(c) or 
	    (c == '\n')	or
	    (c == '\r');
}


int main(int argc, char* argv[])
{
try{
// menu
    std::cout << "stdin test\n"
	         "----------\n"
		 "Choose cfg:\n"
		 "1. Polling read (uses too much of % CPU)\n"
		 "2. Blocking read\n"
		 "3. With timeout\n";

    char type{};
    std::cin >> type;

    bool show_point = true;
    std::cout << "Show dot? (y/n) ";
    char opt;
    std::cin >> opt;
    if (opt == 'n' or opt == 'N')
	show_point = false;

    switch(type){ // Despues de todas las llamadas a std::cin 
	break; case '1': cfg_cin(cin_type::polling_read);
	break; case '2': cfg_cin(cin_type::blocking_read);
	break; case '3': cfg_cin(cin_type::with_timeout);
	break; default : cfg_cin(cin_type::polling_read);
    }



// run
    char c;
    while(1){
	if (::read(STDIN_FILENO, &c, 1)){
	    std::cout << "\nEscribes [";

	    if (isprint(c))
		std::cout << c;
	    else
		atd::print_int_as_hex(std::cout, static_cast<uint8_t>(c));

	    std::cout << "]\n" << std::flush;
	}
	else{
	    if (show_point) // para ver cuándo devuelve control read
		std::cout << '.' << std::flush;
	}
    }

}catch(const std::exception& e)
{
    std::cerr  << argv[0] << ": " << e.what() << '\n';
    return 1;
}

}



