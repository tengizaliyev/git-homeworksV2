#include <iostream>
#include <string>
#include <algorithm>
#include <utility> 

class big_integer {
private:
    std::string value;

public:

    big_integer() = default;

    big_integer(const std::string& str) : value(str) {}

    big_integer(std::string&& str) noexcept : value(std::move(str)) {}

    big_integer(const big_integer& other) : value(other.value) {}

    big_integer& operator=(big_integer&& other) noexcept {
        if (this != &other) {
            value = std::move(other.value);
        }
        return *this;
    }

    big_integer& operator=(const big_integer& other) {
        if (this != &other) {
            value = other.value;
        }
        return *this;
    }

    big_integer operator+(const big_integer& rhs) const {
        std::string a = value;
        std::string b = rhs.value;

        if (a.size() < b.size())
            a.insert(a.begin(), b.size() - a.size(), '0');
        else if (b.size() < a.size())
            b.insert(b.begin(), a.size() - b.size(), '0');

        std::string result;
        int carry = 0;

        for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
            int sum = (a[i] - '0') + (b[i] - '0') + carry;
            carry = sum / 10;
            result.push_back((sum % 10) + '0');
        }

        if (carry) result.push_back('1');

        std::reverse(result.begin(), result.end());
        return big_integer(result);
    }

    big_integer operator*(int multiplier) const {
        if (multiplier == 0) return big_integer("0");

        std::string result;
        int carry = 0;

        for (int i = static_cast<int>(value.size()) - 1; i >= 0; --i) {
            int product = (value[i] - '0') * multiplier + carry;
            carry = product / 10;
            result.push_back((product % 10) + '0');
        }

        while (carry) {
            result.push_back((carry % 10) + '0');
            carry /= 10;
        }

        std::reverse(result.begin(), result.end());
        return big_integer(result);
    }

    friend std::ostream& operator<<(std::ostream& os, const big_integer& num) {
        os << num.value;
        return os;
    }
};

int main() {
    std::cout << "[IN]:" << std::endl;
    auto number1 = big_integer("114575");
    auto number2 = big_integer("78524");

    std::cout << "number1 = " << number1 << std::endl;
    std::cout << "number2 = " << number2 << std::endl;

    auto result = number1 + number2;
    auto multiplied = number1 * 3;

    std::cout << "[OUT]:" << std::endl;
    std::cout << "Sum = " << result << std::endl;
    std::cout << "Multiplied (number1 * 3) = " << multiplied << std::endl;

    return 0;
}
