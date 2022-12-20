#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    auto size = input.size();

    vector<int> next(size);
    vector<int> prev(size);

    for (auto i: aoc::range(size))
    {
        next[i] = (i + 1) % size;
        prev[i] = (i + size - 1) % size;
    }

    for (auto i: aoc::range(size))
        cout << input[next[(i + size - 1) % size]] << " ";
    cout << "\n";


    auto first = 0;
    for (auto i: aoc::range(size))
    {
        auto steps = input[i];
        cout << "Steps " << steps << "\n";

        //A -> B -> C -> D
        //A -> C -> B -> D       

        for (auto s: aoc::range(abs(steps)))
        {
            if (steps > 0)
            {
                cout << "forward\n";

                auto a = prev[i];
                auto b = i;
                auto c = next[i];
                auto d = next[next[i]];

                next[a] = c;
                next[c] = b;
                next[b] = d;

                prev[d] = b;
                prev[b] = c;
                prev[c] = a;

                if (b == first) first = c;
            }
            else if (steps < 0)
            {
                cout << "backward\n";

                auto a = prev[prev[i]];
                auto b = prev[i];
                auto c = i;
                auto d = next[i];

                next[a] = c;
                next[c] = b;
                next[b] = d;

                prev[d] = b;
                prev[b] = c;
                prev[c] = a;

                if (c == first) first = b;
            }    

            auto index = prev[0];
            for (auto i: aoc::range(size))
            {
                cout << input[index] << " ";
                index = next[index];
            }

            cout << "\n";
        }
    }


    return 0;
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;
    return 0;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<int>(filename, R"((-?\d+))");

    vector<int> input;
    for (auto [i]: lines)
    {
        input.push_back(i);
        //cout << i << "\n";
    } 

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    //aoc::check_result(p2, 0);
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
