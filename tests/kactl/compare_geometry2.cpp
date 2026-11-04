// Punto en poligono y circulo minimo contra KACTL. Va aparte porque
// estos archivos de KACTL usan Point<double> como P.
#include "kactl_begin.h"
namespace kg {
#include "geometry/InsidePolygon.h"  // incluye SegmentDistance.h
#include "geometry/MinimumEnclosingCircle.h"
}
#include "kactl_end.h"
#include "../../codigo/geometria/punto.cpp"
#include "../../codigo/geometria/segmentos.cpp"
#include "../../codigo/geometria/punto_en_poligono.cpp"
#include "../../codigo/geometria/circulo_minimo.cpp"

typedef kg::Point<long long> KP;
KP to_k(Pt p) { return KP(p.x, p.y); }
Pt random_point(long long c) { return {rnd(-c, c), rnd(-c, c)}; }

bool is_simple(const vector<Pt>& poly) {
  int n = poly.size();
  for (int i = 0; i < n; i++) {
    Pt a = poly[i], b = poly[(i + 1) % n], c = poly[(i + 2) % n];
    if (a == b || on_segment(a, b, c) || on_segment(b, c, a)) return false;
    for (int j = i + 2; j < n; j++) {
      if (i == 0 && j == n - 1) continue;
      if (segments_intersect(a, b, poly[j], poly[(j + 1) % n])) return false;
    }
  }
  return true;
}

void check_polygon(const vector<Pt>& poly, Pt p) {
  vector<KP> kp;
  for (auto& x : poly) kp.push_back(to_k(x));
  int r = in_polygon(poly, p);
  CHECK((r >= 1) == kg::inPolygon(kp, to_k(p), false));
  CHECK((r == 2) == kg::inPolygon(kp, to_k(p), true));
}

void t_in_polygon() {
  int tested = 0;
  while (tested < 20000) {
    int n = rnd(3, 8);
    long long c = rnd(2, 6);
    vector<Pt> poly(n);
    for (auto& x : poly) x = random_point(c);
    if (!is_simple(poly)) continue;
    tested++;
    for (long long x = -c - 1; x <= c + 1; x++)
      for (long long y = -c - 1; y <= c + 1; y++) check_polygon(poly, {x, y});
  }
  // poligonos estrellados grandes (no convexos)
  for (int it = 0; it < 200; it++) {
    int n = 1000;
    vector<pair<double, Pt>> pts;
    for (int i = 0; i < n; i++) {
      Pt q = random_point(1000000);
      pts.push_back({atan2((double)q.y, (double)q.x), q});
    }
    sort(pts.begin(), pts.end(), [](auto& a, auto& b) { return a.first < b.first; });
    vector<Pt> poly;
    for (auto& [ang, q] : pts) poly.push_back(q);
    for (int q = 0; q < 50; q++) check_polygon(poly, random_point(1000000));
  }
}

void t_circle() {
  for (int it = 0; it < 20000; it++) {
    int n = rnd(1, 50);
    long long c = it % 2 ? 5 : 1000000;
    vector<PtD> p(n);
    vector<kg::P> kp;
    for (auto& x : p) { x = {(double)rnd(-c, c), (double)rnd(-c, c)}; kp.push_back(kg::P(x.x, x.y)); }
    Circle ours = min_enclosing_circle(p);
    auto k = kg::mec(kp);
    CHECK(fabs(ours.r - k.second) <= 1e-6 * max(1.0, k.second));
  }
  int N = 1000000;
  vector<PtD> p(N);
  vector<kg::P> kp;
  for (auto& x : p) { x = {(double)rnd(-1000000000, 1000000000), (double)rnd(-1000000000, 1000000000)}; kp.push_back(kg::P(x.x, x.y)); }
  Circle ours;
  pair<kg::P, double> k;
  double t1 = measure([&] { ours = min_enclosing_circle(p); });
  double t2 = measure([&] { k = kg::mec(kp); });
  CHECK(fabs(ours.r - k.second) <= 1e-6 * k.second);
  report("Circulo minimo, 1e6 puntos", t1, t2);
}

int main() {
  t_in_polygon();
  t_circle();
  printf("OK geometria 2 (igual que KACTL)\n");
  print_timings();
}
