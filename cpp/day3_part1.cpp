#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>


int main()
{
    const char* INPUT_PATH = "D:/AoC/puzzles/inputs/test.txt";
    const int32_t N = 12;

    int32_t counts[N] = {};
    std::string line;

    std::ifstream input_file(INPUT_PATH);
    if (input_file.is_open())
    {
        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            std::bitset<N> temp;
            iss >> temp;
            int32_t code = (int32_t)temp.to_ulong();
            //std::cout << code << "\n";

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
    std::cout << gamma * epsilon << '\n';


    return EXIT_SUCCESS;
}