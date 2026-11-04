// Requiere aritmetica.cpp y euclides.cpp; m >= 1.
// a/b modulo m: -1 si b no es invertible. O(log m).
long long divmod(long long a, long long b, long long m) {
  long long inv = inv_mod(b, m);
  return inv == -1 ? -1 : mulmod(norm_mod(a, m), inv, m);
}
// Inversos de un vector: O(n + log m), una sola inversion.
// nullopt si algun elemento no es coprimo con m.
optional<vector<long long>> batch_inverse(
    const vector<long long>& a, long long m) {
  int n = (int)a.size();
  vector<long long> pref(n + 1, 1 % m), ans(n);
  for (int i = 0; i < n; ++i)
    pref[i + 1] = mulmod(pref[i], norm_mod(a[i], m), m);
  long long cur = inv_mod(pref[n], m);
  if (cur == -1) return nullopt;
  for (int i = n - 1; i >= 0; --i) {
    ans[i] = mulmod(cur, pref[i], m);
    cur = mulmod(cur, norm_mod(a[i], m), m);
  }
  return ans;
}
// ax = b (mod m): {x0, paso}; todas x=x0+k*paso.
// Hay gcd(a,m) residuos distintos modulo m. {-1,-1} si no hay.
pair<long long, long long> linear_congruence(
    long long a, long long b, long long m) {
  a = norm_mod(a, m); b = norm_mod(b, m);
  long long g = gcd(a, m);
  if (b % g) return {-1, -1};
  long long step = m / g;
  return {mulmod(b / g, inv_mod(a / g, step), step), step};
}
