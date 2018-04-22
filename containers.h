#pragma once

#include <string>
#include <set>
#include <mutex>

struct MapContainer
{
    void Insert(const std::string& str);

    std::multiset<std::string> mStrings;
};

extern std::mutex mMapMutex;

struct ShuffleContainer
{
    void Insert(const std::string& str);

    std::multiset<std::string> mStrings;
};

extern std::mutex mShuffleMutex;
