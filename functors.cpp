#include <string>

#include "functors.h"

void MapFunctor::operator() (const std::string& line)
{
    for (std::size_t index = 1; index < line.length(); ++index)
        mLines.push_back(line.substr(0, index));
}

void ReduceFunctor::operator() (const std::string& line)
{
    ++mCounters[line];
}

std::size_t ReduceFunctor::GetMaxLength() const
{
    std::size_t maxLength = 0;
    for (const auto& pair : mCounters)
    {
        if (pair.first.length() > maxLength && pair.second > 1)
            maxLength = pair.first.length();
    }
    return maxLength;
}

