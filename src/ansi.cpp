#include "scenariocpp/internal/ansi.hpp"

namespace
{

int GetAnsiColourCode(scenariocpp::ansi::Colour colour)
{
    switch(colour)
    {
    case scenariocpp::ansi::Colour::Red:
        return 1;
    case scenariocpp::ansi::Colour::Green:
        return 2;
    case scenariocpp::ansi::Colour::Amber:
        return 3;
    case scenariocpp::ansi::Colour::Default:
        return 0;
    }

    return 0;
}

} /* namespace */

std::ostream& operator<<(std::ostream& os, scenariocpp::ansi::Colour colour)
{
    auto colourCode = GetAnsiColourCode(colour);

    if(colourCode)
        return os << "\033[0;3" + std::to_string(colourCode) + "m";

    return os << "\033[0m";
}
