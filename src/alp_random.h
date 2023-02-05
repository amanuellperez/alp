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

#ifndef __ALP_RANDOM_H__
#define __ALP_RANDOM_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones para generar cosas aleatorias
 *
 *   - HISTORIA:
 *           Manuel Perez- 07/04/2019 Escrito
 *
 ****************************************************************************/
#include <random>

namespace alp{
/// Genera un número aleatorio entre [min, max]. 
/// Distribución uniforme.
template <typename Int>
class Random_number{
public:
    Random_number(Int min, Int max)
	: gen{dev()}, num{min, max}
    {
    }
    
    Int operator()() {return num(gen);}

private:
    std::random_device dev;
    std::mt19937 gen;
    std::uniform_int_distribution<Int> num;
};


}// namespace

#endif

