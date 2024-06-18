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

#pragma once

#ifndef __ALP_TIME_H__
#define __ALP_TIME_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones para manejar tiempo, fechas...
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 25/01/2019 Escrito
 *
 ****************************************************************************/
#include <chrono>
#include <thread>

// El sistema internacional define las abreviaturas a usar.
// 30s = 30 segundos y siempre tiene este significado. 
//	 Ninguna aplicación debería de usar 30s para indicar otra cosa ya que
//	 sería confuso. El S.I. no se toca!!! m es metro, g es gramo...
using namespace std::chrono_literals;


namespace alp{


/*!
 *  \brief  Un cronométro para poder medir tiempos.
 *
 *
 *  Ejemplo:
 *  \code
 *	Cronometro c;
 *	c.start();
 *	...
 *	c.stop();
 *	cout << "Tiempo de ejecución: " << c.milisegundos() << " ms"<<endl;
 *  \endcode
 */
class Cronometro{
public:
    void start() {t0 = std::chrono::steady_clock::now();}
    void stop()  {t1 = std::chrono::steady_clock::now();}

    // TODO: devolver no int sino std::chrono::steady_clock::rep
//    std::chrono::steady_clock::rep milisegundos() const
    int milisegundos() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0)
            .count();
    }

  private:
    std::chrono::steady_clock::time_point t0;
    std::chrono::steady_clock::time_point t1;

};

/*!
 *  \brief  Cronómetro de alta resolución.
 *
 *  Usarlo solo en benchmark y cosas así.
 *
 */
class Cronometro_high_resolution{
public:
    void start() 
    { t0 = std::chrono::high_resolution_clock::now(); }

    void stop() 
    { t1 = std::chrono::high_resolution_clock::now(); }

    double nanosegundos() 
    {
	return std::chrono::duration_cast
		<std::chrono::duration<double, std::nano>>(t1-t0).count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> t0;
    std::chrono::time_point<std::chrono::high_resolution_clock> t1;
};





/*****************************************************************************
 * 
 *   - EJEMPLOS:
 *		Program::sleep_for(1s);   // duerme 1 segundo
 *		Program::sleep_for(100ms);   // duerme 100 ms
 *		Program::sleep_for(1);	// error: no se indican unidades!!!
 *
 ***************************************************************************/
/*!
 *  \brief  Duerme el programa por un tiempo.
 *
 *  Ejemplo:
 *  \code
 *	sleep_for(std::chrono::milliseconds{100});
 *
 *  // o más compacto
 *	using namespace std::chrono_literals;
 *	sleep_for(100ms);
 *
 *  \endcode
 *
 */
template<typename Duration>
inline void sleep_for(Duration t)  
{
    ::std::this_thread::sleep_for(t);
}





/// Indica si el año year es bisiesto o no
bool es_bisiesto(int year);

}// namespace

#endif

