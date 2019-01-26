#include <iostream>
#include "function.h"

void func() {
    std::cout << "func" << std::endl;
}

void hulk() {
    std:: cout << "hulk" << std::endl;
}

int main() {

    struct test {
        test();
        //test(const test&) = delete;
        test(test&&) = delete;
        void operator()() const { std::cout<< "test" << std::endl; }
    } t;


    function<void()> f(func);
    f = t;
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