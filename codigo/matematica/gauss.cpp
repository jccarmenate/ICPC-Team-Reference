// Eliminacion de Gauss-Jordan. O(n m min(n, m)).
// a = matriz aumentada de n filas y m + 1 columnas (A | b).
// Retorna 0 (sin solucion), 1 (unica) o 2 (infinitas).
// ans = una solucion (variables libres en 0).
// Cada fila se escala por su mayor valor absoluto: asi EPS es
// relativo y funciona igual con coeficientes chicos o grandes.
int gauss(vector<vector<double>> a, vector<double>& ans) {
  const double EPS = 1e-9;
  int n = a.size(), m = a[0].size() - 1, rank = 0;
  for (auto& r : a) {
    double mx = 0;
    for (double v : r) mx = max(mx, fabs(v));
    if (mx > 0)
      for (double& v : r) v /= mx;
  }
  vector<int> where(m, -1);
  for (int col = 0; col < m && rank < n; col++) {
    int piv = rank;
    for (int i = rank; i < n; i++)
      if (fabs(a[i][col]) > fabs(a[piv][col])) piv = i;
    if (fabs(a[piv][col]) < EPS) continue;
    swap(a[piv], a[rank]);
    where[col] = rank;
    for (int i = 0; i < n; i++) {
      if (i == rank) continue;
      double c = a[i][col] / a[rank][col];
      for (int j = col; j <= m; j++) a[i][j] -= a[rank][j] * c;
    }
    rank++;
  }
  ans.assign(m, 0);
  for (int j = 0; j < m; j++)
    if (where[j] != -1) ans[j] = a[where[j]][m] / a[where[j]][j];
  for (int i = rank; i < n; i++)  // filas sin pivote: quedan 0 = b
    if (fabs(a[i][m]) > EPS) return 0;
  for (int j = 0; j < m; j++)
    if (where[j] == -1) return 2;
  return 1;
}

// Igual pero modulo un primo. Requiere powmod.
// Entradas en [0, mod). El rango es la cantidad de where[j] != -1.
int gauss_mod(vector<vector<long long>> a, vector<long long>& ans,
              long long mod) {
  int n = a.size(), m = a[0].size() - 1;
  vector<int> where(m, -1);
  for (int col = 0, row = 0; col < m && row < n; col++) {
    int piv = row;
    while (piv < n && a[piv][col] == 0) piv++;
    if (piv == n) continue;
    swap(a[piv], a[row]);
    where[col] = row;
    long long inv = powmod(a[row][col], mod - 2, mod);
    for (int j = col; j <= m; j++)
      a[row][j] = a[row][j] * inv % mod;
    for (int i = 0; i < n; i++) {
      if (i == row || a[i][col] == 0) continue;
      long long c = a[i][col];
      for (int j = col; j <= m; j++)
        a[i][j] = ((a[i][j] - a[row][j] * c) % mod + mod) % mod;
    }
    row++;
  }
  ans.assign(m, 0);
  for (int j = 0; j < m; j++)
    if (where[j] != -1) ans[j] = a[where[j]][m];
  for (int i = 0; i < n; i++) {
    long long s = 0;
    for (int j = 0; j < m; j++) s = (s + ans[j] * a[i][j]) % mod;
    if (s != a[i][m]) return 0;
  }
  for (int j = 0; j < m; j++)
    if (where[j] == -1) return 2;
  return 1;
}
