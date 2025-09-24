#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <windows.h>
using namespace std;

void add_part(const vector<int>& A, const vector<int>& B, vector<int>& C, int start, int end) {
    for (int i = start; i < end; i++) {
        C[i] = A[i] + B[i];
    }
}

double run_test(int n, int num_threads) {
    vector<int> A(n, 1), B(n, 2), C(n);
    vector<thread> threads;

    auto start = chrono::high_resolution_clock::now();

    int chunk = n / num_threads;
    for (int i = 0; i < num_threads; i++) {
        int s = i * chunk;
        int e = (i == num_threads - 1) ? n : s + chunk;
        threads.emplace_back(add_part, cref(A), cref(B), ref(C), s, e);
    }

    for (auto& t : threads) t.join();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> dur = end - start;
    return dur.count();
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Количество аппаратных ядер = "
        << thread::hardware_concurrency() << endl;

    vector<int> sizes = { 1000, 10000, 100000, 1000000 };
    vector<int> threads = { 1, 2, 4, 8, 16 };

    for (int n : sizes) {
        cout << "\nРазмер массива = " << n << endl;
        for (int t : threads) {
            double time = run_test(n, t);
            cout << t << " поток(ов): " << time << "s" << endl;
        }
    }
    return 0;
}
