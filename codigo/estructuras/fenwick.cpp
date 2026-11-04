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
    int pos = 0, pw = 1;
    while (2 * pw <= n) pw *= 2;
    for (; pw > 0; pw /= 2)
      if (pos + pw <= n && bit[pos + pw] < k) {
        pos += pw;
        k -= bit[pos];
      }
    return pos;
  }
};
