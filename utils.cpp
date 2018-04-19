#include <assert.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

#include "utils.h"

Offsets SplitFileAtLineBoundary(const std::string& aFileName, int aParts)
{
    assert (aParts > 0);
    uintmax_t fileSize = boost::filesystem::file_size(aFileName);
#ifdef DEBUG_PRINT
    std::cout << "SplitFileAtLineBoundary, fileSize=" << fileSize << ";" << std::endl;
#endif
    uintmax_t partSize = fileSize / aParts;

#ifdef DEBUG_PRINT
    std::cout << "SplitFileAtLineBoundary 1, partSize=" << partSize << ";" << std::endl;
#endif

    Offsets offsets;
    std::ifstream f(aFileName, std::ios::in | std::ios::binary);
    for (uintmax_t offset = 0; offset < fileSize; offset += partSize)
    {
#ifdef DEBUG_PRINT
        std::cout << "SplitFileAtLineBoundary 2, offset=" << offset << ";" << std::endl;
#endif
        if (offset == 0)
            offsets.emplace(offset);
        else
        {
            f.seekg(offset - 1);
            char sym;
            do
            {
                f.read(&sym, 1);
                f.seekg(-2, std::ios_base::cur);
#ifdef DEBUG_PRINT
                std::cout << "SplitFileAtLineBoundary 3, sym=" << sym << ", offset=" << f.tellg() << ";" << std::endl;
#endif
            }
            while (sym != '\n' && f.tellg() > 0);

            auto result = offsets.emplace(f.tellg());
#ifdef DEBUG_PRINT
            std::cout << "SplitFileAtLineBoundary 4, offset=" << f.tellg() << ";" << std::endl;
#endif
            if (!result.second)
                throw std::runtime_error("zero size part");
        }
    }
    return offsets;
}
