// Puentes, puntos de articulacion, componentes biconexas y
// bridge tree. Grafo no dirigido; soporta aristas multiples.
// Uso: add_edge(...), run(), y opcionalmente bridge_tree().
// O(n + m). Requiere C++17.
struct Biconexas {
  int n, m = 0, reloj = 0;
  vector<vector<pair<int, int>>> adj;  // (vecino, id de arista)
  vector<pair<int, int>> aristas;
  vector<int> tin, low, pila;
  vector<bool> es_puente, es_articulacion;
  vector<vector<int>> comps;  // ids de aristas de cada biconexa
  Biconexas(int n_) : n(n_), adj(n_), tin(n_, -1), low(n_, 0),
                      es_articulacion(n_, false) {}
  void add_edge(int u, int v) {
    adj[u].push_back({v, m});
    adj[v].push_back({u, m});
    aristas.push_back({u, v});
    m++;
  }
  void dfs(int u, int id_padre) {
    tin[u] = low[u] = reloj++;
    int hijos = 0;
    for (auto [v, id] : adj[u]) {
      if (id == id_padre) continue;
      if (tin[v] != -1) {  // arista de retroceso
        low[u] = min(low[u], tin[v]);
        if (tin[v] < tin[u]) pila.push_back(id);
        continue;
      }
      pila.push_back(id);
      dfs(v, id);
      hijos++;
      low[u] = min(low[u], low[v]);
      if (low[v] > tin[u]) es_puente[id] = true;
      if (low[v] >= tin[u]) {
        if (id_padre != -1 || hijos > 1) es_articulacion[u] = true;
        comps.emplace_back();
        while (true) {
          int e = pila.back();
          pila.pop_back();
          comps.back().push_back(e);
          if (e == id) break;
        }
      }
    }
  }
  void run() {
    es_puente.assign(m, false);
    for (int u = 0; u < n; u++)
      if (tin[u] == -1) dfs(u, -1);
  }
  // Bridge tree: comp[u] = componente 2-arista-conexa de u.
  // arbol = bosque cuyas aristas son los puentes. Retorna #comps.
  vector<int> comp;
  vector<vector<int>> arbol;
  int bridge_tree() {
    comp.assign(n, -1);
    int c = 0;
    for (int s = 0; s < n; s++) {
      if (comp[s] != -1) continue;
      vector<int> pend = {s};
      comp[s] = c;
      while (!pend.empty()) {
        int u = pend.back();
        pend.pop_back();
        for (auto [v, id] : adj[u])
          if (!es_puente[id] && comp[v] == -1) {
            comp[v] = c;
            pend.push_back(v);
          }
      }
      c++;
    }
    arbol.assign(c, {});
    for (int id = 0; id < m; id++)
      if (es_puente[id]) {
        auto [u, v] = aristas[id];
        arbol[comp[u]].push_back(comp[v]);
        arbol[comp[v]].push_back(comp[u]);
      }
    return c;
  }
};
