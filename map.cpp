#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

#include "map.h"

Map::Map(const std::string& aSrcFileName, int aMapThreadsCount, std::vector<MapContainer>& aMapContainers)
    : mSrcFileName(aSrcFileName)
    , mThreadsCount(aMapThreadsCount)
    , mMapContainers(aMapContainers)
{
}

void Map::Run(const Offsets& aOffsets)
{
    std::vector<unsigned long long> offsets(aOffsets.begin(), aOffsets.end());
    for (std::size_t index = 0; index < offsets.size(); ++index)
    {
        uintmax_t minOffset = offsets[index];
        uintmax_t maxOffset = index + 1 < offsets.size() ? offsets[index + 1] : -1;
        mThreads.emplace_back(std::thread(&Map::ThreadProc, this, minOffset, maxOffset, index));
    }
    WaitThreads();
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
    MapFunctor functor;

#ifdef DEBUG_PRINT
    std::cout << "Map" << aIndex << " " << aMinOffset << " " << aMaxOffset << " " << std::endl;
#endif
    assert (aMaxOffset == 1 || aMaxOffset >= aMinOffset);
    std::ifstream f(mSrcFileName);
    f.seekg(aMinOffset);
    std::string line;
    MapContainer& container = mMapContainers[aIndex];

    while (std::getline(f, line) && (aMaxOffset == -1 || static_cast<uintmax_t>(f.tellg()) <= aMaxOffset))
    {
        if (line.length() > 0 && line[line.length() - 1] == '\r')
            line = line.substr(0, line.length() - 1);
#ifdef DEBUG_PRINT
        std::cout << aIndex << " " << line << " " << std::endl;
#endif
        functor.AddLine(line);
    }

    for (const std::string& line : functor())
        container.Insert(line);
}
