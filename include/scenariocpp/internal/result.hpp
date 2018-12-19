#pragma once

#include <string>

namespace scenariocpp
{
namespace detail
{

template<typename T>
class ReservedMemory
{
public:
    ReservedMemory() = default;
    ~ReservedMemory() = default;

          T* get()       { return reinterpret_cast<T*>(t_); }
    const T* get() const { return reinterpret_cast<T*>(t_); }

private:
    char t_[sizeof(T)];
};

template<typename T>
class OptionalInitialise
{
public:
    OptionalInitialise() = default;

    template<typename ...Args>
    OptionalInitialise(Args&&... args)
    {
        initialized_ = ::new(t_.get()) T{std::forward<Args>(args)...};
    }

    ~OptionalInitialise()
    {
        if (initialized_)
            t_.get()->~T();
    }

          T& get()       { return *t_.get(); }
    const T& get() const { return *t_.get(); }

private:
    bool initialized_ = false;
    ReservedMemory<T> t_;
};

} /* namespace detail */

class Result
{
public:
    Result() = default;

    template<typename ...Args>
    Result(Args&&... args)
        : success_{false},
          error_{std::forward<Args>(args)...} {}

    bool Success() const { return success_; }

    const std::string& Error()
    {
        return error_.get();
    }

private:
    bool success_ = true;
    detail::OptionalInitialise<std::string> error_;
};

} /* namespace scenariocpp */
