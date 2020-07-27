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

#include "../../alp_math.h"

#include "../../alp_string.h"
#include "../../alp_exception.h"
#include "../../alp_test.h"

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

using namespace alp;
using namespace test;

void test_abs()
{
    test::interfaz("abs");

    CHECK_TRUE(abs(0)  == 0, "0");
    CHECK_TRUE(abs(1)  == 1, "1");
    CHECK_TRUE(abs(-10)  == 10, "-10");
    CHECK_TRUE(abs(0.0)  == 0.0, "0.0");
    CHECK_TRUE(abs(1.0)  == 1.0, "1.0");
    CHECK_TRUE(abs(-1.0)  == 1.0, "-1.0");
}

void test_multiplos()
{
    test::interfaz("multiplo_de_mayor_que");

    for (int q = 0; q < 10; ++q){
	cout << 4*q << " --> " << multiplo_de_mayor_que(4, 4*q) << '\n';
	CHECK_TRUE(multiplo_de_mayor_que(4, 4*q) == 4*q, to_string(4*q));
	for (int r = 1; r < 4; ++r){
	    cout << 4*q + r << " --> " << multiplo_de_mayor_que(4, 4*q+r) << '\n';
	    CHECK_TRUE(multiplo_de_mayor_que(4, 4*q+r) == 4*(q+1), to_string(4*(q+1)));
	}
    }

    CHECK_TRUE(es_multiplo(4).de(2), "es_multiplo.de");
    CHECK_TRUE(!es_multiplo(4).de(3), "es_multiplo.de");
}



void test_average()
{
    test::interfaz("average");

    {
    vector<int> v{1,2,3,4,5,6,7,8,9};
    CHECK_TRUE(average(v.begin(), v.end())  == 5, "average(normal)");
    }

    {
    vector<int> v{1,1,1,1,1};
    CHECK_TRUE(average(v.begin(), v.end())  == 1, "average(normal)");
    }

//    { // Esto tiene que lanzar una excepción
//    vector<int> v;
//    CHECK_TRUE_excepcion(average(v.begin(), v.end()), "average(vacio)");
//    }
}

void test_punto_medio()
{
    cout << "\npunto_medio\n"
	 <<   "-----------\n";

    CHECK_TRUE(punto_medio(6) == 3, "punto_medio(6)");
    CHECK_TRUE(punto_medio(7) == 3, "punto_medio(7)");
    CHECK_TRUE(punto_medio(8) == 4, "punto_medio(8)");
    CHECK_TRUE(punto_medio(9) == 4, "punto_medio(9)");
}

inline bool es_igual(double x, double y)
{return abs(x-y) < 0.0001;}

void test_cartesianas2polares()
{
    cout << "\ncartesianas2polares\n"
	 <<   "----------\n";

    test::interfaz("cartesianas2polares");

    {
    auto [r, t] = cartesianas2polares(1.0, 0.0);
    CHECK_TRUE(r == 1.0 and t == 0, "cartesianas2polares(1.0, 0.0)");
    auto [x, y] = polares2cartesianas(r, t);
    CHECK_TRUE(x == 1.0 and y == 0.0, "polares2cartesianas()");
    }

    {
    auto [r, t] = cartesianas2polares(1.0, 1.0);
    CHECK_TRUE(es_igual(r, sqrt(2)) and es_igual(t, 45), "cartesianas2polares(1.0, 1.0)");
    auto [x, y] = polares2cartesianas(r, t);
    CHECK_TRUE(es_igual(x, 1.0) and es_igual(y, 1.0), "polares2cartesianas()");
    }

    {
    auto [r, t] = cartesianas2polares(0.0, 1.0);
    CHECK_TRUE(es_igual(r, 1.0) and es_igual(t, 90), "cartesianas2polares(0.0, 1.0)");
    auto [x, y] = polares2cartesianas(r, t);
    CHECK_TRUE(es_igual(x, 0.0) and es_igual(y, 1.0), "polares2cartesianas()");
    }

}


void test_maximo()
{
    test::interfaz("maximo");

    int x = maximo(3);
    CHECK_TRUE(x == 3, "maximo");
}

template <typename Int>
void test_integral()
{
    double a0 = 10.0;
    double b0 = 30.0;

    Int a{a0};
    Int b{b0};
    CHECK_TRUE(a.value() == a0, "constructor");
    CHECK_TRUE(a == a, "operator==");
    CHECK_TRUE(!(a != a), "operator!=");

    CHECK_TRUE(a < b, "operator<");
    CHECK_TRUE(a <= b, "operator<=");
    CHECK_TRUE(a <= a, "operator<=");
    CHECK_TRUE(b >= b, "operator>=");
    CHECK_TRUE(a >= a, "operator>=");
    CHECK_TRUE(b > a, "operator>");


    b += a;
    CHECK_TRUE(b.value() == a0 + b0, "operator+=");

    b -= a;
    CHECK_TRUE(b.value() == b0, "operator-=");

    b *= 2;
    CHECK_TRUE(b.value() == 2*b0, "operator*=");

    b /= 2;
    CHECK_TRUE(b.value() == b0, "operator/=");

    Int c = a + b;
    CHECK_TRUE(c.value() == a0 + b0, "operator+");

    c = a - b;
    CHECK_TRUE(c.value() == a0 - b0, "operator-");

    c = 3*b;
    CHECK_TRUE(c.value() == 3*b0, "operator*");

    c = b / 2;
    CHECK_TRUE(c.value() == b0 / 2, "operator/");
}

void test_degree2radian(double deg, double rad)
{
    alp::Degree d = deg;
    alp::Radian r{d};

    alp::Aproximado aprox{0.0000001};
    CHECK_TRUE(aprox(r.value(), rad), alp::as_str() << deg << "º to rad");
}


void test_radian2degree(double rad, double deg)
{
    alp::Radian r = rad;
    alp::Degree d{r};

    alp::Aproximado aprox{0.0000001};
    CHECK_TRUE(aprox(d.value(), deg), alp::as_str() << rad << " rad to º");
}


void test_angles()
{
    test::interfaz("Degree");
    test_integral<alp::Degree>();

    test::interfaz("Radian");
    test_integral<alp::Radian>();

    test::interfaz("Degree to Radian and vice");
    test_degree2radian(0.0, 0.0);
    test_radian2degree(0.0, 0.0);

    test_degree2radian(30.0, alp::pi/6.0);
    test_radian2degree(alp::pi/6.0, 30.0);

    test_degree2radian(60.0, alp::pi/3.0);
    test_radian2degree(alp::pi/3.0, 60.0);

    test_degree2radian(90.0, alp::pi/2.0);
    test_radian2degree(alp::pi/2.0, 90.0);

    test_degree2radian(120.0, 2.0*alp::pi/3.0);
    test_radian2degree(2.0*alp::pi/3.0, 120.0);

    test_degree2radian(180.0, alp::pi);
    test_radian2degree(alp::pi, 180.0);

    test_degree2radian(360.0, 2.0*alp::pi);
    test_radian2degree(2.0*alp::pi, 360.0);

    test_degree2radian(-360.0, -2.0*alp::pi);
    test_radian2degree(-2.0*alp::pi, -360.0);

    {// angle equality
	alp::Degree d{180};
	alp::Radian r{pi};
	CHECK_TRUE(d == r, "degree == radian")
	CHECK_TRUE(r == d, "degree == radian")
	CHECK_TRUE(!(d != r), "degree != radian")
	CHECK_TRUE(!(r != d), "degree != radian")
    }


    test::interfaz("Trigonometric (radians)");

    CHECK_TRUE(std::sin(10.0) == alp::sin(Radian{10.0}), "sin");
    CHECK_TRUE(std::cos(10.0) == alp::cos(Radian{10.0}), "cos");
    CHECK_TRUE(std::tan(10.0) == alp::tan(Radian{10.0}), "tan");

    CHECK_TRUE(Radian{std::asin(0.25)} == alp::asin(0.25), "asin");
    CHECK_TRUE(Radian{std::acos(0.25)} == alp::acos(0.25), "acos");
    CHECK_TRUE(Radian{std::atan(10.0)} == alp::atan(10.0), "atan");


    CHECK_TRUE(alp::sin(Degree{10.0}) == alp::sin(Radian{Degree{10.0}}), "sind");
    CHECK_TRUE(alp::cos(Degree{10.0}) == alp::cos(Radian{Degree{10.0}}), "cosd");
    CHECK_TRUE(alp::tan(Degree{10.0}) == alp::tan(Radian{Degree{10.0}}), "tand");

    CHECK_TRUE(alp::asind(0.25) == alp::Degree{alp::asin(0.25)}, "asind");
    CHECK_TRUE(alp::acosd(0.25) == alp::Degree{alp::acos(0.25)}, "acosd");
    CHECK_TRUE(alp::atand(10.0) == alp::Degree{alp::atan(10.0)}, "atand");
}




int main()
{
try{
    test::header("alp::math");

    test_abs();
    test_multiplos();
    test_average();
    test_punto_medio();
    test_cartesianas2polares();
    test_maximo();
    test_angles();

}catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
}

}
