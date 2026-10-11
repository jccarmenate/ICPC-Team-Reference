// Segment Tree iterativo (arreglo de 2N), sin recursion.
// update(p, v): asigna a[p] = v.  query(l, r): rango [l, r).
// O(log n). Cambiar NEUTRO y combinar() para min, max, gcd...
struct SegTree2N {
  int n;
  vector<long long> t;
  long long NEUTRO = 0;
  long long combinar(long long a, long long b) { return a + b; }
  SegTree2N(const vector<long long>& a)
      : n(a.size()), t(2 * a.size()) {
    for (int i = 0; i < n; i++) t[n + i] = a[i];
    for (int i = n - 1; i >= 1; i--)
      t[i] = combinar(t[2 * i], t[2 * i + 1]);
  }
  void update(int p, long long v) {
    p += n;
    t[p] = v;
    for (p /= 2; p >= 1; p /= 2)
      t[p] = combinar(t[2 * p], t[2 * p + 1]);
  }
  long long query(int l, int r) {  // [l, r)
    long long izq = NEUTRO, der = NEUTRO;
    for (l += n, r += n; l < r; l /= 2, r /= 2) {
      if (l & 1) izq = combinar(izq, t[l++]);
      if (r & 1) der = combinar(t[--r], der);
    }
    return combinar(izq, der);
  }
};
