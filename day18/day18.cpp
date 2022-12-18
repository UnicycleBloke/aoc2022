#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer{"Part1"};

    // Keep track of the faces already seen in sets for left/right, up/down, front/back. 
    using Face = tuple<int, int, int>;
    set<Face> xfaces; 
    set<Face> yfaces;
    set<Face> zfaces;

    int area = 0;

    for (auto cube: input)
    {
        auto [x, y , z] = cube;
 
        auto xface = make_tuple(x+1, y, z);
        auto yface = make_tuple(x, y+1, z);
        auto zface = make_tuple(x, y, z+1);

        if (xfaces.find(cube)  == xfaces.end()) area += 1; else area -= 1;
        if (xfaces.find(xface) == xfaces.end()) area += 1; else area -= 1;

        if (yfaces.find(cube)  == yfaces.end()) area += 1; else area -= 1;
        if (yfaces.find(yface) == yfaces.end()) area += 1; else area -= 1;

        if (zfaces.find(cube)  == zfaces.end()) area += 1; else area -= 1;
        if (zfaces.find(zface) == zfaces.end()) area += 1; else area -= 1;

        xfaces.insert(xface);        
        yfaces.insert(yface);        
        zfaces.insert(zface);        

        xfaces.insert(cube);        
        yfaces.insert(cube);        
        zfaces.insert(cube);        
    }

    return area;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer{"Part2"};

    constexpr int SIZE = 20;

    // This represents of volume of water large enough to  hold the rock.
    // 0 means air, 100 means rock (random), 1 means water.
    vector<int> grid(SIZE*SIZE*SIZE, 0);

    // Mark cubies occupied by rock.
    for (auto [x, y, z]: input)
    {
        auto index = x + SIZE*(y + SIZE*z);    
        if ((x >= SIZE) || (y >= SIZE) || (z >= SIZE))
            cout << "SIZE too small: " << x << " " << y << " " << z << " " << index << "\n";     
        grid[index] = 100;
    }

    using Cube = tuple<int, int, int>;
    vector<Cube> next;
    next.push_back(make_tuple(0, 0, 0));

    // Flood fill to find the air gaps.
    while (next.size() > 0)
    {
        vector<Cube> next2;

        auto flood = [&](int x, int y, int z)
        {
            auto index = x + SIZE*(y + SIZE*z);
            if (grid[index] == 0)
            {
                grid[index] = 1;
                next2.push_back(make_tuple(x, y, z));
            }
        };

        for (auto [x, y, z]: next)
        {
            if (x < (SIZE-1)) flood(x+1, y, z);
            if (x > 0)        flood(x-1, y, z);

            if (y < (SIZE-1)) flood(x, y+1, z);
            if (y > 0)        flood(x, y-1, z);

            if (z < (SIZE-1)) flood(x, y, z+1);
            if (z > 0)        flood(x, y, z-1);
        }

        next = next2;
    }

    // Add any air gaps to the input as if they are rock. 
    for (auto x: aoc::range(SIZE))
    {
        for (auto y: aoc::range(SIZE))
        {
            for (auto z: aoc::range(SIZE))
            {
                auto index = x + SIZE*(y + SIZE*z);
                if (grid[index] == 0)
                    input.push_back(make_tuple(x, y, z));
            }
        }
    }

    return part1(input);
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<int,int,int>(filename, R"((\d+),(\d+),(\d+))");

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 3500);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 2048);
}


int main(int argc, char** argv)
{
    aoc::timer timer{"main()"};
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
