#include <iostream>
#include <vector>

template <typename T>
class Table {
private:
    std::vector<std::vector<T>> data;

public:
    Table(size_t rows, size_t cols)
        : data(rows, std::vector<T>(cols)) {
    }

    std::vector<T>& operator[](size_t index) {
        return data[index];
    }

    const std::vector<T>& operator[](size_t index) const {
        return data[index];
    }

    std::pair<size_t, size_t> Size() const {
        if (data.empty()) return { 0, 0 };
        return { data.size(), data[0].size() };
    }
};

int main() {
    Table<int> table(2, 3);

    table[0][0] = 4;
    table[0][1] = 7;
    table[1][2] = 9;

    std::cout << table[0][0] << std::endl;
    std::cout << table[0][1] << std::endl;
    std::cout << table[1][2] << std::endl;

    auto size = table.Size();
    std::cout << "Rows: " << size.first << ", Cols: " << size.second << std::endl;

    return 0;
}
