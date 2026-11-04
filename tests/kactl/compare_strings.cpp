// Compara los algoritmos de strings del notebook contra KACTL.
#include "kactl_begin.h"
namespace kstr {
#include "strings/KMP.h"
#include "strings/Zfunc.h"
#include "strings/Manacher.h"
#include "strings/SuffixArray.h"
}
namespace kaho {
#include "AhoCorasickLower.h"  // parche: alfabeto 'a'..'z'
}
#include "kactl_end.h"
#include "../../codigo/strings/hashing.cpp"
#include "../../codigo/strings/kmp.cpp"
#include "../../codigo/strings/z.cpp"
#include "../../codigo/strings/manacher.cpp"
#include "../../codigo/strings/suffix_array.cpp"
#include "../../codigo/strings/suffix_automaton.cpp"
#include "../../codigo/strings/aho_corasick.cpp"

string random_string(int minlen, int maxlen, int letters) {
  string s(rnd(minlen, maxlen), 'a');
  for (char& c : s) c = 'a' + rnd(0, letters - 1);
  return s;
}

void t_kmp_z_manacher() {
  for (int it = 0; it < 20000; it++) {
    string s = random_string(0, 40, rnd(1, 3)), pat = random_string(1, 5, 2);
    int n = s.size();
    CHECK(prefix_function(s) == kstr::pi(s));
    CHECK(kmp_search(s, pat) == kstr::match(s, pat));
    vector<int> z = z_function(s), kz = kstr::Z(s);
    for (int i = 1; i < n; i++) CHECK(z[i] == kz[i]);
    vector<int> odd, even;
    manacher(s, odd, even);
    auto p = kstr::manacher(s);
    for (int i = 0; i < n; i++) CHECK(odd[i] - 1 == p[1][i] && even[i] == p[0][i]);
  }
  for (string s : {string(1000000, 'a'), random_string(1000000, 1000000, 2)}) {
    vector<int> a, b;
    double t1 = measure([&] { a = prefix_function(s); });
    double t2 = measure([&] { b = kstr::pi(s); });
    CHECK(a == b);
    report(string("KMP prefijo, 1e6 ") + (s[1] == 'a' && s[2] == 'a' ? "'aaa..'" : "aleatorio"), t1, t2);
    double t3 = measure([&] { a = z_function(s); });
    double t4 = measure([&] { b = kstr::Z(s); });
    for (int i = 1; i < (int)s.size(); i++) CHECK(a[i] == b[i]);
    report("Funcion Z, 1e6", t3, t4);
    vector<int> odd, even;
    array<vi, 2> p;
    double t5 = measure([&] { manacher(s, odd, even); });
    double t6 = measure([&] { p = kstr::manacher(s); });
    for (int i = 0; i < (int)s.size(); i++) CHECK(odd[i] - 1 == p[1][i] && even[i] == p[0][i]);
    report("Manacher, 1e6", t5, t6);
  }
}

void t_suffix_structures() {
  for (int it = 0; it < 5000; it++) {
    string s = random_string(1, 40, rnd(1, 4));
    int n = s.size();
    vector<int> sa = suffix_array(s), lcp = lcp_kasai(s, sa);
    kstr::SuffixArray k(s);
    for (int i = 0; i < n; i++) CHECK(sa[i] == k.sa[i + 1]);
    for (int i = 0; i + 1 < n; i++) CHECK(lcp[i] == k.lcp[i + 2]);
    long long distinct = 1LL * n * (n + 1) / 2;
    for (int i = 2; i <= n; i++) distinct -= k.lcp[i];
    SuffixAutomaton sam(s);
    CHECK(sam.distinct() == distinct);
    sam.compute_occurrences();
    for (int q = 0; q < 5; q++) {
      string t = random_string(1, 4, 3);
      CHECK(sam.occurrences(t) == (long long)kstr::match(s, t).size());
    }
  }
  for (string s : {string(200000, 'a'), random_string(200000, 200000, 26)}) {
    vector<int> sa, lcp;
    double t1 = measure([&] { sa = suffix_array(s); lcp = lcp_kasai(s, sa); });
    kstr::SuffixArray* k = nullptr;
    double t2 = measure([&] { k = new kstr::SuffixArray(s); });
    for (int i = 0; i < (int)s.size(); i++) CHECK(sa[i] == k->sa[i + 1]);
    delete k;
    report(string("Suffix Array + LCP, 2e5 ") + (s[1] == 'a' && s[2] == 'a' ? "'aaa..'" : "aleatorio"), t1, t2);
    double t3 = measure([&] { SuffixAutomaton sam(s); CHECK(sam.distinct() > 0); });
    report("Suffix Automaton (vs SA de KACTL), 2e5", t3, t2);
  }
}

void t_aho() {
  for (int it = 0; it < 5000; it++) {
    int letters = rnd(1, 3), k = rnd(1, 8);
    vector<string> pats(k);
    AhoCorasick ac;
    for (auto& p : pats) { p = random_string(1, 5, letters); ac.insert(p); }
    ac.build();
    kaho::AhoCorasick kac(pats);
    string text = random_string(0, 40, letters);
    vector<long long> got = ac.count_matches(text), expected(k, 0);
    for (auto& v : kac.findAll(pats, text))
      for (int id : v) expected[id]++;
    CHECK(got == expected);
  }
  vector<string> pats(2000);
  for (auto& p : pats) p = random_string(1, 20, 26);
  string text = random_string(500000, 500000, 26);
  vector<long long> got, expected(pats.size(), 0);
  double t1 = measure([&] {
    AhoCorasick ac;
    for (auto& p : pats) ac.insert(p);
    ac.build();
    got = ac.count_matches(text);
  });
  double t2 = measure([&] {
    kaho::AhoCorasick kac(pats);
    for (auto& v : kac.findAll(pats, text))
      for (int id : v) expected[id]++;
  });
  CHECK(got == expected);
  report("Aho-Corasick, 2000 patrones, texto 5e5", t1, t2);
}

// Cadenas de Thue-Morse: rompen el hashing modulo 2^64 (lo advierte
// KACTL). El hashing del notebook (modulo 2^61 - 1, base aleatoria)
// debe distinguir todos los substrings.
void t_hash_thue_morse() {
  string s(1 << 14, 'a');
  for (int i = 0; i < (int)s.size(); i++) s[i] = 'a' + __builtin_popcount(i) % 2;
  Hashing h(s);
  for (int len : {64, 256, 1024, 4096}) {
    map<long long, int> seen;
    map<string, long long> by_text;
    for (int i = 0; i + len <= (int)s.size(); i++) {
      long long v = h.get(i, i + len);
      string sub = s.substr(i, len);
      auto it = seen.find(v);
      if (it != seen.end()) CHECK(s.compare(it->second, len, sub) == 0);  // sin colisiones
      else seen[v] = i;
      auto jt = by_text.find(sub);
      if (jt != by_text.end()) CHECK(jt->second == v);
      else by_text[sub] = v;
    }
  }
}

int main() {
  t_kmp_z_manacher();
  t_suffix_structures();
  t_aho();
  t_hash_thue_morse();
  printf("OK strings (igual que KACTL)\n");
  print_timings();
}
