# ICPC Team Reference

Notebook de equipo para competencias estilo ICPC, en C++17 y **sin macros**:
cada bloque se copia tal cual. Incluye el PDF listo para imprimir
([notebook.pdf](notebook.pdf), 20 páginas a 2 columnas con el código
coloreado) y un test aleatorio contra fuerza bruta para cada algoritmo.

## Contenido

| Sección | Algoritmos |
|---|---|
| Utilidades | Plantilla, números aleatorios, stress test, búsqueda binaria (`lower_bound` y `upper_bound` explicados, búsqueda sobre la respuesta y bisección en reales) |
| Estructuras de datos | Segment Tree 4N con lazy, Segment Tree 2N iterativo, Fenwick, Sparse Table, DSU, Sqrt Decomposition, Mo, Heavy-Light Decomposition (con LCA), Centroid Decomposition, ordered set, Li Chao Tree |
| Grafos | Dijkstra, componentes biconexas + puentes + puntos de articulación + bridge tree, SCC + grafo reducido, 2-SAT, Dinic, flujo máximo de costo mínimo, Húngaro, Hopcroft-Karp |
| Matemática | Aritmética modular, Miller-Rabin, Pollard Rho, Euclides extendido (inverso modular, diofántica), teorema chino del resto, Baby Step / Giant Step, criba lineal, Gauss (reales y módulo primo), FFT, NTT, matrices, combinatoria |
| Strings | Hashing, KMP, función Z, Manacher, Suffix Automaton (con DP en el DAG), Suffix Array + LCP, Aho-Corasick |
| Geometría | Punto y primitivas, intersección de segmentos, Convex Hull, puntos antipodales (rotating calipers), círculo mínimo, punto en polígono, par de puntos más cercano |

El PDF termina con una página de fórmulas y una lista de chequeo antes de
enviar.

## Estructura

```
codigo/        un archivo por algoritmo, agrupados por sección
tests/         un test por sección + correr.sh
notebook.tex   fuente del PDF (incluye los archivos de codigo/)
notebook.pdf   PDF generado
```

## Convenciones

- Todo el código asume `#include <bits/stdc++.h>` y `using namespace std;`.
- Índices desde 0. No hay macros ni `typedef` compartidos.
- Si el comentario inicial de un bloque dice *Requiere*, hay que copiar
  también ese otro bloque (por ejemplo, HLD requiere el Segment Tree lazy).
- Los comentarios van sin tildes para evitar problemas de codificación.

## Generar el PDF

Hace falta una distribución de LaTeX (TeX Live o MiKTeX). Compilar dos
veces para que el índice quede actualizado:

```bash
pdflatex notebook.tex
pdflatex notebook.tex
```

El nombre del equipo y la universidad del encabezado se cambian en las
dos líneas `\newcommand{\equipo}` y `\newcommand{\universidad}` al inicio
de `notebook.tex`. Los colores del código están definidos justo debajo.

## Correr los tests

Hace falta `g++` con soporte de C++17:

```bash
bash tests/correr.sh
```

Compila cada test y compara los algoritmos contra una solución de fuerza
bruta con casos aleatorios. Si todo está bien imprime una línea `OK` por
sección.

## Limitaciones conocidas

- Suffix Automaton y Aho-Corasick asumen letras `'a'..'z'`.
- La FFT es exacta mientras los coeficientes del resultado no pasen de
  ~1e14; más allá hay que usar la NTT.
- La intersección de segmentos es la prueba entre dos segmentos; no incluye
  el barrido para detectar cruces entre n segmentos.
- Las funciones recursivas (HLD, centroid, Tarjan) pueden agotar la pila en
  cadenas muy largas si el juez tiene la pila limitada.

## Referencias

Notebooks consultados para elegir el contenido y las variantes:
[KACTL](https://github.com/kth-competitive-programming/kactl),
[Biblioteca UFMG](https://github.com/brunomaletta/Biblioteca),
[El Vasito](https://github.com/mhunicken/icpc-team-notebook-el-vasito) y
[cp-algorithms](https://cp-algorithms.com).
