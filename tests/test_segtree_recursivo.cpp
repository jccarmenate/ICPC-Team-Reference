#include "comun.h"
#include "../codigo/estructuras/segtree_recursivo.cpp"

void t_point_update() {
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
        int l = rnd(0, n - 1), r = rnd(l, n - 1), total = 0;
        for (int i = l; i <= r; i++) total += a[i];
        CHECK(query(1, 0, n - 1, l, r) == total);
      }
    }
  }
}

void t_lazy() {
  for (int it = 0; it < 3000; it++) {
    int sz = rnd(1, 40);
    lazy::n = sz;
    lazy::s.assign(sz, 0);
    for (int& x : lazy::s) x = rnd(-1000, 1000);
    lazy::st.assign(4 * sz, 0);
    lazy::lazy.assign(4 * sz, 0);
    lazy::build(1, 0, sz - 1);
    vector<int> a = lazy::s;
    for (int op = 0; op < 60; op++) {
      int l = rnd(0, sz - 1), r = rnd(l, sz - 1);
      if (rnd(0, 1)) {
        int val = rnd(-100, 100);
        lazy::update(1, 0, sz - 1, l, r, val);
        for (int i = l; i <= r; i++) a[i] += val;
      } else {
        int total = 0;
        for (int i = l; i <= r; i++) total += a[i];
        CHECK(lazy::query(1, 0, sz - 1, l, r) == total);
      }
    }
  }
}

int main() {
  t_point_update();
  t_lazy();
  printf("OK segtree_recursivo\n");
}
