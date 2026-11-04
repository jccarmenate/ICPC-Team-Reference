// Compara la geometria del notebook contra KACTL.
#include "kactl_begin.h"
namespace kg {
#include "geometry/SegmentIntersection.h"  // incluye Point.h y OnSegment.h
#include "geometry/ConvexHull.h"           // typedef Point<ll> P
#include "geometry/HullDiameter.h"
#include "geometry/PolygonArea.h"
#include "geometry/PointInsideHull.h"
#include "geometry/ClosestPair.h"
}
#include "kactl_end.h"
#include "../../codigo/geometria/punto.cpp"
#include "../../codigo/geometria/segmentos.cpp"
#include "../../codigo/geometria/convex_hull.cpp"
#include "../../codigo/geometria/antipodal.cpp"
#include "../../codigo/geometria/punto_en_poligono.cpp"
#include "../../codigo/geometria/par_mas_cercano.cpp"

kg::P to_k(Pt p) { return kg::P(p.x, p.y); }
Pt random_point(long long c) { return {rnd(-c, c), rnd(-c, c)}; }

void t_segments() {
  for (int it = 0; it < 300000; it++) {
    long long c = it % 2 ? 3 : 1000;
    Pt a = random_point(c), b = random_point(c), p = random_point(c), q = random_point(c);
    CHECK(on_segment(a, b, p) == kg::onSegment(to_k(a), to_k(b), to_k(p)));
    CHECK(segments_intersect(a, b, p, q) == !kg::segInter(to_k(a), to_k(b), to_k(p), to_k(q)).empty());
  }
}

void t_hulls() {
  for (int it = 0; it < 20000; it++) {
    int n = rnd(1, 40);
    long long c = vector<long long>{2, 10, 1000, 1000000000}[rnd(0, 3)];
    vector<Pt> p(n);
    for (auto& x : p) x = random_point(c);
    vector<kg::P> kp;
    for (auto& x : p) kp.push_back(to_k(x));
    vector<Pt> h = convex_hull(p);
    vector<kg::P> kh = kg::convexHull(kp);
    CHECK(h.size() == kh.size());
    set<pair<long long, long long>> s1, s2;
    for (auto& x : h) s1.insert({x.x, x.y});
    for (auto& x : kh) s2.insert({x.x, x.y});
    CHECK(s1 == s2);
    if (!kh.empty()) {
      auto d = kg::hullDiameter(kh);
      CHECK(diameter2(h) == (d[0] - d[1]).dist2());
    }
    CHECK(area2(p) == kg::polygonArea2(kp));
    if (h.size() >= 3)
      for (int q = 0; q < 20; q++) {
        Pt x = rnd(0, 1) ? random_point(c + 1) : p[rnd(0, n - 1)];
        int r = in_convex(h, x);
        CHECK((r >= 1) == kg::inHull(kh, to_k(x), false));
        CHECK((r == 2) == kg::inHull(kh, to_k(x), true));
      }
  }
  int N = 200000;
  vector<Pt> p(N);
  for (auto& x : p) x = random_point(1000000000);
  vector<kg::P> kp;
  for (auto& x : p) kp.push_back(to_k(x));
  vector<Pt> h;
  vector<kg::P> kh;
  double t1 = measure([&] { h = convex_hull(p); });
  double t2 = measure([&] { kh = kg::convexHull(kp); });
  CHECK(h.size() == kh.size());
  report("Convex Hull, 2e5 puntos", t1, t2);
  long long d1 = 0, d2 = 0;
  double t3 = measure([&] { d1 = closest_pair(p); });
  double t4 = measure([&] { auto q = kg::closest(kp); d2 = (q.first - q.second).dist2(); });
  CHECK(d1 == d2);
  report("Par mas cercano, 2e5 puntos", t3, t4);
}

void t_closest_small() {
  for (int it = 0; it < 20000; it++) {
    int n = rnd(2, 60);
    long long c = vector<long long>{3, 100, 1000000000}[rnd(0, 2)];
    vector<Pt> p(n);
    for (auto& x : p) x = random_point(c);
    vector<kg::P> kp;
    for (auto& x : p) kp.push_back(to_k(x));
    auto q = kg::closest(kp);
    CHECK(closest_pair(p) == (q.first - q.second).dist2());
  }
}

int main() {
  t_segments();
  t_hulls();
  t_closest_small();
  printf("OK geometria (igual que KACTL)\n");
  print_timings();
}
