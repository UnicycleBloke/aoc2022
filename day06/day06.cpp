#include "utils.h"


template <typename T>
auto solve(const T& input, size_t len)
{
    aoc::timer timer;

    for (size_t i = len - 1; i < input.size(); ++i)
    {
        set<char> s;
        for (size_t j = 0; j < len; ++j)
            s.insert(input[i-j]);

        if (s.size() == len)
            return i + 1;
    }

    return size_t{};
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    auto p1 = solve(lines[0], 4);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 1766U);

    auto p2 = solve(lines[0], 14);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 2383U);
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
