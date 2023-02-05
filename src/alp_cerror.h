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
#ifndef __ALP_CERROR_H__
#define __ALP_CERROR_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Excepciones y funciones para manejar los errores de 
 *	    las funciones de C (errno...)
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 05/01/2019 Escrito
 *
 ****************************************************************************/
#include "alp_exception.h"

namespace alp{


/// Convierte el errno en una cadena de texto con el mensaje de error
// Antes la llamaba errno2msg (borrar comentario. Puesto el 16/02/19)
std::string errno2str();


/***************************************************************************
 *			ERRORES DE RECURSOS
 ***************************************************************************/
// cuando no se puede establecer una conexión
class Cant_open_conexion:public Excepcion
{   
public:
    Cant_open_conexion(const std::string& nom_conexion)
    {
	msg = "-------------------------------------------------------------\n";
	msg+= "Error al intentar abrir la conexión'" + nom_conexion + "'\n";
	msg+= "-------------------------------------------------------------\n";
	msg+= "Detalles:\n";
	msg+= errno2str();
    }
};


}// namespace

#endif


