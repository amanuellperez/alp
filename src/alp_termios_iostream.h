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

#ifndef __ALP_TERMIOS_IOSTREAM_H__
#define __ALP_TERMIOS_IOSTREAM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Flujos para termios
 *
 *  - TODO: 
 *	- Implementar un buffer para leer. El tutorial de termios dice que no
 *	es nada eficiente llamar a read(2) para que lea 1 byte.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	25/11/2019 v0.0 Probando.
 *	05/01/2024 Mejorando cosas.
 *
 ****************************************************************************/
#include "alp_termios_cfg.h"

#include <streambuf>
#include <iostream>

namespace alp{
/*!
 *  \brief  Streambufer que encapsula los flujos de C.
 *
 *  termios ya tiene implementado un buffer, así que no es necesario
 *  implementar otro aquí.
 *
 */
class FILE_streambuf_unbuffered : public std::streambuf{
public:
// Types
// -----
    using char_type = std::streambuf::char_type;
    using traits_type = std::streambuf::traits_type;
    using int_type = std::streambuf::int_type;
    using pos_type = std::streambuf::pos_type;
    using off_type = std::streambuf::off_type;

    FILE_streambuf_unbuffered(int fd = 0) : fd_{fd} 
    { 
	setg(&get_area_, &get_area_ + 1, &get_area_ + 1);
    }

    ~FILE_streambuf_unbuffered() { close();}


    /// Abre el flujo. Devuelve el file descriptor asociado con el flujo.
    /// En caso de error devuelve -1.
    int open(const std::string& path_name, int flags)
    {
	fd_ = ::open(path_name.c_str(), flags);

	return fd_;
    }

    // Convenio: usaré fd == 0 para indicar que está cerrado (fd == 0
    // corresponde al teclado, así que no habrá ningún puerto usb con ese file
    // descriptor)
    bool is_open() const { return fd_ != 0; }

    /// Cierra el flujo asociado.
    void close() 
    { 
	if (is_open())
	    ::close(fd_); 
    }

    /// Devuelve el flujo asociado.
    int fd() const {return fd_;}
    
protected:
    // Buffer management
    // -----------------
    // Synchronizes the controlled sequences with the arrays (vamos, es
    // flush()). That is, if pbase() is non-null the characters [pbase(),
    // pptr()) are writen to the controlled sequence. The pointers may be
    // reset as appropiate.
    //
    // Returns: -1 on failure (cada clase derivada determinará qué considera
    // fallo).
    // Es unbuffered así que no tengo que sincronizar nada.
    virtual int sync() override {return 0;}
    
    // Get area
    // --------
    // Obtains the number of characters available for input in the associated
    // input sequence if known.
    virtual std::streamsize showmanyc() override {return 0;}


    // Reads max N characters from the input sequence and stores them into s.
    // The characters are read as if by repeated call of sbumpc.
    // Returns: the number of characters assigned to s.
    virtual std::streamsize xsgetn(char_type* s, std::streamsize N) override;


    // Ensures that at least one character is avaible in the input area by
    // updating the pointers to the input area and reading more data in from
    // the input sequence. Returns the value of that character on success or
    // traits_type::eof() on failure.
    // (Esta función no consume el caracter)
    virtual int_type underflow() override 
    { 
	int_type c  = read_byte();

	if (traits_type::eq(c, traits_type::eof()))
		return traits_type::eof();

	get_area_ = traits_type::char_type(c);
	setg(&get_area_, &get_area_, &get_area_ + 1);

	return c;
    }

    // Putback
    // -------
    // Puts a character back into the input sequence, posibly modifying the 
    // input sequence.
    virtual int_type pbackfail(int_type c = traits_type::eof()) override
    {return traits_type::eof();}


    // Put area
    // --------
    // Ensures that there is space at the put area for at least one character
    // by saving some initial subsequence of characters starting at pbase() to
    // the output sequence and updating the poinnters to the put area (if
    // needed). If c is not eof() it is either put to the put area or directly
    // saved to the output sequence.
    //
    // Returns: traits::eof() if the function fails, or some other unspecified 
    // value to indicate success (typically returns c to indicate success, 
    // salvo cuando c == traits::eof() en cuyo caso devuelve traits::not_eof(c))
    virtual int_type overflow(int_type c = traits_type::eof()) override
    { return write_byte(c); }

    // Writes up to n characters to the output sequence as if by repeated
    // calls to sputc(c).
    // Returns: the number of characters written.
    virtual std::streamsize xsputn(const char_type* s, std::streamsize n) override;

private:
    // Flujo al que conectamos
    int fd_;


    // CUIDADO: `write_byte` como su nombre indica escribe 1 byte.
    // Pasarle a `::write` 1 explícitamente (al principio le pasaba sizeof(c)
    // que era 4, generando un error)
    int_type write_byte(int_type c) 
    { // DUDA: ¿qué pasa si devuelve 0?
	return ::write(fd_, &c, 1); 
    }


    int_type read_byte()
    {
	char c;
	int ret = ::read(fd_, &c, sizeof(c));
    
	if (ret == 0)
	    return traits_type::eof();

	else if (ret == -1)
	    return traits_type::eof();

	return traits_type::to_int_type(c);
    }

    // Le pongo un buffer de un caracter.
    // El flujo streambuf está pensado para tener un buffer. Sin el buffer no
    // funciona. De hecho me he tirado 2 horas depurando por intentar
    // implementarlo sin buffer.
    char_type get_area_;	
    
};



/*!
 *  \brief  Flujo para escribir en puertos serie (basado en termio)
 *
 */
class Termios_iostream : public std::iostream {
public:
    Termios_iostream() : std::iostream{&sb_} { }

    Termios_iostream(const std::string& usb_port, const Termios_cfg& cfg)
        : Termios_iostream{}
    { 
	open(usb_port, cfg);
    }

    /// Abre el flujo con la configuración indicada. 
    /// setstate(failbit) en caso de fallo.
    void open(const std::string& usb_port, const Termios_cfg& cfg);

    // Convenio: usaré fd == 0 para indicar que está cerrado (fd == 0
    // corresponde al teclado, así que no habrá ningún puerto usb con ese file
    // descriptor)
    bool is_open() const { return sb_.is_open(); }

    /// Cierra el flujo asociado.
    void close() { sb_.close(); }

    /// Devuelve el flujo asociado.
    int fd() const {return sb_.fd();}
    
private:
    FILE_streambuf_unbuffered sb_;
};



// Lee 1 caracter sin bloquear (si termios está configurado para no bloquear)
inline ssize_t read(Termios_iostream& in, char& c)
{ return ::read(in.fd(), &c, 1); }

// Por comodidad defino 
inline ssize_t cin_read(char& c)
{ return ::read(STDIN_FILENO, &c, 1); }

} // namespace

#endif


