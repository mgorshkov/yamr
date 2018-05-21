#pragma once

#include <vector>
#include <thread>

#include "defines.h"
#include "containers.h"

class Shuffle
{
public:
    Shuffle(int aMapThreadsCount,
        int aReduceThreadsCount,
        std::vector<MapContainer>& aMapContainers,
        std::vector<ShuffleContainer>& aShuffleContainers);

    void Run();

private:
    void WaitThreads();
    void ThreadProc(int aIndex);
    std::size_t ReduceIndex(const std::string& line);
    void Worker(int aIndex);

    int mMapThreadsCount;
    int mReduceThreadsCount;

    std::vector<MapContainer>& mMapContainers;
    std::vector<ShuffleContainer>& mShuffleContainers;

    std::vector<std::thread> mThreads;
};
