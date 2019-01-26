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

    function() = default;

    ~function() = default;

    function(std::nullptr_t): storage_() {}

    function(const function& other) {
        storage_(other.storage_);
    }

    function(function&& other): storage_(std::move(other.storage_)){};

    template <class T>
    function(T t) : storage_(t){}

    void swap(function &other) {
        storage_.swap(other.storage_);
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
        return !(storage_.is_small && storage_.callable == nullptr);
    }

    ReturnValue operator()(Args... args) const {
        if (!storage_.is_small && storage_.callable == nullptr)
            throw std::bad_function_call();
        return storage_->Invoke(std::forward<Args>(args)...);
    }

private:
    class ICallable {
    public:
        virtual ~ICallable() = default;
        virtual ReturnValue Invoke(Args...) const = 0;
        virtual ICallable* clone() = 0;
        virtual void cloneTo(unsigned char *) const = 0;
        virtual void moveTo(unsigned char *) const = 0;
    };

    template <typename T>
    class CallableT : public ICallable {
    public:
        CallableT(const T& t)
                : t_(t) {
        }

        CallableT(const T&& t)
                : t_(std::move(t)) {
        }

        ~CallableT() override = default;

        CallableT* clone() override {
            return new CallableT(t_);
        }

        void cloneTo(unsigned char *data) const override{
            new (data) CallableT(t_);
        }

        void moveTo(unsigned char *data) const override {
            new (data) CallableT(std::move(t_));
        }

        ReturnValue Invoke(Args... args) const override {
            return t_(args...);
        }

    private:
        T t_;
    };



    struct storage {
        static const int small_size = sizeof(ICallable*);
        union {
            unsigned char data[small_size];
            ICallable* callable;
        };
        bool is_small;
        storage() {
            is_small = false;
            callable = nullptr;
        }

        ~storage() {
            if (is_small) {
                reinterpret_cast<ICallable*> (&data)->~ICallable();
            } else {
                if (callable != nullptr)
                    delete callable;
            }
        }

        storage(storage&& other) {
            is_small = other.is_small;
            if (!is_small) {
                callable = other.callable;
            } else {
                reinterpret_cast<ICallable*>(&other.data)->move_to(data);
            }
            other.is_small = false;
            other.callable = nullptr;
        }

        storage(const storage& other) {
            is_small = other.is_small;
            if (!is_small) {
                if (other.callable != nullptr)
                    callable = other.callable->clone();
                else
                    callable = nullptr;
            } else {
                reinterpret_cast<ICallable*>(&other.data)->cloneTo(data);
            }
        }


        template <class T>
        storage(const T& t) {
            if (sizeof(CallableT < T > ) < small_size) {
                is_small = true;
                new(data) CallableT <T>(t);
            } else {
                is_small = false;
                callable = new CallableT <T>(t);
            }
        }

        const ICallable* operator->() const {
            if (!is_small) {
                return callable;
            } else {
                return reinterpret_cast<ICallable*>(&data);
            }
        }

        ICallable* operator->() {
            if (!is_small) {
                return callable;
            } else {
                return reinterpret_cast<ICallable*>(&data);
            }
        }

        void swap(storage& other) {
            if (!is_small && !other.is_small) {
                std::swap(other.callable, callable);
            }

            if (is_small && !other.is_small) {
                ICallable* buffer = other.callable;
                reinterpret_cast<ICallable*>(&data)->move_to(other.data);
                callable = buffer;
            }

            if (!is_small && other.is_small) {
                ICallable* buffer = callable;
                reinterpret_cast<ICallable*>(&other.data)->move_to(data);
                other.callable = buffer;
            }

            if (is_small && other.is_small) {
                unsigned char c[small_size];
                reinterpret_cast<ICallable*>(&data)->move_to(c);
                reinterpret_cast<ICallable*>(&other.data)->move_to(data);
                reinterpret_cast<ICallable*>(&c)->move_to(other.data);
            }
            std::swap(is_small, other.is_small);
        }
    } storage_;
    //std::unique_ptr<ICallable> callable_;
};


#endif //FUNCTION_FUNCTION_H
