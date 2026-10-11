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
int componentes(int n, const vector<pair<int, int>>& es, int sin_arista,
                int sin_vertice) {
  DSU d(n);
  for (int i = 0; i < (int)es.size(); i++) {
    if (i == sin_arista) continue;
    if (es[i].first == sin_vertice || es[i].second == sin_vertice) continue;
    d.unir(es[i].first, es[i].second);
  }
  return d.comps - (sin_vertice != -1 ? 1 : 0);
}

void t_biconexas() {
  for (int it = 0; it < 3000; it++) {
    int n = rnd(1, 7), m = rnd(0, 10);
    vector<pair<int, int>> es;
    Biconexas b(n);
    for (int i = 0; i < m && n > 1; i++) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1);
      if (u == v) continue;
      es.push_back({u, v});
      b.add_edge(u, v);
    }
    m = es.size();
    b.run();
    int base = componentes(n, es, -1, -1);
    // puentes
    vector<bool> puente(m);
    for (int i = 0; i < m; i++) {
      puente[i] = componentes(n, es, i, -1) > base;
      CHECK(b.es_puente[i] == puente[i]);
    }
    // puntos de articulacion
    for (int u = 0; u < n; u++)
      CHECK(b.es_articulacion[u] == (componentes(n, es, -1, u) > base));
    // biconexas: dos aristas van juntas <=> comparten un ciclo simple
    DSU clases(max(m, 1));
    vector<bool> visto(n, false);
    function<void(int, int, int, vector<int>&)> ciclos =
        [&](int inicio, int u, int ultima, vector<int>& usadas) {
      for (int id = 0; id < m; id++) {
        if (id == ultima) continue;
        int v = -1;
        if (es[id].first == u) v = es[id].second;
        else if (es[id].second == u) v = es[id].first;
        if (v == -1 || v < inicio) continue;
        if (v == inicio) {
          for (int e : usadas) clases.unir(e, id);
          continue;
        }
        if (visto[v]) continue;
        visto[v] = true;
        usadas.push_back(id);
        ciclos(inicio, v, id, usadas);
        usadas.pop_back();
        visto[v] = false;
      }
    };
    for (int s = 0; s < n; s++) {
      vector<int> usadas;
      visto[s] = true;
      ciclos(s, s, -1, usadas);
      visto[s] = false;
    }
    vector<int> comp_de(m, -1);
    for (int c = 0; c < (int)b.comps.size(); c++)
      for (int e : b.comps[c]) {
        CHECK(comp_de[e] == -1);
        comp_de[e] = c;
      }
    for (int i = 0; i < m; i++) CHECK(comp_de[i] != -1);
    for (int i = 0; i < m; i++)
      for (int j = 0; j < m; j++)
        CHECK((comp_de[i] == comp_de[j]) == (clases.find(i) == clases.find(j)));
    // bridge tree
    int c = b.bridge_tree();
    DSU sin_puentes(n);
    int cant_puentes = 0;
    for (int i = 0; i < m; i++) {
      if (puente[i]) cant_puentes++;
      else sin_puentes.unir(es[i].first, es[i].second);
    }
    CHECK(c == sin_puentes.comps);
    for (int u = 0; u < n; u++)
      for (int v = 0; v < n; v++)
        CHECK((b.comp[u] == b.comp[v]) == (sin_puentes.find(u) == sin_puentes.find(v)));
    int grados = 0;
    for (auto& fila : b.arbol) grados += fila.size();
    CHECK(grados == 2 * cant_puentes);
    CHECK(c - cant_puentes == base);  // el bridge tree es un bosque
  }
}

void t_scc() {
  for (int it = 0; it < 3000; it++) {
    int n = rnd(1, 9), m = rnd(0, 20);
    SCC s(n);
    vector<vector<bool>> alc(n, vector<bool>(n, false));
    vector<pair<int, int>> es;
    for (int i = 0; i < n; i++) alc[i][i] = true;
    for (int i = 0; i < m; i++) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1);
      s.add_edge(u, v);
      es.push_back({u, v});
      alc[u][v] = true;
    }
    for (int k = 0; k < n; k++)
      for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
          if (alc[i][k] && alc[k][j]) alc[i][j] = true;
    int nc = s.run();
    set<int> usados(s.comp.begin(), s.comp.end());
    CHECK((int)usados.size() == nc && *usados.begin() == 0 && *usados.rbegin() == nc - 1);
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        CHECK((s.comp[i] == s.comp[j]) == (alc[i][j] && alc[j][i]));
    set<pair<int, int>> esperado;
    for (auto [u, v] : es)
      if (s.comp[u] != s.comp[v]) {
        CHECK(s.comp[u] < s.comp[v]);  // orden topologico
        esperado.insert({s.comp[u], s.comp[v]});
      }
    auto dag = s.condensar();
    set<pair<int, int>> obtenido;
    int total = 0;
    for (int c = 0; c < nc; c++)
      for (int d : dag[c]) {
        obtenido.insert({c, d});
        total++;
      }
    CHECK(obtenido == esperado && total == (int)esperado.size());
  }
}

void t_twosat() {
  for (int it = 0; it < 5000; it++) {
    int n = rnd(1, 6), m = rnd(0, 14);
    TwoSat ts(n);
    vector<array<int, 4>> cl;
    for (int i = 0; i < m; i++) {
      int a = rnd(0, n - 1), b = rnd(0, n - 1), va = rnd(0, 1), vb = rnd(0, 1);
      int tipo = rnd(0, 9);
      if (tipo == 0) {
        ts.forzar(a, va);
        cl.push_back({a, va, a, va});
      } else if (tipo == 1) {
        ts.implica(a, va, b, vb);
        cl.push_back({a, !va, b, vb});
      } else if (tipo == 2) {
        ts.distintos(a, b);
        cl.push_back({a, 1, b, 1});
        cl.push_back({a, 0, b, 0});
      } else {
        ts.add_or(a, va, b, vb);
        cl.push_back({a, va, b, vb});
      }
    }
    auto cumple = [&](int mask) {
      for (auto& c : cl)
        if (((mask >> c[0]) & 1) != c[1] && ((mask >> c[2]) & 1) != c[3])
          return false;
      return true;
    };
    bool posible = false;
    for (int mask = 0; mask < (1 << n); mask++)
      if (cumple(mask)) posible = true;
    bool res = ts.resolver();
    CHECK(res == posible);
    if (res) {
      int mask = 0;
      for (int i = 0; i < n; i++) if (ts.valor[i]) mask |= 1 << i;
      CHECK(cumple(mask));
    }
  }
}

void t_flujo() {
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
    map<long long, long long> costo_min;  // valor del flujo -> costo
    function<void(int)> rec = [&](int i) {
      if (i == k) {
        vector<int> bal(n, 0);
        long long costo = 0;
        for (int j = 0; j < k; j++) {
          bal[es[j][0]] -= f[j];
          bal[es[j][1]] += f[j];
          costo += f[j] * es[j][3];
        }
        for (int v = 0; v < n; v++)
          if (v != s && v != t && bal[v] != 0) return;
        if (bal[t] < 0) return;
        if (!costo_min.count(bal[t]) || costo < costo_min[bal[t]])
          costo_min[bal[t]] = costo;
        return;
      }
      for (int x = 0; x <= es[i][2]; x++) {
        f[i] = x;
        rec(i + 1);
      }
      f[i] = 0;
    };
    rec(0);
    long long maxf = costo_min.rbegin()->first;
    Dinic d(n);
    for (auto& e : es) d.add_edge(e[0], e[1], e[2]);
    CHECK(d.max_flow(s, t) == maxf);
    long long corte = 0;  // corte minimo a partir de nivel[]
    for (auto& e : es)
      if (d.nivel[e[0]] != -1 && d.nivel[e[1]] == -1) corte += e[2];
    CHECK(corte == maxf);
    for (int j = 0; j < k; j++)  // flujo por arista dentro de la capacidad
      CHECK(d.e[2 * j + 1].cap >= 0 && d.e[2 * j + 1].cap <= es[j][2]);
    MinCostFlow mc(n);
    for (auto& e : es) mc.add_edge(e[0], e[1], e[2], e[3]);
    auto [fl, c] = mc.flujo(s, t);
    CHECK(fl == maxf && c == costo_min[maxf]);
    long long limite = rnd(0, maxf + 1);
    MinCostFlow mc2(n);
    for (auto& e : es) mc2.add_edge(e[0], e[1], e[2], e[3]);
    auto [fl2, c2] = mc2.flujo(s, t, limite);
    CHECK(fl2 == min(limite, maxf) && c2 == costo_min[fl2]);
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
    CHECK(d.max_flow(0, n - 1) == mc.flujo(0, n - 1).first);
  }
}

void t_hungaro() {
  for (int it = 0; it < 3000; it++) {
    int n = rnd(1, 5), m = rnd(n, 6);
    vector<vector<long long>> a(n, vector<long long>(m));
    for (auto& fila : a) for (auto& x : fila) x = rnd(-20, 20);
    vector<int> cols(m);
    iota(cols.begin(), cols.end(), 0);
    long long mejor = LLONG_MAX;
    do {
      long long s = 0;
      for (int i = 0; i < n; i++) s += a[i][cols[i]];
      mejor = min(mejor, s);
    } while (next_permutation(cols.begin(), cols.end()));
    auto [costo, asig] = hungaro(a);
    CHECK(costo == mejor);
    long long s = 0;
    set<int> usadas;
    for (int i = 0; i < n; i++) {
      CHECK(asig[i] >= 0 && asig[i] < m);
      usadas.insert(asig[i]);
      s += a[i][asig[i]];
    }
    CHECK(s == mejor && (int)usadas.size() == n);
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
    auto [fl, c] = mc.flujo(s, t);
    CHECK(fl == n && c == hungaro(a).first);
    // maximo peso = negar costos
    vector<vector<long long>> neg = a;
    for (auto& fila : neg) for (auto& x : fila) x = -x;
    MinCostFlow mx(n + m + 2);
    for (int i = 0; i < n; i++) {
      mx.add_edge(s, i, 1, 0);
      for (int j = 0; j < m; j++) mx.add_edge(i, n + j, 1, 100 - a[i][j]);
    }
    for (int j = 0; j < m; j++) mx.add_edge(n + j, t, 1, 0);
    CHECK(-hungaro(neg).first == 100LL * n - mx.flujo(s, t).second);
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
    int pares = 0;
    for (int u = 0; u < n; u++)
      if (hk.par_izq[u] != -1) {
        CHECK(es.count({u, hk.par_izq[u]}) && hk.par_der[hk.par_izq[u]] == u);
        pares++;
      }
    CHECK(pares == res);
  }
}

int main() {
  t_dijkstra();
  t_biconexas();
  t_scc();
  t_twosat();
  t_flujo();
  t_hungaro();
  t_matching();
  printf("OK grafos\n");
}
