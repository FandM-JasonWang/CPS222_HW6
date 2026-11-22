# CPS222 HW6 - Red-Black Tree (Part 1 & Part 2)

A fully functional, self-balancing Red-Black Tree in C++ featuring insertion, in-order successor deletion, double-black restructuring fixup, post-order memory management with zero memory leaks, and a 3.7 million entry user-ID speed benchmark simulation.

## Features

### Part 1
- **RBT Node & Properties**: Integer color constants (`COLOR_RED = 0`, `COLOR_BLACK = 1`, `COLOR_DOUBLE_BLACK = 2`).
- **Insertion**: Standard BST insertion followed by bottom-up `InsertFixup` using Uncle recoloring and left/right single/double rotations (`RotateLeft`, `RotateRight`).
- **String Serializations**: Recursive `ToInfixString()`, `ToPrefixString()`, and `ToPostfixString()` with color indicators.
- **Deep Copy**: Recursive `CopyTree()` constructor ensuring deep node replication without shared pointers.
- **Search & Bounds**: `Contains()`, `Size()`, `GetMin()`, `GetMax()`.

### Part 2
- **Deletion (`Remove`)**:
  - Employs **in-order successor** (minimum node in the right subtree) for deleting two-child nodes as specified in the course lecture.
  - Handles single-child deletion (e.g. black node with red child recolored directly to black without rotations).
  - Handles black leaf deletion by temporarily retaining the node as a `COLOR_DOUBLE_BLACK` sentinel, running `RemoveFixup()`, and safely unlinking and freeing the node.
- **Double Black Fixup (`RemoveFixup`)**:
  - **Case 1**: Red sibling -> Recolor and rotate parent towards double-black node.
  - **Case 2**: Black sibling with black children -> Sibling becomes red, double-black pushed to parent.
  - **Case 3**: Black sibling with red inner nephew, black outer nephew -> Rotate sibling and recolor to convert to Case 4.
  - **Case 4**: Black sibling with red outer nephew -> Rotate parent, recolor, double-black eliminated.
  - Symmetric branches for left and right children.
- **Memory Management**:
  - Clean recursive post-order destructor `~RedBlackTree()`.
  - Zero memory leaks, zero invalid reads/writes under `valgrind --leak-check=full`.
- **SpeedTest Simulation (`SpeedTest.cpp`)**:
  - Simulates a 3,700,000 user-ID allocation system (e.g. Twitter/X).
  - Simulates 5 users leaving (`Remove()`) and 5 users joining (`Insert()`).
  - High-precision time measurements for individual operations.

## How to Compile and Run (WSL / Linux)

### 1. Compile all targets
```bash
make clean
make all
```

### 2. Run Test Suites
```bash
./first-step
./rbt-tests
```

### 3. Valgrind Memory Leak Check
```bash
valgrind --leak-check=full ./rbt-tests
```

### 4. Run Speed Benchmark
```bash
./speed-test
```

## Author
Jason Wang
