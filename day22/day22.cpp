#include "utils.h"


enum Dir { Right, Down, Left, Up };
Dir left(Dir d)  { return static_cast<Dir>((static_cast<int>(d) + 3) % 4); }
Dir right(Dir d) { return static_cast<Dir>((static_cast<int>(d) + 1) % 4); }

auto part1(vector<string> grid, string moves)
{
    aoc::timer timer;

    auto height = grid.size();
    auto width  = grid[0].size();

    size_t col = 0;
    size_t row = 0;
    Dir    dir = Right;

    while (grid[row][col] == ' ') ++col;

    istringstream is{moves};
    while (is)
    {
        size_t steps;
        char   turn;
        is >> steps >> turn;

        cout << "steps=" << steps << " turn=" << turn << "\n";
        cout << "col=" << col << " row=" << row << " dir=" << static_cast<int>(dir)<< "\n";

        switch (dir)
        {
            case Right:
                for (auto s: aoc::range(steps))
                {
                    grid[row][col] = '>';

                    auto col2 = col;
                    col2 = (col2 + 1) % width;
                    while (grid[row][col2] == ' ') col2 = (col2 + 1) % width;
                    if (grid[row][col2] == '#') break;
                    col = col2;
                    // cout << "col=" << col << " row=" << row << "\n";
                }
                break;

            case Left:
                for (auto s: aoc::range(steps))
                {
                    grid[row][col] = '<';

                    auto col2 = col;
                    col2 = (col2 + width - 1) % width;
                    while (grid[row][col2] == ' ') col2 = (col2 + width - 1) % width;
                    if (grid[row][col2] == '#') break;
                    col = col2;
                    // cout << "col=" << col << " row=" << row << "\n";
                }
                break;

            case Up:
                for (auto s: aoc::range(steps))
                {
                    grid[row][col] = '^';

                    auto row2 = row;
                    row2 = (row2 + height - 1) % height;
                    while (grid[row2][col] == ' ') row2 = (row2 + height - 1) % height;
                    if (grid[row2][col] == '#') break;
                    row = row2;
                    // cout << "col=" << col << " row=" << row << "\n";
                }
                break;

            case Down:
                for (auto s: aoc::range(steps))
                {
                    grid[row][col] = 'v';

                    auto row2 = row;
                    row2 = (row2 + 1) % height;
                    while (grid[row2][col] == ' ') row2 = (row2 + 1) % height;
                    if (grid[row2][col] == '#') break;
                    row = row2;
                    // cout << "col=" << col << " row=" << row << "\n";
                }
                break;
        }  

        // grid[row][col] = '@';
        // for (const auto& s: grid)
        //     cout << ':' << s << ':' << "\n";
        // cout << "\n";

        auto pass = 1000U * (row+1) + 4U * (col+1) + static_cast<int>(dir);
        cout << (row+1) << " " <<  (col+1) << " " << static_cast<int>(dir) << " " << pass << "\n";

        dir = (turn == 'R') ? right(dir): left(dir);


    }

    return 0;
}


auto part2(const vector<string> grid, string moves)
{
    aoc::timer timer;
    return 0;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Allow, aoc::Trim::No);

    auto height = lines.size() - 2;
    auto width  = lines[0].size();
    for (const auto& s: lines)
        width = max(width, s.size());

    vector<string> grid;
    for (auto i: aoc::range(height))
    {
        auto s = lines[i];
        while (s.size() < width)
            s.push_back(' ');
        grid.push_back(s);
    }

    auto p1 = part1(grid, lines[height+1]);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(grid, lines[height+1]);
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
