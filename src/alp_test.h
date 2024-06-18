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
#ifndef __ALP_TEST_H__
#define __ALP_TEST_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Funciones para probar programas.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    06/05/2017 Escrito
 *    02/04/2020 CHECK_STDOUT
 *    10/05/2020 CHECK_EXCEPTION
 *    30/07/2020 CHECK_DONT_COMPILE
 *    23/07/2021 CHECK_PRINT
 *    09/01/2023 Experimentando con std::source_location. Elimino macros.
 *
 ****************************************************************************/
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <source_location>

#include "alp_exception.h"

namespace test{

struct Data{
    static std::string archivo_cabecera;    // que probamos
    static std::string interfaz;    // interfaz que estamos probando
    constexpr static int ancho_msg_prueba = 30;
    static std::string file_error;  // fichero donde se produce el error = __FILE__
    static int line_error;  // línea donde se produce el error == __LINE__
};


inline void print_prueba(const std::string& prueba)
{
    auto flags0 = std::cout.flags();

    std::cout << std::left << std::setfill('.') 
	      << std::setw(Data::ancho_msg_prueba) << prueba;

    std::cout.flags(flags0);	
}

class Error:public alp::Excepcion{
public:
    Error(const std::string& prueba, const std::string& detalles = "")
    {
	std::stringstream msg_error;

	msg_error << "\n\n-------------------------------------------\n"
		"ERROR ";

	if (!Data::file_error.empty())
		msg_error << Data::file_error << '[' << Data::line_error << "]\n";

	if (!Data::interfaz.empty())
	    msg_error << "Función: " << Data::interfaz << "\n";
	
	msg_error <<  "Prueba: " << prueba << "\n";
	
	if (!detalles.empty())
	    msg_error << "Detalles: " << detalles << "\n";

	 msg_error << "\n\n";

	 msg = msg_error.str();
    }
};


inline void header(const std::string& archivo_cabecera)
{ 
    Data::archivo_cabecera = archivo_cabecera; 

    std::cout << "\n\n=================================\n"
	      << "Header: " << archivo_cabecera << '\n'
	      << "=================================\n";
}

inline void interfaz(const std::string& interfaz)
{ 
    Data::interfaz = interfaz; 
    std::cout << "\n\n---------------------------------\n"
	      << "Interface: " << interfaz << '\n'
	      << "---------------------------------\n";
}

// migrando a ingles
inline void interface(const std::string& str)
{ interfaz(str); }


/// El parámetro pasado tiene que ser true.
///
/// Ejemplo:
/// \code
///	test::check_bool(a == 3, true, "operator==");
/// \endcode
inline void check_bool	( bool condicion
			, bool resultado
			, const std::string& prueba
			, const std::source_location loc = std::source_location::current())
{
    std::cout << prueba;

    if(condicion == resultado)
	std::cout << " OK\n";
    else{
	test::Data::file_error = loc.file_name(); 
	test::Data::line_error = loc.line(); 
	throw Error{prueba};
    }
}


///// Comprueba que la condicion pasada es true.
///// Ejemplo:
///// \code
/////	test::check_bool(a == 3, "operator==");
///// \endcode
//inline void check_true(  bool condicion
//		       , const std::string& prueba
//		       , const std::source_location loc = std::source_location::current())
//{ check_bool(condicion, true, prueba, loc); }
//
///// Comprueba que la condicion pasada es false.
//inline void check_false	( bool condicion
//			, const std::string& prueba
//		        , const std::source_location loc = std::source_location::current())
//{ check_bool(condicion, false, prueba, loc); }
//

// Para hacer pruebas me gusta mas usar las letras CHECK_TRUE en mayúsculas.
// De esa forma se ve claramente la diferencia entre la prueba y el check.
inline void CHECK_TRUE(bool condition, 
		const std::string& test_name,
		const std::source_location loc = std::source_location::current())
{ check_bool(condition, true, test_name, loc); }

inline void CHECK_FALSE(bool condition, 
		const std::string& test_name,
		const std::source_location loc = std::source_location::current())
{ check_bool(condition, false, test_name, loc); }



/*
#define CHECK_TRUE(condicion, prueba) \
	    {test::Data::file_error = __FILE__; \
	    test::Data::line_error = __LINE__; \
	    test::check_true((condicion), (prueba));}

#define CHECK_FALSE(condicion, prueba) \
	    {test::Data::file_error = __FILE__; \
	    test::Data::line_error = __LINE__; \
	    test::check_false((condicion), (prueba));}

*/

// Algunas prueba no sé cómo hacerlas automáticamente. Lo que hago es las
// imprimo en pantalla mostrando el resultado que tendría que tener y con un
// script hacemos la comprobación.
template <typename T0, typename T1>
inline void CHECK_STDOUT(const T0& res, const T1& res_ok
		  , const std::source_location loc = std::source_location::current())
{
    std::cout	<< "check[" 
		<< loc.file_name() << '-' << loc.line() <<"]: " 
		<< res << " =? " << res_ok << '\n';
}


#define CHECK_PRINT(print, res_ok) \
	{std::cout << "check[" << __FILE__ << '-' << __LINE__ <<"]: "; \
	 print;	 \
	std::cout << " =? " << res_ok << '\n';}

// Probamos que la llamada de la funcion 'cmd' lance una excepción.
// En caso de no lanzarla aborta la ejecución mostrando el mensaje 'msg'.
#define CHECK_EXCEPTION(cmd, msg)\
    try{ (cmd); std::cerr << "No se lanza la excepción. Prueba: " \
	                  << (msg) <<'\n'; exit(1);}\
    catch(std::exception&){CHECK_TRUE(true, (msg));}


// ¿Cómo comprobar que algo que no tiene que compilar no compila?
// Una forma puede ser usar esta macro. En principio si se ejecuta el test se
// elimina el `cmd` que no compila, compilando y probando todo de forma
// habitual. Luego con un script se van sustituyendo uno por uno los
// CHECK_DONT_COMPILE por el `cmd` y el script prueba a compilarlo teniendo
// que fallar la compilación. Si no falla la compilación el script finaliza
// mostrando un mensaje de error igual a `msg`.
#define CHECK_DONT_COMPILE(cmd, msg)	;

/*!
 *  \brief  Verificamos que se satisface la condición dada para cada elemento
 *  \brief del rango [p0, pe)
 *
 */
template <typename It, typename Condicion>
void check_for_each_condicion(It p0,
                              It pe,
                              Condicion cond,
                              const std::string& prueba)
{
    print_prueba(prueba);

    for(; p0 != pe; ++p0){
	if (!cond(*p0)){
	    std::cerr << "\n\n------------------------------------------\n"
		      << "ERROR: *p = [" << *p0 << "]\n"
		      << "------------------------------------------\n";

	    throw Error{prueba};
	}
    }
    
    std::cout << "OK\n";
}

template <typename It1, typename It2>
// requires forward_iterator(It1) and forward_iterator(It2)
void CHECK_EQUAL_CONTAINERS(It1 p0, It1 pe,
                            It2 q0, It2 qe,
                            const std::string& prueba
			    , const std::source_location loc = 
						std::source_location::current())
{
    test::Data::file_error = loc.file_name();
    test::Data::line_error = loc.line();

    print_prueba(prueba);

    int i = 0;
    for(; p0 != pe and q0 != qe; ++p0, ++q0, ++i)
        if (*p0 != *q0) {
	    std::ostringstream out;
	    out << "p[" << i << "] = [" << *p0 << "] != q = [" << *q0 << "]\n";

	    throw Error{prueba, out.str()};
        }

    if (p0 != pe or q0 != qe) {
        throw Error{prueba, "Contenedores con tamaños diferentes"};
    }

    std::cout << " OK\n";
}


template <typename Cont1, typename Cont2>
// requires Cont1, Cont2 containers
inline void CHECK_EQUAL_CONTAINERS(const Cont1& c1,
                                   const Cont2& c2,
                                   const std::string& prueba
			    , const std::source_location loc = 
						std::source_location::current())
{
    CHECK_EQUAL_CONTAINERS(c1.begin(), c1.end(), c2.begin(), c2.end(), prueba);
}

// TODO: eliminar CHECK_EQUAL_CONTAINERS_C. La necesitaba al usar macros, pero
// ahora puedo usar directamente CHECK_EQUAL_CONTAINERS.
template <typename Cont1, typename Cont2>
// requires Cont1, Cont2 containers
inline void CHECK_EQUAL_CONTAINERS_C(const Cont1& c1,
                                   const Cont2& c2,
                                   const std::string& prueba
			    , const std::source_location loc = 
						std::source_location::current())
{ return CHECK_EQUAL_CONTAINERS(c1, c2, prueba, loc); }

/*
#define CHECK_EQUAL_CONTAINERS(in0, ine, out0, oute, prueba) \
	    {test::Data::file_error = __FILE__; \
	    test::Data::line_error = __LINE__; \
	    test::check_equal_containers((in0), (ine), (out0), (oute), (prueba));}


#define CHECK_EQUAL_CONTAINERS_C(cont1, cont2,  prueba) \
	    {test::Data::file_error = __FILE__; \
	    test::Data::line_error = __LINE__; \
	    test::check_equal_containers((cont1), (cont2), (prueba));}

*/

/// Comprueba que la órbita de f es igual a [p0, pe).
/// Ejemplo:
///
/// \code
///	array<Direccion, 8> res = {{Direccion::NE, Direccion::N,
///                           Direccion::NW, Direccion::W,  Direccion::SW,
///                           Direccion::S,  Direccion::SE, Direccion::E}};
///
///	v = Vector_direccion::E();
///	check_orbita0(res.begin(), res.end(), 
///		[&v]()
///		{
///		    v.gira_45(true);
///		    return v.direccion();
///		}
///		, "Probamos girar +45");
/// \endcode
template <typename It, typename Functor>
void check_orbita0(It p0, It pe, Functor f, const std::string& prueba)
{
    std::cout << prueba << " ... ";

    int n = 0;
    for(auto p = p0; p != pe; ++p){
	++n;

	auto y = f();

	if (y != *p){
	    std::cerr
		<< "\n\n------------------------------------------------\n"
		<< "ERROR: el elemento que se encuentra en la posición "
		<< n << "\ndebería de valer [" << *p
		<< "], pero se obtiene [" << y << "]"
		<< "\n------------------------------------------------\n";

	    throw Error{prueba};
	}
	
    }

    std::cout << "OK\n";
}



/// Ahora comprobamos que f(x) == y, para todo x en [x0, xe), e y en [y0, ye)
/// 
/// Ejemplo:
/// \code
///     vector<Vector_direccion> x{
///            Vector_direccion::E(),  Vector_direccion::S(),
///            Vector_direccion::W(),  Vector_direccion::N(),
///            Vector_direccion::NE(), Vector_direccion::SE(),
///            Vector_direccion::SW(), Vector_direccion::NW()};
///
///     vector<Position> y{{1, 2}, {2, 2}, {2, 1}, {1, 1},
///                           {0, 2}, {1, 3}, {2, 2}, {1, 1}};
///
///	Iterador_4direcciones it{img1, {1,1}};
///
///	check_orbita1(x.begin(), x.end(), y.begin(), y.end(), 
///		[&it](auto x)
///		{
///		    it.muevete(x);
///		    return it.posicion();
///		}
///		, "Probando muevete");
/// \endcode
template <typename It_in, typename It_out, typename Functor>
void check_orbita1(It_in x0, It_in xe, It_out y0, It_out ye, Functor f,
                   const std::string& prueba)
{
    std::cout << prueba << " ... ";

    int n = 0;

    for(; x0 != xe and y0 != ye; ++x0, ++y0){
	++n;

	auto y = f(*x0);

	if (y != *y0){
	    std::cerr
		<< "\n\n------------------------------------------------\n"
		<< "ERROR: el elemento que se encuentra en la posición "
		<< n << "\ndebería de valer [" << *y0
		<< "], pero se obtiene [" << y << "]"
		<< "\n------------------------------------------------\n";

	    throw Error{prueba};
	}
	
    }

    if (x0 != xe or y0 != ye) {
//        std::cout << "\n\n------------------------\n"
//                  << "ERROR: size(x) != size(y)!!!\n"
//                  << "\n\n------------------------\n";
        throw Error{prueba, "size(x) != size(y)"};
    }

    std::cout << "OK\n";
}


// Una función cómoda para depurar
template <typename It>
void print_range(It p0, It pe)
{
    for(; p0 != pe; ++p0)
	std::cout << *p0 << ' ';

    std::cout << '\n';
}


template<typename C>
void print_range2D(const C& c)
{
    for (auto f = c.row_begin(); f != c.row_end(); ++f){
	    for (auto x: *f)
		std::cout << x << ' ';
	std::cout << '\n';
    }
}

inline std::string entre_corchetes(const std::string& s)
{
    std::string res = "[";
    res += s + "]";
    return res;
}

inline std::string entre_corchetes(std::string::const_iterator p0,
                            std::string::const_iterator pe)
{return entre_corchetes(std::string{p0, pe});}

inline std::ostream& eprint(const std::string& s)
{
    return std::cout << '[' << s << ']';
}

inline std::ostream& eprint(std::string::const_iterator p0,
                            std::string::const_iterator pe)
{
    return eprint(std::string{p0, pe});
}

struct Objeto {
    int x, y;
};

inline bool operator==(const Objeto& p, const Objeto& q)
{ return (p.x == q.x) and (p.y == q.y); }

inline bool operator!=(const Objeto& p, const Objeto& q)
{return !(p == q);}


inline std::ostream& operator<<(std::ostream& out, const Objeto& p)
{return out << '(' << p.x << ", " << p.y << ')';}


} // namespace

#endif
