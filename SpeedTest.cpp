#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "RedBlackTree.h"

using namespace std;

// Resource Citations:
// 1. zyBooks Section 16 (Red-Black Trees & Search/Insertion/Deletion Performance)
// 2. F&M CPS222 Assignment 6 Part 2 Guidelines (SpeedTest Simulation)

int main() {
    // Seed random number generator with fixed or current time
    srand(static_cast<unsigned int>(time(nullptr)));

    // Create a simulated 3.7 million ID number system (representing Twitter/X users)
    const int N = 3700000;

    cout << "==========================================================" << endl;
    cout << "       Red-Black Tree 3.7M Scale Speed Test Benchmark      " << endl;
    cout << "==========================================================" << endl;
    cout << "Allocating and inserting " << N << " sequential user IDs..." << endl;

    clock_t start = clock();
    RedBlackTree rbt = RedBlackTree();
    for (int i = 0; i < N; i++) {
        rbt.Insert(i);
    }
    clock_t stop = clock();

    double duration = (static_cast<double>(stop - start)) / CLOCKS_PER_SEC;
    cout << "Collected " << rbt.Size() << " ID numbers in " << fixed << setprecision(4) << duration << " seconds." << endl;
    cout << "----------------------------------------------------------" << endl;

    // Simulation of daily activity:
    // On any given day, an average of 5 people leave the service (remove 5 IDs),
    // and 5 other people join the service (insert 5 new IDs).

    // Part 1: Simulate 5 users leaving the service (Remove operations)
    cout << "\nSimulating 5 users leaving the service (Remove):" << endl;
    int removedCount = 0;
    while (removedCount < 5) {
        // Pick random ID in range [0, N-1]
        int candidateID = rand() % N;
        if (rbt.Contains(candidateID)) {
            clock_t remStart = clock();
            rbt.Remove(candidateID);
            clock_t remStop = clock();

            double remDuration = (static_cast<double>(remStop - remStart)) / CLOCKS_PER_SEC;
            removedCount++;
            cout << "  [Remove " << removedCount << "] ID " << setw(7) << candidateID 
                 << " removed in " << scientific << setprecision(6) << remDuration << " seconds." << endl;
        }
    }

    // Part 2: Simulate 5 users joining the service (Insert operations)
    cout << "\nSimulating 5 users joining the service (Insert):" << endl;
    int addedCount = 0;
    while (addedCount < 5) {
        // Pick new ID outside the initial range [N, N + 10000000]
        int candidateID = N + (rand() % 10000000);
        if (!rbt.Contains(candidateID)) {
            clock_t insStart = clock();
            rbt.Insert(candidateID);
            clock_t insStop = clock();

            double insDuration = (static_cast<double>(insStop - insStart)) / CLOCKS_PER_SEC;
            addedCount++;
            cout << "  [Insert " << addedCount << "] ID " << setw(7) << candidateID 
                 << " inserted in " << scientific << setprecision(6) << insDuration << " seconds." << endl;
        }
    }

    cout << "\n----------------------------------------------------------" << endl;
    cout << "Final Tree Size: " << rbt.Size() << " items (expected " << N << ")" << endl;
    cout << "Speed test completed successfully!" << endl;
    cout << "==========================================================" << endl;

    return 0;
}
