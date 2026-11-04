// Puentes, puntos de articulacion, componentes biconexas y
// bridge tree. Grafo no dirigido; soporta aristas multiples.
// Uso: add_edge(...), run(), y opcionalmente bridge_tree().
// O(n + m). Requiere C++17.
struct Biconnected {
  int n, m = 0, timer = 0;
  vector<vector<pair<int, int>>> adj;  // (vecino, id de arista)
  vector<pair<int, int>> edges;
  vector<int> tin, low, stk;
  vector<bool> is_bridge, is_articulation;
  vector<vector<int>> comps;  // ids de aristas de cada biconexa
  Biconnected(int n_) : n(n_), adj(n_), tin(n_, -1), low(n_, 0),
                      is_articulation(n_, false) {}
  void add_edge(int u, int v) {
    adj[u].push_back({v, m});
    adj[v].push_back({u, m});
    edges.push_back({u, v});
    m++;
  }
  void dfs(int u, int parent_edge) {
    tin[u] = low[u] = timer++;
    int children = 0;
    for (auto [v, id] : adj[u]) {
      if (id == parent_edge) continue;
      if (tin[v] != -1) {  // arista de retroceso
        low[u] = min(low[u], tin[v]);
        if (tin[v] < tin[u]) stk.push_back(id);
        continue;
      }
      stk.push_back(id);
      dfs(v, id);
      children++;
      low[u] = min(low[u], low[v]);
      if (low[v] > tin[u]) is_bridge[id] = true;
      if (low[v] >= tin[u]) {
        if (parent_edge != -1 || children > 1) is_articulation[u] = true;
        comps.emplace_back();
        while (true) {
          int e = stk.back();
          stk.pop_back();
          comps.back().push_back(e);
          if (e == id) break;
        }
      }
    }
  }
  void run() {
    is_bridge.assign(m, false);
    for (int u = 0; u < n; u++)
      if (tin[u] == -1) dfs(u, -1);
  }
  // Bridge tree: comp[u] = componente 2-arista-conexa de u.
  // tree = bosque cuyas aristas son los puentes. Retorna #comps.
  vector<int> comp;
  vector<vector<int>> tree;
  int bridge_tree() {
    comp.assign(n, -1);
    int c = 0;
    for (int s = 0; s < n; s++) {
      if (comp[s] != -1) continue;
      vector<int> pending = {s};
      comp[s] = c;
      while (!pending.empty()) {
        int u = pending.back();
        pending.pop_back();
        for (auto [v, id] : adj[u])
          if (!is_bridge[id] && comp[v] == -1) {
            comp[v] = c;
            pending.push_back(v);
          }
      }
      c++;
    }
    tree.assign(c, {});
    for (int id = 0; id < m; id++)
      if (is_bridge[id]) {
        auto [u, v] = edges[id];
        tree[comp[u]].push_back(comp[v]);
        tree[comp[v]].push_back(comp[u]);
      }
    return c;
  }
};
