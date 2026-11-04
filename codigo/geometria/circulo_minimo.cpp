// Circulo minimo que encierra un conjunto de puntos (Welzl
// incremental aleatorizado). O(n) esperado. Requiere PtD.
struct Circle { double x, y, r; };

bool contains(const Circle& c, PtD p) {
  return hypot(p.x - c.x, p.y - c.y) <= c.r + 1e-9;
}
Circle from_two(PtD a, PtD b) {
  return {(a.x + b.x) / 2, (a.y + b.y) / 2,
          hypot(a.x - b.x, a.y - b.y) / 2};
}
Circle from_three(PtD a, PtD b, PtD c) {  // circunscrito
  double bx = b.x - a.x, by = b.y - a.y;
  double cx = c.x - a.x, cy = c.y - a.y;
  double d = 2 * (bx * cy - by * cx);
  double b2 = bx * bx + by * by, c2 = cx * cx + cy * cy;
  double ux = (cy * b2 - by * c2) / d;
  double uy = (bx * c2 - cx * b2) / d;
  return {a.x + ux, a.y + uy, hypot(ux, uy)};
}
Circle min_enclosing_circle(vector<PtD> p) {
  shuffle(p.begin(), p.end(), mt19937(712));
  int n = p.size();
  Circle c = {p[0].x, p[0].y, 0};
  for (int i = 1; i < n; i++) {
    if (contains(c, p[i])) continue;
    c = {p[i].x, p[i].y, 0};
    for (int j = 0; j < i; j++) {
      if (contains(c, p[j])) continue;
      c = from_two(p[i], p[j]);
      for (int k = 0; k < j; k++)
        if (!contains(c, p[k])) c = from_three(p[i], p[j], p[k]);
    }
  }
  return c;
}
