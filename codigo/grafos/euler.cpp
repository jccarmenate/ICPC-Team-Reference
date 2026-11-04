// Camino / circuito euleriano (Hierholzer iterativo). O(n + m).
// Recorre cada ARISTA exactamente una vez. Admite aristas
// multiples y lazos, dirigido o no dirigido.
// path(): secuencia de m + 1 vertices, o vacio si no existe.
//   Si el primero y el ultimo coinciden es un circuito (se puede
//   rotar para empezar en cualquier vertice).
// Condiciones de existencia (todas las aristas en una componente):
//   No dirigido: 0 vertices de grado impar (circuito) o 2 (camino
//     que va de uno al otro).
//   Dirigido: todos con entrada == salida (circuito), o uno con
//     salida - entrada = 1 (inicio) y otro con entrada - salida = 1
//     (final) y el resto iguales.
struct Euler {
  int n, m = 0;
  bool directed;
  vector<vector<pair<int, int>>> adj;  // (vecino, id de arista)
  vector<int> out_deg, in_deg;
  Euler(int n_, bool directed_) : n(n_), directed(directed_),
      adj(n_), out_deg(n_, 0), in_deg(n_, 0) {}
  void add_edge(int u, int v) {
    adj[u].push_back({v, m});
    if (!directed) adj[v].push_back({u, m});
    out_deg[u]++;
    in_deg[v]++;
    m++;
  }
  vector<int> path() {
    int start = -1, odd_cnt = 0, ends = 0;
    for (int u = 0; u < n; u++) {
      if (directed) {
        int d = out_deg[u] - in_deg[u];
        if (d == 1) { start = u; ends++; }
        else if (d == -1) ends++;
        else if (d != 0) return {};
      } else if ((out_deg[u] + in_deg[u]) % 2 == 1) {
        odd_cnt++;
        if (start == -1) start = u;
      }
    }
    if (directed && ends != 0 && !(ends == 2 && start != -1))
      return {};
    if (!directed && odd_cnt != 0 && odd_cnt != 2) return {};
    if (start == -1)  // circuito: cualquier vertice con aristas
      for (int u = 0; u < n && start == -1; u++)
        if (!adj[u].empty()) start = u;
    if (start == -1) return {0};  // grafo sin aristas
    vector<int> it(n, 0), res, stk = {start};
    vector<bool> used(m, false);
    while (!stk.empty()) {
      int u = stk.back();
      while (it[u] < (int)adj[u].size() && used[adj[u][it[u]].second])
        it[u]++;
      if (it[u] == (int)adj[u].size()) {
        res.push_back(u);  // u ya no tiene aristas libres
        stk.pop_back();
      } else {
        auto [v, id] = adj[u][it[u]];
        used[id] = true;
        stk.push_back(v);
      }
    }
    if ((int)res.size() != m + 1) return {};  // aristas desconectadas
    reverse(res.begin(), res.end());
    return res;
  }
};
