#pragma once

#include "ansi.hpp"

#include <iomanip>
#include <iostream>

namespace scenariocpp
{

struct Logger
{
    static std::ostream& Log(const std::string& identifier,
                             ansi::Colour colour)
           {
               return std::cout << colour
                                << "[ "
                                << std::left
                                << std::setw(15) << identifier
                                << " ] "
                                << ansi::Colour::Default;
           }

    static std::ostream& LogInfo()
    {
        return std::cout << ansi::Colour::Amber
                         << "[-----------------] "
                         << ansi::Colour::Default;
    }
};

} /* logger */
