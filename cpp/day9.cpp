#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <assert.h>


bool IsLow(uint8_t c, uint8_t n0, uint8_t n1) { return c < n0 && c < n1; }
bool IsLow(uint8_t c, uint8_t n0, uint8_t n1, uint8_t n2) { return IsLow(c, n0, n1) && c < n2; }
bool IsLow(uint8_t c, uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3) { return IsLow(c, n0, n1) && IsLow(c, n2, n3); }

struct RiskMap
{
    RiskMap(const std::vector<uint8_t>& _heights) : heights(_heights) {}

    const std::vector<uint8_t>& heights;

    int32_t CheckRisk(int32_t index, int32_t offset0, int32_t offset1) const
    {
        if (IsLow(heights[index], heights[index + offset0], heights[index + offset1]))
            return (int32_t)heights[index] + 1;

        return 0;
    }

    int32_t CheckRisk(int32_t index, int32_t offset0, int32_t offset1, int32_t offset2) const
    {
        if (IsLow(heights[index], heights[index + offset0], 
            heights[index + offset1], heights[index + offset2]))
            return (int32_t)heights[index] + 1;

        return 0;
    }

    int32_t CheckRisk(int32_t index, int32_t offset0, int32_t offset1, int32_t offset2, int32_t offset3) const
    {
        if (IsLow(heights[index], heights[index + offset0], heights[index + offset1], 
            heights[index + offset2], heights[index + offset3]))
            return (int32_t)heights[index] + 1;

        return 0;
    }

    int32_t ComputeBasin(int32_t index, int32_t row_size) const
    {
        std::stack<int32_t> to_check;
        to_check.push(index);

        int32_t size = 0;
        std::unordered_set<int32_t> visited_locations;
        while (!to_check.empty())
        {
            int32_t loc_idx = to_check.top();
            to_check.pop();

            if (heights[loc_idx] == 9 || visited_locations.count(loc_idx) > 0)
                continue;

            visited_locations.emplace(loc_idx);
            ++size;

            // add neighbours
            if (loc_idx % row_size > 0)
                to_check.push(loc_idx - 1);
            if (loc_idx % row_size < row_size - 1)
                to_check.push(loc_idx + 1);
            if (loc_idx >= row_size)
                to_check.push(loc_idx - row_size);
            if (loc_idx < static_cast<int32_t>(heights.size()) - row_size)
                to_check.push(loc_idx + row_size);
        }

        return size;
    }
};


int32_t day9_part1(const char* input_path)
{
    int32_t count = 0;
    std::vector<uint8_t> heights;
    heights.reserve(1024);
    int32_t row_length = 0;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;

        // use first line to compute row length
        {
            std::getline(input_file, line);
            std::istringstream iss(line);
            char val;
            while (iss >> val)
            {
                // convert ASCII code for 0-9 to number
                heights.emplace_back((uint8_t)val - 48);
                ++row_length;
            }
        }

        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            char val;
            while (iss >> val)
                // convert ASCII code for 0-9 to number
                heights.emplace_back((uint8_t)val - 48);
        }
        input_file.close();
    }

    const int32_t num_rows = static_cast<int32_t>(heights.size()) / row_length;
    int32_t index = 0;
    RiskMap risk_map(heights);

    // first row
    {
        // top left corner
        count += risk_map.CheckRisk(index++, 1, row_length);

        while (index < row_length - 1)
            count += risk_map.CheckRisk(index++, 1, -1, row_length);

        // top right corner
        count += risk_map.CheckRisk(index++, -1, row_length);
    }

    for (size_t row = 1; row < num_rows - 1; ++row)
    {
        // first row element
        count += risk_map.CheckRisk(index++, 1, -row_length, row_length);

        while (index < (row + 1) * row_length - 1)
            count += risk_map.CheckRisk(index++, 1, -1, -row_length, row_length);

        // last row element
        count += risk_map.CheckRisk(index++, -1, -row_length, row_length);
    }

    // last row
    {
        // bottom left corner
        count += risk_map.CheckRisk(index++, 1, -row_length);

        while (index < heights.size() - 1)
            count += risk_map.CheckRisk(index++, 1, -1, -row_length);

        // bottom right corner
        count += risk_map.CheckRisk(index++, -1, -row_length);
    }

    assert(index == heights.size());

    return count;
}


void AddBasin(const RiskMap& risk_map, int32_t index, int32_t row_length, std::vector<int32_t> &top3)
{
    int32_t basin = risk_map.ComputeBasin(index, row_length);
    if (basin > top3[0])
    {
        top3[0] = basin;
        std::sort(top3.begin(), top3.end());
    }
}

int32_t day9_part2(const char* input_path)
{
    int32_t count = 0;
    std::vector<uint8_t> heights;
    heights.reserve(1024);
    int32_t row_length = 0;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;

        // use first line to compute row length
        {
            std::getline(input_file, line);
            std::istringstream iss(line);
            char val;
            while (iss >> val)
            {
                // convert ASCII code for 0-9 to number
                heights.emplace_back((uint8_t)val - 48);
                ++row_length;
            }
        }

        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            char val;
            while (iss >> val)
                // convert ASCII code for 0-9 to number
                heights.emplace_back((uint8_t)val - 48);
        }
        input_file.close();
    }

    const int32_t num_rows = static_cast<int32_t>(heights.size()) / row_length;
    int32_t index = 0;
    RiskMap risk_map(heights);

    std::vector<int32_t> top3;
    top3.resize(3, 0);

    // first row
    {
        // top left corner
        if (risk_map.CheckRisk(index, 1, row_length) > 0)
            AddBasin(risk_map, index, row_length, top3);
        ++index;

        while (index < row_length - 1)
        {
            if (risk_map.CheckRisk(index, 1, -1, row_length) > 0)
                AddBasin(risk_map, index, row_length, top3);
            ++index;
        }

        // top right corner
        if (risk_map.CheckRisk(index, -1, row_length) > 0)
            AddBasin(risk_map, index, row_length, top3);
        ++index;
    }

    for (size_t row = 1; row < num_rows - 1; ++row)
    {
        // first row element
        if (risk_map.CheckRisk(index, 1, -row_length, row_length) > 0)
            AddBasin(risk_map, index, row_length, top3);
        ++index;

        while (index < (row + 1) * row_length - 1)
        {
            if (risk_map.CheckRisk(index, 1, -1, -row_length, row_length) > 0)
                AddBasin(risk_map, index, row_length, top3);
            ++index;
        }

        // last row element
        if (risk_map.CheckRisk(index, -1, -row_length, row_length) > 0)
            AddBasin(risk_map, index, row_length, top3);
        ++index;
    }

    // last row
    {
        // bottom left corner
        if (risk_map.CheckRisk(index, 1, -row_length) > 0)
            AddBasin(risk_map, index, row_length, top3);
        ++index;

        while (index < heights.size() - 1)
        {
            if (risk_map.CheckRisk(index, 1, -1, -row_length) > 0)
                AddBasin(risk_map, index, row_length, top3);
            ++index;
        }

        // bottom right corner
        if (risk_map.CheckRisk(index, -1, -row_length) > 0)
            AddBasin(risk_map, index, row_length, top3);
        ++index;
    }

    assert(index == heights.size());

    count = top3[0] * top3[1] * top3[2];

    return count;
}
