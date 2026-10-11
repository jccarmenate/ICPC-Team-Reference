// Multiplicacion y exponenciacion de matrices cuadradas modulo
// mod. O(k^3 log e). Sirve para recurrencias lineales.
// Requiere mod < 3e9 para que no desborde el producto.
vector<vector<long long>> mat_mult(
    const vector<vector<long long>>& a,
    const vector<vector<long long>>& b, long long mod) {
  int k = a.size();
  vector<vector<long long>> c(k, vector<long long>(k, 0));
  for (int i = 0; i < k; i++)
    for (int l = 0; l < k; l++) {
      if (a[i][l] == 0) continue;
      for (int j = 0; j < k; j++)
        c[i][j] = (c[i][j] + a[i][l] * b[l][j]) % mod;
    }
  return c;
}
vector<vector<long long>> mat_pot(vector<vector<long long>> a,
                                  long long e, long long mod) {
  int k = a.size();
  vector<vector<long long>> r(k, vector<long long>(k, 0));
  for (int i = 0; i < k; i++) r[i][i] = 1 % mod;
  while (e > 0) {
    if (e & 1) r = mat_mult(r, a, mod);
    a = mat_mult(a, a, mod);
    e >>= 1;
  }
  return r;
}
