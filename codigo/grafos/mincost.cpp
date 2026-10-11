// Flujo maximo de costo minimo (caminos de aumento con SPFA).
// O(F * V * E) en el peor caso; suficiente en la practica.
// Admite costos negativos mientras no haya ciclos negativos.
// flujo(s, t, limite) retorna (flujo enviado, costo total).
struct MinCostFlow {
  struct Arista { int to; long long cap, costo; };
  int n;
  vector<Arista> e;
  vector<vector<int>> adj;
  vector<long long> dist;
  vector<int> previa;
  vector<bool> en_cola;
  MinCostFlow(int n_) : n(n_), adj(n_), dist(n_), previa(n_),
                        en_cola(n_) {}
  void add_edge(int u, int v, long long cap, long long costo) {
    adj[u].push_back(e.size());
    e.push_back({v, cap, costo});
    adj[v].push_back(e.size());
    e.push_back({u, 0, -costo});
  }
  bool spfa(int s, int t) {
    fill(dist.begin(), dist.end(), LLONG_MAX);
    fill(en_cola.begin(), en_cola.end(), false);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      en_cola[u] = false;
      for (int id : adj[u]) {
        int v = e[id].to;
        if (e[id].cap > 0 && dist[u] + e[id].costo < dist[v]) {
          dist[v] = dist[u] + e[id].costo;
          previa[v] = id;
          if (!en_cola[v]) {
            en_cola[v] = true;
            q.push(v);
          }
        }
      }
    }
    return dist[t] != LLONG_MAX;
  }
  pair<long long, long long> flujo(int s, int t,
                                   long long limite = LLONG_MAX) {
    long long f = 0, c = 0;
    while (f < limite && spfa(s, t)) {
      long long aum = limite - f;
      for (int v = t; v != s; v = e[previa[v] ^ 1].to)
        aum = min(aum, e[previa[v]].cap);
      for (int v = t; v != s; v = e[previa[v] ^ 1].to) {
        e[previa[v]].cap -= aum;
        e[previa[v] ^ 1].cap += aum;
      }
      f += aum;
      c += aum * dist[t];
    }
    return {f, c};
  }
};
