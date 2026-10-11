// Factoriales e inversos modulo un primo. Precalculo O(n),
// C(n, k) en O(1). Requiere n < mod.
struct Combinatoria {
  long long mod;
  vector<long long> fact, inv_fact;
  Combinatoria(int n, long long mod_)
      : mod(mod_), fact(n + 1, 1), inv_fact(n + 1, 1) {
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i % mod;
    inv_fact[n] = potencia(fact[n], mod - 2);
    for (int i = n; i >= 1; i--)
      inv_fact[i - 1] = inv_fact[i] * i % mod;
  }
  long long potencia(long long b, long long e) {
    long long r = 1;
    for (b %= mod; e > 0; e >>= 1) {
      if (e & 1) r = r * b % mod;
      b = b * b % mod;
    }
    return r;
  }
  long long C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * inv_fact[k] % mod * inv_fact[n - k] % mod;
  }
};
