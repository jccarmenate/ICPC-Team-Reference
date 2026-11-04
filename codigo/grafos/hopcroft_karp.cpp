// Matching bipartito maximo (Hopcroft-Karp). O(E sqrt V).
// Izquierda 0..n-1, derecha 0..m-1. add_edge(u izq, v der).
// Konig: cobertura minima de vertices = matching maximo;
// conjunto independiente maximo = n + m - matching.
struct HopcroftKarp {
  int n, m;
  vector<vector<int>> adj;
  vector<int> match_left, match_right, dist;
  HopcroftKarp(int n_, int m_) : n(n_), m(m_), adj(n_),
      match_left(n_, -1), match_right(m_, -1), dist(n_) {}
  void add_edge(int u, int v) { adj[u].push_back(v); }
  bool bfs() {
    queue<int> q;
    bool found_path = false;
    for (int u = 0; u < n; u++) {
      dist[u] = match_left[u] == -1 ? 0 : -1;
      if (dist[u] == 0) q.push(u);
    }
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int v : adj[u]) {
        int w = match_right[v];
        if (w == -1) found_path = true;
        else if (dist[w] == -1) {
          dist[w] = dist[u] + 1;
          q.push(w);
        }
      }
    }
    return found_path;
  }
  bool dfs(int u) {
    for (int v : adj[u]) {
      int w = match_right[v];
      if (w == -1 || (dist[w] == dist[u] + 1 && dfs(w))) {
        match_left[u] = v;
        match_right[v] = u;
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
        if (match_left[u] == -1 && dfs(u)) res++;
    return res;
  }
};
