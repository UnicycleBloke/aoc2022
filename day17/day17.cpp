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


constexpr Rock rocks[5] =
{
//   { "@@@@............", 1, 4},
//   { ".@..@@@..@......", 3, 3},
//   { "..@...@.@@@.....", 3, 3},
//   { "@...@...@...@...", 4, 1},
//   { "@@..@@..........", 2, 2}
  { "####............", 1, 4},
  { ".#..###..#......", 3, 3},
  { "..#...#.###.....", 3, 3},
  { "#...#...#...#...", 4, 1},
  { "##..##..........", 2, 2}
};


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
        //cout << "right\n";
        if ((rx + rock.width) < 7) ++rx;
    }

    void left()
    {
        //cout << "left\n";
        if (rx > 0) --rx;
    }

    void down()
    {
        //cout << "down\n";
        --ry;
    }

    int rx;
    int ry;
    Rock rock;  
};


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

    void update(Faller& faller, bool settled = false)
    {
        for (int x: aoc::range(faller.rock.width))
        {
            for (int y: aoc::range(faller.rock.height))
            {
                char c = faller.rock.at(x, y);
                if (c != '.')
                    grid[faller.ry-y][faller.rx+x] = settled ? '#' : '@';
            }
        }
    }

    bool stuck(Faller& faller)
    {
        bool result = false;
        for (int x: aoc::range(faller.rock.width))
        {
            for (int y: aoc::range(faller.rock.height))
            {
                char c1 = faller.rock.at(x, y);
                char c2 = grid[faller.ry-y][faller.rx+x];
                //cout << c1 << c2 << '|';
                if ((c1 != '.') && (c2 != '.'))
                {
                    result = true;
                }
            }
        }
        //cout << "\n";
        return result;
    }

    void draw(Faller& faller)
    {
        Grid g{*this};
        g.update(faller);

        for (int i: aoc::range(faller.ry+2))
            cout << g.grid[faller.ry+1-i] << "\n"; 
        cout << "\n";
    }
    
    void drop(int index, set<string>& patterns, vector<int>& tops)
    {
        const Rock& rock = rocks[index % 5];
        int rx = 2;
        int ry = top + 3 + rock.height;

        Faller faller{rock, rx, ry};

        while (grid.size() < (ry+3))
            grid.push_back(".......");

        //cout << "start\n";
        //draw(faller);

        while (true)
        {
            Faller f2{faller};
            blow(f2);
            if (!stuck(f2))
                faller = f2;

            Faller f3{faller};
            f3.down();
            if (stuck(f3)) 
                break;
            faller = f3;
        }

        top = max(top, faller.ry);
        tops.push_back(top);
        update(faller, true);

        static int prev_index = 0;
        static int prev_top = 0;
        static int count = 0;


        if ((index > 0) and ((index % 10000) == 0))
        {
            int calc = tops[1705] * ((index - 865) / 1705) + tops[(index - 865) % 1705 + 865];
            
            cout << " dindex=" << (index-prev_index) << " dtop=" << (top-prev_top) << " index=" << index;
            cout << " mod=" << (index % gases.size());
            cout << " top=" << top << " calc=" << calc << " diff=" << (top-calc) << "\n";
            prev_index = index;
            prev_top   = top;
        }

        return;
    }

    vector<string> grid;
    string gases;
    int top{};
    int tick{};
};


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    set<string> patterns;
    vector<int> tops;

    Grid grid{input};  
    for (int count: aoc::range(50'000))
        grid.drop(count, patterns, tops);

    int64_t index = 1'000'000'000'000 - 1;
    int64_t top = tops[1705] * ((index - 865) / 1705) + tops[(index - 865) % 1705 + 865];

    for (int64_t i: aoc::range(2010, 2030))
    {
        int64_t calc = tops[1705] * ((i - 865) / 1705) + tops[(i - 865) % 1705 + 865];
        cout << i << " " << calc << "\n";
    }

    return top;
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;
    return 0;
}


// 1'000'000'000'000

void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress, aoc::Trim::Yes);

    auto p1 = part1(lines[0]);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(lines[0]);
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
