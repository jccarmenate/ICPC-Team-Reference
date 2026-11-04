#include "comun.h"
#include "../codigo/estructuras/dsu.cpp"
#include "../codigo/grafos/dijkstra.cpp"
#include "../codigo/grafos/biconexas.cpp"
#include "../codigo/grafos/scc.cpp"
#include "../codigo/grafos/twosat.cpp"
#include "../codigo/grafos/dinic.cpp"
#include "../codigo/grafos/mincost.cpp"
#include "../codigo/grafos/hungaro.cpp"
#include "../codigo/grafos/hopcroft_karp.cpp"

void t_dijkstra() {
  for (int it = 0; it < 500; it++) {
    int n = rnd(1, 12), m = rnd(0, 30);
    const long long INF = LLONG_MAX;
    vector<vector<pair<int, long long>>> adj(n);
    vector<vector<long long>> d(n, vector<long long>(n, INF));
    for (int i = 0; i < n; i++) d[i][i] = 0;
    for (int i = 0; i < m; i++) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1);
      long long w = rnd(0, 20);
      adj[u].push_back({v, w});
      d[u][v] = min(d[u][v], w);
    }
    for (int k = 0; k < n; k++)
      for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
          if (d[i][k] != INF && d[k][j] != INF)
            d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    for (int s = 0; s < n; s++) CHECK(dijkstra(adj, s) == d[s]);
  }
}

// Cantidad de componentes conexas ignorando una arista y/o un vertice.
int components(int n, const vector<pair<int, int>>& es, int skip_edge,
                int skip_vertex) {
  DSU d(n);
  for (int i = 0; i < (int)es.size(); i++) {
    if (i == skip_edge) continue;
    if (es[i].first == skip_vertex || es[i].second == skip_vertex) continue;
    d.unite(es[i].first, es[i].second);
  }
  return d.comps - (skip_vertex != -1 ? 1 : 0);
}

void t_biconnected() {
  for (int it = 0; it < 3000; it++) {
    int n = rnd(1, 7), m = rnd(0, 10);
    vector<pair<int, int>> es;
    Biconnected b(n);
    for (int i = 0; i < m && n > 1; i++) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1);
      if (u == v) continue;
      es.push_back({u, v});
      b.add_edge(u, v);
    }
    m = es.size();
    b.run();
    int base = components(n, es, -1, -1);
    // puentes
    vector<bool> bridge(m);
    for (int i = 0; i < m; i++) {
      bridge[i] = components(n, es, i, -1) > base;
      CHECK(b.is_bridge[i] == bridge[i]);
    }
    // puntos de articulacion
    for (int u = 0; u < n; u++)
      CHECK(b.is_articulation[u] == (components(n, es, -1, u) > base));
    // biconexas: dos aristas van juntas <=> comparten un ciclo simple
    DSU classes(max(m, 1));
    vector<bool> seen(n, false);
    function<void(int, int, int, vector<int>&)> cycles =
        [&](int start, int u, int last_edge, vector<int>& used_edges) {
      for (int id = 0; id < m; id++) {
        if (id == last_edge) continue;
        int v = -1;
        if (es[id].first == u) v = es[id].second;
        else if (es[id].second == u) v = es[id].first;
        if (v == -1 || v < start) continue;
        if (v == start) {
          for (int e : used_edges) classes.unite(e, id);
          continue;
        }
        if (seen[v]) continue;
        seen[v] = true;
        used_edges.push_back(id);
        cycles(start, v, id, used_edges);
        used_edges.pop_back();
        seen[v] = false;
      }
    };
    for (int s = 0; s < n; s++) {
      vector<int> used_edges;
      seen[s] = true;
      cycles(s, s, -1, used_edges);
      seen[s] = false;
    }
    vector<int> comp_of(m, -1);
    for (int c = 0; c < (int)b.comps.size(); c++)
      for (int e : b.comps[c]) {
        CHECK(comp_of[e] == -1);
        comp_of[e] = c;
      }
    for (int i = 0; i < m; i++) CHECK(comp_of[i] != -1);
    for (int i = 0; i < m; i++)
      for (int j = 0; j < m; j++)
        CHECK((comp_of[i] == comp_of[j]) == (classes.find(i) == classes.find(j)));
    // bridge tree
    int c = b.bridge_tree();
    DSU no_bridges(n);
    int bridge_count = 0;
    for (int i = 0; i < m; i++) {
      if (bridge[i]) bridge_count++;
      else no_bridges.unite(es[i].first, es[i].second);
    }
    CHECK(c == no_bridges.comps);
    for (int u = 0; u < n; u++)
      for (int v = 0; v < n; v++)
        CHECK((b.comp[u] == b.comp[v]) == (no_bridges.find(u) == no_bridges.find(v)));
    int degrees = 0;
    for (auto& row : b.tree) degrees += row.size();
    CHECK(degrees == 2 * bridge_count);
    CHECK(c - bridge_count == base);  // el bridge tree es un bosque
  }
}

void t_scc() {
  for (int it = 0; it < 3000; it++) {
    int n = rnd(1, 9), m = rnd(0, 20);
    SCC s(n);
    vector<vector<bool>> reach(n, vector<bool>(n, false));
    vector<pair<int, int>> es;
    for (int i = 0; i < n; i++) reach[i][i] = true;
    for (int i = 0; i < m; i++) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1);
      s.add_edge(u, v);
      es.push_back({u, v});
      reach[u][v] = true;
    }
    for (int k = 0; k < n; k++)
      for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
          if (reach[i][k] && reach[k][j]) reach[i][j] = true;
    int nc = s.run();
    set<int> used_set(s.comp.begin(), s.comp.end());
    CHECK((int)used_set.size() == nc && *used_set.begin() == 0 && *used_set.rbegin() == nc - 1);
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        CHECK((s.comp[i] == s.comp[j]) == (reach[i][j] && reach[j][i]));
    set<pair<int, int>> expected;
    for (auto [u, v] : es)
      if (s.comp[u] != s.comp[v]) {
        CHECK(s.comp[u] < s.comp[v]);  // orden topologico
        expected.insert({s.comp[u], s.comp[v]});
      }
    auto dag = s.condense();
    set<pair<int, int>> got;
    int total = 0;
    for (int c = 0; c < nc; c++)
      for (int d : dag[c]) {
        got.insert({c, d});
        total++;
      }
    CHECK(got == expected && total == (int)expected.size());
  }
}

void t_twosat() {
  for (int it = 0; it < 5000; it++) {
    int n = rnd(1, 6), m = rnd(0, 14);
    TwoSat ts(n);
    vector<array<int, 4>> cl;
    for (int i = 0; i < m; i++) {
      int a = rnd(0, n - 1), b = rnd(0, n - 1), va = rnd(0, 1), vb = rnd(0, 1);
      int kind = rnd(0, 9);
      if (kind == 0) {
        ts.force(a, va);
        cl.push_back({a, va, a, va});
      } else if (kind == 1) {
        ts.implies(a, va, b, vb);
        cl.push_back({a, !va, b, vb});
      } else if (kind == 2) {
        ts.distinct(a, b);
        cl.push_back({a, 1, b, 1});
        cl.push_back({a, 0, b, 0});
      } else {
        ts.add_or(a, va, b, vb);
        cl.push_back({a, va, b, vb});
      }
    }
    auto satisfies = [&](int mask) {
      for (auto& c : cl)
        if (((mask >> c[0]) & 1) != c[1] && ((mask >> c[2]) & 1) != c[3])
          return false;
      return true;
    };
    bool possible = false;
    for (int mask = 0; mask < (1 << n); mask++)
      if (satisfies(mask)) possible = true;
    bool res = ts.solve();
    CHECK(res == possible);
    if (res) {
      int mask = 0;
      for (int i = 0; i < n; i++) if (ts.value[i]) mask |= 1 << i;
      CHECK(satisfies(mask));
    }
  }
}

void t_flow() {
  for (int it = 0; it < 1500; it++) {
    int n = rnd(2, 5), m = rnd(0, 6), s = 0, t = n - 1;
    vector<array<int, 4>> es;  // u, v, cap, costo
    for (int i = 0; i < m; i++) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1);
      if (u == v) continue;
      es.push_back({u, v, (int)rnd(0, 2), (int)rnd(0, 5)});
    }
    int k = es.size();
    // fuerza bruta: todas las asignaciones de flujo por arista
    vector<int> f(k, 0);
    map<long long, long long> min_cost;  // valor del flujo -> costo
    function<void(int)> rec = [&](int i) {
      if (i == k) {
        vector<int> bal(n, 0);
        long long cost = 0;
        for (int j = 0; j < k; j++) {
          bal[es[j][0]] -= f[j];
          bal[es[j][1]] += f[j];
          cost += f[j] * es[j][3];
        }
        for (int v = 0; v < n; v++)
          if (v != s && v != t && bal[v] != 0) return;
        if (bal[t] < 0) return;
        if (!min_cost.count(bal[t]) || cost < min_cost[bal[t]])
          min_cost[bal[t]] = cost;
        return;
      }
      for (int x = 0; x <= es[i][2]; x++) {
        f[i] = x;
        rec(i + 1);
      }
      f[i] = 0;
    };
    rec(0);
    long long maxf = min_cost.rbegin()->first;
    Dinic d(n);
    for (auto& e : es) d.add_edge(e[0], e[1], e[2]);
    CHECK(d.max_flow(s, t) == maxf);
    long long cut = 0;  // corte minimo a partir de level[]
    for (auto& e : es)
      if (d.level[e[0]] != -1 && d.level[e[1]] == -1) cut += e[2];
    CHECK(cut == maxf);
    for (int j = 0; j < k; j++)  // flujo por arista dentro de la capacidad
      CHECK(d.e[2 * j + 1].cap >= 0 && d.e[2 * j + 1].cap <= es[j][2]);
    MinCostFlow mc(n);
    for (auto& e : es) mc.add_edge(e[0], e[1], e[2], e[3]);
    auto [fl, c] = mc.flow(s, t);
    CHECK(fl == maxf && c == min_cost[maxf]);
    long long limit = rnd(0, maxf + 1);
    MinCostFlow mc2(n);
    for (auto& e : es) mc2.add_edge(e[0], e[1], e[2], e[3]);
    auto [fl2, c2] = mc2.flow(s, t, limit);
    CHECK(fl2 == min(limit, maxf) && c2 == min_cost[fl2]);
    // aristas no dirigidas: rcap equivale a dos aristas dirigidas
    Dinic a(n), b(n);
    for (auto& e : es) {
      a.add_edge(e[0], e[1], e[2], e[2]);
      b.add_edge(e[0], e[1], e[2]);
      b.add_edge(e[1], e[0], e[2]);
    }
    CHECK(a.max_flow(s, t) == b.max_flow(s, t));
  }
  // Dinic contra MinCostFlow en grafos mas grandes
  for (int it = 0; it < 200; it++) {
    int n = rnd(2, 30), m = rnd(0, 120);
    Dinic d(n);
    MinCostFlow mc(n);
    for (int i = 0; i < m; i++) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1), cap = rnd(0, 50);
      if (u == v) continue;
      d.add_edge(u, v, cap);
      mc.add_edge(u, v, cap, rnd(0, 9));
    }
    CHECK(d.max_flow(0, n - 1) == mc.flow(0, n - 1).first);
  }
}

void t_hungarian() {
  for (int it = 0; it < 3000; it++) {
    int n = rnd(1, 5), m = rnd(n, 6);
    vector<vector<long long>> a(n, vector<long long>(m));
    for (auto& row : a) for (auto& x : row) x = rnd(-20, 20);
    vector<int> cols(m);
    iota(cols.begin(), cols.end(), 0);
    long long best = LLONG_MAX;
    do {
      long long s = 0;
      for (int i = 0; i < n; i++) s += a[i][cols[i]];
      best = min(best, s);
    } while (next_permutation(cols.begin(), cols.end()));
    auto [cost, assignment] = hungarian(a);
    CHECK(cost == best);
    long long s = 0;
    set<int> used_edges;
    for (int i = 0; i < n; i++) {
      CHECK(assignment[i] >= 0 && assignment[i] < m);
      used_edges.insert(assignment[i]);
      s += a[i][assignment[i]];
    }
    CHECK(s == best && (int)used_edges.size() == n);
  }
  // contra flujo de costo minimo (asignacion completa)
  for (int it = 0; it < 150; it++) {
    int n = rnd(1, 25), m = rnd(n, 30);
    vector<vector<long long>> a(n, vector<long long>(m));
    MinCostFlow mc(n + m + 2);
    int s = n + m, t = n + m + 1;
    for (int i = 0; i < n; i++) {
      mc.add_edge(s, i, 1, 0);
      for (int j = 0; j < m; j++) {
        a[i][j] = rnd(0, 100);
        mc.add_edge(i, n + j, 1, a[i][j]);
      }
    }
    for (int j = 0; j < m; j++) mc.add_edge(n + j, t, 1, 0);
    auto [fl, c] = mc.flow(s, t);
    CHECK(fl == n && c == hungarian(a).first);
    // maximo peso = negar costos
    vector<vector<long long>> neg = a;
    for (auto& row : neg) for (auto& x : row) x = -x;
    MinCostFlow mx(n + m + 2);
    for (int i = 0; i < n; i++) {
      mx.add_edge(s, i, 1, 0);
      for (int j = 0; j < m; j++) mx.add_edge(i, n + j, 1, 100 - a[i][j]);
    }
    for (int j = 0; j < m; j++) mx.add_edge(n + j, t, 1, 0);
    CHECK(-hungarian(neg).first == 100LL * n - mx.flow(s, t).second);
  }
}

void t_matching() {
  for (int it = 0; it < 3000; it++) {
    int n = rnd(1, 12), m = rnd(1, 12), k = rnd(0, 40);
    HopcroftKarp hk(n, m);
    Dinic d(n + m + 2);
    int s = n + m, t = n + m + 1;
    set<pair<int, int>> es;
    for (int i = 0; i < k; i++) es.insert({rnd(0, n - 1), rnd(0, m - 1)});
    for (auto [u, v] : es) {
      hk.add_edge(u, v);
      d.add_edge(u, n + v, 1);
    }
    for (int i = 0; i < n; i++) d.add_edge(s, i, 1);
    for (int j = 0; j < m; j++) d.add_edge(n + j, t, 1);
    int res = hk.matching();
    CHECK(res == d.max_flow(s, t));
    int pairs = 0;
    for (int u = 0; u < n; u++)
      if (hk.match_left[u] != -1) {
        CHECK(es.count({u, hk.match_left[u]}) && hk.match_right[hk.match_left[u]] == u);
        pairs++;
      }
    CHECK(pairs == res);
  }
}

int main() {
  t_dijkstra();
  t_biconnected();
  t_scc();
  t_twosat();
  t_flow();
  t_hungarian();
  t_matching();
  printf("OK graphs\n");
}
