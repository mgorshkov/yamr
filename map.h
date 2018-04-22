#pragma once

#include <vector>
#include <thread>

#include "defines.h"
#include "containers.h"
#include "functors.h"

class Map
{
public:
    Map(const std::string& aSrcFileName, int aThreadsCount);

    std::vector<MapContainer>&& Run(const Offsets& aOffsets);

private:
    void WaitThreads();
    void ThreadProc(uintmax_t aMinOffset, uintmax_t aMaxOffset, int aIndex);
    void Worker(uintmax_t aMinOffset, uintmax_t aMaxOffset, int aIndex);

    std::string mSrcFileName;
    int mThreadsCount;

    std::vector<MapContainer> mContainers;
    std::vector<std::thread> mThreads;
    MapFunctor mFunctor;
};
