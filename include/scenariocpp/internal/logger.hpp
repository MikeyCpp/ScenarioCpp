#pragma once

#include "ansi.hpp"

#include <iostream>

namespace scenariocpp
{

struct Logger
{
    static std::ostream& Log(const std::string& identifier,
                             ansi::Colour colour);

    static std::ostream& LogInfo();
};

} /* namespace scenariocpp */
