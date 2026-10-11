// Aho-Corasick (letras 'a'..'z'): busca muchos patrones a la vez.
// Uso: insertar() cada patron, build(), luego contar(texto).
// O(suma de largos * 26 + |texto|).
struct AhoCorasick {
  struct Nodo {
    array<int, 26> next;
    int link = 0;
    Nodo() { next.fill(-1); }
  };
  vector<Nodo> t;
  vector<int> orden;        // nodos en orden BFS (sin la raiz)
  vector<int> nodo_patron;  // nodo donde termina cada patron
  AhoCorasick() : t(1) {}
  int insertar(const string& s) {  // retorna el id del patron
    int v = 0;
    for (char ch : s) {
      int c = ch - 'a';
      if (t[v].next[c] == -1) {
        int nuevo = t.size();
        t.emplace_back();
        t[v].next[c] = nuevo;
      }
      v = t[v].next[c];
    }
    nodo_patron.push_back(v);
    return (int)nodo_patron.size() - 1;
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
      orden.push_back(v);
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
  vector<long long> contar(const string& texto) {
    vector<long long> visitas(t.size(), 0);
    int v = 0;
    for (char ch : texto) {
      v = t[v].next[ch - 'a'];
      visitas[v]++;
    }
    for (int i = (int)orden.size() - 1; i >= 0; i--)
      visitas[t[orden[i]].link] += visitas[orden[i]];
    vector<long long> res;
    for (int nodo : nodo_patron) res.push_back(visitas[nodo]);
    return res;
  }
};
