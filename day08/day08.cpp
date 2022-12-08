#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int size = input[0].size();

    auto visible = 0;
    for (auto i: aoc::range(1, size-1, 1))
    {
        for (auto j: aoc::range(1, size-1, 1))
        {
            bool a = true;
            for (auto k: aoc::range(0, j, 1))
                a = a && (input[i][k] < input[i][j]);

            bool b = true;
            for (auto k: aoc::range(0, i, 1))
                b = b && (input[k][j] < input[i][j]);

            bool c = true;
            for (auto k: aoc::range(size-1, j, -1))
                c = c && (input[i][k] < input[i][j]);

            bool d = true;
            for (auto k: aoc::range(size-1, i, -1))
                d = d && (input[k][j] < input[i][j]);
            
            visible += (a || b || c || d);
        }
    }

    return visible + 4 * (size - 1);
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;

    int size = input[0].size();

    auto max_score = 0;
    for (auto i: aoc::range(1, size - 1, 1))
    {
        for (auto j: aoc::range(1, size - 1, 1))
        {
            int height = input[i][j];

            int a = 0;
            int k = i - 1;
            while (k >= 0)
            {
                ++a;
                if (height <= input[k][j]) break;
                --k;
            }

            int b = 0;
            k = j - 1;
            while (k >= 0)
            {
                ++b;
                if (height <= input[i][k]) break;
                --k;
            }

            int c = 0;
            k = i + 1;
            while (k < size)
            {
                ++c;
                if (height <= input[k][j]) break;
                ++k;
            }

            int d = 0;
            k = j + 1;
            while (k < size)
            {
                ++d; 
                if (height <= input[i][k]) break;
                ++k;
            }

            auto score = a * b * c * d;
            max_score = max(score, max_score);
        }
    }

    return max_score;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Allow, aoc::Trim::No);

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 1776);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 234416);
}


int main(int argc, char** argv)
{
    aoc::timer timer;
    try
    {
        if (argc < 2)
        {
            cout << "Provide input file name\n";
            return -1;
        }

        fs::path path{argv[1]};
        if (!fs::is_regular_file(path))
        {
            cout << "Path '" << argv[1] << "' does not exist or is not a file\n";
            return -1;
        }

        run(argv[1]);
    }
    catch (std::exception& e)
    {
        cout << e.what();
    }
}
