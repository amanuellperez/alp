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

#ifndef __ALP_MATH_H__
#define __ALP_MATH_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones matemáticas
 *
 *   - HISTORIA:
 *    Manuel Perez
 *      12/03/2017 Escrito
 *      28/05/2020 Aproximado
 *      27/07/2020 Degree/Radian
 *
 ****************************************************************************/

#include <algorithm>
#include <iostream>
#include <cmath>
#include <limits>


namespace alp{

/// Infinito para un tipo Integer
template <typename Int>
Int infinity() noexcept
{return std::numeric_limits<Int>::max();}

/// -Infinito para un tipo Integer
template <typename Int>
Int neg_infinity() noexcept
{return std::numeric_limits<Int>::min();}


// Tengo que redefinir el valor absoluto (definido en std), ya que en std
// no está garantizado que sea constexpr!!!
template<typename T>
constexpr auto abs(T x)
{
    if (x < 0) 
	return -x;

    return x;
}


/*!
 *  \brief  Truco para poder escribir cosas del tipo: ancho(a, maximo(8));
 *
 *  ¿Cómo escribir una función que determine el ancho dándole una
 *  cota superior? 
 *
 *  Podríamos escribir:	ancho_max(a, 8), pero no queda nada claro.
 *
 *  Sin embargo, escribiendo:
 *	    ancho(a, maximo(8)); = pon el ancho a, como máximo 8.
 *  Creo que se lee mejor. Usémoslo y la práctica ya dirá.
 *
 * TODO: usar Nombre?
 */
template<typename T>
struct Maximo{
    using value_type = T;

    value_type valor;

    /// Conversión implícita de value_type a Maximo.
    Maximo(value_type v):valor{v}{}

    /// Conversión implícita de Maximo a value_type.
    operator value_type() const {return valor;}
};

// TODO: hacerlo así
//template <typename Int>
//// requires: Integer(Int)
//enum class Maximo: Int{};

template<typename T>
Maximo<T> maximo(T v) {return Maximo<T>{v};}


/// Indica si n es impar.
inline bool is_odd(int n)   {return n & 0x1;}

/// Indica si n es par.
inline bool is_even(int n)  {return !is_odd(n);}    


/// Devuelve la coordenada del punto medio del intervalo [0, n]
///
/// Si n es impar devuelve el punto medio exacto.
/// Si n es par dividimos [0, n], en dos intervalos de igual longitud
/// [0, n1] union [n1+1, n], devolviendo n1.
template <typename T>
// requires: integral_type(T)
inline T punto_medio(T n) 
{ return (n >> 1);}
//{ return n/2; }


/// Calcula el máximo común divisor.
/// Versión de Stepanov (Elements of programming)
template <typename T>
T max_comun_divisor(T a, T b)
{
    while (true) {
        if (b == T{0})
            return a;

        a = a % b;

        if (a == T{0})
            return b;

        b = b % a;
    }
}


/// Devuelve el mínimo común múltiplo de a y de b.
template <typename T>
inline T min_comun_multiplo(T a, T b)
{ return (a*b)/max_comun_divisor(a,b); }


/// esta_entre(x, a,b) = ¿x pertenece al intervalo cerrado [a,b]?
template <typename Int>
constexpr bool esta_entre(Int x, Int a, Int b)
{ return (a <= x) and (x <= b); }




/// Devuelve el número más próximo a la sucesión 0, m, 2m, 3m, ...
/// EJEMPLO: si x = 17, m = 5, buscamos aproximar x al número más próximo
/// de 15 y 20, en este caso sería 15. Si x = 18, el número más próximo sería
/// 20
//  multiplo_de(m)_mas_proximo_a(x);
inline int multiplo_de_mas_proximo_a(int m, int x)
{
    // x = q*m + r
    int r = x%m;
    int q{x/m};	

    if(2*r > m) return m*(q+1);
    else return m*q;
}


/// Devuelve el múltiplo de m mayor o igual a x.
/// Ejemplo: 
///	    Si m = 4 y x = 16, devuelve 16.
///	    Si m = 4 y x = 18, devuelve 20.
//	    
//  multiplo_de(m)_mayor_que(x)
inline int multiplo_de_mayor_que(int m, int x)
{
    // x = q*m + r
    int r = x%m;
    int q{x/m};	

    if (r == 0)
	return x;

    return (q+1)*m;
}



/****************************************************************************
 *
 *   - FUNCIÓN: cociente/resto
 *
 *   - DESCRIPCIÓN: Calculan el cociente y el resto que resulta de dividir D
 *	    entre d. 
 *
 *	    D puede ser tanto positivo como negativo. Defino el resto:
 *		    
 *		    D = d·c + r, con 0 <= r < d
 *
 *   - COMENTARIOS:
 *	    Sería más eficiente calcular las dos cosas a la vez. Para ello
 *	    crear alguna clase (Division_entera?) y que calcule las 2 cosas 
 *	    juntas.
 *
 *   - PRECONDICIÓN: d > 0
 *
 *   - DUDA: ¿Implementar el caso d < 0?
 *
 *   - EJEMPLO:
 *		int c = cociente(10, 3);
 *		int r = resto(10, 3);
 *
 *  TODO: eliminar estas??? 
 ****************************************************************************/
inline constexpr int cociente(int D, int d)
{
    if(D >= 0)
	return int{D/d};

    // if(D < 0)
    int c{abs(D)/d};
    int r = abs(D)%d;

    if(r == 0) return -c;

    return (-c-1);
}

inline constexpr int resto(int D, int d)
{ 
    if(D >= 0)
	return D%d;

    // if(D < 0)
    int r = abs(D)%d;

    if(r == 0) return 0;

    return d-r;
}




/*****************************************************************************
 * 
 *   - CLASE: Int_en_base4
 *
 *   - DESCRIPCIÓN: Es un número en base 4.
 *
 *   - RANGO:	El número tiene que estar comprendido de [0, 64).
 *  
 *   - EJEMPLO:
 *		Int_en_base4 n{21};
 *		n.u();	// unidades en base 4
 *		n.d();  // decenas 
 *		n.c();	// centenas
 *
 ***************************************************************************/
class Int_en_base4{
public:
    explicit Int_en_base4(int n);
    int u() const {return u_;}
    int d() const {return d_;}
    int c() const {return c_;}

private:
    // n = u_ + 4d_ + 16c_
    int u_	// unidades
	, d_	// decenas
	, c_;	// centenas
};

inline Int_en_base4::Int_en_base4(int n)
{
    u_ = n%4;
    int coc = int(n/4);	// = cociente(n/4);
    d_ = coc%4;
    c_ = int(coc/4);	// = cociente(coc/4);
}

inline std::ostream& operator<<(std::ostream& out, const Int_en_base4& n)
{ return out << n.d() << n.c() << n.u(); }


/****************************************************************************
 *			    MANEJO DE ÁNGULOS
 ****************************************************************************/
// constantes de interés
constexpr double pi {3.1415926535897};

// Conversión de radianes a grados y viceversa
inline constexpr double radian2degree(double r) { return (r*180.0/pi); }
inline constexpr double degree2radian(double g) { return (g*pi/180.0); }

class Radian;

/*!
 *  \brief  Degree
 *
 *  An angle in degrees.
 *
 */
class Degree{
public:
    using Rep = double;

// Construction
    constexpr Degree() : value_{} {}

    constexpr Degree(const Rep& x) : value_{static_cast<Rep>(x)}{}

    // Conversion radians to degrees
    constexpr Degree(const Radian& x);

// Observer
    constexpr Rep value() const { return value_; }

// Algebraic structure
    constexpr Degree operator-();

    constexpr Degree& operator+=(const Degree& x);
    constexpr Degree& operator-=(const Degree& x);

    constexpr Degree& operator*=(const Rep& a);
    constexpr Degree& operator/=(const Rep& a);


private:
    Rep value_;
};

inline constexpr Degree Degree::operator-()
{ return Degree{-value_}; }


inline constexpr Degree& Degree::operator+=(const Degree& x) 
{
    value_ += x.value_;
    return *this;
}

inline constexpr Degree& Degree::operator-=(const Degree& x)
{
    value_ -= x.value_;
    return *this;
}

inline constexpr Degree& Degree::operator*=(const Rep& a)
{
    value_ *= a;
    return *this;
}

inline constexpr Degree& Degree::operator/=(const Rep& a)
{
    value_ /= a;
    return *this;
}


inline constexpr Degree operator+(Degree a, const Degree& b)
{ return a += b; }

inline constexpr Degree operator-(Degree a, const Degree& b)
{ return a -= b; }

inline constexpr Degree operator*(const Degree::Rep& a, Degree b)
{ return b *= a; }

inline constexpr Degree operator*(Degree b, const Degree::Rep& a)
{ return a * b; }

inline constexpr Degree operator/(Degree b, const Degree::Rep& a)
{ return b /= a; }



// Normaliza el ángulo de [0, 360)
Degree normalize(const Degree& a);


// Equality
inline constexpr bool operator==(const Degree& a, const Degree& b)
{ return a.value() == b.value();}

inline constexpr bool operator!=(const Degree& a, const Degree& b)
{ return !(a == b);}

// Order
inline constexpr bool operator<(const Degree& a, const Degree& b)
{ return a.value() < b.value(); }

inline constexpr bool operator<=(const Degree& a, const Degree& b)
{ return a.value() <= b.value(); }

inline constexpr bool operator>(const Degree& a, const Degree& b)
{ return a.value() > b.value(); }

inline constexpr bool operator>=(const Degree& a, const Degree& b)
{ return a.value() >= b.value(); }

// Iostreams
inline std::ostream& operator<<(std::ostream& out, const Degree& a)
{ return out << a.value() << "º"; }

/*!
 *  \brief  Radian
 *
 *  An angle in degrees.
 *
 */
class Radian{
public:
    using Rep = double;

// Construction
    constexpr Radian() : value_{} {}

    constexpr Radian(const Rep& x) : value_{static_cast<Rep>(x)}{}

    // Conversion radians to degrees
    constexpr Radian(const Degree& x);

// Observer
    constexpr Rep value() const { return value_; }

// Algebraic structure
    constexpr Radian operator-();

    constexpr Radian& operator+=(const Radian& x);
    constexpr Radian& operator-=(const Radian& x);

    constexpr Radian& operator*=(const Rep& a);
    constexpr Radian& operator/=(const Rep& a);

private:
    Rep value_;
};

inline constexpr Radian Radian::operator-()
{ return Radian{-value_}; }


inline constexpr Radian& Radian::operator+=(const Radian& x) 
{
    value_ += x.value_;
    return *this;
}

inline constexpr Radian& Radian::operator-=(const Radian& x)
{
    value_ -= x.value_;
    return *this;
}

inline constexpr Radian& Radian::operator*=(const Rep& a)
{
    value_ *= a;
    return *this;
}

inline constexpr Radian& Radian::operator/=(const Rep& a)
{
    value_ /= a;
    return *this;
}

inline constexpr Radian operator+(Radian a, const Radian& b)
{ return a += b; }

inline constexpr Radian operator-(Radian a, const Radian& b)
{ return a -= b; }

inline constexpr Radian operator*(const Radian::Rep& a, Radian b)
{ return b *= a; }

inline constexpr Radian operator*(Radian b, const Radian::Rep& a)
{ return a * b; }

inline constexpr Radian operator/(Radian b, const Radian::Rep& a)
{ return b /= a; }


// Equality
inline constexpr bool operator==(const Radian& a, const Radian& b)
{ return a.value() == b.value();}

inline constexpr bool operator!=(const Radian& a, const Radian& b)
{ return !(a == b);}


// Order
inline constexpr bool operator<(const Radian& a, const Radian& b)
{ return a.value() < b.value(); }

inline constexpr bool operator<=(const Radian& a, const Radian& b)
{ return a.value() <= b.value(); }

inline constexpr bool operator>(const Radian& a, const Radian& b)
{ return a.value() > b.value(); }

inline constexpr bool operator>=(const Radian& a, const Radian& b)
{ return a.value() >= b.value(); }


// Iostreams
inline std::ostream& operator<<(std::ostream& out, const Radian& a)
{ return out << a.value() << " rad"; }

// Funciones de conversión. Básicamente creo estas dos clases para hacer esto.
inline constexpr Degree::Degree(const Radian& x)
    : value_{radian2degree(x.value())} { }

inline constexpr Radian::Radian(const Degree& x)
    : value_{degree2radian(x.value())} { }


// Angles equality
inline constexpr bool operator==(const Radian& r, const Degree& d)
{ return r == Radian{d}; }

inline constexpr bool operator==(const Degree& d, const Radian& r) 
{ return r == d; }

inline constexpr bool operator!=(const Radian& r, const Degree& d)
{ return !(r == d);}

inline constexpr bool operator!=(const Degree& d, const Radian& r) 
{ return r != d;}


// Trigonometric functions
// -----------------------
// TODO: Las funciones trig. pueden operar usando float, double o long double.
// Parametrizar Radian por la representación (esta todo preparado para hacer
// el cambio de forma sencilla).
inline double sin(const Radian& r) {return std::sin(r.value());}
inline double cos(const Radian& r) {return std::cos(r.value());}
inline double tan(const Radian& r) {return std::tan(r.value());}

inline Radian asin(double x) {return Radian{std::asin(x)};}
inline Radian acos(double x) {return Radian{std::acos(x)};}
inline Radian atan(double x) {return Radian{std::atan(x)};}


inline double sin(const Degree& d) { return sin(Radian{d}); }
inline double cos(const Degree& d) {return cos(Radian{d});}
inline double tan(const Degree& d) {return tan(Radian{d});}

inline Degree asind(double x) {return Degree{asin(x)};}
inline Degree acosd(double x) {return Degree{acos(x)};}
inline Degree atand(double x) {return Degree{atan(x)};}



///// Devuelve el coseno. El ángulo está en grados.
///// Copio esta notación de Octave. ¿Sería mejor cos_en_grados?
//// TODO: g son grados. Cuando hay varias formas de medir un número hay que 
//// usar cosd(Grados{20})  <-- ya está definido en atd::Magnitude!!!
//inline double cosd(double g) { return std::cos(degree2radian(g)); }
//
///// Devuelve el seno. El ángulo está en grados.
///// Copio esta notación de Octave. ¿Sería mejor sen_en_grados?
//inline double sind(double g) { return std::sin(degree2radian(g)); }
//

/// Devuelve el módulo del vector de coordenadas cartesianas (x, y)
inline double modulo_vector(double x, double y)
{return sqrt(x*x + y*y);}

/// Devuelve el ángulo en grados, que forma el punto de coordenadas 
/// (x,y) con el eje x.
///
/// ¡¡¡El punto (0,0) devuelve 0.0!!!
inline double angulo_eje_x(double x, double y)
{
    double ang;
    if(x != 0)
    {
	double xn = x, yn = y;	// cuidado con los castings en atan!!!
	ang = radian2degree(std::atan(yn/xn));
	if(x < 0)   // 2º o 3er cuadrante
	    ang += 180.0;
	else
	{
	    if(y < 0) // 4º cuadrante
		ang += 360.0;
	}

    }
    else
    {
	if(y == 0) ang = 0.0; // (0,0) lo voy a considerar como angulo = 0???
	else 
	{
	    if(y > 0) ang = 90.0;
	    else ang = 270.0;
	}
    }


    return ang;
}

/// Devuelve el vector (x,y) en polares (r, theta). El ángulo dado en grados.
inline std::pair<double, double> cartesianas2polares(double x, double y)
{
    return {modulo_vector(x, y), angulo_eje_x(x, y)};
}

/// Devuelve el vector (r, theta) en cartesianas (x, y). 
inline std::pair<double, double> polares2cartesianas(double r, const Degree& theta)
{
    return {r*cos(theta), r*sin(theta)};
}



/// Calcula la media de los valores [first, last).
/// Precondición: first != last (el rango no está vacío)
/// En caso de pasar un rango vacío se lanza una excepción (para que el
/// programador pueda localizar el error)
template <typename InputIt>
typename std::iterator_traits<InputIt>::value_type
average(InputIt first, InputIt last)
{
    using Value = typename std::iterator_traits<InputIt>::value_type;
    using Difference = typename std::iterator_traits<InputIt>::difference_type;

    if (first == last)
        throw std::logic_error{
            "Se está intentando calcular la media de un rango vacío"};

    Value sum = Value{0};
    Difference num_elementos = Difference{0};

    while (first != last){
	sum += *first;

	++num_elementos;
	++first;
    }

    return sum/num_elementos;
}



// Relación: m es múltiplo de n
template <typename Int>
struct Int_es_multiplo_de{
    constexpr Int_es_multiplo_de(Int n0) noexcept
	:n{n0}{}

    constexpr bool de(Int m) const noexcept
    {return !(n % m);}

    Int n;
};

/*!
 *  \brief  Devuelve true si n es múltiplo de m
 *
 *  Ejemplo: 
 *	if (es_multiplo(20).de(4))  // si 20 es múltiplo de 4
 *	    ...
 */
template <typename Int>
constexpr inline Int_es_multiplo_de<Int> es_multiplo(Int m)
{ return Int_es_multiplo_de<Int>{m}; }



/*!
 *  \brief  ¿Dos números son aproximados?
 *
 * TODO: En inglés, ¿cómo sería? Aproximate??? @_@
 *
 */
template <typename Int>
struct Aproximado{
    Int eps;

    Aproximado(Int eps0) :eps{eps0} { }

    bool operator()(Int x, Int y)
    {
	return -eps < (x-y) and (x-y) < eps;
    }
};


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


}// namespace alp

#endif

