#include <fstream>
#include <sstream>
#include <string>
#include <bitset>


int32_t day3_part1(const char* input_path)
{
    const int32_t N = 12;

    int32_t counts[N] = {};
    std::string line;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            std::bitset<N> temp;
            iss >> temp;
            int32_t code = (int32_t)temp.to_ulong();

            for (int32_t i = 0; i < N; ++i)
            {
                int32_t val_bit = code & 0x1;
                counts[i] += val_bit ? 1 : -1;
                code >>= 1;
            }
        }
        input_file.close();
    }

    int32_t gamma = 0;
    for (int32_t i = N - 1; i >= 0; --i)
    {
        gamma <<= 1;
        if (counts[i] > 0)
            gamma += 1;
    }

    int32_t epsilon = (~0u >> (32 - N)) & (~gamma);

    return gamma * epsilon;
}