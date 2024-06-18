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

#ifndef __ALP_UTF8_H__
#define __ALP_UTF8_H__


/****************************************************************************
 *
 *   - DESCRIPCION: Manejo de unicode utf8
 *
 *   - UTF8: 
 *	utf8 es un sistema de codificación de caracteres de longitud variable:
 *	de 1 a 4 bytes.
 *
 *	num_bytes   byte1	    byte2	byte3	    byte4
 *	    1	    0xxxxxxx    
 *	    2	    110xxxxx	    10xxxxxx
 *	    3	    1110xxxx	    10xxxxxx	10xxxxxx
 *	    4	    11110xxx	    10xxxxxx	10xxxxxx    10xxxxxx
 *
 *	El valor de cada byte indica su función:
 *	- de 00 a 7F (0 a 127)  : primer y único byte de un caracter.
 *	- de 80 a BF (128 a 191): byte de continuación.
 *	- de C2 a DF (194 a 223): primer byte de un caracter de 2 bytes.
 *	- de E0 a EE (224 a 239): primer byte de un caracter de 3 bytes.
 *	- de F0 a FF (240 a 255): primer byte de un caracter de 4 bytes.
 *
 *	Fuentes: 
 *	    https://en.wikipedia.org/wiki/UTF-8
 *	    http://www.fileformat.info/info/unicode/utf8.htm
 *
 *   - HISTORIA:
 *           Manuel Perez- 30/04/2019 Escrito
 *
 ****************************************************************************/

#include <iostream>
#include <cctype>
#include <cstring>

#include "alp_cast.h"
#include "alp_iterator.h"


namespace alp{

/***************************************************************************
 *		FUNCIONES SOBRE LA CODIFICACIÓN DE UTF8
 ***************************************************************************/
/// Devuelve el número de bytes que tiene el caracter UTF8 cuyo primer byte es
/// c.
int utf8_num_bytes(char c);


/// ¿Es un byte de continuación de un utf8?
inline bool is_continuing_byte(char c)
{ return ((c & 0xC0) == 0x80); }

/// ¿Es un byte de continuación de un utf8?
inline bool utf8_is_first_byte(char c)
{return !is_continuing_byte(c);}

/// ¿Es código ascii?
inline bool is_ascii(char c)
{ return (c & 0x80) == 0x00; }

/// ¿Es el primer byte de un utf8 codificado en 2 bytes?
inline bool is_first_byte_utf8_2bytes(char c)
{ return ((c & 0xE0) == 0xC0); }

/// ¿Es el primer byte de un utf8 codificado en 3 bytes?
inline bool is_first_byte_utf8_3bytes(char c)
{ return ((c & 0xF0) == 0xE0);}

/// ¿Es el primer byte de un utf8 codificado en 4 bytes?
inline bool is_first_byte_utf8_4bytes(char c)
{ return ((c & 0xF8) == 0xF0);}


/*!
 *  \brief  Caracter codificado en UTF8
 *
 *  Es propietario de los bytes que codifican el caracter.
 *
 */
class utf8_char_t{
public:
    /// Caracter null
    utf8_char_t() noexcept
	:utf8_char_t{'\0'}
    {}

    /// Construimos un utf8_char_t a partir del código ASCII c
    explicit utf8_char_t(char c) noexcept
	:utf8_char_t{c, '\0'}{}

    /// Construimos un utf8_char_t de dos bytes: c0, c1
    utf8_char_t(char c0, char c1) noexcept;

    /// Construimos un utf8_char_t de tres bytes: c0, c1, c2
    utf8_char_t(char c0, char c1, char c2) noexcept;

    /// Construimos un utf8_char_t de 4 bytes: c0, c1, c2, c3
    utf8_char_t(char c0, char c1, char c2, char c3) noexcept;

    // para poder construirlo como: utf8_char_t x{"á"};
    explicit utf8_char_t(const char* c);

    // acceso a cada byte
    char& operator[](int i) noexcept {return data_[i];}
    const char& operator[](int i) const noexcept {return data_[i];}

    /// Número de máximo de bytes que puede tener un utf8_char_t
    constexpr static size_t max_size() {return data_size_ - 1;}

    /// Número de bytes de este utf8_char_t
    size_t size() const;

    /// Imprime uc en out.
    friend std::ostream& operator<<(std::ostream& out, const utf8_char_t& uc);

private:
    // Número máximo de caracteres del utf8_char_t, incluyendo el '\0' del
    // final. Si añado el '\0' del final es para poder imprimir en
    // std::ostream data_ directamente.
    constexpr static size_t data_size_ = 5;

    char data_[data_size_]; // tiene que ser un array de chars y 
			    // no un std::array
                            // para que funcione std::cout << data_;
			    // Le añado el '\0' para que funcione 
			    // std::cout << data

    // Funciones ayuda
    void reset() noexcept
    {
	for (size_t i = 0; i < data_size_; ++i)
	    data_[i] = '\0';
    }
};


// Construimos un utf8_char_t de dos bytes: c0, c1
inline utf8_char_t::utf8_char_t(char c0, char c1) noexcept
    :utf8_char_t{c0, c1, '\0', '\0'} {}


// Construimos un utf8_char_t de dos bytes: c0, c1, c2
inline utf8_char_t::utf8_char_t(char c0, char c1, char c2) noexcept
    :utf8_char_t{c0, c1, c2, '\0'} {}


// Construimos un utf8_char_t de dos bytes: c0, c1, c2, c3
inline utf8_char_t::utf8_char_t(char c0, char c1, char c2, char c3) noexcept
{
    data_[0] = c0;
    data_[1] = c1;
    data_[2] = c2;
    data_[3] = c3;
    data_[4] = '\0';
}

inline utf8_char_t::utf8_char_t(const char* p)
{
    reset();

    char* q = data_;

    while (*p != '\0' and q != (data_ + data_size_)){
	*q = *p;
	++p; 
	++q;
    }
}

inline size_t utf8_char_t::size() const
{
    size_t i = 0;

    for (; i < max_size(); ++i)
	if (data_[i] == '\0')
	    return i;

    return i;
}


inline bool operator==(const utf8_char_t& a, const utf8_char_t& b)
{
    return	(a[0] == b[0]) and (a[1] == b[1]) 
	    and (a[2] == b[2]) and (a[3] == b[3])
	    and (a[4] == b[4]);
}

inline bool operator!=(const utf8_char_t& a, const utf8_char_t& b)
{return !(a == b);}


/***************************************************************************
 *			FUNCIONES DE INFORMACIÓN
 ***************************************************************************/
/// ¿Es una letra uc?
bool isalpha(const utf8_char_t& uc);

/// ¿Es alfanumérico uc?
bool isalnum(const utf8_char_t& uc);


// indica si uc está codificado en 1 byte
// 1 byte = 0xxxxxxx
inline bool has_1byte(const utf8_char_t& uc)
{ return is_ascii(uc[0]); }

inline bool is_ascii(const utf8_char_t& uc)
{ return has_1byte(uc);}


// indica si uc está codificado en 2 bytes
// 2 bytes = 110xxxx 10xxxxxx
inline bool has_2bytes(const utf8_char_t& uc)
{ // no compruebo que el 2 byte esté bien codificado
    return is_first_byte_utf8_2bytes(uc[0]);
    // and is_continuing_byte(uc[1])
    // and uc[2] == '\0'
}

// indica si uc está codificado en 3 bytes
// 3 bytes = 1110xxx 10xxxxxx 10xxxxxx
inline bool has_3bytes(const utf8_char_t& uc)
{ // no compruebo que el 3 byte esté bien codificado
    return is_first_byte_utf8_3bytes(uc[0]);
    // and is_continuing_byte(uc[1])
    // and is_continuing_byte(uc[2])
    // and uc[3] == '\0'
}


// indica si uc está codificado en 4 bytes
// 4 bytes = 11110xx 10xxxxxx 10xxxxxx 10xxxxxx
inline bool has_4bytes(const utf8_char_t& uc)
{ // no compruebo que el 4 byte esté bien codificado
    return is_first_byte_utf8_4bytes(uc[0]);
    // and is_continuing_byte(uc[1])
    // and is_continuing_byte(uc[2])
    // and is_continuing_byte(uc[3])
    // and uc[4] == '\0'
}





/// Leemos un utf8 caracter de in.
std::istream& utf8_get(std::istream& in, utf8_char_t& uc);


// Imprime uc en hexadecimal
void utf8_print_as_hex(std::ostream& out, const utf8_char_t& uc);


inline std::istream& operator>>(std::istream& in, utf8_char_t& uc)
{ return utf8_get(in, uc); }



/***************************************************************************
 *		     FUNCIONES DE CONVERSIÓN
 ***************************************************************************/
//bool is_iso88951(const utf8_char_t& uc);

/// Convierte el caracter UTF-8 uc en formato ISO-8895-1 (si se puede).
/// Si no se puede devuelve 0.
char to_iso88951(const utf8_char_t& uc);

/***************************************************************************
 *			    utf8_char_view_t
 ***************************************************************************/
/*!
 *  \brief  Es una view de un caracter codificado en UTF8.
 *
 *  No es propietaria de los bytes que codifican el UTF8.
 *
 */
template <typename It>
// requires: is_iterator<It> == char and Value_of<It> 
class utf8_char_view_t{
public:
    using difference_type = typename std::iterator_traits<It>::difference_type;

    // Es regular
    utf8_char_view_t() {p0_ = pe_ = It{};}

    utf8_char_view_t(It p0, It pe):p0_{p0}, pe_{pe} {}

    // Escritura: uc = "á";
    utf8_char_view_t& operator=(const std::string& s);

    utf8_char_t to_utf8_char() const;

    // una view se tiene que poder convertir en el objeto al que apunta (???)
    operator utf8_char_t() const {return to_utf8_char();}

    /// Número de bytes que tiene este caracter
    difference_type num_bytes() const { return pe_ - p0_; }

    /// Avanza al siguiente caracter dentro de la cadena.
    /// Consideramos que esta view está dentro de una cadena de caracteres.
    void next(It string_end);

  private:
    It p0_, pe_; // el caracter = [p0, pe)
};

template<typename It>
utf8_char_view_t<It>& utf8_char_view_t<It>::operator=(const std::string& s)
{
    auto p = p0_;
    auto q = s.begin();
    auto qe = s.end();
    for (; p != pe_ and q != qe; ++p, ++q)
	*p = *q;

    if (p != pe_ or q != qe)
        throw std::logic_error{as_str()
                               << "utf8_char_view_t::operator=(" << s
                               << "): intentando sustituir un caracter con "
                                  "distinto número de bytes que el original."
                                  " El original tiene "
                               << num_bytes() << " bytes."};

    return *this;
}


template<typename It>
utf8_char_t utf8_char_view_t<It>::to_utf8_char() const
{
    switch (num_bytes()){
	case 1: return utf8_char_t{*p0_};
	case 2: return utf8_char_t{*p0_, *(p0_+1)};
	case 3: return utf8_char_t{*p0_, *(p0_+1), *(p0_+2)};
	case 4: return utf8_char_t{*p0_, *(p0_+1), *(p0_+2), *(p0_+3)};
    }

    throw std::logic_error{
        "to_utf8_char(): caracter con número de bytes incorrecto"};
}



/// Busca el siguiente utf8 caracter a partir de std::next(p0).
/// [p0, pe) = cadena de caracteres utf8
template <typename It>
inline It utf8_string_iterator_next(It p0, It pe)
// [[ expects: utf8_is_first_byte(*p0) or p0 == pe ]]
{
    if (p0 == pe) return pe;

    ++p0;
    return std::find_if(p0, pe, utf8_is_first_byte);
}

template<typename It>
inline void utf8_char_view_t<It>::next(It string_end)
{
    p0_ = pe_;
    pe_ = utf8_string_iterator_next(pe_, string_end);
}

template<typename It>
inline bool operator==(const utf8_char_view_t<It>& uv, const utf8_char_t& uc)
{ return uc == uv.to_utf8_char(); }

template<typename It>
inline bool operator!=(const utf8_char_view_t<It>& uv, const utf8_char_t& uc)
{ return !(uv == uc);}


template<typename It>
inline bool operator==(const utf8_char_view_t<It>& uv, const std::string& s)
{ return uv == utf8_char_t{s.c_str()};}

template<typename It>
inline bool operator!=(const utf8_char_view_t<It>& uv, const std::string& s)
{ return !(uv == s);}

template<typename It>
inline std::ostream& operator<<(std::ostream& out, const utf8_char_view_t<It>& uv)
{ return out << uv.to_utf8_char(); }


/***************************************************************************
 *			Utf8_string_iterator
 ***************************************************************************/
/// Convierte un iterador de bytes en un iterador de utf8
template <typename I>
struct Utf8_string_iterator_base{

    // datos
    // DUDA: quizás sea mejor pasar un puntero a una cadena char* de C (c_str())
    // De esta forma ahorraríamos el último puntero pe ya que podemos usar el
    // centinela '\0'. ¿Merece la pena el ahorro?
    I p;		    // iterador al char que apuntamos
    /* const */ I pe; // iterador al último elemento + 1. Es el end de la cadena a la
		      // que apuntamos. Este nunca puede variar. No lo puedo
		      // definir como const ya que entonces no se podría
		      // copiar este iterador!!! No existiría operator=
    utf8_char_view_t<I> uc;    // caracter utf8 al que estamos apuntando

    // traits del iterador
    // Observar que si I es const_iterator, entonces reference y pointer son
    // const.
    using value_type = utf8_char_view_t<I>;
    using reference  = utf8_char_view_t<I>&;
    using pointer    = utf8_char_view_t<I>*;

    using difference_type = typename std::iterator_traits<I>::difference_type;
    // TODO: ¿convertirlo en bidireccional?
    using iterator_category = std::forward_iterator_tag;
//			typename std::iterator_traits<I>::iterator_category;


    // Iteradores const y no const que suministra I
    using It       = typename alp::iterator_traits<I>::iterator;
    using const_It = typename alp::iterator_traits<I>::const_iterator;

    // Versiones const y no const de Iterator. Estas versiones son
    // fundamentales para la construcción correcta del iterador.
    using iterator       = Utf8_string_iterator_base<It>;
    using const_iterator = Utf8_string_iterator_base<const_It>;

    // Construcción
    // ------------
    // Es regular
    Utf8_string_iterator_base() {}

    // Cuando haya concepts se puede elegir qué funciones generar o no
    // dependiendo de si este iterador es const o no const. Mientras tanto uso
    // static_assert para dar errores legibles.
    Utf8_string_iterator_base(const I& p0, const I& pe0)
        : p{p0}, pe{pe0}, uc{p0, utf8_string_iterator_next(p0, pe0)}
    { } 

    
    explicit Utf8_string_iterator_base(const Utf8_string_iterator_base<I>& it)
	: p{it.p}, pe{it.pe}, uc{it.uc} { }

    // convertimos Iterator to const_Iterator
    // explicit operator const_iterator() const
    operator const_iterator() const
    { return const_iterator{p, pe, uc}; }


    friend
    bool operator==(const Utf8_string_iterator_base& a, 
		    const Utf8_string_iterator_base& b)
    { return a.p == b.p; }

    friend
    bool operator!=(const Utf8_string_iterator_base& a, 
		    const Utf8_string_iterator_base& b)
    {return !(a == b);}


    // for input iterator
    reference operator*() /*const*/ {return uc;}

    void operator++() 
    {// TODO: esto es ineficiente ya que uc.next llama tiene anotado
     // el valor de p. Pero ¿qué interfaz definir a uc?
	p = utf8_string_iterator_next(p, pe);
	uc.next(pe);
	
    }

    // for bidirectional iterator
//    void operator--() {--it;}

    // for random access iterator
//   void operator+=(difference_type i) {it += i;}

//    difference_type operator-(const View_iterator_base& b) const
//    {return it - b.it;}

};


// Iteradores mínimos 
template <typename I>
using Utf8_string_iterator_min =
    Utf8_string_iterator_base<typename alp::iterator_traits<I>::iterator>;


template <typename I>
using const_Utf8_string_iterator_min =
    Utf8_string_iterator_base<typename alp::iterator_traits<I>::const_iterator>;


// Iteradores Utf8_string_iterator
template <typename It>
using Utf8_string_iterator = Iterator<Utf8_string_iterator_min<It>>;

template <typename It>
using const_Utf8_string_iterator = Iterator<const_Utf8_string_iterator_min<It>>;


/// auto it = utf8_string_iterator(it0, ite);
template <typename It>
inline Utf8_string_iterator<It> utf8_string_iterator(const It& p0, const It& pe)
{
    return Utf8_string_iterator<It>{Utf8_string_iterator_min<It>{p0, pe}};
}

template <typename It>
inline const_Utf8_string_iterator<It> const_utf8_string_iterator(const It& p0,
                                                                 const It& pe)
{
    return const_Utf8_string_iterator<It>{
				const_Utf8_string_iterator_min<It>{p0, pe}};
}



/***************************************************************************
 *				utf8_string
 ***************************************************************************/
/*!
 *  \brief  Es una cadena de caracteres, donde la representación interna
 *	es utf8.
 *
 *  Tiene el mismo interfaz que std::string.
 *
 */
class utf8_string{
public:
    // Tipos
    // -----
    using size_type	  = std::string::size_type;
    using difference_type = std::string::difference_type;
    using iterator        = Utf8_string_iterator<std::string::iterator>;
    using const_iterator  = const_Utf8_string_iterator<std::string::iterator>;

    // Construcción
    // ------------
    utf8_string() noexcept {}

    /// Hace una copia de str
    utf8_string(const char* str):data_{str} {}

    /// Hace una copia de str
    utf8_string(const std::string& str):data_{str} {}

    /// Construye la utf8_string a partir de [first, last)
    utf8_string(const_iterator& first, const_iterator& last)
        : data_{first.it.p, last.it.p}
    {
    }

    // ...

    // Element access
    // --------------
    // operator[]
    // front
    // back
    char* data() noexcept {return data_.data();}
    const char* data() const noexcept {return data_.data();}

    const char* c_str() const noexcept {return data_.c_str();}

    // Iterators
    // ---------
    iterator begin()
    { return utf8_string_iterator(data_.begin(), data_.end()); }

    iterator end()
    { return utf8_string_iterator(data_.end(), data_.end()); }

    const_iterator begin() const
    { return const_utf8_string_iterator(data_.begin(), data_.end()); }

    const_iterator end() const
    { return const_utf8_string_iterator(data_.end(), data_.end()); }


    // cbegin
    // cend
    // rbegin
    // crbegin
    // rend
    // crend

    // Capacity
    // --------
    bool empty() const {return data_.empty();}
    size_type size() const; // estas son noexcept, a diferencia de std::string
    size_type length() const {return size();}

    // max_size <-- ¿tiene sentido en utf8? Creo que no.
    // reserve
    // capacity
    void shrink_to_fit() {data_.shrink_to_fit();}

    // Operations
    // ----------
    void clear() noexcept {data_.clear();}

    void push_back(const utf8_char_t& uc);

    // ...
    

    // EqualityComparable
    friend bool operator==(const utf8_string& a, const utf8_string& b)
    {return a.data_ == b.data_;}


    // I/O operations
    friend std::ostream& operator<<(std::ostream& out, const utf8_string& s)
    {return out << s.data_;}

    friend std::istream& operator>>(std::istream& in, utf8_string& s)
    {return in >> s.data_;}

    // Considero que el fin de línea '\n' para todo tipo de utf8 (???)
    // Definir esta función fuera. Si no se define fuera no se puede llamar
    // como alp::getline. Las funciones friend son invisibles
    friend std::istream& getline(std::istream& in, utf8_string& s);

private:
    // cadena con los bytes que representan la secuencia de caracteres.
    // Este es el problema: string es una cadena de BYTES no de caracteres, 
    // pero el nombre sugiere que es una cadena de caracteres (si eres
    // de habla inglesa los dos conceptos coinciden!!!)
    std::string data_; 
};



// la idea es contar el número de bytes que son cabecera de utf8.
inline utf8_string::size_type utf8_string::size() const 
// [[ expects: is_well_formed_utf8_string(data_) ]]
{ return std::count_if(data_.cbegin(), data_.cend(), utf8_is_first_byte); }

inline void utf8_string::push_back(const utf8_char_t& uc)
{
    for (size_t i = 0; i < uc.size(); ++i)
	data_.push_back(uc[i]);
}

inline bool operator!=(const utf8_string& a, const utf8_string& b)
{return !(a == b);}


inline std::istream& getline(std::istream& in, utf8_string& s)
{ return std::getline(in, s.data_); }


// Orden lexicográfico
inline int operator<(const utf8_string& a, const utf8_string& b)
{return std::strcoll(a.c_str(), b.c_str());}

inline int operator>(const utf8_string& a, const utf8_string& b)
{return b < a;}

inline int operator<=(const utf8_string& a, const utf8_string& b)
{return !(a > b);}

inline int operator>=(const utf8_string& a, const utf8_string& b)
{return !(a < b);}




/***************************************************************************
 *		    Funciones útiles para parsear utf8_string
 ***************************************************************************/
namespace utf8{

/// ¿Es white space? = ¿es ascii and [ \t\n\v...]?
inline bool isspace(const utf8_char_t& uc)
{ return is_ascii(uc[0]) and std::isspace(uc[0]); }


/// Busca el primer caracter no whitespace = [ \t\n\v...]
/// Si no lo encuentra devuelve pe.
template <typename It>
It skip_whitespace(It p0, It pe)
{
    while (p0 != pe and isspace(*p0))
	++p0;

    return p0;
}

/// Busca el primer caracter whitespace = [ \t\n\v...]
/// En caso de no encontrarlo devuelve pe.
template <typename It>
It find_whitespace(It p0, It pe)
{
    while (p0 != pe and !isspace(*p0))
	++p0;

    return p0;
}


}// namespace utf8

/// Trocea la cadena 's' en palabras (delimitadas por whitespace)
/// Ejemplo: 
/// Si s = "uno. dos, tres"
/// devuelve: {"uno.", "dos,", "tres"}
/// Observar que devuelve "uno." con el punto al final. El delimitador de
/// palabras es el whitespace.
std::vector<utf8_string> split_words(const utf8_string& s);

} // namespace alp


#endif


