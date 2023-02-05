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

#include "alp_string.h"


using std::string;

namespace alp{

/***************************************************************************
 *		     FUNCIONES PARA MANEJAR strings
 ***************************************************************************/
// Una cadena esta vacía cuando solo esté formada por espacios y tabuladores
bool esta_vacia(const std::string& s)
{
    for(auto x: s){
	if(x != ' ' and x != '\t')
	    return false;
    }

    return true;
}


// elimina los espacios en blanco y tabuladores del final de la cadena
std::string rtrim(const std::string& s)
{
    if(s.size() == 0)
	return s;

    size_t i = s.size()-1;

    for(; i > 0; --i){
	if(s[i] != ' ' and s[i] != '\t'){
	    return s.substr(0, i+1);
	}
    }

    if(i == 0)
	return "";
	
    return s;
}


// elimina los espacios en blanco y tabuladores del principio de la cadena
std::string ltrim(const std::string& s)
{
    for(size_t i = 0; i != s.size(); ++i){
	if(s[i] != ' ' and s[i] != '\t'){
	    return s.substr(i, s.size()-i);
	}
    }
	
    return s;
}


// cuenta el número de líneas que tiene la cadena [begin, end).
unsigned num_lineas(std::string::const_iterator begin, 
	    std::string::const_iterator end)
{
    if(begin == end) 
	return 0;

    for(;begin != end; ++begin){
	 if(*begin == '\n')
		return num_lineas(++begin, end) + 1;
    }

    return 1;
}



/// Cambia el último espacio de la cadena [begin, begin+ancho_max) por un '\n'
/// siempre y cuando no encuentre un '\n' antes.
/// TODO: falla con los tabuladores.
auto cambia_ultimo_espacio_por_enter( std::string::iterator begin
				, std::string::iterator end
				, unsigned ancho_max)
{
    auto last_space = end;

    // TODO: mejor usar: last_space = find_last(begin, end, ' ', ancho_max);
    unsigned i = 0;
    for(; i < ancho_max and begin != end; ++i, ++begin){
	if(*begin == ' ')
	    last_space = begin;
	else if(*begin == '\n')
	    i = ancho_max;
    }

    // Si no se ha encontrado ningún espacio, movemos al siguiente espacio.
    if(last_space == end){
	begin = find(begin, end, ' ');
	if(begin != end)
	    *begin++ = '\n';

	return begin;
    }

    // Si hemos llegado al final de [begin, begin+ancho_max)
    if(i == ancho_max){
	if(begin != end and *begin == ' ') // Un espacio justo en b+ancho_max?
		last_space = begin;
	*last_space = '\n';
	return ++last_space;
    }

    return begin;
}


std::string split_lineas_ancho(const std::string& s, unsigned ancho_max)
{
    std::string res = s;

    std::string::iterator first = begin(res);
    std::string::iterator last = end(res);

    while(first != last)
	first = cambia_ultimo_espacio_por_enter(first, last, ancho_max);

    return res;

}

// Devuelve el basename (nombre del fichero, incluida la extensión)
// del path. Se puede usar un separador diferente a '/' (para que se pueda
// usar para más cosas).
std::string basename(const std::string& path, char sep)
{
    auto p = find_last(path, sep);
    
    if(p == path.end()) return std::string{""};

    return std::string{std::next(p), path.end()};
}

// Devuelve el nombre del fichero sin la extensión correspondiente.
std::string nombre_sin_extension(const std::string& nom_fichero)
{
    auto p = find_last(nom_fichero, '.');
    return std::string(begin(nom_fichero), p);
}



// Devuelve el directorio de este path
std::string dirname(const std::string& path, char sep)
{
    // 1.- ¿Tiene extensión?
    auto punto = find_last(path, '.', false);
    if(punto == end(path))
	return path;

    // 2.- Si la tiene, quitamos el fichero.
    // TODO: es un poco ineficiente, habría que buscar a partir de punto!!!
    auto p = find_last(path, sep, false);
    return string{begin(path), p};
}


/****************************************************************************
 *
 *   - FUNCIÓN: split
 *
 *   - DESCRIPCIÓN: Trocea una cadena usando un separador
 *
 *   - EJEMPLO: 
 *	    for(auto x: split("Esto es una prueba", ' '))
 *		cout << '[' << x << "]\n";
 *
 *
 *   - TODO:	
 *	    No funciona bien si ponemos el separador al principio y al final.
 *	    Si pongo dos separadores juntos consideramos que hay una palabra
 *	    vacía (correcto?, yo diría que si).
 *
 *	    split("/uno//dos/tres/"); <-- probar esto
 *
 ****************************************************************************/
std::vector<std::string> split(const std::string& s, char sep)
{
    std::vector<std::string> res;

    for(auto first = s.begin(); first != s.end();){
	auto q = find(first, s.end(), sep);

	res.push_back(std::string{first, q});

	if(q == s.end())
	    return res;
	
	first = next(q);
    }

    return res;
}



/****************************************************************************
 *
 *   - FUNCIÓN: split_into_directory_and_name
 *
 *   - DESCRIPCIÓN: Descompone el nombre de un path en nombre del directorio
 *	    y fichero.
 *
 *	    El nombre lo copio de Stroustrup (no la implementa, solo
 *	    habla de esta función).
 *
 *   - EJEMPLO: 
 *	    string dir, name;
 *	    tie(dir, name) = split_into_directory_and_name("/dir/fic");
 *
 *	    Devuelve: dir = "/dir/" <--- observar que incluye el '/' final
 *		      name = "fic"
 *
 ****************************************************************************/
std::pair<std::string, std::string> split_into_directory_and_name(
	const std::string& path, char sep)
{
    auto q = find_last(path, sep);
    
    if(q != path.end()){
	auto dir  = string{path.begin(), ++q};
	auto name = string{q, path.end()};

	return make_pair(dir, name);
    }

    // si q == path.end() quiere decir que el path es un nombre
    // de fichero (path = "fichero.txt"), así que no hay directorio
    return make_pair("", path);
}



}// namespace alp
