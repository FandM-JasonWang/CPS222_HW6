#include "RedBlackTree.h"
#include <stdexcept>
#include <string>
#include <cassert>

using namespace std;

// Resource Citations:
// 1. Red-Black Tree Visualization and step-by-step verification:
//    https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
// 2. TutorialsPoint Red-Black Tree Insertion & Rotations:
//    https://www.tutorialspoint.com/insertion-in-the-red-black-tree-in-data-structure
// 3. BTech Smart Class Red-Black Trees:
//    http://btechsmartclass.com/data_structures/red-black-trees.html
// 4. zyBooks Section 16 (Red-Black Trees)
// 5. Lecture notes on balanced BSTs and trinode restructuring

RedBlackTree::RedBlackTree() {
    // Empty tree starts with null root and zero items
    root = nullptr;
    numItems = 0;
}

RedBlackTree::RedBlackTree(int value) {
    // Single-node constructor: starts empty and inserts the first value
    root = nullptr;
    numItems = 0;
    Insert(value);
}

RedBlackTree::RedBlackTree(const RedBlackTree &other) {
    // Deep copy constructor: recursively replicates every node and retains its color
    // so mutations in this tree will never corrupt the original tree.
    root = CopyTree(other.root, nullptr);
    numItems = other.numItems;
}

RedBlackTree::~RedBlackTree() {
    // Clean up all dynamically allocated RBTNode heap memory using post-order traversal
    DestroyTree(root);
    root = nullptr;
    numItems = 0;
}

RBTNode* RedBlackTree::CopyTree(const RBTNode *node, RBTNode *parent) {
    // Base case: nothing to copy if subtree is null
    if (node == nullptr) {
        return nullptr;
    }

    // Allocate a new node identical to the source node, setting the parent link correctly
    RBTNode *newNode = new RBTNode(node->data, node->color, parent);
    // Recursively copy both left and right subtrees
    newNode->left = CopyTree(node->left, newNode);
    newNode->right = CopyTree(node->right, newNode);
    return newNode;
}

void RedBlackTree::DestroyTree(RBTNode *node) {
    // Post-order destruction: delete children before deleting the parent node
    if (node != nullptr) {
        DestroyTree(node->left);
        DestroyTree(node->right);
        delete node;
    }
}

unsigned long long int RedBlackTree::Size() const {
    // Directly return the cached item count in O(1) time
    return numItems;
}

RBTNode* RedBlackTree::Search(int value) const {
    // Standard iterative BST lookup to avoid recursion overhead
    RBTNode *curr = root;
    while (curr != nullptr) {
        if (value == curr->data) {
            return curr;
        } else if (value < curr->data) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return nullptr;
}

bool RedBlackTree::Contains(int value) const {
    // Return true if value was found in the tree, false otherwise
    return Search(value) != nullptr;
}

int RedBlackTree::GetMin() const {
    // Cannot get minimum of an empty tree without violating safety
    if (root == nullptr) {
        throw invalid_argument("Cannot get minimum from an empty tree");
    }

    // In a BST, the minimum key is found at the leftmost leaf
    RBTNode *curr = root;
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr->data;
}

int RedBlackTree::GetMax() const {
    // Cannot get maximum of an empty tree
    if (root == nullptr) {
        throw invalid_argument("Cannot get maximum from an empty tree");
    }

    // In a BST, the maximum key is found at the rightmost leaf
    RBTNode *curr = root;
    while (curr->right != nullptr) {
        curr = curr->right;
    }
    return curr->data;
}

string RedBlackTree::ToInfixString(RBTNode *n) const {
    // Infix (in-order) traversal: Left -> Node -> Right
    // Produces sorted elements with required spacing
    if (n == nullptr) {
        return "";
    }
    string colorChar = (n->color == COLOR_RED ? "R" : "B");
    return ToInfixString(n->left) + " " + colorChar + to_string(n->data) + " " + ToInfixString(n->right);
}

string RedBlackTree::ToPrefixString(RBTNode *n) const {
    // Prefix (pre-order) traversal: Node -> Left -> Right
    // Captures the hierarchical structure of the tree from root down
    if (n == nullptr) {
        return "";
    }
    string colorChar = (n->color == COLOR_RED ? "R" : "B");
    return " " + colorChar + to_string(n->data) + " " + ToPrefixString(n->left) + ToPrefixString(n->right);
}

string RedBlackTree::ToPostfixString(RBTNode *n) const {
    // Postfix (post-order) traversal: Left -> Right -> Node
    if (n == nullptr) {
        return "";
    }
    string colorChar = (n->color == COLOR_RED ? "R" : "B");
    return ToPostfixString(n->left) + ToPostfixString(n->right) + " " + colorChar + to_string(n->data) + " ";
}

void RedBlackTree::RotateLeft(RBTNode *x) {
    // Pivot node x around its right child y
    // Why: Balances a tree that leans right
    if (x == nullptr || x->right == nullptr) {
        return;
    }

    RBTNode *y = x->right;

    // Turn y's left subtree into x's right subtree
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }

    // Link y's parent to x's parent
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y; // x was root, so y now becomes the root
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    // Place x on y's left
    y->left = x;
    x->parent = y;
}

void RedBlackTree::RotateRight(RBTNode *x) {
    // Pivot node x around its left child y
    // Why: Balances a tree that leans left
    if (x == nullptr || x->left == nullptr) {
        return;
    }

    RBTNode *y = x->left;

    // Turn y's right subtree into x's left subtree
    x->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = x;
    }

    // Link y's parent to x's parent
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y; // x was root, so y now becomes the root
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }

    // Place x on y's right
    y->right = x;
    x->parent = y;
}

void RedBlackTree::Insert(int value) {
    // Duplicate values are prohibited according to assignment specification
    if (Contains(value)) {
        throw invalid_argument("Duplicate value already exists in the tree");
    }

    // New node always starts colored RED
    RBTNode *z = new RBTNode(value, COLOR_RED);

    // If the tree was empty, z becomes the root
    if (root == nullptr) {
        root = z;
        root->color = COLOR_BLACK; // Root property: root is always black
        numItems++;
        return;
    }

    // Standard binary search tree insertion
    RBTNode *parent = nullptr;
    RBTNode *curr = root;
    while (curr != nullptr) {
        parent = curr;
        if (z->data < curr->data) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    // Attach z as the child of parent
    z->parent = parent;
    if (z->data < parent->data) {
        parent->left = z;
    } else {
        parent->right = z;
    }
    numItems++;

    // Restore Red-Black properties if any violation occurred
    InsertFixup(z);
}

void RedBlackTree::InsertFixup(RBTNode *z) {
    // Loop continues as long as z's parent is RED (violates: no two consecutive reds)
    while (z->parent != nullptr && z->parent->color == COLOR_RED) {
        RBTNode *parent = z->parent;
        RBTNode *grandparent = parent->parent;

        // Parent is red, so grandparent must exist (since root is always black)
        if (grandparent == nullptr) {
            break;
        }

        // Case A: Parent is the left child of grandparent
        if (parent == grandparent->left) {
            RBTNode *uncle = grandparent->right;

            // Case 1: Uncle is RED -> Recolor parent, uncle, and grandparent
            if (uncle != nullptr && uncle->color == COLOR_RED) {
                parent->color = COLOR_BLACK;
                uncle->color = COLOR_BLACK;
                grandparent->color = COLOR_RED;
                // Move z up to grandparent to check for violations higher up
                z = grandparent;
            } else {
                // Case 2a: Uncle is BLACK and z is a right child (Triangle / Left-Right shape)
                if (z == parent->right) {
                    z = parent;
                    RotateLeft(z);
                    parent = z->parent;
                    grandparent = parent->parent;
                }
                // Case 2b: Uncle is BLACK and z is a left child (Line / Left-Left shape)
                parent->color = COLOR_BLACK;
                grandparent->color = COLOR_RED;
                RotateRight(grandparent);
            }
        } else {
            // Case B: Parent is the right child of grandparent (Symmetric mirror of Case A)
            RBTNode *uncle = grandparent->left;

            // Case 1: Uncle is RED -> Recolor parent, uncle, and grandparent
            if (uncle != nullptr && uncle->color == COLOR_RED) {
                parent->color = COLOR_BLACK;
                uncle->color = COLOR_BLACK;
                grandparent->color = COLOR_RED;
                z = grandparent;
            } else {
                // Case 2a: Uncle is BLACK and z is a left child (Triangle / Right-Left shape)
                if (z == parent->left) {
                    z = parent;
                    RotateRight(z);
                    parent = z->parent;
                    grandparent = parent->parent;
                }
                // Case 2b: Uncle is BLACK and z is a right child (Line / Right-Right shape)
                parent->color = COLOR_BLACK;
                grandparent->color = COLOR_RED;
                RotateLeft(grandparent);
            }
        }
    }

    // Root must always remain black (Property 2)
    if (root != nullptr) {
        root->color = COLOR_BLACK;
    }
}

void RedBlackTree::PrivateTests() {
    // Exercise private methods: Rotations on a controlled 3-node structure
    // Let's create an isolated node configuration to test RotateLeft and RotateRight
    RBTNode *n1 = new RBTNode(10, COLOR_BLACK);
    RBTNode *n2 = new RBTNode(20, COLOR_RED, n1);
    n1->right = n2;
    RBTNode *oldRoot = root;
    unsigned long long int oldSize = numItems;

    // Temporarily point root to n1
    root = n1;
    numItems = 2;
    RotateLeft(n1);
    assert(root == n2);
    assert(n2->left == n1);
    assert(n1->parent == n2);

    RotateRight(n2);
    assert(root == n1);
    assert(n1->right == n2);
    assert(n2->parent == n1);

    // Clean up temporary nodes
    delete n2;
    delete n1;

    // Restore root
    root = oldRoot;
    numItems = oldSize;
}

