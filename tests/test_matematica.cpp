#include "comun.h"
#include "../codigo/matematica/aritmetica.cpp"
#include "../codigo/matematica/miller_rabin.cpp"
#include "../codigo/matematica/pollard_rho.cpp"
#include "../codigo/matematica/euclides.cpp"
#include "../codigo/matematica/crt.cpp"
#include "../codigo/matematica/bsgs.cpp"
#include "../codigo/matematica/criba.cpp"
#include "../codigo/matematica/gauss.cpp"
#include "../codigo/matematica/fft.cpp"
#include "../codigo/matematica/ntt.cpp"
#include "../codigo/matematica/matriz.cpp"
#include "../codigo/matematica/combinatoria.cpp"

bool primo_bruto(long long n) {
  if (n < 2) return false;
  for (long long d = 2; d * d <= n; d++)
    if (n % d == 0) return false;
  return true;
}
long long primo_aleatorio(long long lo, long long hi) {
  while (true) {
    long long p = rnd(lo, hi);
    if (primo_bruto(p)) return p;
  }
}
vector<long long> factores_brutos(long long n) {
  vector<long long> f;
  for (long long d = 2; d * d <= n; d++)
    while (n % d == 0) {
      f.push_back(d);
      n /= d;
    }
  if (n > 1) f.push_back(n);
  return f;
}

void t_aritmetica() {
  for (int it = 0; it < 20000; it++) {
    long long m = rnd(1, 1000), b = rnd(0, 1000), e = rnd(0, 30), r = 1 % m;
    for (int i = 0; i < e; i++) r = r * (b % m) % m;
    CHECK(powmod(b, e, m) == r);
  }
  const long long P = (1LL << 61) - 1;  // primo de Mersenne
  for (int it = 0; it < 2000; it++) {
    long long a = rnd(1, P - 1);
    CHECK(powmod(a, P - 1, P) == 1);  // Fermat
    CHECK(mulmod(a, powmod(a, P - 2, P), P) == 1);
  }
}

void t_primos() {
  Criba cr(300000);
  for (int i = 0; i <= 300000; i++) CHECK(es_primo(i) == cr.es_primo(i));
  for (int i = 2; i <= 3000; i++) {
    CHECK(cr.es_primo(i) == primo_bruto(i));
    vector<long long> f;
    for (auto [p, e] : cr.factorizar(i))
      for (int j = 0; j < e; j++) f.push_back(p);
    CHECK(f == factores_brutos(i));
  }
  CHECK(es_primo((1LL << 61) - 1));
  CHECK(es_primo(1000000007) && es_primo(998244353));
  CHECK(!es_primo(3215031751LL));  // pseudoprimo fuerte en bases 2,3,5,7
  CHECK(3825123056546413051LL % 149491 == 0);  // idem en bases 2..23
  CHECK(!es_primo(3825123056546413051LL));
  for (long long c : {561LL, 1105LL, 1729LL, 2465LL, 6601LL, 8911LL})
    CHECK(!es_primo(c));  // numeros de Carmichael
  for (int it = 0; it < 300; it++) {
    long long p = primo_aleatorio(900000000, 2000000000);
    long long q = primo_aleatorio(900000000, 2000000000);
    CHECK(es_primo(p) && es_primo(q) && !es_primo(p * q));
    long long x = rnd(1000000000000LL, 2000000000000LL);
    CHECK(es_primo(x) == primo_bruto(x));
  }
}

void t_pollard() {
  for (long long n = 2; n <= 30000; n++)
    CHECK(factorizar(n) == factores_brutos(n));
  auto verificar = [&](long long n) {
    vector<long long> f = factorizar(n);
    long long prod = 1;
    for (long long p : f) {
      CHECK(es_primo(p));
      prod *= p;
    }
    CHECK(prod == n && is_sorted(f.begin(), f.end()));
    return f;
  };
  for (int it = 0; it < 300; it++) {
    long long p = primo_aleatorio(900000000, 2000000000);
    long long q = primo_aleatorio(900000000, 2000000000);
    vector<long long> esperado = {min(p, q), max(p, q)};
    CHECK(verificar(p * q) == esperado);
    CHECK(verificar(p * p) == (vector<long long>{p, p}));
    long long a = primo_aleatorio(100000, 1000000);
    long long b = primo_aleatorio(100000, 1000000);
    long long c = primo_aleatorio(100000, 1000000);
    vector<long long> tres = {a, b, c};
    sort(tres.begin(), tres.end());
    CHECK(verificar(a * b * c) == tres);
    verificar(rnd(2, 1000000000000000000LL));
    long long chico = rnd(2, 1000000000000LL);
    CHECK(verificar(chico) == factores_brutos(chico));
  }
}

void t_euclides() {
  for (int it = 0; it < 20000; it++) {
    long long a = rnd(0, 1000000000), b = rnd(0, 1000000000), x, y;
    if (it % 3 == 0) { a = rnd(0, 30); b = rnd(0, 30); }
    long long g = extgcd(a, b, x, y);
    CHECK(g == __gcd(a, b) && a * x + b * y == g);
  }
  for (long long m = 1; m <= 60; m++)
    for (long long a = -5; a < m + 5; a++) {
      long long esperado = -1;
      for (long long i = 0; i < m; i++)
        if ((((a % m) + m) % m * i) % m == 1 % m) { esperado = i; break; }
      CHECK(inv_mod(a, m) == esperado);
    }
  for (int it = 0; it < 20000; it++) {
    long long a = rnd(-20, 20), b = rnd(-20, 20), c = rnd(-60, 60);
    bool existe = false;
    for (long long x = -70; x <= 70 && !existe; x++) {
      long long resto = c - a * x;
      if (b == 0) existe = resto == 0;
      else existe = resto % b == 0;
    }
    long long x0, y0, g;
    bool res = diofantica(a, b, c, x0, y0, g);
    CHECK(res == existe);
    if (res) {
      CHECK(a * x0 + b * y0 == c);
      if (g != 0) {  // la familia de soluciones tambien cumple
        long long k = rnd(-5, 5);
        CHECK(a * (x0 + k * (b / g)) + b * (y0 - k * (a / g)) == c);
      }
    }
  }
}

void t_crt() {
  for (int it = 0; it < 30000; it++) {
    long long m1 = rnd(1, 30), m2 = rnd(1, 30);
    long long a1 = rnd(0, m1 - 1), a2 = rnd(0, m2 - 1);
    long long l = m1 / __gcd(m1, m2) * m2, esperado = -1;
    for (long long x = 0; x < l; x++)
      if (x % m1 == a1 && x % m2 == a2) { esperado = x; break; }
    auto [x, mod] = crt(a1, m1, a2, m2);
    CHECK(x == esperado && mod == (esperado == -1 ? -1 : l));
  }
  for (int it = 0; it < 5000; it++) {
    long long m1 = rnd(1, 1000000000), m2 = rnd(1, 1000000000);
    long long l = m1 / __gcd(m1, m2) * m2, x = rnd(0, l - 1);
    CHECK(crt(x % m1, m1, x % m2, m2) == make_pair(x, l));
  }
  // plegado de varias congruencias
  for (int it = 0; it < 2000; it++) {
    long long x = rnd(0, 1000000000000LL);
    pair<long long, long long> acum = {0, 1};
    for (long long m : {1000003LL, 999983LL, 1009LL})
      acum = crt(acum.first, acum.second, x % m, m);
    CHECK(acum.first == x % acum.second);
  }
}

void t_bsgs() {
  for (long long m = 1; m <= 60; m++)
    for (long long a = 0; a < m; a++) {
      vector<long long> primero(m, -1);
      long long cur = 1 % m;
      for (long long x = 0; x <= 3 * m; x++) {
        if (primero[cur] == -1) primero[cur] = x;
        cur = cur * a % m;
      }
      for (long long b = 0; b < m; b++)
        CHECK(log_discreto(a, b, m) == primero[b]);
    }
  for (int it = 0; it < 200; it++) {
    long long m = it % 2 ? 1000000007 : rnd(2, 10000000000LL);
    long long a = rnd(0, m - 1), x = rnd(0, 1000000), b = powmod(a, x, m);
    long long r = log_discreto(a, b, m);
    CHECK(r != -1 && r <= x && powmod(a, r, m) == b);
  }
}

void t_gauss() {
  const long long P = 1000000007;
  for (int it = 0; it < 20000; it++) {
    int n = rnd(1, 4), m = rnd(1, 4);
    vector<vector<double>> a(n, vector<double>(m + 1));
    vector<vector<long long>> am(n, vector<long long>(m + 1));
    bool consistente = rnd(0, 1);
    vector<long long> x0(m);
    for (auto& x : x0) x = rnd(-3, 3);
    for (int i = 0; i < n; i++) {
      long long b = 0;
      for (int j = 0; j < m; j++) {
        long long v = rnd(-3, 3);
        if (rnd(0, 3) == 0) v = 0;
        a[i][j] = v;
        am[i][j] = (v + P) % P;
        b += v * x0[j];
      }
      if (!consistente) b = rnd(-5, 5);
      a[i][m] = b;
      am[i][m] = ((b % P) + P) % P;
    }
    if (rnd(0, 4) == 0 && n > 1) { a[n - 1] = a[0]; am[n - 1] = am[0]; }
    vector<double> ans;
    vector<long long> ansm;
    int r = gauss(a, ans), rm = gauss_mod(am, ansm, P);
    CHECK(r == rm);
    if (consistente) CHECK(r >= 1);
    if (r >= 1)
      for (int i = 0; i < n; i++) {
        double s = 0;
        long long sm = 0;
        for (int j = 0; j < m; j++) {
          s += a[i][j] * ans[j];
          sm = (sm + am[i][j] * ansm[j]) % P;
        }
        CHECK(fabs(s - a[i][m]) < 1e-6 && sm == am[i][m]);
      }
  }
  for (int it = 0; it < 20000; it++) {
    long long p = vector<long long>{2, 3, 5}[rnd(0, 2)];
    int n = rnd(1, 3), m = rnd(1, 3);
    vector<vector<long long>> a(n, vector<long long>(m + 1));
    for (auto& fila : a) for (auto& x : fila) x = rnd(0, p - 1);
    int soluciones = 0, total = 1;
    for (int j = 0; j < m; j++) total *= p;
    for (int cod = 0; cod < total; cod++) {
      vector<long long> x(m);
      int c = cod;
      for (int j = 0; j < m; j++) { x[j] = c % p; c /= p; }
      bool ok = true;
      for (int i = 0; i < n && ok; i++) {
        long long s = 0;
        for (int j = 0; j < m; j++) s += a[i][j] * x[j];
        ok = s % p == a[i][m];
      }
      if (ok) soluciones++;
    }
    vector<long long> ans;
    int r = gauss_mod(a, ans, p);
    CHECK(r == min(soluciones, 2));
    if (r >= 1)
      for (int i = 0; i < n; i++) {
        long long s = 0;
        for (int j = 0; j < m; j++) s += a[i][j] * ans[j];
        CHECK(s % p == a[i][m]);
      }
  }
}

vector<long long> producto_bruto(const vector<long long>& a,
                                 const vector<long long>& b, long long mod) {
  if (a.empty() || b.empty()) return {};
  vector<long long> c(a.size() + b.size() - 1, 0);
  for (size_t i = 0; i < a.size(); i++)
    for (size_t j = 0; j < b.size(); j++) {
      if (mod) c[i + j] = (c[i + j] + a[i] * b[j]) % mod;
      else c[i + j] += a[i] * b[j];
    }
  return c;
}

void t_fft() {
  for (int it = 0; it < 600; it++) {
    int n = rnd(0, 150), m = rnd(0, 150);
    vector<long long> a(n), b(m);
    for (auto& x : a) x = rnd(-1000, 1000);
    for (auto& x : b) x = rnd(-1000, 1000);
    CHECK(multiplicar(a, b) == producto_bruto(a, b, 0));
    for (auto& x : a) x = rnd(0, MOD - 1);
    for (auto& x : b) x = rnd(0, MOD - 1);
    CHECK(multiplicar_mod(a, b) == producto_bruto(a, b, MOD));
  }
  // limite de precision de la FFT: coeficientes del resultado ~1e14
  vector<long long> a(20000), b(20000);
  for (auto& x : a) x = rnd(0, 100000);
  for (auto& x : b) x = rnd(0, 100000);
  CHECK(multiplicar(a, b) == producto_bruto(a, b, 0));
  for (auto& x : a) x = rnd(0, MOD - 1);
  for (auto& x : b) x = rnd(0, MOD - 1);
  CHECK(multiplicar_mod(a, b) == producto_bruto(a, b, MOD));
}

void t_matriz_comb() {
  const long long P = 1000000007;
  vector<long long> fib = {0, 1};
  for (int i = 2; i <= 300; i++) fib.push_back((fib[i - 1] + fib[i - 2]) % P);
  for (int e = 0; e < 300; e++) {
    auto r = mat_pot({{1, 1}, {1, 0}}, e, P);
    CHECK(r[0][1] == fib[e] && r[0][0] == fib[e + 1]);
  }
  for (int it = 0; it < 300; it++) {
    int k = rnd(1, 4);
    long long mod = rnd(1, 1000);
    vector<vector<long long>> a(k, vector<long long>(k));
    for (auto& fila : a) for (auto& x : fila) x = rnd(0, mod - 1);
    vector<vector<long long>> r(k, vector<long long>(k, 0));
    for (int i = 0; i < k; i++) r[i][i] = 1 % mod;
    for (int e = 0; e < 12; e++) {
      CHECK(mat_pot(a, e, mod) == r);
      r = mat_mult(r, a, mod);
    }
  }
  for (long long mod : {1000000007LL, 998244353LL, 101LL}) {
    int n = mod == 101 ? 100 : 300;
    Combinatoria cb(n, mod);
    vector<vector<long long>> pascal(n + 1, vector<long long>(n + 1, 0));
    for (int i = 0; i <= n; i++) {
      pascal[i][0] = 1;
      for (int j = 1; j <= i; j++)
        pascal[i][j] = (pascal[i - 1][j - 1] + pascal[i - 1][j]) % mod;
      for (int j = -1; j <= i + 1; j++)
        CHECK(cb.C(i, j) == (j < 0 || j > i ? 0 : pascal[i][j]));
    }
  }
}

int main() {
  t_aritmetica();
  t_primos();
  t_pollard();
  t_euclides();
  t_crt();
  t_bsgs();
  t_gauss();
  t_fft();
  t_matriz_comb();
  printf("OK matematica\n");
}
