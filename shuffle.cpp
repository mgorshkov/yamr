#include <iostream>
#include <functional>

#include "shuffle.h"
#include "utils.h"

Shuffle::Shuffle(int aMapThreadsCount)
    : mThreadsCount(aMapThreadsCount)
{
}

std::vector<ShuffleContainer>&& Shuffle::Run(std::vector<MapContainer> aMapContainers)
{
    mMapContainers = aMapContainers;

    for (std::size_t index = 0; index < mThreadsCount; ++index)
        mThreads.emplace_back(std::thread(&Shuffle::ThreadProc, this, index));

    WaitThreads();

    return std::move(mShuffleContainers);
}

void Shuffle::WaitThreads()
{
    for (auto& thread : mThreads)
        if (thread.joinable())
            thread.join();
}

void Shuffle::ThreadProc(int aIndex)
{
    try
    {
        Worker(aIndex);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }    
}


std::size_t Shuffle::MapIndex(const std::string& line)
{
    std::hash<std::string> hashFn;
    return hashFn(line) % mThreadsCount;
}

void Shuffle::Worker(int aIndex)
{
    MapContainer& mapContainer = mMapContainers[aIndex];
    for (const auto& line : mapContainer.mStrings)
    {
        auto index = MapIndex(line);
        mShuffleContainers[index].Insert(line);
    }
}
