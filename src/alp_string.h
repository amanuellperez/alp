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

#ifndef __ALP_STRING_H__
#define __ALP_STRING_H__

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>  // stringstream
#include <cctype>

namespace alp{

// convertimos un tipo A en un tipo B
// auto x = to<int>("3");
// auto x = to<string>(3);
template<typename B, typename A>
inline B to(A x)
{
    B y;

    std::stringstream s;
    s << x;
    s >> y;

    return y;
}

// Cuando la representación de cadena de A tiene espacios (como ColorRGB),
// la función to<string> falla, ya que solo devuelve el primer color, hasta el
// espacio (ej: Color = (10, 20, 40), to<string>(Color) devuelve "(10,"
// TODO: hacer una especialización de to<string>!!!
// TODO: ¿esta no es la función estandar std::to_string??? <--- Sí!!!
template<typename A>
inline std::string to_string(A x)
{
    std::string y;

    std::stringstream s;
    s << x;
    y = s.str();

    return y;
}


/*!
 *  \brief  Convertimos un flujo en una cadena.
 *
 *  A la hora de trazar o hacer pruebas es habitual querer convertir distintos
 *  parámetros en strings. Esta clase permite hacer eso.
 *
 *  Ejemplo:
 *	void f(const std::string& s, int a)
 *	{
 *	    Trace t{as_str() << "f(" << s << ", " << a << ")"};
 *	    ...
 *	}
 *
 */
struct as_str{
    std::stringstream out;

    template <typename T>
    as_str& operator<<(const T& x)
    {
	out << x;
	return *this;
    }

    operator std::string() const {return out.str();}
};


/// Busca el primer caracter no whitespace = [ \t\n\v...]
/// Si no lo encuentra devuelve pe.
//template <typename It>
//It skip_whitespace(It p0, It pe)
//{
//    while (p0 != pe and std::isspace(*p0))
//	++p0;
//
//    return p0;
//}
//
///// Busca el primer caracter whitespace = [ \t\n\v...]
///// En caso de no encontrarlo devuelve pe.
//template <typename It>
//It find_whitespace(It p0, It pe)
//{
//    while (p0 != pe and !std::isspace(*p0))
//	++p0;
//
//    return p0;
//}



/****************************************************************************
 *			Funciones para formatear strings
 ***************************************************************************/
/*!
 *  \brief    Añade a la cadena s, n caracteres c, centrando la cadena.
 *
 *  Ejemplo:
 *  \code
 *	    s = add_centrados("hola", 4, '.');
 *	
 *	>>> s = "..hola.."
 *  \endcode
 *
 */
inline std::string add_centrados(const std::string& s, int n, char c)
{
    std::string res;
    int m = n/2;    // la mitad

    for(int i = 0; i < m; ++i)
	res+=c;
    
    res+=s;

    for(int i = m; i < n; ++i)
	res+=c;

    return res;
}


/****************************************************************************
 *
 *   - FUNCIÓN: mismo_ancho_y_centradas
 *
 *   - DESCRIPCIÓN: Devuelve las cadenas s1 y s2 del mismo ancho, 
 *	    centrando la de menor longitud.
 *  
 *   - EJEMPLO: s1 = "hola";
 *		s2 = "nos vemos";
 *		mismo_ancho_y_centradas(s1, s2);
 *
 *	    >>> s1 = "  hola   "
 *		s2 = "nos vemos"
 *
 *   - TODO: funciona mal con los acentos!!!
 *	     La cadena string s="está" tiene 5 caracteres, no 4!!!  
 *	     length() considera que 'á' son 2 caracteres!!!
 *
 *	     ¿No sería mejor hacer:
 *		mismo_ancho(s1, s2); // añade espacios al final para igualar
 *				    // anchos.
 *		centra(s1);
 *		centra(s2);???
 *
 ****************************************************************************/
inline void mismo_ancho_y_centradas(std::string& s1, std::string& s2)
{
    int n1 = s1.length();
    int n2 = s2.length();

    if(n1 == n2) return;
    if(n1 > n2)
	s2 = add_centrados(s2, n1-n2, ' ');
    else s1 = add_centrados(s1, n2-n1, ' ');
}



inline std::string linea(char c = '-', int n = 60)
{return std::string(n, c);}

/// Crea una línea de 'n' caracteres de tipo 'c', con la cadena s en el medio.
///
/// Ejemplo:
/// \code
///	cout << linea("hola");
///
///	> --------------- hola ---------------
/// \endcode
inline std::string linea(const std::string& s, char c = '-', int n = 60)
{ 
    // le añado espacios a ambos lados de s, para que quede más bonito
    // TODO: habría que comprobar que no trae los espacios ya
    std::string fmt = ' ' + s + ' ';
    int len = n-fmt.length();
    if(len < 0) len = n;

    return add_centrados(fmt, len, c); 
}

/// Devuelve la cadena 's' entre dos líneas de longitud 'n' y tipo 'c'.
inline std::string entre_lineas(const std::string& s, char c = '-', int n = 60)
{ return linea(c, n) + "\n" + s + "\n" + linea(c, n); }


/// Una cadena esta vacía cuando solo esté formada por espacios y tabuladores
bool esta_vacia(const std::string& s);

/// elimina los espacios en blanco y tabuladores del principio de la cadena
std::string ltrim(const std::string& s);

/// Elimina los espacios en blanco y tabuladores del final de la cadena.
/// Si se quiere modificar s, usar el siguiente idioma:
///		    s = rtrim(s);
std::string rtrim(const std::string& s);

// elimina los espacios y tabuladores 
// tanto del principio como del final de la cadena
inline std::string trim(const std::string& s)
{ return rtrim(ltrim(s)); }


/*!
 *  \brief  Devuelve un iterador apuntando al último caracter v.
 *	    En caso de no encontrarlo devuelve end().
 *
 *	    Observar que el iterador que devuelve es hacia adelante.
 *
 *	    Copiado de Stroustrup.
 *
 *  Ejemplo:
 *  \code
 *	    auto p = find_last("esto_es_una_prueba", '_');
 *	    p = apunta a "_prueba" <--- observar que apunta a '_', no a 'p'
 *	    
 *	    auto p = find_last("esto_es_una_prueba", '_', false);
 *	    p = apunta a "prueba" <--- observar que apunta a 'p', no a '_'
 *  \endcode
 *
 */
template<typename C, typename Val>
auto find_last(C& c, Val v, bool incluye = true) -> decltype(c.begin())
{
    auto ri = std::find(c.rbegin(),c.rend(),v);
    if (ri == c.rend()) return c.end();

    if(incluye)
	return std::prev(ri.base());

    // no incluimos el elemento seleccionado
    return ri.base();
}

/***************************************************************************
 *	Funciones para manejar paths (TODO: obsoletas. Usar filesystem)
 ***************************************************************************/
constexpr char path_separador = '/';

/// Devuelve el basename (nombre del fichero, incluida la extensión)
/// del path. Se puede usar un separador diferente a '/' (para que se pueda
/// usar para más cosas).
std::string basename(const std::string& path, char sep = path_separador);
   
/// Devuelve el nombre del fichero sin la extensión correspondiente.
// esta es: std::filesystem::path::replace_extension();
std::string nombre_sin_extension(const std::string& nom_fichero);


/// Devuelve el directorio de este path.
/// 
/// Se entiende que el directorio es todo el path sin el nombre del fichero,
/// donde el nombre de fichero se reconoce por la extensión.
/// Esto es, si path = '/home/uno.txt', devuelve '/home/', pero si
/// path = '/home/uno', devuelve '/home/uno' ya que consideramos que 'uno'
/// forma parte del directorio.
/// 
/// Si no gusta este comportamiento por defecto y se quiere cambiar, definir
/// un flag u otra función dirname.
// TODO: cambiar las std::string path, por Filename!!!
// Un path no es un string, sino un Filename!!!
// De hecho crear la clase Filename con estos métodos!!!
std::string dirname(const std::string& path, char sep = path_separador);

/// Cambia la extensión del path, sustituyéndola por new_ext.
inline std::string 
cambia_extension(const std::string& path, const std::string& new_ext)
{ return (nombre_sin_extension(path) + "." + new_ext);}

/// Trocea una cadena usando el separador 'sep'.
std::vector<std::string> split(const std::string& s, char sep);


/*!
 *  \brief  Descompone el nombre de un path en nombre del directorio 
 *	y fichero.
 *  
 *  Ejemplo:
 *  \code
 *	std::string dir, name;
 *	tie(dir, name) = split_into_directory_and_name(path);
 *  \endcode
 *
 *  TODO: seguramente anticuado. Usar filesystem.
 */
std::pair<std::string, std::string> split_into_directory_and_name(
	const std::string& path, char sep = '/');



/// Cuenta el número de líneas que tiene la cadena [begin, end).
/// 
/// Ejemplo: la cadena s = "Esto\nes\nuna\prueba", tiene 4 líneas.
/// Usamos '\n' como separador entre líneas.
unsigned num_lineas(std::string::const_iterator begin, 
	    std::string::const_iterator end);

/// Cuenta el número de líneas que tiene la cadena s.
/// 
/// Ejemplo: la cadena s = "Esto\nes\nuna\prueba", tiene 4 líneas.
/// Usamos '\n' como separador entre líneas.
inline unsigned num_lineas(const std::string& s)
{ return num_lineas(begin(s), end(s)); }


/// Cambia el último espacio de la cadena [begin, begin+ancho_max) por un '\n'
/// siempre y cuando no encuentre un '\n' antes.
/// TODO: falla con los tabuladores.
auto cambia_ultimo_espacio_por_enter( std::string::iterator begin
				, std::string::iterator end
				, unsigned ancho_max);

/// Divide la 'texto' en lineas de longitud máxima 'ancho_max'.
// DUDA: ¿en lugar de usar int ancho_max usar Maximo<int> ancho?
std::string split_lineas_ancho(const std::string& texto, unsigned ancho_max);

std::vector<std::string> split(const std::string& s, char sep);

// TODO: rename to dir_and_name(...) ???
std::pair<std::string, std::string> split_into_directory_and_name(
	const std::string& path, char sep);

}// namespace alp

#endif
