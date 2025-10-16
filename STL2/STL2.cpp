#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;

    std::vector<int> input;
    input.reserve(n);

    for (int i = 0; i < n; ++i) {
        int value;
        std::cin >> value;
        input.push_back(value);
    }

    std::cout << "[IN]:" << std::endl;
    for (int num : input) {
        std::cout << num << std::endl;
    }

    std::set<int> unique_numbers(input.begin(), input.end());

    std::vector<int> sorted_numbers(unique_numbers.begin(), unique_numbers.end());
    std::sort(sorted_numbers.begin(), sorted_numbers.end(), std::greater<int>());

    std::cout << "[OUT]:" << std::endl;
    for (int num : sorted_numbers) {
        std::cout << num << std::endl;
    }

    return 0;
}
