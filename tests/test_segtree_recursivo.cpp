#include "comun.h"
#include "../codigo/estructuras/segtree_recursivo.cpp"

void t_puntual() {
  for (int it = 0; it < 3000; it++) {
    n = rnd(1, 40);
    s.assign(n, 0);
    for (int& x : s) x = rnd(-1000, 1000);
    st.assign(4 * n, 0);
    build(1, 0, n - 1);
    vector<int> a = s;
    for (int op = 0; op < 60; op++) {
      if (rnd(0, 1)) {
        int pos = rnd(0, n - 1), val = rnd(-1000, 1000);
        update(1, 0, n - 1, val, pos);
        a[pos] = val;
      } else {
        int l = rnd(0, n - 1), r = rnd(l, n - 1), suma = 0;
        for (int i = l; i <= r; i++) suma += a[i];
        CHECK(query(1, 0, n - 1, l, r) == suma);
      }
    }
  }
}

void t_lazy() {
  for (int it = 0; it < 3000; it++) {
    int tam = rnd(1, 40);
    lazy::n = tam;
    lazy::s.assign(tam, 0);
    for (int& x : lazy::s) x = rnd(-1000, 1000);
    lazy::st.assign(4 * tam, 0);
    lazy::lazy.assign(4 * tam, 0);
    lazy::build(1, 0, tam - 1);
    vector<int> a = lazy::s;
    for (int op = 0; op < 60; op++) {
      int l = rnd(0, tam - 1), r = rnd(l, tam - 1);
      if (rnd(0, 1)) {
        int val = rnd(-100, 100);
        lazy::update(1, 0, tam - 1, l, r, val);
        for (int i = l; i <= r; i++) a[i] += val;
      } else {
        int suma = 0;
        for (int i = l; i <= r; i++) suma += a[i];
        CHECK(lazy::query(1, 0, tam - 1, l, r) == suma);
      }
    }
  }
}

int main() {
  t_puntual();
  t_lazy();
  printf("OK segtree_recursivo\n");
}
