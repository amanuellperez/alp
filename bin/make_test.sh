#!/bin/bash
# Copyright (C) 2020 Manuel Perez <manuel2perez@proton.me>
#
# This file is part of the MCU++ Library.
#
# MCU++ Library is a free library: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#*****************************************************************************
#
#   - DESCRIPCION: Ejecuta make y prueba 'xx'
#
#   - LINEA DE COMANDOS:
#		
#
#   - COMENTARIOS: 
#
#   - HISTORIA:
#     alp  - 31/03/2019	Escrito
#
#*****************************************************************************
USAGE=\
"Ayuda:\n Compila el directorio actual y ejecuta el nom_ejecutable. \n
En caso de que algo falle finaliza\n\n
Forma de uso: `basename $0` nom_ejecutable\n
nom_ejecutable: nombre del ejecutable que generamos al compilar.\n\n"



#*****************************************************************************
#				FUNCIONES
#*****************************************************************************
# test_exit_code directorio
test_exit_code(){
    local code=$?
    if [ "${code}" -ne 0 ]
    then
	echo ""
	echo "--------------------------------------------"
	echo "Error en el directorio: [$1]"
	echo "Ha fallado la $2"
	echo ""
	exit ${code}
    fi
}

#*****************************************************************************
#				    MAIN
#*****************************************************************************

if [ "$#" -ne 1 ]
then
    echo -e ${USAGE}
    exit 1
fi


FIC_BIN=$1

make all
test_exit_code ${FIC_BIN} "compilación"

./${FIC_BIN}
test_exit_code ${FIC_BIN} "prueba"






