#!/usr/bin/awk -f

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

{
    if ($2 != $4){
	print "ERROR, check no válido: "$2" != "$4;
	print "\tDetalles: "$1;
	error=1;
	exit 1
	}
}

#END{
#    if (!error){
#	print "OK";
#    }
#}

