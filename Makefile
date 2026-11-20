.PHONY: all clean

all: rbt-tests first-step speed-test

rbt-tests: RedBlackTree.cpp RedBlackTreeTests.cpp
	g++ -Wall -std=c++17 -g -O3 RedBlackTree.cpp RedBlackTreeTests.cpp -o rbt-tests

first-step: RedBlackTree.cpp RedBlackTreeTestsFirstStep.cpp
	g++ -Wall -std=c++17 -g -O3 RedBlackTree.cpp RedBlackTreeTestsFirstStep.cpp -o first-step

speed-test: RedBlackTree.cpp SpeedTest.cpp
	g++ -Wall -std=c++17 -g -O3 RedBlackTree.cpp SpeedTest.cpp -o speed-test

clean:
	rm -f rbt-tests first-step speed-test *.o
