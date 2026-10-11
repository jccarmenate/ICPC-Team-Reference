// Punto en poligono. Requiere Pt, orient, en_segmento.
// Ambas retornan 0 = fuera, 1 = en el borde, 2 = dentro.

// Poligono simple cualquiera (convexo o no). O(n).
int en_poligono(const vector<Pt>& pol, Pt p) {
  int n = pol.size();
  bool dentro = false;
  for (int i = 0; i < n; i++) {
    Pt a = pol[i], b = pol[(i + 1) % n];
    if (en_segmento(a, b, p)) return 1;
    if (a.y > b.y) swap(a, b);
    // el rayo horizontal hacia la derecha cruza el lado ab?
    if (a.y <= p.y && p.y < b.y && orient(a, b, p) > 0)
      dentro = !dentro;
  }
  return dentro ? 2 : 0;
}

// Poligono estrictamente convexo en sentido antihorario con
// n >= 3 (la salida de convex_hull). O(log n).
int en_convexo(const vector<Pt>& h, Pt p) {
  int n = h.size();
  long long izq = orient(h[0], h[1], p);
  long long der = orient(h[0], h[n - 1], p);
  if (izq < 0 || der > 0) return 0;
  int lo = 1, hi = n - 1;
  while (hi - lo > 1) {
    int mid = (lo + hi) / 2;
    if (orient(h[0], h[mid], p) >= 0) lo = mid;
    else hi = mid;
  }
  long long o = orient(h[lo], h[hi], p);
  if (o < 0) return 0;
  if (o == 0) return 1;
  if (lo == 1 && izq == 0) return 1;
  if (hi == n - 1 && der == 0) return 1;
  return 2;
}
