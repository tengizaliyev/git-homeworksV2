#include <iostream>
#include <vector>
#include <string>
#include <utility>

template <typename T>
void move_vectors(std::vector<T>& from, std::vector<T>& to) {
    to = std::move(from);
}

int main() {
    std::vector<std::string> one = { "test_string1", "test_string2" };
    std::vector<std::string> two;

    std::cout << "[IN]:" << std::endl;
    for (const auto& s : one) {
        std::cout << s << std::endl;
    }

    move_vectors(one, two);

    std::cout << "[OUT]:" << std::endl;
    std::cout << "Vector one (after move):" << std::endl;
    for (const auto& s : one) {
        std::cout << (s.empty() ? "<empty>" : s) << std::endl;
    }

    std::cout << "Vector two (after move):" << std::endl;
    for (const auto& s : two) {
        std::cout << s << std::endl;
    }

    return 0;
}
