// Suffix Array en O(n log n) (duplicacion + counting sort) y
// arreglo LCP de Kasai en O(n).
// sa[i] = inicio del i-esimo sufijo en orden lexicografico.
// lcp[i] = prefijo comun mas largo entre sa[i] y sa[i + 1].
// Substrings distintos = n (n + 1) / 2 - suma de lcp.
vector<int> suffix_array(const string& t) {
  string s = t;
  s.push_back('\0');  // centinela menor que todo
  int n = s.size(), classes = 1;
  vector<int> p(n), c(n), pn(n), cn(n), cnt(max(n, 256), 0);
  for (int i = 0; i < n; i++) cnt[(unsigned char)s[i]]++;
  for (int i = 1; i < 256; i++) cnt[i] += cnt[i - 1];
  for (int i = 0; i < n; i++) p[--cnt[(unsigned char)s[i]]] = i;
  c[p[0]] = 0;
  for (int i = 1; i < n; i++) {
    if (s[p[i]] != s[p[i - 1]]) classes++;
    c[p[i]] = classes - 1;
  }
  for (int h = 0; (1 << h) < n; h++) {
    int step = 1 << h;
    for (int i = 0; i < n; i++) pn[i] = (p[i] - step + n) % n;
    fill(cnt.begin(), cnt.begin() + classes, 0);
    for (int i = 0; i < n; i++) cnt[c[pn[i]]]++;
    for (int i = 1; i < classes; i++) cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; i--) p[--cnt[c[pn[i]]]] = pn[i];
    cn[p[0]] = 0;
    classes = 1;
    for (int i = 1; i < n; i++) {
      pair<int, int> a = {c[p[i]], c[(p[i] + step) % n]};
      pair<int, int> b = {c[p[i - 1]], c[(p[i - 1] + step) % n]};
      if (a != b) classes++;
      cn[p[i]] = classes - 1;
    }
    c.swap(cn);
  }
  p.erase(p.begin());  // quitar el centinela
  return p;
}
vector<int> lcp_kasai(const string& s, const vector<int>& sa) {
  int n = s.size(), k = 0;
  vector<int> rnk(n), lcp(max(n - 1, 0));
  for (int i = 0; i < n; i++) rnk[sa[i]] = i;
  for (int i = 0; i < n; i++) {
    if (rnk[i] == n - 1) { k = 0; continue; }
    int j = sa[rnk[i] + 1];
    while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
    lcp[rnk[i]] = k;
    if (k > 0) k--;
  }
  return lcp;
}
