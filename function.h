//
// Created by demon1999 on 26.01.19.
//

#include <iostream>
#include <memory>
#include <cassert>
#include <functional>
#include <utility>

#ifndef FUNCTION_FUNCTION_H
#define FUNCTION_FUNCTION_H


template <typename T>
class function;

template <typename ReturnValue, typename... Args>
class function<ReturnValue(Args...)> {
public:

    function() {
        callable_ = nullptr;
    }

    function(std::nullptr_t) {
        callable_ = nullptr;
    }

    function(const function& other) {
       callable_ = std::unique_ptr<ICallable>(other.callable_->clone());
    }

    function(function&& other): callable_(std::move(other.callable_)){};

    template <class T>
    function(T t) {
        callable_ = std::make_unique<CallableT<T>>(t);
    }

    void swap(function &other) {
        callable_.swap(other.callable_);
    }

    function& operator=(const function& other) {
        function f(other);
        this->swap(f);
        return *this;
    }

    function& operator=(function&& other) {
        function f(std::move(other));
        this->swap(f);
        return *this;
    }

    explicit operator bool() const {
        return callable_ != nullptr;
    }
    ReturnValue operator()(Args... args) const {
        if (callable_ == nullptr)
            throw std::bad_function_call();
        return callable_->Invoke(std::forward<Args>(args)...);
    }

private:
    class ICallable {
    public:
        virtual ~ICallable() = default;
        virtual ReturnValue Invoke(Args...) = 0;
        virtual ICallable* clone() = 0;
    };

    template <typename T>
    class CallableT : public ICallable {
    public:
        CallableT(const T& t)
                : t_(t) {
        }

        ~CallableT() override = default;

        virtual CallableT* clone() {
            return new CallableT(t_);
        }

        ReturnValue Invoke(Args... args) override {
            return t_(args...);
        }

    private:
        T t_;
    };

    std::unique_ptr<ICallable> callable_;
};


#endif //FUNCTION_FUNCTION_H
