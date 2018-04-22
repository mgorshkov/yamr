#include <boost/filesystem.hpp>

#include "utils.h"
#include "yamr.h"

Yamr::Yamr(const std::string& aSrcFileName, int aMapThreadsCount, int aReduceThreadsCount)
    : mSrcFileName(aSrcFileName)
    , mMapThreadsCount(aMapThreadsCount)
    , mReduceThreadsCount(aReduceThreadsCount)
    , mMap(aSrcFileName, aMapThreadsCount)
    , mShuffle(aMapThreadsCount)
    , mReduce(aReduceThreadsCount)
{
    if (!boost::filesystem::exists(aSrcFileName))
        throw std::runtime_error("File does not exist");
    if (aMapThreadsCount <= 0)
        throw std::runtime_error("Incorrect mnum");
    if (aReduceThreadsCount <= 0)
        throw std::runtime_error("Incorrect rnum");
}

void Yamr::Run()
{
    auto offsets = SplitFileAtLineBoundary(mSrcFileName, mMapThreadsCount);
    auto mapContainers = mMap.Run(offsets);
    auto shuffleContainers = mShuffle.Run(mapContainers);
    mReduce.Run(shuffleContainers);
}

