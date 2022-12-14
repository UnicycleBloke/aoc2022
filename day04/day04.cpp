#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int count = 0;
    for (auto [s1, e1, s2, e2] : input)
    {
        if       ((s1 >= s2) && (e1 <= e2)) count +=1;
        else if  ((s2 >= s1) && (e2 <= e1)) count +=1;
    }

    return count;
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;

    int count = 0;
    for (auto [s1, e1, s2, e2] : input)
    {
        if      ((s2 >= s1) && (s2 <= e1)) count +=1;
        else if ((s1 >= s2) && (s1 <= e2)) count +=1;
    }

    return count;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<int,int,int,int>(filename, R"((\d+)-(\d+),(\d+)-(\d+))");

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 509);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 870);
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
