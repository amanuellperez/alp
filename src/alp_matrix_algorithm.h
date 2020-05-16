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

#ifndef __ALP_MATRIX_ALGORITHM_H__
#define __ALP_MATRIX_ALGORITHM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Algoritmos para usar con matrices.
 *
 *  - COMENTARIOS: Son algoritmos que operan en contenedores bidimensionales
 *	y no en matrices. Commo al final llamo 'matriz' a los contenedores 
 *	bidimensionales ahora diré: son algoritmos para operar en las
 *	matrices.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *       28/09/2017 Escrito
 *       16/06/2020 Reestructurado.
 *
 ****************************************************************************/

#include "alp_matrix.h"
#include "alp_submatrix.h"

#include "alp_math.h"	// punto_medio
#include "alp_type_traits.h"

#include <fstream>
#include <sstream>
#include <iterator>
#include <numeric>

namespace alp{
/// Devuelve la posición que ocupa el centro de la matrix
// TODO: esto solo depende de rows y cols!!! está definido sobre un
// rectángulo!
template <typename C>
// requires: Contenedor_bidimensional(Container)
inline typename C::Posicion posicion_del_centro(const C& c)
{ return {punto_medio(c.rows()), punto_medio(c.cols())}; }



/*!
 *  \brief  Copia la imagen img0 dentro de la imagen img1.
 *	    Coloca img0 a partir de la posición p0 de img1.
 *
 */
template <typename Container2D>
void copia_dentro  ( const Container2D& m0    // copiamos m0 en
	    , Container2D& m1		     // la imagen m1
	    , typename Container2D::Posicion p0)// colocando la esquina SI de m1 en p0 
{
    if (!esta_dentro(m0.extension()).de(m1.extension()))
        throw Error_de_programacion{
            __FILE__, __LINE__,
            "copia_dentro",
            "Estas intentando copiar una imagen que se sale de m0\n"};

    auto sb = Submatrix{m1, p0, m0.size2D()};

    // copiamos m1 en sb
    std::copy(m0.begin(), m0.end(), sb.begin());
}


/// Convierte un vector 'v' en una Matrix de 'rows x cols' dimensiones.
/// Precondicion: v.size() = rows * cols;
template <typename Int>
Matrix<Int> vector2matrix(std::vector<Int>& v, size_t rows)
{
    Matrix<Int> m{rows, v.size()/rows};

    std::copy(v.begin(), v.end(), m.begin());

    return m;
}



/***************************************************************************
 *			    LECTURA/ESCRITURA
 ***************************************************************************/
// El operator func sirve para si se trata de una matriz de vectores imprimir
// solo ciertos campos. 
// Ejemplo: en una imagen solo quiero ver el color red, o la intensidad.
template <typename T, typename F>
// requires: Contenedor_bidimensional(Container2D)
// F: opera sobre los elementos de la matrix T: F(x) con T x;
inline std::ostream& print(std::ostream& out, const Matrix<T>& m, F func)
{
    using size_type = typename Matrix<T>::Ind;

    for (size_type i = 0; i < m.rows(); ++i){
	for (size_type j = 0; j < m.cols(); ++j)
	    out << func(m(i,j)) << ' ';

	out << '\n';
    }

    return out;
}


template <typename T>
inline std::ostream& print(std::ostream& out, const Matrix<T>& m)
{
    using V = Value_type<Matrix<T>>;
    return print(out, m, Identidad<V>{});
}

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Matrix<T>& m)
{ return print(out, m); }



// Versiones para grabar en un fichero
template <typename T, typename F>
// F: opera sobre los elementos de la matrix T: F(x) con T x;
inline void print(const std::string& nom_fichero, const Matrix<T>& m,
                    F func)
{
    std::ofstream out{nom_fichero};
    if (!out)
	throw File_cant_write(nom_fichero);

    print(out, m, func);
}

template <typename T>
inline void print(const std::string& fname, const Matrix<T>& m)
{
    std::ofstream out{fname};
    if (!out)
	throw File_cant_write(fname);

    print(out, m);
}





/// Lee una matriz de Ints desde un flujo.
// El problema es que cuando vamos a leer una matriz de un flujo a priori
// no conocemos sus dimensiones. 
// Solución 1: cargar todo el fichero en un vector calculando el número de
// filas y columnas y luego convertirlo en matriz.
// Por este mismo motivo no podemos definirlo como:
//	    read(in, matrix); // de qué dimensiones matrix??? 
template <typename Int>
Matrix<Int> read_matrix(std::istream& in)
{
    std::vector<Int> file;
    
    std::size_t rows = 0;

    std::string line;
    while (std::getline(in, line)){
	++rows;
	std::istringstream str{line};
	Int tmp;
	while (str >> tmp)
	    file.push_back(tmp);
    }

    return vector2matrix(file, rows);
}




/// Lee una matriz de Ints desde un fichero.
template <typename Int>
inline Matrix<Int> read_matrix(const std::string& nom_fichero)
{
    std::ifstream in{nom_fichero};
    if (!in)
	throw File_cant_read{nom_fichero};

    return read_matrix<Int>(in);
}
	
/*!
 *  \brief  Calcula la matriz con las diferencias.
 *
 *  Se limita a llamar a std::adjacent_difference para cada fila 
 *  guardando el resultado en Output_row_iterator.
 *
 *  \return Devuelve un iterador a la última fila + 1 escrita.
 *
 */
// TODO: cuando haya concepts se puede llamar simplemente adjacent_difference
// sin el 2D!!!
template <typename Input_row_iterator, typename Output_row_iterator>
Output_row_iterator adjacent_difference2D(Input_row_iterator f0,
                                          Input_row_iterator fe,
                                          Output_row_iterator g0)
{
    for (; f0 != fe; ++f0, ++g0)
	std::adjacent_difference(f0->begin(), f0->end(), g0->begin());

    return g0;
}


/****************************************************************************
 *			ALGORITMOS PARA ITERAR (for_each)
 ***************************************************************************/
/*!
 *  \brief  Iteramos por cada elemento de la imagen.
 *
 *  Esta versión usa iteradores.
 *
 *  Ejemplo:
 *  \code
 *	for_each(imgv,
 *		[&](auto& p) {
 *		    visor.draw_local(imgv.posicion(p), colores_[p->r]);
 *		});
    \endcode
 *
 */
template <typename Img, typename Funcion>
void for_each(Img& img1, Funcion func)
{
    for (auto f = img1.row_begin(); f != img1.row_end(); ++f)
	for (auto p = f->begin(); p != f->end(); ++p)
	    func(p);
}


/*!
 *  \brief  Iteramos por cada elemento de la imagen.
 *
 *  Esta versión usa índices.
 *
 *  Ejemplo:
 *  \code
 *	for_each_ij(imgv,
 *		[&](auto i, auto j) {
 *		    visor.draw_local({i,j}, color(mapa.zona(i,j)));
 *		});
    \endcode
 *
 */
template <typename Img, typename Funcion>
void for_each_ij(Img& img1, Funcion func)
{
    using Ind = typename Img::Ind;
    for(Ind i = Ind{0}; i < img1.rows(); ++i)
	for(Ind j = Ind{0}; j < img1.cols(); ++j)
	    func(i,j);
}




/*!
 *  \brief Iteramos por la imagen ejecutando func. 
 *
 *
 *  EJEMPLO:
 *	for_each1D_adelante(visor.imgv0(), 
 *		[&](auto& p1, auto& p2) {
 *		    if (son_distinguibles(*p1, *p2))
 *			visor.draw_local(p1.posicion(), ColorRGB::rojo());
 *		});
 *
 */
template<typename Img, typename Funcion>
void for_each1D_adelante(const Img& img1, Funcion func)
{
    alp::precondicion(img1.cols() >= 2
	    , __FILE__, __LINE__, "for_each1D_adelante"
	    , "Imagen con menos de 2 columnas");

    auto f = img1.row_begin();

    for(; f != img1.row_end(); ++f){
	auto p1 = (*f).begin();
	auto p2 = std::next(p1);

	for(; p2 != (*f).end(); p1 = p2, ++p2)
	    func(p1, p2);

    }

}



/*!
 *  \brief Iteramos por la imagen ejecutando func. 
 *
 *
 *  EJEMPLO:
 *        for_each2D_adelante(visor.imgv0(), 
 *		[&](auto& p1, auto& p2, auto& q1) {
 *		if (   son_distinguibles(*p1, *p2)
 *		    or son_distinguibles(*p1, *q1))
 *		    visor.draw_local(p1.posicion(), ColorRGB::rojo());
 *        });
 *
 */
template<typename Img, typename Funcion>
void for_each2D_adelante(const Img& img1, Funcion func)
{
    alp::precondicion(img1.rows() >= 2 and img1.cols() >= 2
	    , __FILE__, __LINE__, "for_each2D_adelante"
	    , "Imagen con menos de 2 filas ó columnas");


    auto f1 = img1.row_begin();
    auto f2 = std::next(f1);

    for(; f2 != img1.row_end(); f1 = f2, ++f2){
	auto p1 = (*f1).begin(); auto p2 = std::next(p1);
	auto q1 = (*f2).begin();
    
	for(; p2 != (*f1).end(); p1 = p2, ++p2, ++q1)
	    func(p1, p2, q1);
    }
}



/*!
 *  \brief Iteramos por la imagen ejecutando func. 
 *
 *
 *  EJEMPLO:
 *
 *        for_each2D_diagonal(visor.imgv0(), 
 *		[&](auto& p1, auto& q0, auto& q2) {
 *		if (   son_distinguibles(*p1, *q0)
 *		    or son_distinguibles(*p1, *q2))
 *		    visor.draw_local(p1.posicion(), ColorRGB::rojo());
 *        });
 *
 */
template<typename Img, typename Funcion>
void for_each2D_diagonal(const Img& img1, Funcion func)
{
    alp::precondicion(img1.rows() >= 2 and img1.cols() >= 3
	    , __FILE__, __LINE__, "for_each2D_diagonal"
	    , "Imagen con menos de 2 filas ó 3 columnas");


    auto f1 = img1.row_begin();
    auto f2 = std::next(f1);

    for(; f2 != img1.row_end(); f1 = f2, ++f2){
	auto p1 = (*f1).begin(); ++p1;
	auto q0 = (*f2).begin(); auto q2 = std::next(q0, 2);
    
	for(; q2 != (*f2).end(); ++p1, ++q0, ++q2)
	    func(p1, q0, q2);
    }
}



/*!
 *  \brief Iteramos por la imagen ejecutando func. 
 *
 *
 *  EJEMPLO:
 *        for_each2D_alrededor(visor.imgv0(), 
 *		[&](auto& o1, auto& p0, auto& p1, auto& p2, auto& q1){
 *		if (   son_distinguibles(*p1, *o1)
 *		    or son_distinguibles(*p1, *p0)
 *		    or son_distinguibles(*p1, *p2)
 *		    or son_distinguibles(*p1, *q1))
 *		    visor.draw_local(p1.posicion(), ColorRGB::rojo());
 *        });
 *
 */
template<typename Img, typename Funcion>
void for_each2D_alrededor(const Img& img1, Funcion func)
{
    alp::precondicion(img1.rows() >= 3 and img1.cols() >= 3
	    , __FILE__, __LINE__, "for_each2D_alrededor"
	    , "Imagen con menos de 3 filas ó 3 columnas");


    auto f0 = img1.row_begin();
    auto f1 = std::next(f0);
    auto f2 = std::next(f1);

    for(; f2 != img1.row_end(); f0 = f1, f1 = f2, ++f2){
	auto o1 = std::next((*f0).begin()); 
	auto p0 = (*f1).begin(); 
		auto p1 = std::next(p0); auto p2 = std::next(p1);
	auto q1 = std::next((*f2).begin()); 
    
	for(; p2 != (*f1).end(); ++o1, p0 = p1, p1 = p2, ++p2, ++q1)
	    func(o1, p0, p1, p2, q1);
    }
}



/*!
 *  \brief Iteramos por la imagen ejecutando func. 
 *
 *
 *  EJEMPLO:
 *        for_each2D_alrededor_diagonal(visor.imgv0(), 
 *		[&](auto& p1, auto& p2, auto& q0, auto& q1, auto& q2) {
 *		if (   son_distinguibles(*p1, *p2)
 *		    or son_distinguibles(*p1, *q0)
 *		    or son_distinguibles(*p1, *q1)
 *		    or son_distinguibles(*p1, *q2))
 *		    visor.draw_local(p1.posicion(), ColorRGB::rojo());
 *        });
 *
 */
template<typename Img, typename Funcion>
void for_each2D_alrededor_diagonal(const Img& img1, Funcion func)
{
    alp::precondicion(img1.rows() >= 2 and img1.cols() >= 3
	    , __FILE__, __LINE__, "for_each2D_alrededor_diagonal"
	    , "Imagen con menos de 2 filas ó 3 columnas");


    auto f1 = img1.row_begin();
    auto f2 = std::next(f1);

    for(; f2 != img1.row_end(); f1 = f2, ++f2){
	auto p1 = (*f1).begin(); auto p2 = std::next(p1);
	auto q0 = (*f2).begin(); auto q1 = std::next(q0); 
						    auto q2 = std::next(q0);
    
	for(; q2 != (*f2).end(); ++p1, ++p2, ++q0, ++q1, ++q2)
	    func(p1, p2, q0, q1, q2);
    }
}


}// namespace





#endif


