#include <iostream>
#include <vector>

template <typename T>
T Sqr(const T& value) {
    return value * value;
}

template <typename T>
std::vector<T> Sqr(const std::vector<T>& vec) {
    std::vector<T> result;
    result.reserve(vec.size());
    for (const auto& element : vec) {
        result.push_back(Sqr(element));
    }
    return result;
}

int main() 
{
    int x = 4;
    std::cout << "[IN]: " << x << std::endl;
    std::cout << "[OUT]: " << Sqr(x) << std::endl;
    std::vector<int> v = { -1, 4, 8 };
    std::cout << "[IN]: ";
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i];
        if (i < v.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    auto result = Sqr(v);

    std::cout << "[OUT]: ";
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i];
        if (i < result.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    return 0;
}
