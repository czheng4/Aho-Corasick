/**
 * ChaoHui Zheng
 * 09/17/2021
 * The implementation is based on https://blogs.asarkar.com/assets/docs/algorithms-curated/Aho-Corasick%20Automata%20-%20Stanford.pdf
 * The problem is you are given patterns P1, P2, P3, P4 ... and text string, you need to find all occurrence of patterns in the text.
 */

#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <ctime>
#include <sys/time.h>
using namespace std;

#define MAX_CHILDREN 256

/* get the clock time */
double get_real_time() {
  struct timeval tv;
  double rv;
  gettimeofday(&tv, NULL);
  rv = tv.tv_usec;
  rv /= 1000000.0;
  rv += tv.tv_sec;
  return rv;
}

void error_exit(const string &s) {
  cerr << s << endl;
  exit(1);
}


/* naive approach to match all patterns */
void find(const vector<string> &patterns, const string &text, bool print) {
  double t;
  size_t i,j;
  int occurrence;
  occurrence = 0;
  t = get_real_time();

  for (i = 0; i < patterns.size(); i++) {
    j = 0;
    while ((j = text.find(patterns[i], j)) != std::string::npos) {
      if (print) printf("%s:%d\n", patterns[i].c_str(), (int) j);
      j += 1;
      occurrence++;
     
    }
  }
  printf("Find - occurrence: %d    Time: %lf\n", occurrence, get_real_time() - t);
}

class Node {
public:
  Node(int l, char data);
  Node *children[MAX_CHILDREN]; /* The children nodes */
  unsigned char c;              /* data */
  int level;                    /* the level in the tree */
  bool is_word;                 /* true if this is a terminate state (It's a pattern) */
  Node *failure_link;           /* failure link/suffix link */
  Node *dictionary_link;        /* dictionary link/output link */
};

Node::Node(int l, char data) {
  int i;
  failure_link = nullptr;
  dictionary_link = nullptr;
  level = l;
  c = data;
  is_word = false;
  for (i = 0; i < MAX_CHILDREN; i++) children[i] = nullptr;
}




class Aho {
public:

  Aho(const vector<string> &patterns);
  void BuildTrie(const vector<string> &patterns);
  void PrintConnections();
  void PrintPatterns(const Node *n, string s);
  void MatchPatterns(const string &text, bool print);
  void MatchPatternsWithTriesOnly(const string &text, bool print);
  Node *root;
};


Aho::Aho(const vector<string> &patterns) {
  BuildTrie(patterns);
}

void Aho::MatchPatternsWithTriesOnly(const string &text, bool print) {
  int i, j;
  Node *n;
  int occurrence;
  unsigned char c;
  double t;

  occurrence = 0;
  t = get_real_time();
  for (i = 0; i < (int) text.size(); i++) {
    n = root;
    j = i;

    do {
      if (n->is_word) {
        occurrence++;
        if (print) printf("%s:%d\n", text.substr(i, n->level).c_str(), i);
      }
      c = text[j];
      n = n->children[c];
      j++;

    } while (n != nullptr && j < (int) text.size());

  }

 printf("Trie - occurrence: %d    Time: %lf\n", occurrence, get_real_time() - t);

}

void Aho::MatchPatterns(const string &text, bool print) {
  int i;
  Node *n, *dn;
  unsigned char c;
  double t;
  int occurrence;

  occurrence = 0;
  n = root;

  t = get_real_time();
  for (i = 0; i < (int) text.size(); i++) {

    c = text[i];
    // cout << i << " " << text.size() << " " << (int)c << endl;
    
    while (n->children[c] == nullptr && n != root) {
      n = n->failure_link;
    }

    if (n->children[c] != nullptr) {
      n = n->children[c];
      // cout << n->level << " " << n->c << endl;
      if (n->is_word) {
        occurrence++;
        // cout << "level: " << i << " " << n->level << endl;
        if (print) printf("%s:%d\n", text.substr(i - n->level + 1, n->level).c_str(), i - n->level + 1);
      }
      dn = n;
      while (dn->dictionary_link != nullptr) {
        occurrence++;
        dn = dn->dictionary_link;
        if (print) printf("%s:%d\n", text.substr(i - dn->level + 1, dn->level).c_str(), i - dn->level + 1);
      }
    }
  }
  printf("Aho - occurrence: %d    Time: %lf\n", occurrence, get_real_time() - t);
}

void Aho::BuildTrie(const vector<string> &patterns) {
  string s;
  unsigned char c;
  size_t i, j;
  Node *n, *cn, *fn;
  queue <Node*> q;


  root = new Node(0, '\0');

  /* Build tries first - 
     we traverse from root. If the "to" node does not exist, we create a new one 
  */

  for (i = 0; i < patterns.size(); i++) {
    s = patterns[i];
    // cout << s << endl;
    n = root;
    for (j = 0; j < s.size(); j++) {
      c = s[j];
      if (n->children[c] == nullptr) n->children[c] = new Node(j + 1, c);
      n = n->children[c];
    }
    n->is_word = true;
  }

  root->failure_link = root;

  /* 
    If the node at level 1(one below root), it has failure_link pointing to root. 
  */
  for (i = 0; i < MAX_CHILDREN; i++) {
    if (root->children[i] != nullptr) {
      root->children[i]->failure_link = root;
      q.push(root->children[i]);
    }
  }


  /* If n->failure_link has children i, cn's failure link is n->failure_link[i]
     otherwise, if n->failure_link is root, cn's failure link is root
     otherwise, we follow n->failure_link to next one, and repeat this process */
  while (!q.empty()) {
    n = q.front();
    q.pop();
    for (i = 0; i < MAX_CHILDREN; i++) {

      cn = n->children[i];
      if (cn != nullptr) {
        q.push(cn);
        fn = n->failure_link;
        while (cn->failure_link == nullptr) {
          if (fn->children[i] != nullptr) {
            cn->failure_link = fn->children[i];
          } else if (fn == root) {
            cn->failure_link = root;
          } else {
            fn = fn->failure_link;
          }
        }
        /* build dictionary link. */
        fn = cn->failure_link;
        if (fn->is_word) cn->dictionary_link = fn;
        else cn->dictionary_link = fn->dictionary_link;
      }
    }
  }

  // PrintConnections();
  // PrintPatterns(root, "");
}

void Aho::PrintConnections() {

  int i;

  queue <Node*> q;
  Node *n, *cn;

  q.push(root);
  while (!q.empty()) {
    n = q.front();
    q.pop();
    for (i = 0; i < MAX_CHILDREN; i++) {
      cn = n->children[i];
      if (cn != nullptr) {
        q.push(cn);

        printf("(%d,%c) -> failure_link (%d, %c)", cn->level, cn->c, cn->failure_link->level, cn->failure_link->c);
        if (cn->dictionary_link != nullptr) printf(" -> dictionary_link (%d, %c)", cn->dictionary_link->level, cn->dictionary_link->c);
        printf("\n");
      }
    }
  }

}

void Aho::PrintPatterns(const Node *n, string s) {
  int i;
  Node *cn;
  if (n->is_word) cout << s << endl;
  for (i = 0; i < MAX_CHILDREN; i++) {
    cn = n->children[i];

    if (cn != nullptr) {
      s.push_back(cn->c);
      PrintPatterns(cn, s);
      s.pop_back();
    }
  }
  // PrintStringInTrieRecursion(n);
}


int main(int argc, char **argv) {

  vector <string> patterns;
  vector <string> words;
  string text, s;
  string a;
  ifstream fin;
  Aho *aho;
  int i, seed, num_patterns, num_words, rn;
  bool print;

  if (argc != 8) {
    error_exit("bin/aho_corasick [aho|trie|find|all] pattern_bank word_bank num_patterns num_words seed print[T|F]");
  }

  a = (string) argv[1];
  if (a != "aho" && a != "trie" && a != "all" && a != "find") error_exit("argv[1] must be one of aho|trie|find|all");

  /* read pattern file */
  fin.open(argv[2]);
  if (fin.fail()) error_exit("Couldn't open pattern bank file " + string(argv[2]));
  while (fin >> s) {
    if (s.size() > 3) patterns.push_back(s);
  }
  fin.close();


  /* read word file */
  fin.open(argv[3]);
  if (fin.fail()) error_exit("Couldn't open text bank file " + string(argv[3]));
  while (fin >> s) words.push_back(s);
  fin.close();


  /* read other argv */
  if (sscanf(argv[4], "%d", &num_patterns) != 1 && (
      num_patterns < 0 || num_patterns > (int) patterns.size() )) {
    error_exit("num_patterns " +  string(argv[4]) + "is not valid");
  }

  if (sscanf(argv[5], "%d", &num_words) != 1 && num_words < 0 ) {
    error_exit("num_words " +  string(argv[5]) + "is not valid");
  }

  if (sscanf(argv[6], "%d", &seed) != 1 && seed < 0 ) {
    error_exit("seed " +  string(argv[6]) + "is not valid");
  }


  /* shuffle the patterns and then keep the first "num_patterns" one */
  srand(seed);
  for (i = 0; i < (int) patterns.size(); i++) {
    rn = rand() % (i + 1);
    s = patterns[i];
    patterns[i] = patterns[rn];
    patterns[rn] = s;
  }
  patterns.resize(num_patterns);

  /* get text */
  text = "";
  for (i = 0; i < num_words; i++) {
    text += words[rand() % words.size()];
  }


  s = (string) argv[7];
  print = s == "T";
  if (a == "all") {
    aho = new Aho(patterns);
    aho->MatchPatterns(text, print);
    aho->MatchPatternsWithTriesOnly(text, print);
    find(patterns, text, print);
    delete aho;
  } else if (a == "aho") {
    aho = new Aho(patterns);
    aho->MatchPatterns(text, print);
    delete aho;
  } else if (a == "trie") {
    aho = new Aho(patterns);
    aho->MatchPatternsWithTriesOnly(text, print);
    delete aho;
  } else {
    find(patterns, text, print);
  }

}