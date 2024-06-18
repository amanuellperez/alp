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

#include "../../alp_math_efunc.h"
#include "../../alp_test.h"

#include <iostream>
#include <limits>
#include <functional>


using namespace std;
using namespace test;
using namespace alp;


void test_relacion()
{
    test::interfaz("Estudia_crecimiento_decrecimiento::Relacion");

    Estudia_crecimiento_decrecimiento::Relacion r1{1,3, 'c'};
    
    CHECK_TRUE(r1(0,1) == true, "Relacion() -> true");
    CHECK_TRUE(r1(0,2) == true, "Relacion() -> true");
    CHECK_TRUE(r1(0,3) == false, "Relacion() -> false");
    CHECK_TRUE(r1(-10,10) == false, "Relacion() -> false");

}

void test_estudia_crecimiento()
{
    test::interfaz("Estudia_crecimiento_decrecimiento");

    {// > 01
    Estudia_crecimiento_decrecimiento
	estudia = {   {neg_infinity<int>(), 0, 'D'}
		    , {0, 0, '='} 
		    , {0, infinity<int>(), 'C'} };

    vector<int> f = {1, 2, 3, 3, 3, 3, 2, 1};

    auto est = estudia(f, f.size());

    CHECK_TRUE(est.size() == 3, "Estudia_crecimiento_decrecimiento");

    CHECK_TRUE(	    est[0].i == 0 
		and est[0].j == 2 
		and est[0].name == 'C' 
		and est[0].incr == 2,
               "Estudia_crecimiento_decrecimiento");

    CHECK_TRUE(	    est[1].i == 2 
		and est[1].j == 5 
		and est[1].name == '=' 
		and est[1].incr == 0,
               "Estudia_crecimiento_decrecimiento");

    CHECK_TRUE(	    est[2].i == 5 
		and est[2].j == 7 
		and est[2].name == 'D' 
		and est[2].incr == -2,
               "Estudia_crecimiento_decrecimiento");

    for (auto x: est)
	cout << x;

    cout << '\n';
    }// 01 <

    {// > 02
    Estudia_crecimiento_decrecimiento
	estudia = {   {neg_infinity<int>(), 0, 'D'}
		    , {0, 0, '='} 
		    , {0, infinity<int>(), 'C'} };

    vector<int> f;

    auto est = estudia(f, f.size());

    CHECK_TRUE(est.size() == 0, "Estudia_crecimiento_decrecimiento(vacio)");
    }// 02 < 

    {// > 03
    Estudia_crecimiento_decrecimiento
	estudia = {   {neg_infinity<int>(), 0, 'D'}
		    , {0, 0, '='} 
		    , {0, infinity<int>(), 'C'} };

    vector<int> f = {1,2,3,4,5};

    auto est = estudia(f, f.size());

    CHECK_TRUE(est.size() == 1, "Estudia_crecimiento_decrecimiento(todo <)");

    CHECK_TRUE(	    est[0].i == 0 
		and est[0].j == 4 
		and est[0].name == 'C' 
		and est[0].incr == 4,
               "Estudia_crecimiento_decrecimiento(todo <)");

    }// 03 < 


    {// > 04
    Estudia_crecimiento_decrecimiento
	estudia = {   {neg_infinity<int>(), -5, 'D'}
		    , {-5, 0, 'd'}
		    , {0, 0, '='} 
		    , {0, 5, 'c'}
		    , {5, infinity<int>(), 'C'} };

    vector<int> f = {1, 2, 3, 10, 20, 30, 31, 32, 32, 32, 32,
		    31, 30, 20, 10, 9, 8, 7};

    auto est = estudia(f, f.size());


    CHECK_TRUE(est.size() == 7, "Estudia_crecimiento_decrecimiento");

    CHECK_TRUE(	    est[0].i == 0 
		and est[0].j == 2 
		and est[0].name == 'c' 
		and est[0].incr == 2,
               "Estudia_crecimiento_decrecimiento");

    CHECK_TRUE(	    est[1].i == 2 
		and est[1].j == 5 
		and est[1].name == 'C' 
		and est[1].incr == 27,
               "Estudia_crecimiento_decrecimiento");

    CHECK_TRUE(	    est[2].i == 5 
		and est[2].j == 7 
		and est[2].name == 'c' 
		and est[2].incr == 2,
               "Estudia_crecimiento_decrecimiento");

    CHECK_TRUE(	    est[3].i == 7 
		and est[3].j == 10 
		and est[3].name == '=' 
		and est[3].incr == 0,
               "Estudia_crecimiento_decrecimiento");


    CHECK_TRUE(	    est[4].i == 10
		and est[4].j == 12 
		and est[4].name == 'd' 
		and est[4].incr == -2,
               "Estudia_crecimiento_decrecimiento");


    CHECK_TRUE(	    est[5].i == 12
		and est[5].j == 14 
		and est[5].name == 'D' 
		and est[5].incr == -20,
               "Estudia_crecimiento_decrecimiento");


    CHECK_TRUE(	    est[6].i == 14
		and est[6].j == 17 
		and est[6].name == 'd' 
		and est[6].incr == -3,
               "Estudia_crecimiento_decrecimiento");

    for (auto x: est)
	cout << x;

    cout << '\n';

    vector<char> v;
    for (size_t i = 0; i < f.size(); ++i)
	v.push_back('-');

    estudia.expande(est, v.begin(), v.end()
		, [](char c){return c;}
		, [](){return 'x';});

    string res = "xcxCCxcx==xdxDxddx";
    stringstream s;
    for (auto x: v)
	s << x;

    CHECK_TRUE(s.str() == res, "expande");


    }// 04 < 
}   



int main()
{
try{
    test::init("alp_math_estfunc.h");

    test_relacion();
    test_estudia_crecimiento();


}catch(std::exception& e)
{
    std::cerr << e.what() << endl;
}

}



