#include <iostream>
#include <vector>
#include <algorithm> 

int main() {
    std::vector<int> v = { 1, 1, 2, 5, 6, 1, 2, 4 };

    std::cout << "[IN]: ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;

    std::sort(v.begin(), v.end());

    auto last = std::unique(v.begin(), v.end());

    v.erase(last, v.end());

    std::cout << "[OUT]: ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;

    return 0;
}
