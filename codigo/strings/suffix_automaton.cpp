// Suffix Automaton (letras 'a'..'z'). Construccion O(n * 26).
// Es un DAG donde cada camino desde el estado 0 es un substring
// distinto, asi que muchas preguntas son una DP sobre el DAG.
// A lo sumo 2n estados. Cada estado v representa los substrings
// de largo (len[link[v]], len[v]] con el mismo conjunto de
// posiciones finales.
struct SuffixAutomaton {
  struct State {
    int len = 0, link = -1;
    long long cnt = 0;  // apariciones (tras compute_occurrences)
    array<int, 26> next;
    State() { next.fill(-1); }
  };
  vector<State> st;
  int last = 0;
  SuffixAutomaton(const string& s = "") : st(1) {
    for (char c : s) extend(c);
  }
  void extend(char ch) {
    int c = ch - 'a', cur = st.size();
    st.emplace_back();
    st[cur].len = st[last].len + 1;
    st[cur].cnt = 1;
    int p = last;
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
        int clone = st.size();
        State cloned = st[q];
        cloned.len = st[p].len + 1;
        cloned.cnt = 0;
        st.push_back(cloned);
        while (p != -1 && st[p].next[c] == q) {
          st[p].next[c] = clone;
          p = st[p].link;
        }
        st[q].link = clone;
        st[cur].link = clone;
      }
    }
    last = cur;
  }
  // Estados por len creciente = orden topologico del DAG.
  vector<int> by_len() {
    vector<int> order(st.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(),
         [&](int a, int b) { return st[a].len < st[b].len; });
    return order;
  }
  // Deja en cnt[v] cuantas veces aparecen los substrings de v.
  void compute_occurrences() {
    vector<int> order = by_len();
    for (int i = (int)order.size() - 1; i > 0; i--)
      st[st[order[i]].link].cnt += st[order[i]].cnt;
  }
  long long occurrences(const string& t) {
    int v = 0;
    for (char ch : t) {
      v = st[v].next[ch - 'a'];
      if (v == -1) return 0;
    }
    return st[v].cnt;
  }
  // Cantidad de substrings distintos (no vacios).
  long long distinct() {
    long long r = 0;
    for (int v = 1; v < (int)st.size(); v++)
      r += st[v].len - st[st[v].link].len;
    return r;
  }
  // DP en el DAG: dp[v] = caminos que salen de v (incluye vacio).
  vector<long long> paths() {
    vector<int> order = by_len();
    vector<long long> dp(st.size(), 1);
    for (int i = (int)order.size() - 1; i >= 0; i--)
      for (int u : st[order[i]].next)
        if (u != -1) dp[order[i]] += dp[u];
    return dp;
  }
  // k-esimo substring distinto en orden lexicografico (k >= 1).
  // Retorna "" si hay menos de k.
  string kth(long long k) {
    vector<long long> dp = paths();
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
    int v = 0, l = 0, best = 0;
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
      best = max(best, l);
    }
    return best;
  }
};
