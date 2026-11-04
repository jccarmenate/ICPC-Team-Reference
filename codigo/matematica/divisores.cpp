// Factorizacion simple O(sqrt(n)), n>=1. Para n grande usar
// Pollard Rho; para muchas consultas pequenas, la criba.
vector<pair<long long, int>> trial_factor(long long n) {
  vector<pair<long long, int>> f;
  for (long long p = 2; p <= n / p; p += (p == 2 ? 1 : 2)) {
    if (n % p) continue;
    int e = 0;
    do { n /= p; ++e; } while (n % p == 0);
    f.push_back({p, e});
  }
  if (n > 1) f.push_back({n, 1});
  return f;
}
// f: factores primos distintos, exponentes positivos; producto
// n <= LLONG_MAX. f vacio representa n=1.
// Divisores sin ordenar en O(tau(n)); ordenar si hace falta.
vector<long long> divisors(const vector<pair<long long, int>>& f) {
  vector<long long> d = {1};
  for (auto [p, e] : f) {
    int size = (int)d.size();
    long long power = 1;
    for (int k = 1; k <= e; ++k) {
      power *= p;
      for (int i = 0; i < size; ++i) d.push_back(d[i] * power);
    }
  }
  return d;
}
// {cantidad, suma} de divisores. La suma puede exceder 64 bits.
pair<long long, __int128> divisor_stats(
    const vector<pair<long long, int>>& f) {
  long long count = 1;
  __int128 sum = 1;
  for (auto [p, e] : f) {
    count *= e + 1;
    __int128 term = 1, power = 1;
    for (int k = 1; k <= e; ++k) { power *= p; term += power; }
    sum *= term;
  }
  return {count, sum};
}
// phi(n) = cantidad de enteros de 1..n coprimos con n.
// n>=1, f debe ser su factorizacion; phi(1)=1.
long long euler_phi(long long n,
    const vector<pair<long long, int>>& f) {
  for (auto [p, e] : f) n = n / p * (p - 1);
  return n;
}
// Exponente del primo p>=2 en n!, n>=0. O(log_p(n)).
long long factorial_valuation(long long n, long long p) {
  long long e = 0;
  while (n) { n /= p; e += n; }
  return e;
}
