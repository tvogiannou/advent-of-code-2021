#include <fstream>
#include <sstream>
#include <string>


int32_t day1_part1(const char* input_path)
{
    uint32_t prev = ~0x0u;
    uint32_t count = 0;
    std::string line;
    std::ifstream input_file(input_path);
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

    return count;
}