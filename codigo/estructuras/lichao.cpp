// Li Chao Tree dinamico: minimo de rectas y = m x + b evaluadas
// en x entero de [lo, hi]. add y query en O(log(hi - lo)).
// Sirve para Convex Hull Trick sin exigir pendientes ordenadas.
// Para maximo: insertar (-m, -b) y negar la respuesta.
// Cuidado: m * x + b debe caber en long long.
struct LiChao {
  struct Line {
    long long m, b;
    long long eval(long long x) const { return m * x + b; }
  };
  struct Node { Line r; int lc, rc; };
  long long lo, hi;
  vector<Node> t;
  LiChao(long long lo_, long long hi_) : lo(lo_), hi(hi_) {
    new_node();
  }
  int new_node() {
    t.push_back({{0, LLONG_MAX}, -1, -1});
    return (int)t.size() - 1;
  }
  void add(Line added, int v, long long l, long long r) {
    long long mid = l + (r - l) / 2;
    bool wins_left = added.eval(l) < t[v].r.eval(l);
    bool wins_mid = added.eval(mid) < t[v].r.eval(mid);
    if (wins_mid) swap(t[v].r, added);
    if (l == r || added.b == LLONG_MAX) return;
    if (wins_left != wins_mid) {
      if (t[v].lc == -1) { int id = new_node(); t[v].lc = id; }
      add(added, t[v].lc, l, mid);
    } else {
      if (t[v].rc == -1) { int id = new_node(); t[v].rc = id; }
      add(added, t[v].rc, mid + 1, r);
    }
  }
  void add(long long m, long long b) { add({m, b}, 0, lo, hi); }
  long long query(long long x) {  // LLONG_MAX si no hay rectas
    long long res = LLONG_MAX, l = lo, r = hi;
    int v = 0;
    while (v != -1) {
      res = min(res, t[v].r.eval(x));
      long long mid = l + (r - l) / 2;
      if (x <= mid) { v = t[v].lc; r = mid; }
      else { v = t[v].rc; l = mid + 1; }
    }
    return res;
  }
};
