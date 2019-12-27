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

#include "../../alp_utf8.h"
#include "../../alp_string.h"
#include "../../alp_test.h"

#include <fstream>
#include <map>

using namespace alp;
using namespace test;


void test_utf8_char_view_to_utf8_char_t(const std::string& s, 
                                        const std::string& r)
{
    utf8_char_t res{r.c_str()};
    utf8_char_view_t uv{s.begin(), s.end()};
    utf8_char_t uc = uv.to_utf8_char();
    CHECK_TRUE(uc == res, alp::as_str() << "to_utf8_char("<< s << ")");
}


void test_utf8_char_view_to_utf8_char_t()
{
    test::interfaz("utf8_char_view_t::to_utf8_char");
    test_utf8_char_view_to_utf8_char_t("a", "a");
    test_utf8_char_view_to_utf8_char_t( "é", "é");
    test_utf8_char_view_to_utf8_char_t("ࢥ", "ࢥ");
    test_utf8_char_view_to_utf8_char_t("🚕", "🚕");
}

void test_utf8_char_view(const std::string::iterator& p0, 
			 const std::string::iterator& pe, 
			 std::string& s0,
			 const std::string& c,
			 const std::string& res)
{
    utf8_char_view_t uc{p0, pe};
    uc = c;
    CHECK_TRUE(s0 == res,
               as_str() << "test_utf8_char_view(" << s0 << ", " << c << ")");

}

void test_utf8_char_view()
{
    std::string s = "aéࢥ🚕";


    auto q = s.begin();
    auto pe = s.end();
    auto qe = utf8_string_iterator_next(q, pe);
    test_utf8_char_view(q, qe, s, "o", "oéࢥ🚕");

    q = qe;
    qe = utf8_string_iterator_next(q, pe);
    test_utf8_char_view(q, qe, s, "á", "oáࢥ🚕");

    q = qe;
    qe = utf8_string_iterator_next(q, pe);
    test_utf8_char_view(q, qe, s, "ࢢ", "oáࢢ🚕");

    q = qe;
    qe = utf8_string_iterator_next(q, pe);
    test_utf8_char_view(q, qe, s, "🚁", "oáࢢ🚁");

    test_utf8_char_view_to_utf8_char_t();
}


std::string::iterator test_utf8_string_iterator_next(std::string::iterator q,
                                                     std::string::iterator pe,
                                                     const std::string& res)
{
    auto qe = utf8_string_iterator_next(q, pe);
    CHECK_TRUE((std::string{q, qe} == res),
               as_str() << "utf8_string_iterator_next(" << res << ")");

    return qe;
}



void test_utf8_string_iterator_next()
{
    std::string s = "aáઅ♁𑇣🚕";

    auto pe = s.end();
    auto qe = test_utf8_string_iterator_next(s.begin(), pe, "a");
    qe = test_utf8_string_iterator_next(qe, pe, "á");
    qe = test_utf8_string_iterator_next(qe, pe, "અ");
    qe = test_utf8_string_iterator_next(qe, pe, "♁");
    qe = test_utf8_string_iterator_next(qe, pe, "𑇣");
    qe = test_utf8_string_iterator_next(qe, pe, "🚕");
    qe = test_utf8_string_iterator_next(qe, pe, "");

    s = "";
    test_utf8_string_iterator_next(s.begin(), s.end(), "");

}



void test_iterator()
{
    test::interfaz("utf8_string_iterator");

    std::string s = "aáઅ♁𑇣🚕";

    {
    auto p        = utf8_string_iterator(s.begin(), s.end());

    CHECK_TRUE(*p == "a", "utf8_string_iterator");
    ++p;
    CHECK_TRUE(*p == "á", "utf8_string_iterator");
    ++p;
    CHECK_TRUE(*p == "અ", "utf8_string_iterator");
    ++p;
    CHECK_TRUE(*p == "♁", "utf8_string_iterator");
    ++p;
    CHECK_TRUE(*p == "𑇣", "utf8_string_iterator");
    ++p;
    CHECK_TRUE(*p == "🚕", "utf8_string_iterator");
    ++p;
    }
    {
    auto p        = utf8_string_iterator(s.begin(), s.end());
    *p = "e";
    CHECK_TRUE(s == "eáઅ♁𑇣🚕", "utf8_string_iterator::operator*");
    }
}

void test_const_iterator()
{
    test::interfaz("const_utf8_string_iterator");

    std::string s = "aáઅ♁𑇣🚕";

    {
    auto p        = const_utf8_string_iterator(s.begin(), s.end());

    CHECK_TRUE(*p == "a", "utf8_string_iterator");
    ++p;
    CHECK_TRUE(*p == "á", "const_utf8_string_iterator");
    ++p;
    CHECK_TRUE(*p == "અ", "const_utf8_string_iterator");
    ++p;
    CHECK_TRUE(*p == "♁", "const_utf8_string_iterator");
    ++p;
    CHECK_TRUE(*p == "𑇣", "const_utf8_string_iterator");
    ++p;
    CHECK_TRUE(*p == "🚕", "const_utf8_string_iterator");
    ++p;
    }
//    {
//    auto p        = const_utf8_string_iterator(s.begin(), s.end());
//    CHECK_NO_COMPILA(*p = "e";)
//    CHECK_TRUE(s == "eáઅ♁𑇣🚕", "utf8_string_iterator::operator*");
//    }
}


void test_utf8_string_size(const utf8_string& s, size_t res)
{
    CHECK_TRUE(s.size() == res, as_str() << "test_size(" << s << ")");
}

void test_utf8_string_size()
{
    test::interfaz("utf8_string::size");

    test_utf8_string_size("", 0);
    test_utf8_string_size("a", 1);
    test_utf8_string_size("á", 1);
    test_utf8_string_size("અ", 1);
    test_utf8_string_size("𑇣", 1);
    test_utf8_string_size("áéíóú", 5);
    test_utf8_string_size("áઅ♁𑇣🚕", 5);

    test_utf8_string_size("abcdefghijklmnñopqrstuvwxyz"
		     "ABCDEFGHIJKLMNÑOPQRSTUVWXYZ"
		     "áéíóú0123456789",
                     69);
}

void test_utf8_string_iterator()
{
    test::interfaz("utf8_string::begin/end");
    utf8_string us{"aáઅ♁𑇣🚕"};

    {
	auto p = us.begin();
	CHECK_TRUE(*p == "a", "utf8_string::iterator");
	++p;
	CHECK_TRUE(*p == "á", "utf8_string::iterator");
	++p;
	CHECK_TRUE(*p == "અ", "utf8_string::iterator");
	++p;
	CHECK_TRUE(*p == "♁", "utf8utf8_string::iterator");
	++p;
	CHECK_TRUE(*p == "𑇣", "utf8_string::iterator");
	++p;
	CHECK_TRUE(*p == "🚕", "utf8_string::iterator");
	++p;
	CHECK_TRUE(p == us.end(), "utf8_string::end");
    }
    {
	auto p = us.begin();
	*p = "e";
	CHECK_TRUE(us == "eáઅ♁𑇣🚕", "utf8_string::operator*");
    }

}

void test_utf8_string_operator_equal()
{
    test::interfaz("utf8_string::operator==");
    utf8_string s{"aáઅ♁𑇣🚕"};
    CHECK_TRUE(s == utf8_string{"aáઅ♁𑇣🚕"}, "utf8_string::operator==");
    CHECK_TRUE(s != utf8_string{"eáઅ♁𑇣🚕"}, "utf8_string::operator!=");

}


void test_utf8_string_push_back(const char* uc0, const char* res0)
{
    utf8_char_t uc{uc0};
    utf8_string res{res0};

    utf8_string s;
    s.push_back(uc);

    CHECK_TRUE(s == res, as_str() << "push_back(" << uc << ")");
}

void test_utf8_string_push_back()
{
    test::interfaz("utf8_string::push_back");
    test_utf8_string_push_back("a", "a");
    test_utf8_string_push_back("á", "á");
    test_utf8_string_push_back("અ", "અ");
    test_utf8_string_push_back("♁", "♁");
    test_utf8_string_push_back("𑇣", "𑇣");
    test_utf8_string_push_back("🚕", "🚕");
    
}

void test_utf8_string_getline()
{
    test::interfaz("utf8_string::getline");
    std::cerr << "FALTA PROBAR AUTOMÁTICAMENTE getline\n";
    std::ifstream in{"pru.txt"};
    if (!in)
	throw std::runtime_error{"Error al abrir el fichero pru.txt"};

    utf8_string line;
    while (getline(in, line))
	std::cout << line << '\n';

}

void test_utf8_find()
{
    test::interfaz("find(utf8_string)");
    utf8_string s{"aáઅ♁𑇣🚕"}; 

    auto p = std::find(s.begin(), s.end(),  "અ");
    utf8_string res = "અ♁𑇣🚕";
    utf8_string y;
    if (p != s.end()){
	for (; p != s.end(); ++p){
	    y.push_back(*p);
	}
    }

    CHECK_TRUE(y == res, "find");
}


void test_utf8_string()
{
    test_utf8_string_operator_equal();
    test_utf8_string_size();
    test_utf8_string_iterator();
    test_utf8_string_push_back();
    test_utf8_string_getline();
    test_utf8_find();
}

void test_split_words(const utf8_string& in,
                      const std::vector<utf8_string>& out)
{
    auto words = split_words(in);
    CHECK_EQUAL_CONTAINERS_C(words, out, "split_words");
}

void test_split_words()
{
    test::interfaz("split_words");

    {
    utf8_string s = "";
    std::vector<utf8_string> res;
    test_split_words(s, res);
    }
    {
    utf8_string s = "aáઅ♁𑇣🚕";
    std::vector<utf8_string> res = {"aáઅ♁𑇣🚕"};
    test_split_words(s, res);
    }
    {
    utf8_string s = "aáઅ♁𑇣🚕 uno. dos áha, adiós";
    std::vector<utf8_string> res = {"aáઅ♁𑇣🚕", "uno.", "dos",  "áha,", "adiós"};
    test_split_words(s, res);
    }
}

void test_map()
{
    std::map<utf8_string, int> m;
    m["á"] = 2;

}

int main()
{
try{
    test::header("alp_utf8.h");

    test_utf8_char_view();
    test_utf8_string_iterator_next();
    test_iterator();
    test_const_iterator();
    test_utf8_string();
    test_split_words();
    test_map();

}catch(std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}
}

