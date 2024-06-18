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


#ifndef __ALP_VECTOR_H__
#define __ALP_VECTOR_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Ampliación de std::vector.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *       09/06/2019 Escrito
 *       16/05/2020 Reestructurado.
 *
 ****************************************************************************/

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iterator>
#include <utility>

#include "alp_type_traits.h"

namespace alp{

/// Imprime el vector, usando el separador indicado para separar elementos.
template <typename T>
std::ostream& print(std::ostream& out, const std::vector<T>& v
	    , const std::string& sep = " ")
{
    auto p = v.begin();
    auto pe = v.end();

    if (p == pe)
	return out;

    out << *p;
    ++p;

    for (; p != pe; ++p)
	out << sep << *p;

    return out;
}


/// Hace un pop_back del vector. Es syntactic sugar.
/// De todas formas recordar que si hay dos funciones back and pop_back en
/// vector es para poder garantizar la strong garanty en excepciones.
template <typename T>
inline T pop_back(std::vector<T>& v)
{
    auto res = v.back();
    v.pop_back();
    return res;
}

/// Busca el índice que ocupa el elemento 'x' en el vector 'v'.
///
/// \return Si lo encuentra, el índice i que ocupa el elemento x.
///	    Si no lo encuentra, devuelve v.size().
template<typename T>
auto find_indice(const std::vector<T>& v, const T& x)
{
    for(typename std::vector<T>::size_type i = 0; i < v.size(); ++i)
	if(v[i] == x)
	    return i;

    return v.size();
}


/// Miramos si el elemento 's' pertenece al vector 'v'. 
/// Ejemplo: if(pertenece(s, v)) ...
template<typename T>
inline bool pertenece(const T& s, std::vector<T> v)
{ return (std::find(v.begin(), v.end(), s) != v.end()); }

/// Ordenamos un vector. Ejemplo: sort(v);
template<typename T>
inline void sort(std::vector<T>& v)
{sort(v.begin(), v.end());}


/// Convierte un vector<T> en un map<int, T>, donde el int indica la posición
/// del elemento en el vector.
template<typename T>
std::map<int, T> vector2map(const std::vector<T>& v)
{
    std::map<int, T> m;
    for(typename std::vector<T>::size_type i = 0; i < v.size(); ++i)
	m[i] = v[i];

    return m;
}


/// Copia [p0, pe) en v, borrando cualquier dato que tuviera v.
// TODO: cambiar orden: copy(p0, pe, q0) y no al revés.
template <typename Value, typename It>
inline void copy_new(It p0, It pe, std::vector<Value>& v)
{
    v.clear();

    v.resize(pe - p0);

    std::copy(p0, pe, v.begin());
}


/*****************************************************************************
 * 
 *   - CLASE: Tabla
 *
 *   - DESCRIPCIÓN: Esta clase la uso para expresar mi intención en 
 *	código de leer/escribir un std::vector como tabla en un fichero.
 *
 *	Esto es, amen de escribir los datos del vector, le añado la posición.
 *	La posición es el id del elemento.
 *
 *   - FORMATO:
 *	    Escribimos el vector con el siguiente formato:
 *
 *	    [colores = 4]
 *	    0 = desconocido
 *	    1 = amarillo
 *	    2 = rojo
 *	    3 = verde
 *
 *   - EJEMPLO:
 *
 *	vector<string> v = {"desconocido", "amarillo", "rojo", "verde"};
 *	ofstream out{"kk.txt"};
 *	out << tabla("colores", v);
 *	out.close();
 *
 *	vector<string> u;
 *	ifstream in{"kk.txt"};
 *	in >> tabla("colores", u);
 *
 *	cout << tabla("colores", u);
 *
 *   - NOTAS:
 *	Al principio la parametrice por el tipo T del vector. Sin embargo
 *	eso genera el problema de que el operator<< lee de un const Tabla,
 *	generando un error al intentar inicializar Tabla<T> con un const
 *	vector.
 *
 *	Para evitar este problema lo parametrizo con el contenedor
 *	directamente (y de hecho, queda más natural). De esta forma
 *	si le paso un const vector, la tabla es const y todo funciona bien.
 *
 ***************************************************************************/
template<typename Cont>
class Tabla{
public:
    Tabla(const std::string& ttitulo, Cont& vv)
	:titulo{ttitulo}, v(vv){}

    template<typename C>
    friend std::ostream& operator<<(std::ostream& out, const Tabla<C>& t);

    template<typename C>
    friend std::istream& operator>>(std::istream& in, Tabla<C> t);

private:
    const std::string titulo;
    Cont& v;
};

template<typename C>
std::ostream& operator<<(std::ostream& out, const Tabla<C>& t)
{
    out << '[' << t.titulo << " = " << t.v.size() << "]\n";
    for(size_t i = 0; i != t.v.size(); ++i)
	out << i << " = " << t.v[i] << '\n';

    return out;
}


// CUIDADO: Observar que pasamos por valor Tabla<T> t.
// Esto no supone ningún problema, ya que vamos a escribir en el vector
// t.v, del cual no es propietario Tabla<T>. 
// TODO: ¿validar errores de formato? Si el fichero no se modifica
// externamente y se escribe y lee con estas funciones no debería de tener
// errores
template<typename C>
std::istream& operator>>(std::istream& in, Tabla<C> t)
{
    // leemos la cabecera con el tamaño
    // [colores = 20]
    std::string titulo;
    char inicio;
    in >> inicio;	// [
    in >> titulo;
    if (titulo != t.titulo)
	throw std::runtime_error{"Error: la sección del fichero "
		"empieza con un título [" + titulo + "] diferente "
		"del que tendría que tener [" + t.titulo + "]"};

    char igual;
    in >> igual;

    size_t size;
    in >> size;

    char end;
    in >> end;	//]

    // leemos la tabla
    for(size_t i = 0; i != size; ++i)
    {
	int j;
	in >> j;
	
	char igual;
	in >> igual;
	
	Value_type<C> valor;
	in >> valor;
	t.v.push_back(valor);
    }

    return in;
}



// Función auxiliar: Creamos la tabla sin necesidad de indicar el tipo T
template<typename Cont>
inline Tabla<Cont> tabla( const std::string& titulo
		     , Cont& v) 
{return Tabla<Cont>{titulo, v};}













}// namespace

#endif
