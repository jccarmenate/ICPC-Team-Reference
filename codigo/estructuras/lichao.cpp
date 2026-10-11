// Li Chao Tree dinamico: minimo de rectas y = m x + b evaluadas
// en x entero de [lo, hi]. add y query en O(log(hi - lo)).
// Sirve para Convex Hull Trick sin exigir pendientes ordenadas.
// Para maximo: insertar (-m, -b) y negar la respuesta.
// Cuidado: m * x + b debe caber en long long.
struct LiChao {
  struct Recta {
    long long m, b;
    long long eval(long long x) const { return m * x + b; }
  };
  struct Nodo { Recta r; int izq, der; };
  long long lo, hi;
  vector<Nodo> t;
  LiChao(long long lo_, long long hi_) : lo(lo_), hi(hi_) {
    nuevo();
  }
  int nuevo() {
    t.push_back({{0, LLONG_MAX}, -1, -1});
    return (int)t.size() - 1;
  }
  void add(Recta nueva, int v, long long l, long long r) {
    long long mid = l + (r - l) / 2;
    bool gana_izq = nueva.eval(l) < t[v].r.eval(l);
    bool gana_mid = nueva.eval(mid) < t[v].r.eval(mid);
    if (gana_mid) swap(t[v].r, nueva);
    if (l == r || nueva.b == LLONG_MAX) return;
    if (gana_izq != gana_mid) {
      if (t[v].izq == -1) { int id = nuevo(); t[v].izq = id; }
      add(nueva, t[v].izq, l, mid);
    } else {
      if (t[v].der == -1) { int id = nuevo(); t[v].der = id; }
      add(nueva, t[v].der, mid + 1, r);
    }
  }
  void add(long long m, long long b) { add({m, b}, 0, lo, hi); }
  long long query(long long x) {  // LLONG_MAX si no hay rectas
    long long res = LLONG_MAX, l = lo, r = hi;
    int v = 0;
    while (v != -1) {
      res = min(res, t[v].r.eval(x));
      long long mid = l + (r - l) / 2;
      if (x <= mid) { v = t[v].izq; r = mid; }
      else { v = t[v].der; l = mid + 1; }
    }
    return res;
  }
};
