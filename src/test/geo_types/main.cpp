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

#include "../../alp_geo_types.h"
#include "../../alp_test.h"

#include <iostream>

using namespace std;
using namespace alp;
using namespace test;

void test_segmento1D()
{
    test::interfaz("Segmento1D");

    size_t x = 10;
    size_t y = 20;
    CHECK_TRUE((Segmento1D{x,y}.longitud() == y - x + 1), "longitud");
    CHECK_TRUE((Segmento1D{x,y}.num_pasos() == y - x), "num_pasos");

}


void test_posicion()
{
    test::interfaz("Posicion_ij");

    {
    Posicion_ij<int> a;
    CHECK_TRUE(a.i == 0 and a.j == 0, "a()");
    }

    Posicion_ij<int> a{3,4};
    CHECK_TRUE(a.i == 3 and a.j == 4, "a(i,j)");

    Posicion_ij b{5, 10};

    a += b;
    CHECK_TRUE(a.i == 8 and a.j == 14, "a += b");

    a -= b;
    CHECK_TRUE(a.i == 3 and a.j == 4, "a(i,j)");

    a = Posicion_ij {1, 1};
    b = Posicion_ij {3, 3};

    CHECK_TRUE(!a.esta_a_la_dcha_de(b), "esta_a_la_dcha_de");
    CHECK_TRUE(a.esta_a_la_izda_de(b), "esta_a_la_izda_de");
    CHECK_TRUE(a.esta_encima_de(b), "esta_encima_de");
    CHECK_TRUE(!a.esta_debajo_de(b), "esta_debajo_de");

    a = b;
    CHECK_TRUE(a == b, "==");

    b = Posicion_ij{3,10};
    CHECK_TRUE(a != b, "!=");


    auto c = a + b;
    CHECK_TRUE((c.i == a.i + b.i) and (c.j == a.j + b.j), "c = a + b");

    c = a - b;
    CHECK_TRUE((c.i == a.i - b.i) and (c.j == a.j - b.j), "c = a - b");

    CHECK_TRUE((a_la_dcha_de(a, 1) == a + Posicion_ij{0,1}), "a_la_dcha");
    CHECK_TRUE((a_la_izda_de(a, 1) == a + Posicion_ij{0,-1}), "a_la_izda");
    CHECK_TRUE((arriba_de(a, 1) == a + Posicion_ij{-1,0}), "arriba");
    CHECK_TRUE((abajo_de(a, 1) == a + Posicion_ij{1,0}), "abajo");


    a = Posicion_ij{1, 1};
    b = Posicion_ij{5, 7};

    CHECK_TRUE((a_la_dcha_de(a, 4, b) == a + Posicion_ij{0, 4}),
               "a_la_dcha_de(ok)");

    CHECK_TRUE((a_la_dcha_de(a, 10, b) == Posicion_ij{1, 7}),
               "a_la_dcha_de(exceso)");

    CHECK_TRUE((a_la_izda_de(b, 4, a) == b - Posicion_ij{0, 4}),
               "a_la_izda(ok)");

    CHECK_TRUE((a_la_izda_de(b, 10, a) ==  Posicion_ij{5, 1}),
               "a_la_izda(exceso)");

    a = Posicion_ij{6, 10};
    b = Posicion_ij{10, 20};

    CHECK_TRUE((a_la_dcha_de(a, 2, b) == Posicion_ij{6, 12}),
               "a_la_dcha_de(ok)");



    a = Posicion_ij{1, 1};
    b = Posicion_ij{5, 7};

    cout << longitud_segmento_h(a,b) << '\n';
    CHECK_TRUE(longitud_segmento_h(a, b) == 7, "longitud_segmento_h");
    CHECK_TRUE(longitud_segmento_h(b, a) == 7, "longitud_segmento_h");
    CHECK_TRUE(longitud_segmento_v(a, b) == 5, "longitud_segmento_v");
    CHECK_TRUE(longitud_segmento_v(b, a) == 5, "longitud_segmento_v");
}

void test_size()
{
    Size2D<int> sz{10, 20};
    auto sz_ij = Size2D_to<Size_ij<int>>(sz);
    CHECK_TRUE((sz_ij.rows == 20 and sz_ij.cols == 10),"Size2D_to_Size_ij");

    {
	using Punto = Posicion_ij<int>;
	using Size = typename Coordenada_traits<Punto>::Size;

	auto sz = Size2D_to<Size>(Size2D<int>{10, 20});
    CHECK_TRUE((sz.rows == 20 and sz.cols == 10),"Size2D_to_Size_ij");
    }
}

void test_rectangulo_ij()
{
    test::interfaz("Rectangulo_ij");

    Posicion_ij p0{1, 3};
    Posicion_ij p1{5, 9};

    Rectangulo r{p0, p1};

    CHECK_TRUE(r.p0() == p0 and r.p1() == p1, "Rectangulo{p0, p1}");
    CHECK_TRUE(!r.empty(), "!empty()");

    Rectangulo r2{p1, p0};

    CHECK_TRUE(r == r2, "ordena_extremos/==");

    CHECK_TRUE((r.SI() == Posicion_ij{1,3}), "SI");
    CHECK_TRUE((r.SD() == Posicion_ij{1,9}), "SD");
    CHECK_TRUE((r.II() == Posicion_ij{5,3}), "II");
    CHECK_TRUE((r.ID() == Posicion_ij{5,9}), "ID");

    CHECK_TRUE(r.ancho() == 7, "ancho");
    CHECK_TRUE(r.alto() == 5, "alto");
    CHECK_TRUE(r.rows() == 5, "rows");
    CHECK_TRUE(r.cols() == 7, "cols");

    auto sz = r.size();
    CHECK_TRUE(sz.rows == 5 and sz.cols == 7, "size()");

    r.traslada(Posicion_ij{2, -3});
    CHECK_TRUE((r == Rectangulo{Posicion_ij{3, 0}, Posicion_ij{7, 6}}),
               "traslada");

    CHECK_TRUE(r.contiene_a(Posicion_ij{3,0}), "contiene_a(vertice)");
    CHECK_TRUE(r.contiene_a(Posicion_ij{7,6}), "contiene_a(vertice)");
    CHECK_TRUE(r.contiene_a(Posicion_ij{5,0}), "contiene_a(lado)");
    CHECK_TRUE(r.contiene_a(Posicion_ij{5,3}), "contiene_a(interior)");
    CHECK_TRUE(!r.contiene_a(Posicion_ij{-8,-3}), "!contiene_a(exterior)");

    CHECK_TRUE(pertenece(Posicion_ij{3,0}, r), "pertenece(vertice)");

    Rectangulo<Posicion_ij<int>> r3;  // vacio
    CHECK_TRUE(r3.empty(), "empty()");

    r3 = r;
    CHECK_TRUE(r3 == r, "operator=");
}

void test_rectangulo_acotado()
{
    test::interfaz("Rectangulo_acotado");

    using Rectangulo_acotado = alp::Rectangulo_acotado<Posicion_ij<int>>;

    Rectangulo terreno{Posicion_ij{3, 8}, Posicion_ij{10, 20}};

    Posicion_ij p0{3, 8};
    Posicion_ij p1{6, 10};
    Rectangulo_acotado r{Rectangulo{p0, p1}, terreno};

    CHECK_TRUE(r.p0() == p0, "p0");
    CHECK_TRUE(r.p1() == p1, "p1");

    r.muevete_a_la_dcha(4);
    CHECK_TRUE((r.p0() == Posicion_ij{3,12}
		and r.p1() == Posicion_ij{6,14}), "muevete_a_la_dcha(ok)");

    r.muevete_a_la_izda(4);
    CHECK_TRUE((r.p0() == p0
		and r.p1() == p1), "muevete_a_la_dcha(ok)");

    r.muevete_abajo(2);
    CHECK_TRUE((r.p0() == Posicion_ij{5,8}
		and r.p1() == Posicion_ij{8,10}), "muevete_abajo(ok)");

    r.muevete_arriba(2);
    CHECK_TRUE((r.p0() == p0
		and r.p1() == p1), "muevete_arriba(ok)");


    r.muevete_a_la_dcha(10);
    CHECK_TRUE((r.p0() == Posicion_ij{3,18}
		and r.p1() == Posicion_ij{6,20}), "muevete_a_la_dcha(borde)");

    r.muevete_a_la_izda(10);
    CHECK_TRUE((r.p0() == p0
		and r.p1() == p1), "muevete_a_la_dcha(borde)");

    r.muevete_abajo(4);
    CHECK_TRUE((r.p0() == Posicion_ij{7,8}
		and r.p1() == Posicion_ij{10,10}), "muevete_abajo(borde)");

    r.muevete_arriba(4);
    CHECK_TRUE((r.p0() == p0
		and r.p1() == p1), "muevete_arriba(borde)");


    r.muevete_a_la_dcha(20);
    CHECK_TRUE((r.p0() == Posicion_ij{3,18}
		and r.p1() == Posicion_ij{6,20}), "muevete_a_la_dcha(imposible)");

    r.muevete_a_la_izda(20);
    CHECK_TRUE((r.p0() == p0
		and r.p1() == p1), "muevete_a_la_izda(imposible)");

    r.muevete_abajo(20);
    CHECK_TRUE((r.p0() == Posicion_ij{7,8}
		and r.p1() == Posicion_ij{10,10}), "muevete_a_la_abajo(imposible)");

    r.muevete_arriba(20);
    CHECK_TRUE((r.p0() == p0
		and r.p1() == p1), "muevete_a_la_arriba(imposible)");

    r.mueve_lado_dcho(2, Rectangulo_acotado::Direccion::dcha);
    CHECK_TRUE((r.p0() == Posicion_ij{3, 8} and 
		r.p1() == Posicion_ij{6, 12}), "mueve_lado_dcho(dcha, +ok)");

    r.mueve_lado_dcho(2, Rectangulo_acotado::Direccion::izda);
    CHECK_TRUE((r.p0() == Posicion_ij{3, 8} and 
		r.p1() == Posicion_ij{6, 10}), "mueve_lado_dcho(izda, +ok)");

    r.mueve_lado_dcho(10, Rectangulo_acotado::Direccion::dcha);
    CHECK_TRUE((r.p0() == Posicion_ij{3, 8} and 
		r.p1() == Posicion_ij{6, 20}), "mueve_lado_dcho(dcha, extremo)");

    r.mueve_lado_dcho(12, Rectangulo_acotado::Direccion::izda);
    CHECK_TRUE((r.p0() == Posicion_ij{3, 8} and 
		r.p1() == Posicion_ij{6, 8}), "mueve_lado_dcho(izda, 0)");

    r.mueve_lado_dcho(50, Rectangulo_acotado::Direccion::dcha);
    CHECK_TRUE((r.p0() == Posicion_ij{3, 8} and 
		r.p1() == Posicion_ij{6, 20}), "mueve_lado_dcho(dcha, exceso)");

    r.mueve_lado_dcho(8, Rectangulo_acotado::Direccion::izda);
    CHECK_TRUE((r.p0() == Posicion_ij{3, 8} and 
		r.p1() == Posicion_ij{6, 12}), "mueve_lado_dcho(izda, ok)");

    r.mueve_lado_izdo(2, Rectangulo_acotado::Direccion::dcha);
    CHECK_TRUE((r.p0() == Posicion_ij{3, 10} and 
		r.p1() == Posicion_ij{6, 12}), "mueve_lado_izda(dcha, ok)");

    r.mueve_lado_izdo(2, Rectangulo_acotado::Direccion::izda);
    CHECK_TRUE((r.p0() == Posicion_ij{3, 8} and 
		r.p1() == Posicion_ij{6, 12}), "mueve_lado_izda(dcha, ok)");

    r.mueve_lado_izdo(20, Rectangulo_acotado::Direccion::dcha);
    CHECK_TRUE((r.p0() == Posicion_ij{3, 12} and 
		r.p1() == Posicion_ij{6, 12}), "mueve_lado_izda(dcha, exceso)");

    r.mueve_lado_izdo(20, Rectangulo_acotado::Direccion::izda);
    CHECK_TRUE((r.p0() == Posicion_ij{3, 8} and 
		r.p1() == Posicion_ij{6, 12}), "mueve_lado_izda(izda, exceso)");


    r.mueve_lado_inferior(2, Rectangulo_acotado::Direccion::abajo);
    CHECK_TRUE((r.p0() == Posicion_ij{3, 8} and 
		r.p1() == Posicion_ij{8, 12}), "mueve_lado_inferior(abajo, ok)");


    Rectangulo_acotado r2;
    r2.extension(r);
    CHECK_TRUE(r2 == r, "extension=");
}

int main()
{
try{
    test::header("geo_posicion");

    test_segmento1D();
    test_posicion();
    test_size();
    test_rectangulo_ij();
    test_rectangulo_acotado();

}catch(std::exception& e)
{
    std::cerr << e.what() << endl;
    return 1;
}
}
