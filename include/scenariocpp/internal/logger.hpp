#pragma once

#include "ansi.hpp"

#include <iostream>

namespace scenariocpp
{

struct ErrorLog
{
    ErrorLog() = default;
    ErrorLog(const ErrorLog&) = delete;

    ~ErrorLog()
    {
        std::cout << ansi::Colour::Default << std::endl;
    }
};

struct InfoLog
{
    InfoLog() = default;
    InfoLog(const InfoLog&) = delete;

    ~InfoLog()
    {
        std::cout << ansi::Colour::Default << std::endl;
    }
};

struct Logger
{
    static InfoLog LogInfo(const std::string& identifier = "",
                           ansi::Colour colour = ansi::Colour::Amber);
    static ErrorLog LogError();
};

} /* scenariocpp */

template<typename T>
scenariocpp::ErrorLog&& operator <<(scenariocpp::ErrorLog&& el, const T& t)
{
    std::cout << scenariocpp::ansi::Colour::Red << t;
    return std::move(el);
}

template<typename T>
scenariocpp::InfoLog&& operator <<(scenariocpp::InfoLog&& il, const T& t)
{
    std::cout << t;
    return std::move(il);
}
