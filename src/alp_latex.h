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

#pragma once
#ifndef __ALP_LATEX_H__
#define __ALP_LATEX_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz para escribir ficheros en latex.
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 24/04/2017 Escrito
 *
 ****************************************************************************/

#include <string>
#include <ostream>

namespace alp{
namespace Latex{

class Tabla{
public:
    enum Alineamiento {left, right, center};

    /// Define el formato de la tabla. Ejemplo: "|l|c|c|".
//    explicit Tabla(const std::string& formato)
//	:formato_{formato}{}

    /// Define una tabla  con num_columnas, todas con el mismo tipo
    /// de alineamiento, y con el separador sep.
    ///
    /// Ejemplo:
    /// \code
    ///	    Tabla t{3, Tabla::left, '|'};   // con separador
    ///	    Tabla t{3, Tabla::center, ' '}; // sin separador
    /// \endcode
    Tabla(unsigned num_columnas, Alineamiento align, char sep);

    /// Le indicamos que imprima una línea entre cada fila. Por defecto,
    /// no la imprime.
    void linea_entre_filas(bool v) {imprime_hline_ = v;}

    /// Imprime la cabecera de la tabla. Anota que usará out para imprimir
    /// el resto (CUIDADO: 2 responsabilidades!!!)
    void begin(std::ostream& out);

    /// Imprime el final de la tabla.
    void end();

    /// Imprime un elemento en la tabla
    template<typename T>
    Tabla& operator<<(const T& s);

private:
    // formato de la tabla: \begin{tabular}{formato_} = "|l|l|";
    std::string formato_;

    bool imprime_hline_ = false;
    unsigned num_cols_;	// número de columnas
    unsigned i_ = 0;	// columna siguiente donde escribir

    // flujo donde escribimos
    std::ostream* out_ = nullptr;

    // construye el formato
    std::string formato(unsigned num_columnas, Alineamiento align, char sep);

    char align2char(Alineamiento a);


};



template<typename T>
Tabla& Tabla::operator<<(const T& s)
{
    *out_ << s;
    ++i_;

    // ¿hemos llegado al final de la fila?
    if(i_ == num_cols_){
	i_ = 0;
	*out_ << "\\\\";
	if(imprime_hline_)
	    *out_ << "\\hline";
	*out_ << '\n';
    }
    else
	*out_ << " & ";

    return *this;
}

}// namespace Latex
}// namespace alp

#endif
