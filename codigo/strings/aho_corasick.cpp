// Aho-Corasick (letras 'a'..'z'): busca muchos patrones a la vez.
// Uso: insert() cada patron, build(), luego count_matches(text).
// O(suma de largos * 26 + |text|).
struct AhoCorasick {
  struct Node {
    array<int, 26> next;
    int link = 0;
    Node() { next.fill(-1); }
  };
  vector<Node> t;
  vector<int> order;        // nodos en orden BFS (sin la raiz)
  vector<int> pattern_node;  // nodo donde termina cada patron
  AhoCorasick() : t(1) {}
  int insert(const string& s) {  // retorna el id del patron
    int v = 0;
    for (char ch : s) {
      int c = ch - 'a';
      if (t[v].next[c] == -1) {
        int new_node = t.size();
        t.emplace_back();
        t[v].next[c] = new_node;
      }
      v = t[v].next[c];
    }
    pattern_node.push_back(v);
    return (int)pattern_node.size() - 1;
  }
  void build() {
    queue<int> q;
    for (int c = 0; c < 26; c++) {
      int u = t[0].next[c];
      if (u == -1) t[0].next[c] = 0;
      else q.push(u);
    }
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      order.push_back(v);
      for (int c = 0; c < 26; c++) {
        int u = t[v].next[c];
        if (u == -1) {
          t[v].next[c] = t[t[v].link].next[c];
        } else {
          t[u].link = t[t[v].link].next[c];
          q.push(u);
        }
      }
    }
  }
  // res[i] = cantidad de apariciones del patron i en el texto.
  vector<long long> count_matches(const string& text) {
    vector<long long> visits(t.size(), 0);
    int v = 0;
    for (char ch : text) {
      v = t[v].next[ch - 'a'];
      visits[v]++;
    }
    for (int i = (int)order.size() - 1; i >= 0; i--)
      visits[t[order[i]].link] += visits[order[i]];
    vector<long long> res;
    for (int node : pattern_node) res.push_back(visits[node]);
    return res;
  }
};
