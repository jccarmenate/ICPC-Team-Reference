// Sparse Table: minimo en rango (estatico).
// Construccion O(n log n), consulta O(1). Rango cerrado [l, r].
struct SparseTable {
  vector<vector<int>> t;
  SparseTable(const vector<int>& a) {
    int n = a.size(), niveles = 1;
    while ((1 << niveles) <= n) niveles++;
    t.assign(niveles, a);
    for (int k = 1; k < niveles; k++)
      for (int i = 0; i + (1 << k) <= n; i++)
        t[k][i] = min(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
  }
  int query(int l, int r) {
    int k = 31 - __builtin_clz(r - l + 1);
    return min(t[k][l], t[k][r - (1 << k) + 1]);
  }
};
