#include <iostream>
#include <cassert>
#include <random>
#include <vector>
#include <algorithm>
#include "RedBlackTree.h"

using namespace std;

// Resource Citations:
// 1. Red-Black Tree Visualizer: https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
// 2. TutorialsPoint RBT: https://www.tutorialspoint.com/insertion-in-the-red-black-tree-in-data-structure
// 3. zyBooks Section 16 (Red-Black Trees)

void TestSimpleConstructor(){
	cout << "Testing Simple Constructor... " << endl;
	RedBlackTree rbt = RedBlackTree();
	//cout << "empty r-b-tree: " << rbt.ToInfixString() << endl;
	assert(rbt.ToInfixString() == "");
	assert(rbt.Size() == 0);
	
	cout << "PASSED!"<< endl << endl;
}

void TestConstructor(){
	cout << "Testing Constructor W/Int Input..." << endl;
	RedBlackTree rbt = RedBlackTree(15);
	assert(rbt.ToPrefixString() == " B15 ");
	assert(rbt.Size() == 1);

	cout << "PASSED!"<< endl << endl;
}

void TestInsertFirstNode(){
	cout << "Testing Insert One Node..." << endl;
	RedBlackTree rbt = RedBlackTree();
	rbt.Insert(30);
	//cout << "rbt: " << rbt.ToPrefixString() << endl;
	assert(rbt.ToPrefixString() == " B30 ");
	assert(rbt.Size() == 1);

	cout << "PASSED!" << endl << endl;
}


void TestInsertSecondNode(){
	cout << "Testing Insert Second Node..." << endl;
	RedBlackTree *rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(15);
	assert(rbt->ToPrefixString() == " B30  R15 ");
	assert(rbt->Size() == 2);
	delete rbt;
	
	rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(45);
	assert(rbt->ToPrefixString() == " B30  R45 ");	
	assert(rbt->Size() == 2);
	delete rbt;

	cout << "PASSED!" << endl << endl;
}


void TestInsertThirdNode(){
	cout << "Testing Insert Third Node..." << endl;
	RedBlackTree *rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(15);
	rbt->Insert(10); // Left Left
	//cout << "rbt: "  << rbt->ToPrefixString() << endl;
	assert(rbt->ToPrefixString() == " B15  R10  R30 ");
	delete rbt;
	
	rbt = new RedBlackTree(); 
	rbt->Insert(30);
	rbt->Insert(15);
	rbt->Insert(25); // Left Right (zig-zag)
	assert(rbt->ToPrefixString() == " B25  R15  R30 ");
	delete rbt;
	
	rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(15);
	rbt->Insert(45); // Easy case (children on both sides)
	assert(rbt->ToPrefixString() == " B30  R15  R45 ");
	delete rbt;
	
	// Symmetric cases:
	// Right Right case: Line leaning right triggers RotateLeft
	rbt = new RedBlackTree();
	rbt->Insert(10);
	rbt->Insert(20);
	rbt->Insert(30);
	assert(rbt->ToPrefixString() == " B20  R10  R30 ");
	delete rbt;

	// Right Left case: Zig-zag leaning right triggers RotateRight then RotateLeft
	rbt = new RedBlackTree();
	rbt->Insert(10);
	rbt->Insert(30);
	rbt->Insert(20);
	assert(rbt->ToPrefixString() == " B20  R10  R30 ");
	delete rbt;

	// Symmetric easy case: Root with right child first, then left child
	rbt = new RedBlackTree();
	rbt->Insert(20);
	rbt->Insert(30);
	rbt->Insert(10);
	assert(rbt->ToPrefixString() == " B20  R10  R30 ");
	delete rbt;
	
	cout << "PASSED!" << endl << endl;
}

void TestInsertFourthNode(){
	cout << "Testing Insert Fourth Node..." << endl;

	// Case 1: Uncle is red -> Recoloring without rotation
	RedBlackTree *rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(15);
	rbt->Insert(45);
	rbt->Insert(10);
	assert(rbt->ToPrefixString() == " B30  B15  R10  B45 ");
	assert(rbt->Size() == 4);
	delete rbt;

	// Recolor with fourth node inserted on right side of right child
	rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(15);
	rbt->Insert(45);
	rbt->Insert(50);
	assert(rbt->ToPrefixString() == " B30  B15  B45  R50 ");
	assert(rbt->Size() == 4);
	delete rbt;

	// Ascending 4 nodes: 10, 20, 30, 40
	rbt = new RedBlackTree();
	rbt->Insert(10);
	rbt->Insert(20);
	rbt->Insert(30);
	rbt->Insert(40);
	assert(rbt->ToPrefixString() == " B20  B10  B30  R40 ");
	assert(rbt->Size() == 4);
	delete rbt;

	// Descending 4 nodes: 40, 30, 20, 10
	rbt = new RedBlackTree();
	rbt->Insert(40);
	rbt->Insert(30);
	rbt->Insert(20);
	rbt->Insert(10);
	assert(rbt->ToPrefixString() == " B30  B20  R10  B40 ");
	assert(rbt->Size() == 4);
	delete rbt;
	
	cout << "PASSED!" << endl << endl;
}

void TestInsertFifthNode(){
	cout << "Testing Insert Fifth Node..." << endl;
	RedBlackTree *rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(15);
	rbt->Insert(45);
	rbt->Insert(10);
	rbt->Insert(25);
	//cout << "result: "  << rbt->ToPrefixString() << endl;
	assert(rbt->ToPrefixString() == " B30  B15  R10  R25  B45 ");
	delete rbt;
	
	// Strictly increasing: 10, 20, 30, 40, 50
	rbt = new RedBlackTree();
	rbt->Insert(10);
	rbt->Insert(20);
	rbt->Insert(30);
	rbt->Insert(40);
	rbt->Insert(50);
	assert(rbt->ToPrefixString() == " B20  B10  B40  R30  R50 ");
	assert(rbt->ToInfixString() == " B10  B20  R30  B40  R50 ");
	assert(rbt->Size() == 5);
	delete rbt;

	// Strictly decreasing: 50, 40, 30, 20, 10
	rbt = new RedBlackTree();
	rbt->Insert(50);
	rbt->Insert(40);
	rbt->Insert(30);
	rbt->Insert(20);
	rbt->Insert(10);
	assert(rbt->ToPrefixString() == " B40  B20  R10  R30  B50 ");
	assert(rbt->ToInfixString() == " R10  B20  R30  B40  B50 ");
	assert(rbt->Size() == 5);
	delete rbt;
	
	cout << "PASSED!" << endl << endl;
}


void TestToStrings(){
	cout << "Testing ToString Methods..." << endl;

	RedBlackTree rbt = RedBlackTree();
	rbt.Insert(12);
	rbt.Insert(11);
	rbt.Insert(15);
	rbt.Insert(5);
	rbt.Insert(13);
	rbt.Insert(7);

	assert(rbt.ToPrefixString() == " B12  B7  R5  R11  B15  R13 ");
	assert(rbt.ToInfixString() == " R5  B7  R11  B12  R13  B15 ");
	assert(rbt.ToPostfixString() == " R5  R11  B7  R13  B15  B12 ");

	cout << "PASSED!" << endl << endl;
}

void TestInsertRandomTests(){
	cout << "Testing Random Insert Stuff..." << endl;
	cout << "\t This test passes if it doesn't crash and valgrind reports no issues" << endl;
	RedBlackTree *rbt = new RedBlackTree();
	rbt->Insert(15);
	rbt->Insert(13);
	rbt->Insert(20);
	rbt->Insert(12);
	cout << endl;
	//cout << "tree: " << rbt->ToInfixString() << endl;
	delete rbt;
	
	
	// probably should have a delete or something here
	rbt = new RedBlackTree();
	//cout << endl << "NEW TREE" << endl;
	rbt->Insert(12);
	//cout << "tree: "  << rbt->ToInfixString() << endl;
	rbt->Insert(11);
	//cout << "tree: "  << rbt->ToInfixString() << endl;
	rbt->Insert(15);
	//cout << "tree: "  << rbt->ToInfixString() << endl;
	rbt->Insert(5);
	//cout << "tree: "  << rbt->ToInfixString() << endl;
	rbt->Insert(13);
	//cout << "tree: "  << rbt->ToInfixString() << endl;
	rbt->Insert(7);
	//cout << "tree: "  << rbt->ToInfixString() << endl;
	delete rbt;
	
	
	rbt = new RedBlackTree();
	//cout << endl << "NEW TREE" << endl;
	rbt->Insert(12);
	//cout << "tree: "  << rbt->ToPrefixString() << endl;
	rbt->Insert(10);
	//cout << "tree: "  << rbt->ToPrefixString() << endl;
	rbt->Insert(8);
	//cout << "tree: "  << rbt->ToPrefixString() << endl;
	delete rbt;
	
	cout << "PASSED!" << endl << endl;
}

void TestCopyConstructor(){
	cout << "Testing Copy Constructor..." << endl;

	RedBlackTree rbt1 = RedBlackTree();
	rbt1.Insert(11);
	rbt1.Insert(23);
	rbt1.Insert(9);
	rbt1.Insert(52);
	rbt1.Insert(31);
	rbt1.Insert(4);

	assert(rbt1.ToPrefixString() == " B11  B9  R4  B31  R23  R52 ");

	RedBlackTree rbt2 = RedBlackTree(rbt1);

	assert(rbt2.ToPrefixString() == rbt1.ToPrefixString());
	assert(rbt2.Size() == rbt1.Size());

	rbt1.Insert(200);
	assert(rbt2.ToPrefixString() != rbt1.ToPrefixString());
	assert(rbt2.Size() != rbt1.Size());
	assert(!rbt2.Contains(200));

	cout << "PASSED!" << endl << endl;
}

void TestContains(){
	cout << "Testing Contains..." << endl;

	RedBlackTree *rbt = new RedBlackTree();
	assert(rbt->Contains(6) == false);
	assert(rbt->Contains(0) == false);
	assert(rbt->Contains(-10) == false);
	delete rbt;

	rbt = new RedBlackTree();
	rbt->Insert(40);
	rbt->Insert(22);
	rbt->Insert(15);
	rbt->Insert(31);
	rbt->Insert(55);
	rbt->Insert(12);
	rbt->Insert(17);
	rbt->Insert(29);
	rbt->Insert(34);
	
	// Test all present nodes
	assert(rbt->Contains(40));
	assert(rbt->Contains(22));
	assert(rbt->Contains(15));
	assert(rbt->Contains(31));
	assert(rbt->Contains(55));
	assert(rbt->Contains(12));
	assert(rbt->Contains(17));
	assert(rbt->Contains(29));
	assert(rbt->Contains(34));

	// Test non-present nodes
	assert(!rbt->Contains(10));
	assert(!rbt->Contains(16));
	assert(!rbt->Contains(30));
	assert(!rbt->Contains(100));
	assert(!rbt->Contains(-5));

	delete rbt;
	
	cout << "PASSED!" << endl << endl;
}

void TestGetMinimumMaximum(){
	cout << "Testing Get Minimum and Get Maximum..." << endl;

	// Single node tree
	RedBlackTree singleTree = RedBlackTree(42);
	assert(singleTree.GetMin() == 42);
	assert(singleTree.GetMax() == 42);

	// Multi-node tree
	RedBlackTree multiTree;
	multiTree.Insert(50);
	multiTree.Insert(25);
	multiTree.Insert(75);
	multiTree.Insert(10);
	multiTree.Insert(30);
	multiTree.Insert(90);
	multiTree.Insert(5);
	multiTree.Insert(99);

	assert(multiTree.GetMin() == 5);
	assert(multiTree.GetMax() == 99);

	// Empty tree exception handling
	RedBlackTree emptyTree;
	bool minThrew = false;
	try {
		emptyTree.GetMin();
	} catch (const invalid_argument &) {
		minThrew = true;
	}
	assert(minThrew);

	bool maxThrew = false;
	try {
		emptyTree.GetMax();
	} catch (const invalid_argument &) {
		maxThrew = true;
	}
	assert(maxThrew);

	cout << "PASSED!" << endl << endl;
}

void TestDuplicateException(){
	cout << "Testing Duplicate Insertion Exception..." << endl;

	RedBlackTree rbt;
	rbt.Insert(10);
	rbt.Insert(20);
	rbt.Insert(30);
	assert(rbt.Size() == 3);

	bool threw = false;
	try {
		rbt.Insert(20); // Duplicate!
	} catch (const invalid_argument &) {
		threw = true;
	}
	assert(threw);
	// Size should remain unchanged
	assert(rbt.Size() == 3);

	cout << "PASSED!" << endl << endl;
}

void TestSize(){
	cout << "Testing Size Tracking..." << endl;

	RedBlackTree rbt;
	assert(rbt.Size() == 0);
	rbt.Insert(10);
	assert(rbt.Size() == 1);
	rbt.Insert(5);
	assert(rbt.Size() == 2);
	rbt.Insert(15);
	assert(rbt.Size() == 3);
	rbt.Insert(2);
	assert(rbt.Size() == 4);

	cout << "PASSED!" << endl << endl;
}

void TestPrivateTestsMethod(){
	cout << "Testing Private Tests (rotations / invariants)..." << endl;

	RedBlackTree rbt;
	rbt.PrivateTests();

	cout << "PASSED!" << endl << endl;
}

void TestLargeTree(){
	cout << "Testing Large Tree (100 elements)..." << endl;

	RedBlackTree rbt;
	vector<int> nums;
	for (int i = 1; i <= 100; i++) {
		nums.push_back(i * 3);
	}

	// Pseudo-shuffle using fixed seed for repeatable deterministic testing
	mt19937 g(1337);
	shuffle(nums.begin(), nums.end(), g);

	for (int n : nums) {
		rbt.Insert(n);
	}

	assert(rbt.Size() == 100);
	assert(rbt.GetMin() == 3);
	assert(rbt.GetMax() == 300);

	for (int n : nums) {
		assert(rbt.Contains(n));
	}

	assert(!rbt.Contains(0));
	assert(!rbt.Contains(4));
	assert(!rbt.Contains(301));

	cout << "PASSED!" << endl << endl;
}

int main(){
	TestSimpleConstructor();
	TestConstructor();
	
	TestInsertFirstNode();
	TestInsertSecondNode();
	TestInsertThirdNode();
	TestInsertFourthNode();
	TestInsertFifthNode();

	TestToStrings();
	TestInsertRandomTests();

	TestCopyConstructor();

	TestContains();
	TestGetMinimumMaximum();

	TestDuplicateException();
	TestSize();
	TestPrivateTestsMethod();
	TestLargeTree();
	
	cout << "ALL TESTS PASSED!!" << endl;
	return 0;
}
