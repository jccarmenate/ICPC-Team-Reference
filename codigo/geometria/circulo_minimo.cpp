// Circulo minimo que encierra un conjunto de puntos (Welzl
// incremental aleatorizado). O(n) esperado. Requiere PtD.
struct Circulo { double x, y, r; };

bool contiene(const Circulo& c, PtD p) {
  return hypot(p.x - c.x, p.y - c.y) <= c.r + 1e-9;
}
Circulo por_dos(PtD a, PtD b) {
  return {(a.x + b.x) / 2, (a.y + b.y) / 2,
          hypot(a.x - b.x, a.y - b.y) / 2};
}
Circulo por_tres(PtD a, PtD b, PtD c) {  // circunscrito
  double bx = b.x - a.x, by = b.y - a.y;
  double cx = c.x - a.x, cy = c.y - a.y;
  double d = 2 * (bx * cy - by * cx);
  double b2 = bx * bx + by * by, c2 = cx * cx + cy * cy;
  double ux = (cy * b2 - by * c2) / d;
  double uy = (bx * c2 - cx * b2) / d;
  return {a.x + ux, a.y + uy, hypot(ux, uy)};
}
Circulo circulo_minimo(vector<PtD> p) {
  shuffle(p.begin(), p.end(), mt19937(712));
  int n = p.size();
  Circulo c = {p[0].x, p[0].y, 0};
  for (int i = 1; i < n; i++) {
    if (contiene(c, p[i])) continue;
    c = {p[i].x, p[i].y, 0};
    for (int j = 0; j < i; j++) {
      if (contiene(c, p[j])) continue;
      c = por_dos(p[i], p[j]);
      for (int k = 0; k < j; k++)
        if (!contiene(c, p[k])) c = por_tres(p[i], p[j], p[k]);
    }
  }
  return c;
}
