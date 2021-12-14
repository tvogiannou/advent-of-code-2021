#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <bitset>
#include <vector>
#include <assert.h>


int32_t GetOnesDifferenceCount(const std::vector<int32_t>& codes,
    const std::vector<int32_t>& indices,
    int32_t bit)
{
    int32_t count = 0;
    for (int32_t index : indices)
    {
        int32_t code = codes[index];
        code >>= bit;
        const int32_t val_bit = code & 0x1;
        count += val_bit ? 1 : -1;
    }

    return count;
}


int32_t day3_part2(const char* input_path)
{
    const int32_t N = 12;

    std::string line;
    std::vector<int32_t> codes;
    codes.reserve(1024);

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            std::bitset<N> temp;
            iss >> temp;
            int32_t code = (int32_t)temp.to_ulong();
            codes.push_back(code);
        }
        input_file.close();
    }

    std::vector<int32_t> indices;
    indices.resize(codes.size());
    for (size_t i = 0; i < indices.size(); ++i)
        indices[i] = i;

    // most common value criterion
    int32_t bit = N - 1;
    while (bit >= 0)
    {
        const int32_t ones_dcount = GetOnesDifferenceCount(codes, indices, bit);
        const int32_t compare_val = ones_dcount >= 0 ? 1 : 0;

        // gather new codes
        std::vector<int32_t> prev_indices = indices;
        indices.clear();
        for (int32_t index : prev_indices)
        {
            int32_t code = codes[index];
            code >>= bit;
            const int32_t val_bit = code & 0x1;
            if (val_bit == compare_val)
                indices.push_back(index);
        }

        if (indices.size() < 2)
            break;

        --bit;
    }
    assert(indices.size() == 1);
    int32_t oxygen = codes[indices.front()];

    // least common value criterion
    indices.resize(codes.size());
    for (size_t i = 0; i < indices.size(); ++i)
        indices[i] = i;
    bit = N - 1;
    while (bit >= 0)
    {
        const int32_t ones_dcount = GetOnesDifferenceCount(codes, indices, bit);
        const int32_t compare_val = ones_dcount < 0 ? 1 : 0;

        // gather new codes
        std::vector<int32_t> prev_indices = indices;
        indices.clear();
        for (int32_t index : prev_indices)
        {
            int32_t code = codes[index];
            code >>= bit;
            const int32_t val_bit = code & 0x1;
            if (val_bit == compare_val)
                indices.push_back(index);
        }

        if (indices.size() < 2)
            break;

        --bit;
    }
    assert(indices.size() == 1);
    int32_t co2 = codes[indices.front()];

    return oxygen * co2;
}