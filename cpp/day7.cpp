#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>


int32_t day7_part1(const char* input_path)
{
    std::multiset<int32_t> positions;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;
        std::getline(input_file, line);
        {
            std::istringstream iss(line);
            int32_t val;
            char delim;
            while (iss >> val)
            {
                positions.emplace(val);
                iss >> delim;
            }
        }
        input_file.close();
    }

    auto median_it = positions.begin();
    std::advance(median_it, positions.size() / 2);
    int32_t median = *median_it;
    std::cout << median << "\n";

    int32_t count = 0;
    for (auto it = positions.begin(); it != positions.end(); ++it)
        count += std::abs(*it - median);

    return count;
}

int32_t ComputeFuelCost(const std::vector<int32_t>& positions, int32_t val)
{
    int32_t cost = 0;
    for (int32_t v : positions)
    {
        int32_t n = std::abs(v - val);
        cost += (n * (n + 1)) / 2;
    }

    return cost;
}

int32_t day7_part2(const char* input_path)
{
    std::vector<int32_t> positions;
    uint64_t sum = 0;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;
        std::getline(input_file, line);
        {
            std::istringstream iss(line);
            int32_t val;
            char delim;
            while (iss >> val)
            {
                positions.emplace_back(val);
                sum += (uint64_t)val;
                iss >> delim;
            }
        }
        input_file.close();
    }

    float optimalf = (float)sum / (float)positions.size();
    //std::cout << optimalf << "\n";

    int32_t cost_floor = ComputeFuelCost(positions, (int32_t)optimalf);
    int32_t cost_ceil = ComputeFuelCost(positions, (int32_t)optimalf + 1);
    int32_t cost = std::min(cost_floor, cost_ceil);

    return cost;
}
