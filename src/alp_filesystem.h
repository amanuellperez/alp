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

#ifndef __ALP_FILESYSTEM_H__
#define __ALP_FILESYSTEM_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones para manejar ficheros
 *
 *   - HISTORIA:
 *           Manuel Perez- 17/04/2019 Escrito
 *
 ****************************************************************************/

#include <vector>
#include <string>

namespace alp{

// Lee un fichero. Lo devuelve como std::vector<char>
// Precondición: std::filesystem::is_regular_file(name)
std::vector<char> read_file(const std::string& name);


}// namespace


#endif


