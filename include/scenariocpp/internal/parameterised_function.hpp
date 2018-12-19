#pragma once

namespace scenariocpp
{

namespace detail
{

template<typename T>
struct ParameterisedFunction {};

template<typename T>
struct ParameterisedFunction<void(T)>
{
    using type = T;
};

} /* namespace detail */

} /* namespace scenariocpp */
