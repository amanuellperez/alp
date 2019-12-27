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

#include "../../alp_gen.h"

#include <iostream>

using namespace std;
using namespace alp;

int test_array_circular()
{
    Array_circular<int, 4> a = {{{0, 1,2,3}}};

    for(int i = 0; i < 100; ++i)
	if(a[i] != i%4){
	    cout << "ERROR: i = " << i << " muestra a[i] = " << a[i] 
		<< " en lugar de " << i%4 << endl;
	    return 1;
	}

    const Array_circular<int, 4> a2 = {{{0, 1,2,3}}};

    for(int i = 0; i < 100; ++i)
	if(a2[i] != i%4){
	    cout << "ERROR: i = " << i << " muestra a2[i] = " << a2[i] 
		<< " en lugar de " << i%4 << endl;
	    return 2;
	}

    int i = 0;
    for(auto x: a)
    {
	if (x != i){
	    cout << "ERROR: no se está iterando correctamente en el for range\n"
		"Falla en i = " << i << '\n';
	    return 3;
	}
	++i;

    }

    cout << "Test ejecutado correctamente\n";
    return 0;
}

int main()
{
try{
    test_array_circular();

}catch(std::runtime_error e)
{
    std::cerr << e.what() << endl;
}
    return 0;
}
