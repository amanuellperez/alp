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

#include "alp_utf8.h"

#include "alp_string.h"
#include "alp_cast.h"

namespace alp{

bool isalnum(const utf8_char_t& c)
{
    if (c[1] != '\0')
	return isalpha(c);

    return std::isalnum(c[0]);
}

// Todas las letras acentuadas tienen códigos C3xx
// (ver https://www.utf8-chartable.de/unicode-utf8-table.pl
// TODO: buscar una referencia en unicode!!!)
bool isalpha(const utf8_char_t& c)
{
    if (c[1] == '\0')
	return std::isalpha(c[0]);

    else if (c[2] == '\0')
	return (c[0] & 0xC3) == 0xC3;
    
    else
        throw std::runtime_error{"isalpha(utf8_char_t) no está implementada "
                                 "para UTF8 mayores de 2 bytes"};

    // TODO: esto solo sirve para Latín
    return false;
}

int utf8_num_bytes(char c)
{
    if (is_ascii(c))
	return 1;

    if (is_first_byte_utf8_2bytes(c))
	return 2;

    if (is_first_byte_utf8_3bytes(c))
	return 3;

    if (is_first_byte_utf8_4bytes(c))
	return 4;

    throw std::runtime_error{alp::as_str()
                             << "utf8_num_bytes: El caracter 0x" << std::hex
                             << static_cast<int>(c)
                             << " no es primer caracter de un code point UTF8"};
}


// buscamos el primer byte del siguiente utf8_char_t
// \return el siguiente caracter
// \return eof: si llegamos al final del istream
// Se trata de una función de implementación.
char utf8_get_next_utf8_char_t(std::istream& in)
{
    char c;
    while (in.get(c)){
	if (!is_continuing_byte(c))
	    return c;

        std::cerr << "utf8_next_utf8_char_t: Absorviendo caracter [" << std::hex
                  << char2int(c) << "]\n";
    }

    return std::char_traits<char>::eof();
}


std::istream& utf8_get(std::istream& in, utf8_char_t& uc)
{
    uc = utf8_char_t{};
    char c;

    c = utf8_get_next_utf8_char_t(in);

    if(c != std::char_traits<char>::eof()){
	uc[0] = c;

	switch (utf8_num_bytes(uc[0])){
	    case 1: break;

	    case 2:
		// DUDA: validar que c sea continuing byte???
		in.get(uc[1]);
		break;


	    case 3:
		in.get(uc[1]);
		in.get(uc[2]);
		break;

	    case 4:
		in.get(uc[1]);
		in.get(uc[2]);
		in.get(uc[3]);
		break;
        }

    }

    return in;
}




void utf8_print_as_hex(std::ostream& out, const utf8_char_t& uc)
{
    size_t width = static_cast<size_t>(out.width(0));

    size_t i = 0;
    for (; i < uc.size(); ++i)
	out << char2int(uc[i]);

    // cada byte en hexadecimal son 2 caracteres en pantalla,
    // de ahí el *2
    i *= 2; // i = núm. de caracteres impresos en pantalla

    for (; i < width; ++i)
	out << out.fill();
}


std::ostream& operator<<(std::ostream& out, const utf8_char_t& uc)
{ 
    if (out.flags() & std::ios_base::hex)
	utf8_print_as_hex(out, uc);

    else
	out << uc.data_; 

    return out;

}



// Trocea la cadena 's' en palabras (delimitadas por whitespace)
std::vector<utf8_string> split_words(const utf8_string& s)
{
    std::vector<utf8_string> res;

    if (s.empty())
	return res;

    auto p = s.begin();
    auto pe = s.end();

    if (utf8::isspace(*p))
	p = utf8::skip_whitespace(p, pe);

    while (p != pe){
	// [[ expects: !utf8::isspace(*p) ]]
	auto pwe = utf8::find_whitespace(p, pe);
	res.push_back(utf8_string{p, pwe});
	p = utf8::skip_whitespace(pwe, pe);
    }

    return res;
}

char to_iso88951(const utf8_char_t& c)
{
    if (is_ascii(c))
	return c[0];

    if (!has_2bytes(c))
	return 0;

// Tiene 2 bytes:
    if (c[0] == (char) 0xC2 and ((char) 0xA0 <= c[1] and c[1] <= (char) 0xBF))
	return c[1];

    if (c[0] == (char) 0xC3 and ((char) 0x80 <= c[1] and c[1] <= (char) 0xBF))
	return c[1] + 0xC0 - 0x80;

    return 0;
}


}// namespace

