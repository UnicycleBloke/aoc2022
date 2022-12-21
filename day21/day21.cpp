#include "utils.h"


template <typename T>
auto part1(T& monkeys)
{
    aoc::timer timer;

    return monkeys["root"]->add();
}


template <typename T>
auto part2(T& monkeys)
{
    aoc::timer timer;

    auto root = monkeys["root"];
    auto humn = monkeys["humn"];

    int64_t lower = 0;
    int64_t upper = 1;

    humn->val = lower;
    bool lower_gt = root->gt();

    while (root->gt() == lower_gt) 
    {
        humn->val = upper;
        lower = upper;
        upper = upper * 2;
    }
    lower /= 2;
    upper /= 2;

    humn->val = lower;
    while (true) 
    {
        int64_t guess = (lower + upper) / 2;

        humn->val = guess - 1;
        if (root->eq()) return guess - 1;

        humn->val = guess + 1;
        if (root->eq()) return guess + 1;

        humn->val = guess;
        if (root->eq()) return guess;

        if (root->gt() == lower_gt)
            lower = guess;
        else
            upper = guess;
    }

    return int64_t{};
}


struct Monkey
{
    Monkey* left{};
    Monkey* right{};
    char    op{};
    int64_t val{};
 
    bool lt()
    {
        return left->value() < right->value();
    }

    bool gt()
    {
        return left->value() > right->value();
    }

    bool eq()
    {
        return left->value() == right->value();
    }

    int64_t add()
    {
        return left->value() + right->value();
    }

    int64_t value() const 
    {
        if (left && right)
        {
            switch (op)
            {
                case '+': return left->value() + right->value();
                case '-': return left->value() - right->value();
                case '*': return left->value() * right->value();
                case '/': return left->value() / right->value();
                case '=': return left->value() == right->value();
            }
        }
        return val;
    }     
};


void run(const char* filename)
{
    auto lines  = aoc::read_lines<string, string, char, string>(filename, R"((\w+): (\w+) (\S) (\w+))");
    map<string, decltype(lines)::value_type>  monkeys;
    for (const auto& line: lines)
        monkeys[get<0>(line)] = line;

    auto lines2 = aoc::read_lines<string, int>(filename, R"((\w+): (-?\d+))");
    map<string, int64_t> numbers;
    for (const auto& line: lines2)
        numbers[get<0>(line)] = get<1>(line);

    map<string, Monkey*> monkeys2;
    for (const auto& [monkey, val]: numbers)
        monkeys2[monkey] = new Monkey{nullptr, nullptr, 0, val};
    for (const auto& [monkey, job]: monkeys)
        monkeys2[monkey] = new Monkey{nullptr, nullptr, 0, 0};

    for (const auto& [monkey, job]: monkeys)
    {
        const auto& [m, left, opx, right] = job;
        auto op = (monkey == string{"root"}) ? '=' : opx;

        monkeys2[monkey]->left  = monkeys2[left];
        monkeys2[monkey]->right = monkeys2[right];
        monkeys2[monkey]->op    = op;
    }

    auto p1 = part1(monkeys2);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 62386792426088);

    auto p2 = part2(monkeys2);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 3876027196185);
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
