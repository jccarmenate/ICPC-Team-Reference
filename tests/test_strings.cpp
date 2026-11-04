#include "comun.h"
#include "../codigo/strings/hashing.cpp"
#include "../codigo/strings/kmp.cpp"
#include "../codigo/strings/z.cpp"
#include "../codigo/strings/manacher.cpp"
#include "../codigo/strings/suffix_automaton.cpp"
#include "../codigo/strings/suffix_array.cpp"
#include "../codigo/strings/aho_corasick.cpp"

string random_string(int minlen, int maxlen, int letters) {
  string s(rnd(minlen, maxlen), 'a');
  for (char& c : s) c = 'a' + rnd(0, letters - 1);
  return s;
}
vector<int> brute_occurrences(const string& text, const string& pat) {
  vector<int> res;
  for (size_t i = 0; i + pat.size() <= text.size(); i++)
    if (text.compare(i, pat.size(), pat) == 0) res.push_back(i);
  return res;
}

void t_hashing() {
  for (int it = 0; it < 400; it++) {
    string s = random_string(1, 25, 2), t = random_string(1, 25, 2);
    Hashing hs(s), ht(t);
    int n = s.size(), m = t.size();
    for (int l = 0; l < n; l++)
      for (int r = l; r <= n; r++) {
        for (int l2 = 0; l2 + (r - l) <= n; l2++)
          CHECK((hs.get(l, r) == hs.get(l2, l2 + r - l)) ==
                (s.compare(l, r - l, s, l2, r - l) == 0));
        for (int l2 = 0; l2 + (r - l) <= m; l2++)
          CHECK((hs.get(l, r) == ht.get(l2, l2 + r - l)) ==
                (s.compare(l, r - l, t, l2, r - l) == 0));
      }
  }
}

void t_kmp_z() {
  for (int it = 0; it < 5000; it++) {
    string s = random_string(0, 30, rnd(1, 3));
    int n = s.size();
    vector<int> pi = prefix_function(s), z = z_function(s);
    for (int i = 0; i < n; i++) {
      int best = 0;
      for (int k = 1; k <= i; k++)
        if (s.compare(0, k, s, i - k + 1, k) == 0) best = k;
      CHECK(pi[i] == best);
      int length = 0;
      while (i + length < n && s[length] == s[i + length]) length++;
      CHECK(z[i] == length);
    }
    string pat = random_string(0, 4, 2);
    if (pat.empty()) CHECK(kmp_search(s, pat).empty());
    else CHECK(kmp_search(s, pat) == brute_occurrences(s, pat));
  }
}

void t_manacher() {
  for (int it = 0; it < 5000; it++) {
    string s = random_string(0, 30, rnd(1, 3));
    int n = s.size();
    vector<int> odd, even;
    manacher(s, odd, even);
    for (int i = 0; i < n; i++) {
      int k = 1;
      while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
      CHECK(odd[i] == k);
      k = 0;
      while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
      CHECK(even[i] == k);
    }
  }
}

void t_suffix_automaton() {
  for (int it = 0; it < 3000; it++) {
    string s = random_string(0, 25, rnd(1, 3));
    int n = s.size();
    SuffixAutomaton sa(s);
    CHECK((int)sa.st.size() <= max(2 * n, 2));
    set<string> subs;
    for (int i = 0; i < n; i++)
      for (int l = 1; i + l <= n; l++) subs.insert(s.substr(i, l));
    CHECK(sa.distinct() == (long long)subs.size());
    CHECK(sa.paths()[0] == (long long)subs.size() + 1);
    // k-esimo en orden lexicografico
    long long k = 1;
    for (const string& sub : subs) {
      if (k <= 15 || k + 3 > (long long)subs.size()) CHECK(sa.kth(k) == sub);
      k++;
    }
    CHECK(sa.kth(subs.size() + 1) == "");
    // ocurrencias
    sa.compute_occurrences();
    for (const string& sub : subs)
      CHECK(sa.occurrences(sub) == (long long)brute_occurrences(s, sub).size());
    for (int q = 0; q < 10; q++) {
      string t = random_string(1, 6, 3);
      CHECK(sa.occurrences(t) == (long long)brute_occurrences(s, t).size());
      // substring comun mas largo
      string u = random_string(0, 25, 3);
      int best = 0;
      for (size_t i = 0; i < u.size(); i++)
        for (size_t l = 1; i + l <= u.size(); l++)
          if (subs.count(u.substr(i, l))) best = max(best, (int)l);
      CHECK(sa.lcs(u) == best);
    }
  }
}

void t_suffix_array() {
  for (int it = 0; it < 5000; it++) {
    string s = random_string(0, 40, rnd(1, 4));
    if (it % 10 == 0) for (char& c : s) c = (char)rnd(1, 255);  // bytes
    int n = s.size();
    vector<int> expected(n);
    iota(expected.begin(), expected.end(), 0);
    auto suffix_less = [&](int a, int b) {  // comparacion sin signo de sufijos
      for (; a < n && b < n; a++, b++)
        if (s[a] != s[b]) return (unsigned char)s[a] < (unsigned char)s[b];
      return a > b;  // el sufijo mas corto va primero
    };
    sort(expected.begin(), expected.end(), suffix_less);
    vector<int> sa = suffix_array(s);
    CHECK(sa == expected);
    vector<int> lcp = lcp_kasai(s, sa);
    CHECK((int)lcp.size() == max(n - 1, 0));
    long long total = 0;
    for (int i = 0; i + 1 < n; i++) {
      int k = 0;
      while (sa[i] + k < n && sa[i + 1] + k < n && s[sa[i] + k] == s[sa[i + 1] + k]) k++;
      CHECK(lcp[i] == k);
      total += k;
    }
    set<string> subs;
    for (int i = 0; i < n; i++)
      for (int l = 1; i + l <= n; l++) subs.insert(s.substr(i, l));
    CHECK((long long)subs.size() == 1LL * n * (n + 1) / 2 - total);
  }
}

void t_aho() {
  for (int it = 0; it < 4000; it++) {
    int letters = rnd(1, 3), k = rnd(1, 8);
    AhoCorasick ac;
    vector<string> patterns;
    for (int i = 0; i < k; i++) {
      patterns.push_back(random_string(1, 5, letters));
      CHECK(ac.insert(patterns.back()) == i);
    }
    ac.build();
    for (int q = 0; q < 3; q++) {
      string text = random_string(0, 40, letters);
      vector<long long> res = ac.count_matches(text);
      for (int i = 0; i < k; i++)
        CHECK(res[i] == (long long)brute_occurrences(text, patterns[i]).size());
    }
  }
}

int main() {
  t_hashing();
  t_kmp_z();
  t_manacher();
  t_suffix_automaton();
  t_suffix_array();
  t_aho();
  printf("OK strings\n");
}
