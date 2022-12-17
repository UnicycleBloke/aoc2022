#include "utils.h"


struct Rock
{
    const char* cells;
    int height;
    int width; 

    char at(int x, int y) const
    {
        return cells[y*4+x];
    }
};


// From the problem description - each rock is place on a 4x4 array and flattened to a string.
constexpr Rock rocks[5] =
{
  { "####............", 1, 4},
  { ".#..###..#......", 3, 3},
  { "..#...#.###.....", 3, 3},
  { "#...#...#...#...", 4, 1},
  { "##..##..........", 2, 2}
};


// Current state (position) of a falling rock.
struct Faller
{
    Faller(const Rock& r, int x, int y)
    : rock{r}
    , rx{x}
    , ry{y}
    {
    }

    void right()
    {
        if ((rx + rock.width) < 7) ++rx;
    }

    void left()
    {
        if (rx > 0) --rx;
    }

    void down()
    {
        --ry;
    }

    int  rx;
    int  ry;
    Rock rock;  
};


// Grid representing the cave. Just chars 7 wide and N tall.
// Index 0 represents the ground.
struct Grid
{
    Grid(string input)
    : gases{input}
    {
        string row{"......."};
        grid = vector<string>{10000, row};
        grid[0] = "=======";
    }

    void blow(Faller& faller)
    {
        switch (gases[tick])
        {
            case '>': faller.right(); break;
            case '<': faller.left(); break;
        }
        tick = (tick + 1) % gases.size();
    }        

    // Place characters for the falling rock into the grid. 
    // Settled flag is used to update the actual grid, otherwise this
    // is used for updating a temporary to aid drawing.
    void update(Faller& faller, bool settled = false)
    {
        for (int x: aoc::range(faller.rock.width))
        {
            for (int y: aoc::range(faller.rock.height))
            {
                char c = faller.rock.at(x, y);
                if (c != '.')
                    // Note the sign on y
                    grid[faller.ry - y][faller.rx + x] = settled ? '#' : '@';
            }
        }
    }

    // Checks for overlaps after a move.
    bool stuck(Faller& faller)
    {
        bool result = false;
        for (int x: aoc::range(faller.rock.width))
        {
            for (int y: aoc::range(faller.rock.height))
            {
                char c1 = faller.rock.at(x, y);
                char c2 = grid[faller.ry - y][faller.rx + x];
                if ((c1 != '.') && (c2 != '.'))
                {
                    return true;
                }
            }
        }
        return false;
    }

    void draw(Faller& faller)
    {
        Grid g{*this};
        g.update(faller);

        for (int i: aoc::range(faller.ry + 2))
            cout << g.grid[faller.ry + 1 - i] << "\n"; 
        cout << "\n";
    }

    // Called to drop a rock.    
    bool drop(int index, vector<int>& tops)
    {
        const Rock& rock = rocks[index % 5];
        int rx = 2;
        int ry = top + 3 + rock.height;

        Faller faller{rock, rx, ry};

        while (grid.size() < (ry+3))
            grid.push_back(".......");

        while (true)
        {
            // Blow sideways - use a temporary copy so we can roll back if stuck
            Faller f2{faller};
            blow(f2);
            if (!stuck(f2))
                faller = f2;

            // Drop down - use a temporary copy so we can roll back if stuck
            Faller f3{faller};
            f3.down();
            if (stuck(f3)) 
                break;
            faller = f3;
        }

        // The faller has landed.
        top = max(top, faller.ry);
        tops.push_back(top);
        update(faller, true);

        if (index >= gases.size())
        {
            ostringstream os;
            for (auto i: aoc::range(8))
                os << grid[top - i]; 

            if (index == gases.size())
            {
                pattern = os.str();
                start  = index;
            }
            else
            {
                if (pattern == os.str())
                {
                    period = index - start;
                    start  = start % period;
                    cout << "Repeated pattern at " << index <<  ": start " << start << " after " << period << "\n";
                    return true;
                }
            }
        }
        return false;
    }

    vector<string> grid;
    string gases;
    int top{};
    int tick{};

    string pattern;
    int start{};
    int period{};
    int offset{};
};


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    vector<int> tops;
    Grid grid{input};  
    for (int count: aoc::range(2022))
        grid.drop(count, tops);

    // Indexing off by one 
    return tops[2022 - 1];
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;

    vector<int> tops;
    Grid grid{input};  
    for (int count: aoc::range(200'000))
        if (grid.drop(count, tops)) break;
   
    // Indexing off by one. What would Carl say?
    constexpr int64_t SAGAN = 1'000'000'000'000 - 1; 

    // I determined from a lot of trial an error that the top line is a complete "#######" every so
    // often. This is a floor and leads to a solution with modular arithmetic. The floor first appears
    // at rock 865, and then repeats every 1705 rock. It took a lot longer than it should have done to 
    // get this expression right. I couldn't be bothered to go back and determine these numbers 
    // programmatically. The size of the input is a prime, but I don't know if that was helpful.
    // 
    // I was not able to find a repeating floor for the example, but noticed the floor goes through 
    // cycles. I didn't try to obtain the figures to reproduce the example value for Part 2. 
    //int64_t top = tops[1705] * ((SAGAN - 865) / 1705) + tops[(SAGAN - 865) % 1705 + 865];

    // Actually, I found a simple way to exploit the periodicity. This does not work for Part 1, so I 
    // have still missed something. The sample input length is not prime.
    int64_t top = tops[grid.period] * ((SAGAN - grid.start) / grid.period) + tops[(SAGAN - grid.start) % grid.period + grid.start];

    return top;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress, aoc::Trim::Yes);

    auto p1 = part1(lines[0]);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 3153);

    auto p2 = part2(lines[0]);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 1553665689155);
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
