#include "containers.h"

void MapContainer::Insert(const std::string& str)
{
    std::lock_guard<std::mutex> lock(mMapMutex);
    mStrings.insert(str);
}

std::mutex mMapMutex;

void ShuffleContainer::Insert(const std::string& str)
{
    std::lock_guard<std::mutex> lock(mShuffleMutex);
    mStrings.insert(str);
}

std::mutex mShuffleMutex;