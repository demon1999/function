#include <iostream>
#include "function.h"

void func() {
    std::cout << "func" << std::endl;
}

void hulk() {
    std:: cout << "hulk" << std::endl;
}

int main() {
    function<void()> f(func);
    //f = func;
    f();
    function<void()> g(hulk);
    g.swap(f);
    g();
    f = g;
    f();
    f = std::move(g);
    f();
    //g();
    f = hulk;
    f();

    f = []() {
        std::cout << "heroin\n";
    };
    f();

    function<void(int)> gg = [](int k) {
        std::cout << k << "\n";
    };
    gg(100);
    return 0;
}