#include <assert.h>
#include <boost/filesystem.hpp>
#include <cmath>
#include <fstream>
#include <iostream>

#include "utils.h"

static uintmax_t GetPrevNewLineOffset(uintmax_t aOffset, std::ifstream& f)
{
    if (aOffset == 0)
        return 0;
    char sym;
    f.seekg(aOffset);
    do
    {
        f.read(&sym, 1);
        f.seekg(-2, std::ios_base::cur);
#ifdef DEBUG_PRINT
        std::cout << "GetPrevNewLineOffset, sym=" << sym << ", offset=" << f.tellg() << ";" << std::endl;
#endif
    }
    while (sym != '\n' && sym != '\r' && f.tellg() > 0);
    if (f.tellg() == 0)
        return 0;

    f.seekg(1, std::ios_base::cur);
    return f.tellg();
}

Offsets SplitFileAtLineBoundary(const std::string& aFileName, int aParts)
{
    assert (aParts > 0);
    unsigned long long fileSize = boost::filesystem::file_size(aFileName);
#ifdef DEBUG_PRINT
    std::cout << "SplitFileAtLineBoundary, fileSize=" << fileSize << ";" << std::endl;
#endif
    unsigned long long partSize =
        static_cast<unsigned long long>(std::ceil(static_cast<double>(fileSize) / aParts));

#ifdef DEBUG_PRINT
    std::cout << "SplitFileAtLineBoundary 1, partSize=" << partSize << ";" << std::endl;
#endif

    Offsets offsets;
    std::ifstream f(aFileName, std::ios::in | std::ios::binary);
    uintmax_t offset = 0;
    offsets.emplace(offset);
    offset += partSize;
    while (offset < fileSize)
    {
#ifdef DEBUG_PRINT
        std::cout << "SplitFileAtLineBoundary 2, offset=" << offset << ";" << std::endl;
#endif
        uintmax_t prevNewLineOffset = GetPrevNewLineOffset(offset, f);
#ifdef DEBUG_PRINT
        std::cout << "SplitFileAtLineBoundary 3, offset=" << prevNewLineOffset << std::endl;
#endif
        f.seekg(prevNewLineOffset);
        char sym;
        do
        {
            f.read(&sym, 1);
#ifdef DEBUG_PRINT
            std::cout << "SplitFileAtLineBoundary 4, sym=" << sym << std::endl;
#endif
        }
        while ((sym == '\n' || sym == '\r') && f.tellg() < fileSize);
        f.seekg(-1, std::ios_base::cur);

        auto result = offsets.emplace(f.tellg());
#ifdef DEBUG_PRINT
        std::cout << "SplitFileAtLineBoundary 5, offset=" << f.tellg() << ";" << std::endl;
#endif
        if (!result.second)
            throw std::runtime_error("zero size part");
        offset += partSize;
    }
    return offsets;
}
