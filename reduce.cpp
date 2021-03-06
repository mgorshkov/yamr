#include <iostream>
#include <fstream>
#include <sstream>

#include "reduce.h"

Reduce::Reduce(int aReduceThreadsCount, std::vector<ShuffleContainer>& aShuffleContainers)
    : mThreadsCount(aReduceThreadsCount)
    , mShuffleContainers(aShuffleContainers)
{
}

void Reduce::Run()
{
    for (std::size_t index = 0; index < mThreadsCount; ++index)
    {
        mThreads.emplace_back(std::thread(&Reduce::ThreadProc, this, index));
    }
    WaitThreads();
}

void Reduce::WaitThreads()
{
    for (auto& thread : mThreads)
        if (thread.joinable())
            thread.join();
}

void Reduce::ThreadProc(int aIndex)
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

void Reduce::Worker(int aIndex)
{
    ShuffleContainer& container = mShuffleContainers[aIndex];
#ifdef DEBUG_PRINT
    std::cout << "Reduce" << aIndex << std::endl;
#endif
    std::stringstream fileName;
    fileName << "reduceresult" << aIndex;
    std::ofstream f(fileName.str());
    ReduceFunctor functor;
    f << functor(container.mStrings);
}

