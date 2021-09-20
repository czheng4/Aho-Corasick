# Aho-Corasick

Aho-Corasick is multiple patterns matching algorithm whose matching time complexity is `O(n + m)` where `n` is the size of text and `m` is the number of occurrences.


## Compilation
```
UNIX> make
g++ -Wall -Wextra -o bin/aho_corasic src/aho_corasick.cpp
UNIX> bin/aho_corasick
bin/aho_corasick [aho|trie|find|all] pattern_bank word_bank num_patterns num_words seed print[T|F]
UNIX> bin/aho_corasic all files/common_english_words.txt files/english_words.txt 100 1000000 0 F
Aho - occurrence: 9458    Time: 0.435015
Trie - occurrence: 9458    Time: 0.617180
Find - occurrence: 9458    Time: 1.030039
```