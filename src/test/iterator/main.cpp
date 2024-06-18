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

#include "../../alp_iterator.h"

#include <iostream>
#include <vector>

#include "../../alp_exception.h"
#include "../../alp_test.h"

using namespace std;
using namespace alp;
using namespace test;

template <typename I>
struct Prueba_iterator_base{
    //requires is_iterator<It>
    static_assert(alp::is_iterator_v<I>, "El tipo pasado It no es un iterador"); 

    // datos
    I it;

    using value_type      = typename std::iterator_traits<I>::value_type;
    using reference       = typename std::iterator_traits<I>::reference;
    using pointer         = typename std::iterator_traits<I>::pointer;
    using difference_type = typename std::iterator_traits<I>::difference_type;
    using iterator_category =
        typename std::iterator_traits<I>::iterator_category;

    using It       = typename alp::iterator_traits<I>::iterator;
    using const_It = typename alp::iterator_traits<I>::const_iterator;

    using iterator       = Prueba_iterator_base<It>;
    using const_iterator = Prueba_iterator_base<const_It>;

    static constexpr bool is_const_iterator = std::is_same_v<I, const_It>;

    // Construcción
    // ------------
    Prueba_iterator_base() {}

    explicit Prueba_iterator_base(const It& it0) : it{it0} { }

    explicit Prueba_iterator_base(const const_It& it0) 
    {
        static_assert(is_const_iterator,
            "Se está intentando construir un iterator con un const_iterator");

	it = it0;
    }

    explicit Prueba_iterator_base(const Prueba_iterator_base<It>& p)
	: it{p.it} { }

    explicit Prueba_iterator_base(const Prueba_iterator_base<const_It>& p)
    {
        static_assert(
	    is_const_iterator,
            "Se está intentando construir un iterator con un const_iterator");

	// Lo creo así para que de el error de la aserción y poder depurar
	// más fácilmente los errores
	it = const_It{p.it};
    }

    // operator==
    // ----------
    Prueba_iterator_base& operator=(const It& it0)
    {
        it = it0;
        return *this;
    }

    Prueba_iterator_base& operator=(const const_It& it0)  
    {
        static_assert(is_const_iterator,
            "Se está intentando construir un iterator con un const_iterator");

	it = it0;
	return *this;
    }

    Prueba_iterator_base& operator=(const Prueba_iterator_base<It>& p)
    {
        it = p.it;
        return *this;
    } 

    Prueba_iterator_base& operator=(const Prueba_iterator_base<const_It>& p)
    {
        static_assert(
	    is_const_iterator,
            "Se está intentando construir un iterator con un const_iterator");

	// Lo creo así para que de el error de la aserción y poder depurar
	// más fácilmente los errores
	it = p.it;

	return *this;
    }

    // convertimos Prueba_iterator_base to const_Prueba_iterator_base
    explicit operator const_iterator()
    { return const_iterator{it}; }


    // for input iterator
    reference operator*() const {return *it;}
    void operator++() {++it;}

    // for bidirectional iterator
    void operator--() {--it;}

    // for random access iterator
    void operator+=(difference_type i) {it += i;}

    difference_type difference(const Prueba_iterator_base& b) const
    {return it - b.it;}

    friend
    bool operator==(const Prueba_iterator_base& a, 
		    const Prueba_iterator_base& b)
    { return a.it == b.it; }

    friend
    bool operator!=(const Prueba_iterator_base& a, 
		    const Prueba_iterator_base& b)
    {return !(a == b);}

};


// El min indica que es un iterador mínimo (con el mínimo número de
// operaciones que define un iterador)
template <typename I>
using Prueba_iterator_min = Prueba_iterator_base<
	    typename alp::iterator_traits<I>::iterator>;

template <typename I>
using const_Prueba_iterator_min = Prueba_iterator_base<
	    typename alp::iterator_traits<I>::const_iterator>;

template <typename I>
using Prueba_iterator = Iterator<Prueba_iterator_min<I>>;

template <typename I>
using const_Prueba_iterator = Iterator<const_Prueba_iterator_min<I>>;


template <typename I>
Prueba_iterator<I> prueba_iterator(const I& it)
{return Prueba_iterator<I>{Prueba_iterator_min<I>{it}};}

template <typename I>
const_Prueba_iterator<I> const_prueba_iterator(const I& it)
{return const_Prueba_iterator<I>{const_Prueba_iterator_min<I>{it}};}



void test_iterator()
{
    test::interfaz("Iterator");

    std::vector<int> v{1, 2, 3, 4};

    using iterator       = Iterator<std::vector<int>::iterator>;
    using const_iterator = Iterator<std::vector<int>::const_iterator>;
    
    using pru_iterator       = Prueba_iterator_base<std::vector<int>::iterator>;
    using const_pru_iterator = Prueba_iterator_base<std::vector<int>::const_iterator>;

    {
    iterator p{v.begin()};
    iterator pe{v.end()};

    // iterator bad_p{v.cbegin()}; // no tiene que compilar

    *p = 10;
    size_t i = 0;
    for(; p != pe; ++p, ++i)
	CHECK_TRUE(*p == v[i], "Iterator");
    }

    {
    const_iterator p{v.begin()};
    const_iterator pe{v.end()};

//    iterator bad_p{p}; // no tiene que compilar

   // *p = 10;
    size_t i = 0;
    for(; p != pe; ++p, ++i)
	CHECK_TRUE(*p == v[i], "const_Iterator");
    }



    {
    const_iterator p{v.cbegin()};
    const_iterator pe{v.cend()};

   // *p = 10;
    size_t i = 0;
    for(; p != pe; ++p, ++i)
	CHECK_TRUE(*p == v[i], "const_Iterator(const)");
    }


    {
    iterator it_p{v.begin()};
    iterator it_pe{v.end()};

    const_iterator p{it_p};
    const_iterator pe{it_pe};

    // *p = 10; // no tiene que compilar
    size_t i = 0;
    for(; p != pe; ++p, ++i)
	CHECK_TRUE(*p == v[i], "Iterator to const_Iterator");
    }


    {
    pru_iterator p{v.begin()};
    pru_iterator pe{v.end()};

    *p = 10;
    size_t i = 0;
    for(; p != pe; ++p, ++i)
	CHECK_TRUE(*p == v[i], "Pru_iterator");
    }

    {
    const_pru_iterator p{v.begin()};
    const_pru_iterator pe{v.end()};

   // *p = 10;
    size_t i = 0;
    for(; p != pe; ++p, ++i)
	CHECK_TRUE(*p == v[i], "const_Pru_iterator");
    }


    {
    const_pru_iterator p{v.cbegin()};
    const_pru_iterator pe{v.cend()};

   // *p = 10;
    size_t i = 0;
    for(; p != pe; ++p, ++i)
	CHECK_TRUE(*p == v[i], "const_Pru_iterator");
    }

    {
    pru_iterator it_p{v.begin()};
    pru_iterator it_pe{v.end()};

    const_pru_iterator p{it_p};
    const_pru_iterator pe{it_pe};

    // *p = 10; // no tiene que compilar
    size_t i = 0;
    for(; p != pe; ++p, ++i)
	CHECK_TRUE(*p == v[i], "const_Pru_iterator");
    }



    {
	Prueba_iterator_min<std::vector<int>::iterator> p{pru_iterator{v.begin()}};
        Prueba_iterator_min<std::vector<int>::iterator> pe{pru_iterator{v.end()}};

	// conversión de tipos
	const_Prueba_iterator_min<std::vector<int>::const_iterator> cp{p};
	// *cp = 30; // no tiene que compilar

	*p = 20;
	size_t i = 0;
	for(; p != pe; ++p, ++i)
	    CHECK_TRUE(*p == v[i], "Prueba_iterator_min");
    }

    {
	auto p = prueba_iterator(v.begin());
	auto pe = prueba_iterator(v.end());

	// conversión de tipos
	const_Prueba_iterator<std::vector<int>::const_iterator> cp{p};

	cp = pe;
	// p = cp; // no tiene que compilar
	// *cp = 30; // no tiene que compilar
	
	*p = 20;
	size_t i = 0;
	for(; p != pe; ++p, ++i)
	    CHECK_TRUE(*p == v[i], "Prueba_iterator_min");
    }


    {
	auto p = const_prueba_iterator(v.begin());
	auto pe = const_prueba_iterator(v.end());

	// *p = 20; // no debe de compilar
	size_t i = 0;
	for(; p != pe; ++p)
	    CHECK_TRUE(*p == v[i++], "Prueba_iterator_min");
    }


    {
	auto p = const_prueba_iterator(v.cbegin());
	auto pe = const_prueba_iterator(v.cend());

	// *p = 20; // no debe de compilar
	size_t i = 0;
	for(; p != pe; ++p)
	    CHECK_TRUE(*p == v[i++], "Prueba_iterator_min");
    }



    
}


struct Punto{int x, y;};

// Es necesario definir los dos operadores const y no cosnt para las views no
// const, ya que los view_iterators van a instanciar tanto la versión no const
// como la const.  TODO: mejorar para que los iterator no necesiten el
// operator(const Punto&) si no se necesita. No tiene sentido que si no se va
// a usar este operador se tenga que implementar.
struct First{
    int& operator()(Punto& p) const {return p.x;}
    const int& operator()(const Punto& p) const {return p.x;}
};

struct const_First{
    const int& operator()(const Punto& p) const {return p.x;}
};

struct Second{
    int& operator()(Punto& p) const {return p.y;}
    int operator()(const Punto& p) const {return p.y;}
};


struct Suma{
    int operator()(const Punto& p) const {return p.x + p.y;}
};



template <typename It>
using Iterator_first = View_iterator<It, First>;


template <typename It>
Iterator_first<It> iterator_first(It it)
{ return view_iterator(it, First{}); }

template <typename It>
using const_Iterator_first = const_View_iterator<It, First>;

template <typename It>
const_Iterator_first<It> const_iterator_first(It it)
{ return const_view_iterator(it, First{}); }




template <typename It>
using View_iterator_second = View_iterator<It, Second>;

template <typename It>
View_iterator_second<It> view_iterator2(const It& it)
{ 
    static_assert(std::is_same_v<It, std::vector<Punto>::iterator>, 
	   "It no es el esperado!!!!!!!!!!!!!!!!!"); 

    return view_iterator(it, Second{}); 

}

template <typename It>
using Iterator_suma = View_iterator<It, Suma>;

template <typename It>
Iterator_suma<It> iterator_suma(It it)
{ return view_iterator(it, Suma{}); }



void test_view_iterator()
{
    test::interfaz("View_iterator_base");

    std::vector<Punto> v{Punto{1,4}, Punto{2,5}, Punto{3,6}};
    int i = 1;
    for (auto p = iterator_first(v.begin()); 
			p != iterator_first(v.end()); ++p) {
        CHECK_TRUE(*p == i, "*p");
        ++i;
    }

    {// test to_const
	using const_it = std::vector<Punto>::const_iterator;
	auto p = iterator_first(v.begin()); 
	auto pe = iterator_first(v.end()); 
	const_Iterator_first<const_it> q{p};
	const_Iterator_first<const_it> qe{pe};
	int i = 1;
	for (; q != qe; ++q){
	    CHECK_TRUE(*q == i, "*q");
	    ++i;
	// *p = 10; // sí compila
	//*q = 10; // no compila ya que es const!
	}
    }

    i = 1;
    for (auto p = const_iterator_first(v.cbegin()); 
			p != const_iterator_first(v.cend()); ++p) {
        CHECK_TRUE(*p == i, "*p");
        ++i;
    }



    i = 100;
    for (auto p = view_iterator2(v.begin()); 
					p != view_iterator2(v.end()); ++p){
	*p = i;
	i += 10;
    }

    i = 100;
    for (const auto& p: v){
	CHECK_TRUE(p.y == i, "*p = x");
	i += 10;
    }

    i = 1;
    for (auto	p = const_iterator_first(v.begin());
		p != const_iterator_first(v.end());
		++p){
	// *p = 20; // no tiene que compilar
        CHECK_TRUE(*p == i, "*p const");
        ++i;
    }

    // value
    i = 0;
    for (auto p = iterator_suma(v.begin()); p != iterator_suma(v.end()); ++p){
	// *p = 20; // esto no tiene que compilar
	CHECK_TRUE(*p == (v[i].x + v[i].y), "*p value");
	++i;
    }

    {// lambda
//	auto f = [](const Punto& p) {return p.x;};
//
//	auto p = view_iterator(v.begin(), f);
//	auto q = view_iterator(v.end(), f);
	// No se pueden copiar las views con las lambdas, ya que las lambdas
	// no se pueden copiar!!! Esto no compila:
//	p = q;
//	p = view_iterator(v.end(), f);
    }

}



int main()
{
try{
    test::header("alp_iterator.h");

    test_iterator();
    test_view_iterator();


}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}
}


