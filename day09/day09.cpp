#include "utils.h"


template <int KNOTS>
struct State
{
    using Pos = pair<int, int>;
    array<Pos, KNOTS> knots;
    set<Pos> visits{};

    void left(int steps)
    {
        for (auto s: aoc::range<int>(steps))
        {
            --knots[0].first;
            update();
        }
    }

    void right(int steps)
    {
        for (auto s: aoc::range<int>(steps))
        {
            ++knots[0].first;
            update();
        }
    }

    void up(int steps)
    {
        for (auto s: aoc::range<int>(steps))
        {
            --knots[0].second;
            update();
        }
    }

    void down(int steps)
    {
        for (auto s: aoc::range<int>(steps))
        {
            ++knots[0].second;
            update();
        }
    }

    void update()
    {
        for (auto k: aoc::range<int>(KNOTS-1))
        {
            update(k);
        }

        visits.insert(knots[KNOTS-1]);
    }

    void update(int k)
    {
        auto& head = knots[k];
        auto& tail = knots[k+1];

        if ((abs(head.first - tail.first) > 1) || (abs(head.second - tail.second) > 1))
        {
            tail.first += aoc::sgn(head.first - tail.first);
            tail.second += aoc::sgn(head.second - tail.second);
        }
    }

    template <typename T>
    auto simulate(const T& input)
    {
        for (auto [dir, steps]: input)
        {
            switch (dir)
            {
                case 'R': right(steps); break; 
                case 'L': left(steps); break; 
                case 'U': up(steps); break; 
                case 'D': down(steps); break; 
            }
        }

        return visits.size();
    }
};


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;
    State<2> state;
    return state.simulate(input);
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;
    State<10> state;
    return state.simulate(input);
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<char,int>(filename, R"((\w) (\d+))");

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 6026U);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 2273U);
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
