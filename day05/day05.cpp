#include "utils.h"


template <typename T>
auto part1(vector<stack<char>> stacks, const T& moves)
{
    aoc::timer timer;

    for (auto [count, from, to]: moves)
    {
        // Switch to zero-based indices
        --from;
        --to;

        for (int m = 0; m < count; ++m)
        {
            char c = stacks[from].top();
            stacks[from].pop();
            stacks[to].push(c);
        }
    }

    // Result
    for (const auto& s: stacks)
        cout << s.top();
    cout << '\n';

    return 0;
}


template <typename T>
auto part2(vector<stack<char>> stacks, const T& moves)
{
    aoc::timer timer;

    for (auto [count, from, to]: moves)
    {
        // Switch to zero-based indices
        --from;
        --to;

        // Temporary stack to reverse-reverse order of moved crates 
        stack<char> temp;

        for (int m = 0; m < count; ++m)
        {
            char c = stacks[from].top();
            stacks[from].pop();
            temp.push(c);
        }

        while (temp.size() > 0)
        {
            char c = temp.top();
            temp.pop();
            stacks[to].push(c);
        }
    }

    // Result
    for (const auto& s: stacks)
        cout << s.top();
    cout << '\n';

    return 0;
}


void run(const char* filename)
{
    auto moves = aoc::read_lines<int, int, int>(filename, R"(move (\d+) from (\d+) to (\d+))");
    auto lines = aoc::read_lines(filename, aoc::Blanks::Allow, aoc::Trim::No);

    // I hate these fiddly inputs!
    // Create a vector of stacks 
    vector<stack<char>> temp;
    for (auto line: lines)
    {
        // Line of stack numbers/labels is the terminator
        if (line[1] == '1') break;
        cout << line << '\n';

        size_t index = 0;
        size_t pos   = 1;
        while (line.size() > pos)
        {
            if (line[pos] != ' ')
            {
                // Create a new stack if necessary
                while (temp.size() <= index) 
                    temp.push_back({});
                temp[index].push(line[pos]);
            }

            // Rely on the line formatting in the input to have crates at 
            // indices 1, 5, 9, ... 
            ++index;
            pos += 4;
        }
    } 

    // But the stacks have been read in the wrong order so need to reverse them
    vector<stack<char>> stacks(temp.size());
    for (int i = 0; i < temp.size(); ++i)
    {
        auto& t = temp[i];
        while (t.size() > 0)
        {
            char c = t.top();
            t.pop();
            stacks[i].push(c);
        }
    }

    auto p1 = part1(stacks, moves);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(stacks, moves);
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
