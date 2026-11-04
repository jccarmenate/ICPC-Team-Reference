// Heavy-Light Decomposition. Requiere SegTree (lazy).
// Valores en los vertices. Uso: add_edge de las n-1 aristas y
// luego build(root). Con hld.lca(u, v) ya se tiene el LCA.
// Camino: O(log^2 n). Subarbol y LCA: O(log n).
// Valores en aristas: guardar cada arista en su vertice hijo y
// en el ultimo tramo de path() usar f(pos[u] + 1, pos[v]).
struct HLD {
  int n, timer = 0;
  vector<vector<int>> adj;
  vector<int> parent, depth, sz, heavy, head, pos;
  SegTree st;
  HLD(int n_) : n(n_), adj(n_), parent(n_), depth(n_), sz(n_),
                heavy(n_), head(n_), pos(n_), st(n_) {}
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  void dfs_size(int u) {
    sz[u] = 1;
    heavy[u] = -1;
    for (int v : adj[u]) {
      if (v == parent[u]) continue;
      parent[v] = u;
      depth[v] = depth[u] + 1;
      dfs_size(v);
      sz[u] += sz[v];
      if (heavy[u] == -1 || sz[v] > sz[heavy[u]]) heavy[u] = v;
    }
  }
  void dfs_hld(int u, int c) {
    head[u] = c;
    pos[u] = timer++;
    if (heavy[u] != -1) dfs_hld(heavy[u], c);
    for (int v : adj[u])
      if (v != parent[u] && v != heavy[u]) dfs_hld(v, v);
  }
  void build(int root = 0) {
    parent[root] = root;
    depth[root] = 0;
    dfs_size(root);
    dfs_hld(root, root);
  }
  int lca(int u, int v) {
    while (head[u] != head[v]) {
      if (depth[head[u]] < depth[head[v]]) swap(u, v);
      u = parent[head[u]];
    }
    return depth[u] < depth[v] ? u : v;
  }
  int dist(int u, int v) {
    return depth[u] + depth[v] - 2 * depth[lca(u, v)];
  }
  // Llama f(l, r) para cada tramo [l, r] del camino u-v.
  template <class F>
  void path(int u, int v, F f) {
    while (head[u] != head[v]) {
      if (depth[head[u]] < depth[head[v]]) swap(u, v);
      f(pos[head[u]], pos[u]);
      u = parent[head[u]];
    }
    if (depth[u] > depth[v]) swap(u, v);
    f(pos[u], pos[v]);
  }
  void update_path(int u, int v, long long x) {
    path(u, v, [&](int l, int r) { st.update(l, r, x); });
  }
  long long query_path(int u, int v) {
    long long s = 0;
    path(u, v, [&](int l, int r) { s += st.query(l, r); });
    return s;
  }
  void update_subtree(int u, long long x) {
    st.update(pos[u], pos[u] + sz[u] - 1, x);
  }
  long long query_subtree(int u) {
    return st.query(pos[u], pos[u] + sz[u] - 1);
  }
};
