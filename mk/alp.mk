# Reglas para compilar la librería alp

include $(CPP_ENVRULES)


# Variables genéricas de compilación del proyecto
# Recuerdo haber leido que el orden del ld es importante)
ALP_CXXFLAGS=-I$(INSTALL_INC)
ALP_LDFLAGS=-lstdc++fs -L$(INSTALL_LIB) -latd


# Variables genéricas de compilación del proyecto
PROJ_CXXFLAGS=$(ALP_CXXFLAGS)
PROJ_LDFLAGS=$(ALP_LDFLAGS) 


include $(CPP_GENRULES)


