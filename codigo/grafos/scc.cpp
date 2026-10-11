// Componentes fuertemente conexas (Tarjan) + grafo reducido.
// run() retorna la cantidad de componentes. comp[u] queda en
// orden topologico: si hay arista c1 -> c2 en el DAG, c1 < c2.
// O(n + m).
struct SCC {
  int n, reloj = 0, ncomp = 0;
  vector<vector<int>> adj;
  vector<int> tin, low, comp, pila;
  SCC(int n_) : n(n_), adj(n_), tin(n_, -1), low(n_, 0),
                comp(n_, -1) {}
  void add_edge(int u, int v) { adj[u].push_back(v); }
  void dfs(int u) {
    tin[u] = low[u] = reloj++;
    pila.push_back(u);
    for (int v : adj[u]) {
      if (tin[v] == -1) {
        dfs(v);
        low[u] = min(low[u], low[v]);
      } else if (comp[v] == -1) {
        low[u] = min(low[u], tin[v]);
      }
    }
    if (low[u] == tin[u]) {
      while (true) {
        int v = pila.back();
        pila.pop_back();
        comp[v] = ncomp;
        if (v == u) break;
      }
      ncomp++;
    }
  }
  int run() {
    for (int u = 0; u < n; u++)
      if (tin[u] == -1) dfs(u);
    for (int& c : comp) c = ncomp - 1 - c;
    return ncomp;
  }
  // Grafo reducido (DAG de componentes), sin aristas repetidas.
  vector<vector<int>> condensar() {
    vector<vector<int>> dag(ncomp);
    for (int u = 0; u < n; u++)
      for (int v : adj[u])
        if (comp[u] != comp[v]) dag[comp[u]].push_back(comp[v]);
    for (auto& fila : dag) {
      sort(fila.begin(), fila.end());
      fila.erase(unique(fila.begin(), fila.end()), fila.end());
    }
    return dag;
  }
};
