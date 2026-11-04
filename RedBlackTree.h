#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <iostream>
#include <string>

// Resource citations:
// 1. USFCA Red-Black Tree Visualizer: https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
// 2. TutorialsPoint RBT Insertion: https://www.tutorialspoint.com/insertion-in-the-red-black-tree-in-data-structure
// 3. BTech Smart Class Red-Black Trees: http://btechsmartclass.com/data_structures/red-black-trees.html
// 4. zyBooks Section 16 (Red-Black Trees)

// Color definitions:
// Using integer constants via #define makes color assignments clean and readable (e.g., node->color = COLOR_RED)
// while conforming strictly to the assignment rubric specifications.
#define COLOR_RED 0
#define COLOR_BLACK 1
#define COLOR_DOUBLE_BLACK 2

/**
 * struct RBTNode
 * Represents an individual node in the Red-Black Tree.
 * Each node maintains pointers to its left child, right child, and parent,
 * allowing tree restructuring and rotations to efficiently walk upwards to the root.
 */
struct RBTNode {
    int data;
    int color;
    RBTNode *left = nullptr;
    RBTNode *right = nullptr;
    RBTNode *parent = nullptr;

    // Default node constructor initializes color to RED because every new leaf node in RBT insertion starts as RED.
    RBTNode(int val, int col = COLOR_RED, RBTNode *p = nullptr, RBTNode *l = nullptr, RBTNode *r = nullptr)
        : data(val), color(col), left(l), right(r), parent(p) {}
};

class RedBlackTree {
public:
    // Regular default constructor initializes an empty tree with root == nullptr
    RedBlackTree();

    // Overloaded constructor that creates a tree with a single initial value
    RedBlackTree(int value);

    // Deep copy constructor: ensures independent subtree copy without pointer sharing
    RedBlackTree(const RedBlackTree &other);

    // Destructor: ensures full recursive memory cleanup of all allocated nodes
    ~RedBlackTree();

    // Inserts a new integer into the red-black tree, maintaining all RBT invariants.
    // Throws invalid_argument on duplicate values.
    void Insert(int value);

    // Checks if the given value is present anywhere in the tree.
    bool Contains(int value) const;

    // Returns the minimum integer value stored in the tree.
    int GetMin() const;

    // Returns the maximum integer value stored in the tree.
    int GetMax() const;

    // Returns the total number of nodes currently in the tree.
    unsigned long long int Size() const;

    // Public ToString functions declared inline as required by the assignment spec:
    std::string ToInfixString() const { return ToInfixString(root); };
    std::string ToPrefixString() const { return ToPrefixString(root); };
    std::string ToPostfixString() const { return ToPostfixString(root); };

    // Public method to run internal verification tests on private methods (e.g. rotations)
    void PrivateTests();

private:
    // Instance variables required by the assignment description:
    unsigned long long int numItems = 0;
    RBTNode *root = nullptr;

    // Private recursive traversal helpers:
    std::string ToInfixString(RBTNode *n) const;
    std::string ToPrefixString(RBTNode *n) const;
    std::string ToPostfixString(RBTNode *n) const;

    // Rotations:
    void RotateLeft(RBTNode *x);
    void RotateRight(RBTNode *x);

    // Insertion fixup to restore red-black properties after standard BST insertion
    void InsertFixup(RBTNode *z);

    // Recursive helpers for deep copying and destroying subtrees
    RBTNode* CopyTree(const RBTNode *node, RBTNode *parent);
    void DestroyTree(RBTNode *node);

    // Internal node search
    RBTNode* Search(int value) const;
};

#endif // REDBLACKTREE_H

