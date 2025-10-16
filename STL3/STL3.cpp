#include <iostream>
#include <stdexcept>

template <typename T>
class MyVector {
private:
    T* data; 
    size_t sz;       
    size_t cap;

public:
    MyVector() : data(nullptr), sz(0), cap(0) {}

    ~MyVector() {
        delete[] data;
    }

    size_t size() const {
        return sz;
    }

    size_t capacity() const {
        return cap;
    }

    void push_back(const T& value) {
        if (sz == cap) {
            size_t newCap = (cap == 0) ? 1 : cap * 2;
            T* newData = new T[newCap];

            for (size_t i = 0; i < sz; ++i) {
                newData[i] = data[i];
            }
     
            delete[] data;

            data = newData;
            cap = newCap;
        }
        data[sz++] = value;
    }

    T& at(size_t index) {
        if (index >= sz) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& at(size_t index) const {
        if (index >= sz) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
};

int main() {
    MyVector<int> vec;

    std::cout << "[IN]:" << std::endl;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);

    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec.at(i) << std::endl;
    }

    std::cout << "[OUT]:" << std::endl;
    std::cout << "size = " << vec.size() << std::endl;
    std::cout << "capacity = " << vec.capacity() << std::endl;
    std::cout << "Element at index 2 = " << vec.at(2) << std::endl;

    return 0;
}
