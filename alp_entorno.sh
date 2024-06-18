# -------------------------
# Estructura de directorios
# -------------------------
ALP_ROOT=${MY_HOME}/prj/git/alp		; export ALP_ROOT
ALP_SRC=${ALP_ROOT}/src				; export ALP_SRC
ALP_MK=${ALP_ROOT}/mk	    ; export ALP_MK


# Reglas de compilación de alp
ALP_COMPRULES=${ALP_MK}/alp.mk	; export ALP_COMPRULES

# Reglas genéricas para compilar atd.
# Aunque en principio atd es otro proyecto, en la realidad atd y alp son
# el mismo entorno. La diferencia es que todo lo que está dentro de atd
# tiene que poder compilar para microcontroladores (no usa excepciones ni
# memoria dinámica) mientras que alp usa toda la std.
ATD_GENRULES=${ALP_COMPRULES}	; export ATD_GENRULES

# PATH
PATH=${PATH}:{ALP_ROOT}/bin ;	export PATH

# ------------------------------------------------------
# CPATH = es donde busca g++ los archivos .h (opción -I)
# ------------------------------------------------------
# Este lo defino en cpp_entorno.sh
#CPATH=${CPATH}:${ALP_HOME}/include
#export CPATH

# --------------------------------------------------------
# LIBRARY_PATH = donde busca g++ las librerias estáticas
# --------------------------------------------------------
# Este lo defino en cpp_entorno.sh
#LIBRARY_PATH=${ALP_HOME}/lib
#
#export LIBRARY_PATH 


# Alias
alias s='cd ${ALP_SRC}'

# Habilitamos los cores
#ulimit -c unlimited


