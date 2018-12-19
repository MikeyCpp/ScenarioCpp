#include "scenariocpp/internal/logger.hpp"

#include <iomanip>

std::ostream &scenariocpp::Logger::Log(const std::string &identifier, scenariocpp::ansi::Colour colour)
{
    return std::cout << colour
                     << "[ "
                     << std::left
                     << std::setw(15) << identifier
                     << " ] "
                     << ansi::Colour::Default;
}

std::ostream &scenariocpp::Logger::LogInfo()
{
    return std::cout << ansi::Colour::Amber
                     << "[-----------------] "
                     << ansi::Colour::Default;
}
