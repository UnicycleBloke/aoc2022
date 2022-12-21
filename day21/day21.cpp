#include "utils.h"


template <typename T, typename U>
auto part1(T& monkeys, U numbers)
{
    aoc::timer timer;

    while (numbers.find("root") == numbers.end())
    {
        for (const auto& [key, job]: monkeys)
        {
            const auto& [monkey, left, op, right] = job;

            if (numbers.find(monkey) != numbers.end()) continue;
            if (numbers.find(left)   == numbers.end()) continue;
            if (numbers.find(right)  == numbers.end()) continue;

            switch (op)
            {
                case '+': numbers[monkey] = numbers[left] + numbers[right]; break;
                case '-': numbers[monkey] = numbers[left] - numbers[right]; break;
                case '*': numbers[monkey] = numbers[left] * numbers[right]; break;
                case '/': numbers[monkey] = numbers[left] / numbers[right]; break;
            }
        }
    }

    return numbers["root"];
}


template <typename T>
auto part2(T& monkeys)
{
    aoc::timer timer;

    int64_t lower = 0;
    int64_t upper = 1;

    auto root = monkeys["root"];
    auto humn = monkeys["humn"];

    cout << root->add() << "\n";

    root->op  = '=';
    humn->val = lower;
    bool lower_gt = root->gt();
    bool lower_lt = root->lt();

    cout << lower_gt << " " << lower_lt << "\n";

    while (root->gt() == lower_gt) 
    {
        humn->val = upper;
        lower = upper;
        upper = upper * 2;
    }
    lower /= 2;
    upper /= 2;

    cout << lower << " " << upper << "\n";

    humn->val = lower;
    while (true) 
    {
        int64_t guess = (lower + upper) / 2;

        humn->val = guess - 1;
        cout << lower << " " << upper << " guess=" << (guess - 1) << " " << boolalpha << root->lt() << " " << root->gt() << " " << root->eq() << "\n";
        if (root->eq()) return guess - 1;

        humn->val = guess + 1;
        cout << lower << " " << upper << " guess=" << (guess + 1) << " " << boolalpha << root->lt() << " " << root->gt() << " " << root->eq() << "\n";
        if (root->eq()) return guess + 1;

        humn->val = guess;
        cout << lower << " " << upper << " guess=" << guess << " "       << boolalpha << root->lt() << " " << root->gt() << " " << root->eq() << "\n";
        if (root->eq()) return guess;

        cout << "\n";

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
    auto lines2 = aoc::read_lines<string, int>(filename, R"((\w+): (-?\d+))");

    map<string, decltype(lines)::value_type>  monkeys;
    map<string, int64_t> numbers;

    for (const auto& line: lines)
        monkeys[get<0>(line)] = line;

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

    auto p1 = part1(monkeys, numbers);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(monkeys2);
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
