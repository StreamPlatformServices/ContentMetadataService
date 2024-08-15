#pragma once

#include "StdAfx.h"
#include <coroutine>



//TODO: Analyze this structure
template<typename T>
struct Awaitable
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    handle_type coro;

    Awaitable(handle_type h) : coro(h) {}
    ~Awaitable() { if (coro) coro.destroy(); }

    T get() { return coro.promise().value; }

    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<>) { coro.resume(); }
    T await_resume() { return get(); }

    struct promise_type
    {
        T value;
        auto get_return_object() { return Awaitable{ handle_type::from_promise(*this) }; }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void return_value(T v) { value = v; }
        void unhandled_exception() { std::terminate(); }
    };
};
