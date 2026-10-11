// Algoritmo hungaro: asignacion de costo MINIMO en O(n^2 m).
// a[i][j] = costo de asignar la fila i a la columna j, con n <= m.
// Retorna (costo, asig) con asig[i] = columna de la fila i.
// Para MAXIMO peso: negar los costos y negar el resultado.
pair<long long, vector<int>> hungaro(
    const vector<vector<long long>>& a) {
  int n = a.size(), m = a[0].size();
  const long long INF = LLONG_MAX / 4;
  vector<long long> u(n + 1, 0), v(m + 1, 0);
  vector<int> p(m + 1, 0), camino(m + 1, 0);
  for (int i = 1; i <= n; i++) {
    p[0] = i;
    int j0 = 0;
    vector<long long> minv(m + 1, INF);
    vector<bool> usado(m + 1, false);
    do {
      usado[j0] = true;
      int i0 = p[j0], j1 = 0;
      long long delta = INF;
      for (int j = 1; j <= m; j++) {
        if (usado[j]) continue;
        long long cur = a[i0 - 1][j - 1] - u[i0] - v[j];
        if (cur < minv[j]) { minv[j] = cur; camino[j] = j0; }
        if (minv[j] < delta) { delta = minv[j]; j1 = j; }
      }
      for (int j = 0; j <= m; j++) {
        if (usado[j]) { u[p[j]] += delta; v[j] -= delta; }
        else minv[j] -= delta;
      }
      j0 = j1;
    } while (p[j0] != 0);
    do {
      int j1 = camino[j0];
      p[j0] = p[j1];
      j0 = j1;
    } while (j0 != 0);
  }
  vector<int> asig(n, -1);
  for (int j = 1; j <= m; j++)
    if (p[j] != 0) asig[p[j] - 1] = j - 1;
  return {-v[0], asig};
}
