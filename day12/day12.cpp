#include "utils.h"


struct Point
{
    size_t row{};
    size_t col{};
};


template <typename T>
auto find_steps(const T& input, Point start, Point end)
{
    vector<size_t> row(input[0].size(), 0xFFFF'FFFF);
    vector<vector<size_t>> grid(input.size(), row);

    vector<Point> nodes;
    nodes.push_back(start);
    grid[start.row][start.col]  = 0;

    while (nodes.size() > 0)
    {
        vector<Point> nodes2;

        for (auto pos: nodes)
        {
            auto steps  = grid[pos.row][pos.col];
            auto height = input[pos.row][pos.col];  

            auto update = [&](Point pos)
            {
                if (input[pos.row][pos.col] <= (height + 1))
                {
                    bool visited = (grid[pos.row][pos.col] < 0xFFFF'FFFF);
                    grid[pos.row][pos.col] = min(grid[pos.row][pos.col], steps+1);
                    if (!visited) nodes2.push_back(pos);
                }
            };

            // Left
            if (pos.col > 0) 
                update(Point{pos.row, pos.col-1});

            // Right
            if (pos.col < (grid[0].size() - 1))
                update(Point{pos.row, pos.col+1});

            // Up
            if (pos.row > 0)
                update(Point{pos.row-1, pos.col});

            // Down 
            if (pos.row < (grid.size() - 1))
                update(Point{pos.row+1, pos.col});
        }

        nodes = nodes2;
    }

    return grid[end.row][end.col];
}


template <typename T>
auto part1(const T& input, Point start, Point end)
{
    aoc::timer timer;

    return find_steps(input, start, end);
}


template <typename T>
auto part2(const T& input, Point start, Point end)
{
    aoc::timer timer;

    size_t min_steps = 0xFFFF'FFFF;
    for (auto r: aoc::range(input.size()))
    {
        for (auto c: aoc::range(input[0].size()))
        {
            if (input[r][c] == 'a')
            {
                min_steps = min(min_steps, find_steps(input, Point{r, c}, end));           
            }
        }
    } 

    return min_steps;
}


void run(const char* filename)
{
    auto input = aoc::read_lines(filename, aoc::Blanks::Allow, aoc::Trim::No);

    Point start;
    Point end;
    for (auto r: aoc::range(input.size()))
    {
        for (auto c: aoc::range(input[0].size()))
        {
            if (input[r][c] == 'S')
            {
                start.row = r;
                start.col = c;
                input[r][c] = 'a';
            }
            if (input[r][c] == 'E')
            {
                end.row = r;
                end.col = c;
                input[r][c] = 'z';
            }
        }
    }

    auto p1 = part1(input, start, end);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 528U);

    auto p2 = part2(input, start, end);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 522U);
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
