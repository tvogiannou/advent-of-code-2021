#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>


struct Point { int32_t x, y; };

struct Fold
{
    enum FOLD_TYPE: int { X = 0, Y, INVALID } type;
    int32_t val;

    static Fold FromInput(char c, int32_t v)
    {
        Fold f = { INVALID, v };
        if (c == 'y') f.type = Y;
        else if (c == 'x') f.type = X;

        return f;
    }
};

struct DotsArray
{
    std::vector<uint8_t> values;
    int32_t N_x; // active count of columns in X axis
    int32_t N_y; // active count of rows in Y axis
    int32_t N_offset; // initial count of columns in X axis

    void Print() const
    {
        for (int32_t i = 0; i < N_y; i++)
        {
            for (int32_t j = 0; j < N_x; j++)
                std::cout << (values[i * N_offset + j] ? '#' : '.');
            std::cout << "\n";
        }
    }

    int32_t CountNonZero() const
    {
        int32_t count = 0;
        for (int32_t i = 0; i < N_y; i++)
        {
            for (int32_t j = 0; j < N_x; j++)
            {
                const uint8_t c = values[i * N_offset + j];
                if (c) ++count;
            }
        }

        return count;
    }
};

void ReadDotsAndFolds(const char* input_path, DotsArray& dots, std::vector<Fold>& folds)
{
    std::vector<Point> points;

    std::ifstream input_file(input_path);
    if (input_file.is_open())
    {
        std::string line;

        // first read dots
        dots.N_x = dots.N_y = 0;
        while (std::getline(input_file, line))
        {
            if (line.empty())
                break;

            std::istringstream iss(line);
            Point p;
            char delim;
            iss >> p.x >> delim >> p.y;
            points.emplace_back(p);
            dots.N_x = std::max(dots.N_x, p.x);
            dots.N_y = std::max(dots.N_y, p.y);
        }

        // then read folds
        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            std::string dummy;
            char axis;
            char delim;
            int32_t val;
            iss >> dummy >> dummy >> axis >> delim >> val;

            folds.emplace_back(Fold::FromInput(axis, val));
        }

        input_file.close();
    }

    // max was index, convert to count
    ++dots.N_x;
    ++dots.N_y;
    dots.N_offset = dots.N_x;

    // store points in 3D array
    dots.values.resize(dots.N_x * dots.N_y, 0);
    for (const Point& p : points)
        dots.values[p.y * dots.N_x + p.x] = 1;
}

void FoldUp(DotsArray& dots, int32_t y_fold)
{
    const int32_t d = dots.N_y - y_fold;
    int32_t start = 0;
    int32_t end = y_fold + y_fold;
    if (d < y_fold)
    {
        start = y_fold - d + 1;
        end = dots.N_y - 1;
    }

    for (int32_t i = 0; i < y_fold - start; ++i)
    {
        for (int32_t j = 0; j < dots.N_x; ++j)
            dots.values[(start + i) * dots.N_offset + j] |= 
                            dots.values[(end - i) * dots.N_offset + j];
    }

    dots.N_y = y_fold;
}

void FoldLeft(DotsArray& dots, int32_t x_fold)
{
    const int32_t d = dots.N_x - x_fold;
    int32_t start = 0;
    int32_t end = x_fold + x_fold;
    if (d < x_fold)
    {
        start = x_fold - d + 1;
        end = dots.N_x - 1;
    }

    for (int32_t i = 0; i < dots.N_y; ++i)
    {
        for (int32_t j = 0; j < x_fold - start; ++j)
            dots.values[i * dots.N_offset + start + j] |=
                        dots.values[i * dots.N_offset + end - j];
    }

    dots.N_x = x_fold;
}


int32_t day13_part1(const char* input_path)
{
    DotsArray dots;
    std::vector<Fold> folds;

    ReadDotsAndFolds(input_path, dots, folds);

    const Fold& first_fold = folds.front();
    if (first_fold.type == Fold::FOLD_TYPE::Y)
        FoldUp(dots, first_fold.val);
    else
        FoldLeft(dots, first_fold.val);

    int32_t count = dots.CountNonZero();

    return count;
}

int32_t day13_part2(const char* input_path)
{
    DotsArray dots;
    std::vector<Fold> folds;

    ReadDotsAndFolds(input_path, dots, folds);

    for (const Fold& f : folds)
    {
        if (f.type == Fold::FOLD_TYPE::Y)
            FoldUp(dots, f.val);
        else
            FoldLeft(dots, f.val);
    }

    dots.Print();

    return 0;
}
