#include <iostream>

#include "yamr.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 4)
        {
            std::cerr << "Usage: yamr <src> <mnum> <rnum>" << std::endl;
            return 1;
        }

        std::string src = argv[1];

        int mNum = std::atoi(argv[2]);
        if (mNum == 0)
        {
            std::cerr << "Incorrect mnum:" << mNum << std::endl;
            return 1;
        }

        int rNum = std::atoi(argv[3]);
        if (rNum == 0)
        {
            std::cerr << "Incorrect rnum:" << mNum << std::endl;
            return 1;
        }

        Yamr yamr(src, mNum, rNum);
        yamr.Run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
