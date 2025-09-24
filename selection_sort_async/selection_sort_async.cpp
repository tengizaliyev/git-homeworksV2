#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <numeric>
#include <vector>
#include <windows.h>
using namespace std;

template <class RandomIt, class Compare>
void find_min_index_async(RandomIt first, RandomIt last,
    promise<ptrdiff_t> p, Compare comp) {
    if (first == last) { p.set_value(-1); return; }
    auto min_it = first;
    for (auto it = next(first); it != last; ++it) {
        if (comp(*it, *min_it)) min_it = it;
    }
    p.set_value(distance(first, min_it)); 
}

template <class RandomIt, class Compare = less<typename iterator_traits<RandomIt>::value_type>>
void selection_sort_async(RandomIt first, RandomIt last, Compare comp = Compare{}) {
    const auto n = distance(first, last);
    if (n <= 1) return;

    for (auto i = first; i != last; ++i) {
        promise<ptrdiff_t> p;
        future<ptrdiff_t> f = p.get_future();

        async(launch::async, find_min_index_async<RandomIt, Compare>, i, last, move(p), comp);

        ptrdiff_t rel_index = f.get();
        if (rel_index >= 0) {
            auto min_it = next(i, rel_index);
            if (min_it != i) iter_swap(i, min_it);
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    vector<int> v(20);
    iota(v.begin(), v.end(), 1);
    shuffle(v.begin(), v.end(), mt19937{ random_device{}() });

    cout << "До:  ";
    for (int x : v) cout << x << ' '; cout << '\n';

    selection_sort_async(v.begin(), v.end());

    cout << "После: ";
    for (int x : v) cout << x << ' '; cout << '\n';

    return 0;
}
