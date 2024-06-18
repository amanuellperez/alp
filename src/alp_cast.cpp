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

#include "alp_cast.h"
#include "alp_string.h"

namespace alp{

Fail_cast::Fail_cast(const std::string& funcion, const std::string& detalles)
{
    msg = linea();
    msg+= "\nError al hacer el cast:\nFunción: ";
    msg+= funcion;
    msg+= "\nDetalle: ";
    msg+= detalles;
    msg+="\n";
    msg+= linea();
}
}// namespace


