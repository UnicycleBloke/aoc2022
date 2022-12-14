#include "utils.h"


template <typename T>
auto add_sand(T& grid)
{
    int x = 500;
    int y = 0;

    if (grid[y][x] != '.') return false;

    while ( (grid[y+1][x]   == '.') ||
            (grid[y+1][x-1] == '.') ||
            (grid[y+1][x+1] == '.') )
    {
        if (grid[y+1][x] == '.')
        {
            ++y;
        }
        else if (grid[y+1][x-1] == '.')
        {
            ++y;
            --x;
        } 
        else if (grid[y+1][x+1] == '.')
        {
            ++y;
            ++x;
        }        

        if (y >= (grid.size()-1)) return false;
    }

    grid[y][x] = 'o';         
    return true;
}


template <typename T>
auto part1(T grid)
{
    aoc::timer timer;

    int grains = 0;
    while (add_sand(grid))
    {
        ++grains;
    }

    return grains;
}


template <typename T>
auto part2(T grid)
{
    aoc::timer timer;

    // Add the solid floor
    grid.push_back(vector<char>(grid[0].size(), '='));

    int grains = 0;
    while (add_sand(grid))
    {
        ++grains;
    }

    return grains;
}


void run(const char* filename)
{
    using Pos   = pair<int,int>;
    using Line  = vector<Pos>;
    using Lines = vector<Line>;

    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress, aoc::Trim::Yes);

    int max_x{};
    int max_y{};
    for (const auto& line : lines)
    {
        auto s = aoc::split(line, "->", false);
        for (const auto& p: s)
        {
            auto [x, y] = aoc::parse_line<int, int>(R"((\d+),(\d+))", p);
            max_x = max(max_x, x);
            max_y = max(max_y, y);
        }
    }

    // Ensure wide enough for pyramid of sand.
    max_x += max_y + 10;

    vector<vector<char>> grid(max_y+2, vector<char>(max_x+1, '.'));

    for (const auto& line : lines)
    {
        auto s = aoc::split(line, "->", false);

        auto [px, py] = aoc::parse_line<int, int>(R"((\d+),(\d+))", s[0]);
        grid[py][px] = '#';

        for (auto i: aoc::range(1U, s.size()))
        {
            auto [x, y] = aoc::parse_line<int, int>(R"((\d+),(\d+))", s[i]);

            for (auto i: aoc::range(px, x, aoc::sgn(x - px)))
                grid[py][i] = '#';
            for (auto i: aoc::range(py, y, aoc::sgn(y - py)))
                grid[i][px] = '#';

            px = x;
            py = y;
            grid[py][px] = '#';
        }
    }

    auto p1 = part1(grid);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 843);

    auto p2 = part2(grid);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 27625);
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
