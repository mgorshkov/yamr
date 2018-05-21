#include <iostream>
#include <functional>

#include "shuffle.h"
#include "utils.h"

Shuffle::Shuffle(int aMapThreadsCount,
    int aReduceThreadsCount,
    std::vector<MapContainer>& aMapContainers,
    std::vector<ShuffleContainer>& aShuffleContainers)
    : mMapThreadsCount(aMapThreadsCount)
    , mReduceThreadsCount(aReduceThreadsCount)
    , mMapContainers(aMapContainers)
    , mShuffleContainers(aShuffleContainers)
{
}

void Shuffle::Run()
{
    for (std::size_t index = 0; index < mMapThreadsCount; ++index)
        mThreads.emplace_back(std::thread(&Shuffle::ThreadProc, this, index));

    WaitThreads();
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


std::size_t Shuffle::ReduceIndex(const std::string& line)
{
    std::hash<std::string> hashFn;

    return hashFn(line) % mReduceThreadsCount;
}

void Shuffle::Worker(int aIndex)
{
    MapContainer& mapContainer = mMapContainers[aIndex];
    for (const auto& line : mapContainer.mStrings)
    {
        std::size_t index = ReduceIndex(line);

#ifdef DEBUG_PRINT
        std::hash<std::string> hashFn;
        std::cout << "threads=" << mReduceThreadsCount << ", line=" << line << ", hash=" << hashFn(line) % mReduceThreadsCount << ", index=" << index << std::endl;
#endif
        mShuffleContainers[index].Insert(line);
    }
}
