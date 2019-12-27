// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#include "../../alp_alg.h"

#include "../../alp_error.h"
#include "../../alp_string.h"
#include "../../alp_test.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace alp;
using namespace test;



void comprueba(const vector<string>& v, const string& x, bool res)
{
    cout << "Buscando [" << x << "]: ";

    if( (find_indice(v, x) != v.size()) == res)
	cout << " OK \n";
    else{
	cout << " ERROR\n";
	throw Excepcion{""};
    }
}

void test_find_indice()
{
    vector<int> v = {1,2,3,4,5,6};

    for(int i = 0; i < 10; ++i){
	cout << "Buscando [" << i << "]: ";
	if( find_indice(v, i) != v.size())
	    cout << " ENCONTRADO\n";
	else
	    cout << " NO ESTÁ\n";
    }


    vector<string> v2 = {"uno", "dos", "tres", "cuatro"};
    comprueba(v2, "uno", true);
    comprueba(v2, "rno", false);
    comprueba(v2, "", false);
}





struct Vector_2D
{
    int x, y;
};

inline bool operator==(const Vector_2D& a, const Vector_2D& b)
{ return (a.x == b.x) and (a.y == b.y);}

inline bool operator!=(const Vector_2D& a, const Vector_2D& b)
{return !(a == b);}

inline bool operator<(const Vector_2D& a, const Vector_2D& b)
{ return (a.y < b.y) or (a.y == b.y and a.x < b.y); }

inline bool operator>(const Vector_2D& a, const Vector_2D& b)
{return b < a;}

// Revisar. Hoy estoy empanado.
//inline bool operator<=(const Vector_2D& a, const Vector_2D& b)
//{return !(b < a);}
//
//inline bool operator>=(const Vector_2D& a, const Vector_2D& b)
//{return !(a < b);}

inline std::ostream& operator<<(std::ostream& out, const Vector_2D& v)
{ return out << '(' << v.x << ", " << v.y << ')'; }


void test_cuenta()
{
    cout << "Probando cuenta() ... ";
    vector<Vector_2D> x = {{1,1}, {1,2}, {2,2}, {1,1}, {3,4}, {2,2}, {1,2}, {1,2}};

    // auto num_elementos = cuenta(x);
    auto num_elementos = cuenta(begin(x), end(x));

    vector<Vector_2D> res_first = {{1,1}, {1,2}, {2,2}, {3,4}};
    vector<size_t> res_second   = {2, 3, 2, 1};

    if(num_elementos.size()  != res_first.size()){
	cout << entre_lineas("ERROR en test_cuenta!!!") << '\n';
	cout << "num_elementos.size() != res_first.size()!!!\n";
	throw alp::Excepcion{""};
    }

    for(size_t i = 0; i < num_elementos.size(); ++i){
	if(num_elementos[i].first != res_first[i]
		or num_elementos[i].second != res_second[i]){
	    cout << entre_lineas("ERROR en test_cuenta!!!") << '\n';
	    cout << "Valor incorrecto\n";

	    cout << "Datos:\n";
	    for(auto xi: num_elementos)
		cout << xi.first << " = " << xi.second << endl;

	    cout << "\n\nHa fallado en la posición i = " << i << endl;
	    throw alp::Excepcion{""};
	}
    }

    cout << "OK\n\n";
}   



void test_find2()
{
    cout << "\n\nProbando funciones find\n"
	 << "-----------------------\n";

    // ------------
    // find_pair_if
    // ------------
    {
    vector<int> v1{9,8,3,2,5,6};

    auto res = find_pair_if(v1.begin(), v1.end(), std::less<int>{});

    check_true(*res.first == 2 and *res.second == 5
			      , "find_pair_if(caso normal)");
    }


    {
    vector<int> v1{5,4,3,2,1};

    auto res = find_pair_if(v1.begin(), v1.end(), std::less<int>{});

    check_true(res.first == v1.end() and res.second == v1.end(),
               "find_pair_if(ningún elemento)");
    }

    // ----------------
    // find_pair_if_not
    // ----------------
    {
    vector<int> v1{2,3,4,1,5};

    auto res = find_pair_if_not(v1.begin(), v1.end(), std::less<int>{});

    check_true(*res.first == 4 and *res.second == 1
			      , "find_pair_if_not(caso normal)");
    }


    {
    vector<int> v1{1,2,3,4,5};

    auto res = find_pair_if_not(v1.begin(), v1.end(), std::less<int>{});

    check_true(res.first == v1.end() and res.second == v1.end(),
               "find_pair_if_not(ningún elemento)");
    }


    // -------------
    // rfind_pair_if
    // -------------
    {
    vector<int> v1{5, 4, 3, 8, 7, 6, 5};

    auto res = rfind_pair_if(std::find(v1.begin(), v1.end(), 6), v1.begin(),
                               std::less<int>{});

    check_true(*res.first == 3 and *res.second == 8,
               "rfind_pair_if (normal)");
    }

    {
    vector<int> v1{9,8,7,6,5,4};

    auto res = rfind_pair_if(std::find(v1.begin(), v1.end(), 6), v1.begin(),
                               std::less<int>{});

    check_true(res.first == res.second,
               "rfind_pair_if (ningún elemento)");
    }

    {
    vector<int> v1{1,8,7,6,5,4};

    auto res = rfind_pair_if(std::find(v1.begin(), v1.end(), 6), v1.begin(),
                               std::less<int>{});

    check_true(*res.first == 1 and *res.second == 8,
               "rfind_pair_if (caso extremo)");
    }


    // -----------------
    // rfind_pair_if_not
    // -----------------
    {
    vector<int> v1{10, 11, 12, 4, 5, 6, 7};

    auto res = rfind_pair_if_not(std::find(v1.begin(), v1.end(), 6), v1.begin(),
                               std::less<int>{});

    check_true(*res.first == 12 and *res.second == 4,
               "rfind_pair_if_not (normal)");
    }

    {
    vector<int> v1{1,2,3,4,5,6};

    auto res = rfind_pair_if_not(std::find(v1.begin(), v1.end(), 6), v1.begin(),
                               std::less<int>{});

    check_true(res.first == res.second,
               "rfind_pair_if_not (ningún elemento)");
    }


    // ------------------------------
    // find_first_range_overlapped_if
    // ------------------------------
    {
	vector<int> v1{1,2,3,4,5,6,7};

	auto res = find_first_range_overlapped_if(v1.begin(), v1.end()
			, std::find(v1.begin(), v1.end(), 3)
			, std::find(v1.begin(), v1.end(), 5)
			, std::less<int>{});


	check_true(res.first ==  v1.begin() and res.second == v1.end(),
		"find_first_range_overlapped_if (más grande)");

    }

    {
	vector<int> v1{7,8,9,10,9,8,9,11,7,5,6,9};

	auto res = find_first_range_overlapped_if(v1.begin(), v1.end()
			, std::find(v1.begin(), v1.end(), 10)
			, std::find(v1.begin(), v1.end(), 11)
			, std::less<int>{});


	check_true(*res.first ==  8 and *res.second == 7,
		"find_first_range_overlapped_if (más pequeño)");

    }


    {
	vector<int> v1{9,1,2,3,4,10,7,6,5,4};

	auto res = find_first_range_overlapped_if(v1.begin(), v1.end()
			, std::find(v1.begin(), v1.end(), 3)
			, std::find(v1.begin(), v1.end(), 5)
			, std::less<int>{});

	check_true(*res.first ==  1 and *res.second == 7,
		"find_first_range_overlapped_if (solapado a la izda)");

    }


    {
	vector<int> v1{3,11,10,9,1,2,3,4,5,6};

	auto res = find_first_range_overlapped_if(v1.begin(), v1.end()
			, std::find(v1.begin(), v1.end(), 10)
			, std::find(v1.begin(), v1.end(), 4)
			, std::less<int>{});

	check_true(*res.first ==  1 and res.second == v1.end(),
		"find_first_range_overlapped_if (solapado a la dcha)");

    }

    {
	vector<int> v1{9, 8,7,6,5,4,3,2,1};

	auto res = find_first_range_overlapped_if(v1.begin(), v1.end()
			, std::find(v1.begin(), v1.end(), 6)
			, std::find(v1.begin(), v1.end(), 3)
			, std::less<int>{});

	check_true(res.first ==  v1.end() and res.second == v1.end(),
		"find_first_range_overlapped_if (conjunto vacío)");

    }

}

void test_vector()
{

    {
	vector<int> v{1,2,3,4,5};
	auto r = push_back_unique(v, 9);
	check_true(r == true and v.size() == 6, "push_back_unique(si)");
    }

    {
	vector<int> v{1,2,3,4,5};
	auto r = push_back_unique(v, 2);
	check_true(r == false and v.size() == 5, "push_back_unique(no)");
    }

    {
	vector<int> v{1,2,3,4,5};
	auto r = pop_back(v);
	check_true(r == 5 and v.size() == 4, "pop_back");
    }
}

int main()
{
try{
//    test_find_indice();
//    test_cuenta();
//    test_find2();
    test_vector();

}catch(Excepcion e)
{
    cerr << e.what() << endl;
}
    return 0;
}
