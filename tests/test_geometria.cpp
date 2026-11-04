#include "comun.h"
#include "../codigo/geometria/punto.cpp"
#include "../codigo/geometria/segmentos.cpp"
#include "../codigo/geometria/convex_hull.cpp"
#include "../codigo/geometria/antipodal.cpp"
#include "../codigo/geometria/circulo_minimo.cpp"
#include "../codigo/geometria/punto_en_poligono.cpp"
#include "../codigo/geometria/par_mas_cercano.cpp"

Pt random_point(long long c) { return {rnd(-c, c), rnd(-c, c)}; }

// Implementacion independiente: p sobre el segmento ab.
bool on_segment_brute(Pt a, Pt b, Pt p) {
  if ((b.x - a.x) * (p.y - a.y) != (b.y - a.y) * (p.x - a.x)) return false;
  return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
         min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
// Implementacion independiente con parametros t, u racionales.
bool intersect_brute(Pt a, Pt b, Pt c, Pt d) {
  long long rx = b.x - a.x, ry = b.y - a.y, sx = d.x - c.x, sy = d.y - c.y;
  long long den = rx * sy - ry * sx;
  if (den != 0) {
    long long tn = (c.x - a.x) * sy - (c.y - a.y) * sx;
    long long un = (c.x - a.x) * ry - (c.y - a.y) * rx;
    if (den < 0) { den = -den; tn = -tn; un = -un; }
    return 0 <= tn && tn <= den && 0 <= un && un <= den;
  }
  // paralelos o degenerados: si se tocan, es en algun extremo
  return on_segment_brute(a, b, c) || on_segment_brute(a, b, d) ||
         on_segment_brute(c, d, a) || on_segment_brute(c, d, b);
}

void t_segments() {
  for (int it = 0; it < 300000; it++) {
    long long c = it % 2 ? 3 : 8;
    Pt a = random_point(c), b = random_point(c), p = random_point(c), q = random_point(c);
    CHECK(segments_intersect(a, b, p, q) == intersect_brute(a, b, p, q));
    CHECK(on_segment(a, b, p) == on_segment_brute(a, b, p));
    PtD r;
    bool found = line_intersection(a, b, p, q, r);
    CHECK(found == ((b - a).cross(q - p) != 0));
    if (found) {  // el punto queda sobre ambas rectas
      double e1 = (b.x - a.x) * (r.y - a.y) - (b.y - a.y) * (r.x - a.x);
      double e2 = (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);
      CHECK(fabs(e1) < 1e-7 && fabs(e2) < 1e-7);
    }
  }
}

void t_hull_and_diameter() {
  for (int it = 0; it < 30000; it++) {
    int n = rnd(1, 40);
    long long c = vector<long long>{2, 5, 30, 1000, 1000000000}[rnd(0, 4)];
    vector<Pt> p(n);
    for (auto& x : p) x = random_point(c);
    if (it % 7 == 0) for (auto& x : p) x.y = 2 * x.x + 1;  // colineales
    vector<Pt> h = convex_hull(p);
    int k = h.size();
    vector<Pt> uniques = p;
    sort(uniques.begin(), uniques.end());
    uniques.erase(unique(uniques.begin(), uniques.end()), uniques.end());
    bool collinear = true;
    for (Pt x : uniques)
      if (orient(uniques[0], uniques.back(), x) != 0) collinear = false;
    for (Pt x : h) CHECK(binary_search(uniques.begin(), uniques.end(), x));
    if (collinear) {
      CHECK(k == min((int)uniques.size(), 2));
      CHECK(h[0] == uniques[0] && h.back() == uniques.back());
    } else {
      CHECK(k >= 3 && area2(h) > 0);
      for (int i = 0; i < k; i++) {
        CHECK(orient(h[i], h[(i + 1) % k], h[(i + 2) % k]) > 0);  // estricto
        for (Pt x : p) CHECK(orient(h[i], h[(i + 1) % k], x) >= 0);
      }
    }
    // diametro por rotating calipers contra fuerza bruta
    if (c <= 1000000) {
      long long best = 0;
      for (Pt x : p) for (Pt y : p) best = max(best, (x - y).norm2());
      CHECK(diameter2(h) == best);
    }
    // punto en convexo contra el metodo general y contra fuerza bruta
    if (!collinear)
      for (int q = 0; q < 30; q++) {
        Pt x = rnd(0, 2) ? random_point(c + 1) : p[rnd(0, n - 1)];
        if (c <= 1000 && rnd(0, 3) == 0) {  // punto sobre un lado o diagonal
          Pt u = h[rnd(0, k - 1)], v = h[rnd(0, k - 1)];
          if ((u.x + v.x) % 2 == 0 && (u.y + v.y) % 2 == 0)
            x = {(u.x + v.x) / 2, (u.y + v.y) / 2};
        }
        bool boundary = false, outside = false;
        for (int i = 0; i < k; i++) {
          if (on_segment_brute(h[i], h[(i + 1) % k], x)) boundary = true;
          if (orient(h[i], h[(i + 1) % k], x) < 0) outside = true;
        }
        int expected = boundary ? 1 : outside ? 0 : 2;
        CHECK(in_convex(h, x) == expected);
        CHECK(in_polygon(h, x) == expected);
        vector<Pt> clockwise(h.rbegin(), h.rend());
        CHECK(in_polygon(clockwise, x) == expected);
      }
  }
}

// Poligono simple aleatorio (posiblemente no convexo), por rechazo.
bool is_simple(const vector<Pt>& poly) {
  int n = poly.size();
  for (int i = 0; i < n; i++) {
    Pt a = poly[i], b = poly[(i + 1) % n], c = poly[(i + 2) % n];
    if (a == b) return false;
    if (on_segment(a, b, c) || on_segment(b, c, a)) return false;
    for (int j = i + 2; j < n; j++) {
      if (i == 0 && j == n - 1) continue;  // lados vecinos
      if (intersect_brute(a, b, poly[j], poly[(j + 1) % n])) return false;
    }
  }
  return true;
}

void t_general_polygon() {
  const double PI = acos(-1.0);
  int tested = 0;
  for (int it = 0; tested < 20000; it++) {
    int n = rnd(3, 7);
    long long c = rnd(2, 6);
    vector<Pt> poly(n);
    for (auto& x : poly) x = random_point(c);
    if (!is_simple(poly)) continue;
    tested++;
    for (long long x = -c - 1; x <= c + 1; x++)
      for (long long y = -c - 1; y <= c + 1; y++) {
        Pt p = {x, y};
        bool boundary = false;
        double winding = 0;  // numero de vueltas por suma de angulos
        for (int i = 0; i < n; i++) {
          Pt a = poly[i], b = poly[(i + 1) % n];
          if (on_segment_brute(a, b, p)) boundary = true;
          winding += atan2((double)(a - p).cross(b - p), (double)(a - p).dot(b - p));
        }
        int expected = boundary ? 1 : fabs(winding) > PI ? 2 : 0;
        CHECK(in_polygon(poly, p) == expected);
      }
  }
}

void t_circle() {
  for (int it = 0; it < 20000; it++) {
    int n = rnd(1, 9);
    long long c = it % 3 == 0 ? 3 : 100;
    vector<PtD> p(n);
    for (auto& x : p) x = {(double)rnd(-c, c), (double)rnd(-c, c)};
    auto covers = [&](const Circle& ci) {
      for (PtD x : p)
        if (hypot(x.x - ci.x, x.y - ci.y) > ci.r + 1e-7) return false;
      return true;
    };
    double best = 1e18;
    if (n == 1) best = 0;
    for (int i = 0; i < n; i++)
      for (int j = i + 1; j < n; j++) {
        Circle ci = from_two(p[i], p[j]);
        if (covers(ci)) best = min(best, ci.r);
        for (int k = j + 1; k < n; k++) {
          double area = (p[j].x - p[i].x) * (p[k].y - p[i].y) -
                        (p[j].y - p[i].y) * (p[k].x - p[i].x);
          if (fabs(area) < 0.5) continue;  // colineales
          Circle ct = from_three(p[i], p[j], p[k]);
          if (covers(ct)) best = min(best, ct.r);
        }
      }
    Circle res = min_enclosing_circle(p);
    CHECK(covers(res));
    CHECK(fabs(res.r - best) < 1e-6);
  }
}

void t_closest_pair() {
  for (int it = 0; it < 20000; it++) {
    int n = it % 50 == 0 ? rnd(2, 400) : rnd(2, 40);
    long long c = vector<long long>{3, 20, 1000, 1000000000}[rnd(0, 3)];
    vector<Pt> p(n);
    for (auto& x : p) x = random_point(c);
    if (it % 9 == 0) for (auto& x : p) x.x = 5;        // misma vertical
    if (it % 11 == 0) for (auto& x : p) x.y = -7;      // misma horizontal
    long long best = LLONG_MAX;
    for (int i = 0; i < n; i++)
      for (int j = i + 1; j < n; j++) best = min(best, (p[i] - p[j]).norm2());
    CHECK(closest_pair(p) == best);
  }
}

int main() {
  t_segments();
  t_hull_and_diameter();
  t_general_polygon();
  t_circle();
  t_closest_pair();
  printf("OK geometry\n");
}
