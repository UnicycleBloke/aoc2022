#include "utils.h"


struct Screen
{
    template <typename T>
    auto run(const T& input)
    {
        for (const auto& line: input)
        {
            auto s = aoc::split(line, " ", false);
            if (s[0] == string("noop"))
            {
                ++cycles;
                update();
            }
            else
            {
                ++cycles;
                update();
                ++cycles;
                update();

                auto v = stoi(s[1]);
                reg_x += v;
            }
        }

        cout << "Part1: " << strength << '\n'; 
        cout << "Part2:\n"; 
        draw();
    }

private:
    void update()
    {
        plot();
        switch (cycles)
        {
            case 20:
            case 60:
            case 100:
            case 140:
            case 180:
            case 220:
                strength += cycles * reg_x;
                break;
        }
    }

    void plot()
    {
        if ((cycles >= 1) && (cycles <= 240))
        {            
            int y = (cycles-1) / 40;
            int x = (cycles-1) % 40;
            //grid[x][y] = (abs(reg_x - x) <= 1) ? '#' : '.';
            grid[x][y] = (abs(reg_x - x) <= 1) ? '#' : ' ';
        }
    }

    void draw()
    {
        for (auto y: aoc::range(6))
        {
            for (auto x: aoc::range(40))
            {
                cout << grid[x][y];
            }
            cout << '\n';
        }
        cout << '\n';
    }

private:
    int cycles   = 0;
    int reg_x    = 1;
    int strength = 0;

    array<array<char, 6>, 40> grid{};
};


void run(const char* filename)
{
    Screen s;
    auto input = aoc::read_lines(filename, aoc::Blanks::Allow, aoc::Trim::No);
    s.run(input);
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
