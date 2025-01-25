#pragma once

#include <memory>
#include <stdexcept>

namespace ctl
{
template <typename T>
class cxptr : public std::weak_ptr<T> {
public:

    using std::weak_ptr<T>::weak_ptr;

    // Check if the managed object is still alive
    bool is_valid() const {
        return !this->expired();
    }

    // Access the managed object, throws if it no longer exists
    T& get() {
        if (auto sp = this->lock()) {
            return *sp;
        }
        throw std::runtime_error("The object pointed to by the weak_ptr no longer exists.");
    }

    // Access the managed object with a const reference
    const T& get() const {
        if (auto sp = this->lock()) {
            return *sp;
        }
        throw std::runtime_error("The object pointed to by the weak_ptr no longer exists.");
    }

    // Operator overloading for easier access
    T* operator->() {
        if (auto sp = this->lock()) {
            return sp.get();
        }
        throw std::runtime_error("The object pointed to by the weak_ptr no longer exists.");
    }

    const T* operator->() const {
        if (auto sp = this->lock()) {
            return sp.get();
        }
        throw std::runtime_error("The object pointed to by the weak_ptr no longer exists.");
    }

    // Operator overloading for dereferencing
    T& operator*() {
        return get();
    }

    const T& operator*() const {
        return get();
    }

    // Get a shared_ptr if the object still exists (instead of direct lock())
    std::shared_ptr<T> get_shared_ptr() {
        return this->lock();
    }

    operator std::shared_ptr<T>() const {
        return this->lock();
    }
};
} // namespace cnr
