#include "utils.h"


template <typename T>
auto part1(const T& input, int cycles)
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

    auto first = 0;
    for (auto c: aoc::range(cycles))
    {
        for (auto i: aoc::range(size))
        {
            int64_t steps = input[i];
            auto dir = aoc::sgn(steps);
            steps = abs(steps) % (size - 1);

            cout << "Steps " << steps << "\n";

            //A -> B -> C -> D
            //A -> C -> B -> D       

            for (auto s: aoc::range(steps))
            {
                if (dir > 0)
                {
                    // cout << "forward\n";

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
                else if (dir < 0)
                {
                    // cout << "backward\n";

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

                // auto index = first;
                // for (auto i: aoc::range(size))
                // {
                //     cout << input[index] << " ";
                //     index = next[index];
                // }
                // cout << "\n";
            }

            // auto index = first;
            // for (auto i: aoc::range(size))
            // {
            //     cout << input[index] << " ";
            //     index = next[index];
            // }
            // cout << "\n";
        }
    }

    auto index = 0;
    while (input[index] != 0)
    {
        index = next[index];
    }

    int64_t sum = 0;
    for (auto j: aoc::range(3))
    {
        for (auto i: aoc::range(1000 % size))
        {
            index = next[index];
        }
        cout << input[index] << " ";
        sum += input[index];
    }
    cout << "\n";

    return sum;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<int>(filename, R"((-?\d+))");

    vector<int64_t> input;
    for (auto [i]: lines)
        input.push_back(i);

    vector<int64_t> input2;
    for (auto [i]: lines)
        input2.push_back(int64_t{811589153} * i);

    auto p1 = part1(input, 1);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 5498);

    auto p2 = part1(input2, 10);
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
