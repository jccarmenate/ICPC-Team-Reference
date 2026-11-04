// Manacher: todos los palindromos en O(n).
// odd[i] = k: el mayor palindromo impar centrado en i es
//            s[i-k+1 .. i+k-1] (largo 2k - 1).
// even[i] = k:   el mayor palindromo par centrado entre i-1 e i
//            es s[i-k .. i+k-1] (largo 2k).
// Cantidad total de substrings palindromos = suma de ambos.
void manacher(const string& s, vector<int>& odd,
              vector<int>& even) {
  int n = s.size();
  odd.assign(n, 0);
  even.assign(n, 0);
  for (int i = 0, l = 0, r = -1; i < n; i++) {
    int k = i > r ? 1 : min(odd[l + r - i], r - i + 1);
    while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
    odd[i] = k;
    if (i + k - 1 > r) {
      l = i - k + 1;
      r = i + k - 1;
    }
  }
  for (int i = 0, l = 0, r = -1; i < n; i++) {
    int k = i > r ? 0 : min(even[l + r - i + 1], r - i + 1);
    while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k])
      k++;
    even[i] = k;
    if (i + k - 1 > r) {
      l = i - k;
      r = i + k - 1;
    }
  }
}
