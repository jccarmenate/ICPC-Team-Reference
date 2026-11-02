#include "comun.h"
#include "../codigo/estructuras/merge_sort_tree.cpp"

int main() {
  for (int it = 0; it < 3000; it++) {
    int tam = rnd(1, 40);
    vector<int> a(tam);
    for (int& x : a) x = rnd(-20, 20);
    mergesort::n = tam;
    mergesort::s = a;
    mergesort::build(1, 0, tam - 1);
    for (int q = 0; q < 40; q++) {
      int l = rnd(0, tam - 1), r = rnd(l, tam - 1), value = rnd(-22, 22);
      int cuenta = 0;
      for (int i = l; i <= r; i++) cuenta += a[i] <= value;
      CHECK(mergesort::lesskquery(1, 0, tam - 1, l, r, value) == cuenta);
      vector<int> sub(a.begin() + l, a.begin() + r + 1);
      sort(sub.begin(), sub.end());
      int k = rnd(1, r - l + 1);
      CHECK(mergesort::k_th_smoller(l, r, k) == sub[k - 1]);
    }
  }
  // tamano grande: reconstruir sobre el mismo arreglo global
  int tam = 100000;
  vector<int> a(tam);
  for (int& x : a) x = rnd(-1000000000, 1000000000);
  mergesort::n = tam;
  mergesort::s = a;
  mergesort::build(1, 0, tam - 1);
  for (int q = 0; q < 200; q++) {
    int l = rnd(0, tam - 1), r = rnd(l, min(tam - 1, l + 2000));
    vector<int> sub(a.begin() + l, a.begin() + r + 1);
    sort(sub.begin(), sub.end());
    int k = rnd(1, r - l + 1);
    CHECK(mergesort::k_th_smoller(l, r, k) == sub[k - 1]);
  }
  printf("OK merge_sort_tree\n");
}
