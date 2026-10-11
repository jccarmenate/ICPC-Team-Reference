// Punto entero (exacto). Con coordenadas hasta 1e9 los productos
// caben en long long. PtD: version real para circulos.
struct Pt {
  long long x, y;
  Pt operator+(Pt o) const { return {x + o.x, y + o.y}; }
  Pt operator-(Pt o) const { return {x - o.x, y - o.y}; }
  long long cross(Pt o) const { return x * o.y - y * o.x; }
  long long dot(Pt o) const { return x * o.x + y * o.y; }
  long long norm2() const { return x * x + y * y; }
  bool operator<(Pt o) const {
    return x != o.x ? x < o.x : y < o.y;
  }
  bool operator==(Pt o) const { return x == o.x && y == o.y; }
};
struct PtD { double x, y; };

// > 0: c a la izquierda de a->b.  < 0: derecha.  0: colineales.
long long orient(Pt a, Pt b, Pt c) { return (b - a).cross(c - a); }
int signo(long long v) { return (v > 0) - (v < 0); }
// p esta sobre el segmento cerrado ab?
bool en_segmento(Pt a, Pt b, Pt p) {
  return orient(a, b, p) == 0 && (a - p).dot(b - p) <= 0;
}
// Doble del area con signo (positiva si es antihorario).
long long area2(const vector<Pt>& pol) {
  long long s = 0;
  int n = pol.size();
  for (int i = 0; i < n; i++) s += pol[i].cross(pol[(i + 1) % n]);
  return s;
}
