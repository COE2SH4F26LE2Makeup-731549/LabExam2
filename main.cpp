#include <iostream>
#include "FuelLog.hpp"
#include "ReserveLog.hpp"

using namespace std;

// Lightweight heap allocation tracker using only <iostream> and C++11 primitives (no size_t, no extra headers)
static char g_heapPool[65536];
static int g_poolOffset = 0;
static int g_activeAllocs = 0;
static int g_totalAllocs = 0;

void* operator new(decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    // 8-byte alignment
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void* operator new[](decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void operator delete(void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete(void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

bool approxEqual(double a, double b) {
    double diff = a - b;
    if (diff < 0.0) diff = -diff;
    return diff < 0.001;
}

int main() {
    int passed = 0;
    int failed = 0;

    // Expected reference values computed dynamically
    // Array of 3 elements initialized as: initVal + 0, initVal + 1, initVal + 2
    double initV = static_cast<double>(12.0);
    double extV = static_cast<double>(2.0);

    // --- PARENT CLASS TEST CASES (1 to 5) ---
    // TC 1: Default constructor & empty compute
    {
        FuelLog pDef;
        if (approxEqual(static_cast<double>(pDef.totalFuel()), 0.0) &&
            approxEqual(static_cast<double>(pDef.getTripFuel(0)), 0.0)) {
            cout << "[PASS] TC 01: FuelLog default constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 01: FuelLog default constructor" << endl;
            ++failed;
        }
    }

    // TC 2: Parameterized constructor & base compute()
    {
        FuelLog p1(3, initV, extV);
        bool ok = approxEqual(static_cast<double>(p1.getTripFuel(0)), static_cast<double>(initV)) &&
                  approxEqual(static_cast<double>(p1.getTripFuel(2)), static_cast<double>(initV + static_cast<double>(2)));
        // Verify compute() is non-trivial when initV > 0
        if (ok && p1.getTripFuel(1) == static_cast<double>(initV + static_cast<double>(1))) {
            // Compare against a second identical instance
            FuelLog pCheck(3, initV, extV);
            if (approxEqual(static_cast<double>(p1.totalFuel()), static_cast<double>(pCheck.totalFuel())) &&
                !approxEqual(static_cast<double>(p1.totalFuel()), 0.0)) {
                cout << "[PASS] TC 02: FuelLog parameterized constructor & totalFuel()" << endl;
                ++passed;
            } else {
                cout << "[FAIL] TC 02: FuelLog totalFuel() returned unexpected value" << endl;
                ++failed;
            }
        } else {
            cout << "[FAIL] TC 02: FuelLog element initialization mismatch" << endl;
            ++failed;
        }
    }

    // TC 3: Parent Copy Constructor (Deep Copy verification)
    {
        FuelLog orig(3, initV, extV);
        FuelLog copyObj(orig);
        if (approxEqual(static_cast<double>(copyObj.totalFuel()), static_cast<double>(orig.totalFuel())) &&
            approxEqual(static_cast<double>(copyObj.getTripFuel(1)), static_cast<double>(orig.getTripFuel(1))) &&
            !approxEqual(static_cast<double>(copyObj.totalFuel()), 0.0)) {
            cout << "[PASS] TC 03: FuelLog copy constructor deep copy" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 03: FuelLog copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 4: Parent Copy Assignment Operator & Self-Assignment
    {
        FuelLog a(3, initV, extV);
        FuelLog b(2, initV, extV);
        b = a;
        FuelLog* selfPtr = &b;
        b = *selfPtr; // Self-assignment check
        if (approxEqual(static_cast<double>(b.totalFuel()), static_cast<double>(a.totalFuel())) &&
            approxEqual(static_cast<double>(b.getTripFuel(2)), static_cast<double>(a.getTripFuel(2))) &&
            !approxEqual(static_cast<double>(b.totalFuel()), 0.0)) {
            cout << "[PASS] TC 04: FuelLog copy assignment & self-assignment" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 04: FuelLog copy assignment failed" << endl;
            ++failed;
        }
    }

    // TC 5: Parent Destructor Heap Release Check
    {
        int before = g_activeAllocs;
        {
            FuelLog temp(4, initV, extV);
        }
        int after = g_activeAllocs;
        if (before == after && g_totalAllocs > 0) {
            cout << "[PASS] TC 05: FuelLog destructor released heap memory" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 05: FuelLog destructor leaked heap memory" << endl;
            ++failed;
        }
    }

    // --- CHILD CLASS TEST CASES (6 to 10) ---
    // TC 6: Child Parameterized Construction & Overridden compute()
    {
        FuelLog baseObj(3, initV, extV);
        ReserveLog childObj(3, initV, extV);
        if (!approxEqual(static_cast<double>(childObj.totalFuel()), 0.0) &&
            !approxEqual(static_cast<double>(childObj.totalFuel()), static_cast<double>(baseObj.totalFuel()))) {
            cout << "[PASS] TC 06: ReserveLog overridden totalFuel()" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 06: ReserveLog overridden totalFuel() failed" << endl;
            ++failed;
        }
    }

    // TC 7: Polymorphic Dispatch via Base Pointer
    {
        ReserveLog directChild(3, initV, extV);
        FuelLog* polyPtr = &directChild;
        if (!approxEqual(static_cast<double>(polyPtr->totalFuel()), 0.0) &&
            approxEqual(static_cast<double>(polyPtr->totalFuel()), static_cast<double>(directChild.totalFuel()))) {
            cout << "[PASS] TC 07: ReserveLog polymorphic dispatch via FuelLog*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 07: ReserveLog polymorphic dispatch failed" << endl;
            ++failed;
        }
    }

    // TC 8: Child Copy Constructor
    {
        ReserveLog c1(3, initV, extV);
        ReserveLog c2(c1);
        if (approxEqual(static_cast<double>(c1.totalFuel()), static_cast<double>(c2.totalFuel())) &&
            !approxEqual(static_cast<double>(c2.totalFuel()), 0.0)) {
            cout << "[PASS] TC 08: ReserveLog copy constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 08: ReserveLog copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 9: Child Copy Assignment Operator
    {
        ReserveLog c1(3, initV, extV);
        ReserveLog c2;
        c2 = c1;
        if (approxEqual(static_cast<double>(c1.totalFuel()), static_cast<double>(c2.totalFuel())) &&
            !approxEqual(static_cast<double>(c2.totalFuel()), 0.0)) {
            cout << "[PASS] TC 09: ReserveLog copy assignment operator" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 09: ReserveLog copy assignment operator failed" << endl;
            ++failed;
        }
    }

    // TC 10: Virtual Destructor via Base Pointer Deletion
    {
        int before = g_activeAllocs;
        FuelLog* heapChild = new ReserveLog(4, initV, extV);
        delete heapChild;
        int after = g_activeAllocs;
        if (before == after) {
            cout << "[PASS] TC 10: ReserveLog virtual destructor via FuelLog*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 10: ReserveLog virtual destructor leaked memory" << endl;
            ++failed;
        }
    }

    cout << "========================================" << endl;
    cout << "FINAL SCORE: " << passed << " / 10 PASSED (" << failed << " FAILED)" << endl;
    if (g_activeAllocs == 0) {
        cout << "MEMORY LEAK REPORT: NO MEMORY LEAKS DETECTED (0 active allocations)." << endl;
    } else {
        cout << "MEMORY LEAK REPORT: WARNING! " << g_activeAllocs << " active heap allocation(s) leaked!" << endl;
    }
    cout << "========================================" << endl;

    return (failed == 0 && g_activeAllocs == 0) ? 0 : 1;
}
