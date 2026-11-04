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
      printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #c);        \
      exit(1);                                                    \
    }                                                             \
  } while (0)

// Arbol aleatorio de n vertices con etiquetas mezcladas.
vector<pair<int, int>> random_tree(int n) {
  vector<int> label(n);
  iota(label.begin(), label.end(), 0);
  shuffle(label.begin(), label.end(), gen_test);
  int shape = rnd(0, 2);  // 0 = aleatorio, 1 = casi cadena, 2 = estrella
  vector<pair<int, int>> es;
  for (int i = 1; i < n; i++) {
    int p = shape == 0 ? rnd(0, i - 1)
          : shape == 1 ? rnd(max(0, i - 2), i - 1)
                       : rnd(0, min(i - 1, 1));
    es.push_back({label[p], label[i]});
  }
  return es;
}
