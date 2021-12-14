#include <fstream>
#include <sstream>
#include <string>


int32_t day2_part1(const char* input_path)
{
    uint32_t count = 0;
    std::string line;
    int32_t hpos=0, depth=0;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            std::string movement;
            int32_t scale;

            iss >> movement >> scale;
            //std::cout << movement << " " << scale << "\n";

            if (movement.compare("forward") == 0) hpos += scale;
            else if (movement.compare("down") == 0) depth += scale;
            else if (movement.compare("up") == 0) depth -= scale;
        }
        input_file.close();
    }

    return hpos * depth;
}

int32_t day2_part2(const char* input_path)
{
    uint32_t count = 0;
    std::string line;
    int32_t hpos = 0, depth = 0, aim = 0;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            std::string movement;
            int32_t scale;
            iss >> movement >> scale;

            if (movement.compare("forward") == 0)
            {
                hpos += scale;
                depth += aim * scale;
            }
            else if (movement.compare("down") == 0) aim += scale;
            else if (movement.compare("up") == 0) aim -= scale;
        }
        input_file.close();
    }

    return hpos * depth;
}
