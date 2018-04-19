#include <string>
#include <set>

struct MapContainer
{
    void operator() (const std::string& line);

    std::multiset<std::string> mStrings;
};
