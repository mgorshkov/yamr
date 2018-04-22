#include <iostream>
#include <fstream>
#include <sstream>

#include "reduce.h"

Reduce::Reduce(int aReduceThreadsCount)
    : mThreadsCount(aReduceThreadsCount)
{
}

void Reduce::Run(std::vector<ShuffleContainer> aShuffleContainers)
{
    mShuffleContainers = aShuffleContainers;

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
    std::size_t maxLength = 0;
    for (const std::string& line : container.mStrings)
    {
        auto length = mFunctor(line);
        if (length > maxLength)
            maxLength = length;
    }
    std::stringstream fileName;
    fileName << "reduceresult" << aIndex;
    std::ofstream f(fileName.str());
    f << maxLength;
}

