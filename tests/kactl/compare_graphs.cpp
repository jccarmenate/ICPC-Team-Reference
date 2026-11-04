// Compara los algoritmos de grafos del notebook contra KACTL.
#include "kactl_begin.h"
namespace kbcc {
#include "graph/BiconnectedComponents.h"
}
namespace kscc {
#include "graph/SCC.h"
}
namespace k2sat {
#include "graph/2sat.h"
}
namespace kdinic {
#include "graph/Dinic.h"
}
namespace kmcmf {
#include "graph/MinCostMaxFlow.h"
}
namespace kwm {
#include "graph/WeightedMatching.h"
}
namespace khk {
#include "graph/hopcroftKarp.h"
}
namespace keuler {
#include "graph/EulerWalk.h"
}
#include "kactl_end.h"
#include "../../codigo/grafos/biconexas.cpp"
#include "../../codigo/grafos/scc.cpp"
#include "../../codigo/grafos/twosat.cpp"
#include "../../codigo/grafos/dinic.cpp"
#include "../../codigo/grafos/mincost.cpp"
#include "../../codigo/grafos/hungaro.cpp"
#include "../../codigo/grafos/hopcroft_karp.cpp"
#include "../../codigo/grafos/euler.cpp"

// Componentes biconexas, puentes y articulaciones contra bicomps de
// KACTL (que no reporta los puentes como componentes).
pair<double, double> compare_bcc(int len, int m) {
  vector<pair<int, int>> es;
  for (int i = 0; i < m && len > 1; i++) {
    int u = rnd(0, len - 1), v = rnd(0, len - 1);
    if (u != v) es.push_back({u, v});
  }
  m = es.size();
  Biconnected b(len);
  for (auto [u, v] : es) b.add_edge(u, v);
  double t1 = measure([&] { b.run(); });
  vector<vector<int>> kcomps;
  kbcc::ed.assign(len, {});
  for (int i = 0; i < m; i++) {
    kbcc::ed[es[i].first].emplace_back(es[i].second, i);
    kbcc::ed[es[i].second].emplace_back(es[i].first, i);
  }
  double t2 = measure([&] {
    kbcc::bicomps([&](const vi& c) { kcomps.push_back(c); });
  });
  vector<int> in_comp(m, 0);
  multiset<vector<int>> expected, got;
  for (auto c : kcomps) {
    sort(c.begin(), c.end());
    for (int e : c) in_comp[e]++;
    expected.insert(c);
  }
  for (auto c : b.comps) {
    if (c.size() == 1 && b.is_bridge[c[0]]) continue;
    sort(c.begin(), c.end());
    got.insert(c);
  }
  CHECK(got == expected);
  for (int e = 0; e < m; e++) CHECK(in_comp[e] <= 1 && b.is_bridge[e] == (in_comp[e] == 0));
  // articulacion <=> el vertice esta en 2 o mas bloques (bloque =
  // componente de KACTL o puente)
  vector<set<int>> blocks(len);
  int id = 0;
  for (auto& c : kcomps) {
    for (int e : c) { blocks[es[e].first].insert(id); blocks[es[e].second].insert(id); }
    id++;
  }
  for (int e = 0; e < m; e++)
    if (in_comp[e] == 0) { blocks[es[e].first].insert(id); blocks[es[e].second].insert(id); id++; }
  for (int u = 0; u < len; u++) CHECK(b.is_articulation[u] == (blocks[u].size() >= 2));
  return {t1, t2};
}

pair<double, double> compare_scc(int len, int m) {
  SCC s(len);
  vector<vi> g(len);
  for (int i = 0; i < m; i++) {
    int u = rnd(0, len - 1), v = rnd(0, len - 1);
    s.add_edge(u, v);
    g[u].push_back(v);
  }
  double t1 = measure([&] { s.run(); });
  double t2 = measure([&] { kscc::scc(g, [&](vi&) {}); });
  CHECK(s.ncomp == kscc::ncomps);
  map<int, int> to_k, to_o;  // misma particion: biyeccion entre ids
  for (int u = 0; u < len; u++) {
    auto a = to_k.emplace(s.comp[u], kscc::comp[u]).first;
    auto b = to_o.emplace(kscc::comp[u], s.comp[u]).first;
    CHECK(a->second == kscc::comp[u] && b->second == s.comp[u]);
  }
  return {t1, t2};
}

pair<double, double> compare_twosat(int vars, int clauses) {
  TwoSat ts(vars);
  k2sat::TwoSat kt(vars);
  vector<array<int, 4>> cl(clauses);
  for (auto& c : cl) {
    c = {(int)rnd(0, vars - 1), (int)rnd(0, 1), (int)rnd(0, vars - 1), (int)rnd(0, 1)};
    ts.add_or(c[0], c[1], c[2], c[3]);
    kt.either(c[1] ? c[0] : ~c[0], c[3] ? c[2] : ~c[2]);
  }
  bool r1 = false, r2 = false;
  double t1 = measure([&] { r1 = ts.solve(); });
  double t2 = measure([&] { r2 = kt.solve(); });
  CHECK(r1 == r2);
  if (r1)
    for (auto& c : cl) CHECK(ts.value[c[0]] == (bool)c[1] || ts.value[c[2]] == (bool)c[3]);
  return {t1, t2};
}

struct FlowEdge { int u, v; long long cap, cost; };

pair<double, double> compare_dinic(int len, const vector<FlowEdge>& es) {
  Dinic d(len);
  kdinic::Dinic k(len);
  for (auto& e : es) { d.add_edge(e.u, e.v, e.cap); k.addEdge(e.u, e.v, e.cap); }
  long long f1 = 0, f2 = 0;
  double t1 = measure([&] { f1 = d.max_flow(0, len - 1); });
  double t2 = measure([&] { f2 = k.calc(0, len - 1); });
  CHECK(f1 == f2);
  // el lado de s del corte minimo (alcanzables en el residual) es unico
  for (int u = 0; u < len; u++) CHECK((d.level[u] != -1) == k.leftOfMinCut(u));
  return {t1, t2};
}

pair<double, double> compare_mcmf(int len, const vector<FlowEdge>& es) {
  MinCostFlow mc(len);
  kmcmf::MCMF k(len);
  for (auto& e : es) { mc.add_edge(e.u, e.v, e.cap, e.cost); k.addEdge(e.u, e.v, e.cap, e.cost); }
  pair<long long, long long> r1, r2;
  double t1 = measure([&] { r1 = mc.flow(0, len - 1); });
  double t2 = measure([&] { r2 = k.maxflow(0, len - 1); });
  CHECK(r1 == r2);
  return {t1, t2};
}

vector<FlowEdge> random_flow_graph(int len, int m, long long maxcap, long long maxcost) {
  vector<FlowEdge> es;
  for (int i = 0; i < m; i++) {
    int u = rnd(0, len - 1), v = rnd(0, len - 1);
    if (u != v) es.push_back({u, v, rnd(0, maxcap), rnd(0, maxcost)});
  }
  return es;
}

void t_flows() {
  for (int it = 0; it < 3000; it++) {
    int len = rnd(2, 10);
    auto es = random_flow_graph(len, rnd(0, 30), 10, 10);
    compare_dinic(len, es);
    compare_mcmf(len, es);
  }
  {
    auto es = random_flow_graph(2000, 20000, 1000000000, 0);
    auto [t1, t2] = compare_dinic(2000, es);
    report("Dinic, n=2000 m=2e4 cap 1e9", t1, t2);
  }
  {  // matching bipartito con capacidades unitarias
    int L = 20000, len = 2 * L + 2;
    vector<FlowEdge> es;
    for (int i = 0; i < L; i++) es.push_back({0, 1 + i, 1, 0}), es.push_back({1 + L + i, len - 1, 1, 0});
    for (int i = 0; i < 200000; i++) es.push_back({1 + (int)rnd(0, L - 1), 1 + L + (int)rnd(0, L - 1), 1, 0});
    auto [t1, t2] = compare_dinic(len, es);
    report("Dinic, bipartito unitario 2e4+2e4, 2e5", t1, t2);
  }
  {
    auto es = random_flow_graph(300, 3000, 100, 1000);
    auto [t1, t2] = compare_mcmf(300, es);
    report("MinCostFlow, n=300 m=3000", t1, t2);
  }
  {  // asignacion como flujo: 100 + 100 nodos, 1e4 aristas
    int L = 100, len = 2 * L + 2;
    vector<FlowEdge> es;
    for (int i = 0; i < L; i++) es.push_back({0, 1 + i, 1, 0}), es.push_back({1 + L + i, len - 1, 1, 0});
    for (int i = 0; i < L; i++)
      for (int j = 0; j < L; j++) es.push_back({1 + i, 1 + L + j, 1, rnd(0, 1000000)});
    auto [t1, t2] = compare_mcmf(len, es);
    report("MinCostFlow, asignacion 100x100", t1, t2);
  }
}

void t_assignment() {
  for (int it = 0; it < 3000; it++) {
    int r = rnd(1, 8), c = rnd(r, 9);
    vector<vector<long long>> a(r, vector<long long>(c));
    for (auto& row : a) for (auto& x : row) x = rnd(-1000, 1000);
    auto copy = a;
    CHECK(hungarian(a).first == kwm::weightedMatching(copy).first);
  }
  int N = 500;
  vector<vector<long long>> a(N, vector<long long>(N));
  for (auto& row : a) for (auto& x : row) x = rnd(0, 1000000000);
  auto copy = a;
  long long c1 = 0, c2 = 0;
  double t1 = measure([&] { c1 = hungarian(a).first; });
  double t2 = measure([&] { c2 = kwm::weightedMatching(copy).first; });
  CHECK(c1 == c2);
  report("Hungaro 500x500", t1, t2);
}

pair<double, double> compare_matching(int L, int R, int m) {
  HopcroftKarp hk(L, R);
  vector<vi> g(L);
  for (int i = 0; i < m; i++) {
    int u = rnd(0, L - 1), v = rnd(0, R - 1);
    hk.add_edge(u, v);
    g[u].push_back(v);
  }
  vi right(R, -1);
  int r1 = 0, r2 = 0;
  double t1 = measure([&] { r1 = hk.matching(); });
  double t2 = measure([&] { r2 = khk::hopcroftKarp(g, right); });
  CHECK(r1 == r2);
  return {t1, t2};
}

void t_matching() {
  for (int it = 0; it < 3000; it++) compare_matching(rnd(1, 12), rnd(1, 12), rnd(0, 40));
  auto [t1, t2] = compare_matching(100000, 100000, 400000);
  report("Hopcroft-Karp 1e5+1e5, 4e5 aristas", t1, t2);
}

void t_euler() {
  for (int it = 0; it < 20000; it++) {
    int len = rnd(1, 5), m = rnd(0, 7);
    bool dir = rnd(0, 1);
    Euler e(len, dir);
    vector<vector<pii>> gr(len);
    for (int i = 0; i < m; i++) {
      int u = rnd(0, len - 1), v = rnd(0, len - 1);
      e.add_edge(u, v);
      gr[u].push_back({v, i});
      if (!dir) gr[v].push_back({u, i});
    }
    vector<int> c = e.path();
    if (!c.empty()) CHECK(!keuler::eulerWalk(gr, m, c.front()).empty());
    else
      for (int src = 0; src < len; src++) CHECK(keuler::eulerWalk(gr, m, src).empty());
  }
  // grande: circuito dirigido aleatorio de 1e6 aristas
  int len = 100000, m = 1000000;
  Euler e(len, true);
  vector<vector<pii>> gr(len);
  int cur = 0;
  for (int i = 0; i < m; i++) {
    int nxt = i == m - 1 ? 0 : (int)rnd(0, len - 1);
    e.add_edge(cur, nxt);
    gr[cur].push_back({nxt, i});
    cur = nxt;
  }
  vector<int> c, k;
  double t1 = measure([&] { c = e.path(); });
  double t2 = measure([&] { k = keuler::eulerWalk(gr, m, 0); });
  CHECK((int)c.size() == m + 1 && (int)k.size() == m + 1);
  report("Euler, circuito dirigido 1e6 aristas", t1, t2);
}

int main() {
  for (int it = 0; it < 3000; it++) compare_bcc(rnd(1, 8), rnd(0, 12));
  auto [b1, b2] = compare_bcc(100000, 200000);
  report("Biconexas, n=1e5 m=2e5", b1, b2);
  for (int it = 0; it < 3000; it++) compare_scc(rnd(1, 10), rnd(0, 25));
  auto [s1, s2] = compare_scc(200000, 400000);
  report("SCC, n=2e5 m=4e5", s1, s2);
  for (int it = 0; it < 5000; it++) compare_twosat(rnd(1, 8), rnd(0, 20));
  auto [w1, w2] = compare_twosat(100000, 100000);
  report("2-SAT, 1e5 variables 1e5 clausulas", w1, w2);
  t_flows();
  t_assignment();
  t_matching();
  t_euler();
  printf("OK grafos (igual que KACTL)\n");
  print_timings();
}
