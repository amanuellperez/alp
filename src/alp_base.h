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

#ifndef __ALP_BASE_H__
#define __ALP_BASE_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones básicas del paquete alp.
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 18/03/2017 Escrito
 *
 ****************************************************************************/
#include <sstream>  // stringstream


namespace alp{
/*!
 *  \brief  Generador de nombres
 *
 *   - DESCRIPCIÓN: En ocasiones, interesa escribir código del tipo:
 *		escala(img, Num_filas{400});
 *		escala(img, Num_columnas{400});
 *
 *	Usando sobrecarga de funciones, queda muy clara la llamada a estas
 *	funciones.
 *
 *	En otras ocasiones, por claridad, escribo:
 *		if(!existe(Fichero("foto.jpg"))) ...
 *	que se lee perfectamente.
 *
 *	¿Cómo crear todas estas clases Num_filas, Num_columnas, Fichero...?
 *	Para ello uso esta clase Nombre:
 *		class Num_filas:public Nombre<int>{using Nombre::Nombre;}
 *		class Fichero:public Nombre<string>{using Nombre::Nombre;}
 */
template<typename T>
class Nombre{
public:
    explicit Nombre(T v):v_{v}{}
    const T& operator()() const{return v_;}

private:
    T v_;
};





//// imprimimos un contenedor separado por comas (¿la coma se puede pasar
//// como parámetro a la plantilla y así elegir el tipo de impresión?)
//template<typename It>
//inline std::ostream& print_contenedor(std::ostream& out, It begin, It end
//		, const std::string separador = ", ") // separador entre cada elemento 
//{
//    std::string coma = "";
//    for(auto x = begin; x != end; ++x) 
//    {
//	out << coma << *x;
//	coma = separador;
//    }
//
//    return out;
//}






}//namespace alp


#endif 

