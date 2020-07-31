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

#ifndef __ALP_GEO_TYPES_H__
#define __ALP_GEO_TYPES_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Tipos geométricos
 *
 *   - COMENTARIOS: 
 *	Puntos en 2D: (realmente son los vectores posición de los puntos)
 *	    (vamos a operar con ellos)
 *	    + Posicion_ij: posición dentro de una matriz (i,j)
 *	    + Punto_xy: punto en cartesianas (x,y)
 *	    + Punto_rt: punto en polares (r, theta)
 *
 *	    ¡¡¡RECORDAR QUE REALMENTE SON VECTORES!!!
 *
 *	Puntos en 3D:
 *	    + Punto_xyz: en cartesianas (x,y,z)
 *	    + Punto_rtz: en cilindricas (r, theta, z)
 *	    + Punto_rtf: en esféricas (r, theta, fi)
 *
 *	Rectángulos en 2D:
 *	    + Rectangulo_ij: en (i,j)
 *	    + Rectangulo_xy: en (x,y)
 *
 *	Todo siempre lo vamos a referir al sistema de referencia humano:
 *	horizontal y vertical, a la derecha, izquierda, arriba y abajo.
 *	Este sistema es el natural para nosotros, por eso todas las funciones
 *	lo usan. Además, permiten desvincular mucho código del sistema de
 *	referencia que estemos usando.
 *
 *   - CUIDADO: Aunque los puntos están parametrizados por Int, Int es 
 *	signed. Usar siempre signed. Si se quiere usar size_t (que en
 *	principio parecería ser lo más lógico para matrices) hay que revisar
 *	el código.
 *
 *   - HISTORIA:
 *    A.Manuel L.Perez
 *	14/10/2017 Reescrito
 *	22/02/2019 Generalizo Rectangulo y notación.
 *	31/07/2020 Reestructurado. TODO: Borrar todo lo comentado: sobra.
 *
 ****************************************************************************/

#include <iostream>


namespace alp{
/*!
 *  \brief  Vector en coordenadas locales.
 *
 *  Sistemas de referencias que podemos tener:
 *	1.- Sistema local, asociado al objeto.
 *	2.- Sistema absoluto (sería el sistema de referencia universal de
 *	    Newton).
 *	3.- Diferentes sistemas de referencias.
 *
 *  En principio está pensado para ser manejado en imágenes, de ahí que por
 *  defecto Int = int.
 *
 *  CUIDADO: el sistema de referencia xy es un sistema de ejes cartesiano,
 *  perpendiculares, con el eje x hacia la derecha y el eje y hacia arriba.
 *  Esto es fundamental para coordinarlo con el sistema matricial ij que es el
 *  sistema donde j va hacia la derecha e i hacia abajo.
 *
 */
template <typename Int = int>
struct Vector_xy{
// Types
    using Ind = Int;	// tipo de índice usado

// Data
    Ind x;
    Ind y;

// Constructors
    constexpr Vector_xy() : x{}, y{} {}
    constexpr Vector_xy(Ind x0, Ind y0) : x{x0}, y{y0} {}

// Operations: estructura de espacio vectorial
    constexpr Vector_xy& operator+=(const Vector_xy& b);
    constexpr Vector_xy& operator-=(const Vector_xy& b);
    constexpr Vector_xy& operator*=(const Ind& a);
    constexpr Vector_xy& operator/=(const Ind& a);
};


// Algebraic structure
template <typename I>
inline constexpr 
Vector_xy<I>& Vector_xy<I>::operator+=(const Vector_xy<I>& b)
{
    x += b.x;
    y += b.y;

    return *this;
}

template <typename I>
inline constexpr 
Vector_xy<I>& Vector_xy<I>::operator-=(const Vector_xy<I>& b)
{
    x -= b.x;
    y -= b.y;

    return *this;
}

template <typename I>
inline constexpr 
Vector_xy<I>& Vector_xy<I>::operator*=(const Ind& a)
{
    x *= a;
    y *= a;

    return *this;
}

template <typename I>
inline constexpr 
Vector_xy<I>& Vector_xy<I>::operator/=(const Ind& a)
{
    x /= a;
    y /= a;

    return *this;
}

template <typename I>
inline constexpr Vector_xy<I> operator+(Vector_xy<I> a,
                                            const Vector_xy<I>& b)
{ return a+=b; }

template <typename I>
inline constexpr Vector_xy<I> operator-(Vector_xy<I> a,
                                            const Vector_xy<I>& b)
{ return a-=b; }

template <typename I>
inline constexpr 
Vector_xy<I> operator*(const typename Vector_xy<I>::Ind a, 
                                            Vector_xy<I> b)
{ return b *= a; }

template <typename I>
inline constexpr Vector_xy<I> operator*(Vector_xy<I> b,
			       const typename Vector_xy<I>::Ind a)
{ return a*b;}


template <typename I>
inline constexpr Vector_xy<I> operator/(Vector_xy<I> b,
			       const typename Vector_xy<I>::Ind a)
{ return b /= a; }

// Equality
template <typename I>
inline constexpr bool operator==(const Vector_xy<I>& a, const Vector_xy<I>& b)
{ return (a.x == b.x) and (a.y == b.y); }

template <typename I>
inline constexpr bool operator!=(const Vector_xy<I>& a, const Vector_xy<I>& b)
{ return !(a == b);}



// Iostreams
template <typename I>
std::ostream& operator<<(std::ostream& out, const Vector_xy<I>& p)
{
    return out << '(' << p.x << ", " << p.y << ") local";
}


template <typename Int>
using Punto_xy = Vector_xy<Int>;

///*!
// *  \brief  Punto en coordenadas xy
// *
// *  Es un vector dado en cartesianas.
// *
// *  TODO: fusionarlo con Point_xy_local de rframes.
// *
// */
//template <typename Int>
//// requires: is_integral(Int) and is_signed(Int)
//struct Punto_xy{
//    Int x;
//    Int y;
//	
//    constexpr Punto_xy(Int x0, Int y0):x{x0},y{y0}{}
//
//    Punto_xy& operator+=(const Punto_xy&);
//    Punto_xy& operator-=(const Punto_xy&);
//};
//
//
///***************************************************************************
// *		     IMPLENTACIÓN FUNCIONES INLINE
// ***************************************************************************/
//template <typename Int>
//inline Punto_xy<Int>& Punto_xy<Int>::operator+=(const Punto_xy<Int>& p)
//{x+= p.x; y+= p.y; return *this;}
//
//template <typename Int>
//inline Punto_xy<Int>& Punto_xy<Int>::operator-=(const Punto_xy<Int>& p)
//{x-= p.x; y-= p.y; return *this;}
//
//template <typename Int>
//inline Punto_xy<Int> operator+(Punto_xy<Int> a, Punto_xy<Int> b)
//{return a+=b;}
//
//template <typename Int>
//inline Punto_xy<Int> operator-(Punto_xy<Int> a, Punto_xy<Int> b)
//{return a-=b;}
//
//
//template <typename Int>
//inline bool operator==(Punto_xy<Int> a, Punto_xy<Int> b)
//{ return ((a.x == b.x) && (a.y == b.y)); }
//
//template <typename Int>
//inline bool operator!=(Punto_xy<Int> a, Punto_xy<Int> b)
//{return !(a == b);}
//
//
//template <typename Int>
//inline std::ostream& operator<<(std::ostream& out, const Punto_xy<Int>& p)
//{
//    out << '(' << p.x << ", " << p.y << ')';
//    return out;
//}
//
//
////template <typename N>
////inline bool punto_es_menor_que(const Punto_xy<N>& p1, const Punto_xy<N>& p2)
////{
////    if(p1.x < p2.x) return true;
////
////    if((p1.x == p2.x) and (p1.y < p2.y)) return true;
////
////    return false;
////}
//
//// Esta función se usa en vis_rayo_color.h. TODO: eliminarlo.
//template <typename N>
//inline constexpr Punto_xy<N> grupo(Punto_xy<N> p, int a)
//{ return Punto_xy<N>{alp::cociente(p.x, a), alp::cociente(p.y, a)};}
//

/***************************************************************************
 *				    SIZE2D
 ***************************************************************************/
///// Size que define un rectángulo cuando usamos coordenadas (i,j)
//template <typename Int>
//struct Size_ij 
//{
//    using Ind = Int;
//    Int rows, cols;
//};

///// Size que define un rectángulo cuando usamos coordenadas (x,y)
//template <typename Int>
//struct Size_xy 
//{
//    using Ind = Int;
//    Int ancho, alto;
//};
//
//
///// Size genérico. 
///// Para poder desvincular la implementación del rectángulo del tipo de 
///// coordenada usada necesito una clase genérica Size que use rectángulo
///// La función to_cast se encargará de devolver el tipo Size adecuado
//template <typename Int>
//struct Size2D {Int base, altura;};
//
//template <typename Int>
//constexpr Size_ij<Int> Size2D_to_Size_ij(const Size2D<Int>& sz)
//{ return {sz.altura, sz.base}; }
//
//template <typename Int>
//constexpr Size_xy<Int> Size2D_to_Size_xy(const Size2D<Int>& sz)
//{ return {sz.base, sz.altura}; }
//
//// Generalizamos las funciones de conversión para poder elegirlas de 
//// forma general (las usamos asi dentro de Rectangulo)
//template<typename Size>
//inline auto Size2D_to(const Size2D<typename Size::Ind>& sz)
//{
//    if constexpr (std::is_same_v<Size, Size_ij<typename Size::Ind>>)
//	return Size2D_to_Size_ij(sz);
//
//    else if constexpr (std::is_same_v<Size, Size_xy<typename Size::Ind>>)
//	return Size2D_to_Size_xy(sz);
//
//}

/***************************************************************************
 *			    COORDENADAS TRAITS
 ***************************************************************************/
//template <typename Punto>
//struct Coordenada_traits{
//    using Size = typename Punto::Size;
//};
//
//
///// Tipos que usamos cuando manejamos coordenadas (i, j)
//// TODO: eliminarlas al eliminar Rectangulo
//template <typename Int>
//struct Coordenada_traits<Vector_ij<Int>>{
//    using Punto= Vector_ij<Int>;
//    using Size = Size_ij<Int>;
//};

///// Tipos que usamos cuando manejamos coordenadas (i, j)
//template <typename Int>
//struct Coordenada_traits<Point_ij_local<Int>>{
//    using Punto= Point_ij_local<Int>;
//    using Size = Size_ij<Int>;
//};
     



/***************************************************************************
 *			    RECTÁNGULOS
 ***************************************************************************/



/*!
 *  \brief  Es un rectángulo vulgar y corriente de posiciones.
 *
 *  Al hablar voy a mirar el rectángulo como si estuviera en una hoja de papel
 *  delante de mi. El lado superior es el lado del rectángulo que veo más
 *  arriba, y el lado izquierdo es el que veo más a la izquierda.
 *
 *  Esto es, uso coordenadas humanas (horizontal, vertical; derecha,
 *  izquierda, arriba, abajo).
 *  
 *  Un rectángulo lo podemos caracterizar de varias formas:
 *  // Revisión: realmente esto es un Rango_ij, parece ser que al principio
 *  // mezclé conceptos. Reescribir este rectángulo para que sea un rectángulo
 *  // y no tenga nada de rangos!!!
 *	1.- [p0, p1]    (ventaja: es más natural para una persona(para mi)
 *	2.- [p0, pe)    (ventaja: podemos hablar de conjuntos vacíos)
 *	3.- {p0, Size{rows, cols}}	(este es el natural usando matrices)
 *	4.- {p0, Size{ancho, alto}}	(este es el natural, el humano)
 *
 *  Para poder tener libertad de cambiar cómo representar el rectángulo
 *  directamente, oculto la implementación. No doy acceso directo a p0_ ni pe_
 *  para poder cambiarlo en el futuro (de hecho inicialmente usaba p0_, p1_).
 *
 *  Sin embargo, observar que de las 4 descripciones anteriores la 2 no tiene
 *  sentido geométrico: un rectángulo es un polígono de 4 lados con 4
 *  vértices. Lo que caracteriza el rectángulo son los 4 vértices. El pe es
 *  algo totalmente ficticio, no es natural (aunque útil para poder describir
 *  rectángulos 0).
 *
 *  DUDAS: 
 *  Tengo que definir mínimo 2 tipos de rectángulos:
 *
 *		    Rectangulo_ij y Rectangulo_xy.
 *
 *  Puedo evitar hacer 2 clases distintas parametrizando
 *  Rectangulo<Vector_ij>. El problema es que hablamos de forma distinta
 *  dependiendo de si usamos coordenadas (i, j) o (x, y), y esta forma
 *  distinta de hablar complica la implementación. ¿Merece la pena no duplicar
 *  las dos clases? ¿Se puede concebir de forma distinta para que todo sea más
 *  sencillo? (por ejemplo, ¿usando traits de Posicion?)
 *
 */
//template<typename Punto>
//// requires: Integer(Int)
////	     Int = tipo del índice usado en el contenedor bidimensional
//class Rectangulo{
//public:
//    using Ind = typename Punto::Ind;
//    using Size= typename Coordenada_traits<Punto>::Size;
//
//    // Construcción
//    // ------------
//    /// Definimos un rectángulo vacío
//    Rectangulo():p0_{0,0}, pe_{0,0} {}
//
//    /// Creamos un rectángulo de esquina superior izquierda SI, 
//    /// y esquina inferior derecha ID. Ambas esquinas pertenecen al 
//    /// rectángulo.
//    ///
//    /// Precondición: SI = es la esquina superior izquierda;
//    ///	SD = esquina superior derecha.
//    Rectangulo(const Punto& SI, const Punto& ID);
//
//
//    // Características del rectángulo
//    // ------------------------------
//    /// Esquina superior izquierda
//    Punto p0() const {return p0_;}
//
//    /// Esquinar inferior derecha
//    Punto p1() const {return pe_ - Punto{1, 1};}
//
////    /// Esquina p1() + {1, 1}
////    Punto pe() const {return pe_;} <-- esto no es natural, no dar acceso
//
//    /// Definimos la esquina superior izquierda
//    void p0(const Punto& p)  {p0_ = p;}
//
//    /// Definimos la esquina inferior derecha
//    void p1(const Punto& p)  {pe_ = p + Punto{1, 1};}
//
////    /// Definimos p1() + {1, 1}
////    void pe(const Punto& p)  {pe_ = p;} <-- esto no es natural, no dar acceso
//    /// Vértice superior izquierdo
//    Punto SI() const {return p0_;}
//
//    /// Vértice superior derecho
//    Punto SD() const
//    { return p0_ + a_la_dcha_de(Punto{0, 0}, num_pasos_h(p0_, pe_) - Ind{1}); }
//    //{ return a_la_dcha_de(p0, num_pasos_h(p0_, pe_) - Ind{1}); }
//
//    /// Vértice inferior izquierdo
//    Punto II() const
//    { return p0_ + abajo_de(Punto{0, 0}, num_pasos_v(p0_, pe_) - Ind{1}); }
//    //{ return abajo_de(p0, num_pasos_v(p0_, pe_) - Ind{1}); }
//
//    /// Vértice inferior derecho
//    Punto ID() const {return p1();}
//
//
//    /// ¿Es el rectángulo nulo?
//    bool empty() const {return p0_ == pe_;}
//
//    /// Ancho del rectángulo (el ancho es la dimensión horizontal).
//    Ind ancho() const {return longitud_segmento_h(p0(), p1());}
//
//    /// Alto del rectángulo (el alto es la dimensión vertical).
//    Ind alto() const {return longitud_segmento_v(p0(), p1());}
//
//    /// Número de filas del rectángulo.
//    Ind rows() const {return alto();}
//
//    /// Número de columnas del rectángulo.
//    Ind cols() const {return ancho();}
//
//    /// Devuelve el size del rectángulo
//    Size size() const { return Size2D_to<Size>(Size2D<Ind>{ancho(), alto()}); }
//
//    // Funciones de movimiento
//    // -----------------------
//    /// Amplia el rectángulo hasta incluir la posición p.
////    void amplia(const Punto& p);
//
//    /// Traslada el rectángulo. 
//    /// Recordar que Punto es sinónimo de Vector, luego incr_p es el 
//    /// vector de traslación que queremos mover
////    void traslada(const Punto& incr_p);
//
//    // Relaciones de pertenencia
//    // -------------------------
//    /// Indica si el rectángulo contiene al punto p.
//    /// Que lo contenga quiere decir que está en el interior o es uno de sus
//    /// bordes. Esto es, que no es un punto exterior.
////    bool contiene_a(const Punto& p) const;
////
////    /// Indica si está contenido (dentro) del rectángulo r
////    bool esta_dentro_de(const Rectangulo& r) const;
//
//
//    // Funciones de ayuda
//    // ------------------
//    /// Devuelve los extremos del rectángulo [p0, p1] ordenados
//    /// Hace que p0 = esquina superior izquierda, y p1 = esquina inferior
//    /// derecha
//    static std::pair<Punto, Punto> ordena_extremos(Punto p0, Punto p1);
//
//
//protected:
//    // ¿cómo implementar el rectángulo? Un rectángulo es un conjunto de
//    // puntos, y como tal debería de haber rectángulos conjunto vacío!!!
//    // Como no tengo claro si usar {p0, p1} ó {p0, pe} para implementarlo,
//    // de hecho creo que es mejor a día de hoy usar {p0, pe} aunque
//    // inicialmente usaba {p0, p1}, oculto la implementación para poder
//    // cambiarla en el futuro.
//    Punto p0_;  // Esquina superior izquierda
//    Punto pe_;  // Esquina inferior derecha + {1, 1}
//};
//
//
//template<typename Punto>
//inline Rectangulo<Punto>::Rectangulo(const Punto& SI, const Punto& ID)
//{
//    Punto p1;
//    std::tie(p0_, p1) = ordena_extremos(SI, ID);
//    pe_ = p1 + Punto{1, 1};
//}
//
//template <typename Punto>
//inline bool operator==(const Rectangulo<Punto>& a, const Rectangulo<Punto>& b)
//{ return a.p0() == b.p0() and a.p1() == b.p1(); }
//
//// amplia el rectángulo hasta incluir el pixel p
////template<typename Punto>
////inline void Rectangulo<Int>::amplia(const Punto& p)
////{
////    if(p.i < p0_.i) p0_.i = p.i;
////    if(p.j < p0_.j) p0_.j = p.j;
////    if(p.i > p1_.i) p1_.i = p.i;
////    if(p.j > p1_.j) p1_.j = p.j;
////}
//
//
////template<typename Punto>
////inline void Rectangulo<Punto>::traslada(const Punto& incr_p)
////{
////    p0_ += incr_p;
////    pe_ += incr_p;
////}
//
//// Indica si el punto p está contenido dentro del rectángulo.
//// Ejemplo: Rectangulo r; Punto p;
////	    if (r.contiene_a(p)) ...
////template <typename Punto>
////inline bool Rectangulo<Punto>::contiene_a(const Punto& p) const
////{
////    return  p.esta_a_la_dcha_de(p0()) 
////	    and p.esta_a_la_izda_de(p1())
////	    and p.esta_debajo_de(p0())
////	    and p.esta_encima_de(p1());
////}
////
////
////// Indica si está contenido (dentro) del rectángulo r
////template<typename Punto>
////inline bool Rectangulo<Punto>::esta_dentro_de(const Rectangulo<Punto>& r) const
////{
////    return r.p0().esta_a_la_izda_de(p0()) 
////	and r.p1().esta_a_la_dcha_de(p1())
////	and r.p0().esta_encima_de(p0())
////	and r.p1().esta_debajo_de(p1());
////}
//
//
//// Devuelve los extremos del rectángulo [p0, p1] ordenados
//// Hace que p0 = esquina superior izquierda, y p1 = esquina inferior derecha
//template<typename Punto>
//inline std::pair<Punto, Punto>
//    Rectangulo<Punto>::ordena_extremos(Punto p0, Punto p1)
//{
//    if (p0.esta_a_la_dcha_de(p1))
//	swap_h(p0, p1);
//
//    if (p0.esta_debajo_de(p1))
//	swap_v(p0, p1);
//
//    return {p0, p1};
//}
//
///// ¿p pertenece a R?
//template <typename Punto>
//inline bool pertenece(const Punto& p, const Rectangulo<Punto>& r)
//{
//    return r.contiene_a(p);
//}


/*!
 *  \brief  Rectángulo que se mueve sobre un terreno (otro rectángulo que
 *	lo contiene).
 *
 *	Todas las funciones garantizan que el rectángulo nunca se sale
 *	del terreno. En caso de que se intente salir, el rectángulo se mueve
 *	todo lo que puede, quedándose al borde, pero nunca se sale.
 */
//template <typename Punto>
//// requires: Integer(Int)
////	     Int = tipo del índice usado en el contenedor bidimensional
//class Rectangulo_acotado:public Rectangulo<Punto>{
//public:
//    using Ind = typename Punto::Ind;
//    
//    // construcción
//    // ------------
//    /// Crea un rectángulo vacío
//    Rectangulo_acotado() {}
//
//    /// Crea un rectangulo acotado 'r' dentro del 'terreno' indicado.
//    /// Es el rectángulo r acotado por el terreno (de aquí que pase primero
//    /// r y luego terreno).
//    Rectangulo_acotado(const Rectangulo<Punto>& r,
//		     const Rectangulo<Punto>& terreno)
//	:Rectangulo<Punto>{r}, t_{terreno}
//    {}
//
//
//    // Características
//    // ---------------
//    /// Definimos el terreno. Inicializamos el rectangulo a 0.
//    void terreno(const Rectangulo<Punto>& terreno)
//    {
//      t_ = terreno;
//
//      Rectangulo<Punto>::p0_ = Punto{0, 0};
//      Rectangulo<Punto>::pe_ = Punto{0, 0};
//    }
//
//    /// Terreno donde está acotado el rectángulo
//    const Rectangulo<Punto>& terreno() {return t_;}
//
//    /// Modificamos el rectángulo. No tocamos el terreno.
//    /// precondición: r es interior al terreno
//    void extension(const Rectangulo<Punto>& r)
//    {
//	// Genera un error de compilación si uso r.p0_/pe_. 
//	// ¿es problema con herencias y templates? ¿O no entiendo algo?
//	Rectangulo<Punto>::p0_ = r.p0();
//	Rectangulo<Punto>::pe_ = r.p1() + Punto{1, 1};
//    }
//
//    /// Devuelve la extensión que ocupa el rectángulo.
//    /// Al devolver un rectángulo, no solo indica el tamaño (rows x cols)
//    /// sino que tambien indica la posición que ocupa el rectángulo dentro
//    /// del terreno.
//    Rectangulo<Punto> extension() const 
//    {return {p0(), p1()};}
//
//    // DUDA: ¿por qué no hereda estas funciones? Creo que tiene que ver
//    // con la herencia y las templates que no iba bien (???)
//    Punto p0() const {return Rectangulo<Punto>::p0();}
//    Punto p1() const {return Rectangulo<Punto>::p1();}
//
//    void p0(const Punto& p) {Rectangulo<Punto>::p0(p);}
//    void p1(const Punto& p) {Rectangulo<Punto>::p1(p);}
//
//    // operaciones 
//    // -----------
//    /// Mueve el rectángulo x pixeles a la derecha.
//    /// Precondición: x >= 0
//    void muevete_a_la_dcha(Ind x);
//
//    /// Mueve el rectángulo x pixeles a la izquierda.
//    /// Precondición: x >= 0
//    void muevete_a_la_izda(Ind x);
//
//    /// Mueve el rectángulo x pixeles hacia abajo.
//    /// Precondición: x >= 0
//    void muevete_abajo(Ind x);
//
//    /// Mueve el rectángulo x pixeles hacia arriba.
//    /// Precondición: x >= 0
//    void muevete_arriba(Ind x);
//
//    enum class Direccion{dcha, izda, arriba, abajo};
//
//    /// Mueve el lado derecho x pixeles en la dirección d indicada
//    /// Precondición: d = dcha ó izda
//    // Nota: podríamos usar el signo de x para ir hacia la dcha o la izda
//    // Sin embargo, en general Ind puede ser unsigned, no tiene que haber
//    // x < 0!!!
//    void mueve_lado_dcho(Ind x, Direccion d);
//
//    /// Mueve el lado izquierdo x pixeles en la dirección d indicada
//    /// Precondición: d = dcha ó izda
//    void mueve_lado_izdo(Ind x, Direccion d);
//
//    /// Mueve el lado superior x pixeles en la dirección d indicada
//    /// Precondición: d = arriba ó abajo
//    void mueve_lado_superior(Ind x, Direccion d);
//
//    /// Mueve el lado inferior x pixeles en la dirección d indicada
//    /// Precondición: d = arriba ó abajo
//    void mueve_lado_inferior(Ind x, Direccion d);
//
//
//private:
//    Rectangulo<Punto> t_;  // terreno donde se mueve el rectángulo
//
//
//};
//
//
//
//template <typename Punto>
//void Rectangulo_acotado<Punto>::muevete_a_la_dcha(Ind x)
//{
//    auto d = std::min(num_pasos_h(Rectangulo<Punto>::p1(), t_.p1()), x);
//    Rectangulo<Punto>::traslada(a_la_dcha_de(Punto{0,0}, d));
//}
//
//
//template <typename Punto>
//void Rectangulo_acotado<Punto>::muevete_a_la_izda(Ind x)
//{
//    auto d = std::min(num_pasos_h(Rectangulo<Punto>::p0(), t_.p0()), x);
//    Rectangulo<Punto>::traslada(a_la_izda_de(Punto{0,0}, d));
//}
//
//
//
//template <typename Punto>
//void Rectangulo_acotado<Punto>::muevete_abajo(Ind x)
//{
//    auto d = std::min(num_pasos_v(Rectangulo<Punto>::p1(), t_.p1()) , x);
//    Rectangulo<Punto>::traslada(abajo_de(Punto{0,0}, d));
//}
//
//template <typename Punto>
//void Rectangulo_acotado<Punto>::muevete_arriba(Ind x)
//{
//    auto d = std::min(num_pasos_v(Rectangulo<Punto>::p0(), t_.p0()), x);
//    Rectangulo<Punto>::traslada(arriba_de(Punto{0,0}, d));
//}
//
//
//// Implemento todas estas funciones usando +, no uso - ya que puede que Ind
//// sea unsigned, y al restar podríamos tener underflow!!!
//template <typename Punto>
//void Rectangulo_acotado<Punto>::mueve_lado_dcho(Ind incr_x, Direccion d)
//{
//    if (d == Direccion::dcha){
//	auto p = a_la_dcha_de(p1(), incr_x, t_.p1());
//	std::cout << "vuelta = " << p << '\n';
//	p1(p);
//
//    } else if (d == Direccion::izda){
//	auto p = a_la_izda_de(p1(), incr_x, p0());
//	p1(p);
//    }
//}
//
//
//template <typename Punto>
//void Rectangulo_acotado<Punto>::mueve_lado_izdo(Ind incr_x, Direccion d)
//{
//    if (d == Direccion::dcha){
//	auto p = a_la_dcha_de(p0(), incr_x, p1());
//	p0(p);
//
//    } else if (d == Direccion::izda){
//	auto p = a_la_izda_de(p0(), incr_x, t_.p0());
//	p0(p);
//    }
//}
//
//
//template <typename Punto>
//void Rectangulo_acotado<Punto>::mueve_lado_superior(Ind incr_x, Direccion d)
//{
//    if (d == Direccion::arriba){
//	auto p = arriba_de(p0(), incr_x, t_.p0());
//	p0(p);
//
//    } else if (d == Direccion::abajo){
//	auto p = abajo_de(p0(), incr_x, p1());
//	p0(p);
//    }
//}
//
//
//template <typename Punto>
//void Rectangulo_acotado<Punto>::mueve_lado_inferior(Ind incr_x, Direccion d)
//{
//    if (d == Direccion::arriba){
//	auto p = arriba_de(p1(), incr_x, p0());
//	p1(p);
//
//    } else if (d == Direccion::abajo){
//	auto p = abajo_de(p1(), incr_x, t_.p1());
//	p1(p);
//    }
//}


/*!
 *  \brief  Es un segmento.
 *
 *  Un segmento viene definido por dos puntos [A, B].
 *
 *  Con esta descripción es imposible hablar de segmentos 0 (o vacíos)
 *  ya que el segmento [A, A] está formado por el punto A no siendo vacío.
 *  Por ello, para poder incluir segmentos vacíos se puede implementar un
 *  segmento como [p0, pe). Tenemos dos formas básicas de describir un
 *  segmento:
 *	1.- [p0, p1] = [A, B]
 *	2.- [p0, pe)
 *
 * TODO: modificarlo para que se incluya el segmento vacío. Hacer algo
 * parecido a lo hecho con rectángulo.
 */
template<typename Punto>
class Segmento
{
public:
    Punto A, B;	// extremos del segmento   

    Segmento(const Punto& A0, const Punto& B0):A{A0}, B{B0}{}
};

template<typename Punto>
inline bool operator==(const Segmento<Punto>& a, const Segmento<Punto>& b)
{ return (a.A == b.A) and (a.B == b.B); }

template<typename Punto>
inline bool es_un_punto(const Segmento<Punto>& s)
{ return (s.A == s.B); }

template<typename Punto>
inline std::ostream& operator<<(std::ostream& out, const Segmento<Punto>& s)
{
    out << "[" << s.A << ", " << s.B << "]";

    return out;
}


}// namespace

#endif 
