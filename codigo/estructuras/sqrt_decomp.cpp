// Sqrt Decomposition: suma en rango con suma perezosa por bloque.
// update(l, r, x) y query(l, r) en O(sqrt n). Rangos cerrados.
struct SqrtDecomp {
  int n, B;
  vector<long long> a, block_sum, lazy;
  SqrtDecomp(const vector<long long>& v) : n(v.size()), a(v) {
    B = max(1, (int)sqrt(n));
    block_sum.assign(n / B + 1, 0);
    lazy.assign(n / B + 1, 0);
    for (int i = 0; i < n; i++) block_sum[i / B] += a[i];
  }
  void add_naive(int l, int r, long long x) {  // dentro de un bloque
    for (int i = l; i <= r; i++) {
      a[i] += x;
      block_sum[i / B] += x;
    }
  }
  long long sum_naive(int l, int r) {  // dentro de un bloque
    long long res = 0;
    for (int i = l; i <= r; i++) res += a[i] + lazy[i / B];
    return res;
  }
  void update(int l, int r, long long x) {
    int bl = l / B, br = r / B;
    if (bl == br) { add_naive(l, r, x); return; }
    add_naive(l, (bl + 1) * B - 1, x);
    for (int b = bl + 1; b < br; b++) lazy[b] += x;
    add_naive(br * B, r, x);
  }
  long long query(int l, int r) {
    int bl = l / B, br = r / B;
    if (bl == br) return sum_naive(l, r);
    long long res = sum_naive(l, (bl + 1) * B - 1) + sum_naive(br * B, r);
    for (int b = bl + 1; b < br; b++) res += block_sum[b] + lazy[b] * B;
    return res;
  }
};
