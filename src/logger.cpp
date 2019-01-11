#include "scenariocpp/internal/logger.hpp"

#include <iomanip>

scenariocpp::InfoLog scenariocpp::Logger::LogInfo(const std::string& identifier,
                                                  ansi::Colour colour)
{
    if(identifier.empty())
    {
        std::cout << colour << "[-----------------] ";
    }
    else
    {
        std::cout << colour
                  << "[ "
                  << std::left
                  << std::setw(15) << identifier
                  << " ] ";
    }

    std::cout << ansi::Colour::Default;
    return {};
}

scenariocpp::ErrorLog scenariocpp::Logger::LogError()
{
     return {};
}
