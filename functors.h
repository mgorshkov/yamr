#include <string>
#include <set>
#include <vector>

struct MapContainer
{
    void operator() (const std::string& line);

    std::multiset<std::string> mStrings;
};

struct ShuffleContainer
{
    void operator() (const std::string& line);

    std::vector<std::string> mStrings;
};
