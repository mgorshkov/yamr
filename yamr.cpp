#include <boost/filesystem.hpp>
#include <iostream>
#include <thread>
#include <sstream>
#include <vector>

#include "yamr.h"

#include "utils.h"

Yamr::Yamr(const std::string& aSrcFileName, int aMapThreadsCount, int aReduceThreadsCount)
    : mSrcFileName(aSrcFileName)
    , mMapThreadsCount(aMapThreadsCount)
    , mReduceThreadsCount(aReduceThreadsCount)
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
    Map(offsets);
    Shuffle();
}

void Yamr::Map(const Offsets& aOffsets)
{
    std::vector<uintmax_t> offsets(aOffsets.begin(), aOffsets.end()); 
    for (std::size_t index = 0; index < offsets.size(); ++index)
    {
        uintmax_t minOffset = offsets[index];
        uintmax_t maxOffset = index < offsets.size() ? offsets[index + 1] : -1;
        mMapThreads.emplace_back(std::thread(MapThreadProc, this, minOffset, maxOffset, index));
    }
    WaitMapThreads();
}

void Yamr::WaitMapThreads()
{
    for (auto& thread : mMapThreads)
        if (thread.joinable())
            thread.join();
}

void Yamr::Shuffle()
{
    for (std::size_t index = 0; index < mMapThreadsCount; ++index)
        mShuffleThreads.emplace_back(std::thread(ShuffleThreadProc, this, index));

    WaitShuffleThreads();
}

void Yamr::WaitShuffleThreads()
{
    for (auto& thread : mShuffleThreads)
        if (thread.joinable())
            thread.join();
}

void Yamr::MapThreadProc(Yamr* aThis, uintmax_t aMinOffset, uintmax_t aMaxOffset, int aIndex)
{
    try
    {
        aThis->MapPart(aMinOffset, aMaxOffset, aIndex);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Yamr::MapPart(uintmax_t aMinOffset, uintmax_t aMaxOffset, int aIndex)
{
    std::ifstream f(mSrcFileName);
    f.seekg(aMinOffset);
    std::size_t size = aMaxOffset - aMinOffset;
    std::vector<char> v(size);
    f.read(&v[0], size);
    std::string s(&v[0], &v[size]);
    std::stringstream str(s);
    std::string line;
    MapContainer& aMapContainer = mMapContainers[aIndex];
    while (std::getline(str, line))
        aMapContainer(line);
}

void Yamr::ShuffleThreadProc(Yamr* aThis, int aIndex)
{
    try
    {
        aThis->ShufflePart(aIndex);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }    
}

void Yamr::ShufflePart(int aIndex)
{
}
