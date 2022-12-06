#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int result = 0;
    for (const auto& s: input)
    {
        auto len = s.size();
        auto a   = s.substr(0, len/2);
        auto b   = s.substr(len/2, len/2);

        sort(a.begin(),a.end());
        sort(b.begin(),b.end());

        decltype(a) c;
        set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(c));

        if (c[0] >= 'a' && c[0] <= 'z') result += c[0] - 'a' + 1;
        if (c[0] >= 'A' && c[0] <= 'Z') result += c[0] - 'A' + 27;
    }

    return result;
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;

    int result = 0;
    for (auto it = input.begin(); it != input.end();)
    { 
        auto a = *it++;   
        auto b = *it++;   
        auto c = *it++;   

        sort(a.begin(),a.end());
        sort(b.begin(),b.end());
        sort(c.begin(),c.end());

        decltype(a) d;
        set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(d));

        decltype(a) e;
        set_intersection(d.begin(), d.end(), c.begin(), c.end(), back_inserter(e));

        if (e[0] >= 'a' && e[0] <= 'z') result += e[0] - 'a' + 1;
        if (e[0] >= 'A' && e[0] <= 'Z') result += e[0] - 'A' + 27;
    }

    return result;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);     

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 8105);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 2363);
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
