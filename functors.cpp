#include <string>

#include "functors.h"

void MapFunctor::operator() (const std::string& line)
{
    for (std::size_t index = 1; index <= line.length(); ++index)
        mLines.push_back(line.substr(0, index));
}

int ReduceFunctor::operator() (const std::string& line)
{
    return ++mCounters[line];
}

