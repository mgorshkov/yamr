#pragma once

#include <vector>
#include <string>
#include <unordered_map>

struct MapFunctor
{
    void operator() (const std::string& line);

    std::vector<std::string> mLines;
};

struct ReduceFunctor
{
    int operator() (const std::string& line);

    std::unordered_map<std::string, int> mCounters; // prefix -> count
};
