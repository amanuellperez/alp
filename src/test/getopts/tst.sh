#!/bin/bash

# test_ok "args command line" "res_opcion" "res"
test_ok(){
# validemos que no falle
echo -n "Probando xx $1 ..  "
./xx $1 > /dev/null

if [ $? -ne 0 ]
then
    echo "Error al procesar 'xx $1'" >&2
    exit 1
fi

# validemos el resultado
res=$(./xx $1 |grep -w $2|awk '{print $2}')
if [[ $res != $3 ]]
then
    echo
    echo "ERROR. Resultado esperado [$3], resultado obtenido [$res]"
    exit 1
fi

echo "OK"
}

echo "Probando Getopts:"

test_ok "uno" "resto" "uno"
test_ok "uno" "k" "0"
test_ok "-k uno" "k" "1"
test_ok "--time 12.34 uno" "time" "12.34"
test_ok "-n 25 uno" "n" "25"
test_ok "-i input uno" "i" "input"
test_ok "-u 5 uno" "u" "5"
test_ok "-n -5 uno" "n" "-5"
test_ok "--str uno" "str" "1"
test_ok "--number -23 uno" "number" "-23"


