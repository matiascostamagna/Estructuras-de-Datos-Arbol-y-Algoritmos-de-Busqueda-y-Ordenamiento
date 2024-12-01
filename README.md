# AyED-P3-2024
Planificacion de vuelo de un drone

Se dispone de una imagen de un campo de siembra captada por un satelite que ha sido procesada por un programa de analisis de imagenes para el filtrado de malezas y representada por medio de una matriz de ceros, unos y
un valor indeterminado (X), donde el cero representa las posiciones sin maleza, el uno para la presencia de malezas y el valor indeterminado tiene una funcion que se explica mas adelante. 
La imagen contiene 100 x 100 pixeles.
El objetivo del proyecto es planificar el vuelo de un drone que llegue hasta los puntos donde hay malezas y aplique un herbicida. Como los puntos blancos son aquellos donde hay malezas, implica que el drone debera pasar por
ese punto. Sin embargo, dado que el herbicida tiene un costo importante, solo sera aplicado en lugares donde haya una presencia importante de malezas, lo cual se representa por una superficie cuadrada de 3x3 puntos, donde figure el valor 1. Es decir, ud. dispone de un mapa de manchas con valores 1, las cuales pueden ser pequenas (de superficie menor a 3x3) o significativas (de superficie igual a 3x3 para simplificar el caso).
Para encontrar las manchas de malezas debera aplicar un filtro de 3x3 formado por unos, de forma tal que encuentre una mancha de tamano mınimo por medio de las operaciones de conjuntos implementados como mapas de
bits y al mover el filtro. Al encontrar una mancha de esta forma, debera definir su posicion dentro de la imagen en un sistemas de ejes cartesianos 2D como la posicion del punto del centro de la matriz de 3x3. La unidad es el pixel el cual se representa con un valor dentro de la matriz de la imagen.
El punto de partida del vuelo del drone es la coordenada (1,1) que se representa en la esquina superior izquierda del eje de coordenadas cartesianas. El drone debera realizar el vuelo y volver al mismo punto de partida.
Las coordenadas (X,Y) representan fila,columna partiendo desde arriba hacia abajo para el eje vertical y de izquierda a derecha para el eje horizontal.
Una vez determinadas la posiciones de las manchas, debera planificar el recorrido del drone que pase por todas las manchas. La distancia entre manchas sera calculada como el modulo vectorial entre puntos dentro de
un sistema de coordenadas 2D. Sin embargo, y a los efectos de poner mayor realismo al caso, los valores indeterminados en la imagen representan barreras entre los puntos de las manchas, lo cual implica que no hay una camino posible entre dos manchas que tienen una barrera entre medio. Las barreras se representan como filas o columnas de X’s de longitud 4 como mınimo.
Ejemplo de una fraccion de la imagen:
0000000111000110000010001110
0111000111001000011100001110
0111000111000110011100001110
0111000000001000011100000000
00000000000000000000xxxxxxx1
00000111000x0001110000000000
00000111000x0001110000000000
00000111000x0001110000000000
00000000000x0000000000000000
1110000000000000000000000111
1110000000000000000000000111
1110000000000000000000000111
la cual representa 8 manchas con dos barreras que impiden el vuelo entre algunas de las manchas.
Para resolver el recorrido mas corto de vuelo, represente las manchas y distancias como un grafo no dirigido con peso igual a la distancia euclidiana.
El problema se llama ciclo ”Hamiltoneano” ya que debe pasar por todos los vertices una sola vez (salvo el inicio-final), lo cual implica la ausencia de ciclos en la parte media del camino. Ud. debe determinar la mejor opcion para el recorrido, es decir, el ciclo Hamiltoneado de costo minimo. Para ello debera implementar la busqueda del ciclo optimo aplicando un algoritmo de busqueda en amplitud. En este algoritmo debera utilizar una cola de datos.
El programa debera leer la imagen desde un archivo, detectar las manchas y las barreras, calcular la distancia entre las manchas y las opciones de ciclos y el ciclo Hamiltoneano mas corto detectado.
Se recomienda que el mapa no tenga mas de 10 manchas a recorrer porque el algoritmo del ciclo Hamiltoneado tiene una complejidad alta.
