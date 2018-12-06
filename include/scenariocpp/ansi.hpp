#pragma once

#include <iostream>

namespace scenariocpp
{
namespace ansi
{

enum class Colour
{
    Default,
    Red,
    Amber,
    Green
};

} /* namespace ansi */
} /* namespace scenariocpp */

std::ostream& operator<<(std::ostream& os, scenariocpp::ansi::Colour colour);
