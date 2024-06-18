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

#include "alp_exception.h"

#include "alp_string.h"

#include <string.h>

namespace alp{

Error_de_programacion::Error_de_programacion(const char* file, int line
	    , const std::string& nom_funcion, const std::string& detalles)
{
    msg = "-------------------------------------------------------------\n";
    msg+= "Error para depurar:\nFichero: ";
    msg+= file;
    msg+= "[";
    msg+= as_str() << line << "]\n";
    msg+= "Función: ";
    msg+= nom_funcion;
    msg+= "\nDetalle: ";
    msg+= detalles;
    msg+= "\n";
    msg+= "-------------------------------------------------------------\n";
}


File_not_found::File_not_found(const std::string& nom_fichero)
{
    msg = "-------------------------------------------------------------\n";
    msg+= "Error al abrir el fichero '" + nom_fichero + "'\n";
    msg+= "No se ha encontrado dicho fichero\n";
    msg+= "-------------------------------------------------------------\n";
}


File_cant_read::File_cant_read(const std::string& nom_fichero)
{
    msg = "-------------------------------------------------------------\n";
    msg+= "Error al intentar leer el fichero '" + nom_fichero + "'\n";
    msg+= "-------------------------------------------------------------\n";
}


File_cant_write::File_cant_write(const std::string& nom_fichero)
{
    msg = "-------------------------------------------------------------\n";
    msg+= "Error al intentar escribir en el fichero '" + nom_fichero + "'\n";
    msg+= "-------------------------------------------------------------\n";
}

Imposible_llegar_aqui::Imposible_llegar_aqui(const char* file, int line
					    , const std::string& funcion)
{
    msg = "-------------------------------------------------------------\n";
    msg+= "Error grave de programación.\n" ;
    msg+= "Fichero: ";
    msg+= file;
    msg+= "[" + to<std::string>(line) + "]\n";
    msg+= "Función: " + funcion;
    msg+= "\nSe está llegando a un sitio donde nunca se debería de llegar.\n";
    msg+= "-------------------------------------------------------------\n";
}

Falta_implementar::Falta_implementar(const char* file,
                                     int line,
                                     const std::string& funcion,
                                     const std::string& descripcion)
{
    msg = "-------------------------------------------------------------\n";
    msg+= "Falta implementar: " + descripcion;
    msg+= file;
    msg+= as_str() << "[" << line << "]\n";
    msg+= "Función: " + funcion + '\n';
    msg+= "-------------------------------------------------------------\n";
}


Perror::Perror(const std::string& nom_funcion) 
	: Excepcion(as_str() << nom_funcion << ": " << ::strerror(errno)) {}

}// namespace
