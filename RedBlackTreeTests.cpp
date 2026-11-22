#include <iostream>
#include <cassert>
#include <random>
#include <vector>
#include <algorithm>
#include <sstream>
#include "RedBlackTree.h"

using namespace std;

// Resource Citations:
// 1. Red-Black Tree Visualizer: https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
// 2. TutorialsPoint RBT: https://www.tutorialspoint.com/insertion-in-the-red-black-tree-in-data-structure
// 3. University of Toronto CSC265 RBT Deletion: http://www.cs.toronto.edu/~wgeorge/csc265/2013/09/26/tutorial-3-red-black-tree-deletion.html
// 4. USNA SI321 Red-Black Trees: https://www.usna.edu/Users/cs/crabbe/SI321/current/red-black/red-black.html
// 5. zyBooks Section 16 (Red-Black Trees & In-order successor)

void TestSimpleConstructor() {
    cout << "Testing Simple Constructor... " << endl;
    RedBlackTree rbt = RedBlackTree();
    assert(rbt.ToInfixString() == "");
    assert(rbt.Size() == 0);
    cout << "PASSED!" << endl << endl;
}

void TestInsertFirstNode() {
    cout << "Testing Insert One Node..." << endl;
    RedBlackTree rbt = RedBlackTree();
    rbt.Insert(30);
    assert(rbt.ToPrefixString() == " B30 ");
    assert(rbt.Size() == 1);
    cout << "PASSED!" << endl << endl;
}

void TestInsertSecondNode() {
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

void TestInsertThirdNode() {
    cout << "Testing Insert Third Node..." << endl;
    RedBlackTree *rbt = new RedBlackTree();
    rbt->Insert(30);
    rbt->Insert(15);
    rbt->Insert(10); // Left Left
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

    // Right Right case
    rbt = new RedBlackTree();
    rbt->Insert(10);
    rbt->Insert(20);
    rbt->Insert(30);
    assert(rbt->ToPrefixString() == " B20  R10  R30 ");
    delete rbt;

    // Right Left case
    rbt = new RedBlackTree();
    rbt->Insert(10);
    rbt->Insert(30);
    rbt->Insert(20);
    assert(rbt->ToPrefixString() == " B20  R10  R30 ");
    delete rbt;

    cout << "PASSED!" << endl << endl;
}

void TestInsertFourthNode() {
    cout << "Testing Insert Fourth Node..." << endl;
    RedBlackTree *rbt = new RedBlackTree();
    rbt->Insert(30);
    rbt->Insert(15);
    rbt->Insert(45);
    rbt->Insert(10);
    assert(rbt->ToPrefixString() == " B30  B15  R10  B45 ");
    assert(rbt->Size() == 4);
    delete rbt;

    rbt = new RedBlackTree();
    rbt->Insert(30);
    rbt->Insert(15);
    rbt->Insert(45);
    rbt->Insert(20);
    assert(rbt->ToPrefixString() == " B30  B15  R20  B45 ");
    assert(rbt->Size() == 4);
    delete rbt;

    cout << "PASSED!" << endl << endl;
}

void TestInsertFifthNode() {
    cout << "Testing Insert Fifth Node..." << endl;
    RedBlackTree *rbt = new RedBlackTree();
    rbt->Insert(30);
    rbt->Insert(15);
    rbt->Insert(45);
    rbt->Insert(10);
    rbt->Insert(20);
    assert(rbt->ToPrefixString() == " B30  B15  R10  R20  B45 ");
    assert(rbt->Size() == 5);
    delete rbt;

    cout << "PASSED!" << endl << endl;
}

void TestInsertDuplicates() {
    cout << "Testing Insert Duplicates..." << endl;
    RedBlackTree rbt;
    rbt.Insert(10);
    rbt.Insert(20);
    rbt.Insert(30);
    assert(rbt.Size() == 3);

    bool threw = false;
    try {
        rbt.Insert(20);
    } catch (const invalid_argument &) {
        threw = true;
    }
    assert(threw);
    assert(rbt.Size() == 3);

    cout << "PASSED!" << endl << endl;
}

void TestRandomInsertStuff() {
    cout << "Testing Random Insert Stuff..." << endl;
    cout << "\t This test passes if it doesn't crash and valgrind reports no issues" << endl;
    RedBlackTree *rbt = new RedBlackTree();
    rbt->Insert(15);
    rbt->Insert(13);
    rbt->Insert(20);
    rbt->Insert(12);
    delete rbt;

    rbt = new RedBlackTree();
    rbt->Insert(12);
    rbt->Insert(11);
    rbt->Insert(15);
    rbt->Insert(5);
    rbt->Insert(13);
    rbt->Insert(7);
    delete rbt;

    rbt = new RedBlackTree();
    rbt->Insert(12);
    rbt->Insert(10);
    rbt->Insert(8);
    delete rbt;

    cout << "PASSED!" << endl << endl;
}

void TestToStringMethods() {
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

void TestConstructionOfBigTree() {
    cout << "Testing Construction of Big Tree..." << endl;
    RedBlackTree rbt;
    vector<int> nums;
    for (int i = 1; i <= 100; i++) {
        nums.push_back(i * 3);
    }
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

    cout << "PASSED!" << endl << endl;
}

void TestCopyConstructor() {
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

    // Test copy assignment operator
    RedBlackTree rbt3;
    rbt3 = rbt1;
    assert(rbt3.ToPrefixString() == rbt1.ToPrefixString());
    assert(rbt3.Size() == rbt1.Size());
    assert(rbt3.Contains(200));

    cout << "PASSED!" << endl << endl;
}

void TestRecursiveFixUp() {
    cout << "Testing Recursive FixUp..." << endl;
    // Insert sequence that cascades recoloring multiple levels up
    RedBlackTree rbt;
    int values[] = {50, 25, 75, 12, 37, 6, 18, 3};
    for (int v : values) {
        rbt.Insert(v);
    }
    // Verify properties hold and tree is balanced
    assert(rbt.Size() == 8);
    assert(rbt.Contains(3));
    assert(rbt.Contains(50));
    assert(rbt.GetMin() == 3);
    assert(rbt.GetMax() == 75);

    cout << "PASSED!" << endl << endl;
}

void TestRemoveNodeWithOneChild() {
    cout << "Testing Remove Node With One Child..." << endl;

    // Test Case A (Matches Video Question 3):
    // Root 20 (BLACK) with left child 4 (RED)
    RedBlackTree *rbt = new RedBlackTree();
    rbt->Insert(20);
    rbt->Insert(4);
    assert(rbt->ToPrefixString() == " B20  R4 ");
    assert(rbt->Size() == 2);

    // Call Remove(20): node 4 replaces 20 and is recolored to BLACK!
    rbt->Remove(20);
    assert(rbt->Size() == 1);
    assert(rbt->ToPrefixString() == " B4 ");
    assert(rbt->Contains(4));
    assert(!rbt->Contains(20));
    delete rbt;

    cout << "PASSED!" << endl << endl;
}

void TestRemoveNodeWithOneChildSecond() {
    cout << "Testing Remove Node With One Child..." << endl;

    // Test Case B: Black node with single RED right child
    RedBlackTree *rbt = new RedBlackTree();
    rbt->Insert(20);
    rbt->Insert(35);
    assert(rbt->ToPrefixString() == " B20  R35 ");
    assert(rbt->Size() == 2);

    rbt->Remove(20);
    assert(rbt->Size() == 1);
    assert(rbt->ToPrefixString() == " B35 ");
    assert(rbt->Contains(35));
    assert(!rbt->Contains(20));
    delete rbt;

    cout << "PASSED!" << endl << endl;
}

void TestRemoveNodeWithTwoChildren() {
    cout << "Testing Remove Node With Two Children" << endl;

    RedBlackTree rbt;
    rbt.Insert(20);
    rbt.Insert(10);
    rbt.Insert(30);
    rbt.Insert(5);
    rbt.Insert(15);
    rbt.Insert(25);
    rbt.Insert(35);

    // Node 30 has two children (25 and 35).
    // In-order successor is 35 (minimum in 30's right subtree).
    rbt.Remove(30);
    assert(!rbt.Contains(30));
    assert(rbt.Contains(35));
    assert(rbt.Contains(25));
    assert(rbt.Size() == 6);

    // Node 20 (root) has two children.
    // In-order successor in right subtree is 25.
    rbt.Remove(20);
    assert(!rbt.Contains(20));
    assert(rbt.Contains(25));
    assert(rbt.Size() == 5);

    cout << "PASSED!" << endl << endl;
}

void TestRemoveDoubleBlackCondition() {
    cout << "Testing Remove (Double Black Condition)" << endl;

    // Tree with black root and two black leaves:
    //      20 (B)
    //     /      |
    //   10 (B)   30 (B)
    RedBlackTree *rbt = new RedBlackTree();
    rbt->Insert(20);
    rbt->Insert(10);
    rbt->Insert(30);

    // Removing black leaf 10 triggers Double Black Case 2:
    // Sibling 30 becomes RED, double black moves to root 20 which absorbs it.
    rbt->Remove(10);
    assert(rbt->Size() == 2);
    assert(!rbt->Contains(10));
    assert(rbt->Contains(20));
    assert(rbt->Contains(30));
    assert(rbt->ToPrefixString() == " B20  R30 ");
    delete rbt;

    // Test double black with red sibling rotation (Case 1 -> Case 4)
    rbt = new RedBlackTree();
    rbt->Insert(50);
    rbt->Insert(25);
    rbt->Insert(75);
    rbt->Insert(60);
    rbt->Insert(90);
    rbt->Insert(55);

    rbt->Remove(25);
    assert(!rbt->Contains(25));
    assert(rbt->Size() == 5);
    delete rbt;

    cout << "PASSED!" << endl << endl;
}

void TestRemoveLeaf() {
    cout << "Testing Remove Leaf..." << endl;

    RedBlackTree rbt;
    rbt.Insert(20);
    rbt.Insert(10);
    rbt.Insert(30);
    rbt.Insert(5); // Red leaf

    // Remove red leaf: trivial case, no rotations needed
    rbt.Remove(5);
    assert(!rbt.Contains(5));
    assert(rbt.Size() == 3);
    assert(rbt.ToPrefixString() == " B20  B10  B30 ");

    // Remove black leaf 30: triggers double-black resolution
    rbt.Remove(30);
    assert(!rbt.Contains(30));
    assert(rbt.Size() == 2);

    cout << "PASSED!" << endl << endl;
}

void TestRemoveRootAsOnlyNode() {
    cout << "Testing Remove Root As Only Node" << endl;

    RedBlackTree rbt;
    rbt.Insert(42);
    assert(rbt.Size() == 1);
    assert(rbt.Contains(42));

    rbt.Remove(42);
    assert(rbt.Size() == 0);
    assert(!rbt.Contains(42));
    assert(rbt.ToInfixString() == "");
    assert(rbt.ToPrefixString() == "");
    assert(rbt.ToPostfixString() == "");

    cout << "PASSED!" << endl << endl;
}

void TestContains() {
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

    assert(rbt->Contains(40));
    assert(rbt->Contains(22));
    assert(rbt->Contains(15));
    assert(rbt->Contains(31));
    assert(rbt->Contains(55));
    assert(rbt->Contains(12));
    assert(rbt->Contains(17));
    assert(rbt->Contains(29));
    assert(rbt->Contains(34));

    assert(!rbt->Contains(10));
    assert(!rbt->Contains(16));
    assert(!rbt->Contains(30));
    assert(!rbt->Contains(100));

    delete rbt;
    cout << "PASSED!" << endl << endl;
}

void TestGetMinimumAndMaximum() {
    cout << "Testing Get Minimum and Get Maximum..." << endl;

    RedBlackTree rbt(42);
    assert(rbt.GetMin() == 42);
    assert(rbt.GetMax() == 42);

    rbt.Insert(10);
    rbt.Insert(90);
    assert(rbt.GetMin() == 10);
    assert(rbt.GetMax() == 90);

    // Remove min
    rbt.Remove(10);
    assert(rbt.GetMin() == 42);

    // Remove max
    rbt.Remove(90);
    assert(rbt.GetMax() == 42);

    cout << "PASSED!" << endl << endl;
}

void SpecialTestOne() {
    cout << "Special Test One..." << endl;
    // Removing items that do not exist should have no effect and not crash
    RedBlackTree rbt;
    rbt.Insert(10);
    rbt.Insert(20);
    rbt.Insert(30);

    rbt.Remove(999);
    rbt.Remove(-50);
    rbt.Remove(15);
    assert(rbt.Size() == 3);
    assert(rbt.Contains(10) && rbt.Contains(20) && rbt.Contains(30));

    cout << "PASSED!" << endl << endl;
}

void SpecialTestTwo() {
    cout << "Special Test Two..." << endl;
    // Insert elements and remove them until completely empty, then reuse tree
    RedBlackTree rbt;
    int items[] = {45, 12, 78, 34, 67, 89, 23, 56};
    for (int x : items) {
        rbt.Insert(x);
    }
    assert(rbt.Size() == 8);

    for (int x : items) {
        rbt.Remove(x);
    }
    assert(rbt.Size() == 0);
    assert(rbt.ToInfixString() == "");

    // Re-inserting elements into previously emptied tree
    rbt.Insert(100);
    rbt.Insert(200);
    assert(rbt.Size() == 2);
    assert(rbt.Contains(100) && rbt.Contains(200));

    cout << "PASSED!" << endl << endl;
}

void SpecialTestThree() {
    cout << "Special Test Three..." << endl;
    // Deep copy independence after removals
    RedBlackTree original;
    for (int i = 1; i <= 20; i++) {
        original.Insert(i * 5);
    }
    original.Remove(25);
    original.Remove(50);
    original.Remove(75);

    RedBlackTree copyTree = original;
    assert(copyTree.Size() == original.Size());
    assert(copyTree.ToPrefixString() == original.ToPrefixString());

    // Mutate copy
    copyTree.Insert(999);
    assert(copyTree.Contains(999));
    assert(!original.Contains(999));
    assert(copyTree.Size() == original.Size() + 1);

    cout << "PASSED!" << endl << endl;
}

void SpecialTestFour() {
    cout << "Special Test Four..." << endl;
    // Verify in-order traversal remains sorted after mixed insertions and removals
    RedBlackTree rbt;
    int vals[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    for (int v : vals) {
        rbt.Insert(v);
    }
    rbt.Remove(30);
    rbt.Remove(70);
    rbt.Remove(10);

    // Infix traversal must contain elements in ascending order
    string infix = rbt.ToInfixString();
    stringstream ss(infix);
    string token;
    vector<int> extracted;
    while (ss >> token) {
        // Token has format 'R20' or 'B20'
        int num = stoi(token.substr(1));
        extracted.push_back(num);
    }
    assert(is_sorted(extracted.begin(), extracted.end()));

    cout << "PASSED!" << endl << endl;
}

void SpecialTestFive() {
    cout << "Special Test Five..." << endl;
    // Sequential min/max removal tracking
    RedBlackTree rbt;
    for (int i = 10; i <= 60; i += 10) {
        rbt.Insert(i);
    }
    assert(rbt.GetMin() == 10);
    assert(rbt.GetMax() == 60);

    rbt.Remove(10);
    assert(rbt.GetMin() == 20);

    rbt.Remove(60);
    assert(rbt.GetMax() == 50);

    cout << "PASSED!" << endl << endl;
}

void SpecialTestSix() {
    cout << "Special Test Six..." << endl;
    // Memory leak verification test for dynamic heap allocation
    RedBlackTree *dynTree = new RedBlackTree();
    for (int i = 1; i <= 50; i++) {
        dynTree->Insert(i);
    }
    for (int i = 1; i <= 25; i++) {
        dynTree->Remove(i);
    }
    assert(dynTree->Size() == 25);
    delete dynTree; // Valgrind will track full memory recovery here

    cout << "PASSED!" << endl << endl;
}

void TestDifferentFromVisualizer() {
    cout << "Different From Visualizer Tests..." << endl;
    // Specifically test that we use in-order successor (right subtree minimum)
    // and not in-order predecessor for two-child removal:
    RedBlackTree rbt;
    rbt.Insert(20);
    rbt.Insert(10);
    rbt.Insert(30);
    rbt.Insert(25);
    rbt.Insert(35);

    // Remove 20: In-order successor is 25 (minimum in 20's right subtree)
    rbt.Remove(20);
    assert(!rbt.Contains(20));
    assert(rbt.Contains(25));
    // Root should now be 25
    assert(rbt.ToPrefixString().find("B25") != string::npos);

    cout << "PASSED!" << endl << endl;
}

void ComprehensiveTest() {
    cout << "ComprehensiveTest..." << endl;

    RedBlackTree rbt;
    const int NUM_ITEMS = 200;
    vector<int> values;
    for (int i = 1; i <= NUM_ITEMS; i++) {
        values.push_back(i * 7);
    }

    mt19937 rng(42);
    shuffle(values.begin(), values.end(), rng);

    // Insert all 200 items
    for (int val : values) {
        rbt.Insert(val);
    }
    assert(rbt.Size() == NUM_ITEMS);

    // Remove the first 100 items
    for (int i = 0; i < 100; i++) {
        rbt.Remove(values[i]);
    }
    assert(rbt.Size() == NUM_ITEMS - 100);

    // Verify first 100 are gone
    for (int i = 0; i < 100; i++) {
        assert(!rbt.Contains(values[i]));
    }

    // Verify remaining 100 are present
    for (int i = 100; i < NUM_ITEMS; i++) {
        assert(rbt.Contains(values[i]));
    }

    // Verify in-order order is strictly sorted
    string infix = rbt.ToInfixString();
    stringstream ss(infix);
    string token;
    vector<int> sortedTokens;
    while (ss >> token) {
        sortedTokens.push_back(stoi(token.substr(1)));
    }
    assert(sortedTokens.size() == 100);
    assert(is_sorted(sortedTokens.begin(), sortedTokens.end()));

    // Clean up remaining nodes down to 0
    for (int i = 100; i < NUM_ITEMS; i++) {
        rbt.Remove(values[i]);
    }
    assert(rbt.Size() == 0);
    assert(rbt.ToInfixString() == "");

    cout << "PASSED!" << endl << endl;
}

int main() {
    TestSimpleConstructor();
    TestInsertFirstNode();
    TestInsertSecondNode();
    TestInsertThirdNode();
    TestInsertFourthNode();
    TestInsertFifthNode();
    TestInsertDuplicates();
    TestRandomInsertStuff();
    TestToStringMethods();
    TestConstructionOfBigTree();
    TestCopyConstructor();
    TestRecursiveFixUp();

    TestRemoveNodeWithOneChild();
    TestRemoveNodeWithOneChildSecond();
    TestRemoveNodeWithTwoChildren();
    TestRemoveDoubleBlackCondition();
    TestRemoveLeaf();
    TestRemoveRootAsOnlyNode();

    TestContains();
    TestGetMinimumAndMaximum();

    SpecialTestOne();
    SpecialTestTwo();
    SpecialTestThree();
    SpecialTestFour();
    SpecialTestFive();
    SpecialTestSix();

    TestDifferentFromVisualizer();
    ComprehensiveTest();

    cout << "ALL TESTS PASSED!!" << endl;
    return 0;
}
