#pragma once

#include "no_params.hpp"

#include <unordered_map>

namespace scenariocpp
{

template<typename T>
class Failure
{
public:
    Failure() = default;
    ~Failure() = default;

    bool HasFailure(const T* pt) const
    {
        auto iter = failures_.find(pt);
        return iter != failures_.end();
    }

    void SetFailure(const T* pt)
    {
        failures_[pt] = true;
    }

private:
    std::unordered_map<const T*,bool> failures_;
};

template<>
class Failure<NoParams>
{
public:
    Failure() = default;
    ~Failure() = default;

    bool HasFailure(const NoParams*) const
    {
        return failure_;
    }

    void SetFailure(const NoParams*)
    {
        failure_ = true;
    }

private:
    bool failure_{false};
};

} /* namespace scenariocpp */
