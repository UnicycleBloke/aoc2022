#include "utils.h"


struct Monkey
{
    // That was nasty - numbers got very big but not in the example.
    vector<int64_t> items;
    char op;
    int val;
    int test;
    int if_true;
    int if_false;

    int inspections{};
};


struct Game
{
    enum class Part{One, Two};

    Game(vector<Monkey> m) : monkeys{m} 
    {
        // Test factors are all primes. Tests are all divisions.
        // Product of factors can therefore be used to cap worry level.
        worry_max = 1;
        for (auto& m: monkeys)
            worry_max *= m.test;
    }

    void round(Part part)
    {
        for (auto& m: monkeys)
        {
            for (auto item: m.items)
            {
                ++m.inspections; 

                auto worry = item;
                auto value = (m.val == -1) ? worry : m.val;
                if (m.op == '+') worry += value;
                if (m.op == '*') worry *= value;

                if (part == Part::One)
                    worry /= 3;
                else
                    worry %= worry_max;

                if ((worry % m.test) == 0)
                    monkeys[m.if_true].items.push_back(worry);
                else
                    monkeys[m.if_false].items.push_back(worry);
            }

            m.items.clear();
        }
    }

    int64_t play(Part part, int rounds)
    {
        for (auto i: aoc::range(rounds))
            round(part);
        return result();
    }

    int64_t result()
    {
        vector<int64_t> i;
        for (auto& m: monkeys) 
        {
            i.push_back(m.inspections);
        }
        sort(i.begin(), i.end(), greater<>());
        return i[0] * i[1];
    }

    vector<Monkey> monkeys;
    int64_t        worry_max;
};


template <typename T>
auto part1(const T& monkeys)
{
    aoc::timer timer;
    Game game{monkeys};
    return game.play(Game::Part::One, 20);
}


template <typename T>
auto part2(const T& monkeys)
{
    aoc::timer timer;
    Game game{monkeys};
    return game.play(Game::Part::Two, 10'000);
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress, aoc::Trim::Yes);

    auto items      = aoc::read_lines<string>(filename, R"(Starting items: (.*))", "@");
    auto operation  = aoc::read_lines<char, string>(filename, R"(Operation: new = old (.) (.*))");
    auto test       = aoc::read_lines<int>(filename, R"(Test: divisible by (\d+))");
    auto if_true    = aoc::read_lines<int>(filename, R"(If true: throw to monkey (\d+))");
    auto if_false   = aoc::read_lines<int>(filename, R"(If false: throw to monkey (\d+))");

    vector<Monkey> monkeys;
    for (auto i: aoc::range(items.size()))
    {
        Monkey monkey; 

        auto list = aoc::split(get<0>(items[i]), ",@");
        for (const auto& item : list)
            monkey.items.push_back(stoi(item));

        auto [op, val] = operation[i];
        monkey.op = op;
        if (val.starts_with("old"))
            monkey.val = -1;
        else 
            monkey.val = stoi(val);

        monkey.test     = get<0>(test[i]);
        monkey.if_true  = get<0>(if_true[i]);
        monkey.if_false = get<0>(if_false[i]);

        monkeys.push_back(monkey);
    }

    auto p1 = part1(monkeys);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 112221);

    auto p2 = part2(monkeys);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 25272176808);
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
