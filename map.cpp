#include <iostream>
#include <fstream>
#include <sstream>

#include "map.h"

Map::Map(const std::string& aSrcFileName, int aMapThreadsCount)
    : mSrcFileName(aSrcFileName)
    , mThreadsCount(aMapThreadsCount)
{
}

std::vector<MapContainer>&& Map::Run(const Offsets& aOffsets)
{
    std::vector<uintmax_t> offsets(aOffsets.begin(), aOffsets.end()); 
    for (std::size_t index = 0; index < offsets.size(); ++index)
    {
        uintmax_t minOffset = offsets[index];
        uintmax_t maxOffset = index < offsets.size() ? offsets[index + 1] : -1;
        mThreads.emplace_back(std::thread(&Map::ThreadProc, this, minOffset, maxOffset, index));
    }
    WaitThreads();
    
    return std::move(mContainers);
}

void Map::WaitThreads()
{
    for (auto& thread : mThreads)
        if (thread.joinable())
            thread.join();
}

void Map::ThreadProc(uintmax_t aMinOffset, uintmax_t aMaxOffset, int aIndex)
{
    try
    {
        Worker(aMinOffset, aMaxOffset, aIndex);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Map::Worker(uintmax_t aMinOffset, uintmax_t aMaxOffset, int aIndex)
{
    std::ifstream f(mSrcFileName);
    f.seekg(aMinOffset);
    std::size_t size = aMaxOffset - aMinOffset;
    std::vector<char> v(size);
    f.read(&v[0], size);
    std::string s(&v[0], &v[size]);
    std::stringstream str(s);
    std::string line;
    MapContainer& container = mContainers[aIndex];
    while (std::getline(str, line))
        mFunctor(line);
    for (const std::string& line : mFunctor.mLines)
        container.Insert(line);
}

