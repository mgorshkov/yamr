#pragma once

#include <vector>
#include <thread>

#include "defines.h"
#include "containers.h"

class Shuffle
{
public:
    Shuffle(int aMapThreadsCount);

    std::vector<ShuffleContainer>&& Run(std::vector<MapContainer> aMapContainers);

private:
    void WaitThreads();
    void ThreadProc(int aIndex);
    std::size_t MapIndex(const std::string& line);
    void Worker(int aIndex);

    int mThreadsCount;

    std::vector<MapContainer> mMapContainers;
    std::vector<ShuffleContainer> mShuffleContainers;
    std::vector<std::thread> mThreads;
};
