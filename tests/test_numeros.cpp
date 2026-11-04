#include "comun.h"
#include "../codigo/matematica/enteros.cpp"
#include "../codigo/matematica/aritmetica.cpp"
#include "../codigo/matematica/euclides.cpp"
#include "../codigo/matematica/inversos.cpp"
#include "../codigo/matematica/divisores.cpp"

int main() {
  for (long long a = -100; a <= 100; ++a) {
    for (long long b = -100; b <= 100; ++b) if (b) {
      CHECK(floor_div(a,b) == (long long)floorl((long double)a/b));
      CHECK(ceil_div(a,b) == (long long)ceill((long double)a/b));
    }
    __int128 p = 1;
    for (int e = 0; e <= 70; ++e) {
      auto v = int_power(a,e);
      bool fits = p >= LLONG_MIN && p <= LLONG_MAX;
      CHECK(v.has_value() == fits);
      if (!fits) break;
      CHECK(*v == p);
      p *= a;
    }
  }
  CHECK(int_power(-2,63) == LLONG_MIN);
  CHECK(!int_power(2,63));
  CHECK(int_power(LLONG_MIN,1) == LLONG_MIN);
  CHECK(!int_power(LLONG_MIN,2));
  CHECK(int_power(0,0) == 1);
  CHECK(floor_div(LLONG_MIN,3) == -3074457345618258603LL);
  CHECK(ceil_div(LLONG_MAX,2) == 4611686018427387904LL);
  CHECK(lcm_checked(0,0) == 0);
  CHECK(lcm_checked(LLONG_MAX,LLONG_MAX) == LLONG_MAX);
  CHECK(!lcm_checked(LLONG_MAX,2));
  for (int a = 0; a <= 100; ++a)
    for (int b = 0; b <= 100; ++b) {
      long long l = 0;
      if (a && b) { l = a; while (l % b) l += a; }
      CHECK(lcm_checked(a,b) == l);
    }
  for (long long n = 0, r = 0; n <= 100000; ++n) {
    if ((r+1)*(r+1) <= n) ++r;
    CHECK(integer_sqrt(n) == r);
    CHECK(is_square(n) == (r*r == n));
  }
  CHECK(!is_square(-1));
  CHECK(floor_log2(0) == -1);
  for (int k = 0; k < 64; ++k) {
    CHECK(floor_log2(1ULL << k) == k);
    if (k) CHECK(floor_log2((1ULL << k)-1) == k-1);
  }
  CHECK(almost_equal(1,1+1e-13L));
  CHECK(!almost_equal(1,1.001L));
  CHECK(!almost_equal(1,numeric_limits<long double>::infinity()));
  vector<long long> edge = {LLONG_MIN, LLONG_MIN+1, -1, 0, 1, LLONG_MAX-1, LLONG_MAX};
  for (long long m : {1LL, 2LL, 97LL, LLONG_MAX-1, LLONG_MAX}) {
    auto norm = [m](__int128 x) { return (long long)((x % m + m) % m); };
    for (long long a : edge) {
      CHECK(norm_mod(a,m) == norm(a));
      long long inv = inv_mod(a,m);
      CHECK((inv != -1) == (gcd(norm(a),m) == 1));
      if (inv != -1) CHECK(norm((__int128)a*inv) == 1%m);
      for (long long b : edge) {
        CHECK(addmod(a,b,m) == norm((__int128)a+b));
        CHECK(submod(a,b,m) == norm((__int128)a-b));
        CHECK(mulmod(norm(a),norm(b),m) == norm((__int128)norm(a)*norm(b)));
      }
      long long p = 1%m, s = 0;
      for (int e = 0; e <= 64; ++e) {
        CHECK(powmod(a,e,m) == p);
        CHECK(power_sum(a,e,m) == make_pair(p,s));
        s = norm((__int128)s+p);
        p = norm((__int128)p*a);
      }
    }
  }
  for (int it = 0; it < 20000; ++it) {
    long long n = rnd(0,LLONG_MAX), r = integer_sqrt(n);
    CHECK((__int128)r*r <= n && (__int128)(r+1)*(r+1) > n);
    long long m = rnd(1,LLONG_MAX), a = rnd(LLONG_MIN,LLONG_MAX);
    long long inv = inv_mod(a,m), v = norm_mod(a,m);
    CHECK((inv != -1) == (gcd(v,m) == 1));
    if (inv != -1) CHECK(mulmod(v,inv,m) == 1%m);
  }
  for (long long m = 1; m <= 40; ++m)
    for (long long a = -40; a <= 40; ++a)
      for (long long b = -40; b <= 40; ++b) {
        auto [x,step] = linear_congruence(a,b,m);
        long long first = -1, count = 0;
        for (long long i = 0; i < m; ++i) if ((a*i-b)%m == 0) {
          if (first == -1) first = i;
          ++count;
        }
        CHECK(x == first);
        if (x != -1) CHECK(step == m/count);
        long long v = divmod(a,b,m), inv = inv_mod(b,m);
        CHECK((v == -1) == (inv == -1));
        if (v != -1) CHECK(norm_mod(v*b-a,m) == 0);
      }
  for (long long p : {2LL,3LL,17LL,101LL}) {
    auto table = inverse_table((int)p-1,p);
    CHECK(table[0] == -1);
    for (int i = 1; i < p; ++i) {
      CHECK(i*table[i]%p == 1);
      CHECK(inv_prime(i,p) == table[i]);
      CHECK(inv_prime(i-p,p) == table[i]);
    }
    CHECK(inv_prime(p,p) == -1);
  }
  for (int it = 0; it < 5000; ++it) {
    long long m = rnd(1,1000);
    vector<long long> a(rnd(0,20)), expected;
    bool valid = true;
    for (auto& x : a) { x = rnd(-2000,2000); expected.push_back(inv_mod(x,m)); valid &= expected.back() != -1; }
    auto got = batch_inverse(a,m);
    CHECK(got.has_value() == valid);
    if (valid) CHECK(*got == expected);
  }
  for (long long n = 1; n <= 2000; ++n) {
    auto f = trial_factor(n);
    long long product = 1, last = 1;
    for (auto [p,e] : f) {
      CHECK(p > last && e > 0);
      for (long long d = 2; d*d <= p; ++d) CHECK(p%d != 0);
      for (int k = 0; k < e; ++k) product *= p;
      last = p;
    }
    CHECK(product == n);
    vector<long long> expected;
    long long sum = 0, phi = 0;
    for (long long d = 1; d <= n; ++d) {
      if (n%d == 0) { expected.push_back(d); sum += d; }
      phi += gcd(d,n) == 1;
    }
    auto ds = divisors(f); sort(ds.begin(),ds.end());
    CHECK(ds == expected);
    auto stats = divisor_stats(f);
    CHECK(stats.first == (long long)expected.size() && stats.second == sum);
    CHECK(euler_phi(n,f) == phi);
  }
  CHECK(divisor_stats({{2,62}}).second == LLONG_MAX);
  for (long long p : {2,3,5,7,11}) {
    long long e = 0;
    for (int n = 0; n <= 1000; ++n) {
      if (n) { int v = n; while (v%p == 0) { ++e; v /= (int)p; } }
      CHECK(factorial_valuation(n,p) == e);
    }
  }
  puts("OK operaciones numericas");
}
