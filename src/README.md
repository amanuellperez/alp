# Cosas útiles

El paso del tiempo va determinando qué cosas son útiles y cuáles no. A día de
hoy lo que parece ser que tiende a ser estable son los siguientes:


## Matrices
Paquete para operar con matrices. En principio una matriz no es más que un
contenedor bidimensional. No tiene definido el álgebra de matrices así que
realmente no se debería de llamar matrices. Tienen la reestricción de que son
de tamaño fijo, no pudiendo redimensionar. 

### Ejemplos

#### Iteración como contenedor unidimensional
* Método básico:
```
Matrix<int> m{4, 4};
for (auto x: m)
   cout << x << '\n';
```

* Método con iteradores:
```
Matrix<int> m{4, 4};
for (auto p = m.begin(); p != m.end(); ++p)
   cout << *p << '\n';
   cout << x << '\n';
```

#### Iteración como contenedor bidimensional
* Con iteradores:
```
Matrix<int> m{4, 4};

for(auto f = m.row_begin(); f != m.row_end(); ++f){
   for(auto x: *f)
      cout << x << ' ';
   cout << '\n';
```

* Con índices:
```
Matrix<int> m{4, 4};

for (size_t i = 0; i < m.rows(); ++i)
   for (size_t j = 0; j < m.cols(); ++j)
      m(i,j) = i*m.cols() + j;
```


### Ficheros
* `alp_indices.h`
* `alp_iterator2D.h`
* `alp_iterator.h`
* `alp_matrix.h`
* `alp_matrix_view.h`
* `alp_subcontainer.h`
* `alp_submatrix.h`



## Test
Paquete para automatizar los test. Hay un montón de suites por ahí que hacen
lo mismo y seguramente que mejor, así que este no creo que le merezca a nadie
la pena usarlo. Pretende ser mínimo y útil.

### Ejemplos
TODO

### Ficheros
* `alp_test.h`
* `../bin`: scripts para hacer tests automáticos.



alp_getopts.h
