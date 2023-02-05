// Copyright (C) 2019-2022 Manuel Perez <manuel2perez@proton.me>
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

#ifndef __ALP_GETOPTS_H__
#define __ALP_GETOPTS_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Versión getopts en C++
 *
 *   - COMENTARIOS: Mejora un pelín la versión de C. El cliente no tiene que
 *   lidiar con el switch de las distintas opciones, sino que directamente
 *   pasa una referencia a donde se quiera guardar los resultados de las
 *   opciones y esta clase lo gestiona todo.
 *
 *	La versión de boost tiene un interfaz muy raro.
 *   
 *
 *   - HISTORIA:
 *     Manuel Perez
 *     29/04/2019 Escrito
 *     01/11/2021 Soporte para unsigned int
 *     31/07/2022 Bug: no admitía números negativos. Corregido.
 *		  OJO: ahora los argumentos pueden empezar por '-' y es 
 *		  argumento, no opción.
 *
 ****************************************************************************/
#include <vector>
#include <string>
#include <variant>
#include <map>
#include <stdexcept>

namespace alp{

/// En caso de que se produzca algún error en Getopts se lanza esta excepción
class Getopts_error : public std::runtime_error{
public:
    Getopts_error(const std::string& help, const std::string& msg);
};

/*!
 *  \brief  Clase que extrae las opciones pasadas en la línea de comandos.
 *
 *  Las opciones todas empiezan con -
 *
 *  La idea es definir las opciones y la variable donde meter el resultado 
 *  en caso de existir.
 *
 *  Ejemplo:
 *	Queremos pasar cosas del tipo:
 *	
 *	prog -t 12.5 -k -i input --output dst -abc file1 file2 ...
 *
 *  La única información que necesitamos dar realmente es:
 *	t --> time
 *	k --> es_k_true
 *	i --> input
 *	output --> dst
 *	a ---> true
 *	b ---> true
 *	c ---> true
 *
 *  Si se pasa la opción -t Getopt mete el valor correspondiente en la
 *  variable de tipo float time. Si se pasa la opción -k, pone true la vble 
 *  (bool) es_k_true, y si se pasa la opción -i se guarda en (std::string)
 *  input.
 *
 *  Ver los ejemplos del test.
 *
 */
class Getopts{
public:
    /// help = en caso de que el usuario no pase un número mínimo de
    /// argumentos, la función parse lanza una excepción de tipo
    /// Getopts_bad_option_error con el mensaje de ayuda el help.
    Getopts(const std::string& help):num_min_args_{-1}, help_{help}{}

    /// Como mínimo esperamos num_min_args de argumentos.
    Getopts(int num_min_args, const std::string& help)
        : num_min_args_{num_min_args}, help_{help}
    { }

    // Opciones de caracter: -c
    // ------------------------
    // No defino add_option como template ya que no quiero que se le pueda
    // pasar cualquier cosa.
    /// Asociamos una opción de una letra con res
    void add_option(char opt, bool& res)
    {	res = false; char_opts_[opt] = &res; }

    /// Asociamos una opción de una letra con res
    void add_option(char opt, std::string& res)
    { char_opts_[opt] = &res; }

    /// Asociamos una opción de una letra con res
    void add_option(char opt, int& res)
    { char_opts_[opt] = &res; }

    /// Asociamos una opción de una letra con res
    void add_option(char opt, unsigned int& res)
    { char_opts_[opt] = &res; }

    /// Asociamos una opción de una letra con res
    void add_option(char opt, float& res)
    { char_opts_[opt] = &res; }


    // Opciones de cadena: --opcion
    // ----------------------------
    /// Asociamos la opción con res
    void add_option(const std::string& opt, bool& res)
    { res = false; str_opts_[opt] = &res; }

    /// Asociamos la opción con res
    void add_option(const std::string& opt, std::string& res)
    { str_opts_[opt] = &res; }

    /// Asociamos la opción con res
    void add_option(const std::string& opt, int& res)
    { str_opts_[opt] = &res; }

    /// Asociamos la opción con res
    void add_option(const std::string& opt, unsigned int& res)
    { str_opts_[opt] = &res; }

    /// Asociamos la opción con res
    void add_option(const std::string& opt, float& res)
    { str_opts_[opt] = &res; }

    /// Parse la linea de comandos, guardando el resultado de las opciones
    /// en la variable pasada como parámetro en add_option.
    /// Devuelve la línea de comandos una vez quitadas las opciones.
    /// En caso de error, lanza una excepción Getopts_error
    /// indicando el error sucedido.
    std::vector<std::string> parse(int argc, char* argv[]);


// 31/07/2022 - Lo comento. Nunca lo he usado.
//    /// Devuelve un puntero de argv a la primera posición donde comienza 
//    /// el resto de argumentos (los que no corresponden a opciones).
//    // DUDA: ¿necesito esto? 
//    // En principio lo necesito para interaccionar con funciones de C, lo 
//    // que es poco probable.
//    // Si se usase mucho, un mejor diseño sería que parse devolviera un índice
//    // a la posición de argv donde comienza el resto de los argumentos.
//    static char** args2pointer_in_argv( const std::vector<std::string>& args,
//				int argc, char* argv[])
//    { return &argv[argc - args.size() + 1]; }

private:
// Data
    int num_min_args_;	    // número mínimo de argumentos. si -1 se ignora
    const std::string help_;

    // En vble_option metemos un puntero a donde tenemos que meter el
    // resultado en caso de que una opción esté presente.
    using vble_option =
        std::variant<bool*, std::string*, unsigned int*, int*, float*>;

    // option, vble donde guardar el resultado
    std::map<std::string, vble_option> str_opts_;
    std::map<char, vble_option> char_opts_;
    
// Helper functions
    // Convierte los argumentos (argc, argv) en un vector de strings
    // (más cómodos de manejar). Excluye argv[0], que es el nombre del
    // programa.
    std::vector<std::string> cmd_line2vector(int argc, char* argv[]);

    // Devuelve el par a la opción opt. En caso de no encontrarlo lanza una
    // excepción Getopts_error.
    std::pair<char, Getopts::vble_option&> find_char_option(char opt);

    // Devuelve el par a la opción opt. En caso de no encontrarlo lanza una
    // excepción Getopts_error.
    std::pair<const std::string&, vble_option&> 
				find_string_option(const std::string& opt);

    std::vector<std::string>::iterator 
		    parse_option(std::vector<std::string>::iterator a,
                      std::vector<std::string>::iterator args_end);

    std::vector<std::string>::iterator 
		    parse_string_option(std::vector<std::string>::iterator a,
                      std::vector<std::string>::iterator args_end);

    std::vector<std::string>::iterator 
		    parse_char_option(std::vector<std::string>::iterator a,
                      std::vector<std::string>::iterator args_end);

    std::vector<std::string>::iterator 
	    parse_one_char_option(std::vector<std::string>::iterator a,
				 std::vector<std::string>::iterator args_end);
    std::vector<std::string>::iterator 
	   parse_multichar_option(std::vector<std::string>::iterator a,
                             std::vector<std::string>::iterator args_end);
};


}// namespace


#endif


