// NTT modulo 998244353 = 119 * 2^23 + 1 (raiz primitiva 3).
// Requiere powmod. Exacta, O(n log n), n hasta 2^23.
// Otros primos validos: 469762049 y 167772161 (ambos raiz 3).
const long long MOD = 998244353, ROOT = 3;

void ntt(vector<long long>& a, bool invert) {
  int n = a.size();
  for (int i = 1, j = 0; i < n; i++) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;
    if (i < j) swap(a[i], a[j]);
  }
  for (int len = 2; len <= n; len *= 2) {
    long long wlen = powmod(ROOT, (MOD - 1) / len, MOD);
    if (invert) wlen = powmod(wlen, MOD - 2, MOD);
    for (int i = 0; i < n; i += len) {
      long long w = 1;
      for (int j = 0; j < len / 2; j++) {
        long long u = a[i + j];
        long long v = a[i + j + len / 2] * w % MOD;
        a[i + j] = (u + v) % MOD;
        a[i + j + len / 2] = (u - v + MOD) % MOD;
        w = w * wlen % MOD;
      }
    }
  }
  if (invert) {
    long long inv_n = powmod(n, MOD - 2, MOD);
    for (long long& x : a) x = x * inv_n % MOD;
  }
}
// Producto de polinomios con coeficientes en [0, MOD).
vector<long long> multiply_mod(vector<long long> a,
                                  vector<long long> b) {
  if (a.empty() || b.empty()) return {};
  int sz = a.size() + b.size() - 1, n = 1;
  while (n < sz) n *= 2;
  a.resize(n);
  b.resize(n);
  ntt(a, false);
  ntt(b, false);
  for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % MOD;
  ntt(a, true);
  a.resize(sz);
  return a;
}
