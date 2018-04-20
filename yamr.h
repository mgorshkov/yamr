#pragma once

#include <string>
#include <vector>
#include <thread>

#include "defines.h"
#include "functors.h"

class Yamr
{
public:
    Yamr(const std::string& aSrcFileName, int aMapThreadsCount, int aReduceThreadsCount);

    void Run();

private:
    void Map(const Offsets& aOffsets);
    void WaitMapThreads();
    void MapThreadProc(uintmax_t aMinOffset, uintmax_t aMaxOffset, int aIndex);
    void MapPart(uintmax_t aMinOffset, uintmax_t aMaxOffset, int aIndex);

    void Shuffle();
    void WaitShuffleThreads();
    void ShuffleThreadProc(int aIndex);
    void ShufflePart(int aIndex);

    std::string mSrcFileName;
    int mMapThreadsCount;
    int mReduceThreadsCount;

    std::vector<MapContainer> mMapContainers;
    std::vector<std::thread> mMapThreads;

    std::vector<ShuffleContainer> mShuffleContainers;
    std::vector<std::thread> mShuffleThreads;
};
