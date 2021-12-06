#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


uint32_t to_uint32(const std::string& str)
{
    std::stringstream ss(str);
    uint32_t res;
    ss >> res;
    return res;
}

// Queue to keep track of 3 items
// pushes new to the front while discarding the item at the back
struct Queue3
{
    // maps front of queue to the back
    static constexpr uint32_t _BACK[] = { 1u, 2u, 0u };

    uint32_t _vals[3];
    uint32_t _front;

    void push_front(uint32_t val) { _front = _BACK[_front]; _vals[_front] = val; }
    uint32_t back() { return _vals[_BACK[_front]]; }
};


int main()
{
    const char* INPUT_PATH = "D:/AoC/puzzles/inputs/day1_input.txt";

    Queue3 q = { { ~0x0u, ~0x0u, ~0x0u }, 0u };

    uint32_t count = 0;
    std::string line;

    std::ifstream input_file(INPUT_PATH);
    if (input_file.is_open())
    {
        std::getline(input_file, line); q.push_front(to_uint32(line));
        std::getline(input_file, line); q.push_front(to_uint32(line));
        std::getline(input_file, line); q.push_front(to_uint32(line));

        while (std::getline(input_file, line))
        {
            uint32_t res = to_uint32(line);

            // only need to compare new value with the value at the back of the queue
            if (res > q.back())
                ++count;
            q.push_front(res);
        }
        input_file.close();
    }

    std::cout << count << '\n';

    return 0;
}