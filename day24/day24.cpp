#include "utils.h"


using Row  = vector<int>;
using Grid = vector<vector<int>>;


// Could use a much cheaper way to work out occupancy by having four grids
// for up, down,left and right, and simply using modular arithmetic with 
// time and position - no grid duplication required - for another day. 
Grid advance(const Grid& grid)
{
    size_t ncols = grid[0].size();
    size_t nrows = grid.size();

    Row  row(ncols, 0);
    Grid grid2(nrows, row);

    for (auto r: aoc::range(nrows))
    {
        for (auto c: aoc::range(ncols))
        {
            auto b = grid[r][c];

            if (b & 0x80)
            {
                grid2[r][c] |= 0x80;
            }

            if (b & 0x01) // <
            {
                auto c2 = c;
                c2 = (c2 > 1) ? c2-1 : ncols-2;
                grid2[r][c2] |= 0x01; 
            }
                
            if (b & 0x02) // >
            {
                auto c2 = c;
                c2 = (c2 < (ncols-2)) ? c2+1 : 1;
                grid2[r][c2] |= 0x02; 
            }
                
            if (b & 0x04) // ^
            {
                auto r2 = r;
                r2 = (r2 > 1) ? r2-1 : nrows-2;
                grid2[r2][c] |= 0x04; 
            }

            if (b & 0x08) // v
            {
                auto r2 = r;
                r2 = (r2 < (nrows-2)) ? r2+1 : 1;
                grid2[r2][c] |= 0x08; 
            }
        }
    }

    return grid2;
}


// void print(Grid grid, size_t erow, size_t ecol)
// {
//     size_t ncols = grid[0].size();
//     size_t nrows = grid.size();

//     grid[erow][ecol] = 0xFF;

//     for (auto r: aoc::range(nrows))
//     {
//         for (auto c: aoc::range(ncols))
//         {
//             char x = '.';

//             auto b = grid[r][c];
//             if (b != 0)
//             {
//                 if (b == 0xFF)
//                     x = 'E';
//                 else if (b & 0x80)
//                     x = '#';
//                 else if ((b & 0x0F) == 0x01)
//                     x = '<';
//                 else if ((b & 0x0F) == 0x02)
//                     x = '>';
//                 else if ((b & 0x0F) == 0x04)
//                     x = '^';
//                 else if ((b & 0x0F) == 0x08)
//                     x = 'v';
//                 else
//                 {
//                     x = '0';
//                     x += ((b & 0x01) == 0x01);
//                     x += ((b & 0x02) == 0x02);
//                     x += ((b & 0x04) == 0x04);
//                     x += ((b & 0x08) == 0x08);
//                 }
//             }
//             cout << x;
//         }
//         cout << "\n";
//     }
//     cout << "\n";
// }


template <typename T>
auto traverse(T grid, bool go_to_end, size_t minutes)
{
    size_t ncols = grid[0].size();
    size_t nrows = grid.size();

    size_t time  = 0;

    size_t erow  = go_to_end ? 0 : nrows-1;
    size_t ecol  = go_to_end ? 1 : ncols-2;

    size_t frow  = go_to_end ? nrows-1 : 0;
    size_t fcol  = go_to_end ? ncols-2 : 1;

    // Advance the blizzards by the required time for second and third trips.
    for (auto m: aoc::range(minutes))
        grid = advance(grid);

    // The big insight to cut the search space down is that we don't care about 
    // the paths, but only the locations. There are only nrows x ncols locations.
    // If two paths place you in the same location, they are merged by using a set.
    set<pair<size_t, size_t>> pos;
    pos.insert({erow, ecol}); 
    while (pos.size() > 0)
    {
        Grid grid2 = advance(grid);

        set<pair<size_t, size_t>> pos2;
        for (auto [erow, ecol]: pos)
        {
            // You have reached your destination.
            if ((erow == frow) && (ecol == fcol)) return time;

            // Simple lambda makes the code a lot shorter below and reduces copy-paste errors.
            auto insert_if = [&](auto row, auto col) 
            {
                if (grid2[row][col] == 0) pos2.insert({row, col});
            };

            if (erow > 0)         insert_if(erow-1, ecol); // ^
            if (erow < (nrows-1)) insert_if(erow+1, ecol); // v
            if (ecol > 0)         insert_if(erow, ecol-1); // <
            if (ecol < (ncols-1)) insert_if(erow, ecol+1); // >
            insert_if(erow, ecol);
        }

        grid = grid2;
        pos  = move(pos2);

        ++time;
    }

    return size_t{};
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;
    return traverse(input, true, 0);
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;
    auto there = traverse(input, true,  0);
    auto back  = traverse(input, false, there);
    auto again = traverse(input, true,  there + back);
    cout << there << " " << back << " " << again << "\n";
    return there + back + again;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Allow, aoc::Trim::No);

    size_t ncols = lines[0].size();
    size_t nrows = lines.size();

    // Plenty of room around the edges for growth.
    Row  row(ncols, 0);
    Grid grid(nrows, row);
    for (auto r: aoc::range(nrows))
    {
        const auto& line = lines[r];
        for (auto c: aoc::range(ncols))
        {
            switch (line[c])
            {
                case '#': grid[r][c] |= 0x80; break;
                case '<': grid[r][c] |= 0x01; break;
                case '>': grid[r][c] |= 0x02; break;
                case '^': grid[r][c] |= 0x04; break;
                case 'v': grid[r][c] |= 0x08; break;
            }
        }
    }

    auto p1 = part1(grid);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 326U);

    auto p2 = part2(grid);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 976U);
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
