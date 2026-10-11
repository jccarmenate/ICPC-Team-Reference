// Utilidades compartidas por los tests (no va en el notebook).
#include <bits/stdc++.h>
using namespace std;

mt19937_64 gen_test(12345);
long long rnd(long long l, long long r) {
  return uniform_int_distribution<long long>(l, r)(gen_test);
}
#define CHECK(c)                                                  \
  do {                                                            \
    if (!(c)) {                                                   \
      printf("FALLO %s:%d: %s\n", __FILE__, __LINE__, #c);        \
      exit(1);                                                    \
    }                                                             \
  } while (0)

// Arbol aleatorio de n vertices con etiquetas mezcladas.
vector<pair<int, int>> arbol_aleatorio(int n) {
  vector<int> etiqueta(n);
  iota(etiqueta.begin(), etiqueta.end(), 0);
  shuffle(etiqueta.begin(), etiqueta.end(), gen_test);
  int forma = rnd(0, 2);  // 0 = aleatorio, 1 = casi cadena, 2 = estrella
  vector<pair<int, int>> es;
  for (int i = 1; i < n; i++) {
    int p = forma == 0 ? rnd(0, i - 1)
          : forma == 1 ? rnd(max(0, i - 2), i - 1)
                       : rnd(0, min(i - 1, 1));
    es.push_back({etiqueta[p], etiqueta[i]});
  }
  return es;
}
