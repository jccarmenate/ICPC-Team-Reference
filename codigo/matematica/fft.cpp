// FFT iterativa con complex<double>. O(n log n).
// multiplicar(a, b): producto de polinomios con coeficientes
// enteros. Exacto mientras los coeficientes del resultado no
// pasen de ~1e14 (probado: n = 2e4 con valores hasta 1e5).
// Mas alla de eso, o modulo 998244353, usar NTT.
void fft(vector<complex<double>>& a, bool inversa) {
  const double PI = acos(-1.0);
  int n = a.size();
  for (int i = 1, j = 0; i < n; i++) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;
    if (i < j) swap(a[i], a[j]);
  }
  vector<complex<double>> raiz(n / 2);
  for (int i = 0; i < n / 2; i++)
    raiz[i] = polar(1.0, 2 * PI * i / n * (inversa ? -1 : 1));
  for (int len = 2; len <= n; len *= 2)
    for (int i = 0; i < n; i += len)
      for (int j = 0; j < len / 2; j++) {
        complex<double> u = a[i + j];
        complex<double> v = a[i + j + len / 2] * raiz[n / len * j];
        a[i + j] = u + v;
        a[i + j + len / 2] = u - v;
      }
  if (inversa)
    for (complex<double>& x : a) x /= n;
}
vector<long long> multiplicar(const vector<long long>& a,
                              const vector<long long>& b) {
  if (a.empty() || b.empty()) return {};
  int tam = a.size() + b.size() - 1, n = 1;
  while (n < tam) n *= 2;
  vector<complex<double>> fa(n), fb(n);
  for (int i = 0; i < (int)a.size(); i++) fa[i] = (double)a[i];
  for (int i = 0; i < (int)b.size(); i++) fb[i] = (double)b[i];
  fft(fa, false);
  fft(fb, false);
  for (int i = 0; i < n; i++) fa[i] *= fb[i];
  fft(fa, true);
  vector<long long> res(tam);
  for (int i = 0; i < tam; i++) res[i] = llround(fa[i].real());
  return res;
}
