// Eliminacion de Gauss-Jordan. O(n m min(n, m)).
// a = matriz aumentada de n filas y m + 1 columnas (A | b).
// Retorna 0 (sin solucion), 1 (unica) o 2 (infinitas).
// ans = una solucion (variables libres en 0).
int gauss(vector<vector<double>> a, vector<double>& ans) {
  const double EPS = 1e-9;
  int n = a.size(), m = a[0].size() - 1;
  vector<int> donde(m, -1);
  for (int col = 0, fila = 0; col < m && fila < n; col++) {
    int piv = fila;
    for (int i = fila; i < n; i++)
      if (fabs(a[i][col]) > fabs(a[piv][col])) piv = i;
    if (fabs(a[piv][col]) < EPS) continue;
    swap(a[piv], a[fila]);
    donde[col] = fila;
    for (int i = 0; i < n; i++) {
      if (i == fila) continue;
      double c = a[i][col] / a[fila][col];
      for (int j = col; j <= m; j++) a[i][j] -= a[fila][j] * c;
    }
    fila++;
  }
  ans.assign(m, 0);
  for (int j = 0; j < m; j++)
    if (donde[j] != -1) ans[j] = a[donde[j]][m] / a[donde[j]][j];
  for (int i = 0; i < n; i++) {
    double s = 0;
    for (int j = 0; j < m; j++) s += ans[j] * a[i][j];
    if (fabs(s - a[i][m]) > EPS) return 0;
  }
  for (int j = 0; j < m; j++)
    if (donde[j] == -1) return 2;
  return 1;
}

// Igual pero modulo un primo. Requiere powmod.
// Entradas en [0, mod). El rango es la cantidad de donde[j] != -1.
int gauss_mod(vector<vector<long long>> a, vector<long long>& ans,
              long long mod) {
  int n = a.size(), m = a[0].size() - 1;
  vector<int> donde(m, -1);
  for (int col = 0, fila = 0; col < m && fila < n; col++) {
    int piv = fila;
    while (piv < n && a[piv][col] == 0) piv++;
    if (piv == n) continue;
    swap(a[piv], a[fila]);
    donde[col] = fila;
    long long inv = powmod(a[fila][col], mod - 2, mod);
    for (int j = col; j <= m; j++)
      a[fila][j] = a[fila][j] * inv % mod;
    for (int i = 0; i < n; i++) {
      if (i == fila || a[i][col] == 0) continue;
      long long c = a[i][col];
      for (int j = col; j <= m; j++)
        a[i][j] = ((a[i][j] - a[fila][j] * c) % mod + mod) % mod;
    }
    fila++;
  }
  ans.assign(m, 0);
  for (int j = 0; j < m; j++)
    if (donde[j] != -1) ans[j] = a[donde[j]][m];
  for (int i = 0; i < n; i++) {
    long long s = 0;
    for (int j = 0; j < m; j++) s = (s + ans[j] * a[i][j]) % mod;
    if (s != a[i][m]) return 0;
  }
  for (int j = 0; j < m; j++)
    if (donde[j] == -1) return 2;
  return 1;
}
