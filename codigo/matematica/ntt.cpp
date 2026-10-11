// NTT modulo 998244353 = 119 * 2^23 + 1 (raiz primitiva 3).
// Requiere powmod. Exacta, O(n log n), n hasta 2^23.
// Otros primos validos: 469762049 y 167772161 (ambos raiz 3).
const long long MOD = 998244353, RAIZ = 3;

void ntt(vector<long long>& a, bool inversa) {
  int n = a.size();
  for (int i = 1, j = 0; i < n; i++) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;
    if (i < j) swap(a[i], a[j]);
  }
  for (int len = 2; len <= n; len *= 2) {
    long long wlen = powmod(RAIZ, (MOD - 1) / len, MOD);
    if (inversa) wlen = powmod(wlen, MOD - 2, MOD);
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
  if (inversa) {
    long long inv_n = powmod(n, MOD - 2, MOD);
    for (long long& x : a) x = x * inv_n % MOD;
  }
}
// Producto de polinomios con coeficientes en [0, MOD).
vector<long long> multiplicar_mod(vector<long long> a,
                                  vector<long long> b) {
  if (a.empty() || b.empty()) return {};
  int tam = a.size() + b.size() - 1, n = 1;
  while (n < tam) n *= 2;
  a.resize(n);
  b.resize(n);
  ntt(a, false);
  ntt(b, false);
  for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % MOD;
  ntt(a, true);
  a.resize(tam);
  return a;
}
