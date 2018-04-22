#pragma once

#include <vector>
#include <thread>

#include "defines.h"
#include "containers.h"
#include "functors.h"

class Map
{
public:
    Map(const std::string& aSrcFileName, int aThreadsCount, std::vector<MapContainer>& aMapContainers);

    void Run(const Offsets& aOffsets);

private:
    void WaitThreads();
    void ThreadProc(uintmax_t aMinOffset, uintmax_t aMaxOffset, int aIndex);
    void Worker(unsigned long long aMinOffset, unsigned long long aMaxOffset, int aIndex);

    std::string mSrcFileName;
    int mThreadsCount;

    std::vector<std::thread> mThreads;
    std::vector<MapContainer>& mMapContainers;
};
