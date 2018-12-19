#include "scenariocpp/internal/stringutils.hpp"

namespace
{

bool ShouldSplit(char current, char previous, char next)
{
    if (std::isupper(current)
            && ( std::islower(next) || std::islower(previous)))
    {
        return true;
    }

    if (std::isdigit(current) && !std::isdigit(previous))
    {
        return true;
    }

    return false;
}

std::string SplitCamelCase(std::string str)
{
    if(str.size() <= 3)
        return str;

    str.push_back(' ');

    auto previous = str.begin();
    auto current = previous + 1;
    auto next = current + 1;

    std::string output;

    output.push_back(*previous);

    while(next < str.end())
    {
        if (ShouldSplit(*current, *previous, *next))
        {
            output.push_back(' ');
        }

        output.push_back(*current);

        ++previous;
        ++current;
        ++next;
    }

    return output;
}

std::string StripSnakeCase(std::string str)
{
    for(auto& c : str)
    {
        if(c == '_')
            c = ' ';
    }

    return str;
}

std::string ToLower(std::string str)
{
    for(auto iter = str.begin(); iter != str.end(); ++iter)
    {
        if(!std::isalpha(*iter))
            continue;

        *iter = std::tolower(*iter);
    }

    return str;
}

} /* namespace */

std::string scenariocpp::stringutils::Humanise(std::string str)
{
    return ToLower(SplitCamelCase(StripSnakeCase(std::move(str))));
}

std::string scenariocpp::stringutils::CapitaliseFirst(std::string str)
{
    if(str.empty())
        return str;

    if(!std::isalpha(str.front()))
        return str;

    str.front() = std::toupper(str.front());

    return str;
}

std::string scenariocpp::stringutils::ToUpper(std::string str)
{
    for(auto iter = str.begin(); iter != str.end(); ++iter)
    {
        if(!std::isalpha(*iter))
            continue;

        *iter = std::toupper(*iter);
    }

    return str;
}
