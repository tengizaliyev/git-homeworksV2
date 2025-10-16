#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

int main() {
    std::string text = "Hello world!!";
    std::map<char, int> frequency;

    for (char ch : text) {
        frequency[ch]++;
    }

    std::vector<std::pair<char, int>> vec(frequency.begin(), frequency.end());

    std::sort(vec.begin(), vec.end(), [](auto& a, auto& b) {
        if (a.second == b.second)
            return a.first < b.first;
        return a.second > b.second;
        });

    for (const auto& [ch, count] : vec) {
        std::cout << ch << ": " << count << std::endl;
    }

    return 0;
}
