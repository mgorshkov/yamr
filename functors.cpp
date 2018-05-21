#include <string>

#include "functors.h"

void MapFunctor::AddLine(const std::string& line)
{
    for (std::size_t index = 1; index <= line.length(); ++index)
        mLines.push_back(line.substr(0, index));
}

std::vector<std::string> MapFunctor::operator() () const
{
    return mLines;
}

std::size_t ReduceFunctor::operator() (const std::multiset<std::string>& lines)
{
    std::size_t maxLength = 0;
    for (const std::string& line : lines)
    {
#ifdef DEBUG_PRINT
        std::cout << line << std::endl;
#endif
        int size = ++mCounters[line];
        if (size > 1 && line.length() > maxLength)
        {
#ifdef DEBUG_PRINT
            std::cout << line.length() << std::endl;
#endif
            maxLength = line.length();
        }
    }
    return maxLength + 1;
}



