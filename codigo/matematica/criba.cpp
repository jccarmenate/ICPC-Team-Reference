// Criba lineal en O(n). menor[x] = menor primo que divide a x.
// factorizar(x) en O(log x) para x <= n.
struct Criba {
  vector<int> menor, primos;
  Criba(int n) : menor(n + 1, 0) {
    for (int i = 2; i <= n; i++) {
      if (menor[i] == 0) {
        menor[i] = i;
        primos.push_back(i);
      }
      for (int p : primos) {
        if (p > menor[i] || (long long)i * p > n) break;
        menor[i * p] = p;
      }
    }
  }
  bool es_primo(int x) { return x >= 2 && menor[x] == x; }
  vector<pair<int, int>> factorizar(int x) {  // (primo, exponente)
    vector<pair<int, int>> f;
    while (x > 1) {
      int p = menor[x], e = 0;
      while (x % p == 0) {
        x /= p;
        e++;
      }
      f.push_back({p, e});
    }
    return f;
  }
};
