// Funcion Z. O(n). z[i] = largo del mayor prefijo comun entre s
// y s[i..]. Por convencion z[0] = n.
// Buscar pat en texto: calcular Z de pat + "#" + texto y mirar
// las posiciones con z[i] == largo de pat.
vector<int> funcion_z(const string& s) {
  int n = s.size();
  vector<int> z(n, 0);
  if (n > 0) z[0] = n;
  for (int i = 1, l = 0, r = 0; i < n; i++) {
    if (i < r) z[i] = min(r - i, z[i - l]);
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
    if (i + z[i] > r) {
      l = i;
      r = i + z[i];
    }
  }
  return z;
}
