#include "functors.h"

void MapContainer::operator() (const std::string& line)
{
    mStrings.insert(line);
}

void ShuffleContainer::operator() (const std::string& line)
{
    mStrings.insert(line);
}
