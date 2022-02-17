CXX = g++
CPPFLAGS = -Wall -g

all: bin/.dirstamp bin/main

tests: bin/tests

bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

bin/main: bin/BitBoard.o
	$(CXX) $(CPPFLAGS) main.cpp bin/BitBoard.o -o bin/main

bin/BitBoard.o:
	$(CXX) $(CPPFLAGS) -c BitBoard.cpp -o bin/BitBoard.o  

bin/tests: ./tests/tests.cc
	$(CXX) -std=c++20 -g -Wall $^ -o $@

.PHONY: clean tests
clean:
	rm -rf bin