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

#include "../../alp_string.h"

#include "../../alp_exception.h"
#include "../../alp_test.h"


using namespace std;
using namespace alp;
using namespace test;

void test_rtrim()
{
    CHECK_TRUE(rtrim("")  == "", "cadena nula");
    CHECK_TRUE(rtrim("    ")  == "", "cadena solo espacios");
    CHECK_TRUE(rtrim("xxx")  == "xxx", "xxx");
    CHECK_TRUE(rtrim("xxx  ")  == "xxx", "xxx  ");
    CHECK_TRUE(rtrim("  xxx")  == "  xxx", "  xxx");
    CHECK_TRUE(rtrim("  xxx  ")  == "  xxx", "  xxx  ");
}

void test_basename(const std::string& s)
{
    cout << "[" << s << "]" << "\t---> [" << basename(s) << "]\n";
}

void test_basename()
{
    test_basename("");
    test_basename("/");
    test_basename("/uno/");
    test_basename("uno/dos.txt");
}


void test_num_lineas(const std::string& s, unsigned nl)
{
    cout << "Probando num_lineas ... ";
    if(num_lineas(s) != nl){
	cout << '\n' << linea() << '\n';
	cout << s << '\n';
	cout << linea() << '\n';
	cout << "ERROR: num_lineas(s) = [" << num_lineas(s) << "], "
		"pero tiene [" << nl << "] líneas\n";
    }else
	cout << "OK\n";
}

void test_num_lineas()
{
    test_num_lineas("", 0);
    test_num_lineas("Esto es una prueba", 1);
    test_num_lineas("Esto\nes\nuna\nprueba", 4);
    test_num_lineas("Esto\nes\nuna\nprueba\n", 4);
}


void test_split_lineas_ancho(const string s, int ancho_max)
{
    cout << "Probando:\n" << entre_lineas(s);
    cout << "\nLo decomponemos en un ancho máximo de " << ancho_max << '\n';
    cout << "Resultado:\n" << entre_lineas(split_lineas_ancho(s, ancho_max))
	<< '\n';
}

void test_split_lineas_ancho()
{
    test_split_lineas_ancho("", 10);
    test_split_lineas_ancho("Esto es\nuna prueba. Av ver que tal sale."
		"\nEn un país multicolor, nació una aveja bajo el sol.\n"
		"Y fue famosa en el lugar,\nEsto es otra línea.\n", 10);
    test_split_lineas_ancho("123456 12345 1234 123 12 1", 5);
    test_split_lineas_ancho("multicolor, nació", 10);
    test_split_lineas_ancho("multicolor nació", 10);
}


void test_nombre_sin_extension(const std::string& s, const std::string& res)
{
    cout << s;
    auto y = nombre_sin_extension(s);
    cout << " --> " << y;
    if(y == res)
	cout  << " OK\n";
    else{
	cout << " ERROR!!!\n";
	throw alp::Excepcion{""};
    }
}


void test_nombre_sin_extension()
{
    cout << '\n' << entre_lineas("nombre_sin_extension") << '\n';
    test_nombre_sin_extension("", "");
    test_nombre_sin_extension("uno.txt", "uno");
    test_nombre_sin_extension("/dos/uno.txt", "/dos/uno");
    test_nombre_sin_extension("/dos.tres/uno.txt", "/dos.tres/uno");
}


void test_cambia_extension(const string& path, const string new_ext, const string res)
{
    cout << path << " + " << new_ext;
    auto y = cambia_extension(path, new_ext);
    cout << " --> " << y;
    if(y == res)
	cout  << " OK\n";
    else{
	cout << " ERROR!!!\n";
	throw alp::Excepcion{""};
    }
}

void test_cambia_extension()
{
    cout << '\n' << entre_lineas("cambia_extension") << '\n';
    test_cambia_extension("", "", ".");
    test_cambia_extension("uno.txt", "jpg", "uno.jpg");
    test_cambia_extension("uno.txt", "", "uno.");
    test_cambia_extension("uno", "jpg", "uno.jpg");
    test_cambia_extension("/dos/uno.txt", "pru", "/dos/uno.pru");
    test_cambia_extension("/dos.tres/uno.txt", "esto.es.otra", "/dos.tres/uno.esto.es.otra");
}

/// Prueba una función de 1 parámetro: y = f(x).
template<typename Funcion, typename Param, typename Return>
void check_funcion1(Funcion f, const Param& x, const Return& y)
{
    auto res = f(x);
    cout << '[' << x << "] --> [" << res << ']';
    if(res == y)
	std::cout  << " OK\n";
    else{
	std::cout << " ERROR!!!\n";
	throw alp::Excepcion{""};
    }
}
void test_dirname()
{
    cout << '\n' << entre_lineas("dirname") << '\n';
    auto func = [](const std::string& path){return dirname(path);};

    check_funcion1(func, "/home/dos.txt", "/home/");
    check_funcion1(func, "/home/", "/home/");
    check_funcion1(func, "/", "/");
    check_funcion1(func, "", "");
    check_funcion1(func, "/home/dos", "/home/dos");
}

void test_find_whitespace(const std::string& s, const std::string& res)
{
    auto p = find_whitespace(s.begin(), s.end());
    CHECK_TRUE((std::string{p, s.end()} == res), "find_whitespace");
}

void test_skip_whitespace(const std::string& s, const std::string& res)
{
    auto p = skip_whitespace(s.begin(), s.end());
    CHECK_TRUE((std::string{p, s.end()} == res), "skip_whitespace");
}

void test_whitespace()
{
    test_find_whitespace("", "");
    test_find_whitespace("uno dos", " dos");
    test_find_whitespace("uno	dos", "	dos");
    test_find_whitespace("uno", "");
    test_find_whitespace(" uno dos", " uno dos");

    test_skip_whitespace("", "");
    test_skip_whitespace("uno dos", "uno dos");
    test_skip_whitespace("        uno dos", "uno dos");

}


void test_split_words()
{
AQUI: probando esto
    std::string s = "hola, esto es una prueba";
    auto words = split_words(s);
    for (auto w: words)
	std::cout << w << '\n';
}



int main()
{
try{
    test::header("alp_string.h");

    test_rtrim();

    test_basename();
    test_num_lineas();
    test_split_lineas_ancho();
    test_nombre_sin_extension();
    test_cambia_extension();
    test_dirname();

    test_whitespace();
    test_split_words();

}catch(std::exception& e)
{
    cerr << e.what() << endl;
}
    return 0;
}







