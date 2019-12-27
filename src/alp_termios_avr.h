// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#pragma once

#ifndef __ALP_TERMIOS_AVR_H__
#define __ALP_TERMIOS_AVR_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Configuración por defecto del avr para comunicarse con
 *  termios.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel Lopez
 *	26/11/2019 v0.0
 *
 ****************************************************************************/
#include "alp_termios_cfg.h"

namespace alp{

/// Configura la conexión fd para conectar con la configuración básica por
/// defecto del avr. 
int cfg_avr_uart(int fd, int MIN, int TIME) noexcept;

/// Rellena cfg con la configuración básica que uso para conectarme a UART.
// Para significado de MIN, TIME hacer man termios y buscar MIN == 0
void cfg_avr_uart(Termios_cfg& cfg, int MIN, int TIME) noexcept;

/// Rellena cfg con la configuración básica que uso para conectarme a UART.
/// Modo blocking read: 
/// read(2)  blocks until 1 byte is available, and returns up to
/// the number of bytes requested.
inline void cfg_avr_uart_blocking_read(Termios_cfg& cfg) noexcept
{ cfg_avr_uart(cfg, 1, 0); } 

/// Rellena cfg con la configuración básica que uso para conectarme a UART.
/// Modo polling read: 
/// If  data	is  available,	read(2)  returns immediately, with the
/// lesser of the number of bytes available, or the number of  bytes
/// requested.  If no data is available, read(2) returns 0.
inline void cfg_avr_uart_polling_read(Termios_cfg& cfg) noexcept
{ cfg_avr_uart(cfg, 0, 0); } 

/// Rellena cfg con la configuración básica que uso para conectarme a UART.
/// Modo read with timeout:
/// 'time' specifies the limit for a timer in tenths of a second.  The
/// timer is started when read(2) is called.	read(2) returns either
/// when at least one byte of data is available, or when  the  timer
/// expires.	If the timer expires without any input becoming avail‐
/// able, read(2) returns 0.	If data is already  available  at  the
/// time of the call to read(2), the call behaves as though the data
/// was received immediately after the call.
inline void cfg_avr_uart_read_with_timeout(Termios_cfg& cfg, int time) noexcept
{ cfg_avr_uart(cfg, 0, time); } 


} // namespace

#endif



