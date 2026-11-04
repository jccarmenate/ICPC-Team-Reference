// Compara las estructuras de datos del notebook contra KACTL.
#include "kactl_begin.h"
namespace kseg {
#include "SegmentTreeSum.h"  // parche de SegmentTree.h: suma en long long
}
namespace kft {
#include "data-structures/FenwickTree.h"
}
namespace klca {
#include "graph/LCA.h"  // incluye data-structures/RMQ.h
}
namespace kuf {
#include "data-structures/UnionFind.h"
}
namespace klc {
#include "data-structures/LineContainer.h"
}
#include "kactl_end.h"
#include "../../codigo/estructuras/segtree_2n.cpp"
#include "../../codigo/estructuras/segtree_recursivo.cpp"
#include "../../codigo/estructuras/fenwick.cpp"
#include "../../codigo/estructuras/sparse_table.cpp"
#include "../../codigo/estructuras/dsu.cpp"
#include "../../codigo/estructuras/lichao.cpp"
#include "../../codigo/estructuras/segtree_lazy.cpp"
#include "../../codigo/estructuras/hld.cpp"

void t_segtree() {
  for (int it = 0; it < 2000; it++) {
    int len = rnd(1, 50);
    vector<long long> a(len);
    for (auto& x : a) x = rnd(-1000000000, 1000000000);
    SegTree2N ours(a);
    kseg::Tree ref(len);
    for (int i = 0; i < len; i++) ref.update(i, a[i]);
    // version del equipo (int): valores chicos para que no desborde
    vector<int> small(len);
    for (auto& x : small) x = rnd(-1000, 1000);
    n = len;
    s = small;
    st.assign(4 * len, 0);
    build(1, 0, len - 1);
    kseg::Tree ref_small(len);
    for (int i = 0; i < len; i++) ref_small.update(i, small[i]);
    for (int op = 0; op < 100; op++) {
      int p = rnd(0, len - 1);
      if (rnd(0, 1)) {
        long long v = rnd(-1000000000, 1000000000);
        int w = rnd(-1000, 1000);
        ours.update(p, v);
        ref.update(p, v);
        update(1, 0, len - 1, w, p);
        ref_small.update(p, w);
      } else {
        int l = rnd(0, len - 1), r = rnd(l + 1, len);
        CHECK(ours.query(l, r) == ref.query(l, r));
        CHECK(query(1, 0, len - 1, l, r - 1) == ref_small.query(l, r));
      }
    }
  }
  int N = 200000, Q = 200000;
  vector<long long> a(N);
  for (auto& x : a) x = rnd(-1000, 1000);
  vector<array<long long, 3>> ops(Q);
  for (auto& o : ops) {
    o[0] = rnd(0, 1);
    o[1] = rnd(0, N - 1);
    o[2] = o[0] ? rnd(-1000, 1000) : rnd(o[1] + 1, N);
  }
  vector<long long> got, expected, got_team;
  double t1 = measure([&] {
    SegTree2N t(a);
    for (auto& o : ops)
      if (o[0]) t.update(o[1], o[2]);
      else got.push_back(t.query(o[1], o[2]));
  });
  double t2 = measure([&] {
    kseg::Tree t(N);
    for (int i = 0; i < N; i++) t.update(i, a[i]);
    for (auto& o : ops)
      if (o[0]) t.update(o[1], o[2]);
      else expected.push_back(t.query(o[1], o[2]));
  });
  double t3 = measure([&] {
    n = N;
    s.assign(a.begin(), a.end());
    st.assign(4 * N, 0);
    build(1, 0, N - 1);
    for (auto& o : ops)
      if (o[0]) update(1, 0, N - 1, o[2], o[1]);
      else got_team.push_back(query(1, 0, N - 1, o[1], o[2] - 1));
  });
  CHECK(got == expected && got_team == expected);
  report("SegTree2N, 2e5 operaciones", t1, t2);
  report("Segment Tree recursivo, 2e5 operaciones", t3, t2);
}

void t_fenwick() {
  for (int it = 0; it < 3000; it++) {
    int len = rnd(1, 50);
    Fenwick f(len);
    kft::FT g(len);
    for (int op = 0; op < 100; op++) {
      int kind = rnd(0, 2);
      if (kind == 0) {
        int i = rnd(0, len - 1);
        long long v = rnd(0, 20);
        f.add(i, v);
        g.update(i, v);
      } else if (kind == 1) {
        int l = rnd(0, len - 1), r = rnd(l, len - 1);
        CHECK(f.sum(l, r) == g.query(r + 1) - g.query(l));
      } else {
        long long k = rnd(1, 600);
        CHECK(f.lower_bound(k) == g.lower_bound(k));
      }
    }
  }
  int N = 200000, Q = 1000000;
  vector<array<long long, 3>> ops(Q);
  for (auto& o : ops) {
    o[0] = rnd(0, 2);
    o[1] = rnd(0, N - 1);
    o[2] = o[0] == 0 ? rnd(0, 1000) : o[0] == 1 ? rnd(o[1], N - 1) : rnd(1, 100000000);
  }
  vector<long long> got, expected;
  double t1 = measure([&] {
    Fenwick f(N);
    for (auto& o : ops)
      if (o[0] == 0) f.add(o[1], o[2]);
      else if (o[0] == 1) got.push_back(f.sum(o[1], o[2]));
      else got.push_back(f.lower_bound(o[2]));
  });
  double t2 = measure([&] {
    kft::FT g(N);
    for (auto& o : ops)
      if (o[0] == 0) g.update(o[1], o[2]);
      else if (o[0] == 1) expected.push_back(g.query(o[2] + 1) - g.query(o[1]));
      else expected.push_back(g.lower_bound(o[2]));
  });
  CHECK(got == expected);
  report("Fenwick, 1e6 operaciones", t1, t2);
}

void t_sparse() {
  for (int it = 0; it < 500; it++) {
    int len = rnd(1, 70);
    vector<int> a(len);
    for (int& x : a) x = rnd(-1000000000, 1000000000);
    SparseTable sp(a);
    klca::RMQ<int> rm(a);
    for (int l = 0; l < len; l++)
      for (int r = l; r < len; r++) CHECK(sp.query(l, r) == rm.query(l, r + 1));
  }
  int N = 200000, Q = 1000000;
  vector<int> a(N);
  for (int& x : a) x = rnd(-1000000000, 1000000000);
  vector<pair<int, int>> qs(Q);
  for (auto& [l, r] : qs) { l = rnd(0, N - 1); r = rnd(l, N - 1); }
  long long h1 = 0, h2 = 0;
  double t1 = measure([&] {
    SparseTable sp(a);
    for (auto [l, r] : qs) h1 = h1 * 31 + sp.query(l, r);
  });
  double t2 = measure([&] {
    klca::RMQ<int> rm(a);
    for (auto [l, r] : qs) h2 = h2 * 31 + rm.query(l, r + 1);
  });
  CHECK(h1 == h2);
  report("Sparse Table, 2e5 + 1e6 consultas", t1, t2);
}

void t_dsu() {
  for (int it = 0; it < 2000; it++) {
    int len = rnd(1, 30);
    DSU d(len);
    kuf::UF u(len);
    for (int op = 0; op < 60; op++) {
      int a = rnd(0, len - 1), b = rnd(0, len - 1);
      if (rnd(0, 1)) CHECK(d.unite(a, b) == u.join(a, b));
      else CHECK((d.find(a) == d.find(b)) == u.sameSet(a, b));
      CHECK(d.sz[d.find(a)] == u.size(a));
    }
  }
  int N = 200000, Q = 1000000;
  vector<array<int, 3>> ops(Q);
  for (auto& o : ops) o = {(int)rnd(0, 1), (int)rnd(0, N - 1), (int)rnd(0, N - 1)};
  long long h1 = 0, h2 = 0;
  double t1 = measure([&] {
    DSU d(N);
    for (auto& o : ops)
      h1 = h1 * 3 + (o[0] ? d.unite(o[1], o[2]) : d.find(o[1]) == d.find(o[2]));
  });
  double t2 = measure([&] {
    kuf::UF u(N);
    for (auto& o : ops)
      h2 = h2 * 3 + (o[0] ? u.join(o[1], o[2]) : u.sameSet(o[1], o[2]));
  });
  CHECK(h1 == h2);
  report("DSU, 1e6 operaciones", t1, t2);
}

void t_lichao() {
  const long long X = 1000000;
  for (int it = 0; it < 500; it++) {
    LiChao lc(-X, X);
    klc::LineContainer hull;  // KACTL da maximos: se insertan rectas negadas
    for (int op = 0; op < 200; op++) {
      if (hull.empty() || rnd(0, 2) == 0) {
        long long m = rnd(-X, X), b = rnd(-1000000000000LL, 1000000000000LL);
        lc.add(m, b);
        hull.add(-m, -b);
      } else {
        long long x = rnd(-X, X);
        CHECK(lc.query(x) == -hull.query(x));
      }
    }
  }
  int Q = 200000;
  vector<array<long long, 3>> ops(Q);
  for (int i = 0; i < Q; i++)
    ops[i] = {i == 0 ? 1 : rnd(0, 1), rnd(-X, X), rnd(-1000000000000LL, 1000000000000LL)};
  vector<long long> got, expected;
  double t1 = measure([&] {
    LiChao lc(-X, X);
    for (auto& o : ops)
      if (o[0]) lc.add(o[1], o[2]);
      else got.push_back(lc.query(o[1]));
  });
  double t2 = measure([&] {
    klc::LineContainer hull;
    for (auto& o : ops)
      if (o[0]) hull.add(-o[1], -o[2]);
      else expected.push_back(-hull.query(o[1]));
  });
  CHECK(got == expected);
  report("Li Chao, 2e5 operaciones", t1, t2);
}

// LCA y distancia del HLD contra LCA de KACTL. Retorna los tiempos.
pair<double, double> compare_lca(int len, bool chain, int queries) {
  vector<pair<int, int>> es;
  if (chain) for (int i = 1; i < len; i++) es.push_back({i - 1, i});
  else es = random_tree(len);
  HLD h(len);
  vector<vi> g(len);
  for (auto [u, v] : es) {
    h.add_edge(u, v);
    g[u].push_back(v);
    g[v].push_back(u);
  }
  vector<pair<int, int>> qs(queries);
  for (auto& [a, b] : qs) { a = rnd(0, len - 1); b = rnd(0, len - 1); }
  vector<int> got, expected;
  double t1 = measure([&] {
    h.build(0);
    for (auto [a, b] : qs) got.push_back(h.lca(a, b));
  });
  double t2 = measure([&] {
    klca::LCA l(g);
    for (auto [a, b] : qs) expected.push_back(l.lca(a, b));
  });
  CHECK(got == expected);
  vector<int> depth(len, -1), order = {0};
  depth[0] = 0;
  for (int i = 0; i < (int)order.size(); i++)
    for (int v : g[order[i]])
      if (depth[v] == -1) { depth[v] = depth[order[i]] + 1; order.push_back(v); }
  for (int i = 0; i < queries; i++) {
    auto [a, b] = qs[i];
    CHECK(h.dist(a, b) == depth[a] + depth[b] - 2 * depth[got[i]]);
  }
  return {t1, t2};
}

void t_lca() {
  for (int it = 0; it < 2000; it++) compare_lca(rnd(1, 60), rnd(0, 3) == 0, 50);
  auto [t1, t2] = compare_lca(200000, false, 200000);
  report("HLD lca, arbol aleatorio 2e5", t1, t2);
  auto [t3, t4] = compare_lca(200000, true, 200000);
  report("HLD lca, cadena 2e5", t3, t4);
}

int main() {
  t_segtree();
  t_fenwick();
  t_sparse();
  t_dsu();
  t_lichao();
  t_lca();
  printf("OK estructuras (igual que KACTL)\n");
  print_timings();
}
