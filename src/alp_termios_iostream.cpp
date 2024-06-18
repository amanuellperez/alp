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

#include "alp_termios_iostream.h"

namespace alp{

std::streamsize FILE_streambuf_unbuffered::xsgetn(char_type* s, std::streamsize N)
{
    int ret = ::read(fd_, s, N);

    if (ret == 0){
	return 0;
    }

    else if (ret == -1){
	return 0;
    }

    return ret;
}

std::streamsize FILE_streambuf_unbuffered::xsputn(const char_type* s,
                                                  std::streamsize n)
{ // DUDA: ¿Qué pasa si ret != n? pongo failbit??? A fin de cuentas, 
  // failbit indica que la operación solicitada ha fallado.
    int ret = ::write(fd_, s, n);
    if (ret == -1){
	return 0;
    }

    return ret;
}



/// Abre el flujo con la configuración indicada. 
/// setstate(failbit) en caso de fallo.
void Termios_iostream::open(const std::string& usb_port, const Termios_cfg& cfg)
{

    int fd = sb_.open(usb_port.c_str(), O_RDWR);
    if(fd == -1){
	setstate(ios_base::badbit);
	return;
    }

    cfg.flush_data_not_read(fd);
    if (cfg.apply_cfg_now(fd) == -1){
	setstate(ios_base::badbit);
	return;
    }

}

}// namespace
