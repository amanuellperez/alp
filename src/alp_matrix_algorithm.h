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
 *    Manuel Perez
 *       28/09/2017 Escrito
 *       16/03/2020 Reestructurado.
 *       26/05/2020 transform1D/2D.
 *       06/12/2020 transform1D_alrededor
 *       29/07/2022 y_symmetry, rotate_plus90, rotate_minus90
 *	 27/08/2022 h_differences, operator+ (a+b), operator- (a-b)
 *	 28/08/2022 rotate_180
 *
 ****************************************************************************/

#include "alp_matrix.h"
#include "alp_submatrix.h"

#include "alp_math.h"	// punto_medio
#include "alp_type_traits.h"
#include "alp_cast.h"	// narrow_cast 

#include <fstream>
#include <sstream>
#include <iterator>
#include <numeric>

namespace alp{

/*!
 *  \brief  Copia la imagen img0 dentro de la imagen img1.
 *	    Coloca img0 a partir de la posición p0 de img1.
 *
 */
template <typename Container2D>
void copia_dentro  ( const Container2D& m0    // copiamos m0 en
	    , Container2D& m1		     // la imagen m1
	    , typename Container2D::Position p0)// colocando la esquina SI de m1 en p0 
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
template <typename T, typename I>
Matrix<T, I> vector2matrix(const std::vector<T>& v, I rows)
{
    Matrix<T, I> m{rows, narrow_cast<I>(v.size())/rows};

    std::copy(v.begin(), v.end(), m.begin());

    return m;
}



/***************************************************************************
 *			    LECTURA/ESCRITURA
 ***************************************************************************/
// El operator func sirve para si se trata de una matriz de vectores imprimir
// solo ciertos campos. 
// Ejemplo: en una imagen solo quiero ver el color red, o la intensidad.
//
// OJO: tiene que poder funcionar con Matrix y con Matrix_view y con
// Submatrix... Esto es, opera sobre contenedores bidimensionales!!!
template <typename M, typename F>
// requires: Contenedor_bidimensional(M)
// F: opera sobre los elementos de la matrix T: F(x) con T x;
inline std::ostream& print(std::ostream& out, const M& m, F func)
{
    using size_type = typename M::Ind;

    for (size_type i = 0; i < m.rows(); ++i){
	for (size_type j = 0; j < m.cols(); ++j)
	    out << func(m(i,j)) << ' ';

	out << '\n';
    }

    return out;
}


template <typename M>
inline std::ostream& print(std::ostream& out, const M& m)
{
    using V = Value_type<M>;
    return print(out, m, Identidad<V>{});
}



// Versiones para grabar en un fichero
template <typename M, typename F>
// F: opera sobre los elementos de la matrix T: F(x) con T x;
inline void print(const std::string& nom_fichero, const M& m,
                    F func)
{
    std::ofstream out{nom_fichero};
    if (!out)
	throw File_cant_write(nom_fichero);

    print(out, m, func);
}

template <typename M>
inline void print(const std::string& fname, const M& m)
{
    std::ofstream out{fname};
    if (!out)
	throw File_cant_write(fname);

    print(out, m);
}





/// Lee una matriz de Ints desde un flujo.
/// Lee hasta encontrar una línea vacía o EOF.
// TODO: De momento no es nada robusta la función: da por supuesto que el fichero
//contiene una matriz. No valida que todas las líneas tengan el mismo número
//de columnas, y creo que si metes una línea de palabras en medio simplemente
//la ignora (debería de dar error). 
// El problema es que cuando vamos a leer una matriz de un flujo a priori
// no conocemos sus dimensiones. 
// Solución 1: cargar todo el fichero en un vector calculando el número de
// filas y columnas y luego convertirlo en matriz.
// Por este mismo motivo no podemos definirlo como:
//	    read(in, matrix); // de qué dimensiones matrix???
template <typename T, typename I = size_t>
Matrix<T, I> read_matrix(std::istream& in)
{
    std::vector<T> file;
    
    std::size_t rows = 0;

    std::string line;
    while (std::getline(in, line)){
	if (line.empty())
	    break;

	++rows;
	std::istringstream str{line};
	T tmp;
	while (str >> tmp)
	    file.push_back(tmp);
    }

    return vector2matrix<T,I>(file, rows);
}




/// Lee una matriz de Ints desde un fichero.
template <typename T, typename I = size_t>
inline Matrix<T, I> read_matrix(const std::string& nom_fichero)
{
    std::ifstream in{nom_fichero};
    if (!in)
	throw File_cant_read{nom_fichero};

    return read_matrix<T, I>(in);
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


// Devuelve la matriz de diferencias horizontales.
// Ej:
//	x = 100 90 70       y = -10 -20
//	     20 30 30  -->      +10  0
//
// Sería mas "standard" llamarlo adjacent_difference, pero no me gusta el
// nombre. Además, en matrices siempre tengo dos tipos de diferencias:
// horizontales y verticales (h_differences and v_differences)
template <typename T, typename I>
Matrix<T,I> h_differences(const Matrix<T,I>& x)
{
    if (x.cols() < 2)
	return x;

    Matrix<T, I> y{x.rows(), x.cols() - 1};

    auto f = x.row_begin();
    auto g = y.row_begin();
    
    for (; f != x.row_end(); ++f, ++g){
	auto p0 = f->begin();
	auto p1 = std::next(p0);
	auto q = g->begin();

	for (; p1 != f->end(); ++p0, ++p1, ++q)
	    *q = ((*p1) - (*p0));
    }

    return y;
}


template <typename T, typename I>
Matrix<T,I> operator+(const Matrix<T,I>& a, const Matrix<T,I>& b)
{
    if (a.size2D() != b.size2D())
	throw std::logic_error{"Can't add matrix of different size"};

    Matrix<T, I> res{a.rows(), a.cols()};

    auto pa = a.begin();
    auto pb = b.begin();
    auto q  = res.begin();

    while (pa != a.end()){
	*q = (*pa + *pb); //  == (*q++ = *pa++ + *pb++) ??? @_@ xD
	++pa;
	++pb;
	++q;
    }

    return res;
}


template <typename T, typename I>
Matrix<T,I> operator-(const Matrix<T,I>& a, const Matrix<T,I>& b)
{
    if (a.size2D() != b.size2D())
	throw std::logic_error{"Can't subtract matrix of different size"};

    Matrix<T, I> res{a.rows(), a.cols()};

    auto pa = a.begin();
    auto pb = b.begin();
    auto q  = res.begin();

    while (pa != a.end()){
	*q = (*pa - *pb); //  == (*q++ = *pa++ + *pb++) ??? @_@ xD
	++pa;
	++pb;
	++q;
    }

    return res;
}


// Devuelve la matriz simétrica a m0 , respecto del eje y
template <typename T, typename I>
Matrix<T,I> y_symmetry(const Matrix<T,I>& m)
{
    using Ind = Matrix<T, I>::Ind;

    Matrix<T,I> res{m.rows(), m.cols()};

    for(Ind i = 0; i < m.rows(); ++i)
	for(Ind j = 0, jp = m.cols()-1; j < m.cols(); ++j, --jp)
	    res(i, j) = m(i,jp);

    return res;
}


// Rota la imagen +90 grados.
//
// Para rotar una matriz: m = rotate_plus90(m);
template <typename T, typename In>
Matrix<T, In> rotate_plus90(const Matrix<T,In>& m)
{
    Matrix<T,In> res{m.cols(), m.rows()};

    using size_t = Matrix<T,In>::Ind;
    for(size_t I = 0; I < m.rows(); ++I)
	for(size_t J = 0, i = res.rows() - 1; J < m.cols(); ++J, --i)
	    res(i, I) = m(I, J);

    return res;
}



// Rota la imagen -90 grados.
//
// Para rotar una imagen: m = rotate_minus90(m);
template <typename T, typename In>
Matrix<T, In> rotate_minus90(const Matrix<T, In>& m)
{
    Matrix<T,In> res{m.cols(), m.rows()};
    
    using size_t = Matrix<T,In>::Ind;
    for(size_t I = 0, j = res.cols() - 1; I < m.rows(); ++I, --j)
	for(size_t J = 0; J < m.cols(); ++J)
	    res(J, j) = m(I, J);

    return res;
}


// Rota la imagen +180 grados. Es equivalente a rotarla dos veces 90 grados.
//
// Para rotar una matriz: m = rotate_180(m);
template <typename T, typename In>
Matrix<T, In> rotate_180(const Matrix<T,In>& m)
{
    using size_t = Matrix<T,In>::Ind;

    Matrix<T,In> res{m.rows(), m.cols()};

    for(size_t I = 0; I < m.rows(); ++I)
	for(size_t J = 0; J < m.cols(); ++J)
	    res(m.rows() - 1 - I, m.cols() - 1 - J) = m(I, J);

    return res;
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

/***************************************************************************
 *			    TRANSFORM
 ***************************************************************************/
// Mismos convenios de notación que para los foreach.
// F = función transformadora. q1 = F(p1, p2); Elige q1 a partir de p1 y p2.
// x0 = valor usado para rellenar 'y' en los puntos que no se sabe cómo
// rellenar.
template <typename I, typename S, typename F, typename V>
alp::Matrix<I,S> transform1D_adelante(const alp::Matrix<I,S>& x, const V& x0, F transf)
{
    alp::Matrix<I,S> y{x.rows(), x.cols()};

    auto f = x.row_begin();
    auto g = y.row_begin();
    for (; f != x.row_end(); ++f, ++g){
	auto p1 = f->begin();
	auto p2 = std::next(p1);
	auto q1 = g->begin();

	for (; p2 != f->end(); ++p1, ++p2, ++q1){
	    *q1 = transf(*p1, *p2);
	}

	*q1 = x0;
    }


    return y;
}


// Mismos convenios de notación que para los foreach.
// F = función transformadora. q2 = F(p1, p2); Elige q2 a partir de p1 y p2.
template <typename I, typename S, typename F>
alp::Matrix<I,S> transform1D_atras(const alp::Matrix<I,S>& x, F transf)
{
    alp::Matrix<I,S> y{x.rows(), x.cols()};

    auto f = x.row_begin();
    auto g = y.row_begin();
    for (; f != x.row_end(); ++f, ++g){
	auto p1 = f->begin();
	auto q2 = g->begin();
	auto p2 = std::next(p1);

	*q2 = *p1;
	++q2;
	for (; p2 != f->end(); ++p1, ++p2, ++q2){
	    *q2 = transf(*p1, *p2);
	}
    }


    return y;
}


// Mismos convenios de notación que para los foreach.
// Transf = función transformadora. y1 = Transf(x0, x1, x2); 
// Además hay que indicar cómo transformar el primer y último elemento:
// y0 = Transf_first(x0, x1);
// y[cols-1] = Transf_last(x[cols-2], x[cols-1]);
template <typename I, typename S,
          typename Transf,
          typename Transf_first, typename Transf_last>
alp::Matrix<I, S> transform1D_alrededor(const alp::Matrix<I, S>& x,
                                        Transf transf,
                                        Transf_first transf_first_element,
                                        Transf_last transf_last_element)
{
    alp::Matrix<I,S> y{x.rows(), x.cols()};

    if (x.cols() < 3)
	return x;

    auto f = x.row_begin();
    auto g = y.row_begin();
    for (; f != x.row_end(); ++f, ++g){
	auto p0 = f->begin();
	auto q0 = g->begin();

	auto p1 = std::next(p0);
	auto p2 = std::next(p1);

	auto q1 = std::next(q0);
	auto q2 = std::next(q1);

	*q0 = transf_first_element(*p0, *p1);
	while(p2 != f->end()){
	    *q1 = transf(*p0, *p1, *p2);

	    ++p0; ++p1; ++p2;
	    ++q0; ++q1; ++q2;
	}

	*q1 = transf_last_element(*p0, *p1);
    }


    return y;
}




// F = función transformadora. q1 = F(xp1, xp2, xq1); <--- cuidado con el orden.
// x0 = valor usado para rellenar 'y' en los puntos que no se sabe cómo
// rellenar.
template <typename I, typename S, typename F, typename V>
alp::Matrix<I,S> transform2D_adelante(const alp::Matrix<I,S>& x, const V& x0, F transf)
{
    alp::Matrix<I,S> y{x.rows(), x.cols()};

    auto f1 = x.row_begin();
    auto f2 = std::next(f1);
    auto g = y.row_begin();

    for (; f2 != x.row_end(); ++f1, ++f2, ++g){
	auto xp1 = f1->begin();
	auto xp2 = std::next(xp1);
	auto xq1 = f2->begin();

	auto yp1 = g->begin();

	for (; xp1 != f1->end(); ++xp1, ++xp2, ++xq1, ++yp1){
	    *yp1 = transf(*xp1, *xp2, *xq1);
	}

	*yp1 = x0;
    }



    return y;
}


// F = función transformadora. 
// q1 = F(xo0, xo1, xo2,
//        xp0, xp1, xp2,
//        xq0, xq1, xq2);
template <typename I, typename S, typename V, typename F>
alp::Matrix<I,S> transform2D_alrededor(const alp::Matrix<I,S>& x, const V& x0, F transf)
{
    alp::precondicion(x.rows() >= 3 and x.cols() >= 3
	    , __FILE__, __LINE__, "transform_alrededor"
	    , "Imagen con menos de 3 filas ó 3 columnas");

    alp::Matrix<I,S> y{x.rows(), x.cols()};

    auto f0 = x.row_begin();
    auto f1 = std::next(f0);
    auto f2 = std::next(f1);
    
    auto g1 = y.row_begin();

    // copiamos la primera fila sin hacer nada
//    std::copy(f0->begin(), f0->end(), g1->begin());
    std::fill(g1->begin(), g1->end(), x0);

    ++g1;
    for(; f2 != x.row_end(); ++f0, ++f1, ++f2, ++g1){
	auto xo0 = f0->begin();
	auto xo1 = std::next(xo0);
	auto xo2 = std::next(xo1);

	auto xp0 = f1->begin();
        auto xp1 = std::next(xp0);
        auto xp2 = std::next(xp1);

	auto xq0 = f2->begin();
	auto xq1 = std::next(xq0);
	auto xq2 = std::next(xq1);
    
	auto yp1 = g1->begin();
	*yp1 = x0;
	++yp1;

	while (xp2 != f1->end()){
	    *yp1 = transf(*xo0, *xo1, *xo2,
		          *xp0, *xp1, *xp2, 
			  *xq0, *xq1, *xq2);

	    ++xo0; ++xo1; ++xo2;
	    ++xp0; ++xp1; ++xp2;
	    ++xq0; ++xq1; ++xq2;

	    ++yp1;

	}

	*yp1 = x0;
    }

    // copiamos la última fila
    // std::copy(f2->begin(), f2->end(), g1->begin());
    std::fill(g1->begin(), g1->end(), x0);

    return y;
}




/*!
 *  \brief  Devuelve los puntos de la imagen que se encuentran alrededor de p.
 *
 *  Ejemplo:
 *  \code
 *		for(auto p: alrededor(img0, {3,4})) ...
 *  \endcode
 *
 */
template <typename I, typename S>
inline std::vector<typename Matrix<I, S>::Position>
alrededor(const Matrix<I, S>& m, const typename Matrix<I, S>::Position& p)
{
    return alrededor(m.extension(), p);
}


}// namespace





#endif


