#include "comun.h"
#include "../codigo/geometria/punto.cpp"
#include "../codigo/geometria/segmentos.cpp"
#include "../codigo/geometria/convex_hull.cpp"
#include "../codigo/geometria/antipodal.cpp"
#include "../codigo/geometria/circulo_minimo.cpp"
#include "../codigo/geometria/punto_en_poligono.cpp"
#include "../codigo/geometria/par_mas_cercano.cpp"

Pt pt_aleatorio(long long c) { return {rnd(-c, c), rnd(-c, c)}; }

// Implementacion independiente: p sobre el segmento ab.
bool sobre_bruto(Pt a, Pt b, Pt p) {
  if ((b.x - a.x) * (p.y - a.y) != (b.y - a.y) * (p.x - a.x)) return false;
  return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
         min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
// Implementacion independiente con parametros t, u racionales.
bool cruce_bruto(Pt a, Pt b, Pt c, Pt d) {
  long long rx = b.x - a.x, ry = b.y - a.y, sx = d.x - c.x, sy = d.y - c.y;
  long long den = rx * sy - ry * sx;
  if (den != 0) {
    long long tn = (c.x - a.x) * sy - (c.y - a.y) * sx;
    long long un = (c.x - a.x) * ry - (c.y - a.y) * rx;
    if (den < 0) { den = -den; tn = -tn; un = -un; }
    return 0 <= tn && tn <= den && 0 <= un && un <= den;
  }
  // paralelos o degenerados: si se tocan, es en algun extremo
  return sobre_bruto(a, b, c) || sobre_bruto(a, b, d) ||
         sobre_bruto(c, d, a) || sobre_bruto(c, d, b);
}

void t_segmentos() {
  for (int it = 0; it < 300000; it++) {
    long long c = it % 2 ? 3 : 8;
    Pt a = pt_aleatorio(c), b = pt_aleatorio(c), p = pt_aleatorio(c), q = pt_aleatorio(c);
    CHECK(se_intersecan(a, b, p, q) == cruce_bruto(a, b, p, q));
    CHECK(en_segmento(a, b, p) == sobre_bruto(a, b, p));
    PtD r;
    bool hay = interseccion_rectas(a, b, p, q, r);
    CHECK(hay == ((b - a).cross(q - p) != 0));
    if (hay) {  // el punto queda sobre ambas rectas
      double e1 = (b.x - a.x) * (r.y - a.y) - (b.y - a.y) * (r.x - a.x);
      double e2 = (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);
      CHECK(fabs(e1) < 1e-7 && fabs(e2) < 1e-7);
    }
  }
}

void t_hull_y_diametro() {
  for (int it = 0; it < 30000; it++) {
    int n = rnd(1, 40);
    long long c = vector<long long>{2, 5, 30, 1000, 1000000000}[rnd(0, 4)];
    vector<Pt> p(n);
    for (auto& x : p) x = pt_aleatorio(c);
    if (it % 7 == 0) for (auto& x : p) x.y = 2 * x.x + 1;  // colineales
    vector<Pt> h = convex_hull(p);
    int k = h.size();
    vector<Pt> unicos = p;
    sort(unicos.begin(), unicos.end());
    unicos.erase(unique(unicos.begin(), unicos.end()), unicos.end());
    bool colineales = true;
    for (Pt x : unicos)
      if (orient(unicos[0], unicos.back(), x) != 0) colineales = false;
    for (Pt x : h) CHECK(binary_search(unicos.begin(), unicos.end(), x));
    if (colineales) {
      CHECK(k == min((int)unicos.size(), 2));
      CHECK(h[0] == unicos[0] && h.back() == unicos.back());
    } else {
      CHECK(k >= 3 && area2(h) > 0);
      for (int i = 0; i < k; i++) {
        CHECK(orient(h[i], h[(i + 1) % k], h[(i + 2) % k]) > 0);  // estricto
        for (Pt x : p) CHECK(orient(h[i], h[(i + 1) % k], x) >= 0);
      }
    }
    // diametro por rotating calipers contra fuerza bruta
    if (c <= 1000000) {
      long long mejor = 0;
      for (Pt x : p) for (Pt y : p) mejor = max(mejor, (x - y).norm2());
      CHECK(diametro2(h) == mejor);
    }
    // punto en convexo contra el metodo general y contra fuerza bruta
    if (!colineales)
      for (int q = 0; q < 30; q++) {
        Pt x = rnd(0, 2) ? pt_aleatorio(c + 1) : p[rnd(0, n - 1)];
        if (c <= 1000 && rnd(0, 3) == 0) {  // punto sobre un lado o diagonal
          Pt u = h[rnd(0, k - 1)], v = h[rnd(0, k - 1)];
          if ((u.x + v.x) % 2 == 0 && (u.y + v.y) % 2 == 0)
            x = {(u.x + v.x) / 2, (u.y + v.y) / 2};
        }
        bool borde = false, fuera = false;
        for (int i = 0; i < k; i++) {
          if (sobre_bruto(h[i], h[(i + 1) % k], x)) borde = true;
          if (orient(h[i], h[(i + 1) % k], x) < 0) fuera = true;
        }
        int esperado = borde ? 1 : fuera ? 0 : 2;
        CHECK(en_convexo(h, x) == esperado);
        CHECK(en_poligono(h, x) == esperado);
        vector<Pt> horario(h.rbegin(), h.rend());
        CHECK(en_poligono(horario, x) == esperado);
      }
  }
}

// Poligono simple aleatorio (posiblemente no convexo), por rechazo.
bool es_simple(const vector<Pt>& pol) {
  int n = pol.size();
  for (int i = 0; i < n; i++) {
    Pt a = pol[i], b = pol[(i + 1) % n], c = pol[(i + 2) % n];
    if (a == b) return false;
    if (en_segmento(a, b, c) || en_segmento(b, c, a)) return false;
    for (int j = i + 2; j < n; j++) {
      if (i == 0 && j == n - 1) continue;  // lados vecinos
      if (cruce_bruto(a, b, pol[j], pol[(j + 1) % n])) return false;
    }
  }
  return true;
}

void t_poligono_general() {
  const double PI = acos(-1.0);
  int probados = 0;
  for (int it = 0; probados < 20000; it++) {
    int n = rnd(3, 7);
    long long c = rnd(2, 6);
    vector<Pt> pol(n);
    for (auto& x : pol) x = pt_aleatorio(c);
    if (!es_simple(pol)) continue;
    probados++;
    for (long long x = -c - 1; x <= c + 1; x++)
      for (long long y = -c - 1; y <= c + 1; y++) {
        Pt p = {x, y};
        bool borde = false;
        double giro = 0;  // numero de vueltas por suma de angulos
        for (int i = 0; i < n; i++) {
          Pt a = pol[i], b = pol[(i + 1) % n];
          if (sobre_bruto(a, b, p)) borde = true;
          giro += atan2((double)(a - p).cross(b - p), (double)(a - p).dot(b - p));
        }
        int esperado = borde ? 1 : fabs(giro) > PI ? 2 : 0;
        CHECK(en_poligono(pol, p) == esperado);
      }
  }
}

void t_circulo() {
  for (int it = 0; it < 20000; it++) {
    int n = rnd(1, 9);
    long long c = it % 3 == 0 ? 3 : 100;
    vector<PtD> p(n);
    for (auto& x : p) x = {(double)rnd(-c, c), (double)rnd(-c, c)};
    auto cubre = [&](const Circulo& ci) {
      for (PtD x : p)
        if (hypot(x.x - ci.x, x.y - ci.y) > ci.r + 1e-7) return false;
      return true;
    };
    double mejor = 1e18;
    if (n == 1) mejor = 0;
    for (int i = 0; i < n; i++)
      for (int j = i + 1; j < n; j++) {
        Circulo ci = por_dos(p[i], p[j]);
        if (cubre(ci)) mejor = min(mejor, ci.r);
        for (int k = j + 1; k < n; k++) {
          double area = (p[j].x - p[i].x) * (p[k].y - p[i].y) -
                        (p[j].y - p[i].y) * (p[k].x - p[i].x);
          if (fabs(area) < 0.5) continue;  // colineales
          Circulo ct = por_tres(p[i], p[j], p[k]);
          if (cubre(ct)) mejor = min(mejor, ct.r);
        }
      }
    Circulo res = circulo_minimo(p);
    CHECK(cubre(res));
    CHECK(fabs(res.r - mejor) < 1e-6);
  }
}

void t_par_mas_cercano() {
  for (int it = 0; it < 20000; it++) {
    int n = it % 50 == 0 ? rnd(2, 400) : rnd(2, 40);
    long long c = vector<long long>{3, 20, 1000, 1000000000}[rnd(0, 3)];
    vector<Pt> p(n);
    for (auto& x : p) x = pt_aleatorio(c);
    if (it % 9 == 0) for (auto& x : p) x.x = 5;        // misma vertical
    if (it % 11 == 0) for (auto& x : p) x.y = -7;      // misma horizontal
    long long mejor = LLONG_MAX;
    for (int i = 0; i < n; i++)
      for (int j = i + 1; j < n; j++) mejor = min(mejor, (p[i] - p[j]).norm2());
    CHECK(par_mas_cercano(p) == mejor);
  }
}

int main() {
  t_segmentos();
  t_hull_y_diametro();
  t_poligono_general();
  t_circulo();
  t_par_mas_cercano();
  printf("OK geometria\n");
}
