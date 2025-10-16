#include <iostream>

template <typename T>
class MyUniquePtr {
private:
    T* ptr; 

public:
    explicit MyUniquePtr(T* p = nullptr) : ptr(p) {}

    ~MyUniquePtr() {
        if (ptr) {
            delete ptr;
            std::cout << "destructor called\n";
        }
    }

    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;

    MyUniquePtr(MyUniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const {
        return *ptr;
    }

    T* operator->() const {
        return ptr;
    }

    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    T* get() const {
        return ptr;
    }
};

class Test {
public:
    Test() { std::cout << "Test created\n"; }
    ~Test() { std::cout << "Test destroyed\n"; }
    void hello() { std::cout << "Hello from Test\n"; }
};

int main() {
    std::cout << "[IN]:\n";
    {
        MyUniquePtr<Test> p1(new Test());
        p1->hello();

        MyUniquePtr<Test> p2 = std::move(p1);

        if (!p1.get()) {
            std::cout << "p1 is empty after move\n";
        }

        Test* raw = p2.release();
        std::cout << "Ownership released manually\n";
        delete raw; 
    }

    std::cout << "[OUT]: program finished\n";
    return 0;
}
