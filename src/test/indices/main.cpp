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

#include "../../alp_indices.h"
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

void test_size2D()
{
    test::interfaz("Size_ij");
    
    using Size2D = Size_ij<int>;

    Size2D sz1{2, 3};
    Size2D sz2 = sz1;

    CHECK_TRUE(sz2 == sz1, "operator==");

}

void test_posicion()
{
    test::interfaz("Position_ij");

    {
    Position_ij<int> a;
    CHECK_TRUE(a.i == 0 and a.j == 0, "a()");
    }

    Position_ij<int> a{3,4};
    CHECK_TRUE(a.i == 3 and a.j == 4, "a(i,j)");

    Position_ij b{5, 10};

    a += b;
    CHECK_TRUE(a.i == 8 and a.j == 14, "a += b");

    a -= b;
    CHECK_TRUE(a.i == 3 and a.j == 4, "a(i,j)");

    a = Position_ij {1, 1};
    b = Position_ij {3, 3};

    CHECK_TRUE(!a.esta_a_la_dcha_de(b), "esta_a_la_dcha_de");
    CHECK_TRUE(a.esta_a_la_izda_de(b), "esta_a_la_izda_de");
    CHECK_TRUE(a.esta_encima_de(b), "esta_encima_de");
    CHECK_TRUE(!a.esta_debajo_de(b), "esta_debajo_de");

    a = b;
    CHECK_TRUE(a == b, "==");

    b = Position_ij{3,10};
    CHECK_TRUE(a != b, "!=");


    auto c = a + b;
    CHECK_TRUE((c.i == a.i + b.i) and (c.j == a.j + b.j), "c = a + b");

    c = a - b;
    CHECK_TRUE((c.i == a.i - b.i) and (c.j == a.j - b.j), "c = a - b");

    CHECK_TRUE((a_la_dcha_de(a, 1) == a + Position_ij{0,1}), "a_la_dcha");
    CHECK_TRUE((a_la_izda_de(a, 1) == a + Position_ij{0,-1}), "a_la_izda");
    CHECK_TRUE((arriba_de(a, 1) == a + Position_ij{-1,0}), "arriba");
    CHECK_TRUE((abajo_de(a, 1) == a + Position_ij{1,0}), "abajo");


    a = Position_ij{1, 1};
    b = Position_ij{5, 7};

    CHECK_TRUE((a_la_dcha_de(a, 4, b) == a + Position_ij{0, 4}),
               "a_la_dcha_de(ok)");

    CHECK_TRUE((a_la_dcha_de(a, 10, b) == Position_ij{1, 7}),
               "a_la_dcha_de(exceso)");

    CHECK_TRUE((a_la_izda_de(b, 4, a) == b - Position_ij{0, 4}),
               "a_la_izda(ok)");

    CHECK_TRUE((a_la_izda_de(b, 10, a) ==  Position_ij{5, 1}),
               "a_la_izda(exceso)");

    a = Position_ij{6, 10};
    b = Position_ij{10, 20};

    CHECK_TRUE((a_la_dcha_de(a, 2, b) == Position_ij{6, 12}),
               "a_la_dcha_de(ok)");



    a = Position_ij{1, 1};
    b = Position_ij{5, 7};

    cout << longitud_segmento_h(a,b) << '\n';
    CHECK_TRUE(longitud_segmento_h(a, b) == 7, "longitud_segmento_h");
    CHECK_TRUE(longitud_segmento_h(b, a) == 7, "longitud_segmento_h");
    CHECK_TRUE(longitud_segmento_v(a, b) == 5, "longitud_segmento_v");
    CHECK_TRUE(longitud_segmento_v(b, a) == 5, "longitud_segmento_v");
}


void test_rango()
{
    using Size2D   = Size_ij<int>;
    using Range    = Range_ij<int>;
    using Position = Position_ij<int>;

    {
	Range rg{Position{2, 7}, Position{4, 10}};
	CHECK_TRUE((rg.i0 == 2 and rg.ie == 5 
		    and rg.j0 == 7 and rg.je == 11), "Range_ij()");
    }

    {
	Range rg{Position{2, 7}, Size2D{3, 4}};
	CHECK_TRUE((rg.i0 == 2 and rg.ie == 5 
		    and rg.j0 == 7 and rg.je == 11), "Range_ij()");
    }

}


void test_rango_acotado()
{
    test::interfaz("Range_acotado_ij");

    using Range         = Range_acotado_ij<int>::Range2D;
    using Range_acotado = Range_acotado_ij<int>;

    Range_acotado rg{Range{3, 6, 3, 8}, Range{1, 8, 1, 10}};

    CHECK_TRUE((rg.i0 == 3 and rg.ie == 6
		and rg.j0 == 3 and rg.je == 8), "rango ok");

    CHECK_TRUE((rg.cota.i0 == 1 and rg.cota.ie == 8
		and rg.cota.j0 == 1 and rg.cota.je == 10), "cota ok");

    CHECK_TRUE(rg.rows() == 3, "rows");
    CHECK_TRUE(rg.cols() == 5, "cols");

    auto sz = rg.size();
    CHECK_TRUE((sz.rows == rg.rows() and sz.cols == rg.cols()), "size");

    {
	Range_acotado rg2{rg}; 
	rg2.muevete_a_la_dcha(-1);
	CHECK_TRUE((rg2.i0 == 3 and rg2.ie == 6
		and rg2.j0 == 2 and rg2.je == 7), "muevete_a_la_dcha(-1)");

	rg2.muevete_a_la_dcha(-10);
	CHECK_TRUE((rg2.i0 == 3 and rg2.ie == 6
		and rg2.j0 == 1 and rg2.je == 6), "muevete_a_la_dcha(-de mas)");
    }
    {
	Range_acotado rg2{rg}; 
	rg2.muevete_arriba(-1);
	CHECK_TRUE((rg2.i0 == 4 and rg2.ie == 7
		and rg2.j0 == 3 and rg2.je == 8), "muevete_arriba(-1)");

	rg2.muevete_arriba(-10);
	CHECK_TRUE((rg2.i0 == 5 and rg2.ie == 8
		and rg2.j0 == 3 and rg2.je == 8), "muevete_a_la_dcha(-de mas)");


    }

    for (int j = 0; j < 10; ++j){
	Range_acotado rg2{rg}; 
	rg2.muevete_a_la_dcha(j);

        CHECK_TRUE((rg2.i0 == rg.i0 and rg2.ie == rg.ie),
                   "muevete_a_la_dcha(i, ok)");

        if (rg.je + j > rg.cota.je){
            CHECK_TRUE(
                (rg2.je == rg.cota.je and rg2.j0 == rg2.je - rg2.cols()),
                "muevete_a_la_dcha(j, borde)");
        }

	else
	    CHECK_TRUE((rg2.j0 == rg.j0 + j and rg2.je == rg.je + j)
			, "muevete_a_la_dcha(j, ok)");
    }
 

    for (int j = 0; j < 10; ++j){
	Range_acotado rg2{rg}; 
	rg2.muevete_a_la_izda(j);

        CHECK_TRUE((rg2.i0 == rg.i0 and rg2.ie == rg.ie),
                   "muevete_a_la_izda(i, ok)");

        if (rg.j0 < rg.cota.j0 + j){
            CHECK_TRUE(
                (rg2.j0 == rg.cota.j0 and rg2.je == rg2.j0 + rg2.cols()),
                "muevete_a_la_izda(j, borde)");
        }

	else
	    CHECK_TRUE((rg2.j0 == rg.j0 - j and rg2.je == rg.je - j)
			, "muevete_a_la_izda(j, ok)");
			
    }







    for (int j = 0; j < 10; ++j){
	Range_acotado rg2{rg}; 
	rg2.muevete_abajo(j);

        CHECK_TRUE((rg2.j0 == rg.j0 and rg2.je == rg.je),
                   "muevete_abajo(j, ok)");

        if (rg.ie + j > rg.cota.ie){
            CHECK_TRUE(
                (rg2.ie == rg.cota.ie and rg2.i0 == rg2.ie - rg2.rows()),
                "muevete_abajo(i, borde)");
        }

	else
	    CHECK_TRUE((rg2.i0 == rg.i0 + j and rg2.ie == rg.ie + j)
			, "muevete_abajo(i, ok)");
    }
 

    for (int j = 0; j < 10; ++j){
	Range_acotado rg2{rg}; 
	rg2.muevete_arriba(j);

        CHECK_TRUE((rg2.j0 == rg.j0 and rg2.je == rg.je),
                   "muevete_arriba(j, ok)");

        if (rg.i0 < rg.cota.i0 + j){
            CHECK_TRUE(
                (rg2.i0 == rg.cota.i0 and rg2.ie == rg2.i0 + rg2.rows()),
                "muevete_arriba(i, borde)");
        }

	else
	    CHECK_TRUE((rg2.i0 == rg.i0 - j and rg2.ie == rg.ie - j)
			, "muevete_arriba(i, ok)");
    }


    for (int j = 0; j < 10; ++j){
	Range_acotado rg2{rg}; 
	rg2.mueve_lado_dcho_a_la_dcha(j);

        CHECK_TRUE((rg2.i0 == rg.i0 and rg2.ie == rg.ie and rg2.j0 == rg.j0),
                   "mueve_lado_dcho_a_la_dcha(1. ok)");

        if (rg.je + j > rg.cota.je)
            CHECK_TRUE(rg2.je == rg2.cota.je,
                "mueve_lado_dcho_a_la_dcha(2., borde)");

	else
	    CHECK_TRUE(rg2.je == rg.je + j
			, "mueve_lado_dcho_a_la_dcha(2., ok)");
    }


    for (int j = 0; j < 10; ++j){
	Range_acotado rg2{rg}; 
	rg2.mueve_lado_dcho_a_la_izda(j);

        CHECK_TRUE((rg2.i0 == rg.i0 and rg2.ie == rg.ie and rg2.j0 == rg.j0),
                   "mueve_lado_dcho_a_la_izda(1. ok)");

        if (rg.je - j < rg.j0)
            CHECK_TRUE(rg2.je == rg.j0, 
                "mueve_lado_dcho_a_la_izda(2., borde)");

	else
	    CHECK_TRUE(rg2.je == rg.je - j
			, "mueve_lado_dcho_a_la_izda(2., ok)");
    }



    for (int j = 0; j < 10; ++j){
	Range_acotado rg2{rg}; 
	rg2.mueve_lado_izdo_a_la_dcha(j);

        CHECK_TRUE((rg2.i0 == rg.i0 and rg2.ie == rg.ie and rg2.je == rg.je),
                   "mueve_lado_izdo_a_la_dcha(1. ok)");

        if (rg.j0 + j > rg.je)
            CHECK_TRUE(rg2.j0 == rg.je,
                "mueve_lado_izdo_a_la_dcha(2., borde)");

	else
	    CHECK_TRUE(rg2.j0 == rg.j0 + j
			, "mueve_lado_izdo_a_la_dcha(2., ok)");
    }


    for (int j = 0; j < 10; ++j){
	Range_acotado rg2{rg}; 
	rg2.mueve_lado_izdo_a_la_izda(j);

        CHECK_TRUE((rg2.i0 == rg.i0 and rg2.ie == rg.ie and rg2.je == rg.je),
                   "mueve_lado_izdo_a_la_izda(1. ok)");

        if (rg.j0 - j < rg2.cota.j0)
            CHECK_TRUE(rg2.j0 == rg2.cota.j0, 
                "mueve_lado_izdo_a_la_izda(2., borde)");

	else
	    CHECK_TRUE(rg2.j0 == rg.j0 - j
			, "mueve_lado_izdo_a_la_izda(2., ok)");
    }

    {
	Range_acotado rg{Range{5, 3, 7, 4}, Range{1, 8, 1, 10}};

	CHECK_TRUE((rg.i0 == 3 and rg.ie == 5 and rg.j0 == 4 and rg.je == 7)
		, "ordena_indices(si)");

	rg = Range_acotado{Range{3, 5, 4, 7}, Range{1, 8, 1, 10}};

	CHECK_TRUE((rg.i0 == 3 and rg.ie == 5 and rg.j0 == 4 and rg.je == 7)
		, "ordena_indices(no)");


	rg = Range_acotado{Range{0, 20, 40, 0}, Range{1, 8, 2, 10}};

	cout << rg.i0 << ", " << rg.ie << ", " << rg.j0 << ", " << rg.je << '\n';
	CHECK_TRUE((rg.i0 == 1 and rg.ie == 8 and rg.j0 == 2 and rg.je == 10)
		, "ordena_indices(trunca)");

    }

}





int main()
{
try{
    test::init("alp_indices.h");

    test_segmento1D();
    test_size2D();
    test_rango();
    test_rango_acotado();
    test_posicion();


}catch(std::exception& e)
{
    std::cerr << e.what() << endl;
    return 1;
}
}
