// 2-SAT. Requiere SCC. Variables 0..n-1. O(n + clausulas).
// add_or(a, va, b, vb): agrega la clausula
//   (x_a == va) OR (x_b == vb).
// resolver(): false si es insatisfacible; si no, valor[i] es una
// asignacion valida.
struct TwoSat {
  int n;
  SCC scc;
  vector<bool> valor;
  TwoSat(int n_) : n(n_), scc(2 * n_), valor(n_, false) {}
  // nodo 2a = "x_a verdadero", nodo 2a+1 = "x_a falso"
  void add_or(int a, bool va, int b, bool vb) {
    int x = 2 * a + (va ? 0 : 1), y = 2 * b + (vb ? 0 : 1);
    scc.add_edge(x ^ 1, y);
    scc.add_edge(y ^ 1, x);
  }
  void forzar(int a, bool va) { add_or(a, va, a, va); }
  void implica(int a, bool va, int b, bool vb) {
    add_or(a, !va, b, vb);
  }
  void distintos(int a, int b) {  // x_a XOR x_b
    add_or(a, true, b, true);
    add_or(a, false, b, false);
  }
  bool resolver() {
    scc.run();
    for (int i = 0; i < n; i++) {
      if (scc.comp[2 * i] == scc.comp[2 * i + 1]) return false;
      valor[i] = scc.comp[2 * i] > scc.comp[2 * i + 1];
    }
    return true;
  }
};
