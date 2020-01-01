# -------------------------
# Estructura de directorios
# -------------------------
ALP_ROOT=${MY_HOME}/prj/git/alp		; export ALP_ROOT
ALP_SRC=${ALP_ROOT}/src				; export ALP_SRC
ALP_MK=${ALP_ROOT}/mk	    ; export ALP_MK


# Reglas de compilación de alp
ALP_COMPRULES=${ALP_MK}/alp.mk	; export ALP_COMPRULES

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


