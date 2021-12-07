#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


int main()
{
    const char* INPUT_PATH = "D:/AoC/puzzles/inputs/day2_input.txt";

    uint32_t count = 0;
    std::string line;
    int32_t hpos=0, depth=0;

    std::ifstream input_file(INPUT_PATH);
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

    std::cout << hpos * depth << '\n';


    return EXIT_SUCCESS;
}