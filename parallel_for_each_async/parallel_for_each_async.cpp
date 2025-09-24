#include <algorithm>
#include <future>
#include <iostream>
#include <iterator>
#include <vector>
#include <windows.h>
using namespace std;

template <class It, class Func>
void parallel_for_each(It first, It last, Func f, size_t cutoff = 10000) {
    auto len = distance(first, last);
    if (len <= 0) return;

    if (static_cast<size_t>(len) <= cutoff) {
        for_each(first, last, f);
        return;
    }

    It mid = first;
    advance(mid, len / 2);

    auto fut = async(launch::async,
        [first, mid, f, cutoff] { parallel_for_each(first, mid, f, cutoff); });

    parallel_for_each(mid, last, f, cutoff);

    fut.get();
}

int main() {
    vector<int> v(1'000'000, 1);
    parallel_for_each(v.begin(), v.end(), [](int& x) { x += 1; }, 50'000);
    cout << v.front() << " " << v[500000] << " " << v.back() << '\n'; 
    return 0;
}
