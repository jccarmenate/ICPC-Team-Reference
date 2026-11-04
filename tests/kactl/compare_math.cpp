// Compara la matematica del notebook contra KACTL.
#include "kactl_begin.h"
namespace knt {
#include "number-theory/Factor.h"  // incluye ModMulLL.h y MillerRabin.h
#include "number-theory/CRT.h"     // incluye euclid.h
#include "number-theory/ModLog.h"
}
namespace ksieve {
#include "number-theory/FastEratosthenes.h"
}
namespace kfft {
#include "numerical/FastFourierTransform.h"
}
namespace kntt {
#include "NumberTheoreticTransformPatched.h"  // parche: sin ModPow.h
}
namespace ksl {
#include "numerical/SolveLinear.h"
}
#include "kactl_end.h"
#include "../../codigo/matematica/aritmetica.cpp"
#include "../../codigo/matematica/miller_rabin.cpp"
#include "../../codigo/matematica/pollard_rho.cpp"
#include "../../codigo/matematica/euclides.cpp"
#include "../../codigo/matematica/crt.cpp"
#include "../../codigo/matematica/bsgs.cpp"
#include "../../codigo/matematica/criba.cpp"
#include "../../codigo/matematica/gauss.cpp"
#include "../../codigo/matematica/fft.cpp"
#include "../../codigo/matematica/ntt.cpp"

void t_primes() {
  for (long long x = 0; x < 2000000; x++) CHECK(is_prime(x) == knt::isPrime(x));
  vector<long long> special = {561, 1105, 1729, 2465, 2047, 3215031751LL,
                               3825123056546413051LL, (1LL << 61) - 1,
                               1000000007LL * 1000000009LL, 999999937LL * 999999937LL};
  for (long long x : special) CHECK(is_prime(x) == knt::isPrime(x));
  vector<long long> big(100000);
  for (auto& x : big) x = rnd(1, 7000000000000000000LL);
  for (int i = 0; i < 1000; i++) {  // semiprimos de primos cercanos a 2^31
    long long p = rnd(1000000000, 3000000000LL), q = rnd(1000000000, 3000000000LL);
    while (!knt::isPrime(p)) p++;
    while (!knt::isPrime(q)) q++;
    big.push_back(p * q);
    big.push_back(p);
  }
  vector<int> r1, r2;
  double t1 = measure([&] { for (long long x : big) r1.push_back(is_prime(x)); });
  double t2 = measure([&] { for (long long x : big) r2.push_back(knt::isPrime(x)); });
  CHECK(r1 == r2);
  report("Miller-Rabin, 1e5 numeros hasta 7e18", t1, t2);
  // factorizacion
  vector<long long> nums;
  for (int i = 0; i < 300; i++) nums.push_back(rnd(2, 1000000000000000000LL));
  for (size_t i = 0; i + 1 < big.size(); i++)
    if (big[i] > 3000000000LL * 2 && nums.size() < 600) nums.push_back(big[i]);
  vector<vector<long long>> f1, f2;
  double t3 = measure([&] { for (long long x : nums) f1.push_back(factorize(x)); });
  double t4 = measure([&] {
    for (long long x : nums) {
      auto f = knt::factor(x);
      vector<long long> g(f.begin(), f.end());
      sort(g.begin(), g.end());
      f2.push_back(g);
    }
  });
  CHECK(f1 == f2);
  report("Pollard Rho, 600 numeros hasta 1e18", t3, t4);
}

void t_euclid_crt_log() {
  for (int it = 0; it < 100000; it++) {
    long long a = rnd(0, 1000000000000LL), b = rnd(0, 1000000000000LL), x, y, kx, ky;
    long long g = extgcd(a, b, x, y), kg = knt::euclid(a, b, kx, ky);
    CHECK(g == kg && (__int128)a * x + (__int128)b * y == g);
  }
  for (int it = 0; it < 100000; it++) {  // CRT con solucion (KACTL lo exige)
    long long m = rnd(1, 1000000000), n = rnd(1, 1000000000);
    long long l = m / __gcd(m, n) * n, x = rnd(0, l - 1);
    auto ours = crt(x % m, m, x % n, n);
    CHECK(ours.first == x && ours.second == l);
    CHECK(knt::crt(x % m, m, x % n, n) == x);
  }
  // logaritmo discreto: KACTL da el menor x > 0 y el notebook el menor
  // x >= 0, asi que se comparan los casos donde x = 0 no es solucion.
  for (long long m = 2; m <= 70; m++)
    for (long long a = 0; a < m; a++)
      for (long long b = 0; b < m; b++) {
        long long ours = discrete_log(a, b, m);
        if (ours != 0) CHECK(ours == knt::modLog(a, b, m));
      }
  vector<array<long long, 3>> qs(300);
  for (auto& q : qs) {
    q[2] = rnd(2, 1000000000);
    q[0] = rnd(0, q[2] - 1);
    q[1] = rnd(0, 2) ? powmod(q[0], rnd(1, 1000000000), q[2]) : rnd(0, q[2] - 1);
  }
  vector<long long> r1, r2;
  double t1 = measure([&] { for (auto& q : qs) r1.push_back(discrete_log(q[0], q[1], q[2])); });
  double t2 = measure([&] { for (auto& q : qs) r2.push_back(knt::modLog(q[0], q[1], q[2])); });
  for (size_t i = 0; i < qs.size(); i++)
    if (r1[i] != 0) CHECK(r1[i] == r2[i]);
  report("BSGS, 300 consultas con m hasta 1e9", t1, t2);
}

void t_sieve() {
  vector<int> p1, p2;
  double t1 = measure([&] { Sieve s(1000000); p1 = s.primes; });
  double t2 = measure([&] { p2 = ksieve::eratosthenes(); });
  while (!p2.empty() && p2.back() >= 1000000) p2.pop_back();
  while (!p1.empty() && p1.back() >= 1000000) p1.pop_back();
  CHECK(p1 == p2);
  report("Criba hasta 1e6", t1, t2);
}

vector<long long> exact_product(const vector<long long>& a, const vector<long long>& b) {
  vector<__int128> c(a.size() + b.size() - 1, 0);
  for (size_t i = 0; i < a.size(); i++)
    for (size_t j = 0; j < b.size(); j++) c[i + j] += (__int128)a[i] * b[j];
  return vector<long long>(c.begin(), c.end());
}
vector<long long> kactl_fft(const vector<long long>& a, const vector<long long>& b) {
  kfft::vd x(a.begin(), a.end()), y(b.begin(), b.end());
  vector<long long> r;
  for (double v : kfft::conv(x, y)) r.push_back(llround(v));
  return r;
}

void t_fft() {
  for (int it = 0; it < 2000; it++) {
    int n = rnd(1, 300), m = rnd(1, 300);
    vector<long long> a(n), b(m);
    for (auto& x : a) x = rnd(-1000, 1000);
    for (auto& x : b) x = rnd(-1000, 1000);
    auto e = exact_product(a, b);
    CHECK(multiply(a, b) == e && kactl_fft(a, b) == e);
    for (auto& x : a) x = rnd(0, 998244352);
    for (auto& x : b) x = rnd(0, 998244352);
    CHECK(multiply_mod(a, b) == kntt::conv(a, b));
  }
  // limite de precision: largo 8192 con valores hasta V
  printf("  precision de la FFT (largo 8192, valores en [0, V]):\n");
  for (long long V : {10000LL, 30000LL, 100000LL, 300000LL, 1000000LL}) {
    vector<long long> a(8192), b(8192);
    for (auto& x : a) x = rnd(0, V);
    for (auto& x : b) x = rnd(0, V);
    auto e = exact_product(a, b);
    long long mx = *max_element(e.begin(), e.end());
    bool ours = multiply(a, b) == e, kac = kactl_fft(a, b) == e;
    printf("    V=%-8lld coef. max %.1e   notebook %-7s KACTL %s\n", V, (double)mx,
           ours ? "exacto" : "FALLA", kac ? "exacto" : "FALLA");
    if (mx < 100000000000000LL) CHECK(ours);  // lo que promete el comentario
  }
  int N = 500000;
  vector<long long> a(N), b(N);
  for (auto& x : a) x = rnd(0, 1000);
  for (auto& x : b) x = rnd(0, 1000);
  vector<long long> r1, r2;
  double t1 = measure([&] { r1 = multiply(a, b); });
  double t2 = measure([&] { r2 = kactl_fft(a, b); });
  CHECK(r1 == r2);
  report("FFT, 5e5 x 5e5", t1, t2);
  for (auto& x : a) x = rnd(0, 998244352);
  for (auto& x : b) x = rnd(0, 998244352);
  double t3 = measure([&] { r1 = multiply_mod(a, b); });
  double t4 = measure([&] { r2 = kntt::conv(a, b); });
  CHECK(r1 == r2);
  report("NTT, 5e5 x 5e5", t3, t4);
}

void t_gauss() {
  for (int it = 0; it < 20000; it++) {
    int n = rnd(1, 5), m = rnd(1, 5);
    vector<vector<double>> aug(n, vector<double>(m + 1));
    vector<ksl::vd> A(n, ksl::vd(m));
    ksl::vd b(n), x(m);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) A[i][j] = aug[i][j] = rnd(0, 3) ? rnd(-3, 3) : 0;
      b[i] = aug[i][m] = rnd(-5, 5);
    }
    if (n > 1 && rnd(0, 2) == 0) { aug[n - 1] = aug[0]; A[n - 1] = A[0]; b[n - 1] = b[0]; }
    vector<double> ans;
    int ours = gauss(aug, ans);
    int rank = ksl::solveLinear(A, b, x);
    int expected = rank == -1 ? 0 : rank == m ? 1 : 2;
    CHECK(ours == expected);
  }
  int N = 500;
  vector<vector<double>> aug(N, vector<double>(N + 1));
  vector<ksl::vd> A(N, ksl::vd(N));
  ksl::vd b(N), x(N);
  vector<double> sol(N);
  for (auto& v : sol) v = rnd(-1000, 1000);
  for (int i = 0; i < N; i++) {
    double s = 0;
    for (int j = 0; j < N; j++) { A[i][j] = aug[i][j] = rnd(-1000, 1000); s += A[i][j] * sol[j]; }
    b[i] = aug[i][N] = s;
  }
  vector<double> ans;
  int r1 = 0, r2 = 0;
  double t1 = measure([&] { r1 = gauss(aug, ans); });
  double t2 = measure([&] { r2 = ksl::solveLinear(A, b, x); });
  CHECK(r1 == 1 && r2 == N);
  for (int j = 0; j < N; j++) CHECK(fabs(ans[j] - sol[j]) < 1e-6 && fabs(x[j] - sol[j]) < 1e-6);
  report("Gauss 500x500 (reales)", t1, t2);
}

int main() {
  t_primes();
  t_euclid_crt_log();
  t_sieve();
  t_fft();
  t_gauss();
  printf("OK matematica (igual que KACTL)\n");
  print_timings();
}
