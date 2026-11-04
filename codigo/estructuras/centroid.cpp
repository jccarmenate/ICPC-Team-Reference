// Centroid Decomposition. build() arma el arbol de centroides
// (altura O(log n)) en O(n log n).
// parent[c]: padre de c en el arbol de centroides (-1 la raiz).
// dist[k][v]: distancia de v a su ancestro centroide de nivel k.
// Ejemplo incluido: mark(v) y closest(v) = distancia al
// vertice marcado mas cercano, ambas en O(log n).
struct Centroid {
  int n;
  vector<vector<int>> adj, dist;
  vector<int> sz, parent, level, best;
  vector<bool> removed;
  Centroid(int n_) : n(n_), adj(n_), sz(n_), parent(n_, -1),
      level(n_, 0), best(n_, INT_MAX / 2), removed(n_, false) {}
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  int calc_size(int u, int p) {
    sz[u] = 1;
    for (int v : adj[u])
      if (v != p && !removed[v]) sz[u] += calc_size(v, u);
    return sz[u];
  }
  int find_centroid(int u, int p, int total) {
    for (int v : adj[u])
      if (v != p && !removed[v] && 2 * sz[v] > total)
        return find_centroid(v, u, total);
    return u;
  }
  void calc_dist(int u, int p, int k, int d) {
    dist[k][u] = d;
    for (int v : adj[u])
      if (v != p && !removed[v]) calc_dist(v, u, k, d + 1);
  }
  void build(int u = 0, int p = -1, int k = 0) {
    int c = find_centroid(u, -1, calc_size(u, -1));
    parent[c] = p;
    level[c] = k;
    if ((int)dist.size() <= k) dist.emplace_back(n, 0);
    calc_dist(c, -1, k, 0);
    // Aqui: procesar los caminos que pasan por c
    // (DFS desde c sin cruzar vertices quitados).
    removed[c] = true;
    for (int v : adj[c])
      if (!removed[v]) build(v, c, k + 1);
  }
  void mark(int v) {
    for (int c = v; c != -1; c = parent[c])
      best[c] = min(best[c], dist[level[c]][v]);
  }
  int closest(int v) {  // >= INT_MAX / 2 si no hay marcados
    int res = INT_MAX / 2;
    for (int c = v; c != -1; c = parent[c])
      res = min(res, best[c] + dist[level[c]][v]);
    return res;
  }
};
