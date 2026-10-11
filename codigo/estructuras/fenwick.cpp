// Fenwick Tree (BIT). add y sum en O(log n). Indices 0..n-1.
struct Fenwick {
  int n;
  vector<long long> bit;
  Fenwick(int n_) : n(n_), bit(n_ + 1, 0) {}
  void add(int i, long long v) {
    for (i++; i <= n; i += i & -i) bit[i] += v;
  }
  long long sum(int i) {  // suma de [0, i]
    long long s = 0;
    for (i++; i > 0; i -= i & -i) s += bit[i];
    return s;
  }
  long long sum(int l, int r) { return sum(r) - sum(l - 1); }
  // Menor i con sum(i) >= k (valores no negativos). n si no hay.
  int lower_bound(long long k) {
    int pos = 0, pot = 1;
    while (2 * pot <= n) pot *= 2;
    for (; pot > 0; pot /= 2)
      if (pos + pot <= n && bit[pos + pot] < k) {
        pos += pot;
        k -= bit[pos];
      }
    return pos;
  }
};
