// Criba lineal en O(n). lp[x] = menor primo que divide a x.
// factorize(x) en O(log x) para x <= n.
struct Sieve {
  vector<int> lp, primes;
  Sieve(int n) : lp(n + 1, 0) {
    for (int i = 2; i <= n; i++) {
      if (lp[i] == 0) {
        lp[i] = i;
        primes.push_back(i);
      }
      for (int p : primes) {
        if (p > lp[i] || (long long)i * p > n) break;
        lp[i * p] = p;
      }
    }
  }
  bool is_prime(int x) { return x >= 2 && lp[x] == x; }
  vector<pair<int, int>> factorize(int x) {  // (primo, exponente)
    vector<pair<int, int>> f;
    while (x > 1) {
      int p = lp[x], e = 0;
      while (x % p == 0) {
        x /= p;
        e++;
      }
      f.push_back({p, e});
    }
    return f;
  }
};
