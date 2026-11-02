#include "comun.h"
#include "../codigo/grafos/euler.cpp"

// Fuerza bruta: existe un recorrido que usa todas las aristas?
bool existe_bruto(int n, const vector<pair<int, int>>& es, bool dir) {
  int m = es.size();
  if (m == 0) return true;
  vector<bool> usada(m, false);
  function<bool(int, int)> rec = [&](int u, int usadas) {
    if (usadas == m) return true;
    for (int i = 0; i < m; i++) {
      if (usada[i]) continue;
      int v = -1;
      if (es[i].first == u) v = es[i].second;
      else if (!dir && es[i].second == u) v = es[i].first;
      if (v == -1) continue;
      usada[i] = true;
      bool ok = rec(v, usadas + 1);
      usada[i] = false;
      if (ok) return true;
    }
    return false;
  };
  for (int s = 0; s < n; s++)
    if (rec(s, 0)) return true;
  return false;
}

int main() {
  for (int it = 0; it < 20000; it++) {
    int n = rnd(1, 5), m = rnd(0, 7);
    bool dir = rnd(0, 1);
    Euler e(n, dir);
    vector<pair<int, int>> es;
    for (int i = 0; i < m; i++) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1);
      es.push_back({u, v});
      e.add_edge(u, v);
    }
    vector<int> c = e.camino();
    CHECK(!c.empty() == existe_bruto(n, es, dir));
    if (c.empty()) continue;
    CHECK((int)c.size() == m + 1);
    // cada paso usa una arista distinta y se usan todas
    multiset<pair<int, int>> libres(es.begin(), es.end());
    for (int i = 0; i + 1 < (int)c.size(); i++) {
      int u = c[i], v = c[i + 1];
      auto p = libres.find({u, v});
      if (p == libres.end() && !dir) p = libres.find({v, u});
      CHECK(p != libres.end());
      libres.erase(p);
    }
    CHECK(libres.empty());
  }
  // grande: ciclo dirigido de 200000 aristas
  int n = 200000;
  Euler e(n, true);
  for (int i = 0; i < n; i++) e.add_edge(i, (i + 1) % n);
  vector<int> c = e.camino();
  CHECK((int)c.size() == n + 1 && c.front() == c.back());
  printf("OK euler\n");
}
