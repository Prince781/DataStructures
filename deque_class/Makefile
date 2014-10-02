# deque - Makefile
CXX=g++
CXXFLAGS=-g -ggdb -O0
BIN=deque_test

all: deque.h deque_main.cpp
	$(CXX) $(CXXFLAGS) deque_main.cpp -o $(BIN)

.PHONY: clean
clean: $(BIN)
	$(RM) $(BIN)
