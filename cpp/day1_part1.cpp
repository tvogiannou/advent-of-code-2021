#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


int main()
{
    const char* INPUT_PATH = "D:/AoC/puzzles/inputs/day1_input.txt";

    uint32_t prev = ~0x0u;
    uint32_t count = 0;
    std::string line;
    std::ifstream input_file(INPUT_PATH);
    if (input_file.is_open())
    {
        while (std::getline(input_file, line))
        {
            std::stringstream ss(line);
            uint32_t res;
            ss >> res;

            if (res > prev)
                ++count;
            prev = res;
        }
        input_file.close();
    }

    std::cout << count << '\n';

    return 0;
}