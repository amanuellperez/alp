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

#include "alp_latex.h"

#include "alp_exception.h"

using std::string;

namespace alp{
namespace Latex{

Tabla::Tabla(unsigned num_columnas, Alineamiento align, char sep)
    :formato_{formato(num_columnas, align, sep)}
    , num_cols_{num_columnas}
{}

std::string Tabla::formato(unsigned num_columnas, Alineamiento align, char separador)
{
    std::string s;

    string c{align2char(align)};
    string sep{separador};

    for(unsigned i = 0; i < num_columnas; ++i)
	s += sep + c;

    s+= sep;

    return s;
}

char Tabla::align2char(Alineamiento a)
{
    switch(a){
	case left: return 'l';
	case right: return 'r';
	case center: return 'c';
    }

    throw alp::Imposible_llegar_aqui{__FILE__, __LINE__, "Tabla::align2char"};
}


void Tabla::begin(std::ostream& out)
{
    out_ = &out; 
    *out_ << "\\begin{tabular}{" << formato_ << "}\n";
    if(imprime_hline_)
	    *out_ << "\\hline\n";
}

void Tabla::end()
{
    if(i_ != 0){
	*out_ << "\\\\";
	if(imprime_hline_)
		*out_ << "\\hline";
	*out_ << '\n';
    }

    *out_ << "\\end{tabular}\n";
}

}}// namespace alp::Latex
