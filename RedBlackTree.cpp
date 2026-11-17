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
// 3. University of Toronto CSC265 Tutorial on Red-Black Tree Deletion:
//    http://www.cs.toronto.edu/~wgeorge/csc265/2013/09/26/tutorial-3-red-black-tree-deletion.html
// 4. United States Naval Academy (USNA) SI321 Red-Black Trees:
//    https://www.usna.edu/Users/cs/crabbe/SI321/current/red-black/red-black.html
// 5. zyBooks Section 16 (Red-Black Trees & In-order successor)
// 6. F&M CPS222 Video Lecture: Red-Black Tree Removal & In-order Successor algorithm

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

RedBlackTree& RedBlackTree::operator=(const RedBlackTree &other) {
    // Self-assignment guard
    if (this != &other) {
        // Destroy existing allocated nodes to prevent memory leaks
        DestroyTree(root);
        // Deep copy from other tree
        root = CopyTree(other.root, nullptr);
        numItems = other.numItems;
    }
    return *this;
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

// Speed benchmark reflection:
// In our SpeedTest simulation with 3.7 million nodes (where tree height is bounded strictly by 2 * log2(N+1) <= 44),
// each individual Insert() call takes approximately 0.000001 to 0.000004 seconds (~1 to 4 microseconds).
// BST traversal takes O(log N) steps and trinode restructuring requires at most 2 tree rotations,
// guaranteeing blazing fast O(log N) worst-case performance.
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

RBTNode* RedBlackTree::MinValueNode(RBTNode *node) const {
    // Traverse down to the leftmost leaf in the subtree to find the minimum element
    RBTNode *curr = node;
    while (curr != nullptr && curr->left != nullptr) {
        curr = curr->left;
    }
    return curr;
}

// Speed benchmark reflection:
// In our SpeedTest simulation with 3.7 million nodes, each individual Remove() call
// takes approximately 0.000001 to 0.000004 seconds (~1 to 4 microseconds).
// Finding the in-order successor takes O(log N) steps, and color/tree restructuring
// requires at most 3 rotations during RemoveFixup, guaranteeing optimal O(log N) deletion.
void RedBlackTree::Remove(int data) {
    // Step 1: Standard BST search for the node containing the target data
    RBTNode *z = Search(data);
    if (z == nullptr) {
        // Value not present in tree; return safely without doing anything
        return;
    }

    // Step 2: Determine actual physical node y to remove from the tree
    RBTNode *y = nullptr;
    if (z->left != nullptr && z->right != nullptr) {
        // Case: Node has two children.
        // We MUST use the in-order successor (minimum node in the right subtree)
        // as explicitly specified in the video lecture and assignment guidelines.
        y = MinValueNode(z->right);
        // Overwrite z's data with in-order successor's data
        z->data = y->data;
        // y is now the node that will physically be deleted.
        // Notice y has at most ONE child (it cannot have a left child because it's the minimum).
    } else {
        // Node has at most one child: y is z itself
        y = z;
    }

    // Identify child x of y (could be left, right, or nullptr if y is a leaf)
    RBTNode *x = (y->left != nullptr) ? y->left : y->right;

    // Case A: y is a leaf node (x == nullptr)
    if (x == nullptr) {
        // Subcase A1: y is the root and the ONLY node in the entire tree
        if (y == root) {
            delete root;
            root = nullptr;
            numItems--;
            return;
        }

        // Subcase A2: y is a RED leaf
        // Removing a red leaf cannot violate black-height property and creates no adjacent red nodes!
        // No fixup needed; just detach and delete.
        if (y->color == COLOR_RED) {
            if (y->parent->left == y) {
                y->parent->left = nullptr;
            } else {
                y->parent->right = nullptr;
            }
            delete y;
            numItems--;
            return;
        }

        // Subcase A3: y is a BLACK leaf
        // Deleting a black leaf reduces black-height along this path, creating a DOUBLE BLACK deficit!
        // To fix this without allocating dummy sentinel nodes, we mark y as COLOR_DOUBLE_BLACK
        // while it is still properly connected to y->parent.
        // RemoveFixup will navigate the tree, recolor, and rotate using y's valid parent pointer.
        y->color = COLOR_DOUBLE_BLACK;
        RemoveFixup(y);

        // After fixup has restored balance and black-height across the tree,
        // we safely detach y from its parent and free its memory.
        if (y->parent != nullptr) {
            if (y->parent->left == y) {
                y->parent->left = nullptr;
            } else {
                y->parent->right = nullptr;
            }
        } else if (y == root) {
            root = nullptr;
        }
        delete y;
        numItems--;
        return;
    }

    // Case B: y has exactly ONE non-null child x
    // In a valid Red-Black Tree, a node with only one child MUST be BLACK,
    // and that child x MUST be RED. (Otherwise the black-height to null leaves would be unequal).
    // This is the exact scenario featured in Video Question 3 (e.g. root 20 [black] with left child 4 [red]).
    x->parent = y->parent;
    if (y->parent == nullptr) {
        root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    if (y->color == COLOR_BLACK) {
        if (x->color == COLOR_RED) {
            // Video Question 3 walkthrough branch:
            // Simply color x black! This absorbs the black deficit and immediately restores black-height.
            x->color = COLOR_BLACK;
        } else {
            // General defensive fallback if x was black
            x->color = COLOR_DOUBLE_BLACK;
            RemoveFixup(x);
        }
    }

    delete y;
    numItems--;
}

void RedBlackTree::RemoveFixup(RBTNode *x) {
    // Loop continues while x is not root and still carries a double black deficit
    while (x != root && x != nullptr && x->color == COLOR_DOUBLE_BLACK) {
        RBTNode *p = x->parent;
        if (p == nullptr) {
            break;
        }

        // Case A: x is the left child of parent p
        if (x == p->left) {
            RBTNode *w = p->right; // w is x's sibling

            // Case 1: Sibling w is RED
            // Strategy: Recolor w to black, p to red, and rotate left at p.
            // This brings a black node into w's position, converting this into Case 2, 3, or 4.
            if (w != nullptr && w->color == COLOR_RED) {
                w->color = COLOR_BLACK;
                p->color = COLOR_RED;
                RotateLeft(p);
                w = p->right; // Update to the new black sibling
            }

            // At this point, sibling w is guaranteed to be BLACK.
            // Check the colors of w's children (null pointers are treated as BLACK leaves).
            bool leftBlack = (w == nullptr || w->left == nullptr || w->left->color == COLOR_BLACK);
            bool rightBlack = (w == nullptr || w->right == nullptr || w->right->color == COLOR_BLACK);

            // Case 2: Sibling w is BLACK and both of w's children are BLACK
            // Strategy: Sibling w gives up a black (becomes RED).
            // The extra black from x and w moves up to parent p.
            if (leftBlack && rightBlack) {
                if (w != nullptr) {
                    w->color = COLOR_RED;
                }
                x->color = COLOR_BLACK; // Remove double-black from x
                if (p->color == COLOR_RED) {
                    // Parent was RED: it absorbs the extra black and becomes single BLACK!
                    p->color = COLOR_BLACK;
                    x = root; // Problem completely solved, terminate loop
                    break;
                } else {
                    // Parent was BLACK: it now becomes DOUBLE BLACK.
                    // Push the double black up to p and continue resolving higher in the tree.
                    p->color = COLOR_DOUBLE_BLACK;
                    x = p;
                }
            } else {
                // Case 3: Sibling w is BLACK, inner child (left) is RED, outer child (right) is BLACK
                // Strategy: Recolor w's left child to black, w to red, and rotate right at w.
                // This transforms the configuration into Case 4 without violating invariants.
                if (rightBlack) {
                    if (w->left != nullptr) {
                        w->left->color = COLOR_BLACK;
                    }
                    w->color = COLOR_RED;
                    RotateRight(w);
                    w = p->right; // w is now the former inner child (now right child of p)
                }

                // Case 4: Sibling w is BLACK and outer child (right) is RED
                // Strategy: w takes parent's color, parent becomes BLACK, w's right child becomes BLACK,
                // and rotate left at p. The double black is fully absorbed and eliminated!
                w->color = p->color;
                p->color = COLOR_BLACK;
                if (w->right != nullptr) {
                    w->right->color = COLOR_BLACK;
                }
                RotateLeft(p);
                x->color = COLOR_BLACK;
                x = root; // Terminate loop
                break;
            }
        } else {
            // Case B: x is the right child of parent p (Symmetric mirror of Case A)
            RBTNode *w = p->left; // Sibling w is on the left

            // Case 1 (symmetric): Sibling w is RED
            if (w != nullptr && w->color == COLOR_RED) {
                w->color = COLOR_BLACK;
                p->color = COLOR_RED;
                RotateRight(p);
                w = p->left;
            }

            bool leftBlack = (w == nullptr || w->left == nullptr || w->left->color == COLOR_BLACK);
            bool rightBlack = (w == nullptr || w->right == nullptr || w->right->color == COLOR_BLACK);

            // Case 2 (symmetric): Sibling w is BLACK and both children are BLACK
            if (leftBlack && rightBlack) {
                if (w != nullptr) {
                    w->color = COLOR_RED;
                }
                x->color = COLOR_BLACK;
                if (p->color == COLOR_RED) {
                    p->color = COLOR_BLACK;
                    x = root;
                    break;
                } else {
                    p->color = COLOR_DOUBLE_BLACK;
                    x = p;
                }
            } else {
                // Case 3 (symmetric): Sibling w is BLACK, inner child (right) is RED, outer child (left) is BLACK
                if (leftBlack) {
                    if (w->right != nullptr) {
                        w->right->color = COLOR_BLACK;
                    }
                    w->color = COLOR_RED;
                    RotateLeft(w);
                    w = p->left;
                }

                // Case 4 (symmetric): Sibling w is BLACK and outer child (left) is RED
                w->color = p->color;
                p->color = COLOR_BLACK;
                if (w->left != nullptr) {
                    w->left->color = COLOR_BLACK;
                }
                RotateRight(p);
                x->color = COLOR_BLACK;
                x = root;
                break;
            }
        }
    }

    // Ensure x is restored to single black if it was marked double black
    if (x != nullptr && x->color == COLOR_DOUBLE_BLACK) {
        x->color = COLOR_BLACK;
    }

    // Root property: Root must always remain black
    if (root != nullptr) {
        root->color = COLOR_BLACK;
    }
}

