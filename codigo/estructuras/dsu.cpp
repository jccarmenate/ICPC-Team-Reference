// Union-Find con compresion de caminos y union por tamano.
// Casi O(1) amortizado por operacion.
struct DSU {
  vector<int> parent, sz;
  int comps;
  DSU(int n) : parent(n), sz(n, 1), comps(n) {
    iota(parent.begin(), parent.end(), 0);
  }
  int find(int x) {
    return parent[x] == x ? x : parent[x] = find(parent[x]);
  }
  bool unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return false;
    if (sz[a] < sz[b]) swap(a, b);
    parent[b] = a;
    sz[a] += sz[b];
    comps--;
    return true;
  }
};
