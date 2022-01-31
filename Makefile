CXX = g++
CPPFLAGS = -Wall -g

all: bin/.dirstamp bin/main


bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

bin/main: bin/BitBoard.o
	$(CXX) $(CPPFLAGS) main.cpp bin/BitBoard.o -o bin/main

bin/BitBoard.o:
	$(CXX) $(CPPFLAGS) -c BitBoard.cpp -o bin/BitBoard.o  

.PHONY: clean
clean:
	rm -rf bin