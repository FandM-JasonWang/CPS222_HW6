all: rbt-tests first-step

rbt-tests: RedBlackTree.cpp RedBlackTreeTests.cpp
	g++ -Wall -O3 -std=c++17 -g RedBlackTree.cpp RedBlackTreeTests.cpp -o rbt-tests

first-step: RedBlackTree.cpp RedBlackTreeTestsFirstStep.cpp
	g++ -Wall -O3 -std=c++17 -g RedBlackTree.cpp RedBlackTreeTestsFirstStep.cpp -o first-step

clean:
	rm -f rbt-tests first-step *.o

.PHONY: all clean

