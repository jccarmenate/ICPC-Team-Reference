#include "comun.h"
#include "../codigo/estructuras/merge_sort_tree.cpp"

int main() {
  for (int it = 0; it < 3000; it++) {
    int sz = rnd(1, 40);
    vector<int> a(sz);
    for (int& x : a) x = rnd(-20, 20);
    mergesort::n = sz;
    mergesort::s = a;
    mergesort::build(1, 0, sz - 1);
    for (int q = 0; q < 40; q++) {
      int l = rnd(0, sz - 1), r = rnd(l, sz - 1), value = rnd(-22, 22);
      int tally = 0;
      for (int i = l; i <= r; i++) tally += a[i] <= value;
      CHECK(mergesort::lesskquery(1, 0, sz - 1, l, r, value) == tally);
      vector<int> sub(a.begin() + l, a.begin() + r + 1);
      sort(sub.begin(), sub.end());
      int k = rnd(1, r - l + 1);
      CHECK(mergesort::k_th_smoller(l, r, k) == sub[k - 1]);
    }
  }
  // tamano grande: reconstruir sobre el mismo arreglo global
  int sz = 100000;
  vector<int> a(sz);
  for (int& x : a) x = rnd(-1000000000, 1000000000);
  mergesort::n = sz;
  mergesort::s = a;
  mergesort::build(1, 0, sz - 1);
  for (int q = 0; q < 200; q++) {
    int l = rnd(0, sz - 1), r = rnd(l, min(sz - 1, l + 2000));
    vector<int> sub(a.begin() + l, a.begin() + r + 1);
    sort(sub.begin(), sub.end());
    int k = rnd(1, r - l + 1);
    CHECK(mergesort::k_th_smoller(l, r, k) == sub[k - 1]);
  }
  printf("OK merge_sort_tree\n");
}
