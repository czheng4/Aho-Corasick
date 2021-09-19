grep "Find" result.txt | awk '{print $NF}' > find_time.txt
grep "Aho" result.txt | awk '{print $NF}' > aho_time.txt
grep "Trie" result.txt | awk '{print $NF}' > trie_time.txt