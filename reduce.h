#pragma once

#include <vector>
#include <thread>

#include "defines.h"
#include "containers.h"
#include "functors.h"

class Reduce
{
public:
    Reduce(int aReduceThreadsCount, std::vector<ShuffleContainer>& aShuffleContainers);

    void Run();

private:
    void WaitThreads();
    void ThreadProc(int aIndex);
    void Worker(int aIndex);

    int mThreadsCount;

    std::vector<ShuffleContainer>& mShuffleContainers;
    std::vector<std::thread> mThreads;
};
