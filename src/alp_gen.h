// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
 *           alp  - 14/11/2015 Escrito
 *
 ****************************************************************************/
#include "alp_concepts.h"
#include "alp_math.h"
#include "alp_exception.h"
#include "alp_string.h"
#include "alp_type_traits.h"

#include <fstream>
#include <thread>

namespace alp{


/*****************************************************************************
 * 
 *   - CLASE: IntervaloC
 *
 *   - DESCRIPCIÓN: Intervalo cerrado.
 *   - TODO: Generalizar a abierto, cerrado y semiabierto:
 *	+ ¿con templates? Intervalo<Cerrado> I;
 *			  Intervalo<Abierto> I;
 *
 *	+ ¿con parámetros? Intervalo I{2,3,abierto};?
 *
 ***************************************************************************/
//class IntervaloC{
//public:
//    IntervaloC(int a = 0, int b = 0):a_{a}, b_{b}{}
//
//    int a() const {return a_;}
//    int b() const {return b_;}
//
//    IntervaloC& amplia(int x);
//
//private:
//    int a_, b_;
//};
//
//inline int longitud(IntervaloC I) {return abs(I.b()-I.a()); } 
//
// Idea: modificar para poder escribir: if (pertenece(x).a(I)) ...
// Problema: tengo muchas funciones pertenece:
//  ¿pertenece un elemento a un vector?	    pertenece(x).a(vector)
//  ¿pertenece un elemento a un intervalo?  pertenece(x).a(intervalo)
//  ¿pertenece un punto a una region?	    pertenece(x).a(region)
//
// Pero no puedo sobrecargar pertenece(x). Debería de escribir al reves:
//	pertenece_al(vector).el_elemento(x);
//	pertenece_al(intervalo).el_elemento(x);
//	pertenece_al(region).el_elemento(x);
//
//  pero esto es confuso.
//
//  Otra forma sería modifica el lenguaje y que admitiese cosas del tipo:
//	    pertenece(x)_a(vector); ---> pertenece_a(x, vector);
//
//inline bool pertenece(int x, IntervaloC I) 
//{return esta_entre(x, I.a(), I.b());}
//
//// si x no pertenece al intervalo I, lo ampliamos para que sea uno de sus
//// extremos.
//inline IntervaloC& IntervaloC::amplia(int x)
//{
//    if(!pertenece(x, *this))
//	x < a_? a_ = x: b_ = x;
//
//    return *this;
//}
//
//
//
//
//
//
//// ordenamos los intervalos por a
////inline bool operator<(const IntervaloC& I1, const IntervaloC& I2)
////{ return (I1.a() < I2.a()); }
//
//
//inline std::ostream& operator<<(std::ostream& out, const IntervaloC& I)
//{
//    out << '[' << I.a() << ", " << I.b() << ']';
//
//    return out;
//}




//// copiamos solo el color indicado: [p0, pe) -> [q0, qe)
//template<typename ColorFunc, typename const_It, typename It>
//It copia(const_It p0, const_It pe, It q)
//{
//    ColorFunc color;
//    for(auto x = p0; x != pe; ++x, ++q)
//	color(*q, color(*x));
//
//    return q;
//}

/*****************************************************************************
 * 
 *   - CLASE: flag
 *
 *   - DESCRIPCIÓN: Clase auxiliar para poder escribir código del tipo:
 *	    bool red = true;
 *	    ...
 *	    cout << flag(red, 'R'); // imprime R si está activo o - si no lo
 *	    está
 *
 ***************************************************************************/
class flag{
public:
    flag(bool f, char c):on_{f}, c_{c}{}

    friend std::ostream& operator<<(std::ostream& out, flag f);

private:
    bool on_; // on/off
    char c_; // caracter a mostrar si está activo el flag
};


inline  std::ostream& operator<<(std::ostream& out, flag f)
{
    if(f.on_) out << f.c_;
    else out << '-';
    return out;
}




/****************************************************************************
 *
 *   - FUNCIÓN: existe(Fichero)
 *
 *   - DESCRIPCIÓN: Devuelve true si existe el fichero pasado como parámetro,
 *	    false en caso contrario.
 *	    
 *   - EJEMPLO:
 *	    if(!existe(Fichero{"03.jpg"})) ...
 *
 ****************************************************************************/
class Fichero:public Nombre<std::string>{ public: using Nombre::Nombre; };


inline bool existe(Fichero f)
{
    std::ifstream in{f()};
    if(in) return true;
    else return false;
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
template<Contenedor_unidimensional Cont>
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
template<Contenedor_unidimensional Cont>
inline Tabla<Cont> tabla( const std::string& titulo
		     , Cont& v) 
{return Tabla<Cont>{titulo, v};}




/*****************************************************************************
 * 
 *   - CLASE: Range
 *
 *   - DESCRIPCIÓN: Definimos un rango de números. En principio
 *	solo voy a considerar números enteros (es lo natural). 
 *
 ***************************************************************************/
class Range_mal_definido{
public:
    Range_mal_definido(int i0, int ie)
    {
	msg_ = "Error";
    }

    std::string what() const{ return msg_;}

private:
    std::string msg_;
};

//template<typename T>
class Range{
public:
    using value_type = int; //T;
    class iterator{
    public:
	explicit iterator(value_type i0):i_{i0}{}
	iterator operator++(){++i_; return *this;}
	value_type operator*() const {return i_;}

	bool operator==(const iterator& b) const
	{return (i_ == b.i_);}
	
	bool operator!=(const iterator& b) const
	{return !(*this == b);}

    private:
	value_type i_;	// valor actual
    };

    using const_iterator = iterator;
    
    Range(value_type i0, value_type ie)
	:i0_{i0}, ie_{ie}
    {
	if(i0_ > ie_)
	    throw Range_mal_definido{i0_, ie_};
    }

    iterator begin() {return iterator{i0_};}
    iterator end()   {return iterator{ie_};}

    const_iterator begin() const {return iterator{i0_};}
    const_iterator end()   const {return iterator{ie_};}

private:
    value_type i0_, ie_;   // Range = [i0, ie)
};

inline Range range(int i0, int ie)
{return Range{i0, ie};}

//template<typename T>
//inline Range<T> range(T i0, T ie)
//{
//    return Range<T>{i0, ie};
//}









}// namespace alp


#endif
