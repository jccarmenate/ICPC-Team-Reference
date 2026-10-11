// Convex Hull (Andrew, cadena monotona). O(n log n). Requiere Pt.
// Retorna el poligono en sentido antihorario, sin puntos
// colineales ni repetidos. Para conservar los colineales del
// borde cambiar <= 0 por < 0.
vector<Pt> convex_hull(vector<Pt> p) {
  sort(p.begin(), p.end());
  p.erase(unique(p.begin(), p.end()), p.end());
  if (p.size() < 3) return p;
  vector<Pt> h;
  for (int pasada = 0; pasada < 2; pasada++) {
    int base = h.size();
    for (Pt q : p) {
      while ((int)h.size() >= base + 2 &&
             orient(h[h.size() - 2], h.back(), q) <= 0)
        h.pop_back();
      h.push_back(q);
    }
    h.pop_back();
    reverse(p.begin(), p.end());
  }
  return h;
}
