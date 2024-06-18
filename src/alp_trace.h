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

#ifndef __ALP_TRACE_H__
#define __ALP_TRACE_H__

/****************************************************************************
 *
 *   - DESCRIPCION: Funciones para trazar los programas
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 08/12/2017 Reestructurado.
 *		    16/02/2019 Añado manipuladores.
 *
 ****************************************************************************/
#include "alp_exception.h"

#include <iostream>
#include <fstream>


namespace alp{
/*****************************************************************************
 * 
 *   - CLASE: Trace
 *
 *   - DESCRIPCIÓN: Clase para manejar trazas. 
 *
 *	    Es un flujo de caracteres unbuffered.
 *
 *	    Solo traza las trazas cuyo nivel actual sea menor o igual al
 *	    nivel máximo de traza.
 *
 *	    Si está definida NDEBUG la clase desaparece. (DUDA: ¿sería mejor
 *	    dejarla y que trace? Si trabajo con imágenes busco eficiencia, 
 *	    pero los errores es importante detectarlos)
 *
 *   - EJEMPLO:
 *	    alp::trace.nivel_max(6);  
 *
 *	    // esta no la trazaría ya que el nivel de trazas es 6
 *	    alp::trace(9) << "Nivel 9: " << 5 << '\n';
 *	    alp::trace(5) << "Nivel 5: " << 5 << '\n';
 *
 *   - NIVELES DE TRAZA:
 *	    1-3 : errores graves.
 *	    4-6 : errores de programación.
 *	    7-9 : trazas que se pueden eliminar.
 *
 *   - TODO:
 *	    + Que se pueda elegir trazar en fichero o en cout.
 *	    
 ***************************************************************************/
class Trace{
public:
    template<typename T>
    Trace& operator<<(const T& t)
    {
#ifndef NDEBUG
	if(nivel_ <= nivel_max_){
	    std::cout << t << std::flush;
	}

#endif
	return *this;
    }


    /// Manipuladores: std::endl, ...
    Trace& operator<<
	(std::ostream& (*pf)(std::ostream&))
    {
	std::cout << (*pf);

	return *this;
    }


    Trace& operator()(int n) noexcept
    {
#ifndef NDEBUG
	nivel_ = n;
#endif 
	return *this;
    }


    void nivel_max(int n) noexcept
    {
#ifndef NDEBUG
	nivel_max_ = n;
#endif 
    }


private:
#ifndef NDEBUG
    int nivel_ = 9; // nivel en el que estoy trazando

    int nivel_max_ = 9;
#endif 
};




/*****************************************************************************
 * 
 *   - CLASE: Traza
 *
 *   - DESCRIPCIÓN: Clase para trazar funciones
 *
 ***************************************************************************/
class Traza{
public:
    Traza(const std::string& signature)
    {std::cout << ">>> " << signature << std::endl;}

    ~Traza(){std::cout << "<<<";}
};


extern Trace trace;





/*!
 *  \brief  Fichero de log
 *
 *  De momento es un flujo vulgar y corriente. En futuro se puede añadir
 *  la fecha y hora delante de cada operación de escritura...
 *
 *  En principio es una clase para depurar
 */
class Log{
public:
    /// Abre un fichero de trazas.
    Log(const std::string& nom_fichero):out{nom_fichero} 
    { 
	if (!out)
	    throw File_cant_write(nom_fichero);
    } 

    template <typename T>
    Log& operator<< (const T& x) {out << x; return *this;}

private:
    std::ofstream out;
};



}// namespace alp

#endif


