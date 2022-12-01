#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int res = 0;
    int sum = 0;
    for (const auto& line: input)
    {
        if (line.size() == 0)
        {
            res = max(res, sum);
            sum = 0; 
        }
        else
        {
            sum += stoi(line);
        }
    }

    return res;
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;

    vector<int> res;
    int sum = 0;
    for (const auto& line: input)
    {
        if (line.size() == 0)
        {
            res.push_back(sum);
            sum = 0; 
        }
        else
        {
            sum += std::stoi(line);
        }
    }
    res.push_back(sum);

    sort(res.begin(), res.end(), greater<int>());
    return res[0] + res[1] + res[2];
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, true);     

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 75501);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 215594);
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
