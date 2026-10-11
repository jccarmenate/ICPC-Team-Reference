// Pollard Rho: factorizacion de n < 2^63 en ~O(n^(1/4)).
// Requiere mulmod y es_primo (Miller-Rabin).
// rho(n): un factor no trivial de n (n compuesto).
// factorizar(n): factores primos con repeticion, ordenados.
long long rho(long long n) {
  if (n % 2 == 0) return 2;
  static mt19937_64 gen(20260101);
  while (true) {
    long long c = gen() % (n - 1) + 1;
    long long x = gen() % n, y = x, q = 1;
    auto f = [&](long long v) { return (mulmod(v, v, n) + c) % n; };
    for (int it = 1;; it++) {
      x = f(x);
      y = f(f(y));
      long long p = mulmod(q, llabs(x - y), n);
      if (p != 0) q = p;  // q nunca se vuelve multiplo de n
      if (p == 0 || it % 32 == 0) {
        long long d = __gcd(q, n);
        if (d > 1) return d;
        if (x == y) break;  // ciclo sin exito: otra constante
      }
    }
  }
}
void factorizar_rec(long long n, vector<long long>& fs) {
  if (n == 1) return;
  if (es_primo(n)) { fs.push_back(n); return; }
  long long d = rho(n);
  factorizar_rec(d, fs);
  factorizar_rec(n / d, fs);
}
vector<long long> factorizar(long long n) {
  vector<long long> fs;
  factorizar_rec(n, fs);
  sort(fs.begin(), fs.end());
  return fs;
}
