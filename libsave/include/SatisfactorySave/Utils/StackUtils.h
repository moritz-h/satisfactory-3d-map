#pragma once

#include <memory>
#include <stack>

namespace SatisfactorySave {

    template<typename T>
    class StackGuard {
    public:
        StackGuard(std::stack<T>& stack, T value) : stack_(stack) {
            stack_.push(std::move(value));
        }

        ~StackGuard() {
            stack_.pop();
        }

    private:
        std::stack<T>& stack_;
    };

    template<typename T>
    inline std::unique_ptr<StackGuard<T>> make_stack_guard_push(std::stack<T>& stack, T value) {
        return std::make_unique<StackGuard<T>>(stack, std::move(value));
    }

} // namespace SatisfactorySave
