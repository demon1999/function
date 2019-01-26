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
    return 0;
}