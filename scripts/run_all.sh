rm result.txt
for i in 1 2 4 8 16 32 64 128 256 512 1024 2048; do
  bin/aho_corasic all files/common_english_words.txt files/english_words.txt $i 10000000 0 F >> result.txt
done