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

        // cout << "steps=" << steps << " turn=" << turn << "\n";
        // cout << "col=" << col << " row=" << row << " dir=" << static_cast<int>(dir)<< "\n";

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

        // auto pass = 1000U * (row+1) + 4U * (col+1) + static_cast<int>(dir);
        // cout << (row+1) << " " <<  (col+1) << " " << static_cast<int>(dir) << " " << pass << "\n";

        dir = (turn == 'R') ? right(dir): left(dir);


    }

    return 0;
}


auto change_left(size_t row, size_t col)
{
    if (row < 50)  return make_tuple<size_t, size_t, Dir>(149 - row%50,   0,           Right);
    if (row < 100) return make_tuple<size_t, size_t, Dir>(100,                row%50,  Down);
    if (row < 150) return make_tuple<size_t, size_t, Dir>( 49 - row%50,  50,           Right);
    if (row < 200) return make_tuple<size_t, size_t, Dir>(  0,           50 + row%50,  Down);
    return tuple<size_t, size_t, Dir>{};
}

auto change_right(size_t row, size_t col)
{
    if (row < 50)  return make_tuple<size_t, size_t, Dir>(149 - row%50,  99,           Left);
    if (row < 100) return make_tuple<size_t, size_t, Dir>( 49,          100 + row%50,  Up);
    if (row < 150) return make_tuple<size_t, size_t, Dir>( 49 - row%50, 149,           Left);
    if (row < 200) return make_tuple<size_t, size_t, Dir>(149,           50 + row%50,  Up);
    return tuple<size_t, size_t, Dir>{};
}

auto change_up(size_t row, size_t col)
{
    if (col < 50)  return make_tuple<size_t, size_t, Dir>( 99 - col%50,  50,           Right);
    if (col < 100) return make_tuple<size_t, size_t, Dir>(150 + col%50,   0,           Right);
    if (col < 150) return make_tuple<size_t, size_t, Dir>(149,                col%50,  Up);
    return tuple<size_t, size_t, Dir>{};
}

auto change_down(size_t row, size_t col)
{
    if (col < 50)  return make_tuple<size_t, size_t, Dir>(  0,          100 + col%50,  Down);
    if (col < 100) return make_tuple<size_t, size_t, Dir>(150 + col%50,  49,           Left);
    if (col < 150) return make_tuple<size_t, size_t, Dir>( 50 + col%50,  99,           Left);
    return tuple<size_t, size_t, Dir>{};
}

auto change_face(size_t row, size_t col, Dir dir)
{
    switch (dir)
    {
        case Left:  return change_left(row, col);
        case Right: return change_right(row, col);
        case Up:    return change_up(row, col);
        case Down:  return change_down(row, col);
    }
    return tuple<size_t, size_t, Dir>{};
}


auto part2(vector<string> grid, string moves)
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

        for (auto i: aoc::range(steps))
        {
            auto row2 = row;
            auto col2 = col;
            auto dir2 = dir;

            switch (dir)
            {
                case Right: 
                    grid[row][col] = '>';
                    col2 = (col2 + 1) % width;           
                    break;
                case Left:  
                    grid[row][col] = '<';
                    col2 = (col2 + width - 1) % width;   
                    break;
                case Down:  
                    grid[row][col] = 'v';
                    row2 = (row2 + 1) % height;          
                    break;
                case Up:    
                    grid[row][col] = '^';
                    row2 = (row2 + height - 1) % height; 
                    break;
            }  

            if (grid[row2][col2] == ' ')
            {
                auto [r, c, d] = change_face(row, col, dir);
                row2 = r;
                col2 = c;
                dir2 = d;
            }

            if (grid[row2][col2] == '#') break;
            row = row2;
            col = col2;
            dir = dir2;
        }

        auto pass = 1000U * (row+1) + 4U * (col+1) + static_cast<int>(dir);
        cout << (row+1) << " " <<  (col+1) << " " << static_cast<int>(dir) << " " << pass << "\n";

        dir = (turn == 'R') ? right(dir): left(dir);

        constexpr const char* DIR = "RDLU";
        // grid[row][col] = DIR[static_cast<int>(dir)];
        // cout << "\n";
        // for (const auto& s: grid)
        //     cout << s << "\n";
        // cout << "\n";

    }

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
