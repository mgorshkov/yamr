#pragma once

#include "map.h"
#include "shuffle.h"
#include "reduce.h"

class Yamr
{
public:
    Yamr(const std::string& aSrcFileName, int aMapThreadsCount, int aReduceThreadsCount);

    void Run();

private:
    std::string mSrcFileName;
    int mMapThreadsCount;
    int mReduceThreadsCount;

    std::vector<MapContainer> mMapContainers;
    std::vector<ShuffleContainer> mShuffleContainers;

    Map mMap;
    Shuffle mShuffle;
    Reduce mReduce;
};
