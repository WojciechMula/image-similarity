CPPFLAGS=$(CXXFLAGS) -O3 -Wall -Wextra -pedantic -std=c++11 -march=native

all: test

find: find.cpp
	$(CXX) $(CPPFLAGS) $< -o $@

unittest: unittest.cpp sse.c
	$(CXX) $(CPPFLAGS) $< -o $@

parameters.txt: find
	./find > $@

sse.c: scripts/generate.py scripts/patterns.py parameters.txt
	python scripts/generate.py parameters.txt $@

test: sse.c
	$(CXX) -c -march=native $<
