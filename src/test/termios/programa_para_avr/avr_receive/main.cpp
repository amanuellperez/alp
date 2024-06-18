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

// Este programa recibe todo lo que se envíe por UART y lo muestra en el LCD.
// Lo uso para probar termios.operator<<.
#include <avr_USART_iostream.h>
#include <avr_time.h>
#include <dev_LCD_HD44780.h>

void solo_recive()
{
    avr::UART_iostream uart;
    cfg_basica(uart);
    uart.on();

    avr::LCD_HD44780_1602_ostream lcd{ avr::LCD_HD44780::DPin_RS{4}
		    , avr::LCD_HD44780::DPin_RW{5}
		    , avr::LCD_HD44780::DPin_E {6}
		    , avr::LCD_HD44780::DPin_D4{11, 12, 13, 14}};

    lcd.clear();

    lcd << "Esperando\n";
    while (1) {
	char c = 0;
	if (uart >> c){
	    lcd << c;
	}
	else
	    uart.clear();
    }

}

int main()
{
    solo_recive();
}

