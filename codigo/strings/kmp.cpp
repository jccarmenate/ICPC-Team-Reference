// KMP. O(n + m).
// prefijo(s)[i] = largo del mayor prefijo propio de s[0..i] que
// tambien es sufijo de s[0..i].
vector<int> prefijo(const string& s) {
  int n = s.size();
  vector<int> pi(n, 0);
  for (int i = 1; i < n; i++) {
    int j = pi[i - 1];
    while (j > 0 && s[i] != s[j]) j = pi[j - 1];
    if (s[i] == s[j]) j++;
    pi[i] = j;
  }
  return pi;
}
// Posiciones iniciales de todas las apariciones de pat en texto.
vector<int> buscar(const string& texto, const string& pat) {
  vector<int> pi = prefijo(pat), res;
  int m = pat.size(), j = 0;
  if (m == 0) return res;
  for (int i = 0; i < (int)texto.size(); i++) {
    while (j > 0 && texto[i] != pat[j]) j = pi[j - 1];
    if (texto[i] == pat[j]) j++;
    if (j == m) {
      res.push_back(i - m + 1);
      j = pi[j - 1];
    }
  }
  return res;
}
