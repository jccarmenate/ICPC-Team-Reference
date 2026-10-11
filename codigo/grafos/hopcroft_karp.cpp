// Matching bipartito maximo (Hopcroft-Karp). O(E sqrt V).
// Izquierda 0..n-1, derecha 0..m-1. add_edge(u izq, v der).
// Konig: cobertura minima de vertices = matching maximo;
// conjunto independiente maximo = n + m - matching.
struct HopcroftKarp {
  int n, m;
  vector<vector<int>> adj;
  vector<int> par_izq, par_der, dist;
  HopcroftKarp(int n_, int m_) : n(n_), m(m_), adj(n_),
      par_izq(n_, -1), par_der(m_, -1), dist(n_) {}
  void add_edge(int u, int v) { adj[u].push_back(v); }
  bool bfs() {
    queue<int> q;
    bool hay_camino = false;
    for (int u = 0; u < n; u++) {
      dist[u] = par_izq[u] == -1 ? 0 : -1;
      if (dist[u] == 0) q.push(u);
    }
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int v : adj[u]) {
        int w = par_der[v];
        if (w == -1) hay_camino = true;
        else if (dist[w] == -1) {
          dist[w] = dist[u] + 1;
          q.push(w);
        }
      }
    }
    return hay_camino;
  }
  bool dfs(int u) {
    for (int v : adj[u]) {
      int w = par_der[v];
      if (w == -1 || (dist[w] == dist[u] + 1 && dfs(w))) {
        par_izq[u] = v;
        par_der[v] = u;
        return true;
      }
    }
    dist[u] = -1;
    return false;
  }
  int matching() {
    int res = 0;
    while (bfs())
      for (int u = 0; u < n; u++)
        if (par_izq[u] == -1 && dfs(u)) res++;
    return res;
  }
};
