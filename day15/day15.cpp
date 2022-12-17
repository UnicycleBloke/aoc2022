#include "utils.h"


int distance(int sx, int sy, int bx, int by)
{
    return abs(bx - sx) + abs(by - sy);
}


constexpr int ROW   = 2'000'000;
constexpr int RANGE = 4'000'000;

// constexpr int ROW   = 10;
// constexpr int RANGE = 20;


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    set<int> visible;
    set<int> occupied;

    for (auto [sx, sy, bx, by]: input)
    {
        int d = distance(sx, sy, bx, by);
        for (int x: aoc::range(sx - d, sx + d))
        {
            int d2 = distance(sx, sy, x, ROW);
            if (d2 <= d)
                visible.insert(x);
        }

        if (sy == ROW) occupied.insert(sx);
        if (by == ROW) occupied.insert(bx);
    }

    return visible.size() - occupied.size();
}


// template <typename T>
// auto part2a(const T& input)
// {
//     aoc::timer timer;

//     for (int y: aoc::range(0, RANGE+1))
//     {
//         map<pair<int,int>, int> m;

//         for (auto [sx, sy, bx, by]: input)
//         {
//             int d = distance(sx, sy, bx, by);
//             for (int i: aoc::range(d))
//             {
//                 pair<int,int> l{sx-d+i,sy+i};
//                 int ml = m[l];
//                 m[l] = ml + 1;

//                 pair<int,int> r{sx+d-1,sy-1};
//                 int mr = m[r];
//                 m[r] = mr + 1;

//                 pair<int,int> u{sx,sy-d};
//                 int mu = m[u];
//                 m[u] = mu + 1;

//                 auto pair<int,int> d{sx,sy+d};
//                 int md = m[d];
//                 m[d] = md + 1;
//             }
//         }
//     }

//     return int64_t{};
// }




template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;

    for (int y: aoc::range(0, RANGE+1))
    {
        //cout << y << "\n";

        vector<pair<int,int>> r;

        for (auto [sx, sy, bx, by]: input)
        {
            int d   = distance(sx, sy, bx, by);
            int dy  = abs(sy-y);
            if (dy <= d)
            {
                int off = abs(d-dy);
                r.push_back({max(0, sx-off), min(RANGE, sx+off)});
            }
        }

        // Combine overlapping ranges to find missing spot. 
        // Not working.

        sort(r.begin(), r.end());
        int xmin = r[0].first;
        int xmax = r[0].second;
        for (auto [rmin, rmax]: r)
        {
            // cout << "(" << xmin << "," << xmax << ") + ";
            // cout << "(" << rmin << "," << rmax << ") = ";

            if (rmin > (xmax +1))
                return int64_t{4'000'000} * (xmax+1) + y;
            else
                xmax = max(xmax, rmax);

            // cout << "(" << xmin << "," << xmax << ")\n";
            if (rmax >= RANGE) break;
        }
    }

    return int64_t{};
}


void run(const char* filename)
{    
    auto lines = aoc::read_lines<int,int,int,int>(filename, R"(Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+))");
    
    // auto p1 = part1(lines);
    // cout << "Part1: " << p1 << '\n';
    // //aoc::check_result(p1, 0);

    auto p2 = part2(lines);
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
