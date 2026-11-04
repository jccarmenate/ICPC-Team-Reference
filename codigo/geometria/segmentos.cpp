// Interseccion de segmentos. Requiere Pt, orient, sign,
// on_segment. Exacto con enteros, O(1).
// Los segmentos cerrados ab y cd comparten algun punto?
bool segments_intersect(Pt a, Pt b, Pt c, Pt d) {
  int o1 = sign(orient(a, b, c)), o2 = sign(orient(a, b, d));
  int o3 = sign(orient(c, d, a)), o4 = sign(orient(c, d, b));
  if (o1 * o2 < 0 && o3 * o4 < 0) return true;  // cruce propio
  return on_segment(a, b, c) || on_segment(a, b, d) ||
         on_segment(c, d, a) || on_segment(c, d, b);
}
// Punto de interseccion de las RECTAS ab y cd.
// Retorna false si son paralelas (o coincidentes).
bool line_intersection(Pt a, Pt b, Pt c, Pt d, PtD& res) {
  long long den = (b - a).cross(d - c);
  if (den == 0) return false;
  double t = (double)(c - a).cross(d - c) / (double)den;
  res = {a.x + t * (b.x - a.x), a.y + t * (b.y - a.y)};
  return true;
}
