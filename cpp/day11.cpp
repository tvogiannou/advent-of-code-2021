#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <array>


struct OctopusInfo
{
    std::vector<uint8_t> energies;
    int32_t row_size = 0;
};

using Index8 = std::array<int32_t, 8>;
int32_t GetNeighbours(Index8& neighbours, int32_t index, const OctopusInfo& info)
{
    const int32_t row_size = info.row_size;
    const int32_t num_cells = static_cast<int32_t>(info.energies.size());
    int32_t num_neightbours = 0;

    const int32_t rem = index % row_size;
    const bool left = rem > 0;
    const bool right = rem < row_size - 1;
    const bool up = index >= row_size;
    const bool down = index < num_cells - row_size;

    if (left)
        neighbours[num_neightbours++] = index - 1;
    if (right)
        neighbours[num_neightbours++] = index + 1;
    if (up)
        neighbours[num_neightbours++] = index - row_size;
    if (down)
        neighbours[num_neightbours++] = index + row_size;
    if (left && up)
        neighbours[num_neightbours++] = index - row_size - 1;
    if (right && up)
        neighbours[num_neightbours++] = index - row_size + 1;
    if (left && down)
        neighbours[num_neightbours++] = index + row_size - 1;
    if (right && down)
        neighbours[num_neightbours++] = index + row_size + 1;

    return num_neightbours;
}

void ProcessFlash(OctopusInfo& info, std::unordered_set<int32_t>& flashed, int32_t index)
{
    std::vector<int32_t> to_check;
    to_check.push_back(index);

    while (!to_check.empty())
    {
        int32_t _index = to_check.back();
        to_check.pop_back();

        // ignore if depleted
        if (flashed.count(_index) > 0)
            continue;

        if (info.energies[_index] > 9)
        {
            flashed.emplace(_index);
            
            // increase energy in neighbouring octopuses
            Index8 neighbours = {};
            int32_t num_neightbours = GetNeighbours(neighbours, _index, info);
            for (int32_t i = 0; i < num_neightbours; ++i)
            {
                int32_t neighbour_index = neighbours[i];
                uint8_t& energy_level = info.energies[neighbour_index];
                if (energy_level <= 9) ++energy_level;

                to_check.push_back(neighbour_index);
            }
        }
    }
}

int32_t AdvanceStep(OctopusInfo& info)
{
    // increase energies by one
    for (uint8_t& e : info.energies)
        ++e;

    // find the flashed ones
    std::unordered_set<int32_t> flashed;
    for (int32_t i = 0; i < info.energies.size(); ++i)
    {
        if (info.energies[i] > 9)
            ProcessFlash(info, flashed, i);
    }

    // set the flashed ones to 0 and return their count
    for (auto it = flashed.begin(); it != flashed.end(); ++it)
        info.energies[*it] = 0;

    return static_cast<int32_t>(flashed.size());
}

int32_t day11_part1(const char* input_path)
{
    const int32_t num_steps = 100;
    OctopusInfo info;
    info.energies.reserve(1024);

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
                info.energies.emplace_back((uint8_t)val - 48);
                ++info.row_size;
            }
        }

        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            char val;
            while (iss >> val)
                // convert ASCII code for 0-9 to number
                info.energies.emplace_back((uint8_t)val - 48);
        }
        input_file.close();
    }

    int32_t count = 0;
    for (int32_t i = 0; i < num_steps; i++)
        count += AdvanceStep(info);

    return count;
}

int32_t day11_part2(const char* input_path)
{
    OctopusInfo info;
    info.energies.reserve(1024);

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
                info.energies.emplace_back((uint8_t)val - 48);
                ++info.row_size;
            }
        }

        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            char val;
            while (iss >> val)
                // convert ASCII code for 0-9 to number
                info.energies.emplace_back((uint8_t)val - 48);
        }
        input_file.close();
    }

    int32_t i;
    for (i = 0; ; i++)
        if (AdvanceStep(info) == info.energies.size())
            break;

    return i+1;
}
