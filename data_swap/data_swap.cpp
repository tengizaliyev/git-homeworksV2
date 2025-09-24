#include <iostream>
#include <mutex>
#include <thread>
#include <utility>
#include <windows.h>
using namespace std;

struct Data {
    int value;
    mutable mutex m;
    explicit Data(int v = 0) : value(v) {}
};


void swap_lock(Data& a, Data& b) {
    if (&a == &b) return;
    lock(a.m, b.m);
    lock_guard<mutex> la(a.m, adopt_lock);
    lock_guard<mutex> lb(b.m, adopt_lock);
    swap(a.value, b.value);
}

void swap_scoped(Data& a, Data& b) {
    if (&a == &b) return;
    scoped_lock lk(a.m, b.m);
    swap(a.value, b.value);
}

void swap_unique(Data& a, Data& b) {
    if (&a == &b) return;
    unique_lock<mutex> la(a.m, defer_lock);
    unique_lock<mutex> lb(b.m, defer_lock);
    lock(la, lb);
    swap(a.value, b.value);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Data x{ 10 }, y{ 99 };

    cout << "До обмена:   x=" << x.value << "  y=" << y.value << '\n';
    swap_lock(x, y);
    cout << "После swap_lock:       x=" << x.value << "  y=" << y.value << '\n';
    swap_scoped(x, y);
    cout << "После swap_scoped:     x=" << x.value << "  y=" << y.value << '\n';
    swap_unique(x, y);
    cout << "После swap_unique:     x=" << x.value << "  y=" << y.value << '\n';

    return 0;
}
