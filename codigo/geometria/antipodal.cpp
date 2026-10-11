// Pares antipodales con rotating calipers. O(n). Requiere Pt.
// h = poligono estrictamente convexo en sentido antihorario
// (la salida de convex_hull). El diametro (par de puntos mas
// lejano) siempre es uno de los pares devueltos.
vector<pair<int, int>> antipodales(const vector<Pt>& h) {
  int n = h.size();
  vector<pair<int, int>> pares;
  if (n < 2) return pares;
  if (n == 2) return {{0, 1}};
  for (int i = 0, j = 1; i < n; i++) {
    Pt lado = h[(i + 1) % n] - h[i];
    // avanzar j mientras se aleje de la recta del lado i
    while (lado.cross(h[(j + 1) % n] - h[j]) > 0) j = (j + 1) % n;
    pares.push_back({i, j});
    pares.push_back({(i + 1) % n, j});
  }
  return pares;
}
// Cuadrado de la mayor distancia entre dos puntos del poligono.
long long diametro2(const vector<Pt>& h) {
  long long mejor = 0;
  for (auto [i, j] : antipodales(h))
    mejor = max(mejor, (h[i] - h[j]).norm2());
  return mejor;
}
