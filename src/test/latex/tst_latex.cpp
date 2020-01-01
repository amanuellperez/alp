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

#include "../alp_latex.h"
#include "../alp_error.h"

using namespace std;

int main()
{
try{

    using alp::Latex::Tabla;
    Tabla t{3, Tabla::left, '|'};
    t.linea_entre_filas(true);
    t.begin(std::cout);
    t << "uno" << "dos" << "tres" << "cuatro" << 5 << 6;
    t.end();

}catch(alp::Excepcion e)
{
    std::cerr << e.what() << '\n';
}
    return 0;
}
