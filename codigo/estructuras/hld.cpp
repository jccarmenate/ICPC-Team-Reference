// Heavy-Light Decomposition. Requiere SegTree (lazy).
// Valores en los vertices. Uso: add_edge de las n-1 aristas y
// luego build(raiz). Con hld.lca(u, v) ya se tiene el LCA.
// Camino: O(log^2 n). Subarbol y LCA: O(log n).
// Valores en aristas: guardar cada arista en su vertice hijo y
// en el ultimo tramo de camino() usar f(pos[u] + 1, pos[v]).
struct HLD {
  int n, reloj = 0;
  vector<vector<int>> adj;
  vector<int> padre, prof, tam, pesado, cabeza, pos;
  SegTree st;
  HLD(int n_) : n(n_), adj(n_), padre(n_), prof(n_), tam(n_),
                pesado(n_), cabeza(n_), pos(n_), st(n_) {}
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  void dfs_tam(int u) {
    tam[u] = 1;
    pesado[u] = -1;
    for (int v : adj[u]) {
      if (v == padre[u]) continue;
      padre[v] = u;
      prof[v] = prof[u] + 1;
      dfs_tam(v);
      tam[u] += tam[v];
      if (pesado[u] == -1 || tam[v] > tam[pesado[u]]) pesado[u] = v;
    }
  }
  void dfs_hld(int u, int c) {
    cabeza[u] = c;
    pos[u] = reloj++;
    if (pesado[u] != -1) dfs_hld(pesado[u], c);
    for (int v : adj[u])
      if (v != padre[u] && v != pesado[u]) dfs_hld(v, v);
  }
  void build(int raiz = 0) {
    padre[raiz] = raiz;
    prof[raiz] = 0;
    dfs_tam(raiz);
    dfs_hld(raiz, raiz);
  }
  int lca(int u, int v) {
    while (cabeza[u] != cabeza[v]) {
      if (prof[cabeza[u]] < prof[cabeza[v]]) swap(u, v);
      u = padre[cabeza[u]];
    }
    return prof[u] < prof[v] ? u : v;
  }
  int dist(int u, int v) {
    return prof[u] + prof[v] - 2 * prof[lca(u, v)];
  }
  // Llama f(l, r) para cada tramo [l, r] del camino u-v.
  template <class F>
  void camino(int u, int v, F f) {
    while (cabeza[u] != cabeza[v]) {
      if (prof[cabeza[u]] < prof[cabeza[v]]) swap(u, v);
      f(pos[cabeza[u]], pos[u]);
      u = padre[cabeza[u]];
    }
    if (prof[u] > prof[v]) swap(u, v);
    f(pos[u], pos[v]);
  }
  void update_camino(int u, int v, long long x) {
    camino(u, v, [&](int l, int r) { st.update(l, r, x); });
  }
  long long query_camino(int u, int v) {
    long long s = 0;
    camino(u, v, [&](int l, int r) { s += st.query(l, r); });
    return s;
  }
  void update_subarbol(int u, long long x) {
    st.update(pos[u], pos[u] + tam[u] - 1, x);
  }
  long long query_subarbol(int u) {
    return st.query(pos[u], pos[u] + tam[u] - 1);
  }
};
