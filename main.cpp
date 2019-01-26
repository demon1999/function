#include <iostream>
#include "function.h"
//#include <functional>

void func() {
    std::cout << "func" << std::endl;
}

void hulk() {
    std:: cout << "hulk" << std::endl;
}


struct test2 {
    test2() = default;
    test2(test2&&) noexcept {
        std::cout << "move constructor\n";
    }
    test2(const test2&) {
        std::cout << "const constructor\n";
    }
    test2& operator=(const test2&) {
        std::cout << "copy =\n";
        return *this;
    }
    test2& operator=(test2&&) {
        std::cout << "move =\n";
        return *this;
    }
    int operator()() {
        return 2;
    }
};

struct test {
    test() = default;

    void operator()() const { std::cout<< "test" << std::endl; }
};

int main() {
    //std::function<int()> g2 = test2();
    const function<int()> gg = test2();
    gg();
    return 0;
}