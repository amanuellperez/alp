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

#include "../../../alp_termios_cfg.h"
#include "../../../alp_exception.h"
#include "../../../alp_string.h"

#include <atd_ascii.h>
#include <atd_ostream.h>

#include <iostream>


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
    cfg.no_echo();

    cfg.noncanonical_polling_read();
    if (cfg.apply_cfg_now(STDIN_FILENO) == -1)
	throw alp::Perror("cfg_cin::apply_cfg_now");
}


int main(int argc, char* argv[])
{
try{
    cfg_cin();

    char c;
    while(1){
	if (::read(STDIN_FILENO, &c, 1))
	    std::cout << "\nEscribes [" << c << "]\n" << std::flush;
    }

}catch(const std::exception& e)
{
    std::cerr  << argv[0] << ": " << e.what() << '\n';
    return 1;
}

}



