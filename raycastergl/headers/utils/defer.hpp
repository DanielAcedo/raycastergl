// based on https://oded.blog/2017/10/05/go-defer-in-cpp/
#pragma once

#include <functional>

class ScopeGuard {
public:
    template<class Callable>
    ScopeGuard(Callable &&fn): fn_(std::forward<Callable>(fn)) {}

    ScopeGuard(ScopeGuard &&other): fn_(std::move(other.fn_)) {
        other.fn_ = nullptr;
    }

    ~ScopeGuard() {
        // must not throw
        if (fn_) fn_();
    }

    ScopeGuard(const ScopeGuard &) = delete;
    void operator=(const ScopeGuard &) = delete;

private:
    std::function<void()> fn_;
};

#define __defer_CONCAT_(a, b) a ## b
#define __defer_CONCAT(a, b) __defer_CONCAT_(a,b)
#define DEFER_FN(fn) ScopeGuard __defer_CONCAT(__defer__, __LINE__) = fn
#define DEFER(fn) DEFER_FN([&] ( ) { fn ; })
