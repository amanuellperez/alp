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

// Envia un fichero desde el avr al pc. Para probar el operator>> de
// Termios_iostream.
#include <avr_USART_iostream.h>
#include <avr_time.h>


void solo_envia()
{
    avr::UART_iostream uart;
    cfg_basica(uart);
    uart.on();

    uart << "\n---------------\n";
    uart << "Probando UART!\n";
    uart << "---------------\n";

    while (1) {
        uart << "Esto es el quijote, pero como sería "
                "muy largo, creo que paso\nde escribirlo\n\n";

	wait_ms(200);
    }

}

int main()
{
    solo_envia();
}

