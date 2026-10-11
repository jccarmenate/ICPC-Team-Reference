// Logaritmo discreto (Baby Step / Giant Step). O(sqrt m).
// Requiere mulmod y powmod. Retorna el menor x >= 0 tal que
// a^x = b (mod m), o -1 si no existe. No exige gcd(a, m) = 1.
// Convencion: 0^0 = 1.
long long log_discreto(long long a, long long b, long long m) {
  a %= m;
  b %= m;
  long long k = 1 % m, sumar = 0, g;
  while ((g = __gcd(a, m)) > 1) {  // quitar factores comunes
    if (b == k) return sumar;
    if (b % g != 0) return -1;
    b /= g;
    m /= g;
    sumar++;
    k = mulmod(k, a / g, m);
  }
  long long n = (long long)sqrtl((long double)m) + 1;
  unordered_map<long long, long long> tabla;  // b * a^q -> q
  long long cur = b % m;
  for (long long q = 0; q <= n; q++) {
    tabla[cur] = q;
    cur = mulmod(cur, a % m, m);
  }
  long long an = powmod(a, n, m);
  cur = k % m;
  for (long long p = 1; p <= n; p++) {
    cur = mulmod(cur, an, m);
    auto it = tabla.find(cur);
    if (it != tabla.end()) return n * p - it->second + sumar;
  }
  return -1;
}
