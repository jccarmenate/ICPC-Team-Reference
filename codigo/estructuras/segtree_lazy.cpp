// Segment Tree recursivo (arreglo de 4N) con lazy propagation.
// update(l, r, x): suma x a todo [l, r].
// query(l, r): suma de [l, r]. Indices 0..n-1, rangos cerrados.
// O(log n) por operacion.
// Para min/max: cambiar la combinacion y el neutro de query, y
// en aplicar() usar t[v] += x (sin multiplicar por el largo).
struct SegTree {
  int n;
  vector<long long> t, lz;
  SegTree(int n_) : n(n_), t(4 * n_, 0), lz(4 * n_, 0) {}
  SegTree(const vector<long long>& a) : SegTree((int)a.size()) {
    build(1, 0, n - 1, a);
  }
  void build(int v, int tl, int tr, const vector<long long>& a) {
    if (tl == tr) { t[v] = a[tl]; return; }
    int mid = (tl + tr) / 2;
    build(2 * v, tl, mid, a);
    build(2 * v + 1, mid + 1, tr, a);
    t[v] = t[2 * v] + t[2 * v + 1];
  }
  void aplicar(int v, int tl, int tr, long long x) {
    t[v] += x * (tr - tl + 1);
    lz[v] += x;
  }
  void push(int v, int tl, int tr) {
    if (lz[v] == 0) return;
    int mid = (tl + tr) / 2;
    aplicar(2 * v, tl, mid, lz[v]);
    aplicar(2 * v + 1, mid + 1, tr, lz[v]);
    lz[v] = 0;
  }
  void update(int v, int tl, int tr, int l, int r, long long x) {
    if (r < tl || tr < l) return;
    if (l <= tl && tr <= r) { aplicar(v, tl, tr, x); return; }
    push(v, tl, tr);
    int mid = (tl + tr) / 2;
    update(2 * v, tl, mid, l, r, x);
    update(2 * v + 1, mid + 1, tr, l, r, x);
    t[v] = t[2 * v] + t[2 * v + 1];
  }
  long long query(int v, int tl, int tr, int l, int r) {
    if (r < tl || tr < l) return 0;
    if (l <= tl && tr <= r) return t[v];
    push(v, tl, tr);
    int mid = (tl + tr) / 2;
    return query(2 * v, tl, mid, l, r) +
           query(2 * v + 1, mid + 1, tr, l, r);
  }
  void update(int l, int r, long long x) {
    update(1, 0, n - 1, l, r, x);
  }
  long long query(int l, int r) {
    return query(1, 0, n - 1, l, r);
  }
};
