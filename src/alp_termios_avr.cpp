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

#include "alp_termios_avr.h"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>	// memset

#include <string>

namespace alp{

// Es la configuración habitual que estoy usando en el USART del avr.
int cfg_avr_uart(int fd, int MIN, int TIME) noexcept
{
    Termios_cfg usb;

    cfg_avr_uart(usb, MIN, TIME);	

    usb.flush_data_not_read(fd);

    return usb.apply_cfg_now(fd);
}



// Es la configuración habitual que estoy usando en el USART del avr.
// timeout = in tenths of seconds
void cfg_avr_uart(Termios_cfg& cfg, int MIN, int time) noexcept
{
    // Configuramos el cfg
    cfg.baud_rate<9600>();
    cfg.character_size_bits_8();

    // OJO: si activo este flag, no lee correctamente del AVR!!!
//    cfg.parity_mode_bits_even();
    cfg.enable_receiver();
    cfg.ignore_modem_control_lines();

    // Input flags
    cfg.input_ignore_frame_and_parity_errors();

    // Output flags - Turn off output processing
    cfg.output_turn_off_output_processing();

    // set input mode (non-canonical, no echo,...)
    cfg.noncanonical_mode(MIN, time);
}





}// namespace


