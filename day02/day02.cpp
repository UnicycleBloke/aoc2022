#include "utils.h"


static constexpr const char* tools = "ABC";


// Return the score a given tool: A => 1, ...
int score(char t)
{
    return t - 'A' + 1;
}


// Return the tool for a given score: 1 => A, ...
char tool(int s)
{    
    return tools[(s + 2) % 3];
}


// Determine the score for a single round given the 
// scores for the two tools
int game(int s1, int s2)
{
    int score = s2;

    score += 3; // draw
    if (s1 != s2)
    {
        score += 3; // win
        if (s1 == 1 && s2 == 3) score -= 6; // lose
        if (s1 == 2 && s2 == 1) score -= 6;
        if (s1 == 3 && s2 == 2) score -= 6;
    }

    return score;
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int total = 0;
    for (const auto [c1, c2]: input)
    {
        // Convert XYZ to ABC to reduce confusion
        // Game is played using tool scores rather than letters
        auto s1 = score(c1);
        auto s2 = score(tools[c2-'X']);
        total += game(s1, s2); 
    } 

    return total;
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;

    int total = 0;
    for (const auto [c1, c2]: input)
    {
        auto s1 = score(c1);
        auto s2 = s1;
        // Convert XYZ by using prev, same or next tool as player 1
        switch (c2)
        {
            case 'X': s2 = score(tool(s1-1)); break; // lose
            case 'Y': s2 = score(tool(s1));   break; // draw
            case 'Z': s2 = score(tool(s1+1)); break; // win
        }
        total += game(s1, s2); 
    } 

    return total;

    return 0;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<char, char>(filename, R"((\w) (\w))");

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 13'005);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 11'373);
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
