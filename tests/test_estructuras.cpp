#include "comun.h"
#include "../codigo/estructuras/segtree_lazy.cpp"
#include "../codigo/estructuras/segtree_2n.cpp"
#include "../codigo/estructuras/fenwick.cpp"
#include "../codigo/estructuras/sparse_table.cpp"
#include "../codigo/estructuras/dsu.cpp"
#include "../codigo/estructuras/sqrt_decomp.cpp"
#include "../codigo/estructuras/mo.cpp"
#include "../codigo/estructuras/hld.cpp"
#include "../codigo/estructuras/centroid.cpp"
#include "../codigo/estructuras/ordered_set.cpp"
#include "../codigo/estructuras/lichao.cpp"

// Suma en rango + update en rango contra un arreglo plano.
template <class Estructura>
void probar_rango(int maxn) {
  for (int it = 0; it < 400; it++) {
    int n = rnd(1, maxn);
    vector<long long> a(n);
    for (auto& x : a) x = rnd(-100, 100);
    Estructura st(a);
    for (int op = 0; op < 80; op++) {
      int l = rnd(0, n - 1), r = rnd(l, n - 1);
      if (rnd(0, 1)) {
        long long x = rnd(-50, 50);
        st.update(l, r, x);
        for (int i = l; i <= r; i++) a[i] += x;
      } else {
        long long s = 0;
        for (int i = l; i <= r; i++) s += a[i];
        CHECK(st.query(l, r) == s);
      }
    }
  }
}

void t_segtree_2n() {
  for (int it = 0; it < 400; it++) {
    int n = rnd(1, 40);
    vector<long long> a(n);
    for (auto& x : a) x = rnd(-100, 100);
    SegTree2N st(a);
    for (int op = 0; op < 80; op++) {
      if (rnd(0, 1)) {
        int p = rnd(0, n - 1);
        a[p] = rnd(-100, 100);
        st.update(p, a[p]);
      } else {
        int l = rnd(0, n), r = rnd(l, n);
        long long s = 0;
        for (int i = l; i < r; i++) s += a[i];
        CHECK(st.query(l, r) == s);
      }
    }
  }
}

void t_fenwick() {
  for (int it = 0; it < 400; it++) {
    int n = rnd(1, 40);
    vector<long long> a(n, 0);
    Fenwick fw(n);
    for (int op = 0; op < 80; op++) {
      int tipo = rnd(0, 2);
      if (tipo == 0) {
        int p = rnd(0, n - 1);
        long long v = rnd(0, 20);
        a[p] += v;
        fw.add(p, v);
      } else if (tipo == 1) {
        int l = rnd(0, n - 1), r = rnd(l, n - 1);
        long long s = 0;
        for (int i = l; i <= r; i++) s += a[i];
        CHECK(fw.sum(l, r) == s);
      } else {
        long long k = rnd(1, 300), s = 0;
        int esperado = n;
        for (int i = 0; i < n; i++) {
          s += a[i];
          if (s >= k) { esperado = i; break; }
        }
        CHECK(fw.lower_bound(k) == esperado);
      }
    }
  }
}

void t_sparse() {
  for (int it = 0; it < 400; it++) {
    int n = rnd(1, 70);
    vector<int> a(n);
    for (auto& x : a) x = rnd(-100, 100);
    SparseTable sp(a);
    for (int l = 0; l < n; l++)
      for (int r = l; r < n; r++)
        CHECK(sp.query(l, r) == *min_element(a.begin() + l, a.begin() + r + 1));
  }
}

void t_dsu() {
  for (int it = 0; it < 400; it++) {
    int n = rnd(1, 20);
    DSU d(n);
    vector<int> id(n);
    iota(id.begin(), id.end(), 0);
    for (int op = 0; op < 40; op++) {
      int a = rnd(0, n - 1), b = rnd(0, n - 1);
      if (rnd(0, 1)) {
        bool nuevo = id[a] != id[b];
        CHECK(d.unir(a, b) == nuevo);
        int viejo = id[b], destino = id[a];
        for (int& x : id) if (x == viejo) x = destino;
      } else {
        CHECK((d.find(a) == d.find(b)) == (id[a] == id[b]));
      }
      CHECK(d.comps == (int)set<int>(id.begin(), id.end()).size());
      int cuenta = count(id.begin(), id.end(), id[a]);
      CHECK(d.tam[d.find(a)] == cuenta);
    }
  }
}

void t_mo() {
  for (int it = 0; it < 500; it++) {
    int n = rnd(1, 60), q = rnd(0, 80);
    vector<int> a(n);
    for (auto& x : a) x = rnd(0, 8);
    vector<Consulta> qs;
    for (int i = 0; i < q; i++) {
      int l = rnd(0, n - 1), r = rnd(l, n - 1);
      qs.push_back({l, r, i});
    }
    vector<int> res = mo(a, qs);
    CHECK((int)res.size() == q);
    for (auto& c : qs) {
      set<int> s(a.begin() + c.l, a.begin() + c.r + 1);
      CHECK(res[c.id] == (int)s.size());
    }
  }
}

void t_hld() {
  for (int it = 0; it < 600; it++) {
    int n = rnd(1, 40), raiz = rnd(0, n - 1);
    auto es = arbol_aleatorio(n);
    HLD h(n);
    vector<vector<int>> adj(n);
    for (auto [u, v] : es) {
      h.add_edge(u, v);
      adj[u].push_back(v);
      adj[v].push_back(u);
    }
    h.build(raiz);
    // fuerza bruta: padre y profundidad con BFS
    vector<int> padre(n, -1), prof(n, 0), cola = {raiz};
    for (int i = 0; i < (int)cola.size(); i++) {
      int u = cola[i];
      for (int v : adj[u])
        if (v != padre[u]) {
          padre[v] = u;
          prof[v] = prof[u] + 1;
          cola.push_back(v);
        }
    }
    auto camino = [&](int u, int v) {
      vector<int> a, b;
      while (u != v) {
        if (prof[u] >= prof[v]) { a.push_back(u); u = padre[u]; }
        else { b.push_back(v); v = padre[v]; }
      }
      a.push_back(u);  // el LCA
      a.insert(a.end(), b.begin(), b.end());
      return make_pair(a, u);
    };
    auto en_subarbol = [&](int raiz_sub, int x) {
      while (x != -1 && x != raiz_sub) x = padre[x];
      return x == raiz_sub;
    };
    vector<long long> val(n, 0);
    for (int op = 0; op < 60; op++) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1), tipo = rnd(0, 4);
      long long x = rnd(-50, 50);
      auto [nodos, l] = camino(u, v);
      if (tipo == 0) {
        h.update_camino(u, v, x);
        for (int w : nodos) val[w] += x;
      } else if (tipo == 1) {
        long long s = 0;
        for (int w : nodos) s += val[w];
        CHECK(h.query_camino(u, v) == s);
      } else if (tipo == 2) {
        h.update_subarbol(u, x);
        for (int w = 0; w < n; w++) if (en_subarbol(u, w)) val[w] += x;
      } else if (tipo == 3) {
        long long s = 0;
        for (int w = 0; w < n; w++) if (en_subarbol(u, w)) s += val[w];
        CHECK(h.query_subarbol(u) == s);
      } else {
        CHECK(h.lca(u, v) == l);
        CHECK(h.dist(u, v) == (int)nodos.size() - 1);
      }
    }
  }
}

void t_centroid() {
  for (int it = 0; it < 600; it++) {
    int n = rnd(1, 50);
    auto es = arbol_aleatorio(n);
    Centroid c(n);
    vector<vector<int>> adj(n);
    for (auto [u, v] : es) {
      c.add_edge(u, v);
      adj[u].push_back(v);
      adj[v].push_back(u);
    }
    c.build();
    int niveles = 0;
    while ((1 << niveles) <= n) niveles++;
    CHECK((int)c.dist.size() <= niveles);  // altura O(log n)
    CHECK(count(c.padre.begin(), c.padre.end(), -1) == 1);
    // distancias por BFS desde cada vertice
    vector<vector<int>> d(n, vector<int>(n, -1));
    for (int s = 0; s < n; s++) {
      vector<int> cola = {s};
      d[s][s] = 0;
      for (int i = 0; i < (int)cola.size(); i++)
        for (int v : adj[cola[i]])
          if (d[s][v] == -1) {
            d[s][v] = d[s][cola[i]] + 1;
            cola.push_back(v);
          }
    }
    vector<bool> marcado(n, false);
    for (int op = 0; op < 40; op++) {
      int v = rnd(0, n - 1);
      if (rnd(0, 2) == 0) {
        c.marcar(v);
        marcado[v] = true;
      } else {
        int mejor = INT_MAX / 2;
        for (int w = 0; w < n; w++)
          if (marcado[w]) mejor = min(mejor, d[v][w]);
        int res = c.mas_cercano(v);
        if (mejor == INT_MAX / 2) CHECK(res >= INT_MAX / 2);
        else CHECK(res == mejor);
      }
    }
  }
}

void t_ordered_set() {
  for (int it = 0; it < 200; it++) {
    ordered_set<int> s;
    set<int> ref;
    for (int op = 0; op < 60; op++) {
      int x = rnd(0, 30);
      if (rnd(0, 2) == 0) { s.erase(x); ref.erase(x); }
      else { s.insert(x); ref.insert(x); }
      vector<int> v(ref.begin(), ref.end());
      int y = rnd(0, 31);
      CHECK((int)s.order_of_key(y) == lower_bound(v.begin(), v.end(), y) - v.begin());
      if (!v.empty()) {
        int k = rnd(0, (int)v.size() - 1);
        CHECK(*s.find_by_order(k) == v[k]);
      }
    }
  }
}

void t_lichao() {
  for (int it = 0; it < 500; it++) {
    long long lo = rnd(-60, 20), hi = rnd(lo, lo + 80);
    LiChao lc(lo, hi);
    vector<pair<long long, long long>> rectas;
    for (int op = 0; op < 50; op++) {
      if (rnd(0, 2) == 0) {
        long long m = rnd(-30, 30), b = rnd(-1000, 1000);
        lc.add(m, b);
        rectas.push_back({m, b});
      } else {
        long long x = rnd(lo, hi), mejor = LLONG_MAX;
        for (auto [m, b] : rectas) mejor = min(mejor, m * x + b);
        CHECK(lc.query(x) == mejor);
      }
    }
  }
  // coordenadas grandes
  LiChao lc(-1000000000LL, 1000000000LL);
  vector<pair<long long, long long>> rectas;
  for (int op = 0; op < 3000; op++) {
    long long m = rnd(-1000000, 1000000), b = rnd(-1000000000000LL, 1000000000000LL);
    lc.add(m, b);
    rectas.push_back({m, b});
    long long x = rnd(-1000000000LL, 1000000000LL), mejor = LLONG_MAX;
    for (auto [mm, bb] : rectas) mejor = min(mejor, mm * x + bb);
    CHECK(lc.query(x) == mejor);
  }
}

int main() {
  probar_rango<SegTree>(40);
  probar_rango<SqrtDecomp>(70);
  t_segtree_2n();
  t_fenwick();
  t_sparse();
  t_dsu();
  t_mo();
  t_hld();
  t_centroid();
  t_ordered_set();
  t_lichao();
  printf("OK estructuras\n");
}
