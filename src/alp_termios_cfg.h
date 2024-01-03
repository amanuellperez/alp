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

#pragma once
#ifndef __ALP_TERMIOS_CFG_H__
#define __ALP_TERMIOS_CFG_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Traductor de la API de termios, para que todo sea más
 *	legible. La idea es darle nombres a todas las funciones de bajo nivel.
 *
 *   - COMENTARIOS: 
 *	    Estoy aprendiendo a manejar termios, así que esta es una versión
 *	totalmente experimental.
 *
 *	    Parece ser que el termios tiene dos modos diferentes de trabajo:
 *	    1.- El canónico, que admite preprocesamiento de la entrada.
 *	    2.- El no canónico, que no lo admite.
 *
 *	Cada uno de estos modos tiene configuraciones distintas, con lo que se
 *	están mezclando responsabilidades en termios. 
 *
 *	See: man termios.
 *
 *
 *   - HISTORIA:
 *	Manuel Perez
 *	    05/01/2019 Escrito
 *	    25/11/2019 Reescrito como cfg.
 *		
 *
 ****************************************************************************/
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <string>

namespace alp{
/*!
 *  \brief  Traductor de termios
 *
 *  Contiene toda la funcionalidad de termios. Mejor no usar esta clase
 *  directamente ya que tiene mezclados diferentes formas de trabajo (canónico
 *  y no canonico). Usar mejor las clases particulares Termios_cfg_noncanonical
 *
 *  Configuramos el UART
 *  --------------------
 *  ¿Cómo podemos configurar el UART del AVR? (punto 24.5)
 *  + 1 start bit (obligatorio)
 *  + baud_rate
 *  + character_size_bits: 5, 6, 7, 8 ó 9 data bits
 *  + parity_mode_bits: no, even or odd parity
 *  + number_stop_bits: 1 or 2 stop bits
 *
 */
class Termios_cfg{
public:
    // Idioma:
    //	    abrir primero el flujo llamando a open.
    //	    despues configurarlo. (de hecho no se puede configurar si no se
    //	    tiene el fd)
    Termios_cfg() noexcept;

    /// Define el baud_rate a bauds, tanto de entrada como de salida.
    template <int bauds>
    void baud_rate() noexcept;

    /// Define el baud_rate de entrada a bauds.
    template <int bauds>
    void input_baud_rate() noexcept;

    /// Define el baud_rate de salida a bauds.
    template <int bauds>
    void output_baud_rate() noexcept;

    // número de data bits (5, 6, 7 ó 8)
    void character_size_bits_5()  noexcept {cfg_.c_cflag |= CS5;}
    void character_size_bits_6()  noexcept {cfg_.c_cflag |= CS6;}
    void character_size_bits_7()  noexcept {cfg_.c_cflag |= CS7;}
    void character_size_bits_8()  noexcept {cfg_.c_cflag |= CS8;}

    // número de stops bits (1 ó 2)
    void number_stop_bits_2() noexcept	{cfg_.c_cflag |=CSTOPB;}

    void enable_receiver() noexcept	{cfg_.c_cflag |= CREAD;}

    // El man de termios dice que si se activa PARENB, pero no PARODD se
    // activa la generación de los parity check tanto en entrada como en
    // salida, pero por defecto es par.
    void parity_mode_bits_even() noexcept {cfg_.c_cflag |= PARENB;}
    void parity_mode_bits_odd () noexcept {cfg_.c_cflag |= (PARENB|PARODD);}


    // DUDA: ¿esto qué hace????
    void ignore_modem_control_lines() noexcept {cfg_.c_cflag |= CLOCAL;}

    // Configuramos local modes
    // ------------------------
    /// Enable canonical mode.
    /// Input is made available line by line.	An  input  line  is  available
    /// when  one  of	the line delimiters is typed (NL, EOL, EOL2; or EOF at
    /// the start of line).  Except in the case of EOF, the line delimiter is
    /// included in the buffer returned by read(2).
    void canonical_mode() noexcept { cfg_.c_lflag |= ICANON; }

    /// If  data	is  available,	read(2)  returns immediately, with the
    /// lesser of the number of bytes available, or the number of  bytes
    /// requested.  If no data is available, read(2) returns 0.
    void noncanonical_polling_read() noexcept
    { noncanonical_mode(0, 0); }

    /// read(2)  blocks until MIN bytes are available, and returns up to
    /// the number of bytes requested.
    void noncanonical_blocking_read(int num_bytes) noexcept
    { noncanonical_mode(num_bytes, 0); }


    /// TIME specifies the limit for a timer in tenths of a second.  The
    /// timer is started when read(2) is called.	read(2) returns either
    /// when at least one byte of data is available, or when  the  timer
    /// expires.	If the timer expires without any input becoming avail‐
    /// able, read(2) returns 0.	If data is already  available  at  the
    /// time of the call to read(2), the call behaves as though the data
    /// was received immediately after the call.
    /// El tutorial de termios desaconseja usar este modo, mejor el polling
    /// read.
    void
    noncanoncial_mode_read_with_timeout(int time_in_tenths_of_a_second) noexcept
    { noncanonical_mode(0, time_in_tenths_of_a_second); }


    // 'time' specifies the limit for a timer  in	tenths	of  a  second.
    // Once  an	initial  byte of input becomes available, the timer is
    // restarted after each further byte is received.  read(2)  returns
    // when any of the following conditions is met:
    // *  'num_bytes' bytes have been received.
    // *  The interbyte timer expires.
    // *  The  number  of bytes requested by read(2) has been received.
    // (POSIX does not specify this termination  condition,  and  on
    // some  other  implementations  read(2) does not return in this
    // case.)
    void noncanonical_read_with_interbyte_timeout(int num_bytes,
                                                  int time) noexcept
    { noncanonical_mode(num_bytes, time); }


    // Configuramos la entrada (lo que leemos)
    // ---------------------------------------
    void input_ignore_frame_and_parity_errors() noexcept
    { cfg_.c_iflag |= IGNPAR; }

    /// Echo input characters.
    void echo() noexcept { cfg_.c_lflag |= ECHO; }

    /// No echo input characters.
    void no_echo() noexcept { cfg_.c_lflag &= ~ECHO; }


    // Configuramos la salida (lo que escribimos)
    // ------------------------------------------
    void output_turn_off_output_processing() noexcept {cfg_.c_oflag = 0;}


    // Aplicamos la configuración
    // --------------------------
    /// Aplicamos la configuración existente ahora.
    /// Returns: 0 on success if any of the requested changes could be
    /// successfully carried out.
    /// Otherwise: -1, on failure (errno tiene el error).
    int apply_cfg_now(int fd) const noexcept
    { return tcsetattr(fd, TCSANOW, &cfg_); }

    /// The change occurs after all output written to fd has been trans‐
    /// mitted.  This option should be  used  when  changing  parameters
    /// that affect output.
    //
    /// Returns: 0 on success if any of the requested changes could be
    /// successfully carried out.
    //
    /// Otherwise: -1, on failure (errno tiene el error).
    int apply_cfg_after_output_flush(int fd) const noexcept
    { return tcsetattr(fd, TCSADRAIN, &cfg_); }


    /// The  change  occurs  after  all  output  written to  the object
    /// referred by fd has been transmitted, and all input that has been
    /// received but  not  read	will be discarded before the change is
    /// made.
    int apply_cfg_after_flush(int fd) const noexcept
    { return tcsetattr(fd, TCSAFLUSH, &cfg_); }

    /// Copiamos la configuración del flujo fd en este objeto.
    void copy_cfg_from(int fd) noexcept { tcgetattr(fd, &cfg_); }


    // Más funciones
    // -------------
    // Borramos todos los datos no leídos
    static int flush_data_not_read(int fd) noexcept {return tcflush(fd, TCIFLUSH);}

    // Borramos todos los datos no transmitidos 
    static int flush_data_not_transmitted(int fd) noexcept {return tcflush(fd, TCOFLUSH);}

    // Borramos todos los datos: tanto no leídos como no transmitidos
    static int flush_data(int fd) noexcept {return tcflush(fd, TCIOFLUSH);}

    // Esta es de bajo nivel, mejor no usarla
    /// In noncanonical mode input is available immediately (ver man)
    /// no input processing is performed and line editin is disabled.
    // Este es el modo a usar si se quiere leer tecla a tecla desde
    // std::cin!!!
    // Ver man termios para el significado de (min, time)
    void noncanonical_mode(int min, int time) noexcept;

    /// Imprime la configuración
    void print(std::ostream& out) const;

    void print_input_modes(std::ostream& out) const {print_iflag(out);}
    void print_output_modes(std::ostream& out) const {print_oflag(out);}
    void print_control_modes(std::ostream& out) const {print_cflag(out);}
    void print_local_modes(std::ostream& out) const {print_lflag(out);}
    void print_special_characters(std::ostream& out) const {print_ccflag(out);}

private:
// Data
    termios cfg_;   // configuración de la conexión

// Helpers
    template <int bauds>
    constexpr static int baud_rate_constant() noexcept;

    void print_iflag(std::ostream& out) const;
    void print_oflag(std::ostream& out) const;
    void print_cflag(std::ostream& out) const;
    void print_lflag(std::ostream& out) const;
    void print_ccflag(std::ostream& out) const;

};

inline void Termios_cfg::noncanonical_mode(int min, int time) noexcept
{
    cfg_.c_lflag &= ~ICANON;

    cfg_.c_cc[VMIN]  = min;
    cfg_.c_cc[VTIME] = time;
}


template <int bauds>
constexpr inline int Termios_cfg::baud_rate_constant() noexcept
{
    if	    constexpr (bauds == 0)	return B0;
    else if constexpr (bauds == 50)	return B50;
    else if constexpr (bauds == 75)	return B75;
    else if constexpr (bauds == 110)	return B110;
    else if constexpr (bauds == 134)	return B134;
    else if constexpr (bauds == 150)	return B150;
    else if constexpr (bauds == 200)	return B200;
    else if constexpr (bauds == 300)	return B300;
    else if constexpr (bauds == 600)	return B600;
    else if constexpr (bauds == 1200)	return B1200;
    else if constexpr (bauds == 1800)	return B1800;
    else if constexpr (bauds == 2400)	return B2400;
    else if constexpr (bauds == 4800)	return B4800;
    else if constexpr (bauds == 9600)	return B9600;
    else if constexpr (bauds == 19200)	return B19200;
    else if constexpr (bauds == 38400)	return B38400;
    else if constexpr (bauds == 57600)	return B57600;
    else if constexpr (bauds == 115200)	return B115200;
    else if constexpr (bauds == 230400)	return B230400;

}


template<int bauds>
inline void Termios_cfg::input_baud_rate()  noexcept
{
    cfsetispeed(&cfg_,baud_rate_constant<bauds>());   
}


template<int bauds>
inline void Termios_cfg::output_baud_rate()  noexcept
{
    cfsetospeed(&cfg_, baud_rate_constant<bauds>());
}

template<int bauds>
void Termios_cfg::baud_rate()  noexcept
{
    output_baud_rate<bauds>();
    input_baud_rate <bauds>();
}




}// namespace

#endif


