// Interseccion de segmentos. Requiere Pt, orient, signo,
// en_segmento. Exacto con enteros, O(1).
// Los segmentos cerrados ab y cd comparten algun punto?
bool se_intersecan(Pt a, Pt b, Pt c, Pt d) {
  int o1 = signo(orient(a, b, c)), o2 = signo(orient(a, b, d));
  int o3 = signo(orient(c, d, a)), o4 = signo(orient(c, d, b));
  if (o1 * o2 < 0 && o3 * o4 < 0) return true;  // cruce propio
  return en_segmento(a, b, c) || en_segmento(a, b, d) ||
         en_segmento(c, d, a) || en_segmento(c, d, b);
}
// Punto de interseccion de las RECTAS ab y cd.
// Retorna false si son paralelas (o coincidentes).
bool interseccion_rectas(Pt a, Pt b, Pt c, Pt d, PtD& res) {
  long long den = (b - a).cross(d - c);
  if (den == 0) return false;
  double t = (double)(c - a).cross(d - c) / (double)den;
  res = {a.x + t * (b.x - a.x), a.y + t * (b.y - a.y)};
  return true;
}
