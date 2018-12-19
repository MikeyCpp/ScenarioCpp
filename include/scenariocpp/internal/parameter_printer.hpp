#pragma once

#include <iostream>

namespace scenariocpp
{

template<typename T>
inline std::ostream& PrintParam(std::ostream& os, const T& t)
{
    return os << t;
}

inline std::ostream& PrintParam(std::ostream& os, const std::string& str)
{
    return os << "\"" << str << "\"";
}

inline std::ostream& PrintParam(std::ostream& os, const char* str)
{
    return os << "\"" << (str ? str : "") << "\"";
}

} /* namespace scenariocpp */
