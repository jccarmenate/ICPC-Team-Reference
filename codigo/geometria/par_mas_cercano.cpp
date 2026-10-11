// Par de puntos mas cercano, divide y venceras. O(n log n).
// Requiere Pt. Retorna el CUADRADO de la distancia minima
// (n >= 2). Coordenadas hasta 1e9.
long long cercano_rec(vector<Pt>& p, vector<Pt>& tmp, int l,
                      int r) {  // rango [l, r)
  auto por_y = [](Pt a, Pt b) { return a.y < b.y; };
  if (r - l <= 3) {
    long long d = LLONG_MAX;
    for (int i = l; i < r; i++)
      for (int j = i + 1; j < r; j++)
        d = min(d, (p[i] - p[j]).norm2());
    sort(p.begin() + l, p.begin() + r, por_y);
    return d;
  }
  int mid = (l + r) / 2;
  long long xm = p[mid].x;
  long long d = min(cercano_rec(p, tmp, l, mid),
                    cercano_rec(p, tmp, mid, r));
  merge(p.begin() + l, p.begin() + mid, p.begin() + mid,
        p.begin() + r, tmp.begin(), por_y);
  copy(tmp.begin(), tmp.begin() + (r - l), p.begin() + l);
  int k = 0;  // tmp[0..k) = franja central, ordenada por y
  for (int i = l; i < r; i++) {
    long long dx = p[i].x - xm;
    if (dx * dx >= d) continue;
    for (int j = k - 1; j >= 0; j--) {
      long long dy = p[i].y - tmp[j].y;
      if (dy * dy >= d) break;
      d = min(d, (p[i] - tmp[j]).norm2());
    }
    tmp[k++] = p[i];
  }
  return d;
}
long long par_mas_cercano(vector<Pt> p) {
  sort(p.begin(), p.end());
  vector<Pt> tmp(p.size());
  return cercano_rec(p, tmp, 0, p.size());
}
