// Pares antipodales con rotating calipers. O(n). Requiere Pt.
// h = poligono estrictamente convexo en sentido antihorario
// (la salida de convex_hull). El diametro (par de puntos mas
// lejano) siempre es uno de los pares devueltos.
vector<pair<int, int>> antipodal_pairs(const vector<Pt>& h) {
  int n = h.size();
  vector<pair<int, int>> pairs;
  if (n < 2) return pairs;
  if (n == 2) return {{0, 1}};
  for (int i = 0, j = 1; i < n; i++) {
    Pt side = h[(i + 1) % n] - h[i];
    // avanzar j mientras se aleje de la recta del lado i
    while (side.cross(h[(j + 1) % n] - h[j]) > 0) j = (j + 1) % n;
    pairs.push_back({i, j});
    pairs.push_back({(i + 1) % n, j});
  }
  return pairs;
}
// Cuadrado de la mayor distancia entre dos puntos del poligono.
long long diameter2(const vector<Pt>& h) {
  long long best = 0;
  for (auto [i, j] : antipodal_pairs(h))
    best = max(best, (h[i] - h[j]).norm2());
  return best;
}
