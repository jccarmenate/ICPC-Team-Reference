// Flujo maximo de costo minimo (caminos de aumento con SPFA).
// O(F * V * E) en el peor caso; suficiente en la practica.
// Admite costos negativos mientras no haya ciclos negativos.
// flow(s, t, limit) retorna (flujo enviado, costo total).
struct MinCostFlow {
  struct Edge { int to; long long cap, cost; };
  int n;
  vector<Edge> e;
  vector<vector<int>> adj;
  vector<long long> dist;
  vector<int> prev_edge;
  vector<bool> in_queue;
  MinCostFlow(int n_) : n(n_), adj(n_), dist(n_), prev_edge(n_),
                        in_queue(n_) {}
  void add_edge(int u, int v, long long cap, long long cost) {
    adj[u].push_back(e.size());
    e.push_back({v, cap, cost});
    adj[v].push_back(e.size());
    e.push_back({u, 0, -cost});
  }
  bool spfa(int s, int t) {
    fill(dist.begin(), dist.end(), LLONG_MAX);
    fill(in_queue.begin(), in_queue.end(), false);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      in_queue[u] = false;
      for (int id : adj[u]) {
        int v = e[id].to;
        if (e[id].cap > 0 && dist[u] + e[id].cost < dist[v]) {
          dist[v] = dist[u] + e[id].cost;
          prev_edge[v] = id;
          if (!in_queue[v]) {
            in_queue[v] = true;
            q.push(v);
          }
        }
      }
    }
    return dist[t] != LLONG_MAX;
  }
  pair<long long, long long> flow(int s, int t,
                                   long long limit = LLONG_MAX) {
    long long f = 0, c = 0;
    while (f < limit && spfa(s, t)) {
      long long aug = limit - f;
      for (int v = t; v != s; v = e[prev_edge[v] ^ 1].to)
        aug = min(aug, e[prev_edge[v]].cap);
      for (int v = t; v != s; v = e[prev_edge[v] ^ 1].to) {
        e[prev_edge[v]].cap -= aug;
        e[prev_edge[v] ^ 1].cap += aug;
      }
      f += aug;
      c += aug * dist[t];
    }
    return {f, c};
  }
};
