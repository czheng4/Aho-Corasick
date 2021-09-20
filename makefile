all: bin/aho_corasick

bin/aho_corasick: src/aho_corasick.cpp
	g++ -Wall -Wextra -o bin/aho_corasic src/aho_corasick.cpp

clean:
	rm bin/*