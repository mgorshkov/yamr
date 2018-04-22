#include "containers.h"

void MapContainer::Insert(const std::string& str)
{
    mStrings.insert(str);
}

void ShuffleContainer::Insert(const std::string& str)
{
//    std::lock_guard<std::mutex> lock(mMutex);
    mStrings.insert(str);
}

