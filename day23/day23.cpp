#include "utils.h"


template <typename T>
auto progress(const T& grid)
{
    size_t ncols = grid[0].size();
    size_t nrows = grid.size();

    size_t rmax = 0;
    size_t rmin = 0xFFFFFFFF;
    size_t cmax = 0;
    size_t cmin = 0xFFFFFFFF;

    for (auto r: aoc::range(nrows))
    {
        for (auto c: aoc::range(ncols))
        {
            if (grid[r][c] != '#') continue;
            rmin = min(rmin, r);
            rmax = max(rmax, r);
            cmin = min(cmin, c);
            cmax = max(cmax, c);
        }
    }

    size_t count = 0;
    for (auto r: aoc::range(rmin, rmax+1))
    {
        for (auto c: aoc::range(cmin, cmax+1))
        {
            count += (grid[r][c] == '.');
        }
    }
    return count;
}



template <typename T>
auto part1(T grid)
{
    aoc::timer timer;

    size_t ncols = grid[0].size();
    size_t nrows = grid.size();

    size_t prog  = grid.size();

    // Arrow code!!! Don't do this at home, kids.
    size_t round = 0;
    while (true)
    {
        map<pair<int, int>, int>            count;
        map<pair<int, int>, pair<int, int>> dest; 

        for (auto r: aoc::range(nrows))
        {
            for (auto c: aoc::range(ncols))
            {
                if (grid[r][c] == '.') continue;

                int clear = 0;
                clear += (grid[r-1][c-1] == '.');
                clear += (grid[r-1][c]   == '.');
                clear += (grid[r-1][c+1] == '.');

                clear += (grid[r][c-1]   == '.');
                clear += (grid[r][c+1]   == '.');

                clear += (grid[r+1][c-1] == '.');
                clear += (grid[r+1][c]   == '.');
                clear += (grid[r+1][c+1] == '.');
                if (clear == 8) continue;

                for (auto i: aoc::range(4))
                {
                    bool move = false;
                    switch ((i + round) % 4)
                    {
                        case 0: // N
                            if ((grid[r-1][c-1] == '.') && (grid[r-1][c] == '.') && (grid[r-1][c+1] == '.'))
                            {
                                dest[{r-1, c}] = {r, c};
                                auto num = count[{r-1, c}];
                                count[{r-1, c}] = num + 1;
                                move = true;
                            }
                            break;

                        case 1: // S
                            if ((grid[r+1][c-1] == '.') && (grid[r+1][c] == '.') && (grid[r+1][c+1] == '.'))
                            {
                                dest[{r+1, c}] = {r, c};
                                auto num = count[{r+1, c}];
                                count[{r+1, c}] = num + 1;
                                move = true;
                            }
                            break;

                        case 2: // W
                            if ((grid[r-1][c-1] == '.') && (grid[r][c-1] == '.') && (grid[r+1][c-1] == '.'))
                            {
                                dest[{r, c-1}] = {r, c};
                                auto num = count[{r, c-1}];
                                count[{r, c-1}] = num + 1;
                                move = true;
                            }
                            break;

                        case 3: // E
                            if ((grid[r-1][c+1] == '.') && (grid[r][c+1] == '.') && (grid[r+1][c+1] == '.'))
                            {
                                dest[{r, c+1}] = {r, c};
                                auto num = count[{r, c+1}];
                                count[{r, c+1}] = num + 1;
                                move = true;
                            }
                            break;
                    }

                    if (move) break;
                }
            }
        }

        if (round == 10) prog = progress(grid);
        if (count.size() == 0) break;

        // Update the grid.
        for (auto [to, num]: count)
        {
            if (num == 1)
            {
                auto from = dest[to];

                auto [rfrom, cfrom] = from;
                grid[rfrom][cfrom] = '.';

                auto [rto, cto] = to;
                grid[rto][cto] = '#';
            }
        }

        ++round;
    }

    return pair{prog, round+1};
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress, aoc::Trim::Yes);

    int  ncols = lines[0].size();
    int  nrows  = lines.size();

    // Plenty of room around the edges for growth.
    constexpr int EXTRA = 100;
    vector<char> row(ncols + EXTRA*2, '.');
    vector<vector<char>> grid(nrows + EXTRA*2, row);

    for (auto r: aoc::range(nrows))
    {
        const auto& line = lines[r];
        for (auto c: aoc::range(ncols))
        {
            grid[r + EXTRA][c + EXTRA] = line[c];
        }
    }

    auto [prog, round] = part1(grid);
    cout << "Part1: " << prog << '\n';
    aoc::check_result(prog, 110U);
    cout << "Part2: " << round << '\n';
    aoc::check_result(round, 1057U);
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
