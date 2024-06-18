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

#ifndef __ALP_EXCEPTION_H__
#define __ALP_EXCEPTION_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Las excepciones que lanzo son todas hijas de la 
 *	clase Excepcion. Por ello, todas tendrán el método what() (igual que
 *	las std).
 *
 *   - COMENTARIOS: TODO: esto es una idea, irla probando
 *	    En principio clasifico las excepciones en:
 *	    1.- Error_de_programacion:
 *		Son errores de programación. Nunca se deberían de dar este
 *		tipo de excepciones (pre/postcondiciones, casos de switch
 *		que sabemos que nunca se van a alcanzar...)
 *		Estos errores deben de terminar siempre con un core de la
 *		aplicación, ya que son errores graves.
 *	    
 *	    2.- Error_de_diseño: 
 *		Le pedimos a una función que haga algo y no puede hacerlo
 *		(imposible abrir un fichero, o leerlo, o escribirlo;
 *		intentamos dividir entre cero,...). Esto puede ser debido a
 *		errores de programación o de que el usuario intente hacer algo
 *		indebido (dividir por cero, o abrir un fichero que no existe).
 *		Estas excepciones son debido a que el programa, seguramente,
 *		no este bien diseñado: no hay que permitirle al usuario que
 *		divida entre cero, ni que abra un fichero que no existe...
 *
 *	    3.- Error_de_recursos:
 *		Se dan cuando te quedas sin espacio en disco duro (o no 
 *		puedes leer o escribir en un fichero), o se cierra
 *		una conexión, ... El programa funciona bien, y el usuario
 *		tampoco mete la pata.
 *
 *	    4.- Error_de_uso:
 *		+ El usuario ejecuta un programa:
 *		    > ivi -d 
 *		    y la opción d necesita un dato obligatorio. Esto es
 *		    claramente un error del usuario.
 *		    (¿o de diseño? Si al abrir el programa pregunta por las
 *		    opciones el usuario está obligado a escribirlas
 *		    correctamente)
 *
 *		+ El usuario quiere abrir un fichero y no se encuentra dicho fichero.
 *		+ El usuario le pasa un fichero al programa que no tiene el formato 
 *		adecuado (se ha confundido el usuario, no es que el programa
 *		funcione mal).
 *		+ El usuario pasa el nombre de un fichero que no existe
 *		(file_not_found)
 *
 *   - HISTORIA:
 *           alp  - 08/04/2017 Escrito
 *		    08/02/2019 Heredo de std::exception
 *
 ****************************************************************************/
#include <string>
#include <exception>

namespace alp{
// Estos son errores que no son de programa, sino errores de uso. 
// Por ejemplo:
//	+ El usuario quiere abrir un fichero y no se encuentra dicho fichero.
//	+ El usuario le pasa un fichero al programa que no tiene el formato 
//	  adecuado (se ha confundido el usuario, no es que el programa
//	  funcione mal).
//	+ El usuario pasa el nombre de un fichero que no existe
//	(file_not_found)
class Excepcion : public std::exception{
public:
    Excepcion(const std::string& mensaje):msg{mensaje}{}

    const char* what() const noexcept override {return msg.c_str();}

protected:
    Excepcion(){}
    std::string msg;
};



// Estos son errores que nunca se deberían de dar salvo porque se esté usando
// mal una clase, una función...
// Por ejemplo:
//	+ No se satisfacen las precondiciones de una función.
//	+ El programa llega a un punto que se sabe nunca tendría que llegar.
class Error_de_programacion:public Excepcion{
public:
    // Excepción para depurar
    // Ejemplo: 
    //	throw Error_de_programacion{__FILE__, __LINE__, "funcion", "detalle del	error"};
    Error_de_programacion(const char* file, int line
	    , const std::string& nom_funcion, const std::string& detalles);

protected:
    Error_de_programacion(){}
};



/***************************************************************************
 *			    ERRORES DEL USUARIO
 ***************************************************************************/
// cuando no se encuentra algo que se da por supuesto que tiene que estar
class File_not_found:public Excepcion
{   
public:
    File_not_found(const std::string& nom_fichero);
};



// cuando no se puede leer un fichero por lo que sea
class File_cant_read:public Excepcion
{   
public:
    File_cant_read(const std::string& nom_fichero);
};


// cuando no se puede escribir un fichero por lo que sea
class File_cant_write:public Excepcion
{   
public:
    File_cant_write(const std::string& nom_fichero);
};

// cuando un fichero no tiene el formato esperado
class Error_de_formato:public Excepcion
{   
public:
    using Excepcion::Excepcion;
};


/***************************************************************************
 *			    ERRORES DE PROGRAMACIÓN
 ***************************************************************************/
// cuando no se puede llegar a ciertas partes de código, lanzamos:
class Imposible_llegar_aqui:public Error_de_programacion
{
public:
    Imposible_llegar_aqui(const char* file, int line
						, const std::string& funcion);
};

// Hay cosas que se dejan sin implementar (por ejemplo: al implementar utf8
// solo implemento manejo de 2 bytes, ya que es lo que realmente uso, pero no
// implemento 3 ni 4 bytes).
class Falta_implementar:public Error_de_programacion
{
public:
    Falta_implementar(const char* file, int line, const std::string& funcion,
			const std::string& descripcion);
};


// cuando se da una opción imposible en un switch
// PE: Un color puede ser red, green o blue. Si no se da uno de estos colores
// es que hay algo mal.
class Error_opcion_imposible:public Error_de_programacion
{
public:
    using Error_de_programacion::Error_de_programacion;
};

// cuando no se satisface una precondición
class Precondicion:public Error_de_programacion
{
public:
    using Error_de_programacion::Error_de_programacion;
};

// cuando no se satisface una postcondición
class Postcondicion:public Error_de_programacion
{
public:
    using Error_de_programacion::Error_de_programacion;
};


/*!
 *  \brief  Precondición que tiene que satisfacer una función.
 *
 *  Si no se satisface se lanza una excepción Precondición.
 *
 *  EJEMPLO:
 *	alp::precondicion(img1.cols() < 2
 *	    , __FILE__, __LINE__, "for_each1D_adelante"
 *	    , "Imagen con menos de 2 columnas");
 */
//#ifndef ALP_DEBUG
inline void precondicion(bool condicion
	, const char* file, int line, const std::string& funcion
	, const std::string& msgError)
{
    if(!condicion)
	throw Precondicion{file, line, funcion, msgError};
}
//#else
//inline void precondicion(bool condicion
//	, int line, const char* file, const char* funcion
//	, const std::string& msgError){}
//
//#endif


/***************************************************************************
 *			    ERRORES DE C (errno)
 ***************************************************************************/
class Perror : public Excepcion {
public:
    Perror(const std::string& nom_funcion);
};


}// namespace alp
#endif

