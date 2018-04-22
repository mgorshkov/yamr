#include <boost/filesystem.hpp>

#include "utils.h"
#include "yamr.h"

Yamr::Yamr(const std::string& aSrcFileName, int aMapThreadsCount, int aReduceThreadsCount)
    : mSrcFileName(aSrcFileName)
    , mMapThreadsCount(aMapThreadsCount)
    , mReduceThreadsCount(aReduceThreadsCount)
    , mMap(aSrcFileName, aMapThreadsCount, mMapContainers)
    , mShuffle(aMapThreadsCount, aReduceThreadsCount, mMapContainers, mShuffleContainers)
    , mReduce(aReduceThreadsCount, mShuffleContainers)
{
    if (!boost::filesystem::exists(aSrcFileName))
        throw std::runtime_error("File does not exist");
    if (aMapThreadsCount <= 0)
        throw std::runtime_error("Incorrect mnum");
    if (aReduceThreadsCount <= 0)
        throw std::runtime_error("Incorrect rnum");

    mMapContainers.resize(aMapThreadsCount);
    mShuffleContainers.resize(aReduceThreadsCount);
}

void Yamr::Run()
{
    auto offsets = SplitFileAtLineBoundary(mSrcFileName, mMapThreadsCount);
    mMap.Run(offsets);
    mShuffle.Run();
    mReduce.Run();
}

