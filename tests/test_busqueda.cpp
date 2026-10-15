#include "comun.h"
#include "../codigo/varios/busqueda_binaria.cpp"

void t_cotas() {
  for (int it = 0; it < 20000; it++) {
    int n = rnd(0, 30);
    vector<long long> a(n);
    for (auto& x : a) x = rnd(-10, 10);
    sort(a.begin(), a.end());
    for (long long x = -12; x <= 12; x++) {
      int lb = lower_bound_manual(a, x), ub = upper_bound_manual(a, x);
      CHECK(lb == lower_bound(a.begin(), a.end(), x) - a.begin());
      CHECK(ub == upper_bound(a.begin(), a.end(), x) - a.begin());
      // las identidades del comentario
      int menores = 0, menores_ig = 0, iguales = 0;
      for (long long v : a) {
        if (v < x) menores++;
        if (v <= x) menores_ig++;
        if (v == x) iguales++;
      }
      CHECK(lb == menores && ub == menores_ig && ub - lb == iguales);
      CHECK((lb < n && a[lb] == x) == (iguales > 0));
    }
  }
  // ejemplo del comentario
  vector<long long> a = {1, 3, 3, 3, 7};
  CHECK(lower_bound_manual(a, 3) == 1 && upper_bound_manual(a, 3) == 4);
  CHECK(lower_bound_manual(a, 4) == 4 && upper_bound_manual(a, 4) == 4);
  CHECK(lower_bound_manual(a, 0) == 0 && upper_bound_manual(a, 9) == 5);
}

void t_sobre_respuesta() {
  for (int it = 0; it < 20000; it++) {
    long long lo = rnd(-50, 50), hi = rnd(lo, lo + 60);
    long long corte = rnd(lo - 3, hi + 3);  // ok(m) = m >= corte
    auto ok = [&](long long m) { return m >= corte; };
    long long esperado = hi + 1;
    for (long long m = lo; m <= hi; m++)
      if (ok(m)) { esperado = m; break; }
    CHECK(primer_verdadero(lo, hi, ok) == esperado);
    // mayor m con ok2(m), patron verdadero...falso
    auto ok2 = [&](long long m) { return m <= corte; };
    long long ultimo = lo - 1;
    for (long long m = lo; m <= hi; m++)
      if (ok2(m)) ultimo = m;
    auto neg = [&](long long m) { return !ok2(m); };
    CHECK(primer_verdadero(lo, hi, neg) - 1 == ultimo);
  }
  // rango grande: raiz cuadrada entera de numeros hasta 1e18
  for (int it = 0; it < 20000; it++) {
    long long n = rnd(0, 1000000000000000000LL);
    long long r = primer_verdadero(0, 1000000000, [&](long long m) {
      return m * m > n;
    }) - 1;
    CHECK(r * r <= n && (r + 1) * (r + 1) > n);
  }
}

void t_biseccion() {
  for (int it = 0; it < 2000; it++) {
    double v = rnd(0, 1000000) / 7.0;
    double r = biseccion(0, 2000, [&](double x) { return x * x >= v; });
    CHECK(fabs(r - sqrt(v)) < 1e-9);
  }
}

int main() {
  t_cotas();
  t_sobre_respuesta();
  t_biseccion();
  printf("OK busqueda\n");
}
