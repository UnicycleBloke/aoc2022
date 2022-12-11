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
                {
                    worry /= 3;
                    if ((worry % m.test) == 0)
                        monkeys[m.if_true].items.push_back(worry);
                    else
                        monkeys[m.if_false].items.push_back(worry);
                }
                else
                {
                    // Test factors are all primes. Tests are all divisions.
                    // Product of factors can therefore be used to cap worry level.
                    if ((worry % m.test) == 0)
                        monkeys[m.if_true].items.push_back(worry % worry_max);
                    else
                        monkeys[m.if_false].items.push_back(worry % worry_max);
                }
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

    vector<Monkey> monkeys;
 
    int index = 0;
    while (index < lines.size())
    {
        if (lines[index].starts_with("Monkey"))
        {
            Monkey monkey; 
            ++index;

            auto items = aoc::split(lines[index], " ", true);
            for (auto i : aoc::range(2U, items.size()))
            {
                auto item = stoi(items[i]);
                monkey.items.push_back(item);
            }
            ++index;

            auto [op, val] = aoc::parse_line<char, string>(R"(Operation: new = old (.) (.*))", lines[index]);
            monkey.op = op;
            if (val.starts_with("old"))
                monkey.val = -1;
            else 
                monkey.val = stoi(val);
            ++index;    

            auto [test] = aoc::parse_line<int>(R"(Test: divisible by (\d+))", lines[index]);
            monkey.test = test;
            ++index;

            auto [if_true] = aoc::parse_line<int>(R"(If true: throw to monkey (\d+))", lines[index]);
            monkey.if_true = if_true;
            ++index;

            auto [if_false] = aoc::parse_line<int>(R"(If false: throw to monkey (\d+))", lines[index]);
            monkey.if_false = if_false;
            ++index;

            monkeys.push_back(monkey);
        }
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
