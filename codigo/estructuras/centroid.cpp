// Centroid Decomposition. build() arma el arbol de centroides
// (altura O(log n)) en O(n log n).
// padre[c]: padre de c en el arbol de centroides (-1 la raiz).
// dist[k][v]: distancia de v a su ancestro centroide de nivel k.
// Ejemplo incluido: marcar(v) y mas_cercano(v) = distancia al
// vertice marcado mas cercano, ambas en O(log n).
struct Centroid {
  int n;
  vector<vector<int>> adj, dist;
  vector<int> tam, padre, nivel, mejor;
  vector<bool> quitado;
  Centroid(int n_) : n(n_), adj(n_), tam(n_), padre(n_, -1),
      nivel(n_, 0), mejor(n_, INT_MAX / 2), quitado(n_, false) {}
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  int calc_tam(int u, int p) {
    tam[u] = 1;
    for (int v : adj[u])
      if (v != p && !quitado[v]) tam[u] += calc_tam(v, u);
    return tam[u];
  }
  int buscar(int u, int p, int total) {
    for (int v : adj[u])
      if (v != p && !quitado[v] && 2 * tam[v] > total)
        return buscar(v, u, total);
    return u;
  }
  void calc_dist(int u, int p, int k, int d) {
    dist[k][u] = d;
    for (int v : adj[u])
      if (v != p && !quitado[v]) calc_dist(v, u, k, d + 1);
  }
  void build(int u = 0, int p = -1, int k = 0) {
    int c = buscar(u, -1, calc_tam(u, -1));
    padre[c] = p;
    nivel[c] = k;
    if ((int)dist.size() <= k) dist.emplace_back(n, 0);
    calc_dist(c, -1, k, 0);
    // Aqui: procesar los caminos que pasan por c
    // (DFS desde c sin cruzar vertices quitados).
    quitado[c] = true;
    for (int v : adj[c])
      if (!quitado[v]) build(v, c, k + 1);
  }
  void marcar(int v) {
    for (int c = v; c != -1; c = padre[c])
      mejor[c] = min(mejor[c], dist[nivel[c]][v]);
  }
  int mas_cercano(int v) {  // >= INT_MAX / 2 si no hay marcados
    int res = INT_MAX / 2;
    for (int c = v; c != -1; c = padre[c])
      res = min(res, mejor[c] + dist[nivel[c]][v]);
    return res;
  }
};
