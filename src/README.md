# Cosas útiles

El paso del tiempo va determinando qué cosas son útiles y cuáles no. A día de
hoy lo que parece ser que tiende a ser estable son los siguientes:


## Matrices
Paquete para operar con matrices. En principio una matriz no es más que un
contenedor bidimensional. No tiene definido el álgebra de matrices así que
realmente no se debería de llamar matrices (std::vector realmente es un
contenedor unidimensional pero se llama vector; el nombre va en esta línea).
Tienen la reestricción de que son
de tamaño fijo, no pudiendo redimensionar. 

### Ejemplos
Si alguno no funciona mira los tests, todos deberían de funcionar.

#### Iteración como contenedor unidimensional
* Método básico:
```
Matrix<int> m{4, 4};
for (auto x: m)
   std::cout << x << '\n';
```

* Método con iteradores:
```
Matrix<int> m{4, 4};
for (auto p = m.begin(); p != m.end(); ++p)
   std::cout << *p << '\n';
```

#### Iteración como contenedor bidimensional
* Con iteradores:
```
Matrix<int> m{4, 4};

for(auto f = m.row_begin(); f != m.row_end(); ++f){
   for(auto x: *f)
      std::cout << x << ' ';
   std::cout << '\n';
}
```

* Con índices:
```
Matrix<int> m{4, 4};

for (size_t i = 0; i < m.rows(); ++i)
   for (size_t j = 0; j < m.cols(); ++j)
      m(i,j) = i*m.cols() + j;
```


### Ficheros
* `alp_rframe_ij.h`: Sistema de coordenadas (i,j). (vectores, rectángulos...)
* `alp_iterator.h`
* `alp_matrix_iterator.h`
* `alp_subcontainer.h`
* `alp_matrix.h`: Definición de la clase `Matrix`.
* `alp_matrix_view.h`
* `alp_submatrix.h`: Obsoleto. Usar `Matrix_view`.
* `alp_matrix_algorithm.h`: Algoritmos que operan en matrices.





## Gestión de parámetros a programas: getopts
En UNIX es habitual pasar los paramétros a los programas con opciones con un
guión.

Ejemplos:
```
> ls -l
> sort -n
```

Este paquete sirve para gestionar los argumentos en la línea de comandos de
forma sencilla.


### Ejemplos

#### Ejemplo 1
Supongamos que queremos hacer un programa llamado `prueba` que tiene 
la siguiente forma de uso:

`prueba [-h] [-o output] [-n num] [-f float] fichero`

Para implementarlo basta con escribir:
```
constexpr char USAGE[] = "prueba [-h] [-o output] [-n num] [-f float] fichero"

ing main(int argc, char* argv[])
{
    bool help;
    std::string output;
    int num = 23;	// valor por defecto '23'
    float f = 3.14; // valor por defecto '3.14'

    alp::Getopts getopts{1, USAGE}; 
    getopts.add_option('h', help);
    getopts.add_option('o', output);
    getopts.add_option('n', num);
    getopts.add_option('f', float);

    auto files = getopts.parse(argc, argv);
}
```
El 1 del constructor de `getopts` indica el número de argumentos obligatorios
que hay que pasar. En este caso esperamos un fichero como argumento por eso
ese 1.

Al llamar a `parse` se hace el parse de la línea de comandos buscando las
opciones indicadas.

Las opciones que no llevan argumento, como `-h`, son de tipo `bool`. Si
`parse` encuentra la opción la marca `true`, y en caso contrario `false`. El
resto de opciones no se modifican los valores si no se pasan. Así, por
ejemplo, si no se pasa la opción `-n` será 23, el valor que tenía. La
`std::string output` como no la hemos inicializado seguirá estando vacía en
caso de no pasar la opción `-o`.

_Cuidado, restricción:_ De momento los argumentos opcionales tienen que ir al
principio. Esto es: 
`> prueba fichero -n 4`

no "parsea" correctamente. Al llamar a `parse` devuelve `files = "fichero -n
4"`.

#### Ejemplo 2

También admite argumentos de más de una letra. Supongamos que queremos que la
línea de comandos sea de la forma:

`prueba [--help] [--output output] [--numero num] fichero`

bastaría con implementarlo:
```
constexpr char USAGE[] = "prueba [--help] [--output output] [--numero num] fichero"

ing main(int argc, char* argv[])
{
    bool help;
    std::string output;
    int num = 23;	// valor por defecto '23'

    alp::Getopts getopts{1, USAGE};
    getopts.add_option("help", help);
    getopts.add_option("output", output);
    getopts.add_option("numero", num);

    auto files = getopts.parse(argc, argv);
}
```
Observar que la única diferencia es que se pasa entre comillas (tipo char \*) 
la opción. Si solo se quiere una letra se pasa un caracter entre apóstrofes 
(tipo char).

### Ficheros
* `alp_getopts.h`



## Strings

Del paquete de strings `as_str()` es una función bastante útil. Sirve para
componer rápidamente cadenas.

### Ejemplo 1
Para mostrar un mensaje con el tipo de test que estamos haciendo:
```
    CHECK_TRUE(X == X_res and Y == Y_res,
               alp::as_str() << "rota(" << x << ", " << y << ")");
```

De esa forma el mensaje que se muestra es  `rota(20, 40)` sabiendo qué valores
de `x` e `y` se están probando a rotar.

Esto mismo se puede hacer lanzando excepciones:
```
    throw std::runtime_error{alp::as_str() << "rotate(" << angle << ")"};
```


### Ficheros
* `alp_string.h`





## Test
Paquete para automatizar los test. Hay un montón de suites por ahí que hacen
lo mismo y seguramente que mejor, así que este no creo que le merezca a nadie
la pena usarlo. Pretende ser mínimo y útil.

### Ejemplos
TODO

### Ficheros
* `alp_test.h`
* `../bin`: scripts para hacer tests automáticos.

