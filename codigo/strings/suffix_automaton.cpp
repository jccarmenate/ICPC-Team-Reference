// Suffix Automaton (letras 'a'..'z'). Construccion O(n * 26).
// Es un DAG donde cada camino desde el estado 0 es un substring
// distinto, asi que muchas preguntas son una DP sobre el DAG.
// A lo sumo 2n estados. Cada estado v representa los substrings
// de largo (len[link[v]], len[v]] con el mismo conjunto de
// posiciones finales.
struct SuffixAutomaton {
  struct Estado {
    int len = 0, link = -1;
    long long cnt = 0;  // apariciones (tras calcular_ocurrencias)
    array<int, 26> next;
    Estado() { next.fill(-1); }
  };
  vector<Estado> st;
  int ultimo = 0;
  SuffixAutomaton(const string& s = "") : st(1) {
    for (char c : s) extender(c);
  }
  void extender(char ch) {
    int c = ch - 'a', cur = st.size();
    st.emplace_back();
    st[cur].len = st[ultimo].len + 1;
    st[cur].cnt = 1;
    int p = ultimo;
    while (p != -1 && st[p].next[c] == -1) {
      st[p].next[c] = cur;
      p = st[p].link;
    }
    if (p == -1) {
      st[cur].link = 0;
    } else {
      int q = st[p].next[c];
      if (st[p].len + 1 == st[q].len) {
        st[cur].link = q;
      } else {
        int clon = st.size();
        Estado copia = st[q];
        copia.len = st[p].len + 1;
        copia.cnt = 0;
        st.push_back(copia);
        while (p != -1 && st[p].next[c] == q) {
          st[p].next[c] = clon;
          p = st[p].link;
        }
        st[q].link = clon;
        st[cur].link = clon;
      }
    }
    ultimo = cur;
  }
  // Estados por len creciente = orden topologico del DAG.
  vector<int> por_len() {
    vector<int> orden(st.size());
    iota(orden.begin(), orden.end(), 0);
    sort(orden.begin(), orden.end(),
         [&](int a, int b) { return st[a].len < st[b].len; });
    return orden;
  }
  // Deja en cnt[v] cuantas veces aparecen los substrings de v.
  void calcular_ocurrencias() {
    vector<int> orden = por_len();
    for (int i = (int)orden.size() - 1; i > 0; i--)
      st[st[orden[i]].link].cnt += st[orden[i]].cnt;
  }
  long long ocurrencias(const string& t) {
    int v = 0;
    for (char ch : t) {
      v = st[v].next[ch - 'a'];
      if (v == -1) return 0;
    }
    return st[v].cnt;
  }
  // Cantidad de substrings distintos (no vacios).
  long long distintos() {
    long long r = 0;
    for (int v = 1; v < (int)st.size(); v++)
      r += st[v].len - st[st[v].link].len;
    return r;
  }
  // DP en el DAG: dp[v] = caminos que salen de v (incluye vacio).
  vector<long long> caminos() {
    vector<int> orden = por_len();
    vector<long long> dp(st.size(), 1);
    for (int i = (int)orden.size() - 1; i >= 0; i--)
      for (int u : st[orden[i]].next)
        if (u != -1) dp[orden[i]] += dp[u];
    return dp;
  }
  // k-esimo substring distinto en orden lexicografico (k >= 1).
  // Retorna "" si hay menos de k.
  string kesimo(long long k) {
    vector<long long> dp = caminos();
    if (k > dp[0] - 1) return "";
    string res;
    int v = 0;
    while (k > 0)
      for (int c = 0; c < 26; c++) {
        int u = st[v].next[c];
        if (u == -1) continue;
        if (k <= dp[u]) {
          res += (char)('a' + c);
          v = u;
          k--;
          break;
        }
        k -= dp[u];
      }
    return res;
  }
  // Largo del substring comun mas largo con t. O(|t|).
  int lcs(const string& t) {
    int v = 0, l = 0, mejor = 0;
    for (char ch : t) {
      int c = ch - 'a';
      while (v != 0 && st[v].next[c] == -1) {
        v = st[v].link;
        l = st[v].len;
      }
      if (st[v].next[c] != -1) {
        v = st[v].next[c];
        l++;
      }
      mejor = max(mejor, l);
    }
    return mejor;
  }
};
