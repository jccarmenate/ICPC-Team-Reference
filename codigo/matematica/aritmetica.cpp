// Aritmetica modular de 64 bits (usa __int128 de GCC).
// Siempre m >= 1. Resultados en [0,m), incluso con negativos.
long long norm_mod(long long a, long long m) {
  a %= m;
  return a < 0 ? a + m : a;
}
long long addmod(long long a, long long b, long long m) {
  return (long long)(((__int128)norm_mod(a, m) +
                      norm_mod(b, m)) % m);
}
long long submod(long long a, long long b, long long m) {
  return norm_mod(norm_mod(a, m) - norm_mod(b, m), m);
}
// Requiere 0 <= a,b < m. O(1), sin desborde del producto.
long long mulmod(long long a, long long b, long long m) {
  return (long long)((__int128)a * b % m);
}
// b^e mod m, e >= 0. O(log e). 0^0 = 1 (mod m).
long long powmod(long long b, long long e, long long m) {
  long long r = 1 % m;
  b = norm_mod(b, m);
  while (e > 0) {
    if (e & 1) r = mulmod(r, b, m);
    b = mulmod(b, b, m);
    e >>= 1;
  }
  return r;
}
// Inverso por Fermat: p PRIMO >= 2. -1 si p divide a.
// Para modulo compuesto usar inv_mod de Euclides extendido.
long long inv_prime(long long a, long long p) {
  a = norm_mod(a, p);
  return a == 0 ? -1 : powmod(a, p - 2, p);
}
// Inversos de 1..n en O(n), p primo y 0 <= n < p.
// inv[0] = -1: cero no tiene inverso.
vector<long long> inverse_table(int n, long long p) {
  vector<long long> inv(n + 1, -1);
  if (n >= 1) inv[1] = 1;
  for (int i = 2; i <= n; ++i)
    inv[i] = p - mulmod(p / i, inv[p % i], p);
  return inv;
}
// {a^n, 1+a+...+a^(n-1)} modulo m, n>=0, O(log n).
// Funciona con modulo compuesto y a=1, sin dividir por a-1.
pair<long long, long long> power_sum(long long a,
                                     long long n, long long m) {
  if (n == 0) return {1 % m, 0};
  a = norm_mod(a, m);
  auto [p, s] = power_sum(a, n / 2, m);
  s = mulmod(s, addmod(1, p, m), m);
  p = mulmod(p, p, m);
  if (n & 1) { s = addmod(s, p, m); p = mulmod(p, a, m); }
  return {p, s};
}
