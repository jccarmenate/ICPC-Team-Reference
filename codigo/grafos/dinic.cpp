// Flujo maximo (Dinic). O(V^2 E); O(E sqrt V) en grafos unitarios
// (matching bipartito). En la practica es muy rapido.
// add_edge(u, v, cap, rcap): rcap = cap de vuelta (no dirigido).
// Tras max_flow: nivel[u] != -1 <=> u queda del lado de s en el
// corte minimo. Flujo por la arista i-esima: e[2*i+1].cap
// (solo si se agrego con rcap = 0).
struct Dinic {
  struct Arista { int to; long long cap; };
  int n;
  vector<Arista> e;
  vector<vector<int>> adj;
  vector<int> nivel, it;
  Dinic(int n_) : n(n_), adj(n_), nivel(n_), it(n_) {}
  void add_edge(int u, int v, long long cap, long long rcap = 0) {
    adj[u].push_back(e.size());
    e.push_back({v, cap});
    adj[v].push_back(e.size());
    e.push_back({u, rcap});
  }
  bool bfs(int s, int t) {
    fill(nivel.begin(), nivel.end(), -1);
    nivel[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int id : adj[u]) {
        int v = e[id].to;
        if (e[id].cap > 0 && nivel[v] == -1) {
          nivel[v] = nivel[u] + 1;
          q.push(v);
        }
      }
    }
    return nivel[t] != -1;
  }
  long long dfs(int u, int t, long long f) {
    if (u == t) return f;
    for (int& i = it[u]; i < (int)adj[u].size(); i++) {
      int id = adj[u][i], v = e[id].to;
      if (e[id].cap == 0 || nivel[v] != nivel[u] + 1) continue;
      long long d = dfs(v, t, min(f, e[id].cap));
      if (d > 0) {
        e[id].cap -= d;
        e[id ^ 1].cap += d;
        return d;
      }
    }
    return 0;
  }
  long long max_flow(int s, int t) {
    long long flujo = 0;
    while (bfs(s, t)) {
      fill(it.begin(), it.end(), 0);
      while (long long f = dfs(s, t, LLONG_MAX)) flujo += f;
    }
    return flujo;
  }
};
