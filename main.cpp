

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "vector.h"

using namespace std;
using namespace std::chrono;

void test_basic_operations() {
    cout << "Testing basic operations..." << endl;
    
    fast::vector<int> fv;
    vector<int> sv;
    
    for (int i = 0; i < 1000; ++i) {
        fv.push_back(i);
        sv.push_back(i);
    }
    
    if (fv.size() != sv.size()) {
        cout << "Size mismatch!" << endl;
        return;
    }
    
    for (size_t i = 0; i < fv.size(); ++i) {
        if (fv[i] != sv[i]) {
            cout << "Value mismatch at index " << i << endl;
            return;
        }
    }
    
    cout << "Basic operations test passed!" << endl;
}

void test_performance() {
    cout << "Testing performance..." << endl;
    
    const int N = 1000000;
    
    auto start = high_resolution_clock::now();
    fast::vector<int> fv;
    fv.reserve(N);
    for (int i = 0; i < N; ++i) {
        fv.push_back(i);
    }
    auto end = high_resolution_clock::now();
    auto fast_time = duration_cast<microseconds>(end - start).count();
    
    start = high_resolution_clock::now();
    vector<int> sv;
    sv.reserve(N);
    for (int i = 0; i < N; ++i) {
        sv.push_back(i);
    }
    end = high_resolution_clock::now();
    auto std_time = duration_cast<microseconds>(end - start).count();
    
    cout << "Fast vector time: " << fast_time << " microseconds" << endl;
    cout << "Std vector time: " << std_time << " microseconds" << endl;
    cout << "Speedup: " << (double)std_time / fast_time << "x" << endl;
}

void test_copy_and_move() {
    cout << "Testing copy and move operations..." << endl;
    
    fast::vector<int> fv1;
    for (int i = 0; i < 100; ++i) {
        fv1.push_back(i);
    }
    
    fast::vector<int> fv2 = fv1;
    if (fv2.size() != fv1.size()) {
        cout << "Copy constructor failed!" << endl;
        return;
    }
    
    fast::vector<int> fv3 = std::move(fv1);
    if (fv3.size() != fv2.size() || !fv1.empty()) {
        cout << "Move constructor failed!" << endl;
        return;
    }
    
    cout << "Copy and move test passed!" << endl;
}

void test_edge_cases() {
    cout << "Testing edge cases..." << endl;
    
    fast::vector<int> fv;
    
    if (!fv.empty()) {
        cout << "Empty vector should be empty!" << endl;
        return;
    }
    
    fv.push_back(42);
    if (fv.empty() || fv.size() != 1 || fv[0] != 42) {
        cout << "Push back failed!" << endl;
        return;
    }
    
    fv.pop_back();
    if (!fv.empty()) {
        cout << "Pop back failed!" << endl;
        return;
    }
    
    fv.resize(10, 5);
    if (fv.size() != 10) {
        cout << "Resize failed!" << endl;
        return;
    }
    
    for (int val : fv) {
        if (val != 5) {
            cout << "Resize with value failed!" << endl;
            return;
        }
    }
    
    cout << "Edge cases test passed!" << endl;
}

int main() {
    cout << "Fast Vector Performance Test" << endl;
    cout << "============================" << endl;
    
    test_basic_operations();
    test_copy_and_move();
    test_edge_cases();
    test_performance();
    
    cout << "All tests completed!" << endl;
    return 0;
}
