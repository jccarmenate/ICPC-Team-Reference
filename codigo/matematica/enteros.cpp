// Enteros: GCC C++17. std::pow usa reales; para exactitud usar
// int_power. std::gcd(a,b) admite cero; gcd(0,0)=0.
// std::gcd exige que |a| y |b| quepan en el tipo (no LLONG_MIN).
// std::lcm puede desbordar; esta version detecta el desborde.
optional<long long> lcm_checked(long long a, long long b) {
  assert(a >= 0 && b >= 0);
  if (a == 0 || b == 0) return 0;
  __int128 v = (__int128)(a / gcd(a, b)) * b;
  if (v > LLONG_MAX) return nullopt;
  return (long long)v;
}
// a^e en O(log e). 0^0=1; nullopt si no cabe en long long.
optional<long long> int_power(long long a, long long e) {
  assert(e >= 0);
  long long r = 1;
  while (e) {
    if (e & 1) {
      __int128 v = (__int128)r * a;
      if (v < LLONG_MIN || v > LLONG_MAX) return nullopt;
      r = (long long)v;
    }
    e >>= 1;
    if (e) {
      __int128 v = (__int128)a * a;
      if (v > LLONG_MAX) return nullopt;
      a = (long long)v;
    }
  }
  return r;
}
// Division hacia -infinito / +infinito, incluso con negativos.
// b != 0; excluir LLONG_MIN / -1 (resultado no representable).
long long floor_div(long long a, long long b) {
  assert(b != 0 && !(a == LLONG_MIN && b == -1));
  long long q = a / b, r = a % b;
  return q - (r != 0 && ((r < 0) != (b < 0)));
}
long long ceil_div(long long a, long long b) {
  assert(b != 0 && !(a == LLONG_MIN && b == -1));
  long long q = a / b, r = a % b;
  return q + (r != 0 && ((r < 0) == (b < 0)));
}
// floor(sqrt(n)), exacta en todo long long no negativo.
long long integer_sqrt(long long n) {
  assert(n >= 0);
  long long r = sqrtl((long double)n);
  while ((__int128)r * r > n) --r;
  while ((__int128)(r + 1) * (r + 1) <= n) ++r;
  return r;
}
bool is_square(long long n) {
  if (n < 0) return false;
  long long r = integer_sqrt(n);
  return r * r == n;
}
// floor(log2(n)); -1 para cero. Nunca llamar clz/ctz con 0.
int floor_log2(unsigned long long n) {
  return n ? 63 - __builtin_clzll(n) : -1;
}
// Otros: __builtin_popcountll(n), __builtin_ctzll(n) (n!=0).
// 1ULL << k requiere 0<=k<64; usar unsigned para bits.
// Reales: powl(a,b), sqrtl(x), cbrtl(x), hypotl(x,y), logl(x),
// log2l(x), log10l(x), expl(x), floorl(x), ceill(x), roundl(x).
// log1pl(x) y expm1l(x) evitan perdida de precision cerca de 0.
// Comparacion aproximada (no usar como comparador de sort):
bool almost_equal(long double a, long double b,
                  long double eps = 1e-12L) {
  return a == b || (isfinite(a) && isfinite(b) &&
      fabsl(a - b) <= eps * max({1.0L, fabsl(a), fabsl(b)}));
}
