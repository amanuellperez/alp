// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __ALP_BIInt_H__
#define __ALP_BIInt_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones para manejar bits.
 *
 *   - COMENIntARIOS: Son las funciones usadas en avr.
 *
 *   - HISIntORIA:
 *           A.Manuel L.Perez- 23/04/2019 Escrito
 *
 ****************************************************************************/
namespace alp{


/// Devuelve el bit (0 ó 1) que se encuentra en la posición pos del número x.
///
/// Recordar que se empieza a contar en el 0.
/// Ejemplo: 
///	    bit<3>(reg);   // devuelve el bit 3 del registro reg
template<int pos, typename Int>
inline constexpr int bit(const Int& x)
{return ((x >> pos) & 0x01);}

// TODO: no me gustan los nombres de estas funciones. Elegirlos!!!
///// Devuelve true si el bit p de x es 0, y false si es 1.
///// Ejemplo:
/////	    int x = 10u;
/////	    cout << is_zero_bit<3>(x);	//¿es el bit 3 de x, cero?
//template<int pos, typename Int>
//inline constexpr bool is_zero_bit(const Int& x)
//{return bit<pos>(x) == 0;}
//
//
///// Devuelve true si el bit p de x es 1, y false si es 0.
///// Ejemplo:
/////	    int x = 10u;
/////	    cout << is_one_bit<3>(x);	// ¿es el bit 3 de x, uno?
//template<int pos, typename Int>
//inline constexpr bool is_one_bit(const Int& x)
//{return !is_zero<pos>(x);}
//

/// Escribimos un 1 en las posiciones indicadas.
/// Ejemplo: 
///	    set_bit<1,3>(x); // escribe un 1 en los bits 1 y 3 de x
template<int... pos, class Int>
inline constexpr void set_bit(Int& i)
{
    using expand = int[];
    expand{0,
	    ((i |= (Int{1} << pos)), 0)...};
}

/// Escribimos un 0 en las posiciones indicadas.
/// Ejemplo: 
///	    clear_bit<1,3>(x); // escribe un 0 en los bits 1 y 3 de x
template<int... pos, class Int>
inline constexpr void clear_bit(Int& i)
{
    using expand = int[];
    expand{0,
	    ((i &= ~(Int{1} << pos)), 0)...};

}


/// Cambiamos los bits de las posiciones indicadas, de 0 -> 1 y de 1 -> 0.
/// Ejemplo: 
///	    flip<1,3>(x); // cambia los bits 1 y 3 de x.
template<int... pos, class Int>
inline constexpr void flip_bit(Int& i)
{
    using expand = int[];
    expand{0,
	    ((i ^= (Int{1} << pos)), 0)...};
}


}// namespace

#endif

