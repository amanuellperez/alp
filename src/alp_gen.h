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

#ifndef __ALP_GEN_H__
#define __ALP_GEN_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones genéricas.
 *
 *   - COMENTARIOS: Este tipo de funciones sirven para cualquier programa.
 *
 *   - HISTORIA:
 *     alp
 *       14/11/2015 Escrito
 *       01/08/2020 Limpiando. Este fichero está obsoleto. Si no se usa,
 *		    eliminarlo en unos meses.
 *
 *
 ****************************************************************************/
//#include "alp_concepts.h"
//#include "alp_math.h"
//#include "alp_exception.h"
//#include "alp_string.h"
//#include "alp_type_traits.h"
//
//#include <fstream>
//#include <thread>
//
//namespace alp{
//
//
//
///*****************************************************************************
// * 
// *   - CLASE: flag
// *
// *   - DESCRIPCIÓN: Clase auxiliar para poder escribir código del tipo:
// *	    bool red = true;
// *	    ...
// *	    cout << flag(red, 'R'); // imprime R si está activo o - si no lo
// *	    está
// *
// ***************************************************************************/
//class flag{
//public:
//    flag(bool f, char c):on_{f}, c_{c}{}
//
//    friend std::ostream& operator<<(std::ostream& out, flag f);
//
//private:
//    bool on_; // on/off
//    char c_; // caracter a mostrar si está activo el flag
//};
//
//
//inline  std::ostream& operator<<(std::ostream& out, flag f)
//{
//    if(f.on_) out << f.c_;
//    else out << '-';
//    return out;
//}
//
//
//
//
///****************************************************************************
// *
// *   - FUNCIÓN: existe(Fichero)
// *
// *   - DESCRIPCIÓN: Devuelve true si existe el fichero pasado como parámetro,
// *	    false en caso contrario.
// *	    
// *   - EJEMPLO:
// *	    if(!existe(Fichero{"03.jpg"})) ...
// *
// ****************************************************************************/
////class Fichero:public Nombre<std::string>{ public: using Nombre::Nombre; };
//using Fichero = std::string;
//
//inline bool existe(Fichero f)
//{
//    std::ifstream in{f};
//    if(in) return true;
//    else return false;
//}
//
//
//


//}// namespace alp


#endif
